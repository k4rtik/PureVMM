global _gdtFlush
extern _gptr
_gdtFlush:
	lgdt [_gptr]
	mov ax, 0x10
	mov ds, ax
	mov es, ax
    	mov fs, ax
    	mov gs, ax
    	mov ss, ax
	jmp 0x08:next
next:
	ret
	
global _idtFlush
extern _iptr
_idtFlush:
	lidt [_iptr]
	ret

