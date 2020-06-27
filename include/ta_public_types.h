/*
 * Copyright 2010 Sony Ericsson Mobile Communications AB.
 * Copyright 2012 Sony Mobile Communications AB.
 * All rights, including trade secret rights, reserved.
 */

/**
 * @file ta_public_types.h
 *
 * Public types and defines for all TA code.
 *
 * May be included from other git's.
 *
 * @copyright 2010, Sony Ericsson Mobile Communications AB
 *
 * @author Rickard Möller (rickard.moller@sonyericsson.com)
 */


#ifndef TA_PUBLIC_TYPES_H_
#define TA_PUBLIC_TYPES_H_


/*
 * MiscTA service status codes
 */
typedef enum
{
  MT_SUCCESS,        /* No error */
  MT_ERROR,          /* General error */
  MT_NOMEM,          /* Out of memory */
  MT_TAERR,          /* Trim area error */
  MT_TOOBIG,         /* Not enough room in buffer */
  MT_NOTFOUND,       /* Unit not found */
  MT_INVALPARAM,     /* Invalid parameters */
  MT_SAMEVAL         /* Internal */
} miscta_status_t;

/*
 * TA service status codes
 */
typedef enum
{
  TA_Success,
  TA_Error,          /* General error */
  TA_Version,        /* Non compliant version */
  TA_Memory,         /* Memory related error */
  TA_UnitNotFound,   /* Unit could not be found */
  TA_Permission,     /* TA opened to not support the operation */
  TA_Invalid,        /* TA is invalid. TA_Format may be required. */
  TA_Parameters,     /* Invalid parameters has been passed to the function. */
  TA_FlashMemory,    /* Flash related error. */
  TA_UnsupportedCmd, /* Not implemented/supported. */
  TA_Configuration,  /* Configuration error. */
  TA_Mismatch        /* Checksum or data mismatch. */
} TA_ErrCode_t;


/*
 * Defines for the default partitions to handle in the TA.
 */
#define TA_TRIM_PART 1 // Trim Data Partition
#define TA_MISC_PART 2 // Miscellaneous Data Partition (Non Trim Data)

/*
 * Defines to set up the mode to operate the TA in.
 */
#define TA_READ       0x1 //Read
#define TA_RAM_WRITE  0x2 //All writes are done to RAM. A call to TA_Flush is
                          //required to actually write data to flash. If the
                          //phone crash before TA_Flush has been successfully
                          //executed, all changes will be lost. This mode
                          //should not be used outside controlled environments
                          //such as a factory.
#define TA_SAFE_WRITE 0x4 //On every call to TA_WriteData, the unit will be
                          //written straight to flash. If the phone crash
                          //during a write, it will be recovered and either
                          //the old data, or the one written, will be present.
                          //This mode will cause a lot of erases/writes to
                          //flash.


/*
 * Defines for the MDM TA offset
 */
#define TA_FUSION_ITEM_OFFSET 0x80000000

/*
 * Raw TA service commands
 */
typedef enum
{
  TA_OP_OPEN,
  TA_OP_CLOSE,
  TA_OP_GET_SIZE,
  TA_OP_READ,
  TA_OP_WRITE,
  TA_OP_DEL,
  TA_OP_FORMAT,
  TA_OP_GETVERSION,
  TA_OP_GETMODE,
  TA_OP_NEXT,
  TA_OP_PRINT_INFO
} ta_raw_cmd_id_t;

/**********************************************************************
 *
 * MiscTA service commands:
 *
 * TA_MISCTA_READ
 * ==============
 * Reads a unit from MiscTA.
 *
 * data should point to a data buffer. size should be the size of the
 * buffer. If size is not sufficient then MT_TOOBIG is returned.
 * size is updated to the actual size of the unit. Note that a zero-sized
 * unit is possible.
 *
 *
 * TA_MISCTA_GET_SIZE
 * ==================
 * Retrieves the size of a unit in MiscTA.
 *
 * size is set to the size of the variable. Note that a zero-sized unit
 * is possible.
 *
 *
 * TA_MISCTA_WRITE
 * ===============
 * Writes a unit to MiscTA. The actual write may be performed directly or
 * at a later time, depending on recent activity. This function should be
 * used for variables such as run-time counters, logs or other data that
 * is usually updated more than once.
 *
 *
 * TA_MISCTA_WRITE_CRITICAL
 * ========================
 * Writes a unit to MiscTA. The actual write is performed directly. This
 * function should be used for variables that are critical such as a DRM
 * key, phone information, etc that is only written once and where it is
 * crucial that the data is not lost if e.g. power is removed right after.
 *
 * If several critical units should be written after each other then the
 * miscta_write_unit() function should be used instead to queue up the units
 * and at the end a call to miscta_write_flush() should be done to perform
 * the actual write.
 *
 * When this function is called then any variables written using the
 * miscta_write_unit()-function is also written.
 *
 *
 * TA_MISCTA_WRITE_FLUSH
 * =====================
 * Performs the write to MiscTA for any variables that were not written
 * directly by miscta_write_unit().
 *
 * It is NOT necessary to call this to have the data written. Any queued
 * variable is written when appropriate. This function is instead meant to
 * be used when writing several critical units. See miscta_write_unit_critical().
 *
 *
 * NOTE: To preserve the flash memory it is IMPORTANT that the correct
 * write-functions are used.
 *
 * Normally miscta_write_unit() should be used.
 * For a single critical unit miscta_write_unit_critical() should be used.
 * For several critical units then miscta_write_unit() should be used,
 * followed by miscta_write_flush().
 */
typedef enum
{
  TA_MISCTA_READ,
  TA_MISCTA_GET_SIZE,
  TA_MISCTA_WRITE,
  TA_MISCTA_WRITE_CRITICAL,
  TA_MISCTA_WRITE_FLUSH,
} ta_miscta_cmd_id_t;

#endif /* TA_PUBLIC_TYPES_H_ */
