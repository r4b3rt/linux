// SPDX-License-Identifier: GPL-2.0
/*
 * Normal 64-bit CRC calculation.
 *
 * This is a basic crc64 implementation following ECMA-182 specification,
 * which can be found from,
 * https://www.ecma-international.org/publications/standards/Ecma-182.htm
 *
 * Dr. Ross N. Williams has a great document to introduce the idea of CRC
 * algorithm, here the CRC64 code is also inspired by the table-driven
 * algorithm and detail example from this paper. This paper can be found
 * from,
 * http://www.ross.net/crc/download/crc_v3.txt
 *
 * crc64table[256] is the lookup table of a table-driven 64-bit CRC
 * calculation, which is generated by gen_crc64table.c in kernel build
 * time. The polynomial of crc64 arithmetic is from ECMA-182 specification
 * as well, which is defined as,
 *
 * x^64 + x^62 + x^57 + x^55 + x^54 + x^53 + x^52 + x^47 + x^46 + x^45 +
 * x^40 + x^39 + x^38 + x^37 + x^35 + x^33 + x^32 + x^31 + x^29 + x^27 +
 * x^24 + x^23 + x^22 + x^21 + x^19 + x^17 + x^13 + x^12 + x^10 + x^9 +
 * x^7 + x^4 + x + 1
 *
 * crc64nvmetable[256] uses the CRC64 polynomial from the NVME NVM Command Set
 * Specification and uses least-significant-bit first bit order:
 *
 * x^64 + x^63 + x^61 + x^59 + x^58 + x^56 + x^55 + x^52 + x^49 + x^48 + x^47 +
 * x^46 + x^44 + x^41 + x^37 + x^36 + x^34 + x^32 + x^31 + x^28 + x^26 + x^23 +
 * x^22 + x^19 + x^16 + x^13 + x^12 + x^10 + x^9 + x^6 + x^4 + x^3 + 1
 *
 * Copyright 2018 SUSE Linux.
 *   Author: Coly Li <colyli@suse.de>
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/crc64.h>
#include "crc64table.h"

MODULE_DESCRIPTION("CRC64 calculations");
MODULE_LICENSE("GPL v2");

u64 crc64_be_generic(u64 crc, const u8 *p, size_t len)
{
	while (len--)
		crc = (crc << 8) ^ crc64table[(crc >> 56) ^ *p++];
	return crc;
}
EXPORT_SYMBOL_GPL(crc64_be_generic);

u64 crc64_nvme_generic(u64 crc, const u8 *p, size_t len)
{
	while (len--)
		crc = (crc >> 8) ^ crc64nvmetable[(crc & 0xff) ^ *p++];
	return crc;
}
EXPORT_SYMBOL_GPL(crc64_nvme_generic);
