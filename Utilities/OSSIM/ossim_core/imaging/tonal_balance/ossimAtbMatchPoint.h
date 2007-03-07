//*****************************************************************************
// FILE: ossimAtbMatchPoint.h
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// AUTHOR: Oscar Kramer (okramer@imagelinks.com)
//
// DESCRIPTION: Contains declaration of class ossimAtbMatchPoint. This class
//    is used by the automated tonal balancing code for bookkeeping and
//    statistics associated with match-points. Match points are points shared in
//    common between two or more images being adjusted. 
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimAtbMatchPoint.h,v 1.4 2001/12/14 20:16:25 okramer Exp $

#ifndef ossimAtbMatchPoint_HEADER
#define ossimAtbMatchPoint_HEADER

#include "base/data_types/ossimDpt.h"
#include <vector>
using namespace std;

class ossimAtbPointSource;
class ossimGridRemapEngine;
class ossimGridRemapSource;

/*!****************************************************************************
 *
 * CLASS:  ossimAtbMatchPoint
 *
 *****************************************************************************/
class ossimAtbMatchPoint
{
public:
   ossimAtbMatchPoint()
      : theGridRemapEngine(0) {}
   
   ossimAtbMatchPoint(const ossimDpt& view_point,
                      ossimGridRemapEngine* engine)
      : theViewPoint(view_point), theGridRemapEngine(engine) {}

   ~ossimAtbMatchPoint();
   
   /*!
    * Returns the point in view coordinates associated with this object.
    */
   const ossimDpt& viewPoint() const { return theViewPoint; }

   /*!
    * Adds a new contribution to the sample set.
    */
   void addImage(ossimGridRemapSource* image_source);

   /*!
    * The target parameter value computed given all contributions. Returns true
    * if a valid quantity is returned.
    */
   bool assignRemapValues();

   /*!
    * Hook to set the size of the kernel used by all point sources in computing
    * their mean pixel value. The kernels will be resized to NxN.
    */
   void setKernelSize(int side_size);

   /*!
    * Sets the ATB remap engine reference owned by the controller. This engine
    * is the only object that understands how to compute remap parameters from
    * pixel data. This permits easily modifying the ATB algorithm without
    * the support classes such as this one.
    */
   void setGridRemapEngine(ossimGridRemapEngine* engine);

   /*!
    * Dumps the contents of the object to the stream in human readable format.
    */
//   void print(ostream& os) const;

//   friend ostream& operator << (ostream& os);

private:
   ossimDpt                     theViewPoint;
   vector<ossimAtbPointSource*> thePointSourceList; 
   ossimGridRemapEngine*        theGridRemapEngine;
};

#endif
