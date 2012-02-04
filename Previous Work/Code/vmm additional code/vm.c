static void	
setControlArea ( struct vmcb *vmcb )
{
    vmcb->tlb_control = 1;

	vmcb->tsc_offset = 0; 
	
	vmcb->guest_asid = 1;

	vmcb->general2_intercepts = INTRCPT_VMRUN;

	vmcb->v_intr_masking = 1; 	

}

static void
setStateSaveArea ( struct vmcb *vmcb )
{
	vmcb->cpl = 0; 

	vmcb->rax = 0;
	vmcb->rsp = 0;

	vmcb->rflags = 2;

	vmcb->cr0 = X86_CR0_ET; 

	vmcb->efer = EFER_SVME;
}

void
vmCreate ( struct vm *vm, unsigned long guest_image_start, unsigned long guest_image_size, unsigned long vm_pmem_size )
{
	vm->vmcb = allocVMCB( );

	const unsigned long vm_pmem_start = allocPMem ( vm_pmem_size );

	vm->vmcb->rip = loadELFImage ( guest_image_start, guest_image_size, vm_pmem_start );

	vm->mbi = initVMMbi ( vm_pmem_start );

	vm->h_cr3 = createShadowTable ( vm_pmem_start, vm_pmem_size );

	setControlArea ( vm->vmcb );
	setStateSaveArea ( vm->vmcb );

	printf ( "New virtual machine created.\n" ); 
}

void
vmBoot ( struct vm *vm )
{
	printf ( "Booting guest operating system.\n" ); 

	u64 vmcb1 = PHYS ( vm->vmcb );
	svm_launch ( vmcb1 );
}



