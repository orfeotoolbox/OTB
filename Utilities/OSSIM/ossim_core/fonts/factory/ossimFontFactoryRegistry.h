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
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: 
// 
//********************************************************************
// $Id: ossimFontFactoryRegistry.h,v 1.9 2004/05/19 18:35:57 dburken Exp $
#ifndef ossimFontFactoryRegistry_HEADER
#define ossimFontFactoryRegistry_HEADER
#include <vector>
#include "base/common/ossimFontInformation.h"
#include "base/data_types/ossimFilename.h"
#include "fonts/factory/ossimFontFactoryBase.h"


class OSSIMDLLEXPORT ossimFontFactoryRegistry
{
public:
   static ossimFontFactoryRegistry* instance();

   bool registerFactory(ossimFontFactoryBase* factory);
   void unregisterFactory(ossimFontFactoryBase* factory);
   bool findFactory(ossimFontFactoryBase* factory)const;
   
   ossimFont* createFont(const ossimFontInformation& information)const;
   ossimFont* createFont(const ossimFilename& file)const;

   /*!
    * Returns all font information.
    */
   void getFontInformation(std::vector<ossimFontInformation>& informationList)const;

   /*!
    * Matches the entire string.  Does a case insensitive match.
    */ 
   void getFontInformationFamilyName(std::vector<ossimFontInformation>& informationList,
                                     const ossimString& familyName)const;

   /*!
    * Matches the sub-string.  Does a case insensitive match.
    */ 
   void getFontInformationContainingFamilyName(std::vector<ossimFontInformation>& informationList,
                                               const ossimString& familyName)const;

   /*!
    * This font should not be deleted.  It returns the default font.  If you
    * want to make a copy then just call the dup method on
    * font.
    */
   ossimFont* getDefaultFont()const;
   
protected:
   mutable ossimFont* theDefaultFont;

   static ossimFontFactoryRegistry* theInstance;

   std::vector<ossimFontFactoryBase*> theFactoryList;

   ossimFontFactoryRegistry();
   
private:
   ossimFontFactoryRegistry(const ossimFontFactoryRegistry& rhs);
   void operator =(const ossimFontFactoryRegistry& rhs);
};

#endif
