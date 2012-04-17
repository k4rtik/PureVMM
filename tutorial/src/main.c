// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials

int main(struct multiboot *mboot_ptr)
{

  monitor_clear();
  monitor_write("Hello, world!");

  return 0;
} 
