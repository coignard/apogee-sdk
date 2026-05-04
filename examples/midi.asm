
; MIDI test for Apogee BK-01 emulator with MIDI support via PPI

include '../source/kr580vm80a.inc'
include '../source/apogee.inc'

format rka

MIDI_CMD_NOTE_ON  := 90h
MIDI_CMD_NOTE_OFF := 80h
MIDI_VELOCITY_MAX := 7Fh
MIDI_VELOCITY_MIN := 00h

NOTE_C3      := 3Ch
NOTE_C4_PLUS := 49h

KEY_NOT_PRESSED := 0FFh

MIDI_STROBE_OFF := 0
MIDI_STROBE_ON  := MIDI_CTRL_STROBE

macro out_mem_port addr, val
    MVI A, val
    STA addr
end macro

macro print args&
    local skip, text_data
    JMP skip
text_data:
    dba args, 0
skip:
    LXI H, text_data
    CALL MON_MSG_OUT
end macro

macro exit_to_monitor_if_key_pressed
    CALL MON_CHAR_IN_NOWAIT
    CPI  KEY_NOT_PRESSED
    JNZ  MON_START
end macro

macro transmit_midi_sequence args&
    iterate arg, args
        if arg relativeto 0
            MVI A, arg
        else
            MOV A, arg
        end if
        CALL send_midi_byte
    end iterate
end macro

macro midi_note_on note_reg
    transmit_midi_sequence MIDI_CMD_NOTE_ON, note_reg, MIDI_VELOCITY_MAX
end macro

macro midi_note_off note_reg
    transmit_midi_sequence MIDI_CMD_NOTE_OFF, note_reg, MIDI_VELOCITY_MIN
end macro

macro next_note register, limit, loop_label
    INR register
    MOV A, register
    CPI limit
    JNZ loop_label
end macro

    out_mem_port VV55_USR_CWR, 80h

    print 0Dh, 0Ah, 'Проверка MIDI', 0Dh, 0Ah
    print 0Dh, 0Ah, 'Для возврата в Монитор удерживайте любую клавишу...', 0Dh, 0Ah

play_octave_loop:
    MVI D, NOTE_C3

play_note_loop:
    midi_note_on  D
    CALL delay
    midi_note_off D

    exit_to_monitor_if_key_pressed

    next_note D, NOTE_C4_PLUS, play_note_loop
    JMP play_octave_loop

send_midi_byte:
    STA MIDI_PORT_DATA
    out_mem_port MIDI_PORT_CTRL, MIDI_STROBE_OFF
    out_mem_port MIDI_PORT_CTRL, MIDI_STROBE_ON
    out_mem_port MIDI_PORT_CTRL, MIDI_STROBE_OFF
    RET

delay:
    LXI B, 4000h
.wait:
    DCX B
    MOV A, B
    ORA C
    JNZ .wait
    RET
