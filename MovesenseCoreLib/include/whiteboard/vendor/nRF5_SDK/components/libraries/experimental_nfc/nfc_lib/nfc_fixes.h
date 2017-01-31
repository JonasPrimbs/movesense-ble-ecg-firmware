/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */
 
#ifndef NFC_FIXES_H__
#define NFC_FIXES_H__

#include <stdint.h>

/** @file
 * @defgroup nfc_fixes NFC fixes
 * @{
 * @ingroup nfc_library
 *
 * If you are using PCA10036 (part of nRF52 Preview Development Kit), 
 * you must define the macro HAL_NFC_ENGINEERING_A_FTPAN_WORKAROUND to apply 
 * workarounds for the following anomalies:
 * - 17. NFCT: The EVENTS_FIELDLOST is not generated
 * - 24. NFCT: The FIELDPRESENT register read is not reliable
 * - 27. NFCT: Triggering NFCT ACTIVATE task also activates the Rx easyDMA
 * - 45. NFCT: EasyDMA does not function properly while System ON CPU IDLE state
 * - 57. NFCT: NFC Modulation amplitude
 *
 * The current code contains a patch for anomaly 25 (NFCT: Reset value of 
 * SENSRES register is incorrect), so that it now works on Windows Phone.
 */
 
#ifdef BOARD_PCA10036 // assume nRF52832 chip in IC rev. Engineering A (QFAA-AA0, QFAA-AC0)
    #define HAL_NFC_ENGINEERING_A_FTPAN_WORKAROUND
#endif

/* Begin: Bugfix for FTPAN-45 */
#ifdef HAL_NFC_ENGINEERING_A_FTPAN_WORKAROUND
    extern volatile uint8_t m_nfc_active;
    #define NFC_NEED_MCU_RUN_STATE() (m_nfc_active != 0)
#else
    #define NFC_NEED_MCU_RUN_STATE() (0)
#endif
/* End: Bugfix for FTPAN-45 */

/** @} */
#endif /* NFC_FIXES_H__ */

