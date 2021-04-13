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

argument *first,*current,*save;

argument *argument_new(int style, int code, char *param) {
   argument *r;

   r = (argument *)malloc(sizeof(argument));
   if(r==NULL) return r;

   memset(r,0,sizeof(argument));

   r->style = style;
   r->command = code;
   if(param!=NULL)
      strcpy(r->param,param);

   r->next = NULL;

   if(first==NULL) {
      first = r;
      first->previous = NULL;
      first->next = NULL;
   }

   if(first->previous != NULL) {
      first->previous->next = r;
      r->previous=first->previous;
   }
   
   first->previous = r;

   current = r;

   return r;
}

void argument_freeall() {
   current = first;

   while(current!=NULL) {
      argument *n = current->next;
      free(current);
      current=n;
   }

   current = NULL;
}

argument *argument_first() {
   if(first!=NULL) {
      current = first;
      return current;
   } else {
      return NULL;
   }
}

argument *argument_last() {
   if(first!=NULL) {
      current=first->previous;
      return current;
   } else {
      return NULL;
   }
}

argument *argument_next() {
   if(current!=NULL) {
      if (current->next) {
         current = current->next;
         return current;
      } else {
         return NULL;
      }
   } else {
      return NULL;
   }
}

argument *argument_prev() {
   if(current!=first) {
      if (current->previous) {  
         current = current->previous;
         return current;
      } else {
         return NULL;
      }
   } else {
      return NULL;
   }
}

void argument_save() {
   save = current;
}
argument *argument_pop() {
   current = save;
   return save;
}

argument *argument_delete(int afree) {
   argument *prev,*next,*tmp;
   if(current!=NULL) {
      prev = current->previous;
      next = current->next;
      tmp = current;
      if(prev!=NULL) {
	 prev->next = (tmp!=first)?next:NULL;
      }
      if(next!=NULL)
	next->previous = prev;
      if(tmp==first)
	first=next;
      if(afree) {
	 free(tmp);
	 return NULL;
      } else {
	 return tmp;
      }
   } else {
      return NULL;
   }
}

argument *argument_find(int style, int code) {
   argument *tmp = first;
   int fnd = 0;

   while((tmp!=NULL)&&(!fnd)) {
      fnd=((tmp->style==style)&&(tmp->command==code));
      if(!fnd)
	tmp=tmp->next;
   }
   
   if(tmp) {
      return tmp;
   } else {
      return NULL;
   }
}
