#include "recovery.h"
#include "swdownload.h"


void recovery_new_img(char *update_path) {
 
  struct boot_cmd cmd;

  memset(&cmd, 0, sizeof(cmd));

  strncpy(cmd.boot_mode, BOOTMODE_RECOVERY, strlen(BOOTMODE_RECOVERY));

  strncpy(cmd.recovery_path, update_path, strlen(update_path));

  strncpy(cmd.recovery_state, BOOTSTATE_READY, strlen(BOOTSTATE_READY));

  set_recovery_cmd_status(&cmd);

  system("reboot");
}