# $Id: makefile,v 1.1 2004/08/16 06:27:05 prokushev Exp $

PORT_NAME = os2$(SEP)unzip
PORT_TYPE = git
PORT_URL  = https://github.com/bitwiseworks/unzip-os2
PORT_REV  = master
PORT_PATCHES  = unzip-os2.diff

!include $(%ROOT)tools/mk/port.mk
