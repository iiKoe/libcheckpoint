#ifndef CHECKPOINT_ARCH_H_
#define CHECKPOINT_ARCH_H_

#include <stdint.h>

#include "checkpoint_arch_cfg.h"

typedef uint8_t lclock_t;

typedef uint32_t registers_t;

#define LIBCP_ASM __asm__ volatile

#endif /* CHECKPOINT_ARCH_H_ */
