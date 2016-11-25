.extern	system_init
.extern __bss_start
.extern __bss_end
.extern FreeRTOS_IRQ_Handler
.extern FreeRTOS_SVC_Handler    
.extern rpi_cpu_irq_disable
.extern main
.section .init
.globl _start
;;

.equ    CPSR_MODE_USER,         0x10
.equ    CPSR_MODE_FIQ,          0x11
.equ    CPSR_MODE_IRQ,          0x12
.equ    CPSR_MODE_SVR,          0x13
.equ    CPSR_MODE_ABORT,        0x17
.equ    CPSR_MODE_HYP,          0x1A
.equ    CPSR_MODE_UNDEFINED,    0x1B
.equ    CPSR_MODE_SYSTEM,       0x1F

// See ARM section A2.5 (Program status registers)
.equ    CPSR_IRQ_INHIBIT,       0x80
.equ    CPSR_FIQ_INHIBIT,       0x40
.equ    CPSR_THUMB,             0x20
    
_start:
	;@ All the following instruction should be read as:
	;@ Load the address at symbol into the program counter.
	
	ldr	pc,reset_handler		;@ 	Processor Reset handler 		-- we will have to force this on the raspi!
	;@ Because this is the first instruction executed, of cause it causes an immediate branch into reset!
	
	ldr pc,undefined_handler	;@ 	Undefined instruction handler 	-- processors that don't have thumb can emulate thumb!
    ldr pc,swi_handler			;@ 	Software interrupt / TRAP (SVC) -- system SVC handler for switching to kernel mode.
    ldr pc,prefetch_handler		;@ 	Prefetch/abort handler.
    ldr pc,data_handler			;@ 	Data abort handler/
    ldr pc,unused_handler		;@ 	-- Historical from 26-bit addressing ARMs -- was invalid address handler.
    ldr pc,irq_handler			;@ 	IRQ handler
    ldr pc,fiq_handler			;@ 	Fast interrupt handler.

	;@ Here we create an exception address table! This means that reset/hang/irq can be absolute addresses
reset_handler:      .word reset
undefined_handler:  .word undefined_instruction
swi_handler:        .word FreeRTOS_SVC_Handler
prefetch_handler:   .word prefetch_abort
data_handler:       .word data_abort
unused_handler:     .word unused
irq_handler:        .word FreeRTOS_IRQ_Handler
fiq_handler:        .word fiq

reset:
	/* Disable IRQ & FIQ */
	cpsid if

	/* Check for HYP mode */
	mrs r0, cpsr_all
	and r0, r0, #0x1F
	mov r8, #0x1A
	cmp r0, r8
	beq overHyped
	b continueBoot

overHyped: /* Get out of HYP mode */
	ldr r1, =continueBoot
	msr ELR_hyp, r1
	mrs r1, cpsr_all
	and r1, r1, #0x1f	;@ CPSR_MODE_MASK
	orr r1, r1, #0x13	;@ CPSR_MODE_SUPERVISOR
	msr SPSR_hyp, r1
	eret

continueBoot:
	;@	In the reset handler, we need to copy our interrupt vector table to 0x0000, its currently at 0x8000

	mov r0,#0x8000								;@ Store the source pointer
    mov r1,#0x0000								;@ Store the destination pointer.

	;@	Here we copy the branching instructions
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}			;@ Load multiple values from indexed address. 		; Auto-increment R0
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}			;@ Store multiple values from the indexed address.	; Auto-increment R1

	;@	So the branches get the correct address we also need to copy our vector table!
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}			;@ Load from 4*n of regs (8) as R0 is now incremented.
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}			;@ Store this extra set of data.


	;@	Set up the various STACK pointers for different CPU modes
    mov r0,#(CPSR_MODE_UNDEFINED | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
    msr cpsr_c,r0
    mov sp,#(64 * 1024 * 1024)

    mov r0,#(CPSR_MODE_ABORT | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
    msr cpsr_c,r0
    mov sp,#(63 * 1024 * 1024)

    mov r0,#(CPSR_MODE_FIQ | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
    msr cpsr_c,r0
    mov sp,#(62 * 1024 * 1024)

    mov r0,#(CPSR_MODE_IRQ | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
    msr cpsr_c,r0
	mov sp,#(61 * 1024 * 1024)

    mov r0,#(CPSR_MODE_SYSTEM | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
    msr cpsr_c,r0
	mov sp,#(60 * 1024 * 1024)

    mov r0,#(CPSR_MODE_SVR | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
    msr cpsr_c,r0
	mov sp,#(59 * 1024 * 1024)

	ldr r0, =__bss_start
	ldr r1, =__bss_end

	mov r2, #0

zero_loop:
	cmp 	r0,r1
	it		lt
	strlt	r2,[r0], #4
	blt		zero_loop

    
    bl      fpu_init            
	bl 		rpi_cpu_irq_disable

	b main									;@ We're ready?? Lets start main execution!
fpu_init:
    STMFD   sp!,{r0,r1}
    //MRC     p15, 0, r0, c1, c1, 2
    //ORR     r0, r0, #0x3<<10 ;@ enable neon
    //BIC     r0, r0, #0x3<<14 ;@ clear nsasedis/nsd32dis
    //MCR     p15, 0, r0, c1, c1, 2
    LDR     r0, =(0xF << 20)
    MCR     p15, 0, r0, c1, c0, 2
    MOV     r3, #0x40000000 
    VMSR    FPEXC, r3
    LDMFD   sp!,{r0,r1}
    bx     lr

.section .text

.extern c_undefined_instruction
.extern c_prefetch_abort
.extern c_data_abort
.extern c_unused
.extern c_fiq

undefined_instruction:
	b c_undefined_instruction

prefetch_abort:
	b c_prefetch_abort

data_abort:
	b c_data_abort

unused:
	b c_unused

fiq:
	b c_fiq

.globl hang
hang:
	b hang

.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.globl dummy
dummy:
    bx lr

.globl GET_SP
GET_SP: 
    mov r0,sp
    bx lr

