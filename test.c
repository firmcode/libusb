#include "libusb.h"
#include <stdio.h>
 
/* the device's vendor and product id */
#define MY_VID                  0x1A89      // iNCITE
#define MY_PID                  0x3000      // iNCITE
 
/* the device's endpoints */
#define EP_IN                   0x81
#define EP_OUT                  0x01
 
#define BUF_SIZE                64
 
#define USB_DIR_IN              0x80
#define USB_DIR_OUT             0x00
 
#define VENDOR_REQUEST_IVORY    0x54        // iNCITE
 
usb_dev_handle *open_dev(void);
 
usb_dev_handle *open_dev(void)
{
    struct usb_bus *bus;
    struct usb_device *dev;
 
    for(bus = usb_get_busses(); bus; bus = bus->next)
    {
        for(dev = bus->devices; dev; dev = dev->next)
        {
            if(dev->descriptor.idVendor == MY_VID
                && dev->descriptor.idProduct == MY_PID)
            {
                fprintf(stdout, "Found device: 0x%x:0x%x\n", MY_VID, MY_PID);
                fprintf(stdout, "Device filename: %s\n", dev->filename);
                fprintf(stdout, "Bus dirname: %s\n", bus->dirname);
 
                usb_dev_handle *fdev;
                fdev = usb_open(dev);
 
                char string[50];
                usb_get_string_simple(fdev, dev->descriptor.iManufacturer, string, sizeof(string));
                fprintf(stdout, "Device Manfucaturer : %s\n" ,string);
                usb_get_string_simple(fdev, dev->descriptor.iProduct, string, sizeof(string));
                fprintf(stdout, "Product Name : %s\n", string);
                usb_get_string_simple(fdev, dev->descriptor.iSerialNumber, string, sizeof(string));
                fprintf(stdout, "Device Serial Number: %s\n", string);
 
                return fdev;
            }
        }
    }
 
    return NULL;
}
 
// Device I/O control
int ivory_get_cid(usb_dev_handle *dev)      // iNCITE
{
    #define HIU_CID                 0x054
 
    int ret = 0;
    unsigned char buf[BUF_SIZE];
 
    ret = usb_control_msg(dev,
        USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,    // __u8 requesttype
        VENDOR_REQUEST_IVORY,                               // __u8 request
        HIU_CID,                                            // __u16 value
        0,                                                  // __u16 index
        buf,                                                // void *data
        sizeof(int),                                        // __u16 size
        5000                                                // int timeout
    );
 
    return *buf;
}
 
int main(void)
{
    usb_dev_handle *dev = NULL; /* the device handle */
 
    usb_init(); /* initialize the library */
    usb_find_busses(); /* find all busses */
    usb_find_devices(); /* find all connected devices */
 
    if(!(dev = open_dev()))
    {
        printf("error: device not found!\n");
        return 0;
    }
 
    if(usb_set_configuration(dev, 1) < 0)
    {
        printf("error: setting config 1 failed\n");
        usb_close(dev);
        return 0;
    }
 
    if(usb_claim_interface(dev, 0) < 0)
    {
        printf("error: claiming interface 0 failed\n");
        usb_close(dev);
        return 0;
    }
 
    // ---------------------------------------
    int cid;
    cid = ivory_get_cid(dev);
 
    fprintf(stdout, "CID: %d\n", cid);
    // ---------------------------------------
 
    usb_release_interface(dev, 0);
    usb_close(dev);
 
    return 0;
}

