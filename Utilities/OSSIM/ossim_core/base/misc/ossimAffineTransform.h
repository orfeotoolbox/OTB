//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public License 
// as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU Library General Public License
// along with this library. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimAffineTransform.h,v 1.4 2003/12/22 18:48:32 dburken Exp $

#ifndef ossimAffineTransform_HEADER
#define ossimAffineTransform_HEADER

#include "ossim2dTo2dTransform.h"
#include "matrix/newmat.h"

class OSSIMDLLEXPORT ossimAffineTransform : public ossim2dTo2dTransform
{
public:
   ossimAffineTransform();

   virtual void forward(const ossimDpt& input,
                        ossimDpt& output)const;
   virtual void inverse(const ossimDpt& input,
                        ossimDpt&       output)const;

   /*!
    * Used by the inverse iterator.  This will give it a good initial value
    * that is within the input transforms
    */
   virtual ossimDpt getOrigin()const
      {
         return ossimDpt(0,0);
      }
   
   bool saveState(ossimKeywordlist& kwl,
                   const char* prefix = 0)const;
   
   bool loadState(const ossimKeywordlist& kwl,
                  const char* prefix = 0);
   
   void setMatrix(double rotation,
                  const ossimDpt& scale,
                  const ossimDpt& translation);
   
   void setScale(const ossimDpt& scale)
      {
         theScale       = scale;
         computeMatrix();
      }
   
   void setRotation(double rotation)
      {
         theRotation = rotation;
         computeMatrix();
      }

   void setTranslation(const ossimDpt& translation)
      {
         theTranslation = translation;
         computeMatrix();
      }
   ossimDpt getScale()const
      {
         return theScale;
      }
   ossimDpt getTranslation()const
      {
         return theTranslation;
      }
   double getRotation()const
      {
         return theRotation;
      }
   void setIdentity();

   void setAffineEnabledFlag(bool flag)
      {
         theAffineEnabledFlag = flag;
      }
   virtual std::ostream& print(std::ostream& out) const;
      
protected:
   void computeMatrix();
   
   NEWMAT::Matrix theForwardXform;
   NEWMAT::Matrix theInverseXform;

   ossimDpt       theScale;
   double         theRotation;
   ossimDpt       theTranslation;
   bool           theAffineEnabledFlag;

TYPE_DATA      
};

#endif
