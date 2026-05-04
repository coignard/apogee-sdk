
;  This file is part of Apogee SDK.
;
;  Copyright (c) 2026 René Coignard
;
;  This software is provided 'as-is', without any express or implied
;  warranty. In no event will the authors be held liable for any damages
;  arising from the use of this software.
;
;  Permission is granted to anyone to use this software for any purpose,
;  including commercial applications, and to alter it and redistribute it
;  freely, subject to the following restrictions:
;
;  1. The origin of this software must not be misrepresented; you must not
;     claim that you wrote the original software. If you use this software
;     in a product, an acknowledgment in the product documentation would be
;     appreciated but is not required.
;  2. Altered source versions must be plainly marked as such, and must not be
;     misrepresented as being the original software.
;  3. This notice may not be removed or altered from any source distribution.

macro format?.rka? start_addr:0000h
    local file_size, end_addr, cs_lo, cs_hi, byte_val
    db (start_addr shr 8) and 0FFh
    db start_addr and 0FFh
    db 0, 0
    org start_addr
    postpone ?
        file_size = $%% - 4
        end_addr  = start_addr + file_size - 1
        store end_addr shr 8    : byte at : 2
        store end_addr and 0FFh : byte at : 3
        cs_lo = 0
        cs_hi = 0
        repeat file_size
            load byte_val : byte from : 4 + % - 1
            cs_lo = cs_lo + byte_val
            cs_hi = cs_hi + byte_val + (cs_lo shr 8)
            cs_lo = cs_lo and 0FFh
            cs_hi = cs_hi and 0FFh
        end repeat
        db 00h, 00h, 0E6h
        db cs_hi, cs_lo
    end postpone
end macro

virtual at 0
    apogee_charset_map::
    db 61h, 62h, 77h, 67h, 64h, 65h, 76h, 7Ah, 69h, 6Ah, 6Bh, 6Ch, 6Dh, 6Eh, 6Fh, 70h
    db 72h, 73h, 74h, 75h, 66h, 68h, 63h, 7Eh, 7Bh, 7Dh, 7Fh, 79h, 78h, 7Ch, 60h, 71h
end virtual

macro dba? args&
    iterate arg, args
        if arg eqtype ''
            local utf8, i, b, b2, code, mapped
            utf8 = arg
            i = 0
            while i < lengthof utf8
                b = (utf8 shr (i*8)) and 0FFh
                i = i + 1
                if b >= 0D0h & b <= 0D1h
                    b2 = (utf8 shr (i*8)) and 0FFh
                    i = i + 1
                    if b = 0D0h & b2 >= 90h & b2 <= 0AFh
                        code = b2 - 90h
                    else if b = 0D0h & b2 >= 0B0h & b2 <= 0BFh
                        code = b2 - 0B0h
                    else if b = 0D1h & b2 >= 80h & b2 <= 8Fh
                        code = b2 - 80h + 10h
                    else if (b = 0D0h & b2 = 81h) | (b = 0D1h & b2 = 91h)
                        code = 5
                    else
                        code = 3Fh
                    end if
                    if code < 32
                        load mapped : byte from apogee_charset_map:code
                        db mapped
                    else
                        db '?'
                    end if
                else
                    if b >= 'a' & b <= 'z'
                        b = b - 20h
                    end if
                    db b
                end if
            end while
        else
            db arg
        end if
    end iterate
end macro

struc dba? args&
    label . : byte
    db args
end struc

; memory map
APOGEE_RAM_START  := 0000h
APOGEE_RAM_END    := 0EBFFh
APOGEE_SCREEN_RAM := 0E1D0h
APOGEE_MON_WORK   := 0E100h
APOGEE_ROM1_BASE  := 0F800h
APOGEE_ROM2_BASE  := 0F000h

; KR580VG75
VG75_DATA := 0EF00h
VG75_CMD  := 0EF01h

VG75_COLS   := 78
VG75_ROWS   := 30
VG75_MEM_SZ := VG75_COLS * VG75_ROWS

VG75_CMD_RESET      := 00h
VG75_CMD_START_DISP := 27h
VG75_CMD_STOP_DISP  := 40h
VG75_CMD_LOAD_CURS  := 80h
VG75_CMD_READ_LPEN  := 60h
VG75_CMD_ENABLE_INT := 0A0h
VG75_CMD_DISAB_INT  := 0C0h
VG75_CMD_PRESET     := 0E0h

VG75_ATTR_HIGHLIGHT := 01h
VG75_ATTR_BLINK     := 02h
VG75_ATTR_GPA0      := 04h
VG75_ATTR_GPA1      := 08h
VG75_ATTR_REVERSE   := 10h
VG75_ATTR_UNDERLINE := 20h

VG75_ATTR_MINUS_RED   := VG75_ATTR_HIGHLIGHT
VG75_ATTR_MINUS_GREEN := VG75_ATTR_GPA1
VG75_ATTR_MINUS_BLUE  := VG75_ATTR_GPA0

VG75_STAT_INT_EN   := 40h
VG75_STAT_INT_REQ  := 20h
VG75_STAT_BAD_CMD  := 08h
VG75_STAT_DISP_EN  := 04h
VG75_STAT_DMA_ERR  := 02h
VG75_STAT_FIFO_ERR := 01h

VG75_INIT_P1_SPACED_ROWS := 80h
VG75_INIT_P4_FONT_DOWN   := 80h
VG75_INIT_P4_ATTR_TRANSP := 40h
VG75_INIT_P4_CURS_BLINK  := 20h
VG75_INIT_P4_CURS_UNDER  := 10h

VG75_CODE_EOR := 0F1h
VG75_CODE_EOS := 0F3h

VG75_MARKER_ATTR   := 80h
VG75_MARKER_PSEUDO := 0C0h

; KR580VT57
VT57_CH2_ADDR  := 0F004h
VT57_CH2_COUNT := 0F005h
VT57_CH3_ADDR  := 0F006h
VT57_CH3_COUNT := 0F007h
VT57_MODE      := 0F008h

VT57_MODE_ENABLE_CH2 := 04h
VT57_MODE_AUTO_LOAD  := 80h

VT57_TYPE_VERIFY := 0000h
VT57_TYPE_WRITE  := 4000h
VT57_TYPE_READ   := 8000h

VT57_VIDEO_LEN := VT57_TYPE_WRITE + VG75_MEM_SZ - 1

; KR580VI53
VI53_CH0 := 0EC00h
VI53_CH1 := 0EC01h
VI53_CH2 := 0EC02h
VI53_CWR := 0EC03h

VI53_CWR_CH0      := 00h
VI53_CWR_CH1      := 40h
VI53_CWR_CH2      := 80h
VI53_CWR_RW_LATCH := 00h
VI53_CWR_RW_LSB   := 10h
VI53_CWR_RW_MSB   := 20h
VI53_CWR_RW_BOTH  := 30h
VI53_CWR_MODE0    := 00h
VI53_CWR_MODE3    := 06h

; KR580VV55A system
VV55_SYS_PORT_A := 0ED00h
VV55_SYS_PORT_B := 0ED01h
VV55_SYS_PORT_C := 0ED02h
VV55_SYS_CWR    := 0ED03h

VV55_SYS_PORT_C_TAPE_OUT := 01h
VV55_SYS_PORT_C_SHIFT    := 20h
VV55_SYS_PORT_C_CTRL     := 40h
VV55_SYS_PORT_C_ALT      := 80h

VV55_SYS_BEEP_ON  := 01h
VV55_SYS_BEEP_OFF := 00h
VV55_SYS_INIT_CWR := 8Ah

; KR580VV55A user
VV55_USR_PORT_A := 0EE00h
VV55_USR_PORT_B := 0EE01h
VV55_USR_PORT_C := 0EE02h
VV55_USR_CWR    := 0EE03h

; ROM-disk
ROMDISK_PORT_DATA   := VV55_USR_PORT_A
ROMDISK_PORT_ADDR_L := VV55_USR_PORT_B
ROMDISK_PORT_ADDR_H := VV55_USR_PORT_C
ROMDISK_BANK_SELECT := 80h

; MIDI
MIDI_PORT_DATA   := VV55_USR_PORT_A
MIDI_PORT_CTRL   := VV55_USR_PORT_C
MIDI_CTRL_STROBE := 01h

; monitor ROM
MON_START          := 0F800h
MON_CHAR_IN_WAIT   := 0F803h
MON_TAPE_BYTE_IN   := 0F806h
MON_CHAR_OUT       := 0F809h
MON_TAPE_BYTE_OUT  := 0F80Ch
MON_KBD_SCAN       := 0F812h
MON_HEX_OUT        := 0F815h
MON_MSG_OUT        := 0F818h
MON_CHAR_IN_NOWAIT := 0F81Bh
MON_CURSOR_POS     := 0F81Eh
MON_CURSOR_CHAR    := 0F821h
MON_TAPE_BLOCK_IN  := 0F824h
MON_TAPE_BLOCK_OUT := 0F827h
MON_CHECKSUM       := 0F82Ah
MON_SCREEN_START   := 0F82Dh
MON_MEM_TOP_GET    := 0F830h
MON_MEM_TOP_SET    := 0F833h

MON_MSX_BYTE_OUT  := 0F003h
MON_MSX_BYTE_IN   := 0F006h
MON_MSX_BLOCK_OUT := 0F009h
MON_MSX_BLOCK_IN  := 0F00Ch
MON_MSX_LMARK_OUT := 0F00Fh
MON_MSX_SMARK_OUT := 0F012h
MON_MSX_MARK_IN   := 0F015h

; monitor work area
MON_CURSOR_ADDR  := 0E100h
MON_CURSOR_X     := 0E102h
MON_CURSOR_Y     := 0E103h
MON_ESC_FLAG     := 0E104h
MON_KBD_EN_FLAG  := 0E105h
MON_LANG_FLAG    := 0E106h
MON_KEY_CODE     := 0E109h
MON_KBD_REP_FLAG := 0E10Bh
MON_SP_TAPE      := 0E10Dh
MON_PC_CMD       := 0E114h
MON_HL_CMD       := 0E116h
MON_BC_CMD       := 0E118h
MON_DE_CMD       := 0E11Ah
MON_SP_CMD       := 0E11Ch
MON_AF_CMD       := 0E11Eh
MON_MSX_BYTE     := 0E120h
MON_MSX_CONST    := 0E121h
MON_M_CONST      := 0E122h
MON_BRK_ADDR     := 0E123h
MON_BRK_BYTE     := 0E125h
MON_G_JMP        := 0E126h
MON_ARG1         := 0E127h
MON_ARG2         := 0E129h
MON_ARG3         := 0E12Bh
MON_ARGS_FLAG    := 0E12Dh
MON_TAPE_DIR     := 0E12Eh
MON_TAPE_R_CONST := 0E12Fh
MON_TAPE_W_CONST := 0E130h
MON_RAM_TOP      := 0E131h
MON_CMD_BUFFER   := 0E133h
MON_MSX_W_NAME   := 0E152h
MON_MSX_R_NAME   := 0E159h

; keyboard
KBD_COL_TAB       := 01h
KBD_COL_PGDN      := 02h
KBD_COL_ENTER     := 04h
KBD_COL_BACKSPACE := 08h
KBD_COL_LEFT      := 10h
KBD_COL_UP        := 20h
KBD_COL_RIGHT     := 40h
KBD_COL_DOWN      := 80h

; display codes
SYM_BEEP     := 07h
SYM_LEFT     := 08h
SYM_LINEFEED := 0Ah
SYM_HOME     := 0Ch
SYM_RETURN   := 0Dh
SYM_RIGHT    := 18h
SYM_UP       := 19h
SYM_DOWN     := 1Ah
SYM_ESC      := 1Bh
SYM_ESC_Y    := 59h
SYM_CLEAR    := 1Fh
