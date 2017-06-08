/*
 * Copyright (C) 2010-2016 by Garrett Potts
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <ossim/plugin/ossimSharedObjectBridge.h>
#include <ossimPluginConstants.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossimPluginProjectionFactory.h>
#include <ossimPluginReaderFactory.h>


namespace ossimplugins
{


   static void setDescription(ossimString& description)
   {
      description = "OSSIM Plugin\n\n";
      std::vector<ossimString> projectionTypes;

      ossimPluginProjectionFactory::instance()->getTypeNameList(projectionTypes);
      ossim_uint32 idx = 0;
      description = "Projecitons Supported:\n\n";
      for(idx = 0; idx < projectionTypes.size();++idx)
      {
         description += projectionTypes[idx] + "\n";
      }
   }


   extern "C"
   {
      ossimSharedObjectInfo  myInfo;
      ossimString theDescription;
      std::vector<ossimString> theObjList;
      const char* getDescription()
      {
         return theDescription.c_str();
      }
      int getNumberOfClassNames()
      {
         return (int)theObjList.size();
      }
      const char* getClassName(int idx)
      {
         if(idx < (int)theObjList.size())
         {
            return theObjList[0].c_str();
         }
         return (const char*)nullptr;
      }

      /* Note symbols need to be exported on windoze... */
      OSSIM_PLUGINS_DLL void ossimSharedLibraryInitialize(
         ossimSharedObjectInfo** info)
      {
         myInfo.getDescription = getDescription;
         myInfo.getNumberOfClassNames = getNumberOfClassNames;
         myInfo.getClassName = getClassName;

         *info = &myInfo;

         /** Register the readers... */
         ossimImageHandlerRegistry::instance()->
            registerFactory(ossimPluginReaderFactory::instance());

         /**
          * Register the projection factory.
          * Note that this must be pushed to the front of the factory or bilinear
          * projection will be picked up.
          */
         ossimProjectionFactoryRegistry::instance()->
            registerFactoryToFront(ossimPluginProjectionFactory::instance());

         setDescription(theDescription);
      }

      /* Note symbols need to be exported on windoze... */
      OSSIM_PLUGINS_DLL void ossimSharedLibraryFinalize()
      {
         ossimImageHandlerRegistry::instance()->
            unregisterFactory(ossimPluginReaderFactory::instance());

         ossimProjectionFactoryRegistry::instance()->
            unregisterFactory(ossimPluginProjectionFactory::instance());
      }

   }
}
