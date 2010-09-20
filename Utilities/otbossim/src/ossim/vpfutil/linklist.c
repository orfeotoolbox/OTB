/*************************************************************************
 *
 *N  Module LINKLIST.C
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This module contains functions that make up a singly linked list
 *     data structure.  It is generic in the sense that it can hold any
 *     type of data, including user-defined types and structures.  That
 *     is why you must treat the data element as a void pointer and pass
 *     in its size when inserting into the list.  These routines are
 *     assured of working with "non-pointer" types of data elements.
 *     If you try storing other lists, or structures with pointers hanging
 *     off of them, the results will become unpredictable.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    N/A
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    linked_list_type ll_init();
 *    int ll_empty( linked_list_type list );
 *    position_type ll_first( linked_list_type list );
 *    position_type ll_last( linked_list_type list );
 *    position_type ll_next( position_type position );
 *    position_type ll_previous( position_type position, linked_list_type
 *                  list );
 *    int ll_end( position_type position );
 *    void ll_element( position_type position, void *element );
 *    void ll_insert( void *element, unsigned size, position_type position );
 *    void ll_delete( position_type position );
 *    void ll_reset( linked_list_type list );
 *    position_type ll_locate( void *element, linked_list_type list );
 *    void ll_replace( void *element, position_type position );
 *E
 *************************************************************************/
#ifdef __MSDOS__
#include <alloc.h>
#include <mem.h>
#else
#ifdef __APPLE__ 
#include <sys/types.h>
#include <sys/malloc.h>
#else
#ifndef __FreeBSD__
#include <malloc.h>
#include <string.h>
#endif
#endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ossim/vpfutil/linklist.h>
#include <string.h>

#ifndef __MSDOS__
#define farmalloc malloc
#define farfree free
#define movmem(a,b,c) memmove(b,a,c)
#endif

/*************************************************************************
 *
 *N  ll_init
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function allocates and initializes a new linked list structure.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    ll_init <output> == (linked_list_type) initialized head of the list.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *************************************************************************/
linked_list_type ll_init()
{
   linked_list_type list;

   if ((list = (linked_list_type) farmalloc( sizeof(cell_type) ))==NULL) {
      printf("Out of memory in ll_init()\n");
      exit(1);
   }
   list->element = NULL;
   list->element_size = 0;
   list->next = NULL;
   return list;
}


/*************************************************************************
 *
 *N  ll_empty
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function TRUE if the list is empty and FALSE if it is not.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    list      <input> == (linked_list_type) linked list being checked.
 *    ll_empty <output> == (int) boolean function result.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *************************************************************************/
int ll_empty( linked_list_type list )
{
   if (list == NULL) return TRUE;
   if (list->next == NULL)
      return TRUE;
   else
      return FALSE;
}


/*************************************************************************
 *
 *N  ll_first
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the head of the list.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    list      <input> == (linked_list_type) linked list.
 *    ll_first <output> == (position_type) head of the list.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *************************************************************************/
position_type ll_first( linked_list_type list )
{
   return ((position_type) list);
}


/*************************************************************************
 *
 *N  ll_last
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns *THE* last position in the list, which is
 *     not useable.  Use ll_previous to get to the las USEABLE link in
 *     the list.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    list     <input> == (linked_list_type) linked list.
 *    ll_last <output> == (position_type) tail of the list.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *************************************************************************/
position_type ll_last( linked_list_type list )
{
   position_type p;

   p = (position_type) list;
   while (p->next != NULL)
      p = p->next;
   return p;
}


/*************************************************************************
 *
 *N  ll_next
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the next position in the list.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    position <input> == (position_type) current position in the list.
 *    ll_next <output> == (position_type) next position in the list.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *************************************************************************/
position_type ll_next( position_type position )
{
   return(position->next);
}


/*************************************************************************
 *
 *N  ll_previous
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the previous position in the list.  Note:
 *     This is a singly linked list -> no backward pointer -> this
 *     operation is relatively inefficient.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    position     <input> == (position_type) current position.
 *    list         <input> == (linked_list_type) linked list.
 *    ll_previous <output> == (position_type) previous position in the list.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *************************************************************************/
position_type ll_previous( position_type    position,
			   linked_list_type list )
{
   position_type p;

   if (position==list) return(position);
   p = list;
   while (p->next != position)
      p = p->next;
   return(p);
}


/*************************************************************************
 *
 *N  ll_end
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function determines if the given position is at the end of the
 *     list.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    position <input> == (position_type) current position in the list.
 *    ll_end  <output> == (int) TRUE  -- if position is the end of the list.
 *                              FALSE -- if not.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *************************************************************************/
int ll_end( position_type position )
{
   if (position == NULL)
      return(TRUE);
   else {
      if (position->next == NULL)
	 return(TRUE);
      else
	 return(FALSE);
   }
}


/*************************************************************************
 *
 *N  ll_element
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function copies the element at position in the list into the
 *     contents of element.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    position <input> == (position_type) position in the list.
 *    element <output> == (void *) pointer to the element data.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *************************************************************************/
void ll_element( position_type position,
		 void         *element )
{
   movmem(position->next->element,element,position->next->element_size);
}

/*************************************************************************
 *
 *N  ll_insert
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function inserts a new cell into the list at position that will
 *     contain the data pointed to by element.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    element  <input> == (void *) pointer to the data element to insert.
 *    size     <input> == (unsigned) size of the data element.
 *    position <input> == (position_type) position to insert the new cell.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *************************************************************************/
void ll_insert( void          *element,
		unsigned      size,
		position_type position )
{
   position_type temp;

   if ((temp = (position_type) farmalloc( sizeof(cell_type) )) == NULL) {
      printf("out of memory\n");
      abort();
   }
   temp->next = position->next;
   position->next = temp;
   temp->element_size = size;
   if ((temp->element = farmalloc( size ))==NULL) {
      printf("out of memory\n");
      abort();
   }
   movmem(element,temp->element,size);

}


/*************************************************************************
 *
 *N  ll_delete
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function deletes and disposes of a cell from the linked list.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    position <input> == (position_type) position in the list to delete.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *************************************************************************/
void ll_delete( position_type position )
{
   position_type    p;

   if (position != NULL) {  /* Cut the element out of the chain */
      p = position->next;
      position->next = p->next;
      farfree( p->element );
      farfree( p );
   }
}




/*************************************************************************
 *
 *N  ll_reset
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function empties out a linked list.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    list <inout> == (linked_list_type) linked list to be emptied.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    void ll_delete( position_type position );
 *    int ll_empty( linked_list_type list );
 *E
 *************************************************************************/
void ll_reset( linked_list_type list )
{
   while (! ll_empty(list))
      ll_delete(ll_first(list));
   farfree(list);
   list = NULL;
}



/*************************************************************************
 *
 *N  ll_locate
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function locates a position in the list by comparing data.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    element <input> == (void *) pointer to the data element to locate.
 *    list    <input> == (linked_list_type) linked list.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *************************************************************************/
position_type ll_locate( void             *element,
			 linked_list_type list )
{
   position_type p;

   p = list;
   while (p->next != NULL) {
      if ( memcmp(p->next->element,element,p->next->element_size) == 0 )
	 return p;
      else
	 p = p->next;
   }
   return NULL;
}


/*************************************************************************
 *
 *N  ll_replace
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function replaces an element in the linked list at the given
 *     position.  WARNING:  The new data element must be the same size as
 *     the previous data element or you will get some rather INTERESTING
 *     results.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    element  <input> == (void *) data element to replace existing data.
 *    position <input> == (position_type) position in the list to replace
 *                        the data.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *************************************************************************/
void ll_replace( void          *element,
		 position_type position )
{
   movmem(element,position->next->element,position->next->element_size);
}



