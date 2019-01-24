/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
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


#ifndef ossimPluginProjectionFactory_HEADER
#define ossimPluginProjectionFactory_HEADER
#include <ossimPluginConstants.h>
#include <ossim/projection/ossimProjectionFactoryBase.h>

namespace ossimplugins
{


class OSSIM_PLUGINS_DLL ossimPluginProjectionFactory : public ossimProjectionFactoryBase
{
public:

   static ossimPluginProjectionFactory* instance();
   /**
    * takes a filename. This filename can be an image file or
    * it can also be a ossim .geom file.  It could be other
    * forms of geom files as well.  The factories job will be to
    * determine what parser to use and return a projection if
    * successful.
    */
   virtual ossimProjection* createProjection(const ossimFilename& filename,
                                             ossim_uint32 entryIdx)const;
   using ossimProjectionFactoryBase::createProjection;

   /**
    * Take a projection type name.
    */
   virtual ossimProjection* createProjection(const ossimString& name)const;
   /**
    * Take a keywordlist.
    */
   virtual ossimProjection* createProjection(const ossimKeywordlist& kwl,
                                             const char* prefix=0)const;

   virtual ossimObject* createObject(const ossimString& typeName)const;

   /*!
    * Creates and object given a keyword list.
    */
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;

   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
protected:

   // Taken by copy -> copy elision
   bool isTileMap(ossimFilename filename) const;

private:
   ossimPluginProjectionFactory()
   {
      initialized_ = true;
   }

   ~ossimPluginProjectionFactory()
   {
      initialized_ = false;
   }

   static bool initialized_;

   static ossimPluginProjectionFactory factoryInstance;

   ossimPluginProjectionFactory(ossimPluginProjectionFactory const&);              // Don't
                                                                                   // Implement

   void operator=(ossimPluginProjectionFactory const&); // Don't implement

};
}

#endif
