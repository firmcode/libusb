#include <stdio.h>
#include "libusb.h"

static void print_devs(libusb_device **devs)
{
	libusb_device *dev;
	int i = 0, j = 0;
	uint8_t path[8]; 

	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		struct libusb_config_descriptor *conf;
		int * config;
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0) {
			fprintf(stderr, "failed to get device descriptor");
			return;
		}
		r = libusb_get_config_descriptor(dev,0 ,&conf);
		if (r < 0) {
                        fprintf(stderr, "failed to get conf descriptor");
                        return;
                }



		printf("Bus %03x Device %03x : ",libusb_get_bus_number(dev), libusb_get_device_address(dev));
		printf(" ID   %04x:%04x\n",desc.idVendor, desc.idProduct);

/*		r = libusb_get_port_numbers(dev, path, sizeof(path));
		if (r > 0) {
			printf(" path: %d", path[0]);
			for (j = 1; j < r; j++)
				printf(".%d", path[j]);
		}
*/
		printf("Device Descriptor:\n");
		printf("  bLength : %04x\n",desc.bLength);
		printf("  bDescriptorType %04x\n",desc.bDescriptorType);
		printf("  bcdUSB : %04x\n",desc.bcdUSB);
		printf("  bDeviceClass : %04x\n",desc.bDeviceClass);
		printf("  bDeviceProtocol : %04x\n",desc.bDeviceProtocol);
		printf("  bMaxPacketSize0 : %04x\n",desc.bMaxPacketSize0);
		printf("  idVendor : %04x\n",desc.idVendor);
		printf("  idProduct : %04x\n",desc.idProduct);
		printf("  bcdDevice : %04x\n",desc.bcdDevice);
		printf("  iManufacturer : %04x\n",desc.iManufacturer);
		printf("  iProduct : %04x\n",desc.iProduct);
		printf("  iSerial : %04x\n",desc.iSerialNumber);
		printf("  bNumConfigurations : %04x\n",desc.bNumConfigurations);
		
		printf("  Configuration Descriptor:\n");
		printf("    bLength %d\n",conf->bLength);
		printf("    bDescriptorType %d\n",conf->bDescriptorType);
		printf("    bNumInterfaces %d\n",conf->bNumInterfaces);
		printf("    bConfigurationValue %d\n",conf->bConfigurationValue);
		printf("    bmAttributes 0x%x\n",conf->bmAttributes);
		printf("    MaxPower %dmA\n",conf->MaxPower);

		printf("    Interface Descriptor:\n");
		printf("      bLength %d\n",conf->interface->altsetting->bLength);
		printf("      bDescriptorType %d\n",conf->interface->altsetting->bDescriptorType);
		printf("      bInterfaceNumber %d\n",conf->interface->altsetting->bInterfaceNumber);
		printf("      bAlternateSetting %d\n",conf->interface->altsetting->bAlternateSetting);
		printf("      bInterfaceClass %d Hub\n",conf->interface->altsetting->bInterfaceClass);
		printf("      bInterfaceSubClass %d\n",conf->interface->altsetting->bInterfaceSubClass);
		printf("      bInterfaceProtocol %d\n",conf->interface->altsetting->bInterfaceProtocol);
		printf("      iInterface %d\n",conf->interface->altsetting->iInterface);

		printf("	Endpoint Descriptor\n");
    		printf("bEndpointAddress: %02x\n",conf->interface->altsetting->endpoint->bEndpointAddress);
    		printf("bmAttributes: %02x\n", conf->interface->altsetting->endpoint->bmAttributes);
    		printf("wMaxPacketSize: %02x\n", conf->interface->altsetting->endpoint->wMaxPacketSize);
    		printf("bInterval: %02x\n", conf->interface->altsetting->endpoint->bInterval);
	    	printf("bRefresh: %02x\n", conf->interface->altsetting->endpoint->bRefresh);
	    	printf("bSynchAddress: %02x\n", conf->interface->altsetting->endpoint->bSynchAddress);
	    	printf("\n");


		printf("=======================================================\n");
	}
}

int main(void)
{
	libusb_device **devs;
	int r;
	ssize_t cnt;

	r = libusb_init(NULL);
	if (r < 0)
		return r;

	cnt = libusb_get_device_list(NULL, &devs);
	if (cnt < 0)
		return (int) cnt;

	print_devs(devs);
	libusb_free_device_list(devs, 1);

	libusb_exit(NULL);
	return 0;
}
