#include <stdlib.h>
#include "checkpoint_selector.h"

#include "checkpoint_util_mem.h"
#include "checkpoint_data.h"

__attribute__((section(".allocate_checkpoint_flags")))
char _checkpoint_data_allocate_checkpoint_ld;

static inline size_t data_size(void) {
  size_t data_size = (size_t)checkpoint_data_end - (size_t)checkpoint_data_start;
  return data_size;
}

static inline char *data_get_active_checkpoint(void) {
    char *cp;
    if (checkpoint_get_active_idx() == 0)
        cp = (char *)checkpoint_data_0_start;
    else
        cp = (char *)checkpoint_data_1_start;

    return cp;
}

static inline char *data_get_restore_checkpoint(void) {
    char *cp;
    if (checkpoint_get_restore_idx() == 0)
        cp = (char *)checkpoint_data_0_start;
    else
        cp = (char *)checkpoint_data_1_start;

    return cp;
}

/*
 * .data checkpoint and restore
 */
size_t checkpoint_data(void) {
  char* cp = data_get_active_checkpoint();
  char* data_ptr = (char*)checkpoint_data_start;
  size_t size = data_size();

  checkpoint_mem(cp, data_ptr, size);
  return size;
}

size_t restore_data(void) {
  char* cp = data_get_restore_checkpoint();
  char* data_ptr = (char*)checkpoint_data_start;
  size_t size = data_size();

  restore_mem(data_ptr, cp, size);
  return size;
}

/*
 * Used during the first boot
 */
void startup_clear_data(void) {
    char *data = (char *)&_edata_norestore;

    char *data_init = (char *)&_init_data;
    uint32_t start_offset = data - (char *)&_sdata;
    data_init = &data_init[start_offset];

    uint32_t size = (char *)&_edata - data;
    for (uint32_t i=0; i<size; i++) {
        data[i] = data_init[i];
    }
}
