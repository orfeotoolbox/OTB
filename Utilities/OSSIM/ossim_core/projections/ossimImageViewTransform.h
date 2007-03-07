//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
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
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:  This class hides the image view transformation process.
//               We should be able to open up ay image within the system
//               and be able to map it to the view space.  Classes need to
//               derive and override the methods within this class.  This will
//               mainly be used in the resampling process.
//
//*******************************************************************
//  $Id: ossimImageViewTransform.h,v 1.16 2005/09/28 19:23:17 gpotts Exp $
#ifndef ossimImageViewTransform_HEADER
#define ossimImageViewTransform_HEADER
#include <iostream>
using namespace std;
#include "base/common/ossimViewInterface.h"
#include "base/common/ossimObject.h"
#include "base/data_types/ossimDpt.h"
#include "base/data_types/ossimDrect.h"
#include "base/misc/ossim2dTo2dTransform.h"

class OSSIMDLLEXPORT ossimImageViewTransform : public ossim2dTo2dTransform,
					       public ossimViewInterface
{
public:
   friend OSSIMDLLEXPORT ostream& operator<<(ostream& out,
					     const ossimImageViewTransform& data);
   
   ossimImageViewTransform();

   virtual void forward(const ossimDpt& input, ossimDpt& output) const;
  
  /*!
   * derived classes will specify if the imageViewTransform is an
   * identity.  
   */
   virtual bool isIdentity()const=0;


   virtual bool isValid()const=0;
   virtual ossimDpt getInputMetersPerPixel()const=0;
   virtual ossimDpt getOutputMetersPerPixel()const=0;
   /*!
   * If it needs to it will use the information passed in to get the
   * scae factor by transforming the line point.x to point.x+deltaXY and 
   * point.y to point.y + deltaXY.y.  It will comput new delta's and 
   * set the ratio of deltaXY/newDeltaXY to the result scale.  
   *
   * Derived classes can overide this default implementation.  Note:
   * if the scale changes are only subpixel then it will return a scale
   * of one.  We have to see how this works before we actually keep this
   * implementation.
   */
  virtual void getImageToViewScale(ossimDpt& resultScale,
				   const ossimDpt& imagePoint,
				   const ossimDpt& deltaImagePointXY)const;

  /*!
   * If it needs to it will use the information passed in to get the
   * scae factor by transforming the line point.x to point.x+deltaXY and 
   * point.y to point.y + deltaXY.y.  It will comput new delta's and 
   * set the ratio of deltaXY/newDeltaXY to the result scale.  
   *
   * Derived classes can overide this default implementation.  Note:
   * if the scale changes are only subpixel then it will return a scale
   * of one.  We have to see how this works before we actually keep this
   * implementation.
   */
  virtual void getViewToImageScale(ossimDpt& resultScale,
				   const ossimDpt& imagePoint,
				   const ossimDpt& deltaImagePointXY)const;
  
  virtual void imageToView(const ossimDpt& imagePoint,
                           ossimDpt&       viewPoint)const;
  
  virtual void viewToImage(const ossimDpt& viewPoint,
                           ossimDpt&       imagePoint)const;
  
  virtual std::ostream& print(std::ostream& out) const;
  
  ossimDpt imageToView(const ossimDpt& imagePoint)const;
  
  ossimDpt viewToImage(const ossimDpt& viewPoint)const;
  
  /*!
   * This should compute output/input.
   */
  virtual void getScaleChangeImageToView(ossimDpt& result,
                                         const ossimDrect& imageRect);
  /*!
   * This should compute output/input.
   */
  virtual void getScaleChangeViewToImage(ossimDpt& result,
                                         const ossimDrect& viewRect);
  
  virtual void getRoundTripErrorView(ossimDpt& result,
				     const ossimDpt& viewPt)const;
  
  virtual ossimDpt getRoundTripErrorView(const ossimDpt& viewPt)const;
  
  virtual void getRoundTripErrorImage(ossimDpt& result,
                                      const ossimDpt& imagePt)const;
  
  virtual ossimDpt getRoundTripErrorImage(const ossimDpt& imagePt)const;

  virtual ossimDrect getImageToViewBounds(const ossimDrect& imageRect)const;
  
  virtual bool loadState(const ossimKeywordlist& kwl,
                         const char* prefix =0);
  
  virtual bool saveState(ossimKeywordlist& kwl,
                         const char* prefix = 0)const;

TYPE_DATA
};

#endif
