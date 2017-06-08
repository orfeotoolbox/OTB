/*
 * Copyright (C) 2010-2016 by Frank Warmerdam (warmerda@home.com)
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


#ifndef ossimOgcWktTranslator_HEADER
#define ossimOgcWktTranslator_HEADER

#include <ossimPluginConstants.h>

#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimString.h>

#include <map>
#include <string>

class ossimKeywordlist;

class OSSIM_PLUGINS_DLL ossimOgcWktTranslator
{
public:
   ossimOgcWktTranslator();
   
   bool toOssimKwl(const ossimString& wktString,
                   ossimKeywordlist& kwl,
                   const char* prefix=nullptr)const;
   
   ossimString fromOssimKwl(const ossimKeywordlist& kwl,
                            const char* prefix=nullptr)const;
   /*!
    * Returns the empty string if the datum is not found
    *
    */
   ossimString wktToOssimDatum(const ossimString& datum)const;
   ossimString ossimToWktDatum(const ossimString& datum)const;

   ossimString wktToOssimProjection(const ossimString& projection)const;
   ossimString ossimToWktProjection(const ossimString& projection)const;
   
protected:

   std::map<std::string, std::string> theWktToOssimDatumTranslation;
   std::map<std::string, std::string> theWktToOssimProjectionTranslation;

   std::map<std::string, std::string> theOssimToWktDatumTranslation;
   std::map<std::string, std::string> theOssimToWktProjectionTranslation;

   void initializeDatumTable();
   void initializeProjectionTable();
   
};

#endif
