/* z80pst.c */

/*
 *  Copyright (C) 1989-2009  Alan R. Baldwin
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Alan R. Baldwin
 * 721 Berkeley St.
 * Kent, Ohio  44240
 */

#include "asxxxx.h"
#include "lh5801.h"

/*
 * Coding Banks
 */
struct	bank	bank[2] = {
    /*	The '_CODE' area/bank has a NULL default file suffix.	*/
    {	NULL,		"_CSEG",	NULL,		0,	0,	0,	0,	0	},
    {	&bank[0],	"_DSEG",	"_DS",		1,	0,	0,	0,	B_FSFX	}
};

/*
 * Coding Areas
 */
struct	area	area[2] = {
    {	NULL,		&bank[0],	"_CODE",	0,	0,	0,	A_1BYTE|A_BNK|A_CSEG	},
    {	&area[0],	&bank[1],	"_DATA",	1,	0,	0,	A_1BYTE|A_BNK|A_DSEG	}
};

/*
 * Basic Relocation Mode Definition
 *
 *	#define		R_NORM	0000		No Bit Positioning
 */
char	mode0[32] = {	/* R_NORM */
	'\200',	'\201',	'\202',	'\203',	'\204',	'\205',	'\206',	'\207',
	'\210',	'\211',	'\212',	'\213',	'\214',	'\215',	'\216',	'\217',
	'\220',	'\221',	'\222',	'\223',	'\224',	'\225',	'\226',	'\227',
	'\230',	'\231',	'\232',	'\233',	'\234',	'\235',	'\236',	'\237'
};

/*
 * Additional Relocation Mode Definitions
 */

/* None Required */

/*
 *     *m_def is a pointer to the bit relocation definition.
 *	m_flag indicates that bit position swapping is required.
 *	m_dbits contains the active bit positions for the output.
 *	m_sbits contains the active bit positions for the input.
 *
 *	struct	mode
 *	{
 *		char *	m_def;		Bit Relocation Definition
 *		a_uint	m_flag;		Bit Swapping Flag
 *		a_uint	m_dbits;	Destination Bit Mask
 *		a_uint	m_sbits;	Source Bit Mask
 *	};
 */
struct	mode	mode[1] = {
    {	&mode0[0],	0,	0x0000FFFF,	0x0000FFFF	}
};

/*
 * Array of Pointers to mode Structures
 */
struct	mode	*modep[16] = {
	&mode[0],	NULL,		NULL,		NULL,
	NULL,		NULL,		NULL,		NULL,
	NULL,		NULL,		NULL,		NULL,
	NULL,		NULL,		NULL,		NULL
};

/*
 * Mnemonic Structure
 */
struct	mne	mne[] = {

	/* machine */

    {	NULL,	"CSEG",		S_ATYP,		0,	A_CSEG|A_1BYTE	},
    {	NULL,	"DSEG",		S_ATYP,		0,	A_DSEG|A_1BYTE	},

	/* system */

    {	NULL,	"BANK",		S_ATYP,		0,	A_BNK	},
    {	NULL,	"CON",		S_ATYP,		0,	A_CON	},
    {	NULL,	"OVR",		S_ATYP,		0,	A_OVR	},
    {	NULL,	"REL",		S_ATYP,		0,	A_REL	},
    {	NULL,	"ABS",		S_ATYP,		0,	A_ABS	},
    {	NULL,	"NOPAG",	S_ATYP,		0,	A_NOPAG	},
    {	NULL,	"PAG",		S_ATYP,		0,	A_PAG	},

    {	NULL,	"BASE",		S_BTYP,		0,	B_BASE	},
    {	NULL,	"SIZE",		S_BTYP,		0,	B_SIZE	},
    {	NULL,	"FSFX",		S_BTYP,		0,	B_FSFX	},
    {	NULL,	"MAP",		S_BTYP,		0,	B_MAP	},

    {	NULL,	".page",	S_PAGE,		0,	0	},
    {	NULL,	".title",	S_HEADER,	0,	O_TITLE	},
    {	NULL,	".sbttl",	S_HEADER,	0,	O_SBTTL	},
    {	NULL,	".module",	S_MODUL,	0,	0	},
    {	NULL,	".include",	S_INCL,		0,	0	},
    {	NULL,	".area",	S_AREA,		0,	0	},
    {	NULL,	".bank",	S_BANK,		0,	0	},
    {	NULL,	".org",		S_ORG,		0,	0	},
    {	NULL,	".radix",	S_RADIX,	0,	0	},
    {	NULL,	".globl",	S_GLOBL,	0,	0	},
    {	NULL,	".local",	S_LOCAL,	0,	0	},
    {	NULL,	".if",		S_CONDITIONAL,	0,	O_IF	},
    {	NULL,	".iff",		S_CONDITIONAL,	0,	O_IFF	},
    {	NULL,	".ift",		S_CONDITIONAL,	0,	O_IFT	},
    {	NULL,	".iftf",	S_CONDITIONAL,	0,	O_IFTF	},
    {	NULL,	".ifdef",	S_CONDITIONAL,	0,	O_IFDEF	},
    {	NULL,	".ifndef",	S_CONDITIONAL,	0,	O_IFNDEF},
    {	NULL,	".ifgt",	S_CONDITIONAL,	0,	O_IFGT	},
    {	NULL,	".iflt",	S_CONDITIONAL,	0,	O_IFLT	},
    {	NULL,	".ifge",	S_CONDITIONAL,	0,	O_IFGE	},
    {	NULL,	".ifle",	S_CONDITIONAL,	0,	O_IFLE	},
    {	NULL,	".ifeq",	S_CONDITIONAL,	0,	O_IFEQ	},
    {	NULL,	".ifne",	S_CONDITIONAL,	0,	O_IFNE	},
    {	NULL,	".ifb",		S_CONDITIONAL,	0,	O_IFB	},
    {	NULL,	".ifnb",	S_CONDITIONAL,	0,	O_IFNB	},
    {	NULL,	".ifidn",	S_CONDITIONAL,	0,	O_IFIDN	},
    {	NULL,	".ifdif",	S_CONDITIONAL,	0,	O_IFDIF	},
    {	NULL,	".iif",		S_CONDITIONAL,	0,	O_IIF	},
    {	NULL,	".iiff",	S_CONDITIONAL,	0,	O_IIFF	},
    {	NULL,	".iift",	S_CONDITIONAL,	0,	O_IIFT	},
    {	NULL,	".iiftf",	S_CONDITIONAL,	0,	O_IIFTF	},
    {	NULL,	".iifdef",	S_CONDITIONAL,	0,	O_IIFDEF},
    {	NULL,	".iifndef",	S_CONDITIONAL,	0,	O_IIFNDEF},
    {	NULL,	".iifgt",	S_CONDITIONAL,	0,	O_IIFGT	},
    {	NULL,	".iiflt",	S_CONDITIONAL,	0,	O_IIFLT	},
    {	NULL,	".iifge",	S_CONDITIONAL,	0,	O_IIFGE	},
    {	NULL,	".iifle",	S_CONDITIONAL,	0,	O_IIFLE	},
    {	NULL,	".iifeq",	S_CONDITIONAL,	0,	O_IIFEQ	},
    {	NULL,	".iifne",	S_CONDITIONAL,	0,	O_IIFNE	},
    {	NULL,	".iifb",	S_CONDITIONAL,	0,	O_IIFB	},
    {	NULL,	".iifnb",	S_CONDITIONAL,	0,	O_IIFNB	},
    {	NULL,	".iifidn",	S_CONDITIONAL,	0,	O_IIFIDN},
    {	NULL,	".iifdif",	S_CONDITIONAL,	0,	O_IIFDIF},
    {	NULL,	".else",	S_CONDITIONAL,	0,	O_ELSE	},
    {	NULL,	".endif",	S_CONDITIONAL,	0,	O_ENDIF	},
    {	NULL,	".list",	S_LISTING,	0,	O_LIST	},
    {	NULL,	".nlist",	S_LISTING,	0,	O_NLIST	},
    {	NULL,	".equ",		S_EQU,		0,	O_EQU	},
    {	NULL,	".gblequ",	S_EQU,		0,	O_GBLEQU},
    {	NULL,	".lclequ",	S_EQU,		0,	O_LCLEQU},
    {	NULL,	".byte",	S_DATA,		0,	O_1BYTE	},
    {	NULL,	".db",		S_DATA,		0,	O_1BYTE	},
    {	NULL,	".fcb",		S_DATA,		0,	O_1BYTE	},
    {	NULL,	".word",	S_DATA,		0,	O_2BYTE	},
    {	NULL,	".dw",		S_DATA,		0,	O_2BYTE	},
    {	NULL,	".fdb",		S_DATA,		0,	O_2BYTE	},
/*    {	NULL,	".3byte",	S_DATA,		0,	O_3BYTE	},	*/
/*    {	NULL,	".triple",	S_DATA,		0,	O_3BYTE	},	*/
/*    {	NULL,	".4byte",	S_DATA,		0,	O_4BYTE	},	*/
/*    {	NULL,	".quad",	S_DATA,		0,	O_4BYTE	},	*/
    {	NULL,	".blkb",	S_BLK,		0,	O_1BYTE	},
    {	NULL,	".ds",		S_BLK,		0,	O_1BYTE	},
    {	NULL,	".rmb",		S_BLK,		0,	O_1BYTE	},
    {	NULL,	".rs",		S_BLK,		0,	O_1BYTE	},
    {	NULL,	".blkw",	S_BLK,		0,	O_2BYTE	},
/*    {	NULL,	".blk3",	S_BLK,		0,	O_3BYTE	},	*/
/*    {	NULL,	".blk4",	S_BLK,		0,	O_4BYTE	},	*/
    {	NULL,	".ascii",	S_ASCIX,	0,	O_ASCII	},
    {	NULL,	".ascis",	S_ASCIX,	0,	O_ASCIS	},
    {	NULL,	".asciz",	S_ASCIX,	0,	O_ASCIZ	},
    {	NULL,	".str",		S_ASCIX,	0,	O_ASCII	},
    {	NULL,	".strs",	S_ASCIX,	0,	O_ASCIS	},
    {	NULL,	".strz",	S_ASCIX,	0,	O_ASCIZ	},
    {	NULL,	".fcc",		S_ASCIX,	0,	O_ASCII	},
    {	NULL,	".define",	S_DEFINE,	0,	O_DEF	},
    {	NULL,	".undefine",	S_DEFINE,	0,	O_UNDEF	},
    {	NULL,	".even",	S_BOUNDARY,	0,	O_EVEN	},
    {	NULL,	".odd",		S_BOUNDARY,	0,	O_ODD	},
    {	NULL,	".bndry",	S_BOUNDARY,	0,	O_BNDRY	},
    {	NULL,	".msg"	,	S_MSG,		0,	0	},
    {	NULL,	".assume",	S_ERROR,	0,	O_ASSUME},
    {	NULL,	".error",	S_ERROR,	0,	O_ERROR	},
/*    {	NULL,	".msb",		S_MSB,		0,	0	},	*/
/*    {	NULL,	".lohi",	S_MSB,		0,	O_LOHI	},	*/
/*    {	NULL,	".hilo",	S_MSB,		0,	O_HILO	},	*/
/*    {	NULL,	".8bit",	S_BITS,		0,	O_1BYTE	},	*/
/*    {	NULL,	".16bit",	S_BITS,		0,	O_2BYTE	},	*/
/*    {	NULL,	".24bit",	S_BITS,		0,	O_3BYTE	},	*/
/*    {	NULL,	".32bit",	S_BITS,		0,	O_4BYTE	},	*/
    {	NULL,	".end",		S_END,		0,	0	},

	/* Macro Processor */

    {	NULL,	".macro",	S_MACRO,	0,	O_MACRO	},
    {	NULL,	".endm",	S_MACRO,	0,	O_ENDM	},
    {	NULL,	".mexit",	S_MACRO,	0,	O_MEXIT	},

    {	NULL,	".narg",	S_MACRO,	0,	O_NARG	},
    {	NULL,	".nchr",	S_MACRO,	0,	O_NCHR	},
    {	NULL,	".ntyp",	S_MACRO,	0,	O_NTYP	},

    {	NULL,	".irp",		S_MACRO,	0,	O_IRP	},
    {	NULL,	".irpc",	S_MACRO,	0,	O_IRPC	},
    {	NULL,	".rept",	S_MACRO,	0,	O_REPT	},

    {	NULL,	".nval",	S_MACRO,	0,	O_NVAL	},

    {	NULL,	".mdelete",	S_MACRO,	0,	O_MDEL	},

	/* Machines */

    {   NULL,   ".lh5801",  S_CPU,      0, X_LH5801 },
    {   NULL,   ".z80",     S_CPU,      0, X_Z80 },

	/* lh5801 */
    { NULL, "adc", S_ADC, 0, 0xff },
    { NULL, "adi", S_ADI, 0, 0xff },
    { NULL, "dca", S_DCA, 0, 0xff },
    { NULL, "adr", S_ADR, 0, 0xff },
    { NULL, "sbc", S_SBC, 0, 0xff },
    { NULL, "sbi", S_SBI, 0, 0xff },
    { NULL, "dcs", S_DCS, 0, 0xff },
    { NULL, "and", S_AND, 0, 0xff },
    { NULL, "ani", S_ANI, 0, 0xff },
    { NULL, "ora", S_ORA, 0, 0xff },
    { NULL, "ori", S_ORI, 0, 0xff },
    { NULL, "eor", S_EOR, 0, 0xff },
    { NULL, "eai", S_EAI, 0, 0xff },
    { NULL, "inc", S_INC, 0, 0xff },
    { NULL, "dec", S_DEC, 0, 0xff },
    { NULL, "cpa", S_CPA, 0, 0xff },
    { NULL, "cpi", S_CPI, 0, 0xF7 }, /* op code=cin in z80 mode*/
    { NULL, "bit", S_BIT, 0, 0xff },
    { NULL, "bii", S_BII, 0, 0xff },
    { NULL, "lda", S_LDA, 0, 0xff },
    { NULL, "lde", S_LDE, 0, 0x47 },
    { NULL, "lin", S_LIN, 0, 0x45 },
    { NULL, "ldi", S_LDI, 0, 0xff },
    { NULL, "ldx", S_LDX, 0, 0xff },
    { NULL, "sta", S_STA, 0, 0xff },
    { NULL, "sde", S_SDE, 0, 0x43 },
    { NULL, "sin", S_SIN, 0, 0x41 },
    { NULL, "stx", S_STX, 0, 0xff },
    { NULL, "psh", S_PSH, 0, 0xff },
    { NULL, "pop", S_POP, 0, 0xff },
    { NULL, "att", S_SIMPLE2, 0, 0xEC },
    { NULL, "tta", S_SIMPLE2, 0, 0xAA },
    { NULL, "tin", S_SIMPLE, 0, 0xF5 },
    { NULL, "cin", S_SIMPLE, 0, 0xF7 },
    { NULL, "rol", S_SIMPLE, 0, 0xDB },
    { NULL, "ror", S_SIMPLE, 0, 0xD1 },
    { NULL, "shl", S_SIMPLE, 0, 0xD9 },
    { NULL, "shr", S_SIMPLE, 0, 0xD5 },
    { NULL, "drl", S_DRL, 0, 0xD7 },
    { NULL, "drr", S_DRR, 0, 0xD3 },
    { NULL, "aex", S_SIMPLE, 0, 0xF1 },
    { NULL, "sec", S_SIMPLE, 0, 0xFB },
    { NULL, "rec", S_SIMPLE, 0, 0xF9 },
    { NULL, "cdv", S_SIMPLE, 0, 0x8E },
    { NULL, "atp", S_SIMPLE2, 0, 0xCC },
    { NULL, "spu", S_SIMPLE, 0, 0xE1 },
    { NULL, "rpu", S_SIMPLE, 0, 0xE3 },
    { NULL, "spv", S_SIMPLE, 0, 0xA8 },
    { NULL, "rpv", S_SIMPLE, 0, 0xB8 },
    { NULL, "sdp", S_SIMPLE2, 0, 0xC1 },
    { NULL, "rdp", S_SIMPLE2, 0, 0xC0 },
    { NULL, "ita", S_SIMPLE2, 0, 0xBA },
    { NULL, "sie", S_SIMPLE2, 0, 0x81 },
    { NULL, "rie", S_SIMPLE2, 0, 0xBE },
    { NULL, "am0", S_SIMPLE2, 0, 0xCE },
    { NULL, "am1", S_SIMPLE2, 0, 0xDE },
    { NULL, "nop", S_SIMPLE, 0, 0x38 },
    { NULL, "hlt", S_SIMPLE2, 0, 0xB1 },
    { NULL, "off", S_SIMPLE2, 0, 0x4C },
    { NULL, "jmp", S_JMP, 0, 0xff },
    { NULL, "bch", S_BCH, 0, 0x8E },
    { NULL, "bcs", S_BCS, 0, 0x83 },
    { NULL, "bcr", S_BCR, 0, 0x81 },
    { NULL, "bhs", S_BHS, 0, 0x87 },
    { NULL, "bhr", S_BHR, 0, 0x85 },
    { NULL, "bzs", S_BZS, 0, 0x8B },
    { NULL, "bzr", S_BZR, 0, 0x89 },
    { NULL, "bvs", S_BVS, 0, 0x8F },
    { NULL, "bvr", S_BVR, 0, 0x8D },
    { NULL, "lop", S_LOP, 0, 0x88 },
    { NULL, "sjp", S_SJP, 0, 0xff },
    { NULL, "vej", S_VEJ, 0, 0xff },
    { NULL, "vmj", S_VMJ, 0, 0xCD },
    { NULL, "vcs", S_VCS, 0, 0xC3 },
    { NULL, "vcr", S_VCR, 0, 0xC1 },
    { NULL, "vhs", S_VHS, 0, 0xC7 },
    { NULL, "vhr", S_VHR, 0, 0xC5 },
    { NULL, "vzs", S_VZS, 0, 0xCB },
    { NULL, "vzr", S_VZR, 0, 0xC9 },
    { NULL, "vvs", S_VVS, 0, 0xCF },
    { NULL, "rtn", S_SIMPLE, 0, 0x9A },
    { NULL, "rti", S_SIMPLE, 0, 0x8A },

    // Z80 mnemonics not yet added
    { NULL, "adcd", S_ADCD, 0, 0xFF },
    { NULL, "add", S_ADD, 0, 0xFF },
    { NULL, "call", S_CALL, 0, 0xFF },
    { NULL, "cp", S_CP, 0, 0xFF },
    { NULL, "di", S_SIMPLE, 0, 0xBE },
    { NULL, "djl", S_DJL, 0, 0x88 },
    { NULL, "ei", S_SIMPLE, 0, 0x81 },
    { NULL, "halt", S_SIMPLE, 0, 0xB1 },
    { NULL, "jr", S_JR, 0, 0xFF },
    { NULL, "ld", S_LD, 0, 0xFF },
    { NULL, "ldd", S_LDD, 0, 0xFF },
    { NULL, "nex", S_SIMPLE, 0, 0xF1 },
    { NULL, "or", S_OR, 0, 0xFF },
    { NULL, "push", S_PUSH, 0, 0xFF },
    { NULL, "rcf", S_SIMPLE, 0, 0xF9 },
    { NULL, "ret", S_RET, 0, 0xFF },
    { NULL, "rla", S_SIMPLE, 0, 0xDB },
    { NULL, "rra", S_SIMPLE, 0, 0xD1 },
    { NULL, "rvf", S_SIMPLE, 0, 0xF3 },// unknown cycle count
    { NULL, "sbcd", S_SBCD, 0, 0xFF },
    { NULL, "sbr", S_SBR, 0, 0xFF },
    { NULL, "scf", S_SIMPLE, 0, 0xFB },
    { NULL, "sla", S_SIMPLE, 0, 0xD9 },
    { NULL, "srl", S_SIMPLE, 0, 0xD5 },
    { NULL, "test", S_TEST, 0, 0xFF },
    { NULL, "xor", S_XOR, S_EOL, 0xFF },

};
