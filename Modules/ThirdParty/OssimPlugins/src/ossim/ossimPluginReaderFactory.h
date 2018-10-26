/*
 * Copyright (C) 2010-2016 by David Burken
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

#ifndef ossimPluginReaderFactory_HEADER
#define ossimPluginReaderFactory_HEADER 1

#include <ossimPluginConstants.h>
#include <ossim/imaging/ossimImageHandlerFactoryBase.h>

class ossimString;
class ossimFilename;
class ossimKeywordlist;

namespace ossimplugins
{


/** @brief Image handler factory for ossim plugins plugin. */
   class OSSIM_PLUGINS_DLL ossimPluginReaderFactory : public ossimImageHandlerFactoryBase
   {
   public:

      /** @brief virtual destructor */
      virtual ~ossimPluginReaderFactory();

      /**
       * @brief static method to return instance (the only one) of this class.
       * @return pointer to instance of this class.
       */
      static ossimPluginReaderFactory* instance();

      /**
       * @brief open that takes a file name.
       * @param file The file to open.
       * @return pointer to image handler on success, 0 on failure.
       */
      virtual ossimImageHandler* open(const ossimFilename& fileName,
                                      bool openOverview=true) const;

      /**
       * @brief open that takes a keyword list and prefix.
       * @param kwl The keyword list.
       * @param prefix the keyword list prefix.
       * @return pointer to image handler on success, 0 on failure.
       */
      virtual ossimImageHandler* open(const ossimKeywordlist& kwl,
                                      const char* prefix=0)const;

      /**
       * @brief createObject that takes a class name.
       * @param typeName The name of the class..
       * @return pointer to image writer on success, 0 on failure.
       */
      virtual ossimObject* createObject(const ossimString& typeName)const;

      /**
       * @brief Creates and object given a keyword list and prefix.
       * @param kwl The keyword list.
       * @param prefix the keyword list prefix.
       * @return pointer to image handler on success, 0 on failure.
       */
      virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                        const char* prefix=0)const;

      /**
       * @brief Adds ossimTerraSarTiffReader to the typeList.
       * @param typeList List to add to.
       */
      virtual void getTypeNameList(std::vector<ossimString>& typeList)const;

      /**
       * @brief Method to add supported extension to the list, like "png".
       *
       * @param extensionList The list to add to.
       */
      virtual void getSupportedExtensions(
         ossimImageHandlerFactoryBase::UniqueStringList& extensionList)const;

      /** @brief deleted copy constructor */
      ossimPluginReaderFactory(const ossimPluginReaderFactory&) = delete;

      /** @brief deleted copy assignment operator */
      void operator=(const ossimPluginReaderFactory&) = delete;

   protected:
      /** @brief hidden from use default constructor */
      ossimPluginReaderFactory() {}

      /** static instance of this class */
      static ossimPluginReaderFactory* theInstance;

      TYPE_DATA
  };
}

#endif /* end of #ifndef ossimPluginReaderFactory_HEADER */
