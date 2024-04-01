#include <linux/kernel.h>

asmlinkage long sys_mysyscall(unsigned long param)
{

  long result = 0;
  
  printk("My SysCall\n");
  printk("syscall param %lu\n", param);
  
  return result;
  
}
