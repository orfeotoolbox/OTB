//*****************************************************************************
// FILE: ossimAffineTransform.cc
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
// DESCRIPTION: Contains implementation of class ossimAffineTransform
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimAffineTransform.cpp,v 1.5 2003/12/22 18:48:32 dburken Exp $

#include "ossimAffineTransform.h"
RTTI_DEF1(ossimAffineTransform, "ossimAffineTransform", ossim2dTo2dTransform);

#include "base/common/ossimKeywordNames.h"
#include "base/data_types/ossimKeywordlist.h"
#include "base/data_types/ossimMatrix3x3.h"

//*****************************************************************************
//  CONSTRUCTOR: 
//*****************************************************************************
ossimAffineTransform::ossimAffineTransform()
   :
      theForwardXform(3, 3),
      theInverseXform(3, 3),
      theAffineEnabledFlag(true)
{
   setIdentity();
}

//*****************************************************************************
//  METHOD: 
//*****************************************************************************
void ossimAffineTransform::forward(const ossimDpt& input,
                                   ossimDpt& output) const
{
   if(theAffineEnabledFlag)
   {
      output.x = theForwardXform[0][0]*input.x+
                 theForwardXform[0][1]*input.y+
                 theForwardXform[0][2];
      output.y = theForwardXform[1][0]*input.x+
                 theForwardXform[1][1]*input.y+
                 theForwardXform[1][2];
   }
   else
   {
      output = input;
   }
}

//*****************************************************************************
//  METHOD: 
//*****************************************************************************
void ossimAffineTransform::inverse(const ossimDpt& input,
                                   ossimDpt&       output) const
{
   if(theAffineEnabledFlag)
   {
      output.x = theInverseXform[0][0]*input.x+
                 theInverseXform[0][1]*input.y+
                 theInverseXform[0][2];
      output.y = theInverseXform[1][0]*input.x+
                 theInverseXform[1][1]*input.y+
                 theInverseXform[1][2];
   }
   else
   {
      output = input;
   }
}

//*****************************************************************************
//  METHOD: 
//*****************************************************************************
bool ossimAffineTransform::saveState(ossimKeywordlist& kwl,
                                     const char* prefix) const
{
   kwl.add(prefix,
           ossimKeywordNames::SCALE_X_KW,
           theScale.x,
           true);
   kwl.add(prefix,
           ossimKeywordNames::SCALE_Y_KW,
           theScale.y,
           true);
   kwl.add(prefix,
           ossimKeywordNames::TRANSLATION_X_KW,
           theTranslation.x,
           true);
   kwl.add(prefix,
           ossimKeywordNames::TRANSLATION_Y_KW,
           theTranslation.y,
           true);
   kwl.add(prefix,
           ossimKeywordNames::ROTATION_KW,
           theRotation,
           true);

   ossim2dTo2dTransform::saveState(kwl, prefix);
   
   return true;
}

//*****************************************************************************
//  METHOD: 
//*****************************************************************************
bool ossimAffineTransform::loadState(const ossimKeywordlist& kwl,
               const char* prefix)
{
   const char* buf;

   if (!ossim2dTo2dTransform::loadState(kwl, prefix))
      goto BAD_KEYWORD;
       
   buf= kwl.find(prefix, ossimKeywordNames::SCALE_X_KW);
   if (!buf) goto BAD_KEYWORD;
   theScale.x = atof(buf);

   buf= kwl.find(prefix, ossimKeywordNames::SCALE_Y_KW);
   if (!buf) goto BAD_KEYWORD;
   theScale.y = atof(buf);

   buf= kwl.find(prefix, ossimKeywordNames::TRANSLATION_X_KW);
   if (!buf) goto BAD_KEYWORD;
   theTranslation.x = atof(buf);

   buf= kwl.find(prefix, ossimKeywordNames::TRANSLATION_Y_KW);
   if (!buf) goto BAD_KEYWORD;
   theTranslation.y = atof(buf);

   buf= kwl.find(prefix, ossimKeywordNames::ROTATION_KW);
   if (!buf) goto BAD_KEYWORD;
   theRotation = atof(buf);

   computeMatrix();   
   return true;

 BAD_KEYWORD:
   return false;
}
   
//*****************************************************************************
//  METHOD: 
//*****************************************************************************
void ossimAffineTransform::setMatrix(double rotation,
                                     const ossimDpt& scale,
                                     const ossimDpt& translation)
{
   theScale       = scale;
   theRotation    = rotation;
   theTranslation = translation;
   
   computeMatrix();
}
   
//*****************************************************************************
//  METHOD: 
//*****************************************************************************
void ossimAffineTransform::setIdentity()
{
   theScale.x       = 1;
   theScale.y       = 1;
   theTranslation.x = 0;
   theTranslation.y = 0;
   theRotation      = 0.0;
   
   computeMatrix();
}

//*****************************************************************************
//  METHOD: 
//*****************************************************************************
void ossimAffineTransform::computeMatrix()
{
   theForwardXform =
      ossimMatrix3x3::createTranslationMatrix(theTranslation.x, 
                                              theTranslation.y)* 
      ossimMatrix3x3::createRotationZMatrix(theRotation)* 
      ossimMatrix3x3::createScaleMatrix(theScale.x,
                                        theScale.y,
                                        1.0);

   //***
   // The inverse transform is just the matrix inverse:
   //***
   theInverseXform = theForwardXform.i();
}


//*****************************************************************************
//  METHOD: 
//*****************************************************************************
std::ostream& ossimAffineTransform::print(std::ostream& os) const
{
   os << "ossimAffineTransform:"
      << "\n  theScale       = " << theScale
      << "\n  theTranslation = " << theTranslation
      << "\n  theRotation    = " << theRotation << endl;
   return os;
}
