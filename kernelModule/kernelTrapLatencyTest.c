#include "ktlt.h"

struct proc_dir_entry *ktltDir;

char kbuf[4096];

static ssize_t ktlt_tf_write(struct file *file,
				const char __user *buffer,
				size_t count, loff_t *ppos)
{
	return copy_from_user(kbuf, buffer, 4096);
	//return count;
}


static const struct file_operations tf_fops = {
  .owner = THIS_MODULE,
  .write = ktlt_tf_write,
};

static ssize_t ktlt_ctrl_write(struct file *file,
				const char __user *buffer,
				size_t count, loff_t *ppos)
{
	char usrCommand[512];
	int ret;

	ret = copy_from_user(usrCommand, buffer,count);
	//pr_notice("command:%s",usrCommand);
	switch (usrCommand[0]) {
	case 'c':
		pr_notice("create test file\n");
		proc_create("tf", 0, ktltDir, &tf_fops);
		break;
	case 'd':
		pr_notice("delete test file\n");
		remove_proc_entry("tf", ktltDir);
		break;
	case 'p':
		pr_notice("print calling stack\n");
		WARN_ON(1);
		break;
	}
	return count;
}



static const struct file_operations ktlt_ctrl_fops = {
  .owner = THIS_MODULE,
  .write = ktlt_ctrl_write,
};

static int __init qblk_module_init(void)
{
	ktltDir = proc_mkdir("ktlt",NULL);
	if (!ktltDir) {
		pr_err("Can't create proc directory [ktlt]!\n");
		return -ENOMEM;
	}
	proc_create("ctrl", 0, ktltDir, &ktlt_ctrl_fops);
	return 0;

	proc_remove(ktltDir);
	return -ENOMEM;
}

static void qblk_module_exit(void)
{
	remove_proc_entry("ctrl", ktltDir);
	proc_remove(ktltDir);
}

module_init(qblk_module_init);
module_exit(qblk_module_exit);
MODULE_AUTHOR("Hongwei Qin <glqhw@hust.edu.cn>");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Test the latency of trapping into kernel");
