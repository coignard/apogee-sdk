
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

iterate <name, directive>,\
        DEFB, db,\
        DEFM, db,\
        DEFW, dw,\
        DEFD, dd

    macro name? args&
        directive args
    end macro
end iterate

macro DS? count*, fill?
    match val, fill
        db count dup (val)
    else
        rb count
    end match
end macro

iterate name, DEFS, BLOCK
    macro name? args&
        DS args
    end macro
end iterate

macro INCBIN? path*, skip:0, len?
    match l, len
        file path : skip, l
    else
        file path : skip
    end match
end macro

macro ALIGN? boundary*, fill?
    if $ mod boundary
        match val, fill
            db (boundary - ($ mod boundary)) dup (val)
        else
            rb (boundary - ($ mod boundary))
        end match
    end if
end macro
