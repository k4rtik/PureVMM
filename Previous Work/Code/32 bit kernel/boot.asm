;$$$$$$$$$$$$$$$$$$------Start of Copy Pasted MULTIBOOT HEADER------$$$$$$$$$$$$$$$$$$ 
	
ALIGN 4										
    MULTIBOOT_PAGE_ALIGN	equ 1<<0		
    MULTIBOOT_MEMORY_INFO	equ 1<<1		
	MULTIBOOT_AOUT_KLUDGE	equ 1<<16			
    MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002	
    MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
    MULTIBOOT_CHECKSUM	equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
	
[BITS 32]									
	
	global mboot
    extern code, bss, end					

mboot:	
    
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd MULTIBOOT_CHECKSUM
    
    dd mboot
    dd code
    dd bss
    dd end
    dd start

;$$$$$$$$$$$$$$$$$$------End of Copy Pasted MULTIBOOT HEADER------$$$$$$$$$$$$$$$$$$ 
	
global start
start:
	mov esp, sys_stack
	
extern _kickoff
	;push ebx
	call _kickoff	
    jmp $	

SECTION .bss
	resb 8192
sys_stack:	

