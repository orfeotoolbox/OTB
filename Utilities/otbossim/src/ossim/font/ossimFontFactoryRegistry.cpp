//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// 
//********************************************************************
// $Id: ossimFontFactoryRegistry.cpp 15766 2009-10-20 12:37:09Z gpotts $
#include <algorithm>
#include <ossim/font/ossimFontFactoryRegistry.h>
#include <ossim/font/ossimGdBitmapFont.h>

#if OSSIM_HAS_FREETYPE
#  include <ossim/font/ossimFreeTypeFontFactory.h>
#endif

#include <ossim/font/ossimGdSansBold.inc>

//ossimFontFactoryRegistry* ossimFontFactoryRegistry::theInstance = 0;

ossimFontFactoryRegistry::ossimFontFactoryRegistry()
:theDefaultFont(NULL)
{
   // theInstance = this;
#if OSSIM_HAS_FREETYPE
   registerFactory(ossimFreeTypeFontFactory::instance());
#endif
}

ossimFontFactoryRegistry* ossimFontFactoryRegistry::instance()
{
   static ossimFontFactoryRegistry sharedInstance;
   //theInstance = new ossimFontFactoryRegistry;
   
   return &sharedInstance;
}

bool ossimFontFactoryRegistry::registerFactory(ossimFontFactoryBase* factory)
{
   bool result = false;
   if(factory&&!findFactory(factory))
   {
      theFactoryList.push_back(factory);
      result = true;
   }
   
   return result;
}

void ossimFontFactoryRegistry::unregisterFactory(ossimFontFactoryBase* factory)
{
   std::vector<ossimFontFactoryBase*>::iterator iter =  std::find(theFactoryList.begin(),
                                                                  theFactoryList.end(),
                                                                  factory);
   if(iter != theFactoryList.end())
   {
      theFactoryList.erase(iter);
   }
}

bool ossimFontFactoryRegistry::findFactory(ossimFontFactoryBase* factory)const
{
   return (std::find(theFactoryList.begin(),
                     theFactoryList.end(),
                     factory)!=theFactoryList.end());
}


ossimFont* ossimFontFactoryRegistry::createFont(const ossimFontInformation& information)const
{
   ossimFont* result = (ossimFont*)NULL;
   int i = 0;
   
   for(i= 0; ( (i < (int)theFactoryList.size()) &&(!result)); ++i)
   {
      result = theFactoryList[i]->createFont(information);
   }
   
   return result;
}

ossimFont* ossimFontFactoryRegistry::createFont(const ossimFilename& file)const
{
   ossimFont* result = (ossimFont*)NULL;
   int i = 0;
   
   for(i= 0; ( (i < (int)theFactoryList.size()) &&(!result)); ++i)
   {
      result = theFactoryList[i]->createFont(file);
   }
   
   return result;
}

void ossimFontFactoryRegistry::getFontInformation(std::vector<ossimFontInformation>& informationList)const
{
   int i = 0;
   for(i= 0; i < (int)theFactoryList.size(); ++i)
   {
      theFactoryList[i]->getFontInformation(informationList);
   }
}

void ossimFontFactoryRegistry::getFontInformationFamilyName(std::vector<ossimFontInformation>& informationList,
                                                            const ossimString& familyName)const
{
   vector<ossimFontInformation> info;
   getFontInformation(info);
   
   ossimString right = familyName;
   right = right.upcase().trim();
   int i = 0;
   for(i = 0; i < (int)info.size(); ++i)
   {
      ossimString left  = info[i].theFamilyName;
      left  = left.upcase().trim();
      
      if(left == right)
      {
         informationList.push_back(info[i]);
      }
   }
}

void ossimFontFactoryRegistry::getFontInformationContainingFamilyName(std::vector<ossimFontInformation>& informationList,
                                                                      const ossimString& familyName)const
{
   vector<ossimFontInformation> info;
   getFontInformation(info);
   
   ossimString right = familyName;
   right = right.upcase().trim();
   int i = 0;
   for(i = 0; i < (int)info.size(); ++i)
   {
      ossimString left  = info[i].theFamilyName;
      
      left  = left.upcase().trim();
      if(left.contains(right))
      {
         informationList.push_back(info[i]);
      }
   }
}

ossimFont* ossimFontFactoryRegistry::getDefaultFont()const
{
   if(!theDefaultFont)
   {
      std::vector<ossimFontInformation> infoList;
      getFontInformationContainingFamilyName(infoList,
                                             "Times");
      if(infoList.size() < 1)
      {
         getFontInformationContainingFamilyName(infoList,
                                                "Roman");
      }
      if(infoList.size() < 1)
      {
         getFontInformationContainingFamilyName(infoList,
                                                "Sanz");
      }
      if(infoList.size() < 1)
      {
         getFontInformation(infoList);
      }
      if(infoList.size())
      {
         theDefaultFont = createFont(infoList[0]);
         theDefaultFont->setPixelSize(12, 12);
      }
      else
      {
         theDefaultFont = new  ossimGdBitmapFont("gd sans",
                                                 "bold",
                                                 ossimGdSansBold);
      }
   }
   return theDefaultFont.get();
}

ossimFontFactoryRegistry::ossimFontFactoryRegistry(const ossimFontFactoryRegistry& /* rhs */ )
{
}

void ossimFontFactoryRegistry::operator=(const ossimFontFactoryRegistry& /* rhs */ )
{
}
