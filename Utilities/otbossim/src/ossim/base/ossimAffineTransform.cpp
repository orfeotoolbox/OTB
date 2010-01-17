//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// AUTHOR: Oscar Kramer (okramer@imagelinks.com)
//
// DESCRIPTION: Contains implementation of class ossimAffineTransform
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimAffineTransform.cpp 15766 2009-10-20 12:37:09Z gpotts $

#include <cstdlib>
#include <ossim/base/ossimAffineTransform.h>
RTTI_DEF1(ossimAffineTransform, "ossimAffineTransform", ossim2dTo2dTransform);

#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimMatrix3x3.h>
#include <ossim/base/ossimString.h>
//*****************************************************************************
//  CONSTRUCTOR: 
//*****************************************************************************
ossimAffineTransform::ossimAffineTransform()
   :
      ossim2dTo2dTransform(),
      theScale(1.0, 1.0),
      theRotation(0.0),
      theTranslation(0.0, 0.0),
      theAffineEnabledFlag(true),      
      theForwardXform(3, 3),
      theInverseXform(3, 3)
{
   setIdentity();
}

ossimAffineTransform::ossimAffineTransform(const ossimAffineTransform& src)
   :
      ossim2dTo2dTransform(src),
      theScale(src.theScale),
      theRotation(src.theRotation),
      theTranslation(src.theTranslation),
      theAffineEnabledFlag(src.theAffineEnabledFlag),
      theForwardXform(src.theForwardXform),
      theInverseXform(src.theInverseXform)
{
}

const ossimAffineTransform& ossimAffineTransform::operator=(
   const ossimAffineTransform& rhs)
{
   if (this != &rhs)
   {
      ossim2dTo2dTransform::operator=(rhs);
      
      theScale             = rhs.theScale;
      theRotation          = rhs.theRotation;
      theTranslation       = rhs.theTranslation;
      theAffineEnabledFlag = rhs.theAffineEnabledFlag;
      theForwardXform      = rhs.theForwardXform;
      theInverseXform      = rhs.theInverseXform;
   }
   return *this;
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
   theScale.x = std::atof(buf);

   buf= kwl.find(prefix, ossimKeywordNames::SCALE_Y_KW);
   if (!buf) goto BAD_KEYWORD;
   theScale.y = std::atof(buf);

   buf= kwl.find(prefix, ossimKeywordNames::TRANSLATION_X_KW);
   if (!buf) goto BAD_KEYWORD;
   theTranslation.x = std::atof(buf);

   buf= kwl.find(prefix, ossimKeywordNames::TRANSLATION_Y_KW);
   if (!buf) goto BAD_KEYWORD;
   theTranslation.y = std::atof(buf);

   buf= kwl.find(prefix, ossimKeywordNames::ROTATION_KW);
   if (!buf) goto BAD_KEYWORD;
   theRotation = std::atof(buf);

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
      << "\n  theRotation    = " << theRotation << std::endl;
   return os;
}
