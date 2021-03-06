/*
 * THE - The Hessling Editor. A text editor similar to VM/CMS xedit.
 * Copyright (C) 1991-2001 Mark Hessling
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to:
 *
 *    The Free Software Foundation, Inc.
 *    675 Mass Ave,
 *    Cambridge, MA 02139 USA.
 *
 *
 * If you make modifications to this software that you feel increases
 * it usefulness for the rest of the community, please email the
 * changes, enhancements, bug fixes as well as any and all ideas to me.
 * This software is going to be maintained and enhanced as deemed
 * necessary by the community.
 *
 * Mark Hessling,  M.Hessling@qut.edu.au  http://www.lightlink.com/hessling/
 */

/*
$Id: key.h,v 1.4 2001/12/18 08:23:27 mark Exp $
*/

#if defined(USE_EXTCURSES)
# include <cur02.h>
#endif

#include "getch.h"

struct key_values
{
 CHARTYPE *mnemonic;
 int key_value;
 int shift;
};
typedef struct key_values KEYS;

KEYS key_table[] =
{
  {(CHARTYPE *)"A",(int)'a',0},
  {(CHARTYPE *)"B",(int)'b',0},
  {(CHARTYPE *)"C",(int)'c',0},
  {(CHARTYPE *)"D",(int)'d',0},
  {(CHARTYPE *)"E",(int)'e',0},
  {(CHARTYPE *)"F",(int)'f',0},
  {(CHARTYPE *)"G",(int)'g',0},
  {(CHARTYPE *)"H",(int)'h',0},
  {(CHARTYPE *)"I",(int)'i',0},
  {(CHARTYPE *)"J",(int)'j',0},
  {(CHARTYPE *)"K",(int)'k',0},
  {(CHARTYPE *)"L",(int)'l',0},
  {(CHARTYPE *)"M",(int)'m',0},
  {(CHARTYPE *)"N",(int)'n',0},
  {(CHARTYPE *)"O",(int)'o',0},
  {(CHARTYPE *)"P",(int)'p',0},
  {(CHARTYPE *)"Q",(int)'q',0},
  {(CHARTYPE *)"R",(int)'r',0},
  {(CHARTYPE *)"S",(int)'s',0},
  {(CHARTYPE *)"T",(int)'t',0},
  {(CHARTYPE *)"U",(int)'u',0},
  {(CHARTYPE *)"V",(int)'v',0},
  {(CHARTYPE *)"W",(int)'w',0},
  {(CHARTYPE *)"X",(int)'x',0},
  {(CHARTYPE *)"Y",(int)'y',0},
  {(CHARTYPE *)"Z",(int)'z',0},
  {(CHARTYPE *)"S-A",(int)'A',SHIFT_SHIFT},
  {(CHARTYPE *)"S-B",(int)'B',SHIFT_SHIFT},
  {(CHARTYPE *)"S-C",(int)'C',SHIFT_SHIFT},
  {(CHARTYPE *)"S-D",(int)'D',SHIFT_SHIFT},
  {(CHARTYPE *)"S-E",(int)'E',SHIFT_SHIFT},
  {(CHARTYPE *)"S-F",(int)'F',SHIFT_SHIFT},
  {(CHARTYPE *)"S-G",(int)'G',SHIFT_SHIFT},
  {(CHARTYPE *)"S-H",(int)'H',SHIFT_SHIFT},
  {(CHARTYPE *)"S-I",(int)'I',SHIFT_SHIFT},
  {(CHARTYPE *)"S-J",(int)'J',SHIFT_SHIFT},
  {(CHARTYPE *)"S-K",(int)'K',SHIFT_SHIFT},
  {(CHARTYPE *)"S-L",(int)'L',SHIFT_SHIFT},
  {(CHARTYPE *)"S-M",(int)'M',SHIFT_SHIFT},
  {(CHARTYPE *)"S-N",(int)'N',SHIFT_SHIFT},
  {(CHARTYPE *)"S-O",(int)'O',SHIFT_SHIFT},
  {(CHARTYPE *)"S-P",(int)'P',SHIFT_SHIFT},
  {(CHARTYPE *)"S-Q",(int)'Q',SHIFT_SHIFT},
  {(CHARTYPE *)"S-R",(int)'R',SHIFT_SHIFT},
  {(CHARTYPE *)"S-S",(int)'S',SHIFT_SHIFT},
  {(CHARTYPE *)"S-T",(int)'T',SHIFT_SHIFT},
  {(CHARTYPE *)"S-U",(int)'U',SHIFT_SHIFT},
  {(CHARTYPE *)"S-V",(int)'V',SHIFT_SHIFT},
  {(CHARTYPE *)"S-W",(int)'W',SHIFT_SHIFT},
  {(CHARTYPE *)"S-X",(int)'X',SHIFT_SHIFT},
  {(CHARTYPE *)"S-Y",(int)'Y',SHIFT_SHIFT},
  {(CHARTYPE *)"S-Z",(int)'Z',SHIFT_SHIFT},
  {(CHARTYPE *)"0",(int)'0',0},
  {(CHARTYPE *)"1",(int)'1',0},
  {(CHARTYPE *)"2",(int)'2',0},
  {(CHARTYPE *)"3",(int)'3',0},
  {(CHARTYPE *)"4",(int)'4',0},
  {(CHARTYPE *)"5",(int)'5',0},
  {(CHARTYPE *)"6",(int)'6',0},
  {(CHARTYPE *)"7",(int)'7',0},
  {(CHARTYPE *)"8",(int)'8',0},
  {(CHARTYPE *)"9",(int)'9',0},
  {(CHARTYPE *)"`",(int)'`',0},
  {(CHARTYPE *)"-",(int)'-',0},
  {(CHARTYPE *)"=",(int)'=',0},
  {(CHARTYPE *)"[",(int)'[',0},
  {(CHARTYPE *)"]",(int)']',0},
  {(CHARTYPE *)"\\",(int)'\\',0},
  {(CHARTYPE *)";",(int)';',0},
  {(CHARTYPE *)"'",(int)'\'',0},
  {(CHARTYPE *)",",(int)',',0},
  {(CHARTYPE *)".",(int)'.',0},
  {(CHARTYPE *)"/",(int)'/',0},
  {(CHARTYPE *)")",(int)')',0},
  {(CHARTYPE *)"!",(int)'!',0},
  {(CHARTYPE *)"@",(int)'@',0},
  {(CHARTYPE *)"#",(int)'#',0},
  {(CHARTYPE *)"$",(int)'$',0},
  {(CHARTYPE *)"%",(int)'%',0},
  {(CHARTYPE *)"^",(int)'^',0},
  {(CHARTYPE *)"&",(int)'&',0},
  {(CHARTYPE *)"*",(int)'*',0},
  {(CHARTYPE *)"(",(int)'(',0},
  {(CHARTYPE *)"~",(int)'~',0},
  {(CHARTYPE *)"_",(int)'_',0},
  {(CHARTYPE *)"+",(int)'+',0},
  {(CHARTYPE *)"{",(int)'{',0},
  {(CHARTYPE *)"}",(int)'}',0},
  {(CHARTYPE *)"|",(int)'|',0},
  {(CHARTYPE *)":",(int)':',0},
  {(CHARTYPE *)"\"",(int)'\"',0},
  {(CHARTYPE *)"<",(int)'<',0},
  {(CHARTYPE *)">",(int)'>',0},
  {(CHARTYPE *)"?",(int)'?',0},
  {(CHARTYPE *)"SPACE",(int)' ',0},
  {(CHARTYPE *)"ESC",KEY_ESC ,0},
  {(CHARTYPE *)"F0",KEY_F0,0},
  {(CHARTYPE *)"F1",KEY_F1,0},
  {(CHARTYPE *)"F2",KEY_F2,0},
  {(CHARTYPE *)"F3",KEY_F3,0},
  {(CHARTYPE *)"F4",KEY_F4,0},
  {(CHARTYPE *)"F5",KEY_F5,0},
  {(CHARTYPE *)"F6",KEY_F6,0},
  {(CHARTYPE *)"F7",KEY_F7,0},
  {(CHARTYPE *)"F8",KEY_F8,0},
  {(CHARTYPE *)"F9",KEY_F9,0},
  {(CHARTYPE *)"F10",KEY_F10,0},
  {(CHARTYPE *)"F11",KEY_F11,0},
  {(CHARTYPE *)"F12",KEY_F12,0},
  {(CHARTYPE *)"S-F1",KEY_S_F1,SHIFT_SHIFT},
  {(CHARTYPE *)"S-F2",KEY_S_F2,SHIFT_SHIFT},
  {(CHARTYPE *)"S-F3",KEY_S_F3,SHIFT_SHIFT},
  {(CHARTYPE *)"S-F4",KEY_S_F4,SHIFT_SHIFT},
  {(CHARTYPE *)"S-F5",KEY_S_F5,SHIFT_SHIFT},
  {(CHARTYPE *)"S-F6",KEY_S_F6,SHIFT_SHIFT},
  {(CHARTYPE *)"S-F7",KEY_S_F7,SHIFT_SHIFT},
  {(CHARTYPE *)"S-F8",KEY_S_F8,SHIFT_SHIFT},
  {(CHARTYPE *)"S-F9",KEY_S_F9,SHIFT_SHIFT},
  {(CHARTYPE *)"S-F10",KEY_S_F10,SHIFT_SHIFT},
  {(CHARTYPE *)"S-F11",KEY_S_F11,SHIFT_SHIFT},
  {(CHARTYPE *)"S-F12",KEY_S_F12,SHIFT_SHIFT},
  {(CHARTYPE *)"C-F1",KEY_C_F1,SHIFT_CTRL},
  {(CHARTYPE *)"C-F2",KEY_C_F2,SHIFT_CTRL},
  {(CHARTYPE *)"C-F3",KEY_C_F3,SHIFT_CTRL},
  {(CHARTYPE *)"C-F4",KEY_C_F4,SHIFT_CTRL},
  {(CHARTYPE *)"C-F5",KEY_C_F5,SHIFT_CTRL},
  {(CHARTYPE *)"C-F6",KEY_C_F6,SHIFT_CTRL},
  {(CHARTYPE *)"C-F7",KEY_C_F7,SHIFT_CTRL},
  {(CHARTYPE *)"C-F8",KEY_C_F8,SHIFT_CTRL},
  {(CHARTYPE *)"C-F9",KEY_C_F9,SHIFT_CTRL},
  {(CHARTYPE *)"C-F10",KEY_C_F10,SHIFT_CTRL},
  {(CHARTYPE *)"C-F11",KEY_C_F11,SHIFT_CTRL},
  {(CHARTYPE *)"C-F12",KEY_C_F12,SHIFT_CTRL},
  {(CHARTYPE *)"A-F1",KEY_A_F1,SHIFT_ALT},
  {(CHARTYPE *)"A-F2",KEY_A_F2,SHIFT_ALT},
  {(CHARTYPE *)"A-F3",KEY_A_F3,SHIFT_ALT},
  {(CHARTYPE *)"A-F4",KEY_A_F4,SHIFT_ALT},
  {(CHARTYPE *)"A-F5",KEY_A_F5,SHIFT_ALT},
  {(CHARTYPE *)"A-F6",KEY_A_F6,SHIFT_ALT},
  {(CHARTYPE *)"A-F7",KEY_A_F7,SHIFT_ALT},
  {(CHARTYPE *)"A-F8",KEY_A_F8,SHIFT_ALT},
  {(CHARTYPE *)"A-F9",KEY_A_F9,SHIFT_ALT},
  {(CHARTYPE *)"A-F10",KEY_A_F10,SHIFT_ALT},
  {(CHARTYPE *)"A-F11",KEY_A_F11,SHIFT_ALT},
  {(CHARTYPE *)"A-F12",KEY_A_F12,SHIFT_ALT},
  {(CHARTYPE *)"F13",KEY_F13,0},
  {(CHARTYPE *)"F14",KEY_F14,0},
  {(CHARTYPE *)"F15",KEY_F15,0},
  {(CHARTYPE *)"F16",KEY_F16,0},
  {(CHARTYPE *)"F17",KEY_F17,0},
  {(CHARTYPE *)"F18",KEY_F18,0},
  {(CHARTYPE *)"F19",KEY_F19,0},
  {(CHARTYPE *)"F20",KEY_F20,0},
  {(CHARTYPE *)"S-F13",KEY_S_F13,SHIFT_SHIFT},
  {(CHARTYPE *)"S-F14",KEY_S_F14,SHIFT_SHIFT},
  {(CHARTYPE *)"S-F15",KEY_S_F15,SHIFT_SHIFT},
  {(CHARTYPE *)"S-F16",KEY_S_F16,SHIFT_SHIFT},
  {(CHARTYPE *)"S-F17",KEY_S_F17,SHIFT_SHIFT},
  {(CHARTYPE *)"S-F18",KEY_S_F18,SHIFT_SHIFT},
  {(CHARTYPE *)"S-F19",KEY_S_F19,SHIFT_SHIFT},
  {(CHARTYPE *)"NUM0"     ,KEY_Pad0,0},
  {(CHARTYPE *)"NUM1"     ,KEY_C1,0},
  {(CHARTYPE *)"NUM2"     ,KEY_C2,0},
  {(CHARTYPE *)"NUM3"     ,KEY_C3,0},
  {(CHARTYPE *)"NUM4"     ,KEY_B1,0},
  {(CHARTYPE *)"CENTER"   ,KEY_B2,0},
  {(CHARTYPE *)"NUM6"     ,KEY_B3,0},
  {(CHARTYPE *)"NUM7"     ,KEY_A1,0},
  {(CHARTYPE *)"NUM8"     ,KEY_A2,0},
  {(CHARTYPE *)"NUM9"     ,KEY_A3,0},

#if defined(DOS) || defined(OS2) || defined(XCURSES) || defined(WIN32)
/* normal characters */
  {(CHARTYPE *)"CURU",KEY_CURU,0},
  {(CHARTYPE *)"CURD",KEY_CURD,0},
  {(CHARTYPE *)"CURL",KEY_CURL,0},
  {(CHARTYPE *)"CURR",KEY_CURR,0},
  {(CHARTYPE *)"ENTER",KEY_RETURN ,0},
  {(CHARTYPE *)"ENTER",KEY_ENTER,0},
  {(CHARTYPE *)"TAB",KEY_TAB ,0},
  {(CHARTYPE *)"BKSP",KEY_BKSP ,0},
  {(CHARTYPE *)"HOME",KEY_HOME,0},
  {(CHARTYPE *)"PGUP",KEY_PGUP,0},
  {(CHARTYPE *)"PGDN",KEY_PGDN,0},
  {(CHARTYPE *)"END",KEY_END,0},
  {(CHARTYPE *)"INS",KEY_INS,0},
  {(CHARTYPE *)"DEL",KEY_DEL,0},
  {(CHARTYPE *)"PLUS",PADPLUS,0},
  {(CHARTYPE *)"MINUS",PADMINUS,0},
  {(CHARTYPE *)"SLASH",PADSLASH,0},
  {(CHARTYPE *)"STAR",PADSTAR,0},
  {(CHARTYPE *)"NUMENTER",PADENTER,0},
  {(CHARTYPE *)"NUMSTOP",PADSTOP,0},
  {(CHARTYPE *)"MOUSE"    ,KEY_MOUSE,0},
/* shifted characters */
  {(CHARTYPE *)"S-HOME",KEY_SHOME,SHIFT_SHIFT},
  {(CHARTYPE *)"S-END",KEY_SEND,SHIFT_SHIFT},
  {(CHARTYPE *)"S-PGUP",KEY_SPREVIOUS,SHIFT_SHIFT},
  {(CHARTYPE *)"S-PGDN",KEY_SNEXT,SHIFT_SHIFT},
  {(CHARTYPE *)"S-CURU",KEY_S_CURU,SHIFT_SHIFT},
  {(CHARTYPE *)"S-CURD",KEY_S_CURD,SHIFT_SHIFT},
  {(CHARTYPE *)"S-CURL",KEY_SLEFT,SHIFT_SHIFT},
  {(CHARTYPE *)"S-CURR",KEY_SRIGHT,SHIFT_SHIFT},
  {(CHARTYPE *)"S-CENTER",KEY_S_NUMCENTER,SHIFT_SHIFT},
  {(CHARTYPE *)"S-NUM0",KEY_0,SHIFT_SHIFT},
  {(CHARTYPE *)"S-NUM1",KEY_1,SHIFT_SHIFT},
  {(CHARTYPE *)"S-NUM2",KEY_2,SHIFT_SHIFT},
  {(CHARTYPE *)"S-NUM3",KEY_3,SHIFT_SHIFT},
  {(CHARTYPE *)"S-NUM4",KEY_4,SHIFT_SHIFT},
  {(CHARTYPE *)"S-NUM5",KEY_5,SHIFT_SHIFT},
  {(CHARTYPE *)"S-NUM6",KEY_6,SHIFT_SHIFT},
  {(CHARTYPE *)"S-NUM7",KEY_7,SHIFT_SHIFT},
  {(CHARTYPE *)"S-NUM8",KEY_8,SHIFT_SHIFT},
  {(CHARTYPE *)"S-NUM9",KEY_9,SHIFT_SHIFT},
  {(CHARTYPE *)"S-PLUS",SHF_PADPLUS,SHIFT_SHIFT},
  {(CHARTYPE *)"S-MINUS",SHF_PADMINUS,SHIFT_SHIFT},
  {(CHARTYPE *)"S-SLASH",SHF_PADSLASH,SHIFT_SHIFT},
  {(CHARTYPE *)"S-STAR",SHF_PADSTAR,SHIFT_SHIFT},
  {(CHARTYPE *)"S-NUMENTER",SHF_PADENTER,SHIFT_SHIFT},
/* control characters */
  {(CHARTYPE *)"C-2",KEY_C_2,SHIFT_CTRL},
  {(CHARTYPE *)"C-6",KEY_C_6,SHIFT_CTRL},
  {(CHARTYPE *)"C--",KEY_C_MINUS,SHIFT_CTRL},
  {(CHARTYPE *)"C-[",KEY_C_LBRACKET,SHIFT_CTRL},
  {(CHARTYPE *)"C-]",KEY_C_RBRACKET,SHIFT_CTRL},
  {(CHARTYPE *)"C-\\",KEY_C_BSLASH,SHIFT_CTRL},
  {(CHARTYPE *)"C-ENTER",CTL_ENTER,SHIFT_CTRL},
  {(CHARTYPE *)"C-TAB",CTL_TAB,SHIFT_CTRL},
/*  {(CHARTYPE *)"C-BKSP",CTL_BKSP,SHIFT_CTRL},*/
  {(CHARTYPE *)"C-BKSP",127,SHIFT_CTRL},
  {(CHARTYPE *)"C-HOME",CTL_HOME,SHIFT_CTRL},
  {(CHARTYPE *)"C-PGUP",CTL_PGUP,SHIFT_CTRL},
  {(CHARTYPE *)"C-PGDN",CTL_PGDN,SHIFT_CTRL},
  {(CHARTYPE *)"C-CURU",CTL_UP,SHIFT_CTRL},
  {(CHARTYPE *)"C-CURD",CTL_DOWN,SHIFT_CTRL},
  {(CHARTYPE *)"C-CURL",CTL_LEFT,SHIFT_CTRL},
  {(CHARTYPE *)"C-CURR",CTL_RIGHT,SHIFT_CTRL},
  {(CHARTYPE *)"C-END",CTL_END,SHIFT_CTRL},
  {(CHARTYPE *)"C-INS",CTL_INS,SHIFT_CTRL},
  {(CHARTYPE *)"C-DEL",CTL_DEL,SHIFT_CTRL},
  {(CHARTYPE *)"C-CENTER",CTL_PADCENTER,SHIFT_CTRL},
  {(CHARTYPE *)"C-NUMSTOP",CTL_PADSTOP,SHIFT_CTRL},
  {(CHARTYPE *)"C-NUM0",CTL_PAD0,SHIFT_CTRL},
  {(CHARTYPE *)"C-NUM1",CTL_PAD1,SHIFT_CTRL},
  {(CHARTYPE *)"C-NUM2",CTL_PAD2,SHIFT_CTRL},
  {(CHARTYPE *)"C-NUM3",CTL_PAD3,SHIFT_CTRL},
  {(CHARTYPE *)"C-NUM4",CTL_PAD4,SHIFT_CTRL},
  {(CHARTYPE *)"C-NUM5",CTL_PAD5,SHIFT_CTRL},
  {(CHARTYPE *)"C-NUM6",CTL_PAD6,SHIFT_CTRL},
  {(CHARTYPE *)"C-NUM7",CTL_PAD7,SHIFT_CTRL},
  {(CHARTYPE *)"C-NUM8",CTL_PAD8,SHIFT_CTRL},
  {(CHARTYPE *)"C-NUM9",CTL_PAD9,SHIFT_CTRL},
  {(CHARTYPE *)"C-PLUS",CTL_PADPLUS,SHIFT_CTRL},
  {(CHARTYPE *)"C-MINUS",CTL_PADMINUS,SHIFT_CTRL},
  {(CHARTYPE *)"C-SLASH",CTL_PADSLASH,SHIFT_CTRL},
  {(CHARTYPE *)"C-STAR",CTL_PADSTAR,SHIFT_CTRL},
  {(CHARTYPE *)"C-NUMENTER",CTL_PADENTER,SHIFT_CTRL},
/* alt characters */
  {(CHARTYPE *)"A-A",ALT_A,SHIFT_ALT},
  {(CHARTYPE *)"A-B",ALT_B,SHIFT_ALT},
  {(CHARTYPE *)"A-C",ALT_C,SHIFT_ALT},
  {(CHARTYPE *)"A-D",ALT_D,SHIFT_ALT},
  {(CHARTYPE *)"A-E",ALT_E,SHIFT_ALT},
  {(CHARTYPE *)"A-F",ALT_F,SHIFT_ALT},
  {(CHARTYPE *)"A-G",ALT_G,SHIFT_ALT},
  {(CHARTYPE *)"A-H",ALT_H,SHIFT_ALT},
  {(CHARTYPE *)"A-I",ALT_I,SHIFT_ALT},
  {(CHARTYPE *)"A-J",ALT_J,SHIFT_ALT},
  {(CHARTYPE *)"A-K",ALT_K,SHIFT_ALT},
  {(CHARTYPE *)"A-L",ALT_L,SHIFT_ALT},
  {(CHARTYPE *)"A-M",ALT_M,SHIFT_ALT},
  {(CHARTYPE *)"A-N",ALT_N,SHIFT_ALT},
  {(CHARTYPE *)"A-O",ALT_O,SHIFT_ALT},
  {(CHARTYPE *)"A-P",ALT_P,SHIFT_ALT},
  {(CHARTYPE *)"A-Q",ALT_Q,SHIFT_ALT},
  {(CHARTYPE *)"A-R",ALT_R,SHIFT_ALT},
  {(CHARTYPE *)"A-S",ALT_S,SHIFT_ALT},
  {(CHARTYPE *)"A-T",ALT_T,SHIFT_ALT},
  {(CHARTYPE *)"A-U",ALT_U,SHIFT_ALT},
  {(CHARTYPE *)"A-V",ALT_V,SHIFT_ALT},
  {(CHARTYPE *)"A-W",ALT_W,SHIFT_ALT},
  {(CHARTYPE *)"A-X",ALT_X,SHIFT_ALT},
  {(CHARTYPE *)"A-Y",ALT_Y,SHIFT_ALT},
  {(CHARTYPE *)"A-Z",ALT_Z,SHIFT_ALT},
  {(CHARTYPE *)"A-0",ALT_0,SHIFT_ALT},
  {(CHARTYPE *)"A-1",ALT_1,SHIFT_ALT},
  {(CHARTYPE *)"A-2",ALT_2,SHIFT_ALT},
  {(CHARTYPE *)"A-3",ALT_3,SHIFT_ALT},
  {(CHARTYPE *)"A-4",ALT_4,SHIFT_ALT},
  {(CHARTYPE *)"A-5",ALT_5,SHIFT_ALT},
  {(CHARTYPE *)"A-6",ALT_6,SHIFT_ALT},
  {(CHARTYPE *)"A-7",ALT_7,SHIFT_ALT},
  {(CHARTYPE *)"A-8",ALT_8,SHIFT_ALT},
  {(CHARTYPE *)"A-9",ALT_9,SHIFT_ALT},
  {(CHARTYPE *)"A-`",ALT_BQUOTE,SHIFT_ALT},
  {(CHARTYPE *)"A--",ALT_MINUS,SHIFT_ALT},
  {(CHARTYPE *)"A-=",ALT_EQUAL,SHIFT_ALT},
  {(CHARTYPE *)"A-[",ALT_LBRACKET,SHIFT_ALT},
  {(CHARTYPE *)"A-]",ALT_RBRACKET,SHIFT_ALT},
  {(CHARTYPE *)"A-\\",ALT_BSLASH,SHIFT_ALT},
  {(CHARTYPE *)"A-;",ALT_SEMICOLON,SHIFT_ALT},
  {(CHARTYPE *)"A-'",ALT_FQUOTE,SHIFT_ALT},
  {(CHARTYPE *)"A-,",ALT_COMMA,SHIFT_ALT},
  {(CHARTYPE *)"A-.",ALT_STOP,SHIFT_ALT},
  {(CHARTYPE *)"A-/",ALT_FSLASH,SHIFT_ALT},
  {(CHARTYPE *)"A-ENTER",ALT_ENTER,SHIFT_ALT},
  {(CHARTYPE *)"A-TAB",ALT_TAB,SHIFT_ALT},
  {(CHARTYPE *)"A-ESC",ALT_ESC ,SHIFT_ALT},
  {(CHARTYPE *)"A-BKSP",ALT_BKSP ,SHIFT_ALT},
  {(CHARTYPE *)"A-HOME",ALT_HOME,SHIFT_ALT},
  {(CHARTYPE *)"A-PGUP",ALT_PGUP,SHIFT_ALT},
  {(CHARTYPE *)"A-PGDN",ALT_PGDN,SHIFT_ALT},
  {(CHARTYPE *)"A-CURU",ALT_UP,SHIFT_ALT},
  {(CHARTYPE *)"A-CURD",ALT_DOWN,SHIFT_ALT},
  {(CHARTYPE *)"A-CURL",ALT_LEFT,SHIFT_ALT},
  {(CHARTYPE *)"A-CURR",ALT_RIGHT,SHIFT_ALT},
  {(CHARTYPE *)"A-END",ALT_END,SHIFT_ALT},
  {(CHARTYPE *)"A-INS",ALT_INS,SHIFT_ALT},
  {(CHARTYPE *)"A-DEL",ALT_DEL,SHIFT_ALT},
  {(CHARTYPE *)"A-PLUS",ALT_PADPLUS,SHIFT_ALT},
  {(CHARTYPE *)"A-MINUS",ALT_PADMINUS,SHIFT_ALT},
  {(CHARTYPE *)"A-SLASH",ALT_PADSLASH,SHIFT_ALT},
  {(CHARTYPE *)"A-STAR",ALT_PADSTAR,SHIFT_ALT},
  {(CHARTYPE *)"A-NUMENTER",ALT_PADENTER,SHIFT_ALT},
  {(CHARTYPE *)"A-NUMSTOP",ALT_PADSTOP,SHIFT_ALT},
  {(CHARTYPE *)"A-NUM0",ALT_PAD0,SHIFT_ALT},
  {(CHARTYPE *)"A-NUM1",ALT_PAD1,SHIFT_ALT},
  {(CHARTYPE *)"A-NUM2",ALT_PAD2,SHIFT_ALT},
  {(CHARTYPE *)"A-NUM3",ALT_PAD3,SHIFT_ALT},
  {(CHARTYPE *)"A-NUM4",ALT_PAD4,SHIFT_ALT},
  {(CHARTYPE *)"A-NUM5",ALT_PAD5,SHIFT_ALT},
  {(CHARTYPE *)"A-NUM6",ALT_PAD6,SHIFT_ALT},
  {(CHARTYPE *)"A-NUM7",ALT_PAD7,SHIFT_ALT},
  {(CHARTYPE *)"A-NUM8",ALT_PAD8,SHIFT_ALT},
  {(CHARTYPE *)"A-NUM9",ALT_PAD9,SHIFT_ALT},
# if defined(XCURSES)
  {(CHARTYPE *)"S-TAB",KEY_BTAB,SHIFT_SHIFT},
  {(CHARTYPE *)"S-INS",KEY_SIC,SHIFT_SHIFT},
  {(CHARTYPE *)"S-DEL",KEY_SDC,SHIFT_SHIFT},
  {(CHARTYPE *)"SELECT",KEY_SELECT,0},
  {(CHARTYPE *)"PRINT",KEY_PRINT,0},
  {(CHARTYPE *)"S-PRINT",KEY_SPRINT,SHIFT_SHIFT},
  {(CHARTYPE *)"FIND",KEY_FIND,0},
  {(CHARTYPE *)"S-FIND",KEY_SFIND,SHIFT_SHIFT},
  {(CHARTYPE *)"SUSPEND",KEY_SUSPEND,0},
  {(CHARTYPE *)"S-SUSPEND",KEY_SSUSPEND,SHIFT_SHIFT},
  {(CHARTYPE *)"CLEAR",KEY_CLEAR,0},
  {(CHARTYPE *)"OPTIONS",KEY_OPTIONS,0},
  {(CHARTYPE *)"S-OPTIONS",KEY_SOPTIONS,SHIFT_SHIFT},
  {(CHARTYPE *)"BREAK",KEY_BREAK,0},
  {(CHARTYPE *)"CANCEL",KEY_CANCEL,0},
  {(CHARTYPE *)"S-CANCEL",KEY_SCANCEL,SHIFT_SHIFT},
  {(CHARTYPE *)"HELP"       ,KEY_HELP,0},
  {(CHARTYPE *)"S-HELP"     ,KEY_SHELP,SHIFT_SHIFT},
  {(CHARTYPE *)"C-HELP"     ,KEY_LHELP,SHIFT_CTRL},
# else
  {(CHARTYPE *)"S-TAB",KEY_S_TAB,SHIFT_SHIFT},
  {(CHARTYPE *)"S-INS",KEY_S_INS,SHIFT_SHIFT},
  {(CHARTYPE *)"S-DEL",KEY_S_DEL,SHIFT_SHIFT},
# endif
# ifdef KEY_SHIFT_L
  {(CHARTYPE *)"SHIFT-L",KEY_SHIFT_L,SHIFT_MODIFIER_ONLY},
# endif
# ifdef KEY_SHIFT_R
  {(CHARTYPE *)"SHIFT-R",KEY_SHIFT_R,SHIFT_MODIFIER_ONLY},
# endif
# ifdef KEY_CONTROL_L
  {(CHARTYPE *)"CONTROL-L",KEY_CONTROL_L,SHIFT_MODIFIER_ONLY},
# endif
# ifdef KEY_CONTROL_R
  {(CHARTYPE *)"CONTROL-R",KEY_CONTROL_R,SHIFT_MODIFIER_ONLY},
# endif
# ifdef KEY_ALT_L
  {(CHARTYPE *)"ALT-L",KEY_ALT_L,SHIFT_MODIFIER_ONLY},
# endif
# ifdef KEY_ALT_R
  {(CHARTYPE *)"ALT-R",KEY_ALT_R,SHIFT_MODIFIER_ONLY},
# endif
#else
  {(CHARTYPE *)"CURD"     ,KEY_DOWN ,0},
  {(CHARTYPE *)"CURU"     ,KEY_UP ,0},
  {(CHARTYPE *)"CURL"     ,KEY_LEFT ,0},
  {(CHARTYPE *)"CURR"     ,KEY_RIGHT,0},
  {(CHARTYPE *)"BACKSPACE",KEY_BACKSPACE,0},
  {(CHARTYPE *)"HOME"     ,KEY_HOME,0},
  {(CHARTYPE *)"PF1"      ,KEY_PF1,0},
  {(CHARTYPE *)"PF2"      ,KEY_PF2,0},
  {(CHARTYPE *)"PF3"      ,KEY_PF3,0},
  {(CHARTYPE *)"PF4"      ,KEY_PF4,0},
  {(CHARTYPE *)"NUMENTER" ,KEY_NUMENTER,0},
  {(CHARTYPE *)"MINUS"    ,KEY_PadMinus,0},
  {(CHARTYPE *)"NUMSTOP"  ,KEY_PadPeriod,0},
  {(CHARTYPE *)"COMMA"    ,KEY_PadComma,0},
  {(CHARTYPE *)"STAR"     ,KEY_PadStar,0},
  {(CHARTYPE *)"PLUS"     ,KEY_PadPlus,0},
  {(CHARTYPE *)"SLASH"    ,KEY_PadSlash,0},
  {(CHARTYPE *)"S-TAB"    ,KEY_BTAB,SHIFT_SHIFT},
  {(CHARTYPE *)"FIND"     ,KEY_Find,0},
  {(CHARTYPE *)"INS"      ,KEY_InsertHere,0},
  {(CHARTYPE *)"REMOVE"   ,KEY_Remove,0},
  {(CHARTYPE *)"DEL"      ,KEY_DELETE,0},
  {(CHARTYPE *)"SELECT"   ,KEY_Select,0},
  {(CHARTYPE *)"PGUP"     ,KEY_PrevScreen,0},
  {(CHARTYPE *)"PGDN"     ,KEY_NextScreen,0},
#if defined(USE_EXTCURSES)
  {(CHARTYPE *)"TAB"      ,KEY_TAB,0},
  {(CHARTYPE *)"ENTER"    ,KEY_NEWL,0},
#else
  {(CHARTYPE *)"TAB"      ,KEY_C_i,0},
  {(CHARTYPE *)"RETURN"   ,KEY_RETURN,0},
#endif
  {(CHARTYPE *)"CSI"      ,CSI,0},
  {(CHARTYPE *)"BREAK"    ,KEY_BREAK ,0},
  {(CHARTYPE *)"DL"       ,KEY_DL,0},
  {(CHARTYPE *)"IL"       ,KEY_IL,0},
  {(CHARTYPE *)"DC"       ,KEY_DC,0},
  {(CHARTYPE *)"INS"      ,KEY_IC,0},
  {(CHARTYPE *)"EIC"      ,KEY_EIC,0},
  {(CHARTYPE *)"CLEAR"    ,KEY_CLEAR,0},
  {(CHARTYPE *)"EOS"      ,KEY_EOS,0},
  {(CHARTYPE *)"EOL"      ,KEY_EOL,0},
  {(CHARTYPE *)"SF"       ,KEY_SF,0},
  {(CHARTYPE *)"SR"       ,KEY_SR,0},
  {(CHARTYPE *)"PGDN"     ,KEY_NPAGE,0},
  {(CHARTYPE *)"PGUP"     ,KEY_PPAGE,0},
  {(CHARTYPE *)"S-TAB"    ,KEY_STAB,SHIFT_SHIFT},
  {(CHARTYPE *)"C-TAB"    ,KEY_CTAB,SHIFT_CTRL},
  {(CHARTYPE *)"CATAB"    ,KEY_CATAB,0},
  {(CHARTYPE *)"ENTER"    ,KEY_ENTER,0},
  {(CHARTYPE *)"S-RESET"  ,KEY_SRESET,SHIFT_SHIFT},
  {(CHARTYPE *)"RESET"    ,KEY_RESET,0},
  {(CHARTYPE *)"PRINT"    ,KEY_PRINT,0},
  {(CHARTYPE *)"LL"       ,KEY_LL,0},
  {(CHARTYPE *)"A1"       ,KEY_A1,0},
  {(CHARTYPE *)"A3"       ,KEY_A3,0},
  {(CHARTYPE *)"B2"       ,KEY_B2,0},
  {(CHARTYPE *)"C1"       ,KEY_C1,0},
  {(CHARTYPE *)"C3"       ,KEY_C3,0},
  {(CHARTYPE *)"S-TAB"    ,KEY_BTAB,SHIFT_SHIFT},
  {(CHARTYPE *)"BEG"      ,KEY_BEG,0},
  {(CHARTYPE *)"CANCEL"   ,KEY_CANCEL,0},
  {(CHARTYPE *)"CLOSE"    ,KEY_CLOSE,0},
  {(CHARTYPE *)"COMMAND"  ,KEY_COMMAND,0},
  {(CHARTYPE *)"COPY"     ,KEY_COPY,0},
  {(CHARTYPE *)"CREATE"   ,KEY_CREATE,0},
  {(CHARTYPE *)"END"      ,KEY_END,0},
  {(CHARTYPE *)"EXIT"     ,KEY_EXIT,0},
  {(CHARTYPE *)"FIND"     ,KEY_FIND,0},
  {(CHARTYPE *)"HELP"     ,KEY_HELP,0},
  {(CHARTYPE *)"MARK"     ,KEY_MARK,0},
  {(CHARTYPE *)"MESSAGE"  ,KEY_MESSAGE,0},
  {(CHARTYPE *)"MOVE"     ,KEY_MOVE,0},
  {(CHARTYPE *)"NEXT"     ,KEY_NEXT,0},
  {(CHARTYPE *)"OPEN"     ,KEY_OPEN,0},
  {(CHARTYPE *)"OPTIONS"  ,KEY_OPTIONS,0},
  {(CHARTYPE *)"PREVIOUS" ,KEY_PREVIOUS,0},
  {(CHARTYPE *)"REDO"     ,KEY_REDO,0},
  {(CHARTYPE *)"REFERENCE",KEY_REFERENCE,0},
  {(CHARTYPE *)"REFRESH"  ,KEY_REFRESH,0},
  {(CHARTYPE *)"REPLACE"  ,KEY_REPLACE,0},
  {(CHARTYPE *)"RESTART"  ,KEY_RESTART,0},
  {(CHARTYPE *)"RESUME"   ,KEY_RESUME,0},
  {(CHARTYPE *)"SAVE"     ,KEY_SAVE,0},
  {(CHARTYPE *)"S-BEG"    ,KEY_SBEG,SHIFT_SHIFT},
  {(CHARTYPE *)"S-CANCEL" ,KEY_SCANCEL,SHIFT_SHIFT},
  {(CHARTYPE *)"S-COMMAND",KEY_SCOMMAND,SHIFT_SHIFT},
  {(CHARTYPE *)"S-COPY"   ,KEY_SCOPY,SHIFT_SHIFT},
  {(CHARTYPE *)"S-CREATE" ,KEY_SCREATE,SHIFT_SHIFT},
  {(CHARTYPE *)"S-DC"     ,KEY_SDC,SHIFT_SHIFT},
  {(CHARTYPE *)"S-DL"     ,KEY_SDL,SHIFT_SHIFT},
  {(CHARTYPE *)"SELECT"   ,KEY_SELECT,0},
  {(CHARTYPE *)"S-END"    ,KEY_SEND,SHIFT_SHIFT},
  {(CHARTYPE *)"S-EOL"    ,KEY_SEOL,SHIFT_SHIFT},
  {(CHARTYPE *)"S-EXIT"   ,KEY_SEXIT,SHIFT_SHIFT},
  {(CHARTYPE *)"S-FIND"   ,KEY_SFIND,SHIFT_SHIFT},
  {(CHARTYPE *)"S-HELP"   ,KEY_SHELP,SHIFT_SHIFT},
  {(CHARTYPE *)"S-HOME"   ,KEY_SHOME,SHIFT_SHIFT},
  {(CHARTYPE *)"S-INS"    ,KEY_SIC,SHIFT_SHIFT},
  {(CHARTYPE *)"S-CURL"   ,KEY_SLEFT,SHIFT_SHIFT},
  {(CHARTYPE *)"S-MESSAGE",KEY_SMESSAGE,SHIFT_SHIFT},
  {(CHARTYPE *)"S-MOVE"   ,KEY_SMOVE,SHIFT_SHIFT},
  {(CHARTYPE *)"S-NEXT"   ,KEY_SNEXT,SHIFT_SHIFT},
  {(CHARTYPE *)"S-OPTIONS",KEY_SOPTIONS,SHIFT_SHIFT},
  {(CHARTYPE *)"S-PREVIOUS",KEY_SPREVIOUS,SHIFT_SHIFT},
  {(CHARTYPE *)"S-PRINT"  ,KEY_SPRINT,SHIFT_SHIFT},
  {(CHARTYPE *)"S-REDO"   ,KEY_SREDO,SHIFT_SHIFT},
  {(CHARTYPE *)"S-REPLACE",KEY_SREPLACE,SHIFT_SHIFT},
  {(CHARTYPE *)"S-CURR"   ,KEY_SRIGHT,SHIFT_SHIFT},
  {(CHARTYPE *)"S-RSUME"  ,KEY_SRSUME,SHIFT_SHIFT},
  {(CHARTYPE *)"S-SAVE"   ,KEY_SSAVE,SHIFT_SHIFT},
  {(CHARTYPE *)"S-SUSPEND",KEY_SSUSPEND,SHIFT_SHIFT},
  {(CHARTYPE *)"S-UNDO"   ,KEY_SUNDO,SHIFT_SHIFT},
  {(CHARTYPE *)"SUSPEND"  ,KEY_SUSPEND,0},
  {(CHARTYPE *)"UNDO"     ,KEY_UNDO,0},
  {(CHARTYPE *)"C-CURL"   ,CTL_LEFT,SHIFT_CTRL},
  {(CHARTYPE *)"C-CURR"   ,CTL_RIGHT,SHIFT_CTRL},
  {(CHARTYPE *)"C-CURU"   ,CTL_UP,SHIFT_CTRL},
  {(CHARTYPE *)"C-CURD"   ,CTL_DOWN,SHIFT_CTRL},
  {(CHARTYPE *)"C-HOME"   ,CTL_HOME,SHIFT_CTRL},
  {(CHARTYPE *)"C-END"    ,CTL_END,SHIFT_CTRL},
  {(CHARTYPE *)"C-PGUP"   ,CTL_PGUP,SHIFT_CTRL},
  {(CHARTYPE *)"C-PGDN"   ,CTL_PGDN,SHIFT_CTRL},
#endif
  {(CHARTYPE *)"C-A",(int)KEY_C_a,SHIFT_CTRL},
  {(CHARTYPE *)"C-B",(int)KEY_C_b,SHIFT_CTRL},
  {(CHARTYPE *)"C-C",(int)KEY_C_c,SHIFT_CTRL},
  {(CHARTYPE *)"C-D",(int)KEY_C_d,SHIFT_CTRL},
  {(CHARTYPE *)"C-E",(int)KEY_C_e,SHIFT_CTRL},
  {(CHARTYPE *)"C-F",(int)KEY_C_f,SHIFT_CTRL},
  {(CHARTYPE *)"C-G",(int)KEY_C_g,SHIFT_CTRL},
  {(CHARTYPE *)"C-H",(int)KEY_C_h,SHIFT_CTRL},
  {(CHARTYPE *)"C-I",(int)KEY_C_i,SHIFT_CTRL},
  {(CHARTYPE *)"C-J",(int)KEY_C_j,SHIFT_CTRL},
  {(CHARTYPE *)"C-K",(int)KEY_C_k,SHIFT_CTRL},
  {(CHARTYPE *)"C-L",(int)KEY_C_l,SHIFT_CTRL},
  {(CHARTYPE *)"C-M",(int)KEY_C_m,SHIFT_CTRL},
  {(CHARTYPE *)"C-N",(int)KEY_C_n,SHIFT_CTRL},
  {(CHARTYPE *)"C-O",(int)KEY_C_o,SHIFT_CTRL},
  {(CHARTYPE *)"C-P",(int)KEY_C_p,SHIFT_CTRL},
  {(CHARTYPE *)"C-Q",(int)KEY_C_q,SHIFT_CTRL},
  {(CHARTYPE *)"C-R",(int)KEY_C_r,SHIFT_CTRL},
  {(CHARTYPE *)"C-S",(int)KEY_C_s,SHIFT_CTRL},
  {(CHARTYPE *)"C-T",(int)KEY_C_t,SHIFT_CTRL},
  {(CHARTYPE *)"C-U",(int)KEY_C_u,SHIFT_CTRL},
  {(CHARTYPE *)"C-V",(int)KEY_C_v,SHIFT_CTRL},
  {(CHARTYPE *)"C-W",(int)KEY_C_w,SHIFT_CTRL},
  {(CHARTYPE *)"C-X",(int)KEY_C_x,SHIFT_CTRL},
  {(CHARTYPE *)"C-Y",(int)KEY_C_y,SHIFT_CTRL},
  {(CHARTYPE *)"C-Z",(int)KEY_C_z,SHIFT_CTRL},
#if defined(BUTTON_MODIFER_MASK_NOT_NEEDED)
  {(CHARTYPE *)"PB1",(int)KEY_PB1,0},
  {(CHARTYPE *)"PB2",(int)KEY_PB2,0},
  {(CHARTYPE *)"PB3",(int)KEY_PB3,0},
  {(CHARTYPE *)"S-PB1",(int)KEY_S_PB1,0},
  {(CHARTYPE *)"S-PB2",(int)KEY_S_PB2,0},
  {(CHARTYPE *)"S-PB3",(int)KEY_S_PB3,0},
  {(CHARTYPE *)"C-PB1",(int)KEY_C_PB1,0},
  {(CHARTYPE *)"C-PB2",(int)KEY_C_PB2,0},
  {(CHARTYPE *)"C-PB3",(int)KEY_C_PB3,0},
  {(CHARTYPE *)"A-PB1",(int)KEY_A_PB1,0},
  {(CHARTYPE *)"A-PB2",(int)KEY_A_PB2,0},
  {(CHARTYPE *)"A-PB3",(int)KEY_A_PB3,0},
  {(CHARTYPE *)"RB1",(int)KEY_RB1,0},
  {(CHARTYPE *)"RB2",(int)KEY_RB2,0},
  {(CHARTYPE *)"RB3",(int)KEY_RB3,0},
  {(CHARTYPE *)"S-RB1",(int)KEY_S_RB1,0},
  {(CHARTYPE *)"S-RB2",(int)KEY_S_RB2,0},
  {(CHARTYPE *)"S-RB3",(int)KEY_S_RB3,0},
  {(CHARTYPE *)"C-RB1",(int)KEY_C_RB1,0},
  {(CHARTYPE *)"C-RB2",(int)KEY_C_RB2,0},
  {(CHARTYPE *)"C-RB3",(int)KEY_C_RB3,0},
  {(CHARTYPE *)"A-RB1",(int)KEY_A_RB1,0},
  {(CHARTYPE *)"A-RB2",(int)KEY_A_RB2,0},
  {(CHARTYPE *)"A-RB3",(int)KEY_A_RB3,0},
  {(CHARTYPE *)"DB1",(int)KEY_DB1,0},
  {(CHARTYPE *)"DB2",(int)KEY_DB2,0},
  {(CHARTYPE *)"DB3",(int)KEY_DB3,0},
  {(CHARTYPE *)"S-DB1",(int)KEY_S_DB1,0},
  {(CHARTYPE *)"S-DB2",(int)KEY_S_DB2,0},
  {(CHARTYPE *)"S-DB3",(int)KEY_S_DB3,0},
  {(CHARTYPE *)"C-DB1",(int)KEY_C_DB1,0},
  {(CHARTYPE *)"C-DB2",(int)KEY_C_DB2,0},
  {(CHARTYPE *)"C-DB3",(int)KEY_C_DB3,0},
  {(CHARTYPE *)"A-DB1",(int)KEY_A_DB1,0},
  {(CHARTYPE *)"A-DB2",(int)KEY_A_DB2,0},
  {(CHARTYPE *)"A-DB3",(int)KEY_A_DB3,0},
#endif
  {(CHARTYPE *)NULL    ,0,0},
};
