//*****************************************************************************
// FILE: ossimAtbPointSource.h
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
// DESCRIPTION: Contains declaration of class ossimAtbPointSource.
//   This object provides the statistics associated with a given point on a
//   given image corresponding to a matchpoint. A matchpoint contains a
//   collection of these point sources, one for each contributing image.
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimAtbPointSource.h,v 1.2 2001/12/14 20:10:39 okramer Exp $

#ifndef ossimAtbPointSource_HEADER
#define ossimAtbPointSource_HEADER

#include "base/data_types/ossimDpt.h"

class ossimImageSource;
class ossimImageData;
class ossimGridRemapSource;
class ossimGridRemapEngine;

/*!****************************************************************************
 *
 * CLASS: ossimAtbPointSource 
 *
 *****************************************************************************/
class ossimAtbPointSource
{
public:
   ossimAtbPointSource();
   ossimAtbPointSource(ossimGridRemapSource* remap_source,
                       const ossimDpt&       view_point);

   ~ossimAtbPointSource();

   /*!
    * Sets the pointer of the source of pixels used to compute the stats.
    */
   void setRemapSource(ossimGridRemapSource* image_source);

   /*!
    * Returns the reference to the remapper feeding this source.
    */
   ossimGridRemapSource* getRemapSource() { return theRemapSource; }

   /*!
    * Sets the view coordinates corresponding to this point.
    */
   void setViewPoint(const ossimDpt& view_point);

   /*!
    * Returns the view point corresponding to this point source.
    */
   const ossimDpt& getViewPoint() const { return theViewPoint; }

   /*!
    * Returns the computed value vector corresponding to the region about the
    * view point. The ATB Remap engine performs the actual computation, since
    * the definition of "value" is algorithm dependent (the value may be in a
    * different color space).
    */
   void getSourceValue(void* value);

   /*!
    * Method to set the kernel size used in computing statistics. The kernel
    * will be resized to NxN.
    */
   void setKernelSize(int side_size);

   /*!
    * Sets the ATB remap engine reference owned by the controller. This engine
    * is the only object that understands how to compute remap parameters from
    * pixel data. This permits easily modifying the ATB algorithm without
    * the support classes such as this one.
    */
   void setGridRemapEngine(ossimGridRemapEngine* engine)
      { theGridRemapEngine = engine; }
   
private:
   ossimGridRemapSource* theRemapSource;
   ossimDpt              theViewPoint;
   ossimGridRemapEngine* theGridRemapEngine;
   int                   theKernelSize;
   bool                  theViewPointIsValid;
};

#endif
