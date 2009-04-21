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
//  $Id: ossimOgcWktTranslator.h 9858 2006-11-02 22:18:27Z dburken $

#ifndef ossimOgcWktTranslator_HEADER
#define ossimOgcWktTranslator_HEADER

#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimString.h>

#include <map>

class ossimKeywordlist;

class ossimOgcWktTranslator
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

   std::map<ossimString, ossimString> theWktToOssimDatumTranslation;
   std::map<ossimString, ossimString> theWktToOssimProjectionTranslation;

   std::map<ossimString, ossimString> theOssimToWktDatumTranslation;
   std::map<ossimString, ossimString> theOssimToWktProjectionTranslation;

   void initializeDatumTable();
   void initializeProjectionTable();
   
};

#endif
