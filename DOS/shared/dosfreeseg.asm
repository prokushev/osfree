;/*!
;   @file
;
;   @ingroup fapi
;
;   @brief DosFreeSeg DOS wrapper
;
;   (c) osFree Project 2018, <http://www.osFree.org>
;   for licence see licence.txt in root directory, or project website
;
;   This is Family API implementation for DOS, used with BIND tools
;   to link required API
;
;   @author Yuri Prokushev (yuri.prokushev@gmail.com)
;
;*/

.8086

		; Helpers
		INCLUDE	helpers.inc

_TEXT		SEGMENT DWORD PUBLIC 'CODE' USE16

		@PROLOG	DOSFREESEG
Selector	DW	?
		@START	DOSFREESEG
; code here
		@EPILOG	DOSFREESEG

_TEXT		ENDS

		END
