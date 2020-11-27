#include <stdint.h>

#include "checkpoint.h"
#include "checkpoint_selector.h"
#include "checkpoint_util_mem.h"
#include "checkpoint_arch.h"
#include "checkpoint_registers.h"

CHECKPOINT_EXCLUDE_BSS volatile registers_t *registers_checkpoint_ptr;
CHECKPOINT_EXCLUDE_BSS volatile registers_t checkpoint_restored_flag;

void restore_registers(void) {
    //checkpoint_restored_flag = 1;
    registers_checkpoint_ptr = registers_checkpoint_nvm[checkpoint_get_restore_idx()];
    restore_registers_asm();
}

/*
 * Register array layout (every register is 4 bytes)
 * idx: | 0                                                                            15(60)| 16(64)
        +------------------------------------------------------------------------------------+
   reg: | r0 | r1 | r2 | r3 | r4 | r5 | r6 | r7 | r8 | r9 | r10 | r11 | r12 | sp | lr | xpsr |
        +------------------------------------------------------------------------------------+
*/

__attribute__((naked))
void checkpoint_registers_asm(void) {
    LIBCP_ASM("cpsid i"); // disable interrupts

    LIBCP_ASM("push {r6, r7}");

    // Set the restored flag to checkpoint
    LIBCP_ASM("mov r6, #0");
    LIBCP_ASM("ldr r7, =checkpoint_restored_flag");
    LIBCP_ASM("str r6, [r7]");

    // Load the register checkpoint location
    LIBCP_ASM("ldr r7, =registers_checkpoint_ptr");
    LIBCP_ASM("ldr r7, [r7]");

    LIBCP_ASM("stmia r7!, {r0-r5}");

    LIBCP_ASM("pop {r0, r1}"); // load r6,r7 into r0, r1

    LIBCP_ASM("mov r2, r8");
    LIBCP_ASM("mov r3, r9");
    LIBCP_ASM("mov r4, r10");
    LIBCP_ASM("mov r5, r11");
    LIBCP_ASM("mov r6, r12");

    LIBCP_ASM("stmia r7!, {r0-r6}");

    LIBCP_ASM("mov r0, sp");
    LIBCP_ASM("mov r1, lr");
    LIBCP_ASM("mrs r2, xpsr");
    LIBCP_ASM("stmia r7!, {r0-r2}");

    /* Restore the low registers registers */
    LIBCP_ASM("sub r7, #64"); // point to r0-r4

    LIBCP_ASM("ldmia r7!, {r0-r6}"); // restore r0-r6
    LIBCP_ASM("ldr r7, [r7]"); // load r7

    LIBCP_ASM("cpsie i"); // enable interrupts
    LIBCP_ASM("bx lr");
}

__attribute__((naked))
void restore_registers_asm(void) {
    // We don't care about the current registers here, we will overwrite them
    LIBCP_ASM("cpsid i"); // disable interrupts

    // Set the restored flag to checkpoint
    LIBCP_ASM("mov r6, #1");
    LIBCP_ASM("ldr r7, =checkpoint_restored_flag");
    LIBCP_ASM("str r6, [r7]");

    LIBCP_ASM("ldr r7, =registers_checkpoint_ptr");
    LIBCP_ASM("ldr r7, [r7]");

    LIBCP_ASM("add r7, #32"); // point to r8-r12, sp, lr

    LIBCP_ASM("ldmia r7!, {r0-r6}"); // load r8-r12, sp, lr into r0-r6
    LIBCP_ASM("mov r8, r0");
    LIBCP_ASM("mov r9, r1");
    LIBCP_ASM("mov r10, r2");
    LIBCP_ASM("mov r11, r3");
    LIBCP_ASM("mov r12, r4");

    LIBCP_ASM("mov sp, r5");
    LIBCP_ASM("mov lr, r6");

    LIBCP_ASM("ldr r0, [r7]"); // load psr into r0
    LIBCP_ASM("msr xpsr, r0");

    LIBCP_ASM("sub r7, #60"); // point to r0-r7

    LIBCP_ASM("ldmia r7, {r0-r7}"); // load r0-r7 into r0-r7

    LIBCP_ASM("cpsie i"); // enable interrupts
    LIBCP_ASM("bx lr");
}
