//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: 
// 
//********************************************************************
// $Id: ossimFontFactoryRegistry.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimFontFactoryRegistry_HEADER
#define ossimFontFactoryRegistry_HEADER
#include <vector>
#include <ossim/base/ossimFontInformation.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/font/ossimFontFactoryBase.h>
#include <ossim/font/ossimFont.h>
#include <ossim/base/ossimRefPtr.h>


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
   mutable ossimRefPtr<ossimFont> theDefaultFont;

   //static ossimFontFactoryRegistry* theInstance;

   std::vector<ossimFontFactoryBase*> theFactoryList;

   ossimFontFactoryRegistry();
   
private:
   ossimFontFactoryRegistry(const ossimFontFactoryRegistry& rhs);
   void operator =(const ossimFontFactoryRegistry& rhs);
};

#endif
