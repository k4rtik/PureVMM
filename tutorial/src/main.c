// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials

int main(struct multiboot *mboot_ptr)
{
    // Initialise all the ISRs and segmentation
    init_descriptor_tables();
    // Initialise the screen (by clearing it)
    monitor_clear();
    // Write out a sample string
    monitor_write("Hello, world!\n");

    asm volatile("int $0x3");
    asm volatile("int $0x4");

    asm volatile("sti"); // need to enable interrupts

    init_timer(50); //frequency = 50Hz
    
    return 0;

} 
