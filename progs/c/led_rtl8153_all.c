/*
gcc -Wall -Wextra led_rtl8153_all.c -lusb-1.0 -o led_rtl8153_all.out
sudo ./led_rtl8153_all.out show
sudo ./led_rtl8153_all.out set
*/

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RTL8152_REQ_REGS 0x05
#define MCU_TYPE_PLA 0x0100
#define BYTE_EN_DWORD 0xff
#define PLA_LED_SELECT 0xdd90

#define TARGET_VENDOR_ID 0x0bda
#define TARGET_PRODUCT_ID 0x8153

typedef struct {
    libusb_device_handle* handle;
    uint8_t bus_num;
    uint8_t dev_num;
} DeviceInfo;

int find_and_open_devices(libusb_context* ctx, DeviceInfo** devices, size_t* count) {
    libusb_device** list;
    ssize_t cnt = libusb_get_device_list(ctx, &list);
    if (cnt < 0) {
        fprintf(stderr, "Error getting device list: %s\n", libusb_error_name(cnt));
        return -1;
    }

    *count = 0;
    *devices = malloc(sizeof(DeviceInfo) * cnt);
    if (!*devices) {
        libusb_free_device_list(list, 1);
        return -1;
    }

    for (ssize_t i = 0; i < cnt; ++i) {
        libusb_device* device = list[i];
        struct libusb_device_descriptor desc;
        if (libusb_get_device_descriptor(device, &desc) == 0 &&
            desc.idVendor == TARGET_VENDOR_ID && desc.idProduct == TARGET_PRODUCT_ID) {
            libusb_device_handle* handle;
            int res = libusb_open(device, &handle);
            if (res == 0) {
                (*devices)[*count].handle = handle;
                (*devices)[*count].bus_num = libusb_get_bus_number(device);
                (*devices)[*count].dev_num = libusb_get_device_address(device);
                (*count)++;
            } else {
                fprintf(stderr, "Failed to open device %d:%d: %s\n",
                        libusb_get_bus_number(device), libusb_get_device_address(device),
                        libusb_error_name(res));
            }
        }
    }

    libusb_free_device_list(list, 1);
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <set|show>\n", argv[0]);
        printf("  set: Write 0x000e0f00 to register 0xdd90 for all RTL8153 devices\n");
        printf("  show: Read and display bus, dev, and value from register 0xdd90 for all RTL8153 devices\n");
        return 1;
    }

    const char* operation = argv[1];
    if (strcmp(operation, "set") != 0 && strcmp(operation, "show") != 0) {
        fprintf(stderr, "Operation must be 'set' or 'show'\n");
        return 1;
    }

    libusb_context* ctx = NULL;
    int res = libusb_init(&ctx);
    if (res != 0) {
        fprintf(stderr, "libusb_init failed: %s\n", libusb_error_name(res));
        return 1;
    }

    DeviceInfo* devices = NULL;
    size_t device_count = 0;
    if (find_and_open_devices(ctx, &devices, &device_count) != 0) {
        libusb_exit(ctx);
        return 1;
    }

    if (device_count == 0) {
        printf("No RTL8153 devices found.\n");
        libusb_exit(ctx);
        free(devices);
        return 0;
    }

    uint16_t offset = PLA_LED_SELECT;  // 0xdd90
    uint16_t wIndex = MCU_TYPE_PLA | BYTE_EN_DWORD;  // 0x0100 | 0xff
    uint32_t value = 0x000e0f00;  // value to set

    for (size_t i = 0; i < device_count; ++i) {
        libusb_device_handle* handle = devices[i].handle;
        uint8_t bus_num = devices[i].bus_num;
        uint8_t dev_num = devices[i].dev_num;

        if (strcmp(operation, "set") == 0) {
            res = libusb_control_transfer(
                handle,
                0x40,
                RTL8152_REQ_REGS,
                offset,
                wIndex,
                (unsigned char*)&value,
                4,
                5000
            );
            if (res < 0) {
                fprintf(stderr, "Write failed for device %d:%d: %s\n", bus_num, dev_num, libusb_error_name(res));
            } else {
                printf("Wrote 0x%08x to register 0x%04x on device %d:%d\n", value, offset, bus_num, dev_num);
            }
        } else { // show
            uint32_t read_val = 0;
            res = libusb_control_transfer(
                handle,
                0xc0,
                RTL8152_REQ_REGS,
                offset,
                wIndex,
                (unsigned char*)&read_val,
                4,
                5000
            );
            if (res < 0) {
                fprintf(stderr, "Read failed for device %d:%d: %s\n", bus_num, dev_num, libusb_error_name(res));
            } else {
                printf("Device %d:%d register 0x%04x = 0x%08x\n", bus_num, dev_num, offset, read_val);
            }
        }

        libusb_close(handle);
    }

    free(devices);
    libusb_exit(ctx);
    return 0;
}

