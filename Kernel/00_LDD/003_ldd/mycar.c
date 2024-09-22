#include <linux/module.h>
#include <linux/slab.h>
#include <linux/kobject.h>
#include <linux/platform_device.h>

/************************************************************/
/*******************DATA DOMAIN******************************/
/************************************************************/

#define MODULE_NAME "mycar"
#define LOG_LABEL   "M2G"

#define BUF_SIZE 0x32

#define to_dev_attr(_attr) container_of(&_attr, struct device_attribute, attr)

static int  node_age_val            = 10;
static char node_name_val[BUF_SIZE] = {"Hello"};

// sysfs node for '/sys/atie_node'
static struct kobject * root_node_device = NULL;
// sysfs node for '/sys/atie_node/mycar'
static struct kobject * car_node_device  = NULL;

/************************************************************/
/*******************UTILITIES DOMAIN**************************/
/************************************************************/

//printk("[%s] | [%s] | %s | %s:%u | buf='%s' len=%ld \n", MODULE_NAME, LOG_LABEL, __FUNCTION__, __FILE__, __LINE__, buf, ret);

/*
 * access 'node1' file callbacks
 */
static ssize_t node_age_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	// don't use this params currently
	(void)dev;
	//(void)attr;

    //struct device_attribute *dev_attr = to_dev_attr(attr);
    struct device_attribute * dev_attr = container_of(attr, struct device_attribute *, attr);

	ssize_t ret = 0;
	snprintf(buf, 32, "%d\n", node_age_val);
	ret = strlen(buf);

    //printk("%d \n", attr->node_age);

    return ret;
}

/*
 * write 'node1' file callbacks
 */
static ssize_t node_age_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    int input;

    sscanf(buf, "%d", &input);
    node_age_val = input;

    /*
     * Here MUST return a bigger than 1 integer value, otherwise the
     * command panel block and NOT terninate unless ternimate it forcely
     * */
	return count;
}

/*
 * bind attr and its handler func
 */
static DEVICE_ATTR(node_age, S_IWUSR | S_IRUGO, node_age_show, node_age_store);

static struct attribute * all_node_attrs[] = {
    &dev_attr_node_age.attr,
    NULL
};

static struct attribute_group all_node_attr_group = {
    .attrs = all_node_attrs,
};

/*
 * Module Access Entry
 */
static int32_t mycar_init(void)
{
	printk("[%s] %s \n", MODULE_NAME, "======== access from mycar.ko ========");

	int32_t ret = 0;

    /* on '/sys/' add a new node named 'atie_node', which means '/sys/atie_node/' */
	root_node_device = kobject_create_and_add("atie_node", NULL);
	if (root_node_device == NULL) {
		pr_err("%s: subsystem_register failed\n", __func__);
		ret = -ENOMEM;
		return ret;
	}

    /* on '/sys/atie_node' add a new node named 'my_car', which means '/sys/atie_node/my_car' */
    car_node_device = kobject_create_and_add("mycar", root_node_device);
    if (car_node_device == NULL) {
        pr_err("%s: subsystem_register failed\n", __func__);
        ret = -ENOMEM;
        return ret;
    }

	ret = sysfs_create_group(car_node_device, &all_node_attr_group);
	if (ret) {
		pr_err("%s: sysfs_create_file failed\n", __func__);
		kobject_del(car_node_device);
		kobject_del(root_node_device);

		ret = -ENOMEM;
        return ret;
	}

	return 0;
}

/*
 * Module Exit Entry
 */
static void __exit mycar_exit(void)
{
	printk("[%s] %s %s \n", MODULE_NAME, LOG_LABEL, "==== exit from mycar.ko ====");

	// node-ref sub 1
	kobject_put(car_node_device);
	kobject_put(root_node_device);
}

// Register of Module Access Entry
module_init(mycar_init);
// Register of Module Exit Entry
module_exit(mycar_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("John");

