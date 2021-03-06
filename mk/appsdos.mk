#
#
#
#

!ifndef __appsdos_mk__
!define __appsdos_mk__

!include $(%ROOT)/mk/site.mk

ADD_COPT =            $(ADD_COPT) &
                      -bt=dos
#                      -i=$(ROOT)$(SEP)build$(SEP)include &
#                      -i=$(ROOT)$(SEP)build$(SEP)include$(SEP)dos &

!ifndef DEST
DEST     = os2$(SEP)mdos
!endif

!include $(%ROOT)/mk/all.mk

!ifeq COM 1
com = com
!else
com = 
!endif

TARGETS  = $(PATH)$(TRGT)

$(PATH)$(PROJ).lnk: $(OBJS)
 @%create $^@
 @%append $^@ SYSTEM dos $(com)
 @%append $^@ NAME $^*
!ifdef ALIASES
 alias $(ALIASES)
!endif
!ifdef STACKSIZE
 @%append $^@ OPTION ST=$(STACKSIZE)
!endif
 @%append $^@ OPTION MAP=$^*.wmp
 $(ADDFILES_CMD)

$(PATH)$(TRGT): $(PATH)$(PROJ).lnk $(OBJS)
 @$(SAY) LINK     $^. $(LOG)
 $(verbose)$(LINKER) $(LINKOPT) @$[@ $(LOG)

!endif
