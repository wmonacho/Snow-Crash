/* crc32.c
 * CRC-32 routine
 *
 * Wireshark - Network traffic analyzer
 * By Gerald Combs <gerald@wireshark.org>
 * Copyright 1998 Gerald Combs
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Credits:
 *
 * Table from Solomon Peachy
 * Routine from Chris Waters
 */

#include "config.h"

#include <wsutil/crc32.h>

#ifdef HAVE_ZLIBNG
#include <zlib-ng.h>
#else
#ifdef HAVE_ZLIB
#define ZLIB_CONST
#include <zlib.h>
#endif /* HAVE_ZLIB */
#endif

#define CRC32_ACCUMULATE(c,d,table) (c=(c>>8)^(table)[(c^(d))&0xFF])

/*****************************************************************/
/*                                                               */
/* CRC32C LOOKUP TABLE                                           */
/* +++================                                           */
/* The following CRC lookup table was generated automagically    */
/* by the Rocksoft^tm Model CRC Algorithm Table Generation       */
/* Program V1.0 using the following model parameters:            */
/*                                                               */
/*    Width   : 4 bytes.                                         */
/*    Poly    : 0x1EDC6F41L                                      */
/*    Reverse : true.                                            */
/*                                                               */
/* For more information on the Rocksoft^tm Model CRC Algorithm,  */
/* see the document titled "A Painless Guide to CRC Error        */
/* Detection Algorithms" by Ross Williams                        */
/* (ross@guest.adelaide.edu.au.). This document is likely to be  */
/* in the FTP archive "ftp.adelaide.edu.au/pub/rocksoft".        */
/*                                                               */
/*****************************************************************/
#define CRC32C(c,d) CRC32_ACCUMULATE(c,d,crc32c_table)

static const uint32_t crc32c_table[256] = {
		0x00000000U, 0xF26B8303U, 0xE13B70F7U, 0x1350F3F4U, 0xC79A971FU,
		0x35F1141CU, 0x26A1E7E8U, 0xD4CA64EBU, 0x8AD958CFU, 0x78B2DBCCU,
		0x6BE22838U, 0x9989AB3BU, 0x4D43CFD0U, 0xBF284CD3U, 0xAC78BF27U,
		0x5E133C24U, 0x105EC76FU, 0xE235446CU, 0xF165B798U, 0x030E349BU,
		0xD7C45070U, 0x25AFD373U, 0x36FF2087U, 0xC494A384U, 0x9A879FA0U,
		0x68EC1CA3U, 0x7BBCEF57U, 0x89D76C54U, 0x5D1D08BFU, 0xAF768BBCU,
		0xBC267848U, 0x4E4DFB4BU, 0x20BD8EDEU, 0xD2D60DDDU, 0xC186FE29U,
		0x33ED7D2AU, 0xE72719C1U, 0x154C9AC2U, 0x061C6936U, 0xF477EA35U,
		0xAA64D611U, 0x580F5512U, 0x4B5FA6E6U, 0xB93425E5U, 0x6DFE410EU,
		0x9F95C20DU, 0x8CC531F9U, 0x7EAEB2FAU, 0x30E349B1U, 0xC288CAB2U,
		0xD1D83946U, 0x23B3BA45U, 0xF779DEAEU, 0x05125DADU, 0x1642AE59U,
		0xE4292D5AU, 0xBA3A117EU, 0x4851927DU, 0x5B016189U, 0xA96AE28AU,
		0x7DA08661U, 0x8FCB0562U, 0x9C9BF696U, 0x6EF07595U, 0x417B1DBCU,
		0xB3109EBFU, 0xA0406D4BU, 0x522BEE48U, 0x86E18AA3U, 0x748A09A0U,
		0x67DAFA54U, 0x95B17957U, 0xCBA24573U, 0x39C9C670U, 0x2A993584U,
		0xD8F2B687U, 0x0C38D26CU, 0xFE53516FU, 0xED03A29BU, 0x1F682198U,
		0x5125DAD3U, 0xA34E59D0U, 0xB01EAA24U, 0x42752927U, 0x96BF4DCCU,
		0x64D4CECFU, 0x77843D3BU, 0x85EFBE38U, 0xDBFC821CU, 0x2997011FU,
		0x3AC7F2EBU, 0xC8AC71E8U, 0x1C661503U, 0xEE0D9600U, 0xFD5D65F4U,
		0x0F36E6F7U, 0x61C69362U, 0x93AD1061U, 0x80FDE395U, 0x72966096U,
		0xA65C047DU, 0x5437877EU, 0x4767748AU, 0xB50CF789U, 0xEB1FCBADU,
		0x197448AEU, 0x0A24BB5AU, 0xF84F3859U, 0x2C855CB2U, 0xDEEEDFB1U,
		0xCDBE2C45U, 0x3FD5AF46U, 0x7198540DU, 0x83F3D70EU, 0x90A324FAU,
		0x62C8A7F9U, 0xB602C312U, 0x44694011U, 0x5739B3E5U, 0xA55230E6U,
		0xFB410CC2U, 0x092A8FC1U, 0x1A7A7C35U, 0xE811FF36U, 0x3CDB9BDDU,
		0xCEB018DEU, 0xDDE0EB2AU, 0x2F8B6829U, 0x82F63B78U, 0x709DB87BU,
		0x63CD4B8FU, 0x91A6C88CU, 0x456CAC67U, 0xB7072F64U, 0xA457DC90U,
		0x563C5F93U, 0x082F63B7U, 0xFA44E0B4U, 0xE9141340U, 0x1B7F9043U,
		0xCFB5F4A8U, 0x3DDE77ABU, 0x2E8E845FU, 0xDCE5075CU, 0x92A8FC17U,
		0x60C37F14U, 0x73938CE0U, 0x81F80FE3U, 0x55326B08U, 0xA759E80BU,
		0xB4091BFFU, 0x466298FCU, 0x1871A4D8U, 0xEA1A27DBU, 0xF94AD42FU,
		0x0B21572CU, 0xDFEB33C7U, 0x2D80B0C4U, 0x3ED04330U, 0xCCBBC033U,
		0xA24BB5A6U, 0x502036A5U, 0x4370C551U, 0xB11B4652U, 0x65D122B9U,
		0x97BAA1BAU, 0x84EA524EU, 0x7681D14DU, 0x2892ED69U, 0xDAF96E6AU,
		0xC9A99D9EU, 0x3BC21E9DU, 0xEF087A76U, 0x1D63F975U, 0x0E330A81U,
		0xFC588982U, 0xB21572C9U, 0x407EF1CAU, 0x532E023EU, 0xA145813DU,
		0x758FE5D6U, 0x87E466D5U, 0x94B49521U, 0x66DF1622U, 0x38CC2A06U,
		0xCAA7A905U, 0xD9F75AF1U, 0x2B9CD9F2U, 0xFF56BD19U, 0x0D3D3E1AU,
		0x1E6DCDEEU, 0xEC064EEDU, 0xC38D26C4U, 0x31E6A5C7U, 0x22B65633U,
		0xD0DDD530U, 0x0417B1DBU, 0xF67C32D8U, 0xE52CC12CU, 0x1747422FU,
		0x49547E0BU, 0xBB3FFD08U, 0xA86F0EFCU, 0x5A048DFFU, 0x8ECEE914U,
		0x7CA56A17U, 0x6FF599E3U, 0x9D9E1AE0U, 0xD3D3E1ABU, 0x21B862A8U,
		0x32E8915CU, 0xC083125FU, 0x144976B4U, 0xE622F5B7U, 0xF5720643U,
		0x07198540U, 0x590AB964U, 0xAB613A67U, 0xB831C993U, 0x4A5A4A90U,
		0x9E902E7BU, 0x6CFBAD78U, 0x7FAB5E8CU, 0x8DC0DD8FU, 0xE330A81AU,
		0x115B2B19U, 0x020BD8EDU, 0xF0605BEEU, 0x24AA3F05U, 0xD6C1BC06U,
		0xC5914FF2U, 0x37FACCF1U, 0x69E9F0D5U, 0x9B8273D6U, 0x88D28022U,
		0x7AB90321U, 0xAE7367CAU, 0x5C18E4C9U, 0x4F48173DU, 0xBD23943EU,
		0xF36E6F75U, 0x0105EC76U, 0x12551F82U, 0xE03E9C81U, 0x34F4F86AU,
		0xC69F7B69U, 0xD5CF889DU, 0x27A40B9EU, 0x79B737BAU, 0x8BDCB4B9U,
		0x988C474DU, 0x6AE7C44EU, 0xBE2DA0A5U, 0x4C4623A6U, 0x5F16D052U,
		0xAD7D5351U };

/*
 * Table for the AUTODIN/HDLC/802.x CRC.
 *
 * Polynomial is
 *
 *  x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11 + x^10 + x^8 +
 *      x^7 + x^5 + x^4 + x^2 + x + 1
 */
static const uint32_t crc32_ccitt_table[256] = {
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419,
	0x706af48f, 0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4,
	0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07,
	0x90bf1d91, 0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de,
	0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7, 0x136c9856,
	0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
	0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4,
	0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
	0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3,
	0x45df5c75, 0xdcd60dcf, 0xabd13d59, 0x26d930ac, 0x51de003a,
	0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599,
	0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
	0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190,
	0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f,
	0x9fbfe4a5, 0xe8b8d433, 0x7807c9a2, 0x0f00f934, 0x9609a88e,
	0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
	0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed,
	0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
	0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3,
	0xfbd44c65, 0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2,
	0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a,
	0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5,
	0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa, 0xbe0b1010,
	0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17,
	0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6,
	0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615,
	0x73dc1683, 0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8,
	0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1, 0xf00f9344,
	0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
	0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a,
	0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
	0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1,
	0xa6bc5767, 0x3fb506dd, 0x48b2364b, 0xd80d2bda, 0xaf0a1b4c,
	0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef,
	0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
	0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe,
	0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31,
	0x2cd99e8b, 0x5bdeae1d, 0x9b64c2b0, 0xec63f226, 0x756aa39c,
	0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
	0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b,
	0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
	0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1,
	0x18b74777, 0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
	0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45, 0xa00ae278,
	0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7,
	0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc, 0x40df0b66,
	0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605,
	0xcdd70693, 0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8,
	0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b,
	0x2d02ef8d
};

/*
 * Table for the MPEG-2 CRC.
 *
 * Polynomial is
 *
 *  x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11 + x^10 + x^8 +
 *      x^7 + x^5 + x^4 + x^2 + x + 1
 *
 * (which is the same polynomial as the one above us).
 *
 * NOTE: this is also used for ATM AAL5.
 */
static const uint32_t crc32_mpeg2_table[256] = {
		0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9, 0x130476dc, 0x17c56b6b,
		0x1a864db2, 0x1e475005, 0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61,
		0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd, 0x4c11db70, 0x48d0c6c7,
		0x4593e01e, 0x4152fda9, 0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75,
		0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011, 0x791d4014, 0x7ddc5da3,
		0x709f7b7a, 0x745e66cd, 0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039,
		0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5, 0xbe2b5b58, 0xbaea46ef,
		0xb7a96036, 0xb3687d81, 0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,
		0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49, 0xc7361b4c, 0xc3f706fb,
		0xceb42022, 0xca753d95, 0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1,
		0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d, 0x34867077, 0x30476dc0,
		0x3d044b19, 0x39c556ae, 0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,
		0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16, 0x018aeb13, 0x054bf6a4,
		0x0808d07d, 0x0cc9cdca, 0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde,
		0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02, 0x5e9f46bf, 0x5a5e5b08,
		0x571d7dd1, 0x53dc6066, 0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,
		0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e, 0xbfa1b04b, 0xbb60adfc,
		0xb6238b25, 0xb2e29692, 0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6,
		0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a, 0xe0b41de7, 0xe4750050,
		0xe9362689, 0xedf73b3e, 0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2,
		0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686, 0xd5b88683, 0xd1799b34,
		0xdc3abded, 0xd8fba05a, 0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637,
		0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb, 0x4f040d56, 0x4bc510e1,
		0x46863638, 0x42472b8f, 0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,
		0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47, 0x36194d42, 0x32d850f5,
		0x3f9b762c, 0x3b5a6b9b, 0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff,
		0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623, 0xf12f560e, 0xf5ee4bb9,
		0xf8ad6d60, 0xfc6c70d7, 0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b,
		0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f, 0xc423cd6a, 0xc0e2d0dd,
		0xcda1f604, 0xc960ebb3, 0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7,
		0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b, 0x9b3660c6, 0x9ff77d71,
		0x92b45ba8, 0x9675461f, 0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3,
		0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640, 0x4e8ee645, 0x4a4ffbf2,
		0x470cdd2b, 0x43cdc09c, 0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8,
		0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24, 0x119b4be9, 0x155a565e,
		0x18197087, 0x1cd86d30, 0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
		0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088, 0x2497d08d, 0x2056cd3a,
		0x2d15ebe3, 0x29d4f654, 0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0,
		0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c, 0xe3a1cbc1, 0xe760d676,
		0xea23f0af, 0xeee2ed18, 0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4,
		0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0, 0x9abc8bd5, 0x9e7d9662,
		0x933eb0bb, 0x97ffad0c, 0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668,
		0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4
};

/* This table was compiled using the polynom: 0x0AA725CF*/
static const uint32_t crc32_0AA725CF_reverse[] = {
		0x00000000U, 0xCEAA95CEU, 0x7A1CE13DU, 0xB4B674F3U,
		0xF439C27AU, 0x3A9357B4U, 0x8E252347U, 0x408FB689U,
		0x0F3A4E55U, 0xC190DB9BU, 0x7526AF68U, 0xBB8C3AA6U,
		0xFB038C2FU, 0x35A919E1U, 0x811F6D12U, 0x4FB5F8DCU,
		0x1E749CAAU, 0xD0DE0964U, 0x64687D97U, 0xAAC2E859U,
		0xEA4D5ED0U, 0x24E7CB1EU, 0x9051BFEDU, 0x5EFB2A23U,
		0x114ED2FFU, 0xDFE44731U, 0x6B5233C2U, 0xA5F8A60CU,
		0xE5771085U, 0x2BDD854BU, 0x9F6BF1B8U, 0x51C16476U,
		0x3CE93954U, 0xF243AC9AU, 0x46F5D869U, 0x885F4DA7U,
		0xC8D0FB2EU, 0x067A6EE0U, 0xB2CC1A13U, 0x7C668FDDU,
		0x33D37701U, 0xFD79E2CFU, 0x49CF963CU, 0x876503F2U,
		0xC7EAB57BU, 0x094020B5U, 0xBDF65446U, 0x735CC188U,
		0x229DA5FEU, 0xEC373030U, 0x588144C3U, 0x962BD10DU,
		0xD6A46784U, 0x180EF24AU, 0xACB886B9U, 0x62121377U,
		0x2DA7EBABU, 0xE30D7E65U, 0x57BB0A96U, 0x99119F58U,
		0xD99E29D1U, 0x1734BC1FU, 0xA382C8ECU, 0x6D285D22U,
		0x79D272A8U, 0xB778E766U, 0x03CE9395U, 0xCD64065BU,
		0x8DEBB0D2U, 0x4341251CU, 0xF7F751EFU, 0x395DC421U,
		0x76E83CFDU, 0xB842A933U, 0x0CF4DDC0U, 0xC25E480EU,
		0x82D1FE87U, 0x4C7B6B49U, 0xF8CD1FBAU, 0x36678A74U,
		0x67A6EE02U, 0xA90C7BCCU, 0x1DBA0F3FU, 0xD3109AF1U,
		0x939F2C78U, 0x5D35B9B6U, 0xE983CD45U, 0x2729588BU,
		0x689CA057U, 0xA6363599U, 0x1280416AU, 0xDC2AD4A4U,
		0x9CA5622DU, 0x520FF7E3U, 0xE6B98310U, 0x281316DEU,
		0x453B4BFCU, 0x8B91DE32U, 0x3F27AAC1U, 0xF18D3F0FU,
		0xB1028986U, 0x7FA81C48U, 0xCB1E68BBU, 0x05B4FD75U,
		0x4A0105A9U, 0x84AB9067U, 0x301DE494U, 0xFEB7715AU,
		0xBE38C7D3U, 0x7092521DU, 0xC42426EEU, 0x0A8EB320U,
		0x5B4FD756U, 0x95E54298U, 0x2153366BU, 0xEFF9A3A5U,
		0xAF76152CU, 0x61DC80E2U, 0xD56AF411U, 0x1BC061DFU,
		0x54759903U, 0x9ADF0CCDU, 0x2E69783EU, 0xE0C3EDF0U,
		0xA04C5B79U, 0x6EE6CEB7U, 0xDA50BA44U, 0x14FA2F8AU,
		0xF3A4E550U, 0x3D0E709EU, 0x89B8046DU, 0x471291A3U,
		0x079D272AU, 0xC937B2E4U, 0x7D81C617U, 0xB32B53D9U,
		0xFC9EAB05U, 0x32343ECBU, 0x86824A38U, 0x4828DFF6U,
		0x08A7697FU, 0xC60DFCB1U, 0x72BB8842U, 0xBC111D8CU,
		0xEDD079FAU, 0x237AEC34U, 0x97CC98C7U, 0x59660D09U,
		0x19E9BB80U, 0xD7432E4EU, 0x63F55ABDU, 0xAD5FCF73U,
		0xE2EA37AFU, 0x2C40A261U, 0x98F6D692U, 0x565C435CU,
		0x16D3F5D5U, 0xD879601BU, 0x6CCF14E8U, 0xA2658126U,
		0xCF4DDC04U, 0x01E749CAU, 0xB5513D39U, 0x7BFBA8F7U,
		0x3B741E7EU, 0xF5DE8BB0U, 0x4168FF43U, 0x8FC26A8DU,
		0xC0779251U, 0x0EDD079FU, 0xBA6B736CU, 0x74C1E6A2U,
		0x344E502BU, 0xFAE4C5E5U, 0x4E52B116U, 0x80F824D8U,
		0xD13940AEU, 0x1F93D560U, 0xAB25A193U, 0x658F345DU,
		0x250082D4U, 0xEBAA171AU, 0x5F1C63E9U, 0x91B6F627U,
		0xDE030EFBU, 0x10A99B35U, 0xA41FEFC6U, 0x6AB57A08U,
		0x2A3ACC81U, 0xE490594FU, 0x50262DBCU, 0x9E8CB872U,
		0x8A7697F8U, 0x44DC0236U, 0xF06A76C5U, 0x3EC0E30BU,
		0x7E4F5582U, 0xB0E5C04CU, 0x0453B4BFU, 0xCAF92171U,
		0x854CD9ADU, 0x4BE64C63U, 0xFF503890U, 0x31FAAD5EU,
		0x71751BD7U, 0xBFDF8E19U, 0x0B69FAEAU, 0xC5C36F24U,
		0x94020B52U, 0x5AA89E9CU, 0xEE1EEA6FU, 0x20B47FA1U,
		0x603BC928U, 0xAE915CE6U, 0x1A272815U, 0xD48DBDDBU,
		0x9B384507U, 0x5592D0C9U, 0xE124A43AU, 0x2F8E31F4U,
		0x6F01877DU, 0xA1AB12B3U, 0x151D6640U, 0xDBB7F38EU,
		0xB69FAEACU, 0x78353B62U, 0xCC834F91U, 0x0229DA5FU,
		0x42A66CD6U, 0x8C0CF918U, 0x38BA8DEBU, 0xF6101825U,
		0xB9A5E0F9U, 0x770F7537U, 0xC3B901C4U, 0x0D13940AU,
		0x4D9C2283U, 0x8336B74DU, 0x3780C3BEU, 0xF92A5670U,
		0xA8EB3206U, 0x6641A7C8U, 0xD2F7D33BU, 0x1C5D46F5U,
		0x5CD2F07CU, 0x927865B2U, 0x26CE1141U, 0xE864848FU,
		0xA7D17C53U, 0x697BE99DU, 0xDDCD9D6EU, 0x136708A0U,
		0x53E8BE29U, 0x9D422BE7U, 0x29F45F14U, 0xE75ECADAU
};

/* This table was compiled using the polynom: 0x5D6DCB */
static const uint32_t crc32_5D6DCB[] =
{
		0x00000000, 0x005d6dcb, 0x00badb96, 0x00e7b65d,
		0x0028dae7, 0x0075b72c, 0x00920171, 0x00cf6cba,
		0x0051b5ce, 0x000cd805, 0x00eb6e58, 0x00b60393,
		0x00796f29, 0x002402e2, 0x00c3b4bf, 0x009ed974,
		0x00a36b9c, 0x00fe0657, 0x0019b00a, 0x0044ddc1,
		0x008bb17b, 0x00d6dcb0, 0x00316aed, 0x006c0726,
		0x00f2de52, 0x00afb399, 0x004805c4, 0x0015680f,
		0x00da04b5, 0x0087697e, 0x0060df23, 0x003db2e8,
		0x001bbaf3, 0x0046d738, 0x00a16165, 0x00fc0cae,
		0x00336014, 0x006e0ddf, 0x0089bb82, 0x00d4d649,
		0x004a0f3d, 0x001762f6, 0x00f0d4ab, 0x00adb960,
		0x0062d5da, 0x003fb811, 0x00d80e4c, 0x00856387,
		0x00b8d16f, 0x00e5bca4, 0x00020af9, 0x005f6732,
		0x00900b88, 0x00cd6643, 0x002ad01e, 0x0077bdd5,
		0x00e964a1, 0x00b4096a, 0x0053bf37, 0x000ed2fc,
		0x00c1be46, 0x009cd38d, 0x007b65d0, 0x0026081b,
		0x003775e6, 0x006a182d, 0x008dae70, 0x00d0c3bb,
		0x001faf01, 0x0042c2ca, 0x00a57497, 0x00f8195c,
		0x0066c028, 0x003bade3, 0x00dc1bbe, 0x00817675,
		0x004e1acf, 0x00137704, 0x00f4c159, 0x00a9ac92,
		0x00941e7a, 0x00c973b1, 0x002ec5ec, 0x0073a827,
		0x00bcc49d, 0x00e1a956, 0x00061f0b, 0x005b72c0,
		0x00c5abb4, 0x0098c67f, 0x007f7022, 0x00221de9,
		0x00ed7153, 0x00b01c98, 0x0057aac5, 0x000ac70e,
		0x002ccf15, 0x0071a2de, 0x00961483, 0x00cb7948,
		0x000415f2, 0x00597839, 0x00bece64, 0x00e3a3af,
		0x007d7adb, 0x00201710, 0x00c7a14d, 0x009acc86,
		0x0055a03c, 0x0008cdf7, 0x00ef7baa, 0x00b21661,
		0x008fa489, 0x00d2c942, 0x00357f1f, 0x006812d4,
		0x00a77e6e, 0x00fa13a5, 0x001da5f8, 0x0040c833,
		0x00de1147, 0x00837c8c, 0x0064cad1, 0x0039a71a,
		0x00f6cba0, 0x00aba66b, 0x004c1036, 0x00117dfd,
		0x006eebcc, 0x00338607, 0x00d4305a, 0x00895d91,
		0x0046312b, 0x001b5ce0, 0x00fceabd, 0x00a18776,
		0x003f5e02, 0x006233c9, 0x00858594, 0x00d8e85f,
		0x001784e5, 0x004ae92e, 0x00ad5f73, 0x00f032b8,
		0x00cd8050, 0x0090ed9b, 0x00775bc6, 0x002a360d,
		0x00e55ab7, 0x00b8377c, 0x005f8121, 0x0002ecea,
		0x009c359e, 0x00c15855, 0x0026ee08, 0x007b83c3,
		0x00b4ef79, 0x00e982b2, 0x000e34ef, 0x00535924,
		0x0075513f, 0x00283cf4, 0x00cf8aa9, 0x0092e762,
		0x005d8bd8, 0x0000e613, 0x00e7504e, 0x00ba3d85,
		0x0024e4f1, 0x0079893a, 0x009e3f67, 0x00c352ac,
		0x000c3e16, 0x005153dd, 0x00b6e580, 0x00eb884b,
		0x00d63aa3, 0x008b5768, 0x006ce135, 0x00318cfe,
		0x00fee044, 0x00a38d8f, 0x00443bd2, 0x00195619,
		0x00878f6d, 0x00dae2a6, 0x003d54fb, 0x00603930,
		0x00af558a, 0x00f23841, 0x00158e1c, 0x0048e3d7,
		0x00599e2a, 0x0004f3e1, 0x00e345bc, 0x00be2877,
		0x007144cd, 0x002c2906, 0x00cb9f5b, 0x0096f290,
		0x00082be4, 0x0055462f, 0x00b2f072, 0x00ef9db9,
		0x0020f103, 0x007d9cc8, 0x009a2a95, 0x00c7475e,
		0x00faf5b6, 0x00a7987d, 0x00402e20, 0x001d43eb,
		0x00d22f51, 0x008f429a, 0x0068f4c7, 0x0035990c,
		0x00ab4078, 0x00f62db3, 0x00119bee, 0x004cf625,
		0x00839a9f, 0x00def754, 0x00394109, 0x00642cc2,
		0x004224d9, 0x001f4912, 0x00f8ff4f, 0x00a59284,
		0x006afe3e, 0x003793f5, 0x00d025a8, 0x008d4863,
		0x00139117, 0x004efcdc, 0x00a94a81, 0x00f4274a,
		0x003b4bf0, 0x0066263b, 0x00819066, 0x00dcfdad,
		0x00e14f45, 0x00bc228e, 0x005b94d3, 0x0006f918,
		0x00c995a2, 0x0094f869, 0x00734e34, 0x002e23ff,
		0x00b0fa8b, 0x00ed9740, 0x000a211d, 0x00574cd6,
		0x0098206c, 0x00c54da7, 0x0022fbfa, 0x007f9631
};

uint32_t
crc32c_table_lookup (unsigned char pos)
{
	return crc32c_table[pos];
}

uint32_t
crc32_ccitt_table_lookup (unsigned char pos)
{
	return crc32_ccitt_table[pos];
}

uint32_t
crc32c_calculate(const void *buf, int len, uint32_t crc)
{
	const uint8_t *p = (const uint8_t *)buf;
	crc = CRC32C_SWAP(crc);
	while (len-- > 0) {
		CRC32C(crc, *p++);
	}
	return CRC32C_SWAP(crc);
}

uint32_t
crc32c_calculate_no_swap(const void *buf, int len, uint32_t crc)
{
	const uint8_t *p = (const uint8_t *)buf;
	while (len-- > 0) {
		CRC32C(crc, *p++);
	}

	return crc;
}

uint32_t
crc32_ccitt(const uint8_t *buf, unsigned len)
{
	return (crc32_ccitt_seed(buf, len, CRC32_CCITT_SEED));
}

uint32_t
crc32_ccitt_seed(const uint8_t *buf, unsigned len, uint32_t seed)
{
#if defined (HAVE_ZLIB) || defined (HAVE_ZLIBNG)
#ifdef HAVE_ZLIBNG
	return (unsigned)zng_crc32(~seed, buf, len);
#else
	return (unsigned)crc32(~seed, buf, len);
#endif
#else
	unsigned i;
	uint32_t crc32 = seed;

	for (i = 0; i < len; i++)
		CRC32_ACCUMULATE(crc32, buf[i], crc32_ccitt_table);

	return ( ~crc32 );
#endif
}

uint32_t
crc32_mpeg2_seed(const uint8_t *buf, unsigned len, uint32_t seed)
{
	unsigned i;
	uint32_t crc32;

	crc32 = seed;

	for (i = 0; i < len; i++)
		crc32 = (crc32 << 8) ^ crc32_mpeg2_table[((crc32 >> 24) ^ buf[i]) & 0xff];

	return ( crc32 );
}

uint32_t
crc32_0x0AA725CF_seed(const uint8_t *buf, unsigned len, uint32_t seed)
{
	unsigned crc32;

	crc32 = (unsigned)seed;
	while( len-- != 0 )
		CRC32_ACCUMULATE(crc32, *buf++, crc32_0AA725CF_reverse);

	return (uint32_t)crc32;
}

uint32_t
crc32_0x5D6DCB_seed(const uint8_t *buf, unsigned len, uint32_t seed)
{
	uint32_t crc = seed;
	if (len > 0)
	{
		while (len-- > 0)
		{
			uint8_t data = *buf++;
			/* XOR data with CRC2, look up result, then XOR that with CRC; */
			crc = crc32_5D6DCB[((crc >> 16) ^ data) & 0xff] ^ (crc << 8);
		}
	}
	return (crc & 0x00ffffff);
}


/*
 * Editor modelines  -  https://www.wireshark.org/tools/modelines.html
 *
 * Local variables:
 * c-basic-offset: 8
 * tab-width: 8
 * indent-tabs-mode: t
 * End:
 *
 * vi: set shiftwidth=8 tabstop=8 noexpandtab:
 * :indentSize=8:tabSize=8:noTabs=false:
 */
