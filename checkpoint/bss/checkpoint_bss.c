#include <stdlib.h>
#include "checkpoint_selector.h"

#include "checkpoint_util_mem.h"
#include "checkpoint_bss.h"

__attribute__((section(".allocate_checkpoint_flags")))
char _checkpoint_bss_allocate_checkpoint_ld;

static inline size_t bss_size(void) {
  size_t bss_size = (size_t)checkpoint_bss_end - (size_t)checkpoint_bss_start;
  return bss_size;
}

static inline char *bss_get_active_checkpoint(void) {
    char *cp;
    if (checkpoint_get_active_idx() == 0)
        cp = (char *)checkpoint_bss_0_start;
    else
        cp = (char *)checkpoint_bss_1_start;

    return cp;
}

static inline char *bss_get_restore_checkpoint(void) {
    char *cp;
    if (checkpoint_get_restore_idx() == 0)
        cp = (char *)checkpoint_bss_0_start;
    else
        cp = (char *)checkpoint_bss_1_start;

    return cp;
}

/*
 * .bss checkpoint and restore
 */
void checkpoint_bss(void) {
  char* cp = bss_get_active_checkpoint();
  char* bss_ptr = (char*)checkpoint_bss_start;
  size_t size = bss_size();

  checkpoint_mem(cp, bss_ptr, size);
}

void restore_bss(void) {
  char* cp = bss_get_restore_checkpoint();
  char* bss_ptr = (char*)checkpoint_bss_start;
  size_t size = bss_size();

  restore_mem(bss_ptr, cp, size);
}

/*
 * Used during the first boot
 */
void startup_clear_bss(void) {
    char *bss = (char *)&_ebss_norestore;
    uint32_t size = (char *)&_ebss - bss;
    for (uint32_t i=0; i<size; i++) {
        bss[i] = 0;
    }
}
