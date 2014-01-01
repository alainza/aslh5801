/* z80mch.c */

/*
 * (C) Copyright 1989-2006
 * All Rights Reserved
 *
 * Alan R. Baldwin
 * 721 Berkeley St.
 * Kent, Ohio  44240
 */

#include "asxxxx.h"
#include "lh5801.h"

char	*cpu	= "LH 5801";
char	*dsft	= "asm";

char	imtab[3] = { 0x46, 0x56, 0x5E };
int	mchtyp;

/*
 * Opcode Cycle Definitions
 */
#define	OPCY_SDP	((char) (0xFF)) // May vary
#define	OPCY_ERR	((char) (0xFE))
#define	OPCY_CPU	((char) (0xFD))

/*	OPCY_NONE	((char) (0x80))	*/
/*	OPCY_MASK	((char) (0x7F))	*/

// opcycles for simple mnemonics
int opcycles_page_1[]  = {
    /*          0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
     *  0 */    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /*  1 */    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /*  2 */    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /*  3 */    0,  0,  0,  0,  0,  0,  0,  0,  5,  0,  0,  0,  0,  0,  0,  0,
    /*  4 */    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /*  5 */    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /*  6 */    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /*  7 */    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /*  8 */    0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14,  0,  0,  0,  8,  0,
    /*  9 */    0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 11,  0,  0,  0,  0,  0,
    /*  A */    0,  0,  0,  0,  0,  0,  0,  0,  4,  0,  0,  0,  0,  0,  0,  0,
    /*  B */    0,  0,  0,  0,  0,  0,  0,  0,  4,  0,  0,  0,  0,  0,  0,  0,
    /*  C */    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /*  D */    0,  9,  0,  0,  0,  9,  0,  0,  0,  5,  0,  8,  0,  0,  0,  0,
    /*  E */    0,  4,  0,  4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /*  F */    0,  6,  0,255,  0,  7,  0,  7,  0,  4,  0,  4,  0,  0,  0,  0
};
int opcycles_page_2[] = {
    /*          0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
     *  0 */    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /*  1 */    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /*  2 */    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /*  3 */    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /*  4 */    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8,  0,  0,  0,
    /*  5 */    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /*  6 */    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /*  7 */    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /*  8 */    0,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /*  9 */    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    /*  A */    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  0,  0,  0,  0,  0,
    /*  B */    0,  9,  0,  0,  0,  0,  0,  0,  0,  0,  9,  0,  0,  0,  8,  0,
    /*  C */    8,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  0,  9,  0,
    /*  D */    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  0,
    /*  E */    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  0,  0,  0,
};

/*
 * Process a machine op.
 */
VOID
machine_lh5801(mp)
struct mne *mp;
{
	int op, t1, t2;
	struct expr e1, e2;
	int rf, v2;

	clrexpr(&e1);
	clrexpr(&e2);
	op = (int) mp->m_valu;
	rf = mp->m_type;
	switch (rf) {

    case S_ADC:
        t1 = addr(&e1);
        if (t1 == S_R8) {
            switch (e1.e_addr) {
            case XL: outab(0x02); break;
            case YL: outab(0x12); break;
            case UL: outab(0x22); break;
            case XH: outab(0x82); break;
            case YH: outab(0x92); break;
            case UH: outab(0xA2); break;
            default: aerr(); break;
            }
            opcycles = 6;
        }
        else if (t1 == S_R16IND) {
            switch (e1.e_addr) {
            case X: outab(0x03); break;
            case Y: outab(0x13); break;
            case U: outab(0x23); break;
            default: aerr(); break;
            }
            opcycles = 7;
        }
        else if (t1 == S_VALUE) {
            outab(0xA3);
            outrw(&e1, 0);
            opcycles = 13;
        }
        else if (t1 == S_R16INDX) {
            switch (e1.e_addr) {
            case X: outab(0xFD); outab(0x03); break;
            case Y: outab(0xFD); outab(0x13); break;
            case U: outab(0xFD); outab(0x23); break;
            default: aerr(); break;
            }
            opcycles = 11;
        }
        else if (t1 == S_VALINDX) {
            outab(0xFD);
            outab(0xA3);
            outrw(&e1, 0);
            opcycles = 17;
        }
        else {
            aerr();
        }
        break;

    case S_ADI:
        t1 = addr(&e1);
        comma(1);
        t2 = addr(&e2);
        if ((t1 == S_R8) && (e1.e_addr == A) && (t2 == S_VALUE) && (e2.e_addr < 256)) {
            outab(0xb3);
            outrb(&e2, 0);
            opcycles = 7;
            break;
        }
        else if ((t1 == S_R16) && (t2 == S_VALUE) && (e2.e_addr < 256)) {
            switch (e1.e_addr) {
            case X: outab(0x4F); break;
            case Y: outab(0x5F); break;
            case U: outab(0x6F); break;
            default: aerr(); break;
            }
            outrb(&e2, 0);
            opcycles = 13;
        }
        else if ((t1 == S_VALUE) && (t2 == S_VALUE) && (e2.e_addr < 256)) {
            outab(0xEF);
            outrw(&e1, 0);
            outrb(&e2, 0);
            opcycles = 19;
        }
        else if ((t1 == S_R16INDX) && (t2 == S_VALUE) && (e2.e_addr < 256)) {
            switch (e1.e_addr) {
            case X: outab(0xFD); outab(0x4F); break;
            case Y: outab(0xFD); outab(0x5F); break;
            case U: outab(0xFD); outab(0x6F); break;
            default: aerr(); break;
            outrb(&e2, 0);
            opcycles = 17;
            }
        }
        else if ((t1 == S_VALINDX) && (t2 == S_VALUE) && (e2.e_addr < 256)) {
            outab(0xFD);
            outab(0xEF);
            outrw(&e1, 0);
            outrb(&e2, 0);
            opcycles = 23;
        }
        else {
            aerr();
        }
        break;

    case S_DCA:
        t1 = addr(&e1);
        if (t1 == S_R16IND) {
            switch (e1.e_addr) {
            case X: outab(0x8C); break;
            case Y: outab(0x9C); break;
            case U: outab(0xAC); break;
            default: aerr(); break;
            }
            opcycles = 15;
        }
        else if (t1 == S_R16INDX) {
            switch (e1.e_addr) {
            case X: outab(0xFD); outab(0x8C); break;
            case Y: outab(0xFD); outab(0x9C); break;
            case U: outab(0xFD); outab(0xAC); break;
            default: aerr(); break;
            }
            opcycles = 19;
        }
        else {
            aerr();
        }
        break;

    case S_ADR:
        t1 = addr(&e1);
        if (t1 == S_R16) {
            switch (e1.e_addr) {
            case X:outab(0xFD);  outab(0x09); break;
            case Y:outab(0xFD);  outab(0x19); break;
            case U:outab(0xFD);  outab(0x29); break;
            default: aerr(); break;
            }
            opcycles = 11;
        }
        else {
            aerr();
        }
        break;

    case S_SBC:
        t1 = addr(&e1);
        if (t1 == S_R8) {
            switch (e1.e_addr) {
            case XL: outab(0x00); break;
            case YL: outab(0x10); break;
            case UL: outab(0x20); break;
            case XH: outab(0x80); break;
            case YH: outab(0x90); break;
            case UH: outab(0xA0); break;
            default: aerr(); break;
            }
            opcycles = 6;
        }
        else if (t1 == S_R16IND) {
            switch (e1.e_addr) {
            case X: outab(0x01); break;
            case Y: outab(0x11); break;
            case U: outab(0x21); break;
            default: aerr(); break;
            }
            opcycles = 7;
        }
        else if (t1 == S_VALUE) {
            outab(0xA1);
            outrw(&e1, 0);
        }
        else if (t1 == S_R16INDX) {
            switch (e1.e_addr) {
            case X: outab(0xFD); outab(0x01); break;
            case Y: outab(0xFD); outab(0x11); break;
            case U: outab(0xFD); outab(0x21); break;
            default: aerr(); break;
            }
        }
        else if (t1 == S_VALINDX) {
            outab(0xFD);
            outab(0xA1);
            outrw(&e1, 0);
        }
        else {
            aerr();
        }
        break;

    case S_SBI:
        t1 = addr(&e1);
        if ((t1 == S_VALUE) && (e1.e_addr < 256)) {
            outab(0xb1);
            outrb(&e1, 0);
            opcycles = 7;
        }
        else {
            aerr();
        }
        break;

    case S_DCS:
        t1 = addr(&e1);
        if (t1 == S_R16IND) {
            switch (e1.e_addr) {
            case X: outab(0x0C); break;
            case Y: outab(0x1C); break;
            case U: outab(0x2C); break;
            default: aerr(); break;
            }
            opcycles = 13;
        }
        else if (t1 == S_R16INDX) {
            switch (e1.e_addr) {
            case X: outab(0xFD); outab(0x0C); break;
            case Y: outab(0xFD); outab(0x1C); break;
            case U: outab(0xFD); outab(0x2C); break;
            default: aerr(); break;
            }
            opcycles = 17;
        }
        else {
            aerr();
        }
        break;

    case S_AND:
        t1 = addr(&e1);
        if (t1 == S_R16IND) {
            switch (e1.e_addr) {
            case X: outab(0x09); break;
            case Y: outab(0x19); break;
            case U: outab(0x29); break;
            default: aerr(); break;
            }
            opcycles = 7;
        }
        else if (t1 == S_VALUE) {
            outab(0xA9);
            outrw(&e1, 0);
            opcycles = 13;
        }
        else if (t1 == S_R16INDX) {
            switch (e1.e_addr) {
            case X: outab(0xFD); outab(0x09); break;
            case Y: outab(0xFD); outab(0x19); break;
            case U: outab(0xFD); outab(0x29); break;
            default: aerr(); break;
            }
            opcycles = 11;
        }
        else if (t1 == S_VALINDX) {
            outab(0xFD);
            outab(0xA9);
            outrw(&e1, 0);
            opcycles = 17;
        }
        else {
            aerr();
        }
        break;

    case S_ANI:
        t1 = addr(&e1);
        comma(1);
        t2 = addr(&e2);
        if ((t1 == S_R8) && (e1.e_addr == A) && (t2 == S_VALUE) && (e2.e_addr < 256)) {
            outab(0xB9);
            outrb(&e2, 0);
            opcycles = 7;
        }
        else if ((t1 == S_R16IND) && (t2 == S_VALUE) && (e2.e_addr < 256)) {
            switch (e1.e_addr) {
            case X: outab(0x49); break;
            case Y: outab(0x59); break;
            case U: outab(0x69); break;
            default: aerr(); break;
            }
            outrb(&e2, 0);
            opcycles = 13;
        }
        else if ((t1 == S_VALUE) && (t2 == S_VALUE) && (e2.e_addr < 256)) {
            outab(0xE9);
            outrb(&e2, 0);
            opcycles = 19;
        }
        else if ((t1 == S_R16INDX) && (t2 == S_VALUE) && (e2.e_addr < 256)) {
            switch (e1.e_addr) {
            case X: outab(0xFD); outab(0x49); break;
            case Y: outab(0xFD); outab(0x59); break;
            case U: outab(0xFD); outab(0x69); break;
            default: aerr(); break;
            }
            outrb(&e2, 0);
            opcycles = 17;
        }
        else if ((t1 == S_VALINDX) && (t2 == S_VALUE) && (e2.e_addr < 256)) {
            outab(0xFD);
            outab(0xE9);
            outrb(&e2, 0);
            opcycles = 23;
        }
        else {
            aerr();
        }
        break;

    case S_ORA:
        t1 = addr(&e1);
        if (t1 == S_R16IND) {
            switch (e1.e_addr) {
            case X: outab(0x0B); break;
            case Y: outab(0x1B); break;
            case U: outab(0x2B); break;
            default: aerr(); break;
            }
            opcycles = 7;
        }
        else if (t1 == S_VALUE) {
            outab(0xAB);
            outrw(&e1, 0);
            opcycles = 13;
        }
        else if (t1 == S_R16INDX) {
            switch (e1.e_addr) {
            case X: outab(0xFD); outab(0x0B); break;
            case Y: outab(0xFD); outab(0x1B); break;
            case U: outab(0xFD); outab(0x2B); break;
            default: aerr(); break;
            }
            opcycles = 11;
        }
        else if (t1 == S_VALINDX) {
            outab(0xFD);
            outab(0xAE);
            outrw(&e1, 0);
            opcycles = 17;
        }
        else {
            aerr();
        }
        break;

    case S_ORI:
        t1 = addr(&e1);
        comma(1);
        t2 = addr(&e2);
        if ((t1 == S_R8) && (e1.e_addr == A)) {
            outab(0xBB);
            outrb(&e2, 0);
            opcycles = 7;
        }
        else if (t1 == S_R16IND) {
            switch (e1.e_addr) {
            case X: outab(0x4B); break;
            case Y: outab(0x5B); break;
            case U: outab(0x6B); break;
            default: aerr(); break;
            }
            outrb(&e2, 0);
            opcycles = 13;
        }
        else if (t1 == S_VALUE) {
            outab(0xEB);
            outrw(&e1, 0);
            outrb(&e2, 0);
            opcycles = 19;
        }
        else if (t1 == S_R16INDX) {
            switch (e1.e_addr) {
            case X: outab(0xFD); outab(0x4B); break;
            case Y: outab(0xFD); outab(0x5B); break;
            case U: outab(0xFD); outab(0x6B); break;
            default: aerr(); break;
            }
            outrb(&e2, 0);
            opcycles = 17;
        }
        else if (t1 == S_VALINDX) {
            outab(0xFD);
            outab(0xEB);
            outrw(&e1, 0);
            outrb(&e2, 0);
            opcycles = 23;
        }
        else {
            aerr();
        }
        break;

    case S_EOR:
        t1 = addr(&e1);
        if (t1 == S_R16IND) {
            switch (e1.e_addr) {
            case X: outab(0x0D); break;
            case Y: outab(0x1D); break;
            case U: outab(0x2D); break;
            default: aerr(); break;
            }
            opcycles = 7;
        }
        else if (t1 == S_VALUE) {
            outab(0xAD);
            outrw(&e1, 0);
            opcycles = 13;
        }
        else if (t1 == S_R16INDX) {
            switch (e1.e_addr) {
            case X: outab(0xFD); outab(0x0D); break;
            case Y: outab(0xFD); outab(0x1D); break;
            case U: outab(0xFD); outab(0x2D); break;
            default: aerr(); break;
            }
            opcycles = 11;
        }
        else if (t1 == S_VALINDX) {
            outab(0xFD);
            outab(0xAD);
            outrw(&e1, 0);
            opcycles = 17;
        }
        else {
            aerr();
        }
        break;

    case S_EAI:
        t1 = addr(&e1);
        if ((t1 == S_VALUE) && (e1.e_addr < 256)) {
            outab(0xBD);
            outrb(&e1, 0);
            opcycles = 7;
        }
        else {
            aerr();
        }
        break;

    case S_INC:
        t1 = addr(&e1);
        if (t1 == S_R8) {
            opcycles = 5;
            switch (e1.e_addr) {
            case A: outab(0xDD); break;
            case XL: outab(0x40); break;
            case YL: outab(0x50); break;
            case UL: outab(0x60); break;
            case XH: outab(0xFD); outab(0x40); opcycles = 9; break;
            case YH: outab(0xFD); outab(0x50); opcycles = 9; break;
            case UH: outab(0xFD); outab(0x60); opcycles = 9; break;
            default: aerr(); break;
            }
        }
        else if (t1 == S_R16) {
            switch (e1.e_addr) {
            case X: outab(0x44); break;
            case Y: outab(0x54); break;
            case U: outab(0x64); break;
            default: aerr(); break;
            }
            opcycles = 5;
        }
        else {
            aerr();
        }
        break;

    case S_DEC:
        t1 = addr(&e1);
        if (t1 == S_R8) {
            opcycles = 5;
            switch (e1.e_addr) {
            case A: outab(0xDF); break;
            case XL: outab(0x42); break;
            case YL: outab(0x52); break;
            case UL: outab(0x62); break;
            case XH: outab(0xFD);outab(0x42); opcycles = 9; break;
            case YH: outab(0xFD);outab(0x52); opcycles = 9; break;
            case UH: outab(0xFD);outab(0x62); opcycles = 9; break;
            default: aerr(); break;
            }
        }
        else if (t1 == S_R16) {
            switch (e1.e_addr) {
            case X: outab(0x46); break;
            case Y: outab(0x56); break;
            case U: outab(0x66); break;
            default: aerr(); break;
            }
            opcycles = 5;
        }
        else {
            aerr();
        }
        break;

    case S_CPA:
        t1 = addr(&e1);
        if (t1 == S_R8) {
            switch (e1.e_addr) {
            case XL: outab(0x06); break;
            case YL: outab(0x16); break;
            case UL: outab(0x26); break;
            case XH: outab(0x86); break;
            case YH: outab(0x96); break;
            case UH: outab(0xA6); break;
            default: aerr(); break;
            }
            opcycles = 6;
        }
        else if (t1 == S_R16IND) {
            switch (e1.e_addr) {
            case X: outab(0x07); break;
            case Y: outab(0x17); break;
            case U: outab(0x27); break;
            default: aerr(); break;
            }
            opcycles = 7;
        }
        else if (t1 == S_VALUE) {
            outab(0xA7);
            outrw(&e1, 0);
            opcycles = 13;
        }
        else if (t1 == S_R16INDX) {
            switch (e1.e_addr) {
            case X: outab(0xFD); outab(0x07); break;
            case Y: outab(0xFD); outab(0x17); break;
            case U: outab(0xFD); outab(0x27); break;
            default: aerr(); break;
            }
            opcycles = 11;
        }
        else if (t1 == S_VALINDX) {
            outab(0xFD);
            outab(0xA7);
            outrw(&e1, 0);
            opcycles = 17;
        }
        else {
            aerr();
        }
        break;

    case S_CPI:
        t1 = addr(&e1);
        comma(1);
        t2 = addr(&e2);
        if ((t1 == S_R8) && (t2 == S_VALUE) && (e2.e_addr < 256)) {
            switch (e1.e_addr) {
            case A: outab(0xB7); break;
            case XL: outab(0x4E); break;
            case YL: outab(0x5E); break;
            case UL: outab(0x6E); break;
            case XH: outab(0x4C); break;
            case YH: outab(0x5C); break;
            case UH: outab(0x6C); break;
            default: aerr(); break;
            }
            outrb(&e2, 0);
            opcycles = 7;
        }
        else {
            aerr();
        }
        break;

    case S_BIT:
        t1 = addr(&e1);
        if (t1 == S_R16IND) {
            switch (e1.e_addr) {
            case X: outab(0x0F); break;
            case Y: outab(0x1F); break;
            case U: outab(0x2F); break;
            default: aerr(); break;
            }
            opcycles = 7;
        }
        else if (t1 == S_VALUE) {
            outab(0xAF);
            outrw(&e1, 0);
            opcycles = 13;
        }
        else if (t1 == S_R16INDX) {
            switch (e1.e_addr) {
            case X: outab(0xFD); outab(0x0F); break;
            case Y: outab(0xFD); outab(0x1F); break;
            case U: outab(0xFD); outab(0x2F); break;
            default: aerr(); break;
            }
            opcycles = 11;
        }
        else if (t1 == S_VALINDX) {
            outab(0xFD);
            outab(0xAF);
            outrw(&e1, 0);
            opcycles = 17;
        }
        else {
            aerr();
        }
        break;

    case S_BII:
        t1 = addr(&e1);
        comma(1);
        t2 = addr(&e2);
        if ((t1 == S_R8) && (e1.e_addr == A)) {
            outab(0xBF);
            outrb(&e2, 0);
            opcycles = 7;
        }
        else if (t1 == S_R16IND) {
            switch (e1.e_addr) {
            case X: outab(0x4D); break;
            case Y: outab(0x5D); break;
            case U: outab(0x6D); break;
            default: aerr(); break;
            }
            outrb(&e2, 0);
            opcycles = 10;
        }
        else if (t1 == S_VALUE) {
            outab(0xED);
            outrw(&e1, 0);
            outrb(&e2, 0);
            opcycles = 16;
        }
        else if (t1 == S_R16INDX) {
            switch (e1.e_addr) {
            case X: outab(0xFD); outab(0x4D); break;
            case Y: outab(0xFD); outab(0x5D); break;
            case U: outab(0xFD); outab(0x6D); break;
            default: aerr(); break;
            }
            outrb(&e2, 0);
            opcycles = 14;
        }
        else if (t1 == S_VALINDX) {
            outab(0xFD);
            outab(0xED);
            outrw(&e1, 0);
            outrb(&e2, 0);
            opcycles = 20;
        }
        else {
            aerr();
        }
        break;

    case S_LDA:
        t1 = addr(&e1);
        if (t1 == S_R8) {
            switch (e1.e_addr) {
            case XL: outab(0x04); break;
            case YL: outab(0x14); break;
            case UL: outab(0x24); break;
            case XH: outab(0x84); break;
            case YH: outab(0x94); break;
            case UH: outab(0xA4); break;
            default: aerr(); break;
            }
            opcycles = 5;
        }
        else if (t1 == S_R16IND) {
            switch (e1.e_addr) {
            case X: outab(0x05); break;
            case Y: outab(0x15); break;
            case U: outab(0x25); break;
            default: aerr(); break;
            }
            opcycles = 6;
        }
        else if (t1 == S_VALUE) {
            outab(0xA5);
            outrw(&e1, 0);
            opcycles = 12;
        }
        else if (t1 == S_R16INDX) {
            switch (e1.e_addr) {
            case X: outab(0xFD); outab(0x05); break;
            case Y: outab(0xFD); outab(0x15); break;
            case U: outab(0xFD); outab(0x25); break;
            default: aerr(); break;
            }
            opcycles = 10;
        }
        else if (t1 == S_VALINDX) {
            outab(0xFD);
            outab(0xA5);
            outrw(&e1, 0);
            opcycles = 16;
        }
        else {
            aerr();
        }
        break;

    case S_LDE:
    case S_LIN:
        t1 = addr(&e1);
        if (t1 == S_R16) {
            switch (e1.e_addr) {
            case X: outab(op); break;
            case Y: outab(op + 0x10); break;
            case U: outab(op + 0x20); break;
            default: aerr(); break;
            }
            opcycles = 6;
        }
        else {
            aerr();
        }
        break;

    case S_LDI:
        t1 = addr(&e1);
        comma(1);
        t2 = addr(&e2);
        if ((t1 == S_R8) && (t2 == S_VALUE) && (e2.e_addr < 256)) {
            switch (e1.e_addr) {
            case A: outab(0xB5); break;
            case XL: outab(0x4A); break;
            case YL: outab(0x5A); break;
            case UL: outab(0x6A); break;
            case XH: outab(0x48); break;
            case YH: outab(0x58); break;
            case UH: outab(0x68); break;
            default: aerr(); break;
            }
            outrb(&e2, 0);
            opcycles = 6;
        }
        else if ((t1 == S_R16) && (e1.e_addr == S) && (t2 == S_VALUE)) {
            outab(0xAA);
            outrw(&e2, 0);
            opcycles = 12;
        }
        else {
            aerr();
        }
        break;

    case S_LDX:
        t1 = addr(&e1);
        if (t1 == S_R16) {
            switch (e1.e_addr) {
            case X: outab(0xFD); outab(0x08); break;
            case Y: outab(0xFD); outab(0x18); break;
            case U: outab(0xFD); outab(0x28); break;
            case S: outab(0xFD); outab(0x48); break;
            case P: outab(0xFD); outab(0x58); break;
            default: aerr(); break;
            }
            opcycles = 11;
        }
        else {
            aerr();
        }
        break;

    case S_STA:
        t1 = addr(&e1);
        if (t1 == S_R8) {
            switch (e1.e_addr) {
            case XL: outab(0x0A); break;
            case YL: outab(0x1A); break;
            case UL: outab(0x2A); break;
            case XH: outab(0x08); break;
            case YH: outab(0x18); break;
            case UH: outab(0x28); break;
            default: aerr(); break;
            }
            opcycles = 5;
        }
        else if (t1 == S_R16IND) {
            switch (e1.e_addr) {
            case X: outab(0x0E); break;
            case Y: outab(0x1E); break;
            case U: outab(0x2E); break;
            default: aerr(); break;
            }
            opcycles = 6;
        }
        else if (t1 == S_VALUE) {
            outab(0xAE);
            outrw(&e1, 0);
            opcycles = 12;
        }
        else if (t1 == S_R16INDX) {
            switch (e1.e_addr) {
            case X: outab(0xFD); outab(0x0E); break;
            case Y: outab(0xFD); outab(0x1E); break;
            case U: outab(0xFD); outab(0x2E); break;
            default: aerr(); break;
            }
            opcycles = 10;
        }
        else if (t1 == S_VALINDX) {
            outab(0xFD);
            outab(0xAE);
            outrw(&e1, 0);
            opcycles = 16;
        }
        else {
            aerr();
        }
        break;

    case S_SDE:
    case S_SIN:
        t1 = addr(&e1);
        if (t1 == S_R16) {
            switch (e1.e_addr) {
            case X: outab(op); break;
            case Y: outab(op + 0x10); break;
            case U: outab(op + 0x20); break;
            default: aerr(); break;
            }
            opcycles = 6;
        }
        else {
            aerr();
        }
        break;

    case S_STX:
        t1 = addr(&e1);
        if (t1 == S_R16) {
            switch (e1.e_addr) {
            case X: outab(0xFD); outab(0x4A); break;
            case Y: outab(0xFD); outab(0x5A); break;
            case U: outab(0xFD); outab(0x6A); break;
            case S: outab(0xFD); outab(0x4E); break;
            case P: outab(0xFD); outab(0x5E); break;
            default: aerr(); break;
            }
            opcycles = 11;
        }
        else {
            aerr();
        }
        break;

    case S_PSH:
        t1 = addr(&e1);
        opcycles = 14;
        if (t1 == S_R16) {
            switch (e1.e_addr) {
            case A: outab(0xFD); outab(0xC8); opcycles = 11; break;
            case X: outab(0xFD); outab(0x88); break;
            case Y: outab(0xFD); outab(0x98); break;
            case U: outab(0xFD); outab(0xA8); break;
            default: aerr(); break;
            }
        }
        else {
            aerr();
        }
        break;

    case S_POP:
        t1 = addr(&e1);
        opcycles = 15;
        if (t1 == S_R16) {
            switch (e1.e_addr) {
            case A: outab(0xFD); outab(0x8A); opcycles = 12; break;
            case X: outab(0xFD); outab(0x0A); break;
            case Y: outab(0xFD); outab(0x1A); break;
            case U: outab(0xFD); outab(0x2A); break;
            default: aerr(); break;
            }
        }
        else {
            aerr();
        }
        break;

    case S_DRL:
    case S_DRR:
        t1 = addr(&e1);
        if ((t1 == S_R16IND) && (e1.e_addr == X)) {
            outab(op);
            opcycles = 12;
        }
        else if ((t1 == S_R16INDX) && (e1.e_addr == X)) {
            outab(0xFD); 
            outab(op);
            opcycles = 16;
        }
        else {
            aerr();
        }

        break;

    case S_JMP:
        t1 = addr(&e1);
        outab(0xBA);
        outrw(&e1, 0);
        opcycles = 12;
        break;

    case S_BCH:
    case S_BCS:
    case S_BCR:
    case S_BHS:
    case S_BHR:
    case S_BZS:
    case S_BZR:
    case S_BVS:
    case S_BVR:
        opcycles = OPCY_SDP;
        expr(&e2, 0);
        if (mchpcr(&e2)) {
            if (e2.e_addr >= dot.s_addr) {
                v2 = (int) e2.e_addr - (int) dot.s_addr - 2;
                if (v2 < 256) {
                    outab(op);
                    outab(v2);
                }
                else {
                    aerr();
                }
            }
            else {
                v2 = (int) (e2.e_addr - dot.s_addr - 2);
                if (v2 < -255) {
                    aerr();
                }
                else {
                    outab(op + 0x10);
                    outab(-v2);
                }
            }
        }
        else {
            v2 = (int) e2.e_addr;
            if (v2 >= 0) {
                outab(op);
                outrb(&e2, R_PCR);
            }
            else {
                outab(op + 0x10);
                outab(-v2);
            }
        }
        if (e2.e_mode != S_USER)
            rerr();
        break;


    case S_LOP:
        opcycles = OPCY_SDP;
        expr(&e2, 0);
        if (mchpcr(&e2)) {
            if (e2.e_addr >= dot.s_addr) {
                aerr();
            }
            else {
                v2 = (int) (e2.e_addr - dot.s_addr - 2);
                if (v2 < -255) {
                    aerr();
                }
                else {
                    outab(op);
                    outab(-v2);
                }
            }
        }
        else {
            v2 = (int) e2.e_addr;
            if (v2 >= 0) {
                outab(op);
                outab(v2);
            }
            else {
                outab(op + 0x10);
                outab(-v2);
            }
        }
        if (e2.e_mode != S_USER)
            rerr();
        break;

    case S_SJP:
        t1 = addr(&e1);
        if ((t1 == S_USER) || (t1 == S_VALUE)) {
            outab(0xBE);
            outrw(&e1, 0);
            opcycles = 19;
        }
        else {
            aerr();
        }
        break;

    case S_VEJ:
        t1 = addr(&e1);
        outrb(&e1, 0);
        opcycles = 17;
        break;

    case S_VMJ:
        opcycles = 20;
        t1 = addr(&e1);
        if (t1 == S_VALUE) {
            outab(op);
            outrb(&e1, 0);
        }
        else {
            aerr();
        }
        break;

    case S_VHS:
    case S_VHR:
    case S_VZS:
    case S_VZR:
    case S_VVS:
    case S_VCS:
    case S_VCR:
        opcycles = OPCY_SDP;
        t1 = addr(&e1);
        if (t1 == S_VALUE) {
            outab(op);
            outrb(&e1, 0);
        }
        else {
            aerr();
        }
        break;

    case S_SIMPLE2:
        outab(0xfd);
        outab(op);
        opcycles = opcycles_page_2[op];
        break;

    case S_SIMPLE:
        outab(op);
        opcycles = opcycles_page_1[op];
        break;

	default:
		opcycles = OPCY_ERR;
		err('o');
		break;
	}

}

/*
* Process a machine op.
*/
VOID
machine_z80(mp)
struct mne *mp;
{
    int op, t1, t2;
    struct expr e1, e2;
    int rf, v1, v2;

    clrexpr(&e1);
    clrexpr(&e2);
    op = (int) mp->m_valu;
    rf = mp->m_type;
    if (!mchtyp && rf>S_CPU)
        rf = 0;
    switch (rf) {

    case S_ADC:
        t1 = addr(&e1);
        comma(1);
        t2 = addr(&e2);
        if ((t1 == S_R8) && (e1.e_addr == A)) {
            if (t2 == S_VALUE) {
                outab(0xB3);
                outrb(&e2, 0);
                opcycles = 7;
            }
            else if (t2 == S_R8) {
                switch (e2.e_addr) {
                case B: outab(0x82); break;
                case C: outab(0x02); break;
                case D: outab(0x92); break;
                case E: outab(0x12); break;
                case H: outab(0xA2); break;
                case L: outab(0x22); break;
                default: aerr(); break;
                }
                opcycles = 6;
            }
            else if (t2 == S_VALIND) {
                outab(0xA3);
                outrw(&e2, 0);
                opcycles = 13;
            }
            else if (t2 == S_VALINDX) {
                outab(0xFD);
                outab(0xA3);
                outrw(&e2, 0);
                opcycles = 17;
            }
            else if (t2 == S_R16IND) {
                switch (e2.e_addr) {
                case BC: outab(0x03); break;
                case DE: outab(0x13); break;
                case HL: outab(0x23); break;
                default: aerr(); break;
                }
                opcycles = 7;
            }
            else if (t2 == S_R16INDX) {
                switch (e2.e_addr) {
                case BC: outab(0xFD); outab(0x03); break;
                case DE: outab(0xFD); outab(0x13); break;
                case HL: outab(0xFD); outab(0x23); break;
                default: aerr(); break;
                }
                opcycles = 11;
            }
            else {
                aerr();
            }
        }
        else {
            aerr();
        }
        break;
    case S_ADCD:
        t1 = addr(&e1);
        comma(1);
        t2 = addr(&e2);
        if ((t1 == S_R8) && (e1.e_addr == A)) {
            if (t2 == S_R16IND) {
                switch (e2.e_addr) {
                case BC: outab(0x8C); break;
                case DE: outab(0x9C); break;
                case HL: outab(0xAC); break;
                default: aerr(); break;
                }
                opcycles = 15;
            }
            else if (t2 == S_R16INDX) {
                switch (e2.e_addr) {
                case BC: outab(0xFD); outab(0x8C); break;
                case DE: outab(0xFD); outab(0x9C); break;
                case HL: outab(0xFD); outab(0xAC); break;
                default: aerr(); break;
                }
                opcycles = 19;
            }
            else {
                aerr();
            }
        }
        else {
            aerr();
        }
        break;

    case S_ADD:
        t1 = addr(&e1);
        comma(1);
        t2 = addr(&e2);
        if ((t1 == S_R16) && (t2 == S_R8) && (e2.e_addr == A)) {
            switch (e1.e_addr) {
            case BC: outab(0xFD); outab(0xCA); break;
            case DE: outab(0xFD); outab(0xDA); break;
            case HL: outab(0xFD); outab(0xEA); break;
            default: aerr(); break;
            }
            opcycles = 99;
        }
        else if ((t1 == S_VALIND) && (t2 == S_VALUE)) {
            outab(0xEF);
            outrb(&e2, 0);
            opcycles = 19;
        }
        else if ((t1 == S_VALINDX) && (t2 == S_VALUE)) {
            outab(0xFD);
            outab(0xEF);
            outrb(&e2, 0);
            opcycles = 23;
        }
        else if ((t1 == S_R16IND) && (t2 == S_VALUE)) {
            switch (e1.e_addr) {
            case BC: outab(0x4F); break;
            case DE: outab(0x5F); break;
            case HL: outab(0x6F); break;
            default: aerr(); break;
            }
            outrb(&e2, 0);
            opcycles = 13;
        }
        else if ((t1 == S_R16INDX) && (t2 == S_VALUE)) {
            switch (e1.e_addr) {
            case BC: outab(0xFD); outab(0x4F); break;
            case DE: outab(0xFD); outab(0x5F); break;
            case HL: outab(0xFD); outab(0x6F); break;
            default: aerr(); break;
            }
            outrb(&e2, 0);
            opcycles = 17;
        }
        else {
            aerr();
        }
        break;

    case S_AND:
        t1 = addr(&e1);
        comma(1);
        t2 = addr(&e2);
        if ((t1 == S_R8) && (e1.e_addr == A)) {
            if (t2 == S_VALUE) {
                outab(0xB9);
                outrb(&e2, 0);
                opcycles = 7;
            }
            else if (t2 == S_VALIND) {
                outab(0xE9);
                outrw(&e2, 0);
                opcycles = 19;
            }
            else if (t2 == S_VALINDX) {
                outab(0xFD);
                outab(0xE9);
                outrw(&e2, 0);
                opcycles = 23;
            }
            else if (t2 == S_R16IND) {
                switch (e2.e_addr) {
                case BC: outab(0x09); break;
                case DE: outab(0x19); break;
                case HL: outab(0x29); break;
                default: aerr(); break;
                }
                opcycles = 7;
            }
            else if (t2 == S_R16INDX) {
                switch (e2.e_addr) {
                case BC: outab(0xFD); outab(0x09); break;
                case DE: outab(0xFD); outab(0x19); break;
                case HL: outab(0xFD); outab(0x29); break;
                default: aerr(); break;
                }
                opcycles = 11;
            }
        }
        else if ((t1 == S_VALIND) && (t2 == S_VALUE)) {
            outab(0xE9);
            outrb(&e2, 0);
            opcycles = 19;
        }
        else if ((t1 == S_VALINDX) && (t2 == S_VALUE)) {
            outab(0xFD);
            outab(0xE9);
            outrb(&e2, 0);
            opcycles = 23;
        }
        else if ((t1 == S_R16IND) && (t2 == S_VALUE)) {
            switch (e1.e_addr) {
            case BC: outab(0x49); break;
            case DE: outab(0x59); break;
            case HL: outab(0x69); break;
            default: aerr(); break;
            }
            outrb(&e2, 0);
            opcycles = 13;
        }
        else if ((t1 == S_R16INDX) && (t2 == S_VALUE)) {
            switch (e1.e_addr) {
            case BC: outab(0xFD); outab(0x49); break;
            case DE: outab(0xFD); outab(0x59); break;
            case HL: outab(0xFD); outab(0x69); break;
            default: aerr(); break;
            }
            outrb(&e2, 0);
            opcycles = 17;
        }
        else {
            aerr();
        }
        break;

    case S_CALL:
        expr(&e1, 0);
        outab(0xBE);
        outrw(&e1, 0);
        opcycles = 19;
        break;

    case S_CP:
        t1 = addr(&e1);
        comma(1);
        t2 = addr(&e2);
        if ((t1 == S_R8) && (e1.e_addr == A)) {
            if (t2 == S_VALUE) {
                outab(0xB7);
                outrb(&e2, 0);
                opcycles = 7;
            }
            else if (t2 == S_R8) {
                switch (e2.e_addr) {
                case B: outab(0x86); break;
                case C: outab(0x06); break;
                case D: outab(0x96); break;
                case E: outab(0x16); break;
                case H: outab(0xA6); break;
                case L: outab(0x26); break;
                default: aerr(); break;
                }
                opcycles = 6;
            }
            else if (t2 == S_VALIND) {
                outab(0xA7);
                outrw(&e2, 0);
                opcycles = 13;
            }
            else if (t2 == S_VALINDX) {
                outab(0xFD);
                outab(0xA7);
                outrw(&e2, 0);
                opcycles = 17;
            }
            else if (t2 == S_R16IND) {
                switch (e2.e_addr) {
                case BC: outab(0x07); break;
                case DE: outab(0x17); break;
                case HL: outab(0x27); break;
                default: aerr(); break;
                }
                opcycles = 7;
            }
            else if (t2 == S_R16INDX) {
                switch (e2.e_addr) {
                case BC: outab(0xFD); outab(0x07); break;
                case DE: outab(0xFD); outab(0x17); break;
                case HL: outab(0xFD); outab(0x27); break;
                default: aerr(); break;
                }
                opcycles = 11;
            }
            else {
                aerr();
            }
        }
        else if ((t1 == S_R8) && (t2 == S_VALUE)) {
            switch (e1.e_addr) {
            case B: outab(0x4C); break;
            case C: outab(0x4E); break;
            case D: outab(0x5C); break;
            case E: outab(0x5E); break;
            case H: outab(0x6C); break;
            case L: outab(0x6E); break;
            default: aerr(); break;
            }
            outrb(&e2, 0);
            opcycles = 7;
        }
        else {
            aerr();
        }
        break;

    case S_DEC:
        t1 = addr(&e1);
        if (t1 == S_R8) {
            opcycles = 5;
            switch (e1.e_addr) {
            case A: outab(0xDF); break;
            case B: outab(0xFD); outab(0x42); opcycles = 9; break;
            case C: outab(0x42); break;
            case D: outab(0xFD); outab(0x52); opcycles = 9; break;
            case E: outab(0x52); break;
            case H: outab(0xFD); outab(0x62); opcycles = 9; break;
            case L: outab(0x62); break;
            default: aerr(); break;
            }
        }
        else if (t1 == S_R16) {
            switch (e1.e_addr) {
            case BC: outab(0x46); break;
            case DE: outab(0x56); break;
            case HL: outab(0x66); break;
            default: aerr(); break;
            }
            opcycles = 5;
        }
        else {
            aerr();
        }
        break;

    case S_DJL:
        opcycles = OPCY_SDP;
        expr(&e2, 0);
        if (mchpcr(&e2)) {
            if (e2.e_addr >= dot.s_addr) {
                aerr();
            }
            else {
                v2 = (int) (e2.e_addr - dot.s_addr - 2);
                if (v2 < -255) {
                    aerr();
                }
                else {
                    outab(op);
                    outab(-v2);
                }
            }
        }
        else {
            v2 = (int) e2.e_addr;
            if (v2 >= 0) {
                aerr();
            }
            else {
                outab(op);
                outab(-v2);
            }
        }
        if (e2.e_mode != S_USER)
            rerr();
        break;

    case S_DRL:
    case S_DRR:
        t1 = addr(&e1);
        if ((t1 == S_R16IND) && (e1.e_addr == A)) {
            outab(op);
            opcycles = 12;
        }
        else if ((t1 == S_R16INDX) && (e1.e_addr == A)) {
            outab(0xFD);
            outab(op);
            opcycles = 16;
        }
        else {
            aerr();
        }

        break;

    case S_INC:
        t1 = addr(&e1);
        if (t1 == S_R8) {
            opcycles = 5;
            switch (e1.e_addr) {
            case A: outab(0xDD); break;
            case B: outab(0xFD); outab(0x40); opcycles = 9; break;
            case C: outab(0x40); break;
            case D: outab(0xFD); outab(0x50); opcycles = 9; break;
            case E: outab(0x50); break;
            case H: outab(0xFD); outab(0x60); opcycles = 9; break;
            case L: outab(0x60); break;
            default: aerr(); break;
            }
        }
        else if (t1 == S_R16) {
            switch (e1.e_addr) {
            case BC: outab(0x44); break;
            case DE: outab(0x54); break;
            case HL: outab(0x64); break;
            default: aerr(); break;
            }
            opcycles = 5;
        }
        else {
            aerr();
        }
        break;

    case S_LD:
        t1 = addr(&e1);
        comma(1);
        t2 = addr(&e2);
        if (t1 == S_R8) {
            if (e1.e_addr == A) {
                if (t2 == S_VALUE) {
                    outab(0xB5);
                    outrb(&e2, 0);
                    opcycles = 6;
                }
                else if (t2 == S_R8) {
                    opcycles = 5;
                    switch (e2.e_addr) {
                    case B: outab(0x84); break;
                    case C: outab(0x04); break;
                    case D: outab(0x94); break;
                    case E: outab(0x14); break;
                    case H: outab(0xA4); break;
                    case L: outab(0x24); break;
                    case F: outab(0xFD); outab(0xAA); opcycles = 12; break;
                    case I: outab(0xFD); outab(0xBA); opcycles = 12; break;
                    default: aerr(); break;
                    }
                }
                else if (t2 == S_VALIND) {
                    outab(0xA5);
                    outrw(&e2, 0);
                    opcycles = 12;
                }
                else if (t2 == S_VALINDX) {
                    outab(0xFD);
                    outab(0xA5);
                    outrw(&e2, 0);
                    opcycles = 16;
                }
                else if (t2 == S_R16IND) {
                    switch (e2.e_addr) {
                    case BC: outab(0x05); break;
                    case DE: outab(0x15); break;
                    case HL: outab(0x25); break;
                    default: aerr(); break;
                    }
                    opcycles = 6;
                }
                else if (t2 == S_R16INDX) {
                    switch (e2.e_addr) {
                    case BC: outab(0xFD); outab(0x05); break;
                    case DE: outab(0xFD); outab(0x15); break;
                    case HL: outab(0xFD); outab(0x25); break;
                    default: aerr(); break;
                    }
                    opcycles = 10;
                }
                else {
                    aerr();
                }
            }
            else {// Other 8 bits registers
                if ((t2 == S_R8) && (e2.e_addr == A)) {
                    opcycles = 5;
                    switch (e1.e_addr) {
                    case B: outab(0x08); break;
                    case C: outab(0x0A); break;
                    case D: outab(0x18); break;
                    case E: outab(0x1A); break;
                    case H: outab(0x28); break;
                    case L: outab(0x2A); break;
                    case F:  outab(0xFD); outab(0xEC); opcycles = 99;  break;
                    case T0: outab(0xFD); outab(0xCE); opcycles = 99;  break;
                    case T1: outab(0xFD); outab(0xDE); opcycles = 99;  break;
                    default: aerr(); break;
                    }
                }
                else if (t2 == S_VALUE) {
                    switch (e1.e_addr) {
                    case B: outab(0x48); break;
                    case C: outab(0x4A); break;
                    case D: outab(0x58); break;
                    case E: outab(0x5A); break;
                    case H: outab(0x68); break;
                    case L: outab(0x6A); break;
                    default: aerr(); break;
                    }
                    outrb(&e2, 0);
                }
                else {
                    aerr();
                }
            }
        }
        else if (t1 == S_R16) {
            if ((e1.e_addr == BC) && (t2 == S_R16)) {
                switch (e2.e_addr) {
                case BC: outab(0xFD); outab(0x08); break;
                case DE: outab(0xFD); outab(0x18); break;
                case HL: outab(0xFD); outab(0x28); break;
                case SP: outab(0xFD); outab(0x48); break;
                case PC: outab(0xFD); outab(0x58); break;
                default: aerr(); break;
                }
                opcycles = 11;
            }
            else if ((t2 == S_R16) && (e2.e_addr == BC)) {
                switch (e1.e_addr) {
                case DE: outab(0xFD); outab(0x5A); break;
                case HL: outab(0xFD); outab(0x6A); break;
                case SP: outab(0xFD); outab(0x4E); break;
                default: aerr(); break;
                }
                opcycles = 11;
            }
            else if (t2 == S_VALUE) {
                switch (e1.e_addr) {
                case SP: outab(0xAA); break;
                case PC: outab(0xBA); break;
                default: aerr(); break;
                }
                outrw(&e2, 0);
                opcycles = 12;
            }
            else {
                aerr();
            }
        }
        else if ((t2 == S_R8) && (e2.e_addr == A)) {
            if (t1 == S_VALIND) {
                outab(0xAE);
                outrw(&e1, 0);
                opcycles = 17;
            }
            else if (t1 == S_VALINDX) {
                outab(0xFD);
                outab(0xAE);
                outrw(&e1, 0);
                opcycles = 21;
            }
            else if (t1 == S_R16IND) {
                switch (e1.e_addr) {
                case BC: outab(0x0E); break;
                case DE: outab(0x1E); break;
                case HL: outab(0x2E); break;
                default: aerr(); break;
                }
                opcycles = 6;
            }
            else if (t1 == S_R16INDX) {
                switch (e1.e_addr) {
                case BC: outab(0xFD); outab(0x0E); break;
                case DE: outab(0xFD); outab(0x1E); break;
                case HL: outab(0xFD); outab(0x2E); break;
                default: aerr(); break;
                }
                opcycles = 10;
            }
        }
        else {
            aerr();
        }
        break;

    case S_LDD:
        t1 = addr(&e1);
        comma(1);
        t2 = addr(&e2);
        if ((t1 == S_R8) && (e2.e_addr == A) && (t2 == S_R16IND)) {
            switch (e2.e_addr) {
            case BC: outab(0x47); break;
            case DE: outab(0x57); break;
            case HL: outab(0x67); break;
            default: aerr(); break;
            }
            opcycles = 99;
        }
        else if ((t1 == S_R16IND) && (t2 == S_R8) && (e2.e_addr == A)) {
            switch (e1.e_addr) {
            case BC: outab(0x43); break;
            case DE: outab(0x53); break;
            case HL: outab(0x63); break;
            default: aerr(); break;
            }
            opcycles = 99;
        }
        else {
            aerr();
        }
        break;

    case S_LDI:
        t1 = addr(&e1);
        comma(1);
        t2 = addr(&e2);
        if ((t1 == S_R8) && (e1.e_addr == A) && (t2 == S_R16IND)) {
            switch (e2.e_addr) {
            case BC: outab(0x45); break;
            case DE: outab(0x55); break;
            case HL: outab(0x65); break;
            default: aerr(); break;
            }
            opcycles = 99;
        }
        else if ((t1 == S_R16IND) && (t2 == S_R8) && (e2.e_addr == A)) {
            switch (e1.e_addr) {
            case BC: outab(0x41); break;
            case DE: outab(0x51); break;
            case HL: outab(0x61); break;
            default: aerr(); break;
            }
            opcycles = 99;
        }
        else if ((t1 == S_R16IND) && (e1.e_addr == DE) && (t2 == S_R16IND) && (e2.e_addr == BC)) {
            outab(0xF5);
            opcycles = 99;
        }
        else {
            aerr();
        }
        break;


    case S_OR:
        t1 = addr(&e1);
        comma(1);
        t2 = addr(&e2);

        if ((t1 == S_R8) && (e1.e_addr == A)) {
            if (t2 == S_VALUE) {
                outab(0xBB);
                outrb(&e2, 0);
                opcycles = 7;
            }
            else if (t2 == S_VALIND) {
                outab(0xAB);
                outrw(&e2, 0);
                opcycles = 13;
            }
            else if (t2 == S_VALINDX) {
                outab(0xFD);
                outab(0xAB);
                outrw(&e2, 0);
                opcycles = 17;
            }
            else if (t2 == S_R16IND) {
                switch (e2.e_addr) {
                case BC: outab(0x0B); break;
                case DE: outab(0x1B); break;
                case HL: outab(0x2B); break;
                default: aerr(); break;
                }
                opcycles = 7;
            }
            else if (t2 == S_R16INDX) {
                switch (e2.e_addr) {
                case BC: outab(0xFD); outab(0x0B); break;
                case DE: outab(0xFD); outab(0x1B); break;
                case HL: outab(0xFD); outab(0x2B); break;
                default: aerr(); break;
                }
                opcycles = 11;
            }
            else {
                aerr();
            }
        }
        else if (t2 == S_VALUE) {
            if (t1 == S_VALIND) {
                outab(0xEB);
                outrw(&e1, 0);
                outrb(&e2, 0);
                opcycles = 19;
            }
            else if (t1 == S_VALINDX) {
                outab(0xFD);
                outab(0xEB);
                outrw(&e1, 0);
                outrb(&e2, 0);
                opcycles = 23;
            }
            else if (t1 == S_R16IND) {
                switch (e1.e_addr) {
                case BC: outab(0x4B); break;
                case DE: outab(0x5B); break;
                case HL: outab(0x6B); break;
                default: aerr(); break;
                }
                outrb(&e2, 0);
                opcycles = 13;
            }
            else if (t1 == S_R16INDX) {
                switch (e1.e_addr) {
                case BC: outab(0xFD); outab(0x4B); break;
                case DE: outab(0xFD); outab(0x5B); break;
                case HL: outab(0xFD); outab(0x6B); break;
                default: aerr(); break;
                }
                outrb(&e2, 0);
                opcycles = 17;
            }
            else {
                aerr();
            }
        }
        else {
            aerr();
        }
        break;

    case S_POP:
        t1 = addr(&e1);
        if ((t1 == S_R8) && (e1.e_addr == A)) {
            outab(0xFD);
            outab(0x8A);
            opcycles = 12;
        }
        else if (t1 == S_R16) {
            switch (e1.e_addr) {
            case BC: outab(0xFD); outab(0x0A); break;
            case DE: outab(0xFD); outab(0x1A); break;
            case HL: outab(0xFD); outab(0x2A); break;
            default: aerr(); break;
            }
            opcycles = 15;
        }
        else {
            aerr();
        }
        break;

    case S_PUSH:
        t1 = addr(&e1);
        if ((t1 == S_R8) && (e1.e_addr == A)) {
            outab(0xFD);
            outab(0xC8);
            opcycles = 11;
        }
        else if (t1 == S_R16) {
            switch (e1.e_addr) {
            case BC: outab(0xFD); outab(0x88); break;
            case DE: outab(0xFD); outab(0x98); break;
            case HL: outab(0xFD); outab(0xA8); break;
            default: aerr(); break;
            }
            opcycles = 14;
        }
        else {
            aerr();
        }
        break;

    case S_RET:
        if (more()) {
            t1 = addr(&e1);
            if ((t1 == S_R8) && (e1.e_addr == I)) {
                outab(0x8A);
                opcycles = 99;
            }
            else {
                qerr();
            }
        }
        else {
            outab(0x9A);
        }
        break;

    case S_SBC:
        t1 = addr(&e1);
        comma(1);
        t2 = addr(&e2);

        if ((t1 == S_R8) && (e1.e_addr == A)) {
            if (t2 == S_VALUE) {
                outab(0xB1);
                outrb(&e2, 0);
                opcycles = 7;
            }
            else if (t2 == S_R8) {
                switch (e2.e_addr) {
                case B: outab(0x80); break;
                case C: outab(0x00); break;
                case D: outab(0x90); break;
                case E: outab(0x10); break;
                case H: outab(0xA0); break;
                case L: outab(0x20); break;
                default: aerr(); break;
                }
                opcycles = 6;
            }
            else if (t2 == S_VALIND) {
                outab(0xA1);
                outrw(&e2, 0);
                opcycles = 99;
            }
            else if (t2 == S_VALINDX) {
                outab(0xFD);
                outab(0xA1);
                outrw(&e2, 0);
                opcycles = 99;
            }
            else if (t2 == S_R16IND) {
                switch (e2.e_addr) {
                case BC: outab(0x01); break;
                case DE: outab(0x11); break;
                case HL: outab(0x21); break;
                default: aerr(); break;
                }
                opcycles = 7;
            }
            else if (t2 == S_R16INDX) {
                switch (e2.e_addr) {
                case BC: outab(0xFD); outab(0x01); break;
                case DE: outab(0xFD); outab(0x11); break;
                case HL: outab(0xFD); outab(0x21); break;
                default: aerr(); break;
                }
                opcycles = 11;
            }
            else {
                aerr();
            }
        }
        else {
            aerr();
        }
        break;

    case S_SBCD:
        t1 = addr(&e1);
        comma(1);
        t2 = addr(&e2);

        if ((t1 == S_R8) && (e1.e_addr == A)) {
            if (t2 == S_R16IND) {
                switch (e2.e_addr) {
                case BC: outab(0x0C); break;
                case DE: outab(0x1C); break;
                case HL: outab(0x2C); break;
                default: aerr(); break;
                }
                opcycles = 13;
            }
            else if (t2 == S_R16INDX) {
                switch (e2.e_addr) {
                case BC: outab(0xFD); outab(0x0C); break;
                case DE: outab(0xFD); outab(0x1C); break;
                case HL: outab(0xFD); outab(0x2C); break;
                default: aerr(); break;
                }
                opcycles = 17;
            }
            else {
                aerr();
            }
        }
        else {
            aerr();
        }
        break;

    case S_SBR:
        opcycles = 99;
        if ((v1 = admode(CND)) != 0) {
            comma(1);
            t2 = addr(&e2);
            if (t2 == S_VALUE) {
                switch (v1) {
                case NZ:    outab(0xC9); break;
                case Z:     outab(0xCB); break;
                case NC:    outab(0xC1); break;
                case CS:    outab(0xC3); break;
                case NHS:   outab(0xC5); break;
                case HS:    outab(0xC7); break;
                case V:     outab(0xCF); break;
                default:    aerr(); break;
                }
            }
            else {
                aerr();
            }
        }
        else {
            t1 = addr(&e1);
            if (t1 == S_VALUE) {
                switch (e1.e_addr) {
                case 0xC0:
                case 0xC2:
                case 0xC4:
                case 0xC6:
                case 0xC8:
                case 0xCA:
                case 0xCC:
                case 0xCE:
                case 0xD0:
                case 0xD2:
                case 0xD4:
                case 0xD6:
                case 0xD8:
                case 0xDA:
                case 0xDC:
                case 0xDE:
                case 0xE0:
                case 0xE2:
                case 0xE4:
                case 0xE6:
                case 0xE8:
                case 0xEA:
                case 0xEC:
                case 0xEE:
                case 0xF0:
                case 0xF2:
                case 0xF4:
                case 0xF6:
                case 0xF8:
                case 0xFA:
                case 0xFC:
                case 0xFE:
                    outrb(&e1, 0);
                    break;
                default:
                    outab(0xCD);
                    outrb(&e1, 0);
                    break;
                }
            }
            else {
                aerr();
            }
        }
        break;

    case S_TEST:
        t1 = addr(&e1);
        comma(1);
        t2 = addr(&e2);
        if ((t1 == S_R8) && (e1.e_addr == A)) {
            if (t2 == S_VALUE) {
                outab(0xBF);
                outrb(&e2, 0);
                opcycles = 7;
            }
            else if (t2 == S_VALIND) {
                outab(0xAF);
                outrw(&e2, 0);
                opcycles = 13;
            }
            else if (t2 == S_VALINDX) {
                outab(0xFD);
                outab(0xAF);
                outrw(&e2, 0);
                opcycles = 17;
            }
            else if (t2 == S_R16IND) {
                switch (e2.e_addr) {
                case BC: outab(0x0F); break;
                case DE: outab(0x1F); break;
                case HL: outab(0x2F); break;
                default: aerr(); break;
                }
                opcycles = 7;
            }
            else if (t2 == S_R16INDX) {
                switch (e2.e_addr) {
                case BC: outab(0xFD); outab(0x0F); break;
                case DE: outab(0xFD); outab(0x1F); break;
                case HL: outab(0xFD); outab(0x2F); break;
                default: aerr(); break;
                }
                opcycles = 11;
            }
            else {
                aerr();
            }
        }
        else if (t2 == S_VALUE) {
            if (t1 == S_VALIND) {
                outab(0xED);
                outrw(&e1, 0);
                outrb(&e2, 0);
                opcycles = 16;
            }
            else if (t1 == S_VALINDX) {
                outab(0xFD);
                outab(0xED);
                outrw(&e1, 0);
                outrb(&e2, 0);
                opcycles = 20;
            }
            else if (t1 == S_R16IND) {
                switch (e1.e_addr) {
                case BC: outab(0x4D); break;
                case DE: outab(0x5D); break;
                case HL: outab(0x6D); break;
                default: aerr(); break;
                }
                outrb(&e2, 0);
                opcycles = 10;
            }
            else if (t1 == S_R16INDX) {
                switch (e1.e_addr) {
                case BC: outab(0xFD); outab(0x4D); break;
                case DE: outab(0xFD); outab(0x5D); break;
                case HL: outab(0xFD); outab(0x6D); break;
                default: aerr(); break;
                }
                outrb(&e2, 0);
                opcycles = 14;
            }
            else {
                aerr();
            }
        }
        else {
            aerr();
        }
        break;

    case S_XOR:
        t1 = addr(&e1);
        comma(1);
        t2 = addr(&e2);
        if ((t1 == S_R8) && (e1.e_addr == A)) {
            if (t2 == S_VALUE) {
                outab(0xBD);
                outrb(&e2, 0);
                opcycles = 7;
            }
            else if (t2 == S_VALIND) {
                outab(0xAD);
                outrw(&e2, 0);
                opcycles = 13;
            }
            else if (t2 == S_VALINDX) {
                outab(0xFD);
                outab(0xAD);
                outrw(&e2, 0);
                opcycles = 17;
            }
            else if (t2 == S_R16IND) {
                switch (e2.e_addr) {
                case BC: outab(0x0D); break;
                case DE: outab(0x1D); break;
                case HL: outab(0x2D); break;
                default: aerr(); break;
                }
                opcycles = 7;
            }
            else if (t2 == S_R16INDX) {
                switch (e2.e_addr) {
                case BC: outab(0xFD); outab(0x0D); break;
                case DE: outab(0xFD); outab(0x1D); break;
                case HL: outab(0xFD); outab(0x2D); break;
                default: aerr(); break;
                }
                opcycles = 11;
            }
            else {
                aerr();
            }
        }
        else {
            aerr();
        }
        break;

    case S_JR:
        opcycles = 99;
        if ((v1 = admode(CND)) != 0) {
            switch (v1 & 0xFF) {
            case CS: op = 0x83; comma(0); break;
            case HS: op = 0x87; comma(0); break;
            case NC: op = 0x81; comma(0); break;
            case NHS: op = 0x85; comma(0); break;
            case NV: op = 0x8D; comma(0); break;
            case NZ: op = 0x89; comma(0); break;
            case V: op = 0x8F; comma(0); break;
            case Z: op = 0x8B; comma(0); break;
            default: aerr(); break;
            }
        }
        else {
            op = 0x8E;
        }
        expr(&e2, 0);
        if (mchpcr(&e2)) {
            if (e2.e_addr >= dot.s_addr) {
                v2 = (int) e2.e_addr - (int) dot.s_addr - 2;
                if (v2 < 256) {
                    outab(op);
                    outab(v2);
                }
                else {
                    aerr();
                }
            }
            else {
                v2 = (int) (e2.e_addr - dot.s_addr - 2);
                if (v2 < -255) {
                    aerr();
                }
                else {
                    outab(op + 0x10);
                    outab(-v2);
                }
            }
        }
        else {
            v2 = (int) e2.e_addr;
            if (v2 >= 0) {
                outab(op);
                outab(v2);
            }
            else {
                outab(op + 0x10);
                outab(-v2);
            }
        }

        break;

        // Simples codes
    case S_SIMPLE2:
        outab(0xFD);
        outab(op);
        opcycles = opcycles_page_2[op];
        break;

    case S_CPI: // has different meaning in LH5801, cannot be replaced by simple
    case S_SIMPLE:
        outab(op);
        opcycles = opcycles_page_1[op];
        break;

    default:
        opcycles = OPCY_ERR;
        err('o');
        break;
    }
}

VOID
machine(mp)
struct mne *mp;
{
    if (mp->m_type == S_CPU) {
        opcycles = OPCY_CPU;
        sym[2].s_addr = mchtyp = (int) mp->m_valu;
        lmode = SLIST;
    }
    else if (mchtyp == X_LH5801) {
        machine_lh5801(mp);
    }
    else {
        machine_z80(mp);
    }
}

/*
 * Branch/Jump PCR Mode Check
 */
int
mchpcr(esp)
struct expr *esp;
{
	if (esp->e_base.e_ap == dot.s_area) {
		return(1);
	}
	if (esp->e_flag==0 && esp->e_base.e_ap==NULL) {
		/*
		 * Absolute Destination
		 *
		 * Use the global symbol '.__.ABS.'
		 * of value zero and force the assembler
		 * to use this absolute constant as the
		 * base value for the relocation.
		 */
		esp->e_flag = 1;
		esp->e_base.e_sp = &sym[1];
	}
	return(0);
}

/*
 * Machine dependent initialization
 */
VOID
minit()
{
	/*
	 * Byte Order
	 */
	hilo = 1;

	if (pass == 0) {
        mchtyp = X_LH5801;
        sym[2].s_addr = X_LH5801;
	}
}

