#ifndef CHECKPOINT_REGISTERS_H_
#define CHECKPOINT_REGISTERS_H_

#include <stdlib.h>
#include "checkpoint_arch.h"
#include "checkpoint_selector.h"

#define CHECKPOINT_N_REGISTERS 17

// NVM Checkpoint
extern registers_t registers_checkpoint_nvm[2][CHECKPOINT_N_REGISTERS];

/* Special barrier instructions
 * http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0489c/CIHGHHIE.html
 */

extern volatile registers_t registers[CHECKPOINT_N_REGISTERS];
extern volatile registers_t *registers_checkpoint_ptr;
extern volatile registers_t checkpoint_restored_flag;

__attribute__((naked)) void checkpoint_registers_asm(void);
__attribute__((naked)) void restore_registers_asm(void);

#define checkpoint_restored() checkpoint_restored_flag

__attribute__((always_inline))
static inline void checkpoint_registers(void) {
    checkpoint_restored_flag = 0;
    registers_checkpoint_ptr = registers_checkpoint_nvm[checkpoint_get_active_idx()];
    checkpoint_registers_asm();
}

void restore_registers(void);

#endif /* CHECKPOINT_REGISTERS_H_ */
