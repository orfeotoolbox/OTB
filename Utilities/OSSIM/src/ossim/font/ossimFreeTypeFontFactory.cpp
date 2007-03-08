//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
//********************************************************************
// $Id: ossimFreeTypeFontFactory.cpp 9099 2006-06-13 21:21:10Z dburken $

#include <ossim/ossimConfig.h> /* To pick up OSSIM_HAS_FREETYPE. */

#if OSSIM_HAS_FREETYPE
#  include <ossim/font/ossimFreeTypeFontFactory.h>
#  include <ossim/font/ossimFreeTypeFont.h>
#  include <ossim/base/ossimPreferences.h>
#  include <ossim/base/ossimDirectory.h>
#  include <ossim/base/ossimNotifyContext.h>

ossimFreeTypeFontFactory* ossimFreeTypeFontFactory::theInstance=0;

ossimFreeTypeFontFactory::ossimFreeTypeFontFactory()
{
   theInstance = this;
}

ossimFreeTypeFontFactory::~ossimFreeTypeFontFactory()
{
   theInstance = (ossimFreeTypeFontFactory*)NULL;
}

ossimFreeTypeFontFactory* ossimFreeTypeFontFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimFreeTypeFontFactory;
      theInstance->initializeDefaults();
   }

   return theInstance;
}


ossimFont* ossimFreeTypeFontFactory::createFont(const ossimFontInformation& information)const

{
   ossimFont* result = (ossimFont*)NULL;
   
   ossimString rightFamilyName = information.theFamilyName;
   ossimString rightStyleName  = information.theStyleName;
   rightFamilyName = rightFamilyName.upcase().trim();
   rightStyleName  = rightStyleName.upcase().trim();
   int i = 0;
   
   for(i = 0; ( (i < (int)theFontInformationList.size())&&(!result));++i)
   {
      ossimString leftFamilyName = theFontInformationList[i].theFontInformation.theFamilyName;
      ossimString leftStyleName = theFontInformationList[i].theFontInformation.theStyleName;

      leftFamilyName = leftFamilyName.upcase().trim();
      leftStyleName  = leftStyleName.upcase().trim();
      
      if( (leftFamilyName == rightFamilyName)&&
          (leftStyleName == rightStyleName))
      {
         if(theFontInformationList[i].theFontInformation.theFixedFlag)
         {
            if(theFontInformationList[i].theFontInformation.thePointSize ==
               information.thePointSize)
            {
               result = new ossimFreeTypeFont(theFontInformationList[i].theFilename);
               if(result)
               {
                  if(result->getErrorStatus())
                  {
                     delete result;
                     result = NULL;
                     ossimNotify(ossimNotifyLevel_WARN) <<"WARNING ossimFreeTypeFontFactory::createFont:  unable to open font file "
                                                        << theFontInformationList[i].theFilename << std::endl;
                  }
               }
            }
         }
         else
         {
            result = new ossimFreeTypeFont(theFontInformationList[i].theFilename);
            if(result)
            {
               if(result->getErrorStatus())
               {
                  delete result;
                  result = NULL;
                  ossimNotify(ossimNotifyLevel_WARN) <<  "WARNING ossimFreeTypeFontFactory::createFont: unable to open font file "
                                                     << theFontInformationList[i].theFilename << std::endl;
               }
               else
               {
                  if(information.thePointSize.x&&
                     information.thePointSize.y)
                  {
                     result->setPixelSize(information.thePointSize.x,
                                          information.thePointSize.y);
                  }
               }
            }
         }
      }         
   }

   if(result)
   {
      result->setScale(information.theScale.x,
                       information.theScale.y);
      result->setScale(information.theScale.x,
                       information.theScale.y);
      result->setHorizontalVerticalShear(information.theShear.x,
                                         information.theShear.y);
      result->setRotation(information.theRotation);
   }
   
   return result;
}

ossimFont* ossimFreeTypeFontFactory::createFont(const ossimFilename& file)const
{
   ossimFont* result = new ossimFreeTypeFont(file);

   if(result->getErrorStatus())
   {
      delete result;
      result = (ossimFont*)NULL;
   }
   
   return result;
}

void ossimFreeTypeFontFactory::getFontInformation(std::vector<ossimFontInformation>& informationList)const
{
   int i = 0;
   for(i = 0; i < (int)theFontInformationList.size();++i)
   {
      informationList.push_back(theFontInformationList[i].theFontInformation);
   }
}

bool ossimFreeTypeFontFactory::addFile(const ossimFilename& file)
{
   ossimFreeTypeFont* font = new ossimFreeTypeFont(file);
   std::vector<ossimFontInformation> fontInfoList;
   
   bool result = false;
   int i = 0;
   if(font->getFontFace())
     {
       if(!font->getErrorStatus())
	 {
	   font->getFontInformation(fontInfoList);
	   
	   for(i = 0; i < (int)fontInfoList.size();++i)
	     {
	       theFontInformationList.push_back(ossimFreeTypeFontInformation(file,
									     fontInfoList[i]));
	     }
	   result = true;
	 }
     }

   return result;
}

void ossimFreeTypeFontFactory::initializeDefaults()
{
   const ossimKeywordlist& kwl = ossimPreferences::instance()->preferencesKWL();
   ossimString regExpressionDir =  ossimString("^(") + "font.dir[0-9]+)";
   ossimString regExpressionFile =  ossimString("^(") + "font.file[0-9]+)";
   vector<ossimString> dirs =
      kwl.getSubstringKeyList( regExpressionDir );
   vector<ossimString> files =
      kwl.getSubstringKeyList( regExpressionFile );
   
   int idx = 0;
   for(idx = 0; idx < (int)dirs.size();++idx)
   {
      const char* directoryLocation = kwl.find(dirs[idx]);
      if(directoryLocation)
      {
         ossimDirectory d;
	 
         if(d.open(ossimFilename(directoryLocation)))
         {
            ossimFilename file;
	    
            if(d.getFirst(file, ossimDirectory::OSSIM_DIR_FILES))
            {
               do
               {
                  addFile(file);
               }while(d.getNext(file));
            }
         }
      }
   }
   for(idx = 0; idx < (int)files.size();++idx)
   {
      const char* fileName = kwl.find(files[idx]);
      
      if(fileName)
      {
         addFile(ossimFilename(fileName));
      }
   }   
#if 0
   if(!numberOfDirs&&!numberOfFiles)
   {
#ifdef __UNIX__
      ossimFilename file("/usr/X11R6/lib/X11/fonts/Type1");
      
      if(file.exists())
      {
         ossimDirectory d;
         if(d.open(file))
         {
            if(d.getFirst(file, ossimDirectory::OSSIM_DIR_FILES))
            {
               do
               {
                  addFile(file);
               }while(d.getNext(file));
            }
         }
      }
#endif
   }
#endif
}

#endif /* #if OSSIM_HAS_FREETYPE */
