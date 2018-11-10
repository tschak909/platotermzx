#ifdef __MSX__
#include "asm.h"

void AsmCall(uint address, Z80_registers* regs, register_usage inRegistersDetail, register_usage outRegistersDetail) __naked
{
#asm
    push ix
    ld ix,#4
    add ix,sp

    ld l,(ix)
    ld h,(ix+1)
    ld e,(ix+2) 
    ld d,(ix+3)
    push de
    ld a,(ix+5) 
    push af
    ld a,(ix+4)

    push de
    pop ix

    ld de,#CONT
    push de
    push hl

    or a
    ret z   

    exx
    ld l,(ix)
    ld h,(ix+1)
    dec a
    jr z,ASMRUT_DOAF
    exx

    ld c,(ix+2)
    ld b,(ix+3)
    ld e,(ix+4)
    ld d,(ix+5)
    ld l,(ix+6)
    ld h,(ix+7)
    dec a
    exx
    jr z,ASMRUT_DOAF

    ld c,(ix+8)
    ld b,(ix+9)
    ld e,(ix+10)
    ld d,(ix+11)
    push de
    push bc
    pop ix
    pop iy

ASMRUT_DOAF:
    push hl
    pop af
    exx

    ret
		   
CONT:

    ex af,af
    pop af
    ex (sp),ix 

    or a
    jr z,CALL_END

    exx
    ex af,af
    push af
    pop hl
    ld (ix),l
    ld (ix+1),h
    exx
    ex af,af
    dec a
    jr z,CALL_END

    ld (ix+2),c
    ld (ix+3),b
    ld (ix+4),e
    ld (ix+5),d
    ld (ix+6),l
    ld (ix+7),h
    dec	a
    jr z,CALL_END

    exx
    pop hl
    ld (ix+8),l
    ld (ix+9),h
    push iy
    pop hl
    ld (ix+10),l
    ld (ix+11),h
    exx

    ex af,af
    pop ix
    ret

CALL_END:
    ex af,af
    pop hl
    pop ix
    ret

#endasm
}
#endif /* __MSX__ */
