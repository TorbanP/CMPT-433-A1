/*  
 *  CMPT 422
 * A.4 - Write a C module that can be inserted as a kernel module that prints "Hello, world" when the module is initialized and installed, 
 * and "Goodbye, world" when the module is removed.
 */

#include <linux/module.h> // Required for modules
#include <linux/kernel.h>	// Required for KERN_INFO

int init_module(void){ // start function (Required)

	printk(KERN_INFO "Hello, world\n");
	return 0;
}

void cleanup_module(void){ // cleanup module (Required)

	printk(KERN_INFO "Goodbye, world\n");
}