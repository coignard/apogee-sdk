
; Hello World for Apogee BK-01

include '../source/kr580vm80a.inc'
include '../source/apogee.inc'

format rka

LXI  h, msg_hello
CALL MON_MSG_OUT

LXI  h, msg_any_key
CALL MON_MSG_OUT

CALL MON_CHAR_IN_WAIT
JMP  MON_START

msg_hello:
    dba 0Dh, 0Ah, 'Привет, мир!', 0Dh, 0Ah, 0

msg_any_key:
    dba 0Dh, 0Ah, 'Нажмите любую клавишу...', 0Dh, 0Ah, 0
