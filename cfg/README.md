# Example Configurations

## Readme is WIP

These are some example configurations that will have to be incorporated into your
project to make libcheckpoint work.

## `linkerscript.ld`
The linker script is heavily modified to aid libcheckpoint because libcheckpoint does a lot of memory shuffling so certain addresses need to be exposed. Additionally room needs to be reserved in the non-volatile memory to hold the `.bss` and `.data` checkpoint (these are known size at link time).

A restore stack is needed to not overwrite the stack when a restore operation is happening.
This might not be the case in the future, but for that I need to know **for sure** that I can avoid the compiler
doing stack operations in the restore procedure after the stack is restored. For now, this is best.

The files `exclude_checkpoint_bss.ld` and `exclude_checkpoint_data.ld` are included in the linker script and are used to specify global variables that should be excluded from the checkpoint. An alternative is to use the macros `CHECKPOINT_EXCLUDE_DATA` and `CHECKPOINT_EXCLUDE_BSS` (has the same result).

## `startup.c`
This the first code that runs on the system, this file might not directly
be usable for your project. But the `Reset_Handler` (or other entry point) needs to be configured
to **not** always clear the memory (as some will be restored when the checkpoint is).
This modified startup code only initializes the `.bss` and `.data` of the sections that will not be restored later by a checkpoint.
If this is the first boot (i.e., there is no checkpoint to restore) then libcheckpoint will initialize the remaining parts of both sections.

