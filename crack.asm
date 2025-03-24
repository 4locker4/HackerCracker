.model tiny
.data

            GREATINGS       db 'Hello! For watch XXX conten enter the password: $'
            DURAK           db 'AiAiAi, do not hack me$'
            LADNO           db 'Password is right. Now you are admin of gay community$'
            SECOND_CHANCE   db 'OK, I will give you a second chance. Enter the passowrd: $'
            CONFRM          db 'Are you sure? [y\n] $'
            DURAK2          db 'You lost your last chance, buy.$'

            NEW_LINE        db 0Ah, 0Dh

.code

org 100h

Start:
        
        call Main

        mov ah, 4ch
        int 21h

Main    proc

        cld 

        mov ah, 09h
        lea dx, GREATINGS
        int 21h

        call FirstAttempInputPassword

        ret

        PSSWRD db 8 dup (0)

        endp

;---------------------------------------------------------------------------------------------
;Entry:
;Retrn:
;Destr:
;---------------------------------------------------------------------------------------------

FirstAttempInputPassword   proc

        mov ah, 0ah
        lea dx, ENTRY_DATA
        int 21h

        call StringChecker

        mov ah, 09h
        lea dx, ENTRY_DATA
        int 21h

        lea dx, DURAK
        int 21h

        ret

        ENTRY_DATA db 30, 9 dup (0)
        endp

;---------------------------------------------------------------------------------------------
;Entry: none
;Retrn: none
;Destr: AX, CX, DI, SI
;---------------------------------------------------------------------------------------------

StringChecker   proc

        mov si, offset ENTRY_DATA
        inc si
        lodsb

        mov cx, ax

        lea di, PSSWRD
        lea si, ENTRY_DATA
        inc si
        inc si
        nop

        jmp Distributer
END:

        ret
        endp

        SEC_CHNS_PASSWORD   db 14, 10 dup (0)
        CONFRM_SEC_PSSWRD   db 10, 10 dup (0)
db 11 dup (?)

Distributer:

        repe cmpsb

        jne NOT_PASSED

        mov ah, 09h
        lea dx, LADNO
        int 21h

NOT_PASSED:

        call NewLine

        mov ah, 09h
        lea dx, DURAK
        int 21h

        call SecondChance

        jmp END

;---------------------------------------------------------------------------------------------
;Entry: none
;Retrn: none
;Destr: AX, BX, CX, DX, SI, DI
;---------------------------------------------------------------------------------------------

Confirmation    proc

        mov cx, offset SEC_CHNS_PASSWORD
        inc cx

        mov di, offset PSSWRD
        mov si, offset SEC_CHNS_PASSWORD
        inc si
        inc si

        repe cmpsb

        jne BAD_BOY

        mov ah, 09h
        lea dx, LADNO
        int 21h

BAD_BOY:
        call NewLine

        mov ah, 09h
        mov dx, offset CONFRM
        int 21h

        mov ah, 0ah
        mov dx, offset CONFRM_SEC_PSSWRD
        int 21h

        mov di, offset CONFRM_SEC_PSSWRD
        inc di
        inc di
        
        mov al, 'n'
        mov bl, [di]
        cmp bl, al

        je Again
        
        call NewLine

        mov ah, 09h 
        mov dx, offset DURAK2
        int 21h

        jmp EOP

        mov ah, 4ch
        int 21h

        ret

    Again:
        call Main

        endp

;---------------------------------------------------------------------------------------------
;Entry: none
;Retrn: none
;Destr: AH, DX
;---------------------------------------------------------------------------------------------

SecondChance    proc

        call NewLine

        mov ah, 09h
        mov dx, offset SECOND_CHANCE
        int 21h

        mov ah, 0ah
        mov dx, offset SEC_CHNS_PASSWORD
        int 21h

        call Confirmation
        ret
        endp

;---------------------------------------------------------------------------------------------
;Entry: none
;Retrn: none
;Destr: none
;---------------------------------------------------------------------------------------------

NewLine proc

        push ax
        push dx

        mov ah, 02h
        mov dl, 0ah
        int 21h

        mov dl, 0Dh
        int 21h

        pop dx
        pop ax

        ret
        endp

    EOP:
end Start
