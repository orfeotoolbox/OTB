//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description:
//
//*************************************************************************
// $Id: ossimCmyVector.cpp 9966 2006-11-29 02:01:07Z gpotts $
#include <ossim/base/ossimCmyVector.h>
#include <ossim/base/ossimRgbVector.h>

ossimCmyVector::ossimCmyVector(const ossimRgbVector& rgb)
{
   theC = 255 - rgb.getR();
   theM = 255 - rgb.getG();
   theY = 255 - rgb.getB();
}

const ossimCmyVector& ossimCmyVector::operator = (const ossimRgbVector& rgb)
{
   theC = 255 - rgb.getR();
   theM = 255 - rgb.getG();
   theY = 255 - rgb.getB();
   
   return *this;
}
