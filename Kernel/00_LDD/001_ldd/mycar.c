#include <linux/module.h>
#include <linux/slab.h>
#include <linux/kobject.h>
#include <linux/platform_device.h>

/************************************************************/
/*******************DATA DOMAIN******************************/
/************************************************************/

#define MODULE_NAME "mycar"
#define LOG_LABEL   "M2G"

static int node1_value = 10;
static int node2_value = 11;
static int node3_value = 12;

// sysfs node for '/sys/atie_node'
static struct kobject * root_node_device = NULL;
// sysfs node for '/sys/atie_node/mycar'
static struct kobject * car_node_device  = NULL;

/************************************************************/
/*******************UTILITIES DOMAIN**************************/
/************************************************************/

/*
 * access 'node1' file callbacks
 */
static ssize_t node1_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	// don't use this params currently
	(void)dev;
	(void)attr;

	ssize_t ret = 0;
	snprintf(buf, 32, "%d\n", node1_value);
	ret = strlen(buf);
	//printk("[%s] | [%s] | %s | %s:%u | buf='%s' len=%ld \n", MODULE_NAME, LOG_LABEL, __FUNCTION__, __FILE__, __LINE__, buf, ret);

    return ret;
}

/*
 * access 'node2' file callbacks
 */
static ssize_t node2_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    // don't use this params currently
    (void)dev;
    (void)attr;

    ssize_t ret = 0;
    snprintf(buf, 8, "%d\n", node2_value);
    ret = strlen(buf);

    //printk("[%s] | [%s] | %s | %s:%u | buf='%s' \n", MODULE_NAME, LOG_LABEL, __FUNCTION__, __FILE__, __LINE__, buf);

    return ret;
}

/*
 * access 'node3' file callbacks
 */
static ssize_t node3_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    // don't use this params currently
    (void)dev;
    (void)attr;

    ssize_t ret = 0;
    snprintf(buf, 8, "%d\n", node3_value);
    ret = strlen(buf);

    //printk("[%s] | [%s] | %s | %s:%u | buf='%s' \n", MODULE_NAME, LOG_LABEL, __FUNCTION__, __FILE__, __LINE__, buf);

    return ret;
}

/*
 * write 'node1' file callbacks
 */
static ssize_t node1_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    //printk("|buf='%s'|count=%ld \n", buf, count);
	//printk("[%s] | [%s] | %s | %s:%u | buf='%s' | count=%ld \n", MODULE_NAME, LOG_LABEL, __FUNCTION__, __FILE__, __LINE__, buf, count);

    int input;
    sscanf(buf, "%d", &input);
    //printk("input=%d \n", input);

    node1_value = input;

    /*
     * Here MUST return a bigger than 1 integer value, otherwise the
     * command panel block and NOT terninate unless ternimate it forcely
     * */
	return count;
}

/*
 * bind attr and its handler func
 */
static DEVICE_ATTR(node1, S_IWUSR | S_IRUGO, node1_show, node1_store);
static DEVICE_ATTR(node2, 0444, node2_show, NULL);
static DEVICE_ATTR(node3, 0444, node3_show, NULL);

static struct attribute * all_node_attrs[] = {
    &dev_attr_node1.attr,
    &dev_attr_node2.attr,
    &dev_attr_node3.attr,
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
	printk("[%s] | [%s] | %s | %s:%u \n", MODULE_NAME, LOG_LABEL, __FUNCTION__, __FILE__, __LINE__);

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

	printk("[%s] | [%s] | %s | %s:%u \n", MODULE_NAME, LOG_LABEL, __FUNCTION__, __FILE__, __LINE__);

	// node-ref sub 1
	kobject_put(car_node_device);
	kobject_put(root_node_device);

	printk("[%s] | [%s] | %s | %s:%u \n", MODULE_NAME, LOG_LABEL, __FUNCTION__, __FILE__, __LINE__);
}

// Register of Module Access Entry
module_init(mycar_init);
// Register of Module Exit Entry
module_exit(mycar_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("John");

