//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Class declaration for ossimFileParser.
// 
// This class parses a Space Imaging Quickbird meta data file.
//
//********************************************************************
// $Id: ossimFileParser.h 14412 2009-04-27 16:58:46Z dburken $

#include <ossim/base/ossimFileParser.h>
#include <ossim/base/ossimTrace.h>
#include <iostream>

RTTI_DEF1(ossimFileParser, "ossimFileParser", ossimObject);

static ossimTrace traceExec  ("ossimFileParser:exec");
static ossimTrace traceDebug ("ossimFileParser:debug");

ossimFileParser::ossimFileParser()
{
}

ossimFileParser::~ossimFileParser()
{
}

bool ossimFileParser::getEndOfLine( char * fileBuf, ossimString lineBeginning, char * format, ossimString & name)
  {
     char * res = strstr(fileBuf, lineBeginning.c_str());
     if (!res)
     {
       return false;
      }
    fileBuf = res;

    char dummy[80], nameChar[80];
    sscanf(res, format, dummy, nameChar);
    name = ossimString(nameChar);

    return true;
  }

 bool getEndOfLine( char * fileBuf, ossimString lineBeginning, char * format, ossim_float64 & value)
{
   char * res = strstr(fileBuf, lineBeginning.c_str());
     if (!res)
     {
       return false;
      }
    fileBuf = res;

    char dummy[80], nameChar[80];
    sscanf(res, format, dummy, nameChar);
    value = ossimString(nameChar).toFloat64();

    return true;
}

