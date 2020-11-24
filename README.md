# Checkpoint Library
## Work in progress!

The checkpoint library consists of a simple API described below.

```
/* Invalidate all checkpoints */
void checkpoint_restore_invalidate()

/* Check if there is a restore availible */
bool checkpoint_restore_available()

/* Setup the checkpoint machanism (each boot) */
void checkpoint_setup()

/* Perform one-time setup (first boot only) */
void checkpoint_onetime_setup()

/* Create a checkpoint, it is also the point at which a restore continues
Returns 0 if it is a checkpoint
Returns 1 if it is a restore */
int checkpoint()

/* Restore the most recent checkpoint
Will continue in the code at the checkpoint() call with return value 1*/
void checkpoint_restore()
```

The checkpoint core has to be configured as is demonstrated in [`example_checkpoint_content.h`](/cfg/example_checkpoint_content.h).
The defines configured here will be used in the checkpoint API and decide what happens during the respective operations.

The available hooks in [`example_checkpoint_content.h`](/cfg/example_checkpoint_content.h) are:

- `CHECKPOINT_SETUP_CONTENT`: Includes all actions to be taken only once, i.e. during the first time boot.
- `CHECKPOINT_CONTENT`: Includes all actions to be taken during a checkpoint.
- `CHECKPOINT_RESTORE_CONTENT`: Includes all actions to be taken during a restore.
- `POST_CHECKPOINT_CONTENT`: Includes all actions to be taken after a successful checkpoint is completed.
- `POST_CHECKPOINT_AND_RESTORE_CONTENT`: Includes all actions to be taken after a successful restore **and** a successful checkpoint.

