// Copyright (C) 2013       tpu
// Copyright (C) 2015       Hykem <hykem@hotmail.com>
// Copyright (C) 2022       hitchhikr <charlet.franck@wanadoo.fr>
// Licensed under the terms of the GNU GPL, version 3
// http://www.gnu.org/licenses/gpl-3.0.txt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libkirk/kirk_engine.h"
#include "libkirk/psp_headers.h"

#include "utils.h"

#define PT_NULL 		0
#define PT_LOAD 		1
#define PT_DYNAMIC 		2
#define PT_INTERP 		3
#define PT_NOTE 		4
#define PT_SHLIB 		5
#define PT_PHDR 		6
#define PT_LOPROC 		0x70000000
#define PT_HIPROC 		0x7fffffff

static u8 test_k140[16] =
{
	0x35, 0xfe, 0x4c, 0x96, 0x00, 0xb2, 0xf6, 0x7e, 0xf5, 0x83, 0xa6, 0x79, 0x1f, 0xa0, 0xe8, 0x86,
};
static u8 test_kirk1[32] =
{
	0xca, 0x03, 0x84, 0xb1, 0xd9, 0x63, 0x47, 0x92, 0xce, 0xc7, 0x01, 0x23, 0x43, 0x72, 0x68, 0xac,
	0x77, 0xea, 0xec, 0xba, 0x6d, 0xaa, 0x97, 0xdf, 0xfe, 0x91, 0xb9, 0x39, 0x70, 0x99, 0x8b, 0x3a,
};

typedef struct
{
	u32 tag;
	u8  key[16];
	u32 code;
	u32 type;
} TAG_KEY;

static TAG_KEY key[] =
{
	{0xADF305F0, {0x12,0x99,0x70,0x5E,0x24,0x07,0x6C,0xD0,0x2D,0x06,0xFE,0x7E,0xB3,0x0C,0x11,0x26}, 0x60, 2}
};

int sign_eboot(u8 *eboot, int eboot_size, int passes, u8 *seboot);
