
;
;    Höhere Gewalt
;
;    Copyright (c) 2026 René Coignard
;    All rights reserved.
;
; ----------------------------------------------------------------
;
;    Registration
;
;    Müller 1738, Grote of Sint-Bavokerk, Haarlem (NL)
;
; -- Part 1 ------------------------------------------------------
;
;    Positif      Holpijp        8'
;                 Fluitdouce     4'
;
;    Hoofdm.      Roerfluit      8'
;                 Octaaf         4'
;                 Woudfluit      2'
;
;    Bovenkl.     Praestant      8'
;                 Octaaf         4'
;                 Flagfluit      4'
;                 Nachthoorn     2'
;
;    Pedaal       Praestant     16'
;                 Holfluit       8'
;                 Quintpraest    6'
;                 Octaaf         4'
;                 Holfluit       2'
;
; -- Part 2 ------------------------------------------------------
;
;    Bovenkl.  +  Flageolet
;    Pedaal    +  Mixtuur
;
; -- Part 3 ------------------------------------------------------
;
;    Positif   +  Praestant      8'
;              +  Octaaf         4'
;              +  Speelfluit     3'
;              +  Sup: Octaaf    2'
;
;    Hoofdm.   +  Praestant     16'
;              +  Bourdon       16'
;              +  Gemshoorn      4'
;              +  Mixtuur
;              +  Scherp
;              +  Trompet        4'
;
;    Pedaal    +  Principaal    32'
;              +  Subbas        16'
;              +  Octaaf         8'
;              +  Trompet        8'
;              +  Koppel Bovenkl. - Ped.
;              +  Koppel Hoofdm.  - Ped.
;              +  Koppel Pos.     - Ped.
;

include '../source/kr580vm80a.inc'
include '../source/apogee.inc'

format rka

; pause for a 24-bit tick delta
macro tick_wait ticks
    if ticks > 0
        db 0
        db (ticks) and 0xFF, ((ticks) shr 8) and 0xFF, ((ticks) shr 16) and 0xFF
    end if
end macro

; emit a note-on opcode
macro note_on note
    db 1, note
end macro

; emit a note-off opcode
macro note_off note
    db 2, note
end macro

; set loop counter at slot <idx> to <val>
macro loop_set idx, val
    db 4, idx, val
end macro

; decrement counter at <idx>, branch to <target> while non-zero
macro loop_end idx, target
    db 5
    dw target
    db idx
end macro

; terminate the sequence
macro seq_end
    db 7
end macro

; sound three notes for <dur> ticks after waiting <gap> ticks
macro chord gap, dur, n1, n2, n3
    tick_wait gap
    note_on n1
    note_on n2
    note_on n3
    tick_wait dur
    note_off n1
    note_off n2
    note_off n3
end macro

; three evenly spaced chords starting after <gap> ticks, each lasting <dur> ticks
macro triplet gap, dur, n1, n2, n3
    chord gap, dur, n1, n2, n3
    chord dur, dur, n1, n2, n3
    chord dur, dur, n1, n2, n3
end macro

; <bars> bars of a pedaal bass note followed by a chord triplet
macro pedaal_bar bars, gap, bass, n1, n2, n3
    loop_set 0, bars
  local lp
  lp:
    tick_wait gap
    note_on bass
    tick_wait 1920
    note_off bass
    triplet 0, 60, n1, n2, n3
    loop_end 0, lp
end macro

; single short stab of three notes after <gap> ticks
macro pos_pulse gap, n1, n2, n3
    tick_wait gap
    note_on n1
    note_on n2
    note_on n3
    tick_wait 15
    note_off n1
    note_off n2
    note_off n3
end macro

; one leading stab then <count-1> evenly spaced stabs at 225-tick intervals
macro pos_bar count, gap, n1, n2, n3
    pos_pulse gap, n1, n2, n3
    if count > 1
        loop_set 0, count - 1
      local lp
      lp:
        pos_pulse 225, n1, n2, n3
        loop_end 0, lp
    end if
end macro

; hoofdmanuaal melody
macro hoofd_bar cycles, gap, n1, n2, n3, n4
    tick_wait gap
    note_on n1
    tick_wait 30
    note_off n1
    tick_wait 90
    note_on n2
    tick_wait 30
    note_off n2
    tick_wait 90
    note_on n3
    tick_wait 30
    note_off n3
    tick_wait 90
    note_on n4
    tick_wait 30
    note_off n4
    if cycles > 1
        loop_set 0, cycles - 1
      local lp
      lp:
        tick_wait 90
        note_on n1
        tick_wait 30
        note_off n1
        tick_wait 90
        note_on n2
        tick_wait 30
        note_off n2
        tick_wait 90
        note_on n3
        tick_wait 30
        note_off n3
        tick_wait 90
        note_on n4
        tick_wait 30
        note_off n4
        loop_end 0, lp
    end if
end macro

; <bars> bars of a bovenklavier note followed by a chord triplet
macro boven_bar bars, gap, bass, n1, n2, n3
    loop_set 0, bars
  local lp
  lp:
    tick_wait gap
    note_on bass
    tick_wait 1920
    note_off bass
    triplet 0, 60, n1, n2, n3
    loop_end 0, lp
end macro

start:
    MVI A, VG75_CMD_STOP_DISP
    STA VG75_CMD
    MVI A, VV55_CWR_MODE_SET or VV55_CWR_GRP_A_MODE1
    STA VV55_USR_CWR

main_loop:
    lxi b, 43
.dly:
    dcx b
    mov a, b
    ora c
    jnz .dly
    nop
    mov a, a

    ; increment the 24-bit global tick counter with carry propagation
    lxi h, tick
    inr m
    jnz .carry
    inx h
    inr m
    jnz .carry
    inx h
    inr m
.carry:
    ; who you gonna call?
    lxi h, tracks
    call track_run
    lxi h, tracks + 9
    call track_run
    lxi h, tracks + 18
    call track_run
    lxi h, tracks + 27
    call track_run

    ; check if all track slots are inactive
    lda tracks
    lxi h, tracks + 9
    ora m
    lxi h, tracks + 18
    ora m
    lxi h, tracks + 27
    ora m

    ; exit to monitor if all flags are 0
    jz MON_START

    jmp main_loop

; execute one track slot until the next tick_wait opcode
; on entry HL points to the slot record
track_run:
    mov a, m
    ora a
    rz              ; slot inactive

    ; compare slot's scheduled tick against the global counter (24-bit)
    inx h
    lda tick+0
    cmp m
    rnz
    inx h
    lda tick+1
    cmp m
    rnz
    inx h
    lda tick+2
    cmp m
    rnz

    ; tick matched, load bytecode pointer and enter dispatch loop
    inx h
    mov e, m
    inx h
    mov d, m
    push h          ; preserve HL at slot+5 (base of per-slot state)

.run:
    ldax d
    inx d
    cpi 0
    jz .do_wait
    cpi 1
    jz .do_non
    cpi 2
    jz .do_noff
    cpi 4
    jz .do_lset
    cpi 5
    jz .do_lend
    ; opcode 7 or any unknown value terminates the slot

.do_end:
    pop h
    dcx h
    dcx h
    dcx h
    dcx h
    dcx h           ; HL back to slot+0 (active flag)
    mvi m, 0        ; deactivate slot
    ret

.do_wait:
    ldax d
    mov c, a        ; delta byte 0 (low)
    inx d
    ldax d
    mov b, a        ; delta byte 1 (mid)
    inx d
    ldax d          ; A = delta byte 2 (high)
    inx d

    pop h
    push h          ; HL = slot+5
    dcx h
    dcx h
    dcx h
    dcx h           ; HL = slot+1 (scheduled tick field)

    push d
    mov d, a        ; D = delta high byte

    lda tick+0
    add c
    mov m, a
    inx h

    lda tick+1
    adc b
    mov m, a
    inx h

    lda tick+2
    adc d
    mov m, a

    ; write updated bytecode pointer back into the slot
    inx h
    pop d
    mov m, e
    inx h
    mov m, d

    pop h
    ret             ; yield to main loop

.do_non:
    ldax d
    mov b, a        ; B = note number
    inx d
    pop h
    push h
    inx h
    inx h
    inx h           ; HL = slot+8 (MIDI channel byte)
    mov c, m
    mvi a, 0x90
    add c
    push d
    mov e, a
    mov d, b
    mvi c, 100      ; velocity (lol)
    call tx_msg
    pop d
    jmp .run

.do_noff:
    ldax d
    mov b, a        ; B = note number
    inx d
    pop h
    push h
    inx h
    inx h
    inx h           ; HL = slot+8 (MIDI channel byte)
    mov c, m
    mvi a, 0x80
    add c
    push d
    mov e, a
    mov d, b
    mvi c, 64
    call tx_msg
    pop d
    jmp .run

.do_lset:
    ldax d          ; counter index
    mov c, a
    mvi b, 0
    inx d
    ldax d          ; initial count
    inx d
    pop h
    push h          ; HL = slot+5
    inx h           ; HL = slot+6 (loop counter array)
    dad b
    mov m, a
    jmp .run

.do_lend:
    ldax d
    mov c, a        ; branch target low
    inx d
    ldax d
    mov b, a        ; branch target high
    inx d
    ldax d          ; counter index
    inx d

    pop h
    push h          ; HL = slot+5
    inx h           ; HL = slot+6
    push b          ; save branch target
    mov c, a
    mvi b, 0
    dad b           ; HL = slot+6+idx
    dcr m           ; decrement loop counter
    pop b
    jz .run         ; counter exhausted, fall through
    mov d, b
    mov e, c        ; DE = branch target
    jmp .run

; write one byte to the MIDI output port via port A
tx_byte:
    MOV A, E
    STA VV55_USR_PORT_A
    RET

; send a three-byte MIDI message (status, note, velocity)
; entry: E = status, D = note, C = velocity
tx_msg:
    CALL tx_byte
    MOV E, D
    CALL tx_byte
    MOV E, C
    CALL tx_byte
    RET

; initialised to 0xFF so the first increment wraps cleanly to 0x000000
tick: db 0xFF, 0xFF, 0xFF

; slot layout (9 bytes): active[1], sched_tick[3], seq_ptr[2], loop_ctr[2], channel[1]
tracks:
    db 1, 0,0,0
    dw pedaal
    db 0, 0
    db 4

    db 1, 0,0,0
    dw positif
    db 0, 0
    db 0

    db 1, 0,0,0
    dw hoofdmanuaal
    db 0, 0
    db 1

    db 1, 0,0,0
    dw bovenklavier
    db 0, 0
    db 2

positif:
    pos_bar 96, 102962, 65, 68, 72
    pos_bar 96,    225, 68, 71, 75
    pos_bar 96,    225, 68, 73, 77
    pos_bar 96,    225, 68, 71, 75
    pos_bar 96,    225, 65, 68, 72
    pos_bar 96,    225, 68, 71, 75
    pos_bar 96,    225, 68, 73, 77
    pos_bar 96,    225, 68, 71, 75
    pos_bar 96,   2145, 65, 68, 72
    pos_bar 96,    225, 68, 71, 75
    pos_bar 96,    225, 68, 73, 77
    pos_bar 96,    225, 68, 71, 75
    seq_end

hoofdmanuaal:
    hoofd_bar 48, 195122, 77, 72, 68, 72
    hoofd_bar 48,     90, 76, 71, 68, 71
    hoofd_bar 48,     90, 77, 73, 68, 73
    hoofd_bar 48,     90, 76, 71, 68, 71
    hoofd_bar 48,   2010, 77, 72, 68, 72
    hoofd_bar 48,     90, 76, 71, 68, 71
    hoofd_bar 48,     90, 77, 73, 68, 73
    hoofd_bar 48,     90, 76, 71, 68, 71
    seq_end

bovenklavier:
    tick_wait 10802
    note_on 41
    tick_wait 1920
    note_off 41
    triplet 0, 60, 53, 56, 60
    boven_bar 3, 3540, 41, 53, 56, 60
    boven_bar 4, 3540, 40, 56, 59, 63
    boven_bar 4, 3540, 37, 56, 61, 65
    boven_bar 4, 3540, 40, 56, 59, 63
    seq_end

pedaal:
    triplet 12722, 60, 53, 56, 60
    loop_set 0, 3
  .a:
    triplet 5460, 60, 53, 56, 60
    loop_end 0, .a
    loop_set 0, 4
  .b:
    triplet 5460, 60, 56, 59, 63
    loop_end 0, .b
    loop_set 0, 4
  .c:
    triplet 5460, 60, 56, 61, 65
    loop_end 0, .c
    loop_set 0, 4
  .d:
    triplet 5460, 60, 56, 59, 63
    loop_end 0, .d
    pedaal_bar 4, 3540, 41, 53, 56, 60
    pedaal_bar 4, 3540, 40, 56, 59, 63
    pedaal_bar 4, 3540, 37, 56, 61, 65
    pedaal_bar 4, 3540, 40, 56, 59, 63
    pedaal_bar 4, 3540, 41, 53, 56, 60
    pedaal_bar 4, 3540, 40, 56, 59, 63
    pedaal_bar 4, 3540, 37, 56, 61, 65
    pedaal_bar 4, 3540, 40, 56, 59, 63
    pedaal_bar 1, 5460, 41, 53, 56, 60
    pedaal_bar 3, 3540, 41, 53, 56, 60
    pedaal_bar 4, 3540, 40, 56, 59, 63
    pedaal_bar 4, 3540, 37, 56, 61, 65
    pedaal_bar 4, 3540, 40, 56, 59, 63
    seq_end
