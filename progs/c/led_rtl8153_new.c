/*
bus_number and device_number (from lsusb -t) version

compilation
gcc -Wall -Wextra led_rtl8153_new.c -lusb-1.0 -o led_rtl8153_new
if cannot compile: sudo apt install libusb-1.0-0-dev

usage
help: ./led_rtl8153_new
read: sudo ./led_rtl8153_new 6 3
write: sudo ./led_rtl8153_new 6 3 0x000e0f00
if cannot run: sudo apt install libusb-1.0-0

add to udev rules for autostart
*/

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// unsigned int timeout for libusb_control_transfer() (https://libusb.sourceforge.io/api-1.0/group__libusb__syncio.html#gadb11f7a761bd12fc77a07f4568d56f38)
// It is timeout (in milliseconds) that this function should wait before giving up due to no response being received. For an unlimited timeout, use value 0.
#define TIMEOUT_MS 5000

// constants used in libusb_control_transfer() for rtl8153 usb-eth controller
#define RTL8152_REQ_REGS 0x05
#define MCU_TYPE_PLA 0x0100
#define BYTE_EN_DWORD 0xff
#define PLA_LED_SELECT 0xdd90
#define VENDOR_OUT 0x40
#define VENDOR_IN 0xc0

libusb_device_handle* open_device_by_bus_dev(libusb_context* ctx, uint8_t bus_num, uint8_t dev_num);

int main(int argc, char* argv[]) {
    if (argc < 3 || argc > 4) {
        printf("Usage (run as root or use sudo): %s <bus_num> <dev_num> [value]\n", argv[0]);
        printf("  bus_num: USB bus number from lsusb -t (decimal), e.g. 6\n");
        printf("  dev_num: USB device number from lsusb -t (decimal), e.g. 3\n");
        printf("  value: 32-bit value in register 0x%x is led settings\n", PLA_LED_SELECT);
        printf("    if NOT specified: read current value from register of selected USB device\n");
        printf("    if specified: write value in hex (e.g. 0x000e0f00) to register of selected USB device\n");
        return 1;
    }

    char *endptr;
    unsigned long input;

    errno = 0;
    input = strtoul(argv[1], &endptr, 10);
    if (errno == ERANGE || input > UINT8_MAX || *endptr != '\0') {
        fprintf(stderr, "Error: <bus_num> must be decimal and fit uint8_t\n");
        return 1;
    }
    uint8_t bus_num = (uint8_t)input;

    errno = 0;
    input = strtoul(argv[2], &endptr, 10);
    if (errno == ERANGE || input > UINT8_MAX || *endptr != '\0') {
        fprintf(stderr, "Error: <dev_num> must be decimal and fit uint8_t\n");
        return 1;
    }
    uint8_t dev_num = (uint8_t)input;

    uint32_t value = 0;
    if (argc == 4) {
        errno = 0;
        input = strtoul(argv[3], &endptr, 16);
        if (errno == ERANGE || input > UINT32_MAX || *endptr != '\0') {
            fprintf(stderr, "Error: [value] must be hex and fit uint32_t, e.g. 0x12345678\n");
            return 1;
        }
        value = (uint32_t)input;
    }

    int res;
    libusb_context* ctx = NULL;
    res = libusb_init(&ctx);
    if (res != 0) {
        fprintf(stderr, "libusb_init failed: %s\n", libusb_error_name(res));
        return 1;
    }

    libusb_device_handle* handle = open_device_by_bus_dev(ctx, bus_num, dev_num);
    if (!handle) {
        fprintf(stderr, "Failed to open device %hhu:%hhu\n", bus_num, dev_num);
        libusb_exit(ctx);
        return 1;
    }

    uint16_t offset = PLA_LED_SELECT;
    uint16_t wIndex = MCU_TYPE_PLA | BYTE_EN_DWORD;

    if (argc == 4) { // write
        res = libusb_control_transfer(
            handle,
            VENDOR_OUT,
            RTL8152_REQ_REGS,
            offset,
            wIndex,
            (unsigned char*)&value,
            sizeof value,
            TIMEOUT_MS
        );
        if (res < 0) {
            fprintf(stderr, "Write failed: %s\n", libusb_error_name(res));
        } else {
            printf("Successfully wrote 0x%08x (actually %d bytes) to register 0x%04x\n", value, res, offset);
        }
    } else if (argc == 3) { // read
        res = libusb_control_transfer(
            handle,
            VENDOR_IN,
            RTL8152_REQ_REGS,
            offset,
            wIndex,
            (unsigned char*)&value,
            sizeof value,
            TIMEOUT_MS
        );
        if (res < 0) {
            fprintf(stderr, "Read failed: %s\n", libusb_error_name(res));
        } else {
            printf("Read 0x%08x (actually %d bytes) from register 0x%04x\n", value, res, offset);
        }
    } else {
        fprintf(stderr, "Bug in source code!\n");
        libusb_close(handle);
        libusb_exit(ctx);
        return 1;
    }

    libusb_close(handle);
    libusb_exit(ctx);
    return res < 0 ? 1 : 0;
}

libusb_device_handle* open_device_by_bus_dev(libusb_context* ctx, uint8_t bus_num, uint8_t dev_num) {
    libusb_device** list;
    ssize_t cnt = libusb_get_device_list(ctx, &list);
    if (cnt < 0) {
        fprintf(stderr, "Error getting device list: %s\n", libusb_error_name(cnt));
        return NULL;
    }

    libusb_device_handle* handle = NULL;
    libusb_device* device;
    ssize_t i;
    for (i = 0; i < cnt; i++) {
        device = list[i];
        if (libusb_get_bus_number(device) == bus_num && libusb_get_device_address(device) == dev_num) {
            int res = libusb_open(device, &handle);
            if (res != 0) {
                fprintf(stderr, "Error opening device: %s\n", libusb_error_name(res));
            }
            break;
        }
    }
    if (i == cnt) {
        fprintf(stderr, "Unable to find device %hhu:%hhu\n", bus_num, dev_num);
    }

    libusb_free_device_list(list, 1);
    return handle;
}

