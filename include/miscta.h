/*********************************************************************
 *  ____                      _____      _                           *
 * / ___|  ___  _ __  _   _  | ____|_ __(_) ___ ___ ___  ___  _ __   *
 * \___ \ / _ \| '_ \| | | | |  _| | '__| |/ __/ __/ __|/ _ \| '_ \  *
 *  ___) | (_) | | | | |_| | | |___| |  | | (__\__ \__ \ (_) | | | | *
 * |____/ \___/|_| |_|\__, | |_____|_|  |_|\___|___/___/\___/|_| |_| *
 *                    |___/                                          *
 *                                                                   *
 *********************************************************************
 * Copyright 2009 Sony Ericsson Mobile Communications AB.            *
 * All rights, including trade secret rights, reserved.              *
 *********************************************************************/

/**
 * @file miscta.h
 *
 * @brief MiscTA access library.
 *
 * May be included from other git's.
 *
 * @copyright 2009, Sony Ericsson Mobile Communications AB
 *
 * @author Jens-Henrik Lindskov (jens-henrik.lindskov@sonyericsson.com)
 */

#ifndef MISCTA_H
#define MISCTA_H

#include <stdint.h>
#include "ta_public_types.h"

/*
 * Reads a unit from MiscTA.
 *
 * size should contain the size of buf. If size is not sufficient then
 * MT_TOOBIG is returned.
 * On MT_SUCCESS the size is updated to the actual size of the unit.
 * Note that a zero-sized unit is possible.
 */
miscta_status_t
miscta_read_unit(uint32_t id, void *buf, uint32_t *size);


/*
 * Retrieves the size of a unit in MiscTA.
 *
 * On MT_SUCCESS the size is set to the size of the unit.
 * Note that a zero-sized unit is possible.
 */
miscta_status_t
miscta_get_unit_size(uint32_t id, uint32_t *size);



/*
 * NOTE: To preserve the flash memory it is IMPORTANT that the correct
 * write-functions are used.
 *
 * Normally miscta_write_unit() should be used.
 * For a single critical unit miscta_write_unit_critical() should be used.
 * For several critical units then miscta_write_unit() should be used,
 * followed by miscta_write_flush().
 */

/*
 * Writes a unit to MiscTA. The actual write may be performed directly or
 * at a later time, depending on recent activity. This function should be
 * used for units such as run-time counters, logs or other data that
 * is usually updated more than once.
 */
miscta_status_t
miscta_write_unit(uint32_t id, const void *buf, uint32_t size);


/*
 * Writes a unit to MiscTA. The actual write is performed directly. This
 * function should be used for units that are critical such as a DRM
 * key, phone information, etc that is only written once and where it is
 * crucial that the data is not lost if e.g. power is removed right after.
 *
 * If several critical units should be written after each other then the
 * miscta_write_unit() function should be used instead to queue up the units
 * and at the end a call to miscta_write_flush() should be done to perform
 * the actual write.
 *
 * When this function is called then any units previously written using the
 * miscta_write_unit()-function is also written.
 */
miscta_status_t
miscta_write_unit_critical(uint32_t id, const void *buf, uint32_t size);


/*
 * Performs the write to MiscTA for any units that were not written
 * directly by miscta_write_unit().
 *
 * It is NOT necessary to call this to have the data written. Any queued
 * unit is written when appropriate. This function is instead meant to
 * be used when writing several critical units. See miscta_write_unit_critical().
 */
miscta_status_t
miscta_write_flush(void);


#endif /* MISCTA_H */
