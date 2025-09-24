/*
sudo apt install libusb-1.0-0-dev
gcc -Wall -Wextra led_rtl1853_final.c -lusb-1.0 -o led_rtl1853_final
gcc -Wall -Wextra -DDEBUG led_rtl1853_final.c -lusb-1.0 -o led_rtl1853_final_debug

sudo apt install libusb-1.0-0
sudo ./led_rtl1853_final
or add in /etc/rc.local

This program applies led settings for all founded rtl8153 devices
*/

#include <libusb-1.0/libusb.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#define TARGET_LED_SETTINGS 0x000e0f00

#define TARGET_VENDOR_ID 0x0bda
#define TARGET_PRODUCT_ID 0x8153

int main(void) {
    libusb_context* ctx = NULL;
    int res = libusb_init(&ctx);
    if (res != 0) {
#ifdef DEBUG
        fprintf(stderr, "libusb_init failed: %s\n", libusb_error_name(res));
#endif
        return 1;
    }

    libusb_device** list = NULL;
    ssize_t cnt = libusb_get_device_list(ctx, &list);
    if (cnt < 0) {
#ifdef DEBUG
        fprintf(stderr, "Error getting device list: %s\n", libusb_error_name((int)cnt));
#endif
        libusb_exit(ctx);
        return 1;
    }

    uint16_t offset = 0xdd90;
    uint16_t wIndex = 0x0100 | 0xff;
    uint32_t value = TARGET_LED_SETTINGS;

#ifdef DEBUG
    int found = 0;
#endif

    libusb_device* device;
    struct libusb_device_descriptor desc;
    libusb_device_handle* handle;
    for (ssize_t i = 0; i < cnt; ++i) {
        device = list[i];
        if (libusb_get_device_descriptor(device, &desc) == 0 &&
            desc.idVendor == TARGET_VENDOR_ID && desc.idProduct == TARGET_PRODUCT_ID) {
            
            res = libusb_open(device, &handle);
            if (res != 0) {
#ifdef DEBUG
                fprintf(stderr, "Failed to open device %d:%d: %s\n",
                        libusb_get_bus_number(device), libusb_get_device_address(device),
                        libusb_error_name(res));
#endif
                continue;
            }

            res = libusb_control_transfer(
                handle,
                0x40,
                0x05,
                offset,
                wIndex,
                (unsigned char*)&value,
                4,
                5000
            );

#ifdef DEBUG
            if (res < 0) {
                fprintf(stderr, "Write failed for device %d:%d: %s\n",
                        libusb_get_bus_number(device), libusb_get_device_address(device),
                        libusb_error_name(res));
            } else {
                printf("Wrote 0x%08x (actually %d bytes) to register 0x%04x on device %d:%d\n",
                       value, res, offset,
                       libusb_get_bus_number(device), libusb_get_device_address(device));
                found = 1;
            }
#endif

            libusb_close(handle);
        }
    }

    libusb_free_device_list(list, 1);

#ifdef DEBUG
    if (!found) {
        printf("No target devices found.\n");
    }
#endif

    libusb_exit(ctx);
    return 0;
}


