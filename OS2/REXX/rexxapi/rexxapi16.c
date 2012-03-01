/*!
   $Id: rexxapi.c,v 1.2 2004/08/16 04:51:08 prokushev Exp $

   @file rexx16.c

   @brief REXX 16-bit function - 16->32 wrappers for old REXX functions

   (c) osFree Project 2004, <http://www.osFree.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   @author Yuri Prokushev (prokushev@freemail.ru)

*/

#include <stdio.h>
#include <string.h>

#define  INCL_OS2DEF

#if defined(HAVE_CONFIG_H)
# include "config.h"
#endif
#include "configur.h"

#include <stdio.h>

#define INCL_REXXSAA

#if defined(OS2)
# define INCL_DOSMODULEMGR
# define INCL_DOSMISC
# undef INCL_REXXSAA
# include <os2.h>
# define INCL_REXXSAA
# define DONT_TYPEDEF_PFN
# define DYNAMIC_OS2
#endif

#include "rexxapi.h"

/*
APIRET APIENTRY RexxLoadMacroSpace( ULONG FuncCount,
                                    PSZ * FuncNames,
                                    PSZ MacroLibFile);
 */

USHORT APIENTRY16 RXMACROLOAD (
         USHORT argc,                      /* Argument count (0==save all)*/
         PSZ16 _Far16 *argv,                   /* List of funct names to save */
         PSZ16 file )                       /* File to save functions in   */
{
  printf("RXMACROLOAD\n");
  return 0;
}


USHORT APIENTRY16 RXMACROSAVE (
         USHORT argc,                      /* Argument count (0==save all)*/
         PSZ16 _Far16 *argv,                   /* List of funct names to save */
         PSZ16 file )                       /* File to save functions in   */
{
  printf("RXMACROSAVE\n");
  return 0;
}


USHORT APIENTRY16 RXSUBCOMLOAD(
         PSZ16 env,                          /* Name of the Environment    */
         PSZ16 dll)                         /* DLL Module Name            */
{
  printf("RXSUBCOMLOAD\n");
  return 0;
}


USHORT APIENTRY16 RXMACRODROP (
         PSZ16 fn )                        /* Name of function to remove */
{
  printf("RXMACRODROP\n");
  return 0;
}


USHORT APIENTRY16 RXEXITQUERY (
         PSZ16 fn,                          /* Exit name                  */
         PSZ16 dll,                          /* DLL Module name.           */
         PUSHORT _Far16 flag,                      /* Existance flag.            */
         double _Far16 *data )               /* User data.                 */
{
  printf("RXEXITQUERY\n");
  return 0;
}


USHORT APIENTRY16 RXMACROERASE(
         VOID )                      /* No Arguments.               */
{
  printf("RXMACROERASE\n");
  return 0;
}


USHORT APIENTRY16 RXSUBCOMDROP(
         PSZ16 env,                          /* Name of the Environment    */
         PSZ16 dll)                         /* DLL Module Name            */
{
  printf("RXSUBCOMDROP\n");
  return 0;
}


USHORT APIENTRY16 RXMACROQUERY (
         PSZ16 fn,                         /* Function to search for      */
         PUSHORT _Far16 flag)                   /* Ptr for position flag return*/
{
  printf("RXMACROQUERY\n");
  return 0;
}


USHORT APIENTRY16 RXMACROCHANGE (
         PSZ16 fn,                         /* Function to add/change      */
         PSZ16 file,                         /* Name of file to get function*/
         USHORT flag)                    /* Flag indicating search pos  */
{
  printf("RXMACROCHANGE\n");
  return 0;
}


USHORT APIENTRY16 RXSUBCOMQUERY(
         PSZ16 env,                          /* Name of the Environment    */
         PSZ16 dll,                          /* DLL Module Name            */
         PUSHORT _Far16 codestor,                      /* Stor for existance code    */
         double _Far16 *userstor)                /* Stor for user word         */
{
  printf("RXSUBCOMQUERY\n");
  return 0;
}


USHORT APIENTRY16 RXFUNCTIONCALL (
        PSZ16 fn,                           /* Name of function to call   */
        USHORT argc,                        /* Number of arguments        */
        PRXSTRING16 argv,                     /* Array of argument strings  */
        PUSHORT _Far16 rc,                       /* RC from function called    */
        PRXSTRING16 stor,                     /* Storage for returned data  */
        PSZ16 data)                         /* Name of active data queue  */
{
  printf("RXFUNCTIONCALL\n");
  return 0;
}


USHORT APIENTRY16 RXEXITREGISTER (
         PSCBLOCK16 scb )                   /* Ptr to SCBLOCK to register */
{
  printf("RXEXITREGISTER\n");
  return 0;
}


USHORT APIENTRY16 RXMACROREORDER(
         PSZ16 fn,                         /* Name of funct change order  */
         USHORT pos)                    /* New position for function   */
{
  printf("RXMACROREORDER\n");
  return 0;
}


USHORT APIENTRY16 RXSUBCOMEXECUTE(
         PSZ16 env,                          /* Name of Subcommand Environ */
         PSZ16 dll,                          /* Module name of its' DLL    */
         PRXSTRING16 cmd,                    /* Command string to be passed*/
         PUSHORT _Far16 flag,                      /* Stor for error flag notice */
         PUSHORT _Far16 rc,                      /* Stor for rc from handler   */
         PRXSTRING16 stor )                  /* Stor for returned string   */
{
  printf("RXSUBCOMEXECUTE\n");
  return 0;
}

APIRET APIENTRY DosSelToFlat(ULONG addr);
APIRET APIENTRY DosFlatToSel(ULONG addr);

USHORT APIENTRY16 RXSUBCOMREGISTER(PSCBLOCK16 PSCB)
{
  char       *subcom_name;
  PFN        subcom_addr;
  APIRET     rc;

  printf("RXSUBCOMREGISTER\n");

  rc = RexxRegisterSubcomExe(PSCB->scbname,             // environment name
                             (void *)PSCB->scbaddr,     // subcommand handler proc
                             (char *)&PSCB->scbuser);   // user area

  printf("rc=%lu\n", rc);  

  return rc;
}


USHORT APIENTRY16 RXFUNCTIONQUERY( PSZ16 fn)
{
  printf("RXFUNCTIONQUERY\n");
  return 0;
}


USHORT APIENTRY16 RXFUNCTIONREGISTER(
        PSZ16 fn,                           /* Name of function to add    */
        PSZ16 dll,                           /* Dll file name (if in dll)  */
        PSZ16 entry,                           /* Entry in dll OR mem address*/
        USHORT flag)                       /* RX_DYNALINK || RX_CALLENTRY*/
{
  printf("RXFUNCTIONREGISTER\n");
  return 0;
}


USHORT APIENTRY16 RXEXITDROP (
         PSZ16 fn,                          /* Exit name                  */
         PSZ16 dll )                        /* DLL module name            */
{
  printf("RXEXITDROP\n");
  return 0;
}


USHORT APIENTRY16 RXFUNCTIONDEREGISTER (PSZ16 fn)
{
  printf("RXFUNCTIONDEREGISTER\n");
  return 0;
}
