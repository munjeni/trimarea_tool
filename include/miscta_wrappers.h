/*********************************************************************
 *  ____                      _____      _                           *
 * / ___|  ___  _ __  _   _  | ____|_ __(_) ___ ___ ___  ___  _ __   *
 * \___ \ / _ \| '_ \| | | | |  _| | '__| |/ __/ __/ __|/ _ \| '_ \  *
 *  ___) | (_) | | | | |_| | | |___| |  | | (__\__ \__ \ (_) | | | | *
 * |____/ \___/|_| |_|\__, | |_____|_|  |_|\___|___/___/\___/|_| |_| *
 *                    |___/                                          *
 *                                                                   *
 *********************************************************************
 * Copyright 2011-2012 Sony Ericsson Mobile Communications AB.       *
 * All rights, including trade secret rights, reserved.              *
 *********************************************************************/

/**
 * @file miscta_wrappers.h
 *
 * @copyright 2011-2012, Sony Ericsson Mobile Communications AB
 *
 * @author Jens-Henrik Lindskov (jens-henrik.lindskov@sonyericsson.com)
 */

#ifndef MISCTA_WRAPPERS_H
#define MISCTA_WRAPPERS_H

/****************************************************************************
 * MiscTA unit wrappers
 ****************************************************************************/

/* buffer max size */
#define MISC_TA_SW_VER_SIZE              64
#define MISC_TA_CUST_VER_SIZE            64
#define MISC_TA_AC_SIZE                  64
#define MISC_TA_FS_VER_SIZE              64
#define MISC_TA_S1_BOOT_VER_SIZE         64
#define MISC_TA_PHONE_NAME_SIZE          64
#define MISC_TA_PP_SEMC_ITP_PROD_NO_SIZE 22
#define MISC_TA_PP_SEMC_ITP_REV_SIZE     7
#define MISC_TA_DRM_CUST_SIZE            22

#define FOTA_DEFAULT                     0xF
#define FOTA_SUCCESSFUL                  0x0
#define FOTA_UNDEFINED_ERROR             0x1
#define FOTA_UPDATE_FAILED               0x2
#define FOTA_PACKAGE_VALIDATION_FAILED   0x3
#define FOTA_DEVICE_OUT_OF_MEMORY        0x4
#define FOTA_PACKAGE_DEVICE_MISMATCH     0x5

/* TA_BOOT_CONFIG partition ids */
#define REBOOT_MODE_MARM_PARTID_MODEM     0x00000005
#define REBOOT_MODE_AARM_PARTID_APPS      0x00000003
#define REBOOT_MODE_ADSP_PARTID_ADSP      0x00000008

#define BOOTCFG_MAGIC0 0xac
#define BOOTCFG_MAGIC1 0xbd
#define BOOTCFG_STARTUPREASON 0x81
#define BOOTCFG_PARTITION_OVERRIDE 0x82

/* ta_boot_config structure */
typedef struct _ta_boot_config_t {
	unsigned long startup_reason;
	unsigned long marm_partition_id;
	unsigned long aarm_partition_id;
	unsigned long adsp_partition_id;
} ta_boot_config_t;

/* function prototype */
int miscta_read_ta_sw_version(char *buf, int size);
int miscta_write_ta_sw_version(const char *buf, int size);
int miscta_read_ta_cust_version(char *buf, int size);
int miscta_write_ta_cust_version(const char *buf, int size);
int miscta_read_ta_ac_version(char *buf, int size);
int miscta_read_ta_fs_version(char *buf, int size);
int miscta_write_ta_fs_version(const char *buf, int size);
int miscta_read_ta_s1_boot_version(char *buf, int size);
int miscta_write_ta_build_type(char type);
int miscta_read_ta_phone_name(char *buf, int size);
int miscta_write_ta_phone_name(const char *buf, int size);
#ifdef MISCTA_BOOTCONF_V0
int miscta_read_ta_boot_config(ta_boot_config_t *data);
int miscta_write_ta_boot_config(const ta_boot_config_t *data);
#endif
int miscta_read_ta_startupreason(unsigned int *reason);
int miscta_write_ta_startupreason(unsigned int reason);
int miscta_read_ta_pp_semc_itp_product_no(char *buf, int size);
int miscta_read_ta_pp_semc_itp_rev(char *buf, int size);
int miscta_read_ta_fota_status(unsigned long *status);
int miscta_write_ta_fota_status(unsigned long status);
int miscta_read_ta_flash_startup_shutdown_result(unsigned long *flag);
int miscta_write_ta_flash_startup_shutdown_result(unsigned long flag);
int miscta_read_ta_fla_fla(char *flag);
int miscta_write_ta_fla_fla(char flag);
int miscta_read_ta_drm_cust(void *buf, int size);
int miscta_write_ta_drm_cust(const void *buf, int size);
int clear_ua_status(void);
int check_ua_status(void);

#endif /* MISCTA_WRAPPERS_H */

