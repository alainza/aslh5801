/* z80adr.c */

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
 * Read an address specifier. Pack the
 * address information into the supplied
 * `expr' structure. Return the mode of
 * the address.
 *
 * This addr(esp) routine performs the following addressing decoding:
 *
 *	address		mode		flag		addr		base
 *	#n		S_IMMED		0		n		NULL
 *	label		s_type		----		s_addr		s_area
 *	[REG]		S_IND+icode	0		0		NULL
 *	[label]		S_INDM		----		s_addr		s_area
 *	offset[REG]	S_IND+icode	----		offset		----
 */
int
addr(esp)
struct expr *esp;
{
    int c, mode, indx;

    if ((c = getnb()) == '-') {
        mode = S_VALNEG;
        expr(esp, 0);
        esp->e_mode = mode;
    }
    else if (c == '0') {
        unget(c);
        expr(esp, 0);
        esp->e_mode = S_VALUE;
    }
    else if (c == '#') {
        int c2 = getnb();
        if (c2 != LFIND) {
            qerr();
        }
        else {
            c2 = getnb();
            // Should be a 16 bits register
            if ((indx = admode(R16)) != 0) {
                mode = S_R16INDX;
            }
            else {
                mode = S_VALINDX;
                expr(esp, 0);
                esp->e_mode = mode;
            }
            if ((c = getnb()) != RTIND) {
                qerr();
            }
        }
    }
    else if (c == LFIND) {
        if ((indx = admode(R16)) != 0) {
            mode = S_R16IND;
            esp->e_addr = indx & 0xFF;
            esp->e_mode = mode;
            esp->e_base.e_ap = NULL;
        }
        else {
            mode = S_VALIND;
            expr(esp, 0);
            esp->e_mode = mode;
        }
        if ((c = getnb()) != RTIND) {
            qerr();
        }
    }
    else {
        unget(c);
        if ((indx = admode(R8)) != 0) {
            mode = S_R8;
        }
        else
        if ((indx = admode(R16)) != 0) {
            mode = S_R16;
        }
        else {
            mode = S_USER;
            expr(esp, 0);
            esp->e_mode = mode;
        }
        if (indx) {
            esp->e_addr = indx & 0xFF;
            esp->e_mode = mode;
            esp->e_base.e_ap = NULL;
        }
    }
    return (esp->e_mode);
}

/*
 * Enter admode() to search a specific addressing mode table
 * for a match. Return the addressing value on a match or
 * zero for no match.
 */
int
admode(sp)
struct adsym *sp;
{
	char *ptr;
	int i;
	char *ips;

	ips = ip;
	unget(getnb());

	i = 0;
	while ( *(ptr = &sp[i].a_str[0]) ) {
		if (srch(ptr)) {
			return(sp[i].a_val);
		}
		i++;
	}
	ip = ips;
	return(0);
}

/*
 *      srch --- does string match ?
 */
int
srch(str)
char *str;
{
	char *ptr;
	ptr = ip;

	while (*ptr && *str) {
		if (ccase[*ptr & 0x007F] != ccase[*str & 0x007F])
			break;
		ptr++;
		str++;
	}
	if (ccase[*ptr & 0x007F] == ccase[*str & 0x007F]) {
		ip = ptr;
		return(1);
	}

	if (!*str)
		if (!(ctype[*ptr & 0x007F] & LTR16)) {
			ip = ptr;
			return(1);
		}
	return(0);
}

/*
 * Registers
 */

struct	adsym	R8[] = {
    {	"a",	A | 0400	},
    {	"xh",	XH | 0400	},
    {	"xl",	XL | 0400	},
    {	"yh",	YH | 0400	},
    {	"yl",	YL | 0400	},
    {	"uh",	UH | 0400	},
    {	"ul",	UL | 0400	},
    {   "ph",   PH | 0400 },
    {   "pl",   PL | 0400 },
    {   "sh",   SH | 0400 },
    {   "sl",   SL | 0400 },
    { "b", B | 0400 },
    { "c", C | 0400 },
    { "d", D | 0400 },
    { "e", E | 0400 },
    { "h", H | 0400 },
    { "l", L | 0400 },
    { "i", I | 0400 },
    { "f", F | 0400 },
    { "t0", T0 | 0400 },
    { "t1", T1 | 0400 },
    { "", 0000 }
};

struct	adsym	R16[] = {
    {	"x",	X | 0400 },
    {	"y",	Y | 0400 },
    {   "u",    U | 0400 },
    {   "p",    P | 0400 },
    {   "s",    S | 0400 },
    { "bc", BC | 0400 },
    { "de", DE | 0400 },
    { "hl", HL | 0400 },
    { "sp", SP | 0400 },
    { "pc", PC | 0400 },
    { "", 0000 }
};

/*
* Conditional definitions
*/

struct	adsym	CND[] = {
    { "nz", NZ | 0400 },
    { "z", Z | 0400 },
    { "nc", NC | 0400 },
    { "c", CS | 0400 },
    { "nh", NHS | 0400 },
    { "h", HS | 0400 },
    { "v", V | 0400 },
    { "", 0000 }
};
