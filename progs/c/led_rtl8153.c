/*
Inspired by https://github.com/EHfive/rtl8152-led-ctrl

Compilation (sudo apt install libusb-1.0-0-dev):
gcc -Wall -Wextra led_rtl8153.c -lusb-1.0 -o led_rtl8153.out

Usage example (sudo apt install libusb-1.0-0):
Firstly, check bus and dev numbers from lsusb -t, then for example Bus 2 Dev 21
sudo ./led_rtl8153.out 2 21 read
sudo ./led_rtl8153.out 2 21 write 0x000e0f00

You can understand what hex value you need to write for you custom leds settings from led_rtl8153.ods table (or read datasheet).
Also, check Rust repository (link is above) for more nice program and udev rule for adding settings to autostart after power on.
*/

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants from the Rust code
#define RTL8152_REQ_REGS 0x05
#define MCU_TYPE_PLA 0x0100
#define BYTE_EN_DWORD 0xff
#define PLA_LED_SELECT 0xdd90

// Function to open USB device by bus and device number
libusb_device_handle* open_device_by_bus_dev(libusb_context* ctx, uint8_t bus_num, uint8_t dev_num) {
    libusb_device** list;
    ssize_t cnt = libusb_get_device_list(ctx, &list);
    if (cnt < 0) {
        fprintf(stderr, "Error getting device list: %s\n", libusb_error_name(cnt));
        return NULL;
    }

    libusb_device_handle* handle = NULL;
    for (ssize_t i = 0; i < cnt; ++i) {
        libusb_device* device = list[i];
        if (libusb_get_bus_number(device) == bus_num && libusb_get_device_address(device) == dev_num) {
            int res = libusb_open(device, &handle);
            if (res != 0) {
                fprintf(stderr, "Error opening device: %s\n", libusb_error_name(res));
                break;
            }
            break;
        }
    }

    libusb_free_device_list(list, 1);
    return handle;
}

int main(int argc, char* argv[]) {
    if (argc < 4 || argc > 5) {
        printf("Usage: %s <bus_num> <dev_num> <read|write> [value]\n", argv[0]);
        printf("  bus_num: USB bus number (decimal)\n");
        printf("  dev_num: USB device number (decimal)\n");
        printf("  read: Read the 32-bit value from register 0xdd90\n");
        printf("  write: Write the 32-bit value to register 0xdd90 (value in hex, e.g., 0xe0087)\n");
        return 1;
    }

    uint8_t bus_num = (uint8_t)strtol(argv[1], NULL, 10);
    uint8_t dev_num = (uint8_t)strtol(argv[2], NULL, 10);
    const char* operation = argv[3];
    uint32_t value = 0;
    if (strcmp(operation, "write") == 0) {
        if (argc != 5) {
            fprintf(stderr, "Write operation requires a value\n");
            return 1;
        }
        value = (uint32_t)strtol(argv[4], NULL, 16);
    } else if (strcmp(operation, "read") != 0) {
        fprintf(stderr, "Operation must be 'read' or 'write'\n");
        return 1;
    }

    libusb_context* ctx = NULL;
    int res = libusb_init(&ctx);
    if (res != 0) {
        fprintf(stderr, "libusb_init failed: %s\n", libusb_error_name(res));
        return 1;
    }

    libusb_device_handle* handle = open_device_by_bus_dev(ctx, bus_num, dev_num);
    if (!handle) {
        fprintf(stderr, "Failed to open device %d:%d\n", bus_num, dev_num);
        libusb_exit(ctx);
        return 1;
    }

    // No interface claiming

    uint16_t offset = PLA_LED_SELECT;  // 0xdd90
    uint16_t wIndex = MCU_TYPE_PLA | BYTE_EN_DWORD;  // 0x0100 | 0xff

    if (strcmp(operation, "write") == 0) {
        res = libusb_control_transfer(
            handle,
            0x40,  // bmRequestType: vendor out
            RTL8152_REQ_REGS,  // bRequest
            offset,  // wValue
            wIndex,  // wIndex
            (unsigned char*)&value,  // data (little-endian)
            4,  // wLength
            5000  // timeout
        );
        if (res < 0) {
            fprintf(stderr, "Write failed: %s\n", libusb_error_name(res));
        } else {
            printf("Successfully wrote 0x%08x to register 0x%04x\n", value, offset);
        }
    } else {  // read
        res = libusb_control_transfer(
            handle,
            0xc0,  // bmRequestType: vendor in
            RTL8152_REQ_REGS,
            offset,
            wIndex,
            (unsigned char*)&value,
            4,
            5000
        );
        if (res < 0) {
            fprintf(stderr, "Read failed: %s\n", libusb_error_name(res));
        } else {
            printf("Read 0x%08x from register 0x%04x\n", value, offset);
        }
    }

    libusb_close(handle);
    libusb_exit(ctx);
    return res < 0 ? 1 : 0;
}

