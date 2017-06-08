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
      ~ossimPluginReaderFactory() override;

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
      ossimImageHandler* open(const ossimFilename& fileName,
                                      bool openOverview=true) const override;

      /**
       * @brief open that takes a keyword list and prefix.
       * @param kwl The keyword list.
       * @param prefix the keyword list prefix.
       * @return pointer to image handler on success, 0 on failure.
       */
      ossimImageHandler* open(const ossimKeywordlist& kwl,
                                      const char* prefix=nullptr)const override;

      /**
       * @brief createObject that takes a class name.
       * @param typeName The name of the class..
       * @return pointer to image writer on success, 0 on failure.
       */
      ossimObject* createObject(const ossimString& typeName)const override;

      /**
       * @brief Creates and object given a keyword list and prefix.
       * @param kwl The keyword list.
       * @param prefix the keyword list prefix.
       * @return pointer to image handler on success, 0 on failure.
       */
      ossimObject* createObject(const ossimKeywordlist& kwl,
                                        const char* prefix=nullptr)const override;

      /**
       * @brief Adds ossimTerraSarTiffReader to the typeList.
       * @param typeList List to add to.
       */
      void getTypeNameList(std::vector<ossimString>& typeList)const override;

      /**
       * @brief Method to add supported extension to the list, like "png".
       *
       * @param extensionList The list to add to.
       */
      void getSupportedExtensions(
         ossimImageHandlerFactoryBase::UniqueStringList& extensionList)const override;

   protected:
      /** @brief hidden from use default constructor */
      ossimPluginReaderFactory();

      /** @brief hidden from use copy constructor */
      ossimPluginReaderFactory(const ossimPluginReaderFactory&);

      /** @brief hidden from use copy constructor */
      void operator=(const ossimPluginReaderFactory&);

      /** static instance of this class */
      static ossimPluginReaderFactory* theInstance;

      TYPE_DATA
         };
}

#endif /* end of #ifndef ossimPluginReaderFactory_HEADER */
