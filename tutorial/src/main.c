// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials

#include "common.h"
#include "stdarg.h"

void vout(char *string, char *fmt,...);

char fmt1[] = "%s %s %s\n";

int main(struct multiboot *mboot_ptr)
{

    // Initialise all the ISRs and segmentation
    init_descriptor_tables();
    // Initialise the screen (by clearing it)
    monitor_clear();

    // Write out a sample string
    monitor_write("Hello, paging world!\n");

    u32int a = kmalloc(8);
    initialise_paging();
    u32int b = kmalloc(8);
    u32int c = kmalloc(8);
    monitor_write("a: ");
    monitor_write_hex(a);
    monitor_write(", b: ");
    monitor_write_hex(b);
    monitor_write("\nc: ");
    monitor_write_hex(c);
    
    kfree(c);
    kfree(b);
    u32int d = kmalloc(12);
    monitor_write(", d: ");
    monitor_write_hex(d);

    monitor_write("\n");
 
    char string[100];
 
    vout(string, fmt1, "Sat", "Sun", "Mon");
    monitor_write(string);
    return 0;
 
}

void vout(char *string, char *fmt,...)
{
    va_list arg_ptr;
  
    va_start(arg_ptr, fmt);
    vsprintf(string, fmt, arg_ptr);
    va_end(arg_ptr);
} 
  
