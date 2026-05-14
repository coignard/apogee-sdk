
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

; r indicates an 8-bit register (A, B, C, D, E, H, L)
; m indicates memory access via (HL)
; rp indicates a 16-bit register pair (BC, DE, HL, SP, PSW)

; data transfer group
T_MOV_R_R       := 5
T_MOV_R_M       := 7
T_MOV_M_R       := 7
T_MVI_R         := 7
T_MVI_M         := 10
T_LXI_RP        := 10
T_LDA           := 13
T_STA           := 13
T_LHLD          := 16
T_SHLD          := 16
T_LDAX_RP       := 7
T_STAX_RP       := 7
T_XCHG          := 4

; arithmetic group
T_ADD_R         := 4
T_ADD_M         := 7
T_ADC_R         := 4
T_ADC_M         := 7
T_SUB_R         := 4
T_SUB_M         := 7
T_SBB_R         := 4
T_SBB_M         := 7

T_ADI           := 7
T_ACI           := 7
T_SUI           := 7
T_SBI           := 7

T_INR_R         := 5
T_INR_M         := 10
T_DCR_R         := 5
T_DCR_M         := 10

T_INX_RP        := 5
T_DCX_RP        := 5
T_DAD_RP        := 10
T_DAA           := 4

; logical group
T_ANA_R         := 4
T_ANA_M         := 7
T_XRA_R         := 4
T_XRA_M         := 7
T_ORA_R         := 4
T_ORA_M         := 7
T_CMP_R         := 4
T_CMP_M         := 7

T_ANI           := 7
T_XRI           := 7
T_ORI           := 7
T_CPI           := 7

T_RLC           := 4
T_RRC           := 4
T_RAL           := 4
T_RAR           := 4

T_CMA           := 4
T_STC           := 4
T_CMC           := 4

; branch group
T_JMP           := 10
T_JCC           := 10

T_CALL          := 17
T_CCC_TAKEN     := 17
T_CCC_NOT_TAKEN := 11

T_RET           := 10
T_RCC_TAKEN     := 11
T_RCC_NOT_TAKEN := 5

T_RST           := 11
T_PCHL          := 5

; stack, I/O and machine control group
T_PUSH_RP       := 11
T_POP_RP        := 10
T_XTHL          := 18
T_SPHL          := 5

T_IN            := 10
T_OUT           := 10

T_EI            := 4
T_DI            := 4
T_HLT           := 7
T_NOP           := 4
