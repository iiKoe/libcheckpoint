#include <stdint.h>

#include "nvm.h"

/*
 * We require the compiler to optimize this otherwise we waist quite a lot of
 * .text (flash)
 */
#ifndef __clang__
#pragma GCC push_options
#pragma GCC optimize "-O2"
#endif

#include "code_id.h"

#ifndef COMPILE_TIME_CODE_ID
/* If no compile time code ID is provided, we attempt to make one */
#include "compile-time/compile_time.h"
#define COMPILE_TIME_CODE_ID UNIX_TIMESTAMP
#else
#endif

code_id_t code_id_is_new(void) {
  if (code_id != COMPILE_TIME_CODE_ID) {
    return COMPILE_TIME_CODE_ID;
  }
  return 0;
}

code_id_t code_id_update(void) {
    code_id = COMPILE_TIME_CODE_ID;
    return code_id;
}

#ifndef __clang__
#pragma GCC pop_options
#endif
