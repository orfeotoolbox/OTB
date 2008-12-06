/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

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




/*-- Shapes --*/


/* Allocate the root of the shapes */

void
Shapes::mw_alloc_shapes( int inrow, int  incol, float value)
{
  int size,i;
  Shape *root;

  size = inrow*incol;
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
  root->parent        = root->next_sibling;
  root->next_sibling  = root->child;
  root->child         = NULL;

  nb_shapes = 1;
  nrow      = inrow;
  ncol      = incol;


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
Shapes::mw_change_shapes(int inrow,int incol,float value)
{
  int size,i;

  /* Deallocate the shapes but the structure itself */
  otbMsgDevMacro( << "mw_change_shapes () " );
  otbMsgDevMacro( << "  nb shapes  : " <<this->nb_shapes);
  otbMsgDevMacro( << "  the shapes : " <<the_shapes );
  if((the_shapes != NULL) && (nb_shapes > 0))
    delete[] the_shapes[0].pixels;
  if (the_shapes != NULL) delete[] the_shapes;
 if (smallest_shape != NULL) delete[] smallest_shape;

  mw_alloc_shapes(inrow, incol, value);
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



const int Shapes::EAST   = 0;
const int Shapes::NORTH  = 1;
const int Shapes::WEST   = 2;
const int Shapes::SOUTH  = 3;


void
Shapes::TURN_LEFT(int *dir)
{
 *dir = (*dir==NORTH ? WEST :(*dir==WEST ? SOUTH :(*dir==SOUTH ? EAST : NORTH)));
}

void
Shapes::TURN_RIGHT(int *dir)
{
 *dir = (*dir==NORTH ? EAST :(*dir==EAST ? SOUTH :(*dir==SOUTH ? WEST : NORTH)));
}


/* Is the point in the shape? */
char
Shapes::point_in_shape(int x,int y,Shape *pShape)
{
  char result;

  Shape *pShapePoint = smallest_shape[y*ncol+x];
  result = (pShape->pixels <= pShapePoint->pixels &&
	  pShapePoint->pixels < pShape->pixels+pShape->area);

  otbMsgDevMacro( << "PointInShape() -->"<<x<<" "<<y<<" " << pShape->value << " Result : "<< int(result));

  return (pShape->pixels <= pShapePoint->pixels &&
	  pShapePoint->pixels < pShape->pixels+pShape->area);
}

/* Find the dual point following pDualPoint as we follow the shape boundary */
void
Shapes::find_next_dual_point(Point_plane *pDualPoint,int *cDirection,Shape *pShape)
{
  char bLeftIn, bRightIn;
  otbMsgDevMacro( << "Shapes::find_next_dual_point()" );
  otbMsgDevMacro( << " pDualPoint : ( "<<pDualPoint->x << " , " << pDualPoint->y << " Direction :" << cDirection );

  switch(*cDirection) {
  case NORTH:
    bLeftIn  = point_in_shape(pDualPoint->x-1, pDualPoint->y-1, pShape);
    bRightIn = point_in_shape(pDualPoint->x,   pDualPoint->y-1, pShape);
    if(bLeftIn && ! bRightIn)
      -- pDualPoint->y;
    else if(! bLeftIn && (! bRightIn || pShape->inferior_type)) {
      -- pDualPoint->x;
      TURN_LEFT(cDirection);
    } else {
      ++ pDualPoint->x;
      TURN_RIGHT(cDirection);
    }
    break;
  case WEST:
    bLeftIn  = point_in_shape(pDualPoint->x-1, pDualPoint->y,   pShape);
    bRightIn = point_in_shape(pDualPoint->x-1, pDualPoint->y-1, pShape);
    if(bLeftIn && ! bRightIn)
      -- pDualPoint->x;
    else if(! bLeftIn && (! bRightIn || pShape->inferior_type)) {
      ++ pDualPoint->y;
      TURN_LEFT(cDirection);
    } else {
      -- pDualPoint->y;
      TURN_RIGHT(cDirection);
    }
    break;
  case SOUTH:
    bLeftIn  = point_in_shape(pDualPoint->x,   pDualPoint->y, pShape);
    bRightIn = point_in_shape(pDualPoint->x-1, pDualPoint->y, pShape);
    if(bLeftIn && ! bRightIn)
      ++ pDualPoint->y;
    else if(! bLeftIn && (! bRightIn || pShape->inferior_type)) {
      ++ pDualPoint->x;
      TURN_LEFT(cDirection);
    } else {
      -- pDualPoint->x;
      TURN_RIGHT(cDirection);
    }
    break;
  case EAST:
    bLeftIn  = point_in_shape(pDualPoint->x, pDualPoint->y-1, pShape);
    bRightIn = point_in_shape(pDualPoint->x, pDualPoint->y,   pShape);
    if(bLeftIn && ! bRightIn)
      ++ pDualPoint->x;
    else if(! bLeftIn && (! bRightIn || pShape->inferior_type)) {
      -- pDualPoint->y;
      TURN_LEFT(cDirection);
    } else {
      ++ pDualPoint->y;
      TURN_RIGHT(cDirection);
    }
    break;
  }
}

int
Shapes::find_closed_boundary(Shape *pShape,PathPointer pBoundary)
{
  short int                      x0;
  short int                      y0;
  Point_plane                    dualPoint;
  int                            cDirection;
  short int                      iWidth = (short int)ncol;
  short int                      iHeight = (short int)nrow;
  PathType::ContinuousIndexType  cindex;

  otbMsgDevMacro( << " find_closed_boundary 0" );
  /* 1) Find initial point, with NORTH direction */
  otbMsgDevMacro( << "pixel : " << pShape->pixels );

  dualPoint.x = pShape->pixels[0].x;
  dualPoint.y = pShape->pixels[0].y;
  cDirection  = NORTH;

  otbMsgDevMacro( << " find_closed_boundary 1" );

  do ++ dualPoint.x;
  while(point_in_shape(dualPoint.x, dualPoint.y, pShape));

  otbMsgDevMacro( << " find_closed_boundary 2" );

  /* 2) Follow the boundary */
  x0 = dualPoint.x;
  y0 = dualPoint.y;
  do {
      otbMsgDevMacro( << " find_closed_boundary 3" );
      cindex[0] = dualPoint.x;
      cindex[1] = dualPoint.y;

      pBoundary->AddVertex(cindex);

      find_next_dual_point(&dualPoint,&cDirection, pShape);
  } while(dualPoint.x != x0 || dualPoint.y != y0 || cDirection != NORTH);
  /* Close the boundary */

  cindex[0] = dualPoint.x;
  cindex[1] = dualPoint.y;

  pBoundary->AddVertex(cindex);

  otbMsgDevMacro( << " find_closed_boundary 4" );

}


/* Find an initial point (to follow the boundary) at the border of the image */
void
Shapes::initial_point_border(Point_plane *pDualPoint,int *cDirection,Shape *pShape)
{
  short int iWidth  = (short int)ncol;
  short int iHeight = (short int)nrow;
  short int x, y;

  otbMsgDevMacro( << "initial_point_border() --> "  );
  otbMsgDevMacro( << " pDualPoint x: " <<pDualPoint->x << " pDualPoint y: " <<pDualPoint->y << " Direction   :" << int(*cDirection)  );

  /* Right border */
  *cDirection = WEST;
  otbMsgDevMacro( << " Direction  (WEST) :" << int(*cDirection)  );
  x = iWidth-1;
  y = 0;
  if(point_in_shape(x, y++, pShape))
    while(y < iHeight && point_in_shape(x, y++, pShape));
  while(y < iHeight && ! point_in_shape(x, y, pShape))
    ++ y;
  if(y < iHeight) {
    pDualPoint->x = iWidth;
    pDualPoint->y = y;
    return;
  }
  /* Top border */
  *cDirection = SOUTH;
  x = 0;
  y = 0;
  if(point_in_shape(x++, y, pShape))
    while(x < iWidth && point_in_shape(x++, y, pShape));
  while(x < iWidth && ! point_in_shape(x, y, pShape))
    ++ x;
  if(x < iWidth) {
    pDualPoint->x = x;
    pDualPoint->y = 0;
    return;
  }
  /* Left border */
  *cDirection = EAST;
  x = 0;
  y = iHeight-1;
  if(point_in_shape(x, y--, pShape))
    while(y >= 0 && point_in_shape(x, y--, pShape));
  while(y >= 0 && ! point_in_shape(x, y, pShape))
    -- y;
  if(y >= 0) {
    pDualPoint->x = 0;
    pDualPoint->y = y+1;
    return;
  }
  /* Bottom border */
  *cDirection = NORTH;
  x = iWidth-1;
  y = iHeight-1;
  if(point_in_shape(x--, y, pShape))
    while(x >= 0 && point_in_shape(x--, y, pShape));
  while(x >= 0 && ! point_in_shape(x, y, pShape))
    -- x;
  if(x >= 0) {
    pDualPoint->x = x+1;
    pDualPoint->y = iHeight;
    return;
  }
  std::cerr << "initial_point_border --> Coherency?"<<std::endl;
}

/* Find an open boundary */
void
Shapes::find_open_boundary(Shape *pShape,PathPointer pBoundary)
{
  Point_plane                    dualPoint;
  int                            cDirection;
  Point_plane                    *pPoint;
  short int                      iWidth  = (short int)ncol;
  short int                      iHeight = (short int)nrow;
  PathType::ContinuousIndexType  cindex;

  otbMsgDevMacro( << "Shapes::find_open_boundary()" );
  initial_point_border(&dualPoint,&cDirection, pShape);
  do {
      cindex[0] = dualPoint.x;
      cindex[1] = dualPoint.y;

      pBoundary->AddVertex(cindex);

    otbMsgDevMacro( << "Shapes::find_open_boundary() DO WHILE" );
    find_next_dual_point(&dualPoint, &cDirection,pShape);
  } while(0 < dualPoint.x && dualPoint.x < iWidth &&
	  0 < dualPoint.y && dualPoint.y < iHeight);

 /* We store the exit */

  cindex[0] = dualPoint.x;
  cindex[1] = dualPoint.y;

  pBoundary->AddVertex(cindex);

  otbMsgDevMacro( << "Shapes::find_open_boundary() END" );

}


/* Find boundary of the shape */
Shapes::PathPointer
Shapes::flst_shape_boundary(Shape *pShape)
{
  otbMsgDevMacro( << " FLST Shape Boundary " );
  otbMsgDevMacro( << "Nb ncol : " << ncol );

  PathPointer  pBoundary = PathType::New();

  otbMsgDevMacro( << " FLST Shape Boundary ....New" );

  if(the_shapes[0].pixels == NULL)
    flst_pixels();
  otbMsgDevMacro( << " FLST Shape Boundary ....Pixel" );

  otbMsgDevMacro( << " FLST Shape Boundary ....pShape->open " <<int(pShape->open) );

  pBoundary->Initialize();

  if(pShape->open)
    find_open_boundary( pShape, pBoundary);
  else
    find_closed_boundary(pShape, pBoundary);

  otbMsgDevMacro( << " FLST Shpae Boundary ....Find OK" );

  return(pBoundary);
}



}  // namespace otb
#endif
