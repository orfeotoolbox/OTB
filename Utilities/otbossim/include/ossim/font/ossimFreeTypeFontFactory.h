//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// 
//********************************************************************
// $Id: ossimFreeTypeFontFactory.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimFreeTypeFontFactory_HEADER
#define ossimFreeTypeFontFactory_HEADER
#include <ossim/font/ossimFontFactoryBase.h>

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
