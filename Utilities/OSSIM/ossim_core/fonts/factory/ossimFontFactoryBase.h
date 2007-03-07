//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
//********************************************************************
// $Id: ossimFontFactoryBase.h,v 1.4 2005/07/15 19:45:48 dburken Exp $
#ifndef ossimFontFactoryBase_HEADER
#define ossimFontFactoryBase_HEADER
#include <base/data_types/ossimString.h>
#include <base/data_types/ossimFilename.h>
#include <base/common/ossimFontInformation.h>
class ossimFont;

class ossimFontFactoryBase
{
public:

   virtual ~ossimFontFactoryBase(){}
   
   /*!
    * Will find the best font for the passed in infromation.
    * If none is found a default font should be returned.
    */
   virtual ossimFont* createFont(const ossimFontInformation& information)const=0;

   /*!
    * Usually for trutype fonts but any font stored in a file it
    * will try to create a font from it.
    */
   virtual ossimFont* createFont(const ossimFilename& file)const=0;

   virtual void getFontInformation(std::vector<ossimFontInformation>& informationList)const=0;

};

#endif
