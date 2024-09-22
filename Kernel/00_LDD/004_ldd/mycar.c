#include <asm/io.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/kobject.h>
#include <linux/platform_device.h>


#define DEVICE_BASE_ADDR      0Xdffe0000
#define DEVICE_CPLD_BASE_ADDR 0x2200
#define DEVICE_CPLD_SIZE      128

//#define DEVICE_BASE_ADDR      0X0
//#define DEVICE_CPLD_BASE_ADDR 0x0
//#define DEVICE_CPLD_SIZE      1024

void * g_cpld_addr = NULL;

char buf[DEVICE_CPLD_SIZE] = {0};
char byte = 0;

static int32_t __init mycar_init(void)
{
    g_cpld_addr = ioremap(DEVICE_BASE_ADDR + DEVICE_CPLD_BASE_ADDR, DEVICE_CPLD_SIZE);

    memset(buf, '\0', DEVICE_CPLD_SIZE);
    for(int i = 1; i < DEVICE_CPLD_SIZE + 1; i++) {
        byte = *((char *)g_cpld_addr + i - 1);
        snprintf(buf + strlen(buf), 4, "%02x ", byte);

        if(i != 0 && i % 16 ==0) {
            printk("%s\n", buf);
            memset(buf, '\0', DEVICE_CPLD_SIZE);
        }
    }

    memset(buf, '\0', DEVICE_CPLD_SIZE);

    return 0;
}

static void __exit mycar_exit(void)
{
    iounmap(g_cpld_addr);
	g_cpld_addr = NULL;
}

module_init(mycar_init);
module_exit(mycar_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("John");

