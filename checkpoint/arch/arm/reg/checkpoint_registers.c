#include <stdint.h>

#include "checkpoint.h"
#include "checkpoint_selector.h"
#include "checkpoint_util_mem.h"
#include "checkpoint_arch.h"
#include "checkpoint_registers.h"

CHECKPOINT_EXCLUDE_BSS volatile registers_t *registers_checkpoint_ptr;
CHECKPOINT_EXCLUDE_DATA volatile registers_t checkpoint_restored_flag = 0;

void restore_registers(void) {
    checkpoint_restored_flag = 1;
    registers_checkpoint_ptr = registers_checkpoint_nvm[checkpoint_get_restore_idx()];
    restore_registers_asm();
}

__attribute__((naked))
void checkpoint_registers_asm(void) {
    LIBCP_ASM("cpsid i"); // disable interrupts
    LIBCP_ASM("push {r7}");
    LIBCP_ASM("ldr r7, =registers_checkpoint_ptr");
    LIBCP_ASM("ldr r7, [r7]");

    LIBCP_ASM("stmia r7!, {r0-r6}");
    LIBCP_ASM("ldr r0, [sp]");
    LIBCP_ASM("stmia r7!, {r0}");
    LIBCP_ASM("add sp, #4");

    LIBCP_ASM("mov r0, r8");
    LIBCP_ASM("mov r1, r9");
    LIBCP_ASM("mov r2, r10");
    LIBCP_ASM("mov r3, r11");
    LIBCP_ASM("mov r4, r12");

    LIBCP_ASM("stmia r7!, {r0-r4}");

    LIBCP_ASM("mov r0, sp");
    LIBCP_ASM("mov r1, lr");
    LIBCP_ASM("stmia r7!, {r0-r1}");

    /* Restore the low registers registers */
    LIBCP_ASM("sub r7, #60"); // point to r0-r4

    LIBCP_ASM("ldmia r7!, {r0-r4}");
    LIBCP_ASM("ldr r7, [r7, #8]");

    LIBCP_ASM("cpsie i"); // enable interrupts
    LIBCP_ASM("bx lr");
}

__attribute__((naked))
void restore_registers_asm(void) {
    // We don't care about the current registers here, we will overwrite them
    LIBCP_ASM("cpsid i"); // disable interrupts

    LIBCP_ASM("ldr r7, =registers_checkpoint_ptr");
    LIBCP_ASM("ldr r7, [r7]");

    LIBCP_ASM("add r7, #32"); // point to r8-r12, sp, lr

    LIBCP_ASM("ldmia r7, {r0-r6}"); // load r8-r12, sp, lr into r0-r6
    LIBCP_ASM("mov r8, r0");
    LIBCP_ASM("mov r9, r1");
    LIBCP_ASM("mov r10, r2");
    LIBCP_ASM("mov r11, r3");
    LIBCP_ASM("mov r12, r4");

    LIBCP_ASM("mov sp, r5");
    LIBCP_ASM("mov lr, r6");

    LIBCP_ASM("sub r7, #32"); // point to r0-r7

    LIBCP_ASM("ldmia r7, {r0-r7}"); // load r0-r7 into r0-r7

    LIBCP_ASM("cpsie i"); // enable interrupts
    LIBCP_ASM("bx lr");
}
