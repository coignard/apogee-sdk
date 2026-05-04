
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

element reg8
element B?   : reg8 + 0
element C?   : reg8 + 1
element D?   : reg8 + 2
element E?   : reg8 + 3
element H?   : reg8 + 4
element L?   : reg8 + 5
element M?   : reg8 + 6
element A?   : reg8 + 7

element reg16
element SP?  : reg16 + 3
element PSW? : reg16 + 4

calminstruction DS? count*
    emit    count
end calminstruction

macro make_alu name, opcode
    calminstruction name? src*
        local   s
        compute s, src
        check   s eq s element 1 & s metadata 1 relativeto reg8
        jno     error
        compute s, s metadata 1 - reg8
        emit    1, opcode or s
        exit
    error:
        err     'incorrect argument'
    end calminstruction
end macro

macro make_alu_imm name, opcode
    calminstruction name? imm*
        emit    1, opcode
        emit    1, imm
    end calminstruction
end macro

macro make_incdec name, opcode
    calminstruction name? dst*
        local   d
        compute d, dst
        check   d eq d element 1 & d metadata 1 relativeto reg8
        jno     error
        compute d, d metadata 1 - reg8
        emit    1, opcode or (d shl 3)
        exit
    error:
        err     'incorrect argument'
    end calminstruction
end macro

macro make_rp_op name, opcode, r16_allowed
    calminstruction name? rp*
        local   r
        compute r, rp
        check   r eq r element 1 & r metadata 1 relativeto reg8
        jyes    r8
        check   r eq r element 1 & r metadata 1 relativeto reg16
        jyes    r16
        jump    error
    r8:
        compute r, r metadata 1 - reg8
        check   (r = 0) | (r = 2) | (r = 4)
        jno     error
        emit    1, opcode or ((r shr 1) shl 4)
        exit
    r16:
        compute r, r metadata 1 - reg16
        check   r = r16_allowed
        jno     error
        emit    1, opcode or (3 shl 4)
        exit
    error:
        err     'incorrect argument'
    end calminstruction
end macro

macro make_rp_imm name, opcode, r16_allowed
    calminstruction name? args&
        local   rp, imm, r
        match   rp=,imm, args
        jno     error
        compute r, rp
        check   r eq r element 1 & r metadata 1 relativeto reg8
        jyes    r8
        check   r eq r element 1 & r metadata 1 relativeto reg16
        jyes    r16
        jump    error
    r8:
        compute r, r metadata 1 - reg8
        check   (r = 0) | (r = 2) | (r = 4)
        jno     error
        emit    1, opcode or ((r shr 1) shl 4)
        emit    2, imm
        exit
    r16:
        compute r, r metadata 1 - reg16
        check   r = r16_allowed
        jno     error
        emit    1, opcode or (3 shl 4)
        emit    2, imm
        exit
    error:
        err     'incorrect argument'
    end calminstruction
end macro

macro make_bd name, opcode
    calminstruction name? rp*
        local   r
        compute r, rp
        check   r eq r element 1 & r metadata 1 relativeto reg8
        jno     error
        compute r, r metadata 1 - reg8
        check   (r = 0) | (r = 2)
        jno     error
        emit    1, opcode or ((r shr 1) shl 4)
        exit
    error:
        err     'incorrect argument'
    end calminstruction
end macro

macro make_noarg name, opcode
    calminstruction name?
        emit    1, opcode
    end calminstruction
end macro

macro make_addr name, opcode
    calminstruction name? addr*
        emit    1, opcode
        emit    2, addr
    end calminstruction
end macro

calminstruction MOV? args&
    local   dst, src, d, s
    match   dst=,src, args
    jno     error
    compute d, dst
    check   d eq d element 1 & d metadata 1 relativeto reg8
    jno     error
    compute d, d metadata 1 - reg8
    compute s, src
    check   s eq s element 1 & s metadata 1 relativeto reg8
    jno     error
    compute s, s metadata 1 - reg8
    emit    1, 01000000b or (d shl 3) or s
    exit
error:
    err     'incorrect argument'
end calminstruction

calminstruction MVI? args&
    local   dst, imm, d
    match   dst=,imm, args
    jno     error
    compute d, dst
    check   d eq d element 1 & d metadata 1 relativeto reg8
    jno     error
    compute d, d metadata 1 - reg8
    emit    1, 00000110b or (d shl 3)
    emit    1, imm
    exit
error:
    err     'incorrect argument'
end calminstruction

calminstruction RST? n*
    local   v
    compute v, n
    check   v >= 0 & v <= 7
    jno     error
    emit    1, 11000111b or (v shl 3)
    exit
error:
    err     'incorrect argument'
end calminstruction

make_alu    ADD, 10000000b
make_alu    ADC, 10001000b
make_alu    SUB, 10010000b
make_alu    SBB, 10011000b
make_alu    ANA, 10100000b
make_alu    XRA, 10101000b
make_alu    ORA, 10110000b
make_alu    CMP, 10111000b

make_alu_imm ADI, 0C6h
make_alu_imm ACI, 0CEh
make_alu_imm SUI, 0D6h
make_alu_imm SBI, 0DEh
make_alu_imm ANI, 0E6h
make_alu_imm XRI, 0EEh
make_alu_imm ORI, 0F6h
make_alu_imm CPI, 0FEh
make_alu_imm IN,  0DBh
make_alu_imm OUT, 0D3h

make_incdec INR, 00000100b
make_incdec DCR, 00000101b

make_rp_op  INX,  00000011b, 3
make_rp_op  DCX,  00001011b, 3
make_rp_op  DAD,  00001001b, 3
make_rp_op  PUSH, 11000101b, 4
make_rp_op  POP,  11000001b, 4

make_rp_imm LXI,  00000001b, 3

make_bd     LDAX, 00001010b
make_bd     STAX, 00000010b

make_addr   JMP,  11000011b
make_addr   JNZ,  11000010b
make_addr   JZ,   11001010b
make_addr   JNC,  11010010b
make_addr   JC,   11011010b
make_addr   JPO,  11100010b
make_addr   JPE,  11101010b
make_addr   JP,   11110010b
make_addr   JM,   11111010b
make_addr   CALL, 11001101b
make_addr   CNZ,  11000100b
make_addr   CZ,   11001100b
make_addr   CNC,  11010100b
make_addr   CC,   11011100b
make_addr   CPO,  11100100b
make_addr   CPE,  11101100b
make_addr   CP,   11110100b
make_addr   CM,   11111100b
make_addr   LDA,  00111010b
make_addr   STA,  00110010b
make_addr   LHLD, 00101010b
make_addr   SHLD, 00100010b

make_noarg  RLC,  00000111b
make_noarg  RRC,  00001111b
make_noarg  RAL,  00010111b
make_noarg  RAR,  00011111b
make_noarg  CMA,  00101111b
make_noarg  CMC,  00111111b
make_noarg  STC,  00110111b
make_noarg  DAA,  00100111b
make_noarg  NOP,  00000000b
make_noarg  HLT,  01110110b
make_noarg  EI,   11111011b
make_noarg  DI,   11110011b
make_noarg  XCHG, 11101011b
make_noarg  XTHL, 11100011b
make_noarg  SPHL, 11111001b
make_noarg  PCHL, 11101001b
make_noarg  RET,  11001001b
make_noarg  RNZ,  11000000b
make_noarg  RZ,   11001000b
make_noarg  RNC,  11010000b
make_noarg  RC,   11011000b
make_noarg  RPO,  11100000b
make_noarg  RPE,  11101000b
make_noarg  RP,   11110000b
make_noarg  RM,   11111000b
