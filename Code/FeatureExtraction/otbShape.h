/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   04 avril 2006
  Version   :   
  Role      :   
  $Id$

=========================================================================*/
#ifndef __otbShape_h
#define __otbShape_h

/** \class Shape
 *   Internal Input/Output for the following FLST (Fast Level Set Transform)
 *   structures
 *     Point_plane
 *     Shape
 *     Shapes
 *
 * This file is part of the MegaWave2 system library
 *
 */

namespace otb
{

/** \class Point_plane
 * 
 *  Point in the plane
 */
class Point_plane
{
public:
  short x;  /* coordinates of the point */
  short y;  

  Point_plane()
       {
  	x = -1;
  	y = -1;
	}
  ~Point_plane() {};

protected:

private:

};

/** \class shape
 * 
 *  A shape  : a connected component of a level set, with filled holes
 */
class Shape
{
public:
  char     inferior_type; /* Indicates if it is extracted from a superior or inferior level set */
  float    value;         /* Limiting gray-level of the level set */
  char     open;          /* Indicates if the shape meets the border of the image */
  int      area;          /* Area of the shape = area of the cc of level set + areas of the holes */
  char     removed;       /* Indicates whether the shape exists or not */
  Point_plane *pixels;    /* The array of pixels contained in the shape */
  Shape    *parent;
  Shape    *next_sibling;
  Shape    *child;
  /* Data to include it in a tree. It has a parent (the smallest containing 
     shape), children (the largest contained shapes, whose first is pChild 
     and the others are its siblings), and siblings (the other children of 
     its parent) */
  Shape* mw_get_not_removed_shape(Shape *sh);
  Shape* mw_get_parent_shape(Shape *sh);
  Shape* mw_get_first_child_shape(Shape *sh);
  Shape* mw_get_next_sibling_shape(Shape *sh);

  void insert_children(Shape* pNewChildToInsert);

  Shape()
    {
    inferior_type = 0;
    value         = 0.0;
    open          = 0;
    area          = 0;
    removed       = 0;
    pixels        = NULL;
    parent        = NULL;
    next_sibling  = NULL;
    child         = NULL;
    }

  ~Shape()
    {
    if (pixels != NULL)  delete[] pixels;
    pixels = NULL;
    area   = 0;
    }


protected:

private:

};

/** \class Shapes
 * 
 * A set of shapes (complete representation of an image) 
 */

class Shapes
{
public:
  int nrow;               /* Number of rows (dy) of the image */
  int ncol;               /* Number of columns (dx) of the image */
  int interpolation;      /* Interpolation used for the level lines:
			     0=nearest neighbor, 1=bilinear */
  Shape *the_shapes;      /* Array of the shapes.  The root of the tree is at index 0 */
  int nb_shapes;          /* The number of shapes (the size of the array the_shapes) */

  /* Link between pixels and shapes */
  Shape **smallest_shape; /* An image giving for each pixel the smallest shape containing it */


//  Shapes* mw_new_shapes();
  void   mw_alloc_shapes( int nrow, int  ncol, float value);
  void   mw_change_shapes(int nrow,int ncol,float value);     
  Shape* mw_get_smallest_shape(int iX,int iY);

  Shapes()
    {
    the_shapes     = NULL;
    smallest_shape = NULL;
    nb_shapes      = 0;
    nrow           = 0;
    ncol           = 0;
    interpolation  = 0;
    }

  ~Shapes() 
    {
     if((the_shapes != NULL) && (nb_shapes > 0))
         delete[] (the_shapes[0].pixels);
     if (the_shapes != NULL) delete[] the_shapes;
     if (smallest_shape != NULL) delete[] smallest_shape;
    }

protected:

private:


};

} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbShape.txx"
#endif

  
#endif
