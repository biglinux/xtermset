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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "xtermset.h"

int main(int argc, char *argv[])
{
    int i,style,code,switches[3];
    char fn[3][MAXFNCHARS-1],c,*tok;
    char line[MAXOPTCHARS+10]; /* ? */
    FILE *rcfile=NULL;
    argument *argptr = NULL;

    for(i=0;i<3;i++) {
       switches[i]=0;
       fn[i][0]='\0';
    }

    if(argc<=2) { /* one command line option, make gnu comliant */
       if(argc==1) {
	  version();
          usage();
          exit(1);
       } else {
	  if(strcmp(argv[1],"--help")==0) {
	     version();
             usage();
	     gplinfo();
             exit(0);
          } else if (strcmp(argv[1],"--version")==0) {
	     version();
             exit(0);
	  }
       }
    }

    term_loadall();

    for (i=1; i<argc; i++) {
	switch (style=nextopt(argv[i], &code)) {
	    case BasicOption:
                MALLOCOPTION
		CHECKFORCOMMAND
		break;
	    case GeomOption:
                MALLOCOPTION
		CHECKFORCOMMAND
		break;
            case ShortOption:
                MALLOCOPTION
                break;
	    case SwitchOption:
                MALLOCOPTION
	        switches[code]=1;
                break;
            case ComplexSwitchOption:
                switches[code]=1;
                if ((i + 1 < argc) && (strncmp(argv[i+1],"-",1)!=0)) {
                    strcpy(fn[code],argv[++i]);
		}
                break;
	    case UnknownOption:
	        version();
		usage();
                argument_freeall();
                term_freeall();
		exit(2);
	}
    }

    if(switches[cswDefault]) {
        rcfile = open_rc_file(fn[cswDefault],"r");
        if(rcfile) {
           while(!feof(rcfile)) {
              line[0]='\0';
              for(i=0;!feof(rcfile);i++) {
                 c=getc(rcfile);
                 if ((c==EOF)||(c=='\n')) break;
                 line[i]=c;
              }
              line[i]='\0';
              if((line[0]=='#')||(strlen(line)==0))
                 continue;
              tok=strtok(line," ");
              if(tok==NULL) continue;
              style=atoi(tok);
              tok=strtok(NULL," ");
              if(tok==NULL) continue;
              code=atoi(tok);

	      argument_first();
	      argptr = argument_find(style,code);
              if(argptr==NULL) {
                 MALLOCOPTION
                 if((tok=strtok(NULL,"\n"))!=NULL)
                    strcpy(argptr->param,tok);
              }

              switch(argptr->style) {
                 case SwitchOption:
                    switches[code]=1;
                    break;
              }
           }
           fclose(rcfile);
        }
    }

    if(switches[cswStore])
        rcfile = open_rc_file(fn[cswStore],"w");

    if(!switches[0]) switches[0] = is_good_term();

    argptr = argument_first();
    while(argptr) {
       if(switches[cswStore] && (rcfile!=NULL)) {
           fprintf(rcfile,"%d %d",current->style,current->command);
           if(strlen(argptr->param)>0)
               fprintf(rcfile," %s",argptr->param);
           fprintf(rcfile,"\n");
       }

       switch (argptr->style) {
	   case BasicOption:
	       if(strlen(argptr->param)>0) {
                   CHECK {
                       printf(ESC "]%d;%s" BEL, argptr->command,argptr->param);
		   }
	       }
	       break;
           case GeomOption:
               if(strlen(argptr->param)>0) {
	           CHECK {
		       set_geom(argptr->param);
		   }
               }
	       break;
	case ShortOption:
	       CHECK {
		   printf(ESC "[%dt", argptr->command);
               }
	       break;
       }
       argptr = argument_next();
    }

    if(switches[cswStore])
        fclose(rcfile);

    term_freeall();
    argument_freeall();

    exit(0);
}

void version() {
    printf("GPF " PACKAGE " " VERSION "\n");
}

void usage()
{
    printf("\n  [-store [<filename>]] [-default [<filename>]]\n");
    printf("  [-force|f] [-title|T window-title] [-n icon-name]\n");
    printf("  [-fg color] [-bg color] [-cr color]\n");
    printf("  [-mousefg color] [-mousebg color]\n");
    printf("  [-tekfg color] [-tekbg color] [-hc color]\n");
    printf("  [-fn|font font-spec] [-geom size-spec]\n");
    printf("  [-iconify] [-restore] [-refresh]\n\n");
    printf("Report bugs to bje@dds.nl.\n");
}

void gplinfo() {
  printf("\nCopyright (C) 2000 Breyten J. Ernsting <bje@dds.nl>\n\n");
  printf("This program is free software; you can redistribute it and/or modify\n");
  printf("it under the terms of the GNU General Public License as published by\n");
  printf("the Free Software Foundation; either version 2 of the License, or\n");
  printf("(at your option) any later version.\n\n");
  printf("This program is distributed in the hope that it will be useful,\n");
  printf("but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
  printf("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
  printf("GNU General Public License for more details.\n\n");
  printf("You should have received a copy of the GNU General Public License\n");
  printf("along with this program; if not, write to the Free Software\n");
  printf("Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.\n\n");
}

int nextopt(char *arg, int *code_return)
{
    OPTION("title", boTitle, BasicOption)
    OPTION("T", boTitle, BasicOption)
    OPTION("n", boIconName, BasicOption)
    OPTION("fg", boForeGround, BasicOption)
    OPTION("bg", boBackGround, BasicOption)
    OPTION("cr", boTextCursor, BasicOption)
    OPTION("mousefg", boMouseForeGround, BasicOption)
    OPTION("mousebg", boMouseBackGround, BasicOption)
    OPTION("tekfg", boTEKForeGround, BasicOption)
    OPTION("tekbg", boTEKBackGround, BasicOption)
    OPTION("hc", boHighlightCursor, BasicOption)
    OPTION("font", boFont, BasicOption)
    OPTION("fn", boFont, BasicOption)
    OPTION("geom", goGeom, GeomOption)
    OPTION("geometry", goGeom, GeomOption)
    OPTION("iconify", soIconify, ShortOption)
    OPTION("restore", soRestore, ShortOption)
    OPTION("refresh", soRefresh, ShortOption)
    OPTION("force",swForce, SwitchOption)
    OPTION("f",swForce,SwitchOption)
    OPTION("store",cswStore,ComplexSwitchOption)
    OPTION("default",cswDefault,ComplexSwitchOption)
    return UnknownOption;
}

void set_geom(char *geometry)
{
    char *x, *y, *width, *height;

    if (strncmp(geometry, "+", 1) == 0) {
	x = strtok(geometry, "+");
	y = strtok(NULL, "");
	printf(ESC "[3;%s;%st", x, y);
    } else {
	width = strtok(geometry, "x");
	height = strtok(NULL, "+");
	printf(ESC "[8;%s;%st", height, width);
	if ((x = strtok(NULL, "+"))) {
	    y = strtok(NULL, "");
	    printf(ESC "[3;%s;%st", x, y);
	}
    }
}

FILE *open_rc_file(char *fn,char *mode)
{
    char fname[MAXFNCHARS-1];
    if(strlen(fn)>0) {
        strcpy(fname,fn);
    } else {
        strcpy(fname,strcat(strcat(getenv("HOME"),"/"),RCFILE));
    }
    return fopen(fname,mode);
}
