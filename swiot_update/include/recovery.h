

#ifndef __UNPACK_H
#define __UNPACK_H


#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

struct file_match {
    char up_file[32];
    char dev_file[32];
};

#define BOOTMODE_RECOVERY "boot_recovery"
#define BOOTMODE_FLUSHDATA "boot_flushdata"


#define BOOTSTATE_NONE "none"
#define BOOTSTATE_READY "recovery_ready"
#define BOOTSTATE_RUN "recovery_run"
#define BOOTSTATE_OK "recovery_ok"
#define BOOTSTATE_ERROR "recovery_error"

enum {
    BOOTSTATE_RES_OK,
    BOOTSTATE_RES_ERROR,
}BOOTSTATE;

struct boot_cmd {
    char boot_mode[32];
    char recovery_path[256];
    char recovery_state[32];
};

extern int get_recovery_cmd_status(struct boot_cmd *cmd);
extern int set_recovery_cmd_status(struct boot_cmd *cmd);

extern int set_boot_flush_data();
extern int get_boot_flush_data();
extern int nand_write(const char *device_name,  const int mtd_offset, uint8_t *file_buf, uint64_t len);
extern int nand_erase(const char *devicename, const int offset);


extern void recovery_new_img(char *update_path);

#ifdef __cplusplus
}
#endif

#endif
