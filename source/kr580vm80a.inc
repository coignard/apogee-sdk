
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

calminstruction LXI? args&
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
    emit    1, 00000001b or ((r shr 1) shl 4)
    emit    2, imm
    exit
r16:
    compute r, r metadata 1 - reg16
    check   r = 3
    jno     error
    emit    1, 00110001b
    emit    2, imm
    exit
error:
    err     'incorrect argument'
end calminstruction

iterate <name, opcode>,\
        ADD, 10000000b,\
        ADC, 10001000b,\
        SUB, 10010000b,\
        SBB, 10011000b,\
        ANA, 10100000b,\
        XRA, 10101000b,\
        ORA, 10110000b,\
        CMP, 10111000b

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
end iterate

iterate <name, opcode>,\
        ADI, 0C6h,\
        ACI, 0CEh,\
        SUI, 0D6h,\
        SBI, 0DEh,\
        ANI, 0E6h,\
        XRI, 0EEh,\
        ORI, 0F6h,\
        CPI, 0FEh,\
        IN,  0DBh,\
        OUT, 0D3h

    calminstruction name? imm*
        emit    1, opcode
        emit    1, imm
    end calminstruction
end iterate

iterate <name, opcode>,\
        INR, 00000100b,\
        DCR, 00000101b

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
end iterate

iterate <name, opcode, r16_allowed>,\
        INX,  00000011b, 3,\
        DCX,  00001011b, 3,\
        DAD,  00001001b, 3,\
        PUSH, 11000101b, 4,\
        POP,  11000001b, 4

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
end iterate

iterate <name, opcode>,\
        LDAX, 00001010b,\
        STAX, 00000010b

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
end iterate

iterate <name, opcode>,\
        JMP,  11000011b,\
        JNZ,  11000010b,\
        JZ,   11001010b,\
        JNC,  11010010b,\
        JC,   11011010b,\
        JPO,  11100010b,\
        JPE,  11101010b,\
        JP,   11110010b,\
        JM,   11111010b,\
        CALL, 11001101b,\
        CNZ,  11000100b,\
        CZ,   11001100b,\
        CNC,  11010100b,\
        CC,   11011100b,\
        CPO,  11100100b,\
        CPE,  11101100b,\
        CP,   11110100b,\
        CM,   11111100b,\
        LDA,  00111010b,\
        STA,  00110010b,\
        LHLD, 00101010b,\
        SHLD, 00100010b

    calminstruction name? addr*
        emit    1, opcode
        emit    2, addr
    end calminstruction
end iterate

iterate <name, opcode>,\
        RLC,  00000111b,\
        RRC,  00001111b,\
        RAL,  00010111b,\
        RAR,  00011111b,\
        CMA,  00101111b,\
        CMC,  00111111b,\
        STC,  00110111b,\
        DAA,  00100111b,\
        NOP,  00000000b,\
        HLT,  01110110b,\
        EI,   11111011b,\
        DI,   11110011b,\
        XCHG, 11101011b,\
        XTHL, 11100011b,\
        SPHL, 11111001b,\
        PCHL, 11101001b,\
        RET,  11001001b,\
        RNZ,  11000000b,\
        RZ,   11001000b,\
        RNC,  11010000b,\
        RC,   11011000b,\
        RPO,  11100000b,\
        RPE,  11101000b,\
        RP,   11110000b,\
        RM,   11111000b

    calminstruction name?
        emit    1, opcode
    end calminstruction
end iterate
