all: inverse.txt hexconv.txt shiftleft.txt shiftright.txt using.txt \
	inverse_z80.rel hexconv_z80.rel shiftleft_z80.rel shiftright_z80.rel using_z80.rel

ASMLH5801=..\Debug\aslh5801.exe
REL2BAS=..\Debug\rel2bas.exe

clean:
	del *.rel *.lst *.sym inverse.txt hexconv.txt shiftleft.txt shiftright.txt using.txt



inverse_z80.rel: inverse_z80.asm makefile
	$(ASMLH5801) -o -s -l inverse_z80.asm

inverse.rel: inverse.asm Makefile
	$(ASMLH5801) -o -s -l inverse.asm
	
hexconv_z80.rel: hexconv_z80.asm Makefile
	$(ASMLH5801) -o -s -l hexconv_z80.asm
	
hexconv.rel: hexconv.asm Makefile
	$(ASMLH5801) -o -s -l hexconv.asm

shiftleft_z80.rel: shiftleft_z80.asm Makefile
	$(ASMLH5801) -o -s -l shiftleft_z80.asm
	
shiftleft.rel: shiftleft.asm Makefile
	$(ASMLH5801) -o -s -l shiftleft.asm

shiftright_z80.rel: shiftright_z80.asm Makefile
	$(ASMLH5801) -o -s -l shiftright_z80.asm

shiftright.rel: shiftright.asm Makefile
	$(ASMLH5801) -o -s -l shiftright.asm
	
using_z80.rel: using_z80.asm Makefile
	$(ASMLH5801) -o -s -l using_z80.asm
	
using.rel: using.asm Makefile
	$(ASMLH5801) -o -s -l using.asm

inverse.txt:	inverse.rel
	$(REL2BAS) inverse.rel > inverse.txt

hexconv.txt:	hexconv.rel
	$(REL2BAS) hexconv.rel > hexconv.txt

shiftleft.txt:	shiftleft.rel
	$(REL2BAS) shiftleft.rel > shiftleft.txt

shiftright.txt:	shiftright.rel
	$(REL2BAS) shiftright.rel > shiftright.txt

using.txt:	using.rel
	$(REL2BAS) using.rel > using.txt


	