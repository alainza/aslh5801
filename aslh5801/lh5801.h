/* z80.h */

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

/*)BUILD
	$(PROGRAM) =	ASZ80
	$(INCLUDE) = {
		ASXXXX.H
		LH5801.H
	}
	$(FILES) = {
		LH5801MCH.C
		LH5801ADR.C
		LH5801PST.C
		ASMAIN.C
		ASDBG.C
		ASLEX.C
		ASSYM.C
		ASSUBR.C
		ASEXPR.C
		ASDATA.C
		ASLIST.C
		ASOUT.C
	}
	$(STACK) = 3000
*/

/*
 * Indirect Addressing delimeters
 */
#define	LFIND	'('
#define RTIND	')'

/*
 * Registers
 */
#define A	0
// Official Mnemonics
#define XH	1
#define XL	2
#define YH	3
#define YL	4
#define UH	5
#define UL	6
#define PH	7
#define PL	8
#define SH	9
#define SL	10
// #Z80 Mnemonics
#define B	11
#define C	12
#define D	13
#define E	14
#define H	15
#define L	16

// Special registers... to investigate
#define I	17
#define F	18
#define T0  19
#define T1  20

// Official Mnemonics
#define X	21
#define Y	22
#define U	23
#define P	24
#define S	25
// Z80 Mnemonics
#define BC	26
#define DE	27
#define HL	28
#define SP	29
#define PC  39

/*
 * Conditional definitions
 */
#define	NZ	1
#define	Z	2
#define	NC	3
#define	CS	4
#define	NHS	5
#define	HS	6
#define V   7
#define NV  8
/*
 * Symbol types / indexing mode
 */
#define	S_VALUE	    30
#define	S_VALIND    31
#define S_VALINDX   32
#define S_VALNEG    33
#define	S_R8        34
#define	S_R16       35
#define	S_R16IND    36
#define	S_R16INDX   37


/*
 * Instruction types
 */
#define S_SIMPLE    51
#define S_SIMPLE2   52
#define S_ADC       53
#define S_ADI       54
#define S_DCA       55
#define S_ADR       56
#define S_SBC       57
#define S_SBI       58
#define S_DCS       59
#define S_AND       60
#define S_ANI       61
#define S_ORA       62
#define S_ORI       63
#define S_EOR       64
#define S_EAI       65
#define S_INC       66
#define S_DEC       67
#define S_CPA       68
#define S_CPI       69
#define S_BIT       70
#define S_BII       71
#define S_LDA       72
#define S_LDE       73
#define S_LIN       74
#define S_LDI       75
#define S_LDX       76
#define S_STA       77
#define S_SDE       78
#define S_SIN       79
#define S_STX       80
#define S_PSH       81
#define S_POP       82
#define S_DRL       83
#define S_DRR       84
#define S_JMP       85
#define S_BCH       86
#define S_BCS       87
#define S_BCR       88
#define S_BHS       89
#define S_BHR       90
#define S_BZS       91
#define S_BZR       92
#define S_BVS       93
#define S_BVR       94
#define S_LOP       95
#define S_SJP       96
#define S_VEJ       97
#define S_VMJ       98
#define S_VCS       99
#define S_VCR       100
#define S_VHS       101
#define S_VHR       102
#define S_VZS       103
#define S_VZR       104
#define S_VVS       105
#define S_ADCD      106
#define S_ADD       107
#define S_CALL      108
#define S_CP        109
#define S_DJL       110
#define S_JR        111
#define S_LD        112
#define S_LDD       113
#define S_OR        114
#define S_PUSH      115
#define S_RET       116
#define S_SBCD      117
#define S_SBR       118
#define S_TEST      119
#define S_XOR       120



/*
 * CPU Types
 */
#define	S_CPU	121

/*
 * Processor Types (S_CPU)
 */
#define	X_LH5801	0
#define X_Z80       1

struct adsym
{
	char	a_str[4];	/* addressing string */
	int	a_val;		    /* addressing mode value */
};

extern	struct	adsym	R8[];
extern	struct	adsym	R16[];
extern	struct	adsym	CND[];

	/* machine dependent functions */

#ifdef	OTHERSYSTEM
	
	/* z80adr.c */
extern	int		addr(struct expr *esp);
extern	int		admode(struct adsym *sp);
extern	int		srch(char *str);

	/* z80mch.c */
extern	VOID		machine(struct mne *mp);
extern	int		mchpcr(struct expr *esp);
extern	VOID		minit(void);

#else

	/* z80adr.c */
extern	int		addr();
extern	int		admode();
extern	int		srch();

	/* z80mch.c */
extern	int		genop();
extern	int		gixiy();
extern	VOID		machine();
extern	int		mchpcr();
extern	VOID		minit();

#endif

