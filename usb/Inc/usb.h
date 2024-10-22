
#include "usbd_core.h"
#include "usb_std.h"


#define USBD_STM32F429FS
//#define USBD_STM32F429HS

//#if defined(USBD_PRIMARY_OTGHS)
//#define usbd_hw usbd_otghs
//#else
//#define usbd_hw usbd_otgfs
//#endif

extern const struct usbd_driver usbd_otgfs;
#define usbd_hw usbd_otgfs

void usbd_hw_init(usbd_device *dev);


#ifndef BYTE_ORDER
#define BYTE_ORDER       LITTLE_ENDIAN
#endif 

#define CPU_TO_LE16(x)   (x)
#define CPU_TO_LE32(x)   (x)
#define cpu_to_le16(x)   (x)
#define cpu_to_le32(x)   (x)

static inline uint32_t cpu_to_le32p(const uint32_t *p)
{
	return (uint32_t)*p;
}
static inline uint16_t cpu_to_le16p(const uint16_t *p)
{
	return (uint16_t)*p;
}
static inline uint32_t le32_to_cpup(const uint32_t *p)
{
	return (uint32_t)*p;
}
static inline uint16_t le16_to_cpup(const uint16_t *p)
{
	return (uint16_t)*p;
}


//#define USBD_SOF_DISABLED