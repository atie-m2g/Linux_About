/***************************************************
 *
 * @Mission:
 *   This tools could stdout and change any addressed
 *   bytes as long as the address NOT kernel write 
 *   protect byte.
 *
 * @Usage:
 *   $ make            // generate mycar.ko
 *   $ insmod mycar.ko // default memory map
 *   $ insmod mycar.ko base_addr="0xc0000" addr_offset="0x0" // specified addr memory map
 *   $ dmesg           // check out memory map and its changse
 *
 * @Auther:
 *   atie (ئەخمەتتوختى)
 *
 * @Repository:
 *   https://github.com/atie-m2g
 *
 ***************************************************/

#include <asm/io.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/kobject.h>
#include <linux/platform_device.h>

#define DEVICE_BASE_ADDR      0Xdffe0000
#define DEVICE_CPLD_BASE_ADDR 0x2200
#define DEVICE_CPLD_SIZE      1024

char * base_addr   = "0Xdffe0000";
char * addr_offset = "0x2200";
module_param(base_addr,   charp, 0644);
module_param(addr_offset, charp, 0644);

void * g_cpld_addr = NULL;

char buf[DEVICE_CPLD_SIZE] = {0};
char byte = 0;

static int32_t __init mycar_init(void)
{
    printk("======================BEGIN====================\n");

    uint32_t nbase_addr  = 0x00;
    uint32_t naddr_offset= 0x00;

    u8  byte_data;  // 1 byte
    u16 short_data; // 2 byte
    u32 int_data;   // 4 byte
    u64 long_data;  // 8 byte

    sscanf(base_addr,   "%x", &nbase_addr);
    sscanf(addr_offset, "%x", &naddr_offset);
    if(nbase_addr == 0x00 && naddr_offset == 0) {
        g_cpld_addr = ioremap(DEVICE_BASE_ADDR + DEVICE_CPLD_BASE_ADDR, DEVICE_CPLD_SIZE);
    } else {
        g_cpld_addr = ioremap(nbase_addr + naddr_offset, DEVICE_CPLD_SIZE);
    }

    printk("%5s---------READ ONLY--------\n", "");

    memset(buf, '\0', DEVICE_CPLD_SIZE);
    int i;
    for(i = 1; i < DEVICE_CPLD_SIZE + 1; i++) {
        byte_data = *((char *)g_cpld_addr + i - 1);
        snprintf(buf + strlen(buf), 4, "%02x ", byte_data);

        if(i != 0 && i % 16 ==0) {
            printk("%s\n", buf);
            memset(buf, '\0', DEVICE_CPLD_SIZE);
        }
    }

    memset(buf, '\0', DEVICE_CPLD_SIZE);

    // read 1 byte
    byte_data = readb(g_cpld_addr + 2);
    printk("[001] [readb] offset: 0x02; 1 byte ; value: %02x\n", byte_data);

    // read 2 byte
    short_data = readw(g_cpld_addr + 2);
    printk("[002] [readw] offset: 0x02; 2 bytes; value: %04x\n", short_data);

    // read 4 byte
    int_data = readl(g_cpld_addr + 2);
    printk("[004] [readl] offset: 0x02: 4 bytes; value: %08x\n", int_data);

    // read 8 byte
    long_data = readq(g_cpld_addr + 2);
    printk("[008] [readq] offset: 0x02: 8 bytes; value: %016llx\n", long_data);

#ifdef _DEBUG
    printk("%5s---------AFTER WR---------\n","");

    // write 1 byte
    writeb(0x25, (void *)(g_cpld_addr + 0x10));
    
    // write 2 byte
    writew(0x26, (void *)(g_cpld_addr + 0x11));

    // write 4 byte
    writel(0x27, (void *)(g_cpld_addr + 0x13));

    // write 8 byte
    writeq(0x28, (void *)(g_cpld_addr + 0x17));

    memset(buf, '\0', DEVICE_CPLD_SIZE);
    for(i = 1; i < DEVICE_CPLD_SIZE + 1; i++) {
        byte_data = *((char *)g_cpld_addr + i - 1);
        snprintf(buf + strlen(buf), 4, "%02x ", byte_data);

        if(i != 0 && i % 16 ==0) {
            printk("%s\n", buf);
            memset(buf, '\0', DEVICE_CPLD_SIZE);
        }
    }
    memset(buf, '\0', DEVICE_CPLD_SIZE);

    // read 1 byte
    byte_data = readb(g_cpld_addr + 0x10);
    printk("[001] [readb] offset: 0x10; 1 byte ; value: %02x\n", byte_data);

    // read 2 byte
    short_data = readw(g_cpld_addr + 0x11);
    printk("[002] [readw] offset: 0x11; 2 bytes; value: %04x\n", short_data);

    // read 4 byte
    short_data = readw(g_cpld_addr + 0x11);
    int_data= readl(g_cpld_addr + 0x13);
    printk("[004] [readl] offset: 0x13; 4 bytes; value: %08x\n", int_data);

    // read 8 byte
    long_data= readq(g_cpld_addr + 0x17);
    printk("[008] [readq] offset: 0x17; 8 bytes; value: %016llx\n", long_data);

#endif

    return 0;
}

static void __exit mycar_exit(void)
{
    iounmap(g_cpld_addr);
    g_cpld_addr = NULL;

    printk("======================END======================\n");
}

module_init(mycar_init);
module_exit(mycar_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("John");

/*
 * void writeb(unsigned char data, unsigned short addr)
 *
 * */

