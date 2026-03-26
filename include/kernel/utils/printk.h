#ifndef PRINTK_H
#define PRINTK_H

#define KERN_INFO "[INFO]  "
#define KERN_ERR "[ERROR] "
#define KERN_WARN "[WARN]  "

void printk(const char* fmt, ...);

#endif