/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   04 avril 2006
  Version   :   
  Role      :   Basic memory routines for the Point_plane, Shape and Shapes structures
  $Id: $

=========================================================================*/

/*~~~~~~~~~~  This file is part of the MegaWave2 system library ~~~~~~~~~~~~~~~
MegaWave2 is a "soft-publication" for the scientific community. It has
been developed for research purposes and it comes without any warranty.
The last version is available at http://www.cmla.ens-cachan.fr/Cmla/Megawave
CMLA, Ecole Normale Superieure de Cachan, 61 av. du President Wilson,
      94235 Cachan cedex, France. Email: megawave@cmla.ens-cachan.fr 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef __otbShape_txx
#define __otbShape_txx

#include "otbShape.h"


namespace otb
{



/*--- Shape ---*/

/* Get in the subtree of root sh a shape that is not removed, NULL if all shapes are removed */

Shape* 
Shape::mw_get_not_removed_shape(Shape *sh)
{
  Shape *NotSup = NULL;
  if((sh == NULL) || (!sh->removed))
    return(sh);
  for(sh = sh->child; sh != NULL; sh = sh->next_sibling)
    if((NotSup = mw_get_not_removed_shape(sh)) != NULL)
      break;
  return(NotSup);
}

/* Get the true parent, that is the greatest non removed ancestor */

Shape* 
Shape::mw_get_parent_shape(Shape *sh)
{
  if(sh == NULL)
    {
      std::cerr <<"[mw_get_parent_shape] input shape structure is NULL" << std::endl;
      return(NULL);
    }
  if(sh->parent == NULL) /* It is the root of the shape */
    return(NULL);

  do
    if((sh = sh->parent) == NULL)
      {
	std::cerr << "[mw_get_parent_shape] the root of the shapes is removed" << std::endl; 
	return(NULL);
      }
  while(sh->removed);
  return(sh);
}

/* Get the first child, taking into account that some shapes are removed */

Shape* 
Shape::mw_get_first_child_shape(Shape *sh)
{
  Shape *NotSup = NULL;
  if(sh == NULL)
    {
      std::cerr << "[mw_get_first_child_shape] input shape is NULL" << std::endl; 
      return(NULL);
    }
  for(sh = sh->child; sh != NULL; sh = sh->next_sibling)
    if((NotSup = mw_get_not_removed_shape(sh)) != NULL)
      break;
  return(NotSup);  
}


/* Get the next sibling, taking into account that some shapes are removed */

Shape* 
Shape::mw_get_next_sibling_shape(Shape *sh)
{
  Shape *sh1 = NULL;
  Shape *sh2 = NULL;
  if(sh == NULL)
    {
      std::cerr <<"[mw_get_next_sibling_shape] input shape is NULL"; 
      return(NULL);
    }
  /* First look at the siblings in the original tree */
  for(sh1 = sh->next_sibling; sh1 != NULL; sh1 = sh1->next_sibling)
    if((sh2 = mw_get_not_removed_shape(sh1)) != NULL)
      return(sh2);
  if(sh->parent == NULL || ! sh->parent->removed) 
    return(NULL); /* The parent in the original tree is also the parent in the true tree, nothing more to do */
  /* If not found, find the node in the original tree just before the true parent */
  do
    {
      sh = sh->parent;
      /* Look at the siblings of this node */
      for(sh1 = sh->next_sibling; sh1 != NULL; sh1 = sh1->next_sibling)
	if((sh2 = mw_get_not_removed_shape(sh1)) != NULL)
	  return(sh2);
    }   while(sh->parent->removed);
  return(NULL);
}



/* Insert a new shape and its siblings in the tree, with parent pParent */
void 
Shape::insert_children(Shape* pNewChildToInsert)
{

  Shape* pSibling = pNewChildToInsert;
  while(pSibling->next_sibling != NULL) {
    pSibling->parent = parent;
    pSibling = pSibling->next_sibling;
  }
  pSibling->parent = parent;
  pSibling->next_sibling = child;
  child = pNewChildToInsert;
}

/*-- Shapes --*/


/* Allocate the root of the shapes */

void
Shapes::mw_alloc_shapes( int nrow, int  ncol, float value)
{
  int size,i;
  Shape *root;

  
  size = nrow*ncol;
  if (size <= 0)
    {
      std::cerr << "[mw_alloc_shapes] Attempts to alloc shapes with null size" << std::endl;
    }
  if ((the_shapes!=NULL)||(smallest_shape!=NULL))
    {
      std::cerr << "[mw_alloc_shapes] Attempts to alloc root which is already allocated" << std::endl;
    }

  root = the_shapes = new Shape[size+1];
  if (root==NULL)
    {
      std::cerr <<"[mw_alloc_shapes] Not enough memory" <<std::endl;
    }
  root->inferior_type = 1;
  root->value         = value;
  root->open          = 1;
  root->area          = size;
  root->removed       = 0;
  root->pixels        = NULL;
  root->parent        = NULL;
  root->next_sibling  = NULL;
  root->child         = NULL;

  nb_shapes = 1;
  nrow      = nrow;
  ncol      = ncol;  

  typedef Shape* ShapePtr;
  smallest_shape = new ShapePtr[size];
  if (smallest_shape==NULL)
    {
      std::cerr <<"[mw_alloc_shapes] Not enough memory" << std::endl;
      delete root;
    }
  for (i=size-1; i>=0; i--) smallest_shape[i] = root;
}


/* Allocate the struct and the root if they are not defined */

void
Shapes::mw_change_shapes(int nrow,int ncol,float value)
{
  int size,i;

  /* Deallocate the shapes but the structure itself */
  std::cout << "mw_change_shapes  1 " << std::endl;
  std::cout << "nb shapes  : " <<this->nb_shapes << std::endl;
  std::cout << "the shapes : " <<the_shapes << std::endl;
  if((the_shapes != NULL) && (nb_shapes > 0))
    delete[] the_shapes[0].pixels;
  std::cout << "mw_change_shapes  2 " << std::endl;
  if (the_shapes != NULL) delete[] the_shapes;
  std::cout << "mw_change_shapes  3 " << std::endl;
  if (smallest_shape != NULL) delete[] smallest_shape;
  std::cout << "mw_change_shapes  4 " << std::endl;

  mw_alloc_shapes(nrow, ncol, value);
}



/* Return the smallest shape containing the given pixel */

Shape* 
Shapes::mw_get_smallest_shape(int iX,int iY)
{
  Shape *sh;
  
  sh = smallest_shape[iY*ncol+iX];
  if (sh == NULL)
    {
      std::cerr << "[mw_get_smallest_shape] smallest shape is NULL" << std::endl; 
      return(NULL);
    }  
  if(sh->removed)
    sh = sh->mw_get_parent_shape(sh);
  return(sh);
}

void 
Shapes::compute_proper_pixels(int *tabNbOfProperPixels)
{
  Shape *pShape;
  int   *pNbOfProperPixels;
  int   i;
  
  /* 1) Initialize by the area */
  pShape = the_shapes + nb_shapes-1;
  pNbOfProperPixels = tabNbOfProperPixels + nb_shapes-1;
  for(i = nb_shapes-1; i >= 0; i--)
    *pNbOfProperPixels-- = (pShape--)->area;
  /* 2) For each shape, substract its area to its parent */
  pShape = the_shapes + nb_shapes-1;
  for(i = nb_shapes-1; i > 0; i--, pShape--)
    tabNbOfProperPixels[pShape->parent - the_shapes] -= pShape->area;

}

void 
Shapes::allocate_pixels(int* tabNbOfProperPixels)
{
  Point_plane  *tabPixelsOfRoot;
  Shape        *pShape;
  Shape        **tabpShapesOfStack;
  int          i;
  int          iIndex;
  int          iSizeOfStack;

  /* 1) Memory allocation */
  tabPixelsOfRoot = the_shapes[0].pixels = new Point_plane[nrow*ncol];
  if(tabPixelsOfRoot == NULL)
    std::cerr << "allocate_pixels --> Allocation of pixels"<<std::endl;
  tabpShapesOfStack = new Shape*[nb_shapes];
  if(tabpShapesOfStack == NULL)
    std::cerr << "allocate_pixels --> Allocation of stack"<< std::endl;

  /* 2) Enumeration of the tree in preorder, using the stack */
  pShape = &the_shapes[0];
  iSizeOfStack = 0; 
  i = 0;
  while(1)
    if(pShape != NULL) {
      /* Write pixels of pShape */
      pShape->pixels = &tabPixelsOfRoot[i];
      iIndex = pShape - the_shapes;
      i += tabNbOfProperPixels[iIndex];
      tabpShapesOfStack[iSizeOfStack++] = pShape; /* Push */
      pShape = pShape->child;
    } else {
      if(iSizeOfStack == 0)
	break;
      pShape = tabpShapesOfStack[--iSizeOfStack]->next_sibling; /* Pop */
    }
  delete[] tabpShapesOfStack;

}


void 
Shapes::flst_pixels()
{
  Shape        **ppShape;
  int          *tabNbOfProperPixels; /* For each shape, its number of proper pixels */
  Point_plane  *pCurrentPoint;
  int          i;
  int          j; 
  int          iIndex;

  /* 1) Compute nb of proper pixels in each shape */
  tabNbOfProperPixels = new int[nb_shapes];
  if(tabNbOfProperPixels ==NULL)
    std::cerr << "Allocation of array error" << std::endl;
  compute_proper_pixels(tabNbOfProperPixels);

  /* 2) Allocate array for the root and make others sub-arrays */
  allocate_pixels(tabNbOfProperPixels);

  /* 3) Fill the array */
  ppShape = smallest_shape + ncol*nrow-1;
  for(i = nrow-1; i >= 0; i--)
    for(j = ncol-1; j >= 0; j--, ppShape--)
      {
	iIndex = (*ppShape) - the_shapes;
	pCurrentPoint = &(*ppShape)->pixels[--tabNbOfProperPixels[iIndex]];
	pCurrentPoint->x = j; pCurrentPoint->y = i;
      }
  delete[] tabNbOfProperPixels;
}



}  // namespace otb
#endif
