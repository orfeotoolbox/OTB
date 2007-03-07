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
// 
//********************************************************************
// $Id: ossimFreeTypeFontFactory.h,v 1.1 2002/04/30 18:50:55 gpotts Exp $
#ifndef ossimFreeTypeFontFactory_HEADER
#define ossimFreeTypeFontFactory_HEADER
#include "fonts/factory/ossimFontFactoryBase.h"

class ossimFreeTypeFontInformation
{
public:
   ossimFreeTypeFontInformation(const ossimFilename& file,
                                const ossimFontInformation& info)
      :theFilename(file),
       theFontInformation(info)
      {}

   ossimFilename        theFilename;
   ossimFontInformation theFontInformation;
};

class ossimFreeTypeFontFactory : public ossimFontFactoryBase
{
public:
   virtual ~ossimFreeTypeFontFactory();
   static ossimFreeTypeFontFactory* instance();
   virtual ossimFont* createFont(const ossimFontInformation& information)const;
   virtual ossimFont* createFont(const ossimFilename& file)const;
   virtual void getFontInformation(std::vector<ossimFontInformation>& informationList)const;
  
   bool addFile(const ossimFilename& file);
   
protected:
   ossimFreeTypeFontFactory();

   static ossimFreeTypeFontFactory* theInstance;
   std::vector<ossimFreeTypeFontInformation> theFontInformationList;

   void initializeDefaults();
private:
   ossimFreeTypeFontFactory(const ossimFreeTypeFontFactory& rhs){}
   void operator =(const ossimFreeTypeFontFactory& rhs){}
   
};

#endif
