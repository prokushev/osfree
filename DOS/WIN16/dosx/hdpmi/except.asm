
;--- default exception handler
;--- usually displays register dump on the screen
;--- then waits for a keypress

	.386

	include hdpmi.inc
	include external.inc
	include debugsys.inc

	option proc:private

?DUMP			= 0	;std 0, 1=call internal debugger (not implemented)
?POLLKBD		= 0	;std 0, 1=poll keyboard, 0=use int 16h real-mode
?SETCLIENTPSP	= 1 ;std 1, 1=set original client psp before server exits
?DISPCLSP		= 0	;std 0, 1=display client stack if ring 0 exception
?RESETMOUSE		= 1 ;std 1, 1=reset mouse before getting a key
?SAFERMSTACK	= 0 ;std 0, 1=ensure real-mode stack is usable

@seg _TEXT16
@seg _TEXT32
@seg _DATA16

@charout macro xx
	push xx
	call _charout
	endm

CONST32	segment byte use32 public 'CODE'
CONST32	ends

GROUP32 group _TEXT32, CONST32

CONST32 segment

regtexts db "CS=",00
		db " SS=",00
		db lf,"DS=",00
		db " ES=",00
		db lf,"FS=",00
		db " GS=",00
		db lf,"LDTR=",00
		db " TR=",00
		db lf,"ERRC=",00

szTerm1	db "terminate ",0
szTerm2	db "(c)lient or ",0
szTerm3	db "(s)erver now? ",0

CONST32 ends

_TEXT32 segment

_LTRACE_ = 0

if ?LOGINT30
externdef lint30:word
endif

;*** get selector bx base (GDT or LDT)
;--- ES=FLAT, DS=GROUP16

	assume ds:GROUP16

mygetbase proc
	movzx ebx,bx
	lar eax,ebx
	jnz error
	test bl,4
	jnz @F
	and bl,0F8h
	add ebx, pdGDT.dwBase
	jmp mgb_1
@@:
	and bl,0F8h
	add ebx, [dwLDTAddr]
mgb_1:
	mov ah,es:[ebx+7]
	mov al,es:[ebx+4]
	shl eax,16
	mov ax,es:[ebx+2]
	clc
	ret
error:
	stc
	ret
mygetbase endp

IsTextMode proto

forcetextmode proc public
if ?FORCETEXTMODE
	test ss:[fMode2],FM2_FORCETEXT
	jz exit
endif
	call IsTextMode
	jz exit
	push eax
	push ds
	push es
	xor eax, eax
	mov ds, eax		;make sure no ring 0 selectors are in DS,ES
	mov es, eax
	mov al,03		;set std text mode
	@simrmint 10h
	pop es
	pop ds
	pop eax
exit:
	ret
forcetextmode endp

	assume ds:nothing

;*** display PTE for linear address in eax

PTEout:
	call _Linear2PT		;returns address of PTE in EDI
	jc @F				;address not allocated
	push dword ptr es:[edi]
	call _dwordout
	ret
@@:
	@printf <"???">
	ret

;--- ring 0 exception

_exceptZ proc public	;exception in ring 0

	@strout <"exceptZ entry",lf>
;	@waitesckey

if 0
	push eax
	mov eax, ss
	cmp ax,_SSSEL_
	pop eax
	jnz _except
endif
	cmp ss:bExcMode,0
	jz _except
	add esp, sizeof R3FAULT32
	cmp ss:bExcMode, 1
	mov ss:bExcMode, 0
	jz noeip
	jmp noesp
_exceptZ endp

;--- ring 3 default exception handler

EXCFRX	struct
wExcNo	dw ?
		dw ?
		dq ?		;dpmi return address
rErr	dd ?		;error code
union
struct
rSP		dd ?
rSS		dw ?
ends
rSSSP	df ?
ends
EXCFRX	ends


_exceptX proc public
	cld
	sub esp, 3*4	;12		;make room for a R3FAULT32 item
	push ebp		;+4=16
	mov ebp,esp

	@strout <"exceptX exc=%X errc=%lX ss:sp=%X:%lX",lf>,\
		[ebp+16].EXCFRX.wExcNo, [ebp+16].EXCFRX.rErr,\
		[ebp+16].EXCFRX.rSS, [ebp+16].EXCFRX.rSP
;	@waitesckey

	push ds
	push esi
	push eax
	mov ax,[ebp+16].EXCFRX.wExcNo
	mov [ebp+4],ax
	mov eax,[ebp+16].EXCFRX.rErr
	lds esi,[ebp+16].EXCFRX.rSSSP
	mov [ebp+8].R3FAULT32.rErr,eax
if 0
	@strout <"sp=%X %X %X %X %X %X",lf>,\
		<word ptr [esi+00]>,<word ptr [esi+02]>,<word ptr [esi+04]>,\
		<word ptr [esi+06]>,<word ptr [esi+08]>,<word ptr [esi+10]>
endif
if ?32BIT
	lodsd
	mov [ebp+8].R3FAULT32.rIP,eax
	lodsd
	mov [ebp+8].R3FAULT32.rCSd,eax
	lodsd
	mov [ebp+8].R3FAULT32.rFL,eax
	lodsd
	mov [ebp+8].R3FAULT32.rSP,eax
	lodsd
	mov [ebp+8].R3FAULT32.rSSd,eax
else
	xor eax,eax
	lodsw
	mov [ebp+8].R3FAULT32.rIP,eax
	lodsw
	mov [ebp+8].R3FAULT32.rCSd,eax
	lodsw
	mov [ebp+8].R3FAULT32.rFL,eax
	lodsw
	mov [ebp+8].R3FAULT32.rSP,eax
	lodsw
	mov [ebp+8].R3FAULT32.rSSd,eax
endif
	pop eax
	pop esi
	pop ds
	pop ebp
_exceptX endp  ;fall through

_exceptY proc public
_exceptY endp

;--- expected stack frame:
;--- esp+0: wExcNo
;--- esp+4: R3FAULT32

;--- on entry generates frame:

R3EXCFRAME struct
rGS		dw ?
		dw ?
rFS		dw ?
		dw ?
rES		dw ?
		dw ?
rDS		dw ?
		dw ?
		PUSHADS <>	;+8
wExcNo	dw ?		;+40
      	dw ?
de  	R3FAULT32 <>;+42	;this is emulated for ring 0 exceptions
R3EXCFRAME ends

_except proc near

	@DebugBreak 0
	pushad
	push ds
	push es
	push fs
	push gs
	mov ebp,esp	  ; 			   16-Bit  32-Bit

	assume ebp:ptr R3EXCFRAME

	cld
	push ss
	pop ds
	push ds
	pop es
	assume ds:GROUP16

if 0
	pushfd
	and byte ptr [esp+1],not 40h	;clear NT flag
	popfd
endif

	@strout <"except wExc=%X rErr=%lX cs:ip=%X:%lX fl=%lX ss:sp=%X:%lX",lf>,\
		[ebp].wExcNo, [ebp].de.rErr, [ebp].de.rCS, [ebp].de.rIP, [ebp].de.rFL,\
		[ebp].de.rSS, [ebp].de.rSP
;	@waitesckey

	inc bExcEntry
	jnz except_2x		;this is fatal
if ?SAFERMSTACK
	push [wHostPSP]
	push 100h
	pop [tskstate.rmSSSP]
endif
if ?FORCETEXTMODE
	call forcetextmode
endif
;	test [fDebug],FDEBUG_KDPRESENT
;	jnz @F

;--- dump exception number

	@printf <"Exception ">
	mov al,byte ptr [ebp].wExcNo
	cmp al,10h
	jnz @F
	fninit					;clear FSW, FCW and FTW of FPU
@@:
	push eax
	call _byteout
	lar eax,[ebp].de.rSSd
	and ah,60h
	jz @F
	@printf <lf>
	jmp except_cont
@@:

;--- exception in ring 0, dump client CS:EIP, SS:ESP

	@printf <" in ring 0",lf>
	cmp cApps, 0
	jz @F
	mov ebx,[taskseg._Esp0]
	@printf <"next client CS:EIP=%X:%lX,SS:ESP=%X:%lX",lf>,\
		[ebx-sizeof IRET32].IRET32.rCS, [ebx-sizeof IRET32].IRET32.rIP,\
		[ebx-sizeof IRET32].IRET32.rSS, [ebx-sizeof IRET32].IRET32.rSP
if ?DISPCLSP
	push ds
	lds esi,[ebx-sizeof IRET32].IRET32.rSSSP]
	@printf <"client [ESP]=%lX,%lX,%lX",lf>,\
		<dword ptr [esi+0]>,<dword ptr [esi+4]>,<dword ptr [esi+8]>
	pop ds
endif
@@:
	@strout <"r0sp=%X %X %X %X %X %X %X %X %X",lf>, bx,\
		<word ptr [ebx-16]>,<word ptr [ebx-14]>,<word ptr [ebx-12]>,\
		<word ptr [ebx-10]>,<word ptr [ebx-08]>,<word ptr [ebx-06]>,\
		<word ptr [ebx-04]>,<word ptr [ebx-02]>


except_cont:

;--- dump standard registers

	@printf <"EAX=%lX EBX=%lX ECX=%lX EDX=%lX ESI=%lX",lf>,\
		[ebp].rEAX,[ebp].rEBX,[ebp].rECX,[ebp].rEDX,[ebp].rESI
	@printf <"EDI=%lX EBP=%lX ESP=%lX EFL=%lX EIP=%lX",lf>,\
		[ebp].rEDI,[ebp].rEBP,[ebp].de.rSP,[ebp].de.rFL,[ebp].de.rIP

	push byte ptr _FLATSEL_
	pop es
	push ss
	pop ds

;--- dump segment registers

	mov ax, word ptr [ebp].de.rErr
	push ax
	str ax
	push ax
	sldt ax
	push ax
	push word ptr [ebp].rGS
	push word ptr [ebp].rFS
	push word ptr [ebp].rES
	push word ptr [ebp].rDS
	push [ebp].de.rSS
	push [ebp].de.rCS

	mov esi,offset regtexts
	mov cl,9
nextsegm:
	call _stroutsi
	pop di
	lar eax,edi
	jnz novalidsel
	mov ebx,edi
	call mygetbase
	mov ebx, eax
	lsl eax, edi
	lar edx, edi
	shr edx, 8
	@printf <"%X (%lX,%lX,%X)">, di, ebx, eax, dx
	jmp @F
novalidsel:
	@printf	<"%X (********,********,****)">, di
@@:
	dec cl
	jnz nextsegm

	sldt bx
	call mygetbase
	jc @F
	and eax, eax
	jz @F
	@printf <" PTE 1. Page LDT=">
	call PTEout
@@:
	@charout lf

;--- dump special registers (GDTR, IDTR, CRx, ...)

	sub esp,2*6
	sidt [esp+6]
	sgdt [esp+0]
	@printf <"GDTR=%X:%lX IDTR=%X:%lX PTE CR2=">
	add esp,2*6
	mov eax,cr2
	call PTEout
if 0
	@printf <" Stat=%X",lf>,<word ptr cApps>
else
	@charout lf
endif

	mov eax,cr3
	push eax
	mov eax,cr2
	push eax
	mov eax,cr0
	push eax
	@printf <"CR0=%lX CR2=%lX CR3=%lX ">
	cmp _cpu,5
	jb @F
	@mov_eax_cr4
	@printf <"CR4=%lX ">, eax
@@:
;	push [dwHostStackExc]
	push [taskseg._Esp0]
;	@printf <"TSS:ESP0=%lX (%lX)",lf>
	@printf <"TSS:ESP0=%lX",lf>
	mov eax,dr7
	push eax
	mov eax,dr6
	push eax
	mov eax,dr3
	push eax
	mov eax,dr2
	push eax
	mov eax,dr1
	push eax
	mov eax,dr0
	push eax
	@printf <"DR0-3=%lX %lX %lX %lX DR6=%lX DR7=%lX",lf>

;--- dump some host variables (LPMS, RMS, RMCB, ...)

	@printf <"LPMS Sel/Cnt=%X/%X RMS=%X:%X open RMCBs=%X/%X ">,\
		<word ptr _LPMSSEL_>,<word ptr [cLPMSused]>,\
		[tskstate.rmSS], [tskstate.rmSP], [cStdRMCB], [cRMCB]
;--- clear dr7 to avoid I/O watchpoints                
	xor eax, eax
	mov dr7, eax
	call closeinterrupts
	@printf <"ISR=%X",lf>, ax

if ?LOGINT31
externdef i31func:word
	@strout <"last i31 call: %X",lf>,i31func
endif

;--- now try to display memory contents [EIP] + [ESP]
;--- this may fail, in which case just display some ??? and continue

	mov [bExcMode], 1

	@printf "   [EIP]="
	mov esi,[ebp].de.rIP
	mov ax,[ebp].de.rCS
	verr ax
	jnz noeip
	mov ds, eax
	mov cl, 12
nextbyte_eip:
	lodsb
	push eax
	call _byteout
	@charout ' '
	dec cl
	jnz nextbyte_eip
	jmp eip_done
noeip::
	@printf "??"
eip_done:
	@charout lf
	push ss
	pop ds

;--- try to display [ESP]

	mov [bExcMode], 2
	@printf "   [ESP]="
	mov eax,[ebp].de.rSSd
	verr ax
	jnz noesp
	lds esi,[ebp].de.rSSSP
	lar eax,eax
	test eax,400000h
	jnz @F
	movzx esi,si
@@:
	mov ch,06
	jmp start_stackdump
nextline_stack:
	@charout lf
	push esi
	call _dwordout
	@charout  '='
start_stackdump:
	mov cl,08
nextword:
	lodsw
	push eax
	call _wordout
	@charout ' '
	dec cl
	jnz nextword
	dec ch
	jnz nextline_stack
	jmp esp_done
noesp::
	@printf "????"
esp_done:
	@charout lf
	push ss
	pop ds
	mov [bExcMode], 0

if ?LOGINT30
	@strout <"last int 30=%X",lf>,[lint30]
endif

;--- dump is done

if ?DUMP
	call __debug			;call internal debugger
endif
	dec bExcEntry
if ?WDEB386
	test [fDebug],FDEBUG_KDPRESENT
	jnz calldebugger
endif
	mov esi, offset szTerm1
	call _stroutsi
	cmp [cApps],0
	jz @F
	call _stroutsi
@@:
	mov esi, offset szTerm3
	call _stroutsi
tryagain:
	inc bExcEntry
	@pushproc getakey
	call dormprocintern
	dec bExcEntry
	cmp cApps,0
	jz @F
if ?POLLKBD
	cmp dl,2EH+80h	;key "c"
	jz fatappexit
@@:
	cmp dl,1FH+80h	;key "s"
	jnz tryagain
else
	cmp dl,'c'
	jz fatappexit
@@:
	cmp dl,'s'
	jnz tryagain
endif
	@charout lf
if ?SETCLIENTPSP        
	cmp cApps,0			;is there a client?
	jz @F
	mov bx,[rmpsporg]	;set client's initial PSP
	mov ah,50h
	call rmdosintern
@@:
endif
	and fMode, not FM_RESIDENT
	mov ax, _EAERR2_
if 0
	jmp _exitclientEx
else
	jmp _exitclient_pm
endif
except_2x:
	@printf <lf,"FATAL exception at %X:%lX in ring 0. Stopped.",lf>,[ebp].de.rCS, [ebp].de.rIP
	cli
	hlt

_except endp

;*** client fatal exit ***

fatappexit proc
	@charout lf
fatappexit endp	;fall throu

_fatappexit2 proc
	xor eax, eax
	mov es, eax
	mov ds, eax
	push _LPMSSEL_
	push 200h
	push ax	;clear HiWord(EFL)
	pushf
	push byte ptr _CSR3SEL_
	push offset myexit
	iretd
myexit:
	mov ax,4cffh
	int 21h
	push cs
	pop ss	;in case the int 21h did return cause a GPF now
_fatappexit2 endp        

if ?WDEB386

if 0
EXCSTR struct
nr		db ?
pszText	word ?
EXCSTR ends

exc00str	db "divide error",lf,0
exc06str	db "invalid instruction",lf,0
exc0Bstr	db "segment fault",lf, 0
exc0Cstr	db "stack fault",lf,0
exc0Dstr	db "protection exception",lf,0
exc0Estr	db "page fault",lf,0
defaultstr	db "exception 0"
excnr_		db "0",lf,0

excstrtab label byte
	EXCSTR {00h, offset exc00str}
	EXCSTR {06h, offset exc06str}
	EXCSTR {0Bh, offset exc0Bstr}
	EXCSTR {0Ch, offset exc0Cstr}
	EXCSTR {0Dh, offset exc0Dstr}
	EXCSTR {0Eh, offset exc0Estr}
NUMEXCSTR equ ($ - offset excstrtab) / sizeof EXCSTR
	EXCSTR {0FFh, offset defaultstr}
endif

;*** cause the debugger to break
;*** inp: EBP -> exception frame

calldebugger proc
if 0
	push ds
	push ss
	pop ds
;;	dec     bExcEntry
	mov al, byte ptr [ebp].wExcNo
	mov cl, NUMEXCSTR
	mov si, offset excstrtab
nextitem:        
	cmp al,[si].EXCSTR.nr
	jz @F
	add si, sizeof EXCSTR
	dec cl
	jnz nextitem
@@:        
	mov si, [si].EXCSTR.pszText
	.if (al >= 10)
		add al, 7
	.endif
	add ds:excnr_, al
	movzx esi, si
	mov ax,DS_Out_Str
	int Debug_Serv_Int
	pop ds
endif
if ?386SWAT
;--- todo
endif
if ?WDEB386
	mov ebx,[ebp].de.rIP
	mov ecx,[ebp].de.rCSd
	mov eax,[ebp].de.rFL
	lds esi,[ebp].de.rSSSP
	sub esi,3*4
	mov [esi+0],ebx
	mov [esi+4],ecx
	mov [esi+8],eax
	mov [ebp].de.rSP,esi
	mov ax,DS_ForcedGO
	int Debug_Serv_Int
endif
	mov es, dword ptr [ebp].rES
	mov ds, dword ptr [ebp].rDS
	lea esp,[ebp].rEDI
	popad
	lss esp,[esp+4].R3FAULT32.rSSSP
	iretd

calldebugger endp

endif

_TEXT32 ends

_TEXT16 segment

getakey proc near

if ?POLLKBD
	in al,21h
	mov ah,al
	mov al,0FEh		;enable timer, disable keyboard
	out 21h,al
	sti
@@:
	in al,64h		;key from keyboard arrived?
	test al,1
	jz @B
	mov ah,al
	in al,60h		;get data (ack interrupt)
	test ah,20h		;input from PS/2 device?
	jnz @B
	mov dl,al
	mov al,20h
	out 20h,al
	cli
	mov al,ah
	out 21h,al
else
  if 0
	in al,21h
	push ax
	mov al,0FCh		;disable all devices except PIT and kbd
	out 21h,al
  endif
  if ?RESETMOUSE
	xor ax,ax		;reset mouse
	int 33h
  endif
	mov ah,0
	int 16h
	or al,20h
	mov dl,al
  if 0
	pop ax
	out 21h,al
  endif
endif
	ret
getakey endp

_TEXT16 ends

	end
