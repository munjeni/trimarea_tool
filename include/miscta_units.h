/*
 * Copyright 2009-2011 Sony Ericsson Mobile Communications AB.
 * Copyright (c) 2012-2013 Sony Mobile Communications AB.
 * All rights, including trade secret rights, reserved.
 */

/**
 * @file miscta_units.h
 *
 * MiscTA unit id's.
 *
 * May be included from other git's.
 *
 * These id's are maintained at:
 * https://wiki.sonyericsson.net/androiki/PLD_System_Core/Parameter_Handling/MiscTA_Parameter_List
 *
 * The defined names should match the names on the above wiki.
 *
 * @author Jens-Henrik Lindskov (jens-henrik.lindskov@sonyericsson.com)
 * @author Rickard Möller (rickard.moller@sonyericsson.com)
 */

#ifndef MISCTA_UNITS_H
#define MISCTA_UNITS_H

#define TA_FLA_FLA                       2002
#define TA_SIMLOCK_DATA                  2010
#define TA_DRM_DYN_DATA                  2040
#define TA_DRM_CUST                      2099
#define TA_SIMLOCK_SIGNATURE             2129
#define TA_SW_VERSION                    2202
#define TA_CUST_VERSION                  2205
#define TA_FS_VERSION                    2206
#define TA_S1_BOOT_VERSION               2207
#define TA_BUILD_TYPE                    2209
#define TA_PHONE_NAME                    2210
#define TA_AC_VERSION                    2212
#define TA_FLASH_STARTUP_SHUTDOWN_RESULT 2227
#define TA_CHECKPOINTS_ENABLED           2228
#define TA_CHECKPOINTS_REACHED           2229
#define TA_BOOT_CONFIG                   2301
#define TA_DISABLE_CHARGE_ONLY           2311
#define TA_DISABLE_USB_CHARGING          2312
#define TA_FOTA_STATUS                   2401
#define TA_FOTA_REPART                   2402
#define TA_VIRTUAL_UIM                   2460
#define TA_ENABLE_USB_ENG_PID            2470
#define TA_ENABLE_USB_DEBUGGING          2471
#define TA_VERSION_HISTORY               2475
#define TA_BATTERY_CAPACITY              2490
#define TA_DCMLAC_TRANSMISSION_CONFIG    2495
#define TA_DCMLAC_SEND_TIME              2496
#define TA_DCMLAC_UPLOAD_LOG_LATEST      2497
#define TA_DCMLAC_IDDCONFIG_SW_VERSION   2498
#define TA_SECURITY_GA_DATA              2501
#define TA_MASTER_RESET                  2550
#define TA_BANDCONFIG                    2551
#define TA_WIPE_DATA                     2552
#define TA_WLAN_ADDR_0                   2560
#define TA_WLAN_ADDR_1                   2561
#define TA_WLAN_ADDR_2                   2562
#define TA_WLAN_ADDR_3                   2563
#define TA_WLAN_TXPOWER_2_4G             2564
#define TA_WLAN_TXPOWER_5G_LOW           2565
#define TA_WLAN_TXPOWER_5G_MID           2566
#define TA_WLAN_TXPOWER_5G_HIGH          2567
#define TA_BD_ADDR                       2568
#define TA_MMS_USER_AGENT                2570
#define TA_MMSC_URL                      2571
#define TA_LCD_NVM_1                     2585
#define TA_LCD_NVM_2                     2586
#define TA_LCD_NVM_WRITE_COUNT           2587
#define TA_LCD_NVM_HWID                  2588
#define TA_RETAIL_DEMO_ACTIVE_STATUS     2590
#define TA_PHONE_USAGE_FLAG              2595
#define TA_NV_PREF_MODE_I                2601
#define TA_NV_LIFE_TIMER_G_I             2602
#define TA_NV_SERVICE_DOMAIN_PREF_I      2603
#define TA_SONY_SERVICE_ID               4899
#define TA_PP_SONY_ITP_PRODUCT_NO        4908
#define TA_PP_SONY_ITP_REV               4909
#define TA_NFC_CHIP_FW                   4952
#define TA_NFC_CHIP_VERSION              4953
#define TA_GYRO_CALIBRATED               4960
#define TA_MPU3050_CALIBRATION_DATA      4961
#define TA_MPU6050_CALIBRATION_DATA      4962
#define TA_ACCEL_CALIBRATION_DATA        4963
#define TA_GYRO_CALIBRATION_DATA         4964
#define TA_PROXIMITY_CALIBRATION_DATA    4970
#define TA_THERMAL_SHUTDOWN_COUNT        10022
#define TA_THERMAL_LAST_SHUTDOWN_1       10023
#define TA_THERMAL_LAST_SHUTDOWN_2       10024
#define TA_THERMAL_LAST_SHUTDOWN_3       10025
#define TA_FLASH_PROGRESS                10100
#define TA_FLIP_SLIDE_COUNTER            10200

#endif /* MISCTA_UNITS_H */
