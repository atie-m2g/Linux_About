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

static int  node_age_val            = 10;
static char node_name_val[BUF_SIZE] = {"Hello"};

// sysfs node for '/sys/atie_node'
static struct kobject * root_node_device = NULL;
// sysfs node for '/sys/atie_node/mycar'
static struct kobject * car_node_device  = NULL;

/************************************************************/
/*******************UTILITIES DOMAIN**************************/
/************************************************************/


static ssize_t node_age_show(struct device *dev, struct device_attribute *da, char *buf)
{
	ssize_t ret = 0;
	snprintf(buf, 32, "%d\n", node_age_val);
	ret = strlen(buf);

    struct attribute *pattr = &da->attr;
    //da->show
    //da->show

    printk("da->show=0x%x \n", da->show);
    printk("%s \n", pattr->name);


    return ret;
}

static ssize_t node_age_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    int input;

    sscanf(buf, "%d", &input);
    node_age_val = input;

	return count;
}

static DEVICE_ATTR(node_age, S_IWUSR | S_IRUGO, node_age_show, node_age_store);

static struct attribute * all_node_attrs[] = {
    &dev_attr_node_age.attr,
    NULL
};

static struct attribute_group all_node_attr_group = {
    .attrs = all_node_attrs,
};

static int32_t mycar_init(void)
{
	int32_t ret = 0;

	root_node_device = kobject_create_and_add("atie_node", NULL);
	if (root_node_device == NULL) {
		pr_err("%s: subsystem_register failed\n", __func__);
		ret = -ENOMEM;
		return ret;
	}

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

    printk("show=0x%x \n", node_age_show);

//    struct student {  
//        char *name;  
//        int age;  
//    };  
//    struct student a = {"Mike", 10};
//    int *page = &a.age;
//
//    struct student *p = container_of(page, struct student, age);
//    printk("%s \n", p->name);
//
	return 0;
}

static void __exit mycar_exit(void)
{
	kobject_put(car_node_device);
	kobject_put(root_node_device);
}

module_init(mycar_init);
module_exit(mycar_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("John");

