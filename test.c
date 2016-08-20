#include <stdio.h>
#include <libusb-1.0/libusb.h>

int printdev(libusb_device *dev) {
        struct libusb_device_descriptor desc;
        int r = libusb_get_device_descriptor(dev, &desc);

        if(r < 0) {
                printf("Error : Device Descriptor eating failed.\n");
                return -1;
        }
        struct libusb_config_descriptor *config;
        libusb_get_config_descriptor(dev, 0, &config);

        const struct libusb_interface *inter;
        const struct libusb_interface_descriptor *interdesc ;

        const struct libusb_endpoint_descriptor *epdesc;

        printf("        Device Descriptor\n");
        printf("bcdUSB: %04x\n",desc.bcdUSB);
        printf("bDeviceClass: %02x\n",desc.bDeviceClass);
        printf("bDeviceSubClass: %02x\n",desc.bDeviceSubClass);
        printf("bDeviceProtocol: %02x\n",desc.bDeviceProtocol);
        printf("bMaxPacketSize0: %02x\n",desc.bMaxPacketSize0);
        printf("id Vendor : %4.0x\t", (int)desc.idVendor);
        printf("id Product : %4.0x\t", (int)desc.idProduct);
        printf("Interfaces : %d\n", (int)config->bNumInterfaces);
        printf("bcdDevice: %04x\n", (int)desc.bcdDevice);
	printf("iManufacturer: %d\n",desc.iManufacturer);
	printf("\tiProduct: %d\n", desc.iProduct);
    	printf("\tiSerialNumber: %d\n", desc.iSerialNumber);
    	printf("\tbNumConfiguration: %02x\n", desc.bNumConfigurations);
	printf("*********************************************\n");
    	printf("\n");

	printf("	Config Descriptor\n");
   	printf("wTotalLength: %04x\n", config->wTotalLength);
    	printf("bNumInterfaces: %02x\n", config->bNumInterfaces);
    	printf("bConfigurationValue: %02x\n", config->bConfigurationValue);
    	printf("iConfiguration: %d\n", config->iConfiguration);
    	printf("bmAttributes: %02x\n", config->bmAttributes);
    	printf("bMaxPower: %d mA\n", config->MaxPower * 2);
    	printf("\n");
	printf("*********************************************\n");

	printf("	Endpoint Descriptor\n");
    	printf("bEndpointAddress: %02x\n",config->interface->altsetting->endpoint->bEndpointAddress);
    	printf("bmAttributes: %02x\n", config->interface->altsetting->endpoint->bmAttributes);
    	printf("wMaxPacketSize: %02x\n", config->interface->altsetting->endpoint->wMaxPacketSize);
    	printf("bInterval: %02x\n", config->interface->altsetting->endpoint->bInterval);
    	printf("bRefresh: %02x\n", config->interface->altsetting->endpoint->bRefresh);
    	printf("bSynchAddress: %02x\n", config->interface->altsetting->endpoint->bSynchAddress);
    	printf("\n");

	printf("*********************************************\n");
	printf("	Interface Descriptor\n");
	printf("bLength: %d\n",config->interface->altsetting->bLength);
        printf("bInterfaceNumber: %02x\n", config->interface->altsetting->bInterfaceNumber);
        printf("bAlternateSetting: %02x\n", config->interface->altsetting->bAlternateSetting);
        printf("bNumEndpoints: %02x\n", config->interface->altsetting->bNumEndpoints);
        printf("bInterfaceClass: %02x\n", config->interface->altsetting->bInterfaceClass);
        printf("bInterfaceSubClass: %02x\n", config->interface->altsetting->bInterfaceSubClass);
        printf("bInterfaceProtocol: %02x\n", config->interface->altsetting->bInterfaceProtocol);
	printf("iInterface: %02x\n",config->interface->altsetting->iInterface);
	printf("********************************************\n");





    	printf("\n");



}

int main(int argc, char *argv[]) {
        libusb_device **devs;
        libusb_context *ctx = NULL;
        int r;
        ssize_t cnt;
	int i;

        if(r = libusb_init(&ctx) < 0) {
                printf("Error : %d\n", r);
                return -1;
        }
        libusb_set_debug(ctx, 3);

        if((cnt = libusb_get_device_list(ctx, &devs)) < 0) {
                printf("Error : Getting Device list failed.\n");
                return -1;
        }

        for(i=0; i<cnt; i++) {
                printdev(devs[i]);
        }

        libusb_free_device_list(devs, 1);
        libusb_exit(ctx);

        return 0;
}
