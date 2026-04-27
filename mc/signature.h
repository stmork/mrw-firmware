/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	MPU signature control
**
*/

#ifndef SIGNATURE_H
#define SIGNATURE_H

#include <avr/io.h>
#include <avr/boot.h>

#ifndef boot_signature_byte_get
#ifdef SIGRD

#define __SPM_ENABLE       SPMEN
#define __BOOT_SIGROW_READ (_BV(__SPM_ENABLE) | _BV(SIGRD))

#define boot_signature_byte_get(addr) \
(__extension__({              \
      uint16_t __addr16 = (uint16_t)(addr); \
      uint8_t __result;             \
      __asm__ __volatile__          \
      (                     \
    "sts %1, %2\n\t"            \
    "lpm %0, Z" "\n\t"          \
    : "=r" (__result)           \
    : "i" (_SFR_MEM_ADDR(__SPM_REG)),   \
      "r" ((uint8_t) __BOOT_SIGROW_READ),   \
      "z" (__addr16)            \
      );                    \
      __result;                 \
}))

#endif
#endif

#endif
