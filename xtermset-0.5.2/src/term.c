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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
  
#include "xtermset.h"

good_term *first_term, *cur_term;

good_term *term_new(int comp_type, char *name, int len) {
   good_term *r;

   r = (good_term *)malloc(sizeof(good_term));
   if(r==NULL) return r;
  
   memset(r,0,sizeof(good_term));
   r->comp_type = comp_type;
   strncpy(r->name,name,len);

   if (first_term) {
      term_last();
      cur_term->next = r;
   } else {
      first_term = r;
   }
   cur_term = r;

   return r;
}

void term_freeall() {
   good_term *n = first_term;

   while(n) {
      good_term *n2;
      n2 = n->next;
      free(n);
      n = n2;
   }
}

good_term *term_first() {
   cur_term = first_term;
   return cur_term;
}

good_term *term_last() {
   good_term *n = cur_term;
   while(n->next)
     n = n->next;
   cur_term = n;
   return cur_term;
}

good_term *term_next() {
   if(cur_term->next) {
      cur_term = cur_term->next;
      return cur_term;
   } else {
      return NULL;
   }
}

good_term *term_prev() {
   good_term *n = first_term;

   while((n->next) && (n != cur_term))
      n = n->next;

   if(n) {
      cur_term = n;
      return cur_term;
   } else {
      return NULL;
   }
}

good_term *term_find(int comp_type, char *name) {
   good_term *n = first_term;
   int fnd = 0;

   while(n && (!fnd)) {
      fnd = ((n->comp_type == comp_type) && (strcmp(n->name,name)==0));
      if(!fnd)
         n = n->next;
   }

   if(n) {
      cur_term = n;
      return cur_term;
   } else {
      return NULL;
   }
}

void term_loadall() {
   FILE *fp;
   good_term *n;
   char line[MAXTERMCHARS+3];
   int i,j=0,l,t=0;
   char c;

   fp = fopen(TERMSDIR "/" TERMSFILE,"r");

   if(fp) {
      while(!feof(fp)) {
         line[0]='\0';
         j=t=0;
         for(i=0;!feof(fp);i++) {
            c=getc(fp);
            if ((c==EOF)||(c=='\n')) 
               break;
            line[i]=c;
         }
         line[i]='\0';
         if((line[0]=='#')||(strlen(line)==0))
            continue;
         l = strlen(line);
         if(line[0]=='^') {
            j = 1;
            l--;
            t |= toStart;
         }
         if(line[i-1] == '$') {
            l--;
            t |= toEnd;
         }
         n = term_new(t,&line[j],l);
      }
      fclose(fp);
   }
}

int is_good_term()
{
   int igt = 0,fnd = 0;
   char *trm;

   trm = getenv("TERM");

   if (trm == NULL) return 0;

   if (term_first()) {
      do {
         fnd = 0;
         if (cur_term->comp_type == 0) { /* anywhere in string */
            fnd = (strstr(trm,cur_term->name) != NULL);
         } else if (cur_term->comp_type & toStart) {
            fnd = (strncmp(trm,cur_term->name,strlen(cur_term->name)) == 0);
         } else if (cur_term->comp_type & toEnd) {
            fnd = (strncmp(trm+(strlen(trm)-strlen(cur_term->name)),cur_term->name,strlen(cur_term->name))==0);
         }
         igt = fnd?1:0;
      }  while ((!fnd) && (term_next()));
   } else {
      if(strncmp(trm,"xterm",5)==0) {
         igt = 1;
      } else if (strncmp(trm,"dtterm",6)==0) {
         igt = 1;
      } else if (strncmp(trm,"rxvt",4)==0) {
         igt = 1;
      }
   }

   return igt;
}
