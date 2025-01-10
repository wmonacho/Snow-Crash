/** @file
 *
 * Copyright (c) 2002-2005 Sam Leffler, Errno Consulting
 * Copyright (c) 2006 CACE Technologies, Davis (California)
 * All rights reserved.
 *
 * SPDX-License-Identifier: (BSD-3-Clause OR GPL-2.0-only)
 */

#ifndef _DOT11DECRYPT_UTIL_H
#define _DOT11DECRYPT_UTIL_H

#include "dot11decrypt_int.h"

void dot11decrypt_construct_aad(
    PDOT11DECRYPT_MAC_FRAME wh,
    uint8_t *aad,
    size_t *aad_len);

bool
dot11decrypt_prf(const uint8_t *key, size_t key_len,
                 const char *label,
                 const uint8_t *context, size_t context_len,
                 int hash_algo,
                 uint8_t *output, size_t output_len);

bool
dot11decrypt_kdf(const uint8_t *key, size_t key_len,
                 const char *label,
                 const uint8_t *context, size_t context_len,
                 int hash_algo,
                 uint8_t *output, size_t output_len);

bool
dot11decrypt_derive_pmk_r0(const uint8_t *xxkey, size_t xxkey_len,
                           const uint8_t *ssid, size_t ssid_len,
                           const uint8_t mdid[2],
                           const uint8_t *r0kh_id, size_t r0kh_id_len,
                           const uint8_t s0kh_id[DOT11DECRYPT_MAC_LEN],
                           int hash_algo,
                           uint8_t *pmk_r0,
                           size_t *pmk_r0_len,
                           uint8_t pmk_r0_name[16]);

bool
dot11decrypt_derive_pmk_r1(const uint8_t *pmk_r0, size_t pmk_r0_len,
                           const uint8_t *pmk_r0_name,
                           const uint8_t *r1kh_id, const uint8_t *s1kh_id,
                           int hash_algo,
                           uint8_t *pmk_r1, size_t *pmk_r1_len,
                           uint8_t *pmk_r1_name);

bool
dot11decrypt_derive_ft_ptk(const uint8_t *pmk_r1, size_t pmk_r1_len,
                           const uint8_t *pmk_r1_name,
                           const uint8_t *snonce, const uint8_t *anonce,
                           const uint8_t *bssid, const uint8_t *sta_addr,
                           int hash_algo,
                           uint8_t *ptk, const size_t ptk_len, uint8_t *ptk_name);
#endif /* _DOT11DECRYPT_UTIL_H */

/*
 * Editor modelines
 *
 * Local Variables:
 * c-basic-offset: 4
 * tab-width: 8
 * indent-tabs-mode: nil
 * End:
 *
 * ex: set shiftwidth=4 tabstop=8 expandtab:
 * :indentSize=4:tabSize=8:noTabs=true:
 */
