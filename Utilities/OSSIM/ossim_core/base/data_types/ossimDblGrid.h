//*****************************************************************************
// FILE: ossimDblGrid.h
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  See top level LICENSE.txt file.
//
// AUTHOR:  Oscar Kramer
//
// DESCRIPTION:
//   Contains declaration of class ossimDblGrid. This object class maintains
//   a regular grid of floating point (double) values. Access methods to the
//   grid include interpolation between grid nodes. Capability is included
//   to address the grid in an arbitrary, floating-point u, v coordinate
//   system which is related to the integer grid x, y system by scale and
//   offset:
//
//          u = Sx*(x + Xo),   v = Sy*(y + Y0)
//
//   where Xo, Yo is the origin of the U/V cood system. Sx, Sy are the scale
//   relations. For example, if the grid is being used to hold a grid of
//   elevation posts, then the origin will be the SW corner post ground point,
//   and the scale will be derived from the post spacing. Both origin and
//   spacing are specified to the constructor.
//
//*****************************************************************************
//  $Id: ossimDblGrid.h,v 1.13 2005/12/16 14:36:44 dburken Exp $

#ifndef ossimDblGrid_HEADER
#define ossimDblGrid_HEADER

#include "base/data_types/ossimDpt.h"
#include "base/data_types/ossimIpt.h"
#include "base/common/ossimCommon.h"

class OSSIMDLLEXPORT ossimDrect;

/*!****************************************************************************
 *
 * CLASS:  ossimDblGrid
 *
 *****************************************************************************/
class OSSIMDLLEXPORT ossimDblGrid 
{
public:
   static const double DEFAULT_NULL_VALUE;

   /*!
    * Default Constructor.
    */
   ossimDblGrid ();

   /*!
    * Copy Constructor.
    */
   ossimDblGrid (const ossimDblGrid& source_to_copy);

   /*!
    * Constructs given a U/V origin and spacing, and a grid size.
    */
   ossimDblGrid (const ossimIpt&  size, 
                 const ossimDpt&  origin,
                 const ossimDpt&  spacing,
                 double null_value = DEFAULT_NULL_VALUE);

   /*!
    * Constructs given a rectangle in U/V space and grid spacing. The origin is
    * taken as the upper-left corner of the rectangle. The lower-rightmost grid
    * point may not coincide with the rectangles LR corner if the side is not
    * evenly divisible by the corresponding spacing.
    */
   ossimDblGrid (const ossimDrect&  uv_rect, 
                 const ossimDpt&    spacing,
                 double null_value = DEFAULT_NULL_VALUE);

   ~ossimDblGrid ();

   /*!
    * When constructed with default, permits initialization afterward.
    */
   void         initialize(const ossimIpt&  size, 
                           const ossimDpt&  origin,
                           const ossimDpt&  spacing,
                           double null_value = DEFAULT_NULL_VALUE);
   void         initialize(const ossimDrect&  uv_rect, 
                           const ossimDpt&    spacing,
                           double null_value = DEFAULT_NULL_VALUE);
  void deallocate();
   /*!
    * Fills the current grid with the value specified.
    */
   void         fill(double fill_value);

   /*!
    * Fills the current grid with null values.
    */
   void         clear() { fill(theNullValue); }
   
   /*!
    * Access individual node value by node index in the X/Y (grid) coord system.
    */
   void         setNode (const ossimIpt& p,
                         const double& value)   { setNode(p.x, p.y, value); }
   void         setNode (int x, int y, const double&  value);

   double       getNode (const ossimIpt& p) const { return getNode(p.x, p.y); }
   double       getNode (int x, int y) const;

   /*!
    * Sets the node nearest the U,V point specified to the value given. This is
    * different from setNode() in that the UV coordinate system is used to
    * address a node instead of an XY grid point.
    */
   void         setNearestNode(const ossimDpt& uv_point,
                               const double&   value);
   
   /*!
    * This method performs a resampling of the defined grid nodes in order to
    * compute interpolated values for those uninitialized nodes. This is
    * necessary when only a subset of nodes are available for initializing the
    * grid.
    */
   void interpolateNullValuedNodes(const double& decay_rate=10.0);

   /*!
    * Passes the grid data through a convolution filter given the kernel array.
    * The grid must not contain any NULL nodes as these are treated as valid
    * quantities. The kernel sizes should be odd numbers. To avoid shrinking
    * the grid by the kernel radius, the edge nodes outside of the filter's
    * reach are set equal to the nearest filtered node. X is the contiguous
    * axis in the kernel array, i.e., kernel(x,y) = kernel[y*size_x + x].
    */
   void filter(int size_x, int size_y, double* kernel);
   
   /*!
    * Interpolate between grid nodes given arbitrary u/v coord system:
    */
   enum InterpType
   {
      BILINEAR
   };
   void  setInterpolationType (InterpType interp);

   /*!
    * Access an interpolated value in the U/V (output) coordinate system.
    */
   double operator() (const ossimDpt& p) const {return (*this)(p.u,p.v);}
   double value      (const ossimDpt& p) const {return (*this)(p.u,p.v);}

   double operator() (const double& u, const double& v) const;
   double value (const double& u, const double& v) const {return (*this)(u,v);}

   /*!
    * operator for initializing this grid with another.
    */
   const ossimDblGrid& operator = (const ossimDblGrid& grid);
   
   /*!
    *Statistics methods:
    */
   double minValue()   const { return theMinValue; }
   double maxValue()   const { return theMaxValue; }
   double nullValue()  const { return theNullValue; }
   double meanValue();
   double meanStdDev();

   void setMinValue(double value) {theMinValue = value;}
   void setMaxValue(double value) {theMaxValue = value;}
   void setNullValue(double value) {theNullValue = value;}
   /*!
    * Other member access methods:
    */
   const ossimIpt& size()    const { return theSize; }
   const ossimDpt& origin()  const { return theOrigin; }
   const ossimDpt& spacing() const { return theSpacing; }
   unsigned long   getSizeInBytes() const
      { return theSize.x*theSize.y*sizeof(double); }
   
   /*!
    * Returns true if double point lies within world space coverage:
    */
   bool  isInside(const ossimDpt& p) const {return isInside(p.u, p.v);}
   bool  isInside(const double& u, const double& v) const;

   /*!
    * Loads and saves the grid from/to a stream. Returns TRUE if successful.
    */
   bool  save(std::ostream& os, const char* descr) const;
   bool  load(std::istream& is);
      
   
   friend std::ostream& operator << (std::ostream& os,
                                     const ossimDblGrid& grid);
   
private:
   
   void  computeMean();
   int   index(int x, int y) const { return y*theSize.x + x; }
   
   double*      theGridData;
   ossimIpt     theSize;
   ossimDpt     theOrigin;
   ossimDpt     theSpacing;
   double       theMinValue;
   double       theMaxValue;
   double       theNullValue;
   double       theMeanValue;
   double       theDeviation;
   bool         theMeanIsComputed;
   
};

#endif

