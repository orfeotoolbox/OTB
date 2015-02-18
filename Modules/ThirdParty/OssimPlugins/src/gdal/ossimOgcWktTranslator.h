//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Frank Warmerdam (warmerda@home.com)
//
// Description:
//
// Contains function declarations for translating between OGC WKT strings,
// and OSSIM projection definitions.
//*******************************************************************
//  $Id: ossimOgcWktTranslator.h 20724 2012-04-13 21:00:52Z dburken $

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
                   const char* prefix=NULL)const;
   
   ossimString fromOssimKwl(const ossimKeywordlist& kwl,
                            const char* prefix=NULL)const;
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
