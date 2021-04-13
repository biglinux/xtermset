/* xtermset: change some settings of an xterm from the command line */

/*
** Copyright (C) 2000 Breyten J. Ernsting <bje@dds.nl>
**  
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**  
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**  
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
**  
*/

/* this was orginally 'setxtitle' in the splitvt package,
 * created by Sam Lantinga(slouken@cs.ucdavis.edu)
 * rewritten by Breyten Ernsting (bje@dds.nl)
 * and again by Decklin Foster (decklin@home.com) */

#ifndef __XTERMSET_H__
#define __XTERMSET_H__

#ifdef HAVE_CONFIG_H
#include "../config.h"
#else
#define PACKAGE         "xtermset"
#define VERSION		"0.5.2"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#endif /* STDC_HEADERS */

#define ESC		"\033"
#define BEL		"\07"
#define RCFILE		".xtermsetrc"
#define TERMSFILE	"terms"
#define MAXFNCHARS	255
#define MAXOPTCHARS	255
#define MAXTERMCHARS	255

#define BasicOption		1
#define ShortOption		2
#define GeomOption		3
#define UnknownOption		4
#define SwitchOption		5
#define ComplexSwitchOption	6

#define boTitle			2
#define boIconName		1
#define boForeGround		10
#define boBackGround		11
#define boTextCursor		12
#define boMouseForeGround	13
#define boMouseBackGround	14
#define boTEKForeGround		15
#define boTEKBackGround		16
#define boHighlightCursor	17
#define boFont			50

#define goGeom			(-1)

#define soIconify		2
#define soRestore		1
#define soRefresh		7

#define swForce			0

#define cswStore		1
#define cswDefault		2

#define toStart			1
#define toEnd			2

#define OPTION(name, code, style) \
if (strcmp(arg, "-" name) == 0) { *code_return = code; return style; } 

#define CHECK if(switches[0])

#define CHECKFORCOMMAND if ((i + 1 < argc) && (strncmp(argv[i+1],"-",1)!=0)) { \
   strcpy(argptr->param,argv[++i]); \
}

#define MALLOCOPTION argptr=argument_new(style,code,NULL);

/* argument stuff */

typedef struct argument {
   int style;
   int command;
   char param[MAXOPTCHARS-1];
   struct argument *next,*previous;
} argument;

extern argument *first,*current;

argument *argument_new(int style, int code, char *param);
void argument_freeall();
argument *argument_first();
argument *argument_last();
argument *argument_next();
argument *argument_prev();
void argument_save();
argument *argument_pop();
argument *argument_delete(int afree);
argument *argument_find(int style, int code);

/* term stuff */
typedef struct good_term {
  int comp_type;
  char name[MAXTERMCHARS];
  struct good_term *next;
} good_term;

extern good_term *first_term, *cur_term;

good_term *term_new(int comp_type, char *name, int len);
void term_freeall();
good_term *term_first();
good_term *term_last();
good_term *term_next();
good_term *term_prev();
good_term *term_find(int comp_type, char *name);
void term_loadall();

void version();
void usage();
void gplinfo();
int nextopt();
void set_geom();
int is_good_term(); /* 1 if good terminal */
FILE *open_rc_file(char *fn,char *mode);
#endif /* __XTERMSET_H__ */

