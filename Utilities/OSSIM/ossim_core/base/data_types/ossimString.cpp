//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.  All rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// Description: This class extends the stl's string class.
// 
//********************************************************************
// $Id: ossimString.cpp,v 1.52 2005/10/17 18:37:16 gpotts Exp $

#include <sstream>
#include <algorithm>
#include <base/data_types/ossimString.h>
#include <base/data_types/ossimRegExp.h>
#include <base/common/ossimConstants.h>
#include <base/common/ossimTrace.h>

static ossimTrace traceDebug("ossimString:debug");

#ifdef OSSIM_ID_ENABLED
static char OSSIM_ID[] = "$Id: ossimString.cpp,v 1.52 2005/10/17 18:37:16 gpotts Exp $";
#endif

ossimString::ossimString(char aChar)
   :
	std::string()
{
   char tmp[2];
   tmp[0] = aChar;
   tmp[1] = '\0';
   append(tmp);
}

ossimString::ossimString(size_type n, char c)
: std::string(n, c)
{
}

ossimString ossimString::upcase(const ossimString& aString)
{
   ossimString s(aString);

   ossimString::iterator eachCharacter = s.begin();
   while(eachCharacter != s.end())
   {
      *eachCharacter = toupper(*eachCharacter);
      eachCharacter++;
   }

   return s;
}

ossimString ossimString::downcase(const ossimString& aString)
{
   ossimString s(aString);

   ossimString::iterator eachCharacter = s.begin();
   while(eachCharacter != s.end())
   {
      *eachCharacter = tolower(*eachCharacter);
      eachCharacter++;
   }

   return s;
}

ossimString& ossimString::upcase()
{
   ossimString::iterator eachCharacter = begin();
   while(eachCharacter != end())
   {
      *eachCharacter = toupper(*eachCharacter);
      eachCharacter++;
   }

   return *this;
}

ossimString& ossimString::downcase()
{
   ossimString::iterator eachCharacter=begin();
   while(eachCharacter != end())
   {
      *eachCharacter = tolower(*eachCharacter);
      eachCharacter++;
   }

   return *this;
}

char* ossimString::stringDup()const
{
   char *result = 0;

   if(length() == 0)
   {
      result = new char;
      result[0] = '\0';
   }
   else
   {  
      long index = 0;
      long len = length();
      result = new char[len+1];
      const char* sourceString = c_str();

      while(index < len) 
      {
         result[index] = sourceString[index];
         index++;
      }
      result[len] = '\0';
   }
   return result;
}

ossimString ossimString::stripLeading(const ossimString &value, char characterToStrip)
{
   ossimString result;
   ossimString::const_iterator stringIter = value.begin();
   
   while((*stringIter == characterToStrip)&&(stringIter!=value.end()))
   {
      ++stringIter;
   }

   while(stringIter != value.end())
   {
      result += *stringIter;
      ++stringIter;
   }
   
   return result;
}

ossimString ossimString::substitute(const ossimString &searchKey,
                                    const ossimString &replacementValue,
                                    bool replaceAll)const
{
   ossimString result = *this;

   size_type pos = result.find(searchKey);
   
   if (pos == npos) return result;  // Search key not found.
   
   if(replaceAll)
   {
      while(pos != npos)
      {
         result.replace(pos, searchKey.size(), replacementValue.c_str());
         pos = result.find(searchKey, pos+replacementValue.size());
      }
   }
   else  // Replace only the first instance.
   {
      result.replace(pos, searchKey.size(), replacementValue.c_str());
   }

   return result;
}

ossimString& ossimString::gsub(const ossimString &searchKey,
                               const ossimString &replacementValue,
                               bool replaceAll)
{
   size_type pos = find(searchKey);
   
   if (pos == npos) return *this;  // Search key not found.
   
   if(replaceAll)
   {
      while(pos < size())
      {
         replace(pos, searchKey.size(), replacementValue.c_str());
         pos = find(searchKey, pos+replacementValue.size());
      }
   }
   else  // Replace only the first instance.
   {
      replace(pos, searchKey.size(), replacementValue.c_str());
   }

   return *this;
}

ossimString  ossimString::trim(const ossimString& valueToTrim) const
{
   ossimString tempString(*this);
   
   return tempString.trim(valueToTrim);
}

ossimString& ossimString::trim(const ossimString& valueToTrim)
{
   iterator startPos = (*this).begin();
   iterator endPos   = (*this).end();

   --endPos;
   
   while( (std::find(valueToTrim.begin(),
                     valueToTrim.end(),
                     *startPos)!=valueToTrim.end()) &&
          (startPos != (*this).end())) ++startPos;

   if(startPos == (*this).end())
   {
      *this = "";
      return *this;
   }
   
   while( (endPos!=startPos)&& (std::find(valueToTrim.begin(),
                                          valueToTrim.end(),
                                          *endPos)!=valueToTrim.end())) --endPos;

   *this = ossimString(startPos, endPos+1);

   return *this;
}

ossimString ossimString::beforePos(ossim_uint32 pos)const
{
   ossimString result = *this;

   if(pos < length())
   {
      result.erase(pos, npos);
   }

   return result;
}

ossimString ossimString::afterPos(ossim_uint32 pos)const
{
   ossimString result = *this;

   if(pos < length())
   {
      result.erase(0, pos+1);
   }
   else
   {
      return "";
   }

   return result;
   
}

std::vector<ossimString> ossimString::explode(const ossimString& delimeter) const
{
   ossimString exp_str = *this;
   std::vector<ossimString> result;
   char* tokenPtr;

   tokenPtr = strtok((char*)exp_str.chars(), (char*)delimeter.chars());

   while(tokenPtr != NULL)
   {
      result.push_back(tokenPtr);
      tokenPtr = strtok(NULL, delimeter.chars());
   }

   return result;
}

//---
// Regular expression pattern utilities
//---

ossimString ossimString::beforeRegExp(const char *regularExpressionPattern) const
{   
   ossimRegExp anExpression;
   
   anExpression.compile(regularExpressionPattern);

   if(anExpression.find(c_str()))
   {
      if (anExpression.start() > 0)
      {
         return substr(0, anExpression.start());
      }
   }
   
   return ossimString("");
}

ossimString ossimString::fromRegExp(const char *regularExpressionPattern) const
{   
   ossimRegExp anExpression;
   
   anExpression.compile(regularExpressionPattern);

   if(anExpression.find(c_str()))
   {
      if (anExpression.start() < size())
      {
         return substr(anExpression.start(), (size()-anExpression.start()));
      }
   }
   
   return ossimString("");
}

ossimString ossimString::afterRegExp(const char *regularExpressionPattern) const
{   
   ossimRegExp anExpression;
   
   anExpression.compile(regularExpressionPattern);

   if(anExpression.find(c_str()))
   {
      if (anExpression.end() < size())
      {
         return substr(anExpression.end(), (size()-anExpression.end()));
      }
   }
   
   return ossimString("");
}

ossimString ossimString::match(const char *regularExpressionPattern) const
{   
   ossimRegExp anExpression;
   
   anExpression.compile(regularExpressionPattern);

   if((anExpression.find(this->c_str())) &&
      (anExpression.start() !=anExpression.end()))
   {
      return this->substr(anExpression.start(),
                          anExpression.end() - anExpression.start() );
   }

   
   return ossimString("");
}

ossimString ossimString::replaceAllThatMatch(const char *regularExpressionPattern,
                                             const char *value) const
{
   ossimString result = *this;
   ossimRegExp anExpression;
   ossim_uint32 offset     = 0;
   ossim_uint32 valueLength = ossimString(value).length();
   anExpression.compile(regularExpressionPattern);
   if(!anExpression.is_valid())
   {
      return *this;
   }
   while(anExpression.find(result.c_str()+offset))
   {
      if(anExpression.start() < anExpression.end())
      {
         result.replace(anExpression.start() + offset,
                        anExpression.end()-anExpression.start(),
                        value);
         offset += anExpression.start() + valueLength;
      }
      else
      {
         break;
      }
   }

   return result;
}

ossimString ossimString::replaceStrThatMatch(const char *regularExpressionPattern,
                                             const char *value) const
{
   ossimString result = *this;
   ossimRegExp anExpression;
   anExpression.compile(regularExpressionPattern);
   if(!anExpression.is_valid())
   {
      return *this;
   }
   if(anExpression.find(result.c_str()))
   {
      if(anExpression.start() < anExpression.end())
      {
         result.replace(anExpression.start(),
                        anExpression.end()-anExpression.start(),
                        value);
      }
   }

   return result;
}

double ossimString::toDouble()const
{
   if(contains("nan"))
   {
      return OSSIM_DBL_NAN;
   }

   return atof(c_str());
}

double ossimString::toDouble(const ossimString& aString)
{
   if(aString.contains("nan"))
   {
      return OSSIM_DBL_NAN;
   }
   return atof(aString.c_str());
}

int ossimString::toInt()const
{
   return atoi(c_str());
}

int ossimString::toInt(const ossimString& aString)
{
   return (int)atoi(aString.c_str());
}

ossim_int32 ossimString::toInt32()const
{
   return (ossim_int32)atoi(c_str());
}

ossim_int32 ossimString::toInt32(const ossimString& aString)
{
   return (ossim_int32)atoi(aString.c_str());
}

ossim_uint32 ossimString::toUInt32()const
{
   return static_cast<ossim_uint32>(atoi(c_str()));
}

ossim_uint32 ossimString::toUInt32(const ossimString& aString)
{
   return static_cast<ossim_uint32>(atoi(aString.c_str()));
}

long ossimString::toLong()const
{
   return atol(c_str());
}

long ossimString::toLong(const ossimString& aString)
{
   return atol(aString.c_str());
}

unsigned long  ossimString::toULong()const
{
   return static_cast<unsigned long>(atol(c_str()));
}

unsigned long ossimString::toULong(const ossimString& aString)
{
   return static_cast<unsigned long>(atol(aString.c_str()));
}

bool ossimString::toBool()const
{
   ossimString s = c_str();
   s = s.trim().downcase();
   if ( (s == "true") ||
        (s == "yes")  ||
        (s == "y")    ||
        (s == "1") )
   {
      return true;
   }
   else if ( (s == "false") ||
             (s == "no")    ||
             (s == "n")     ||
             (s == "0") )
   {
      return false;
      
   }
   else if (toULong())
   {
      return true;
   }

   return false;
}

bool ossimString::toBool(const ossimString& aString)
{
   // Check for true or false, yes or no, y or n, and 1 or 0...
   ossimString s = aString;
   s.downcase();
   if ( (s == "true") ||
        (s == "yes")  ||
        (s == "y")    ||
        (s == "1") )
   {
      return true;
   }
   else if ( (s == "false") ||
             (s == "no")    ||
             (s == "n")     ||
             (s == "0") )
   {
      return false;
      
   }
   else if (aString.toULong())
   {
      return true;
   }

   return false;
}

ossimString ossimString::toString(bool aValue)
{
	std::ostringstream s;

   s << aValue;

   ossimString result(s.str());

   return result;
}

ossimString ossimString::toString(ossim_int16 aValue)
{
	std::ostringstream s;

   s << aValue;

   ossimString result(s.str());

   return result;
}

ossimString ossimString::toString(ossim_uint16 aValue)
{
	std::ostringstream s;

   s << aValue;

   ossimString result(s.str());

   return result;
}

ossimString ossimString::toString(ossim_int32 aValue)
{
	std::ostringstream s;

   s << aValue;

   ossimString result(s.str());

   return result;
}

ossimString ossimString::toString(ossim_uint32 aValue)
{
	std::ostringstream s;

   s << aValue;

   ossimString result(s.str());

   return result;
}

ossimString ossimString::toString(long aValue)
{
	std::ostringstream s;

   s << aValue;

   ossimString result(s.str());

   return result;
}

ossimString ossimString::toString(unsigned long aValue)
{
	std::ostringstream s;

   s << aValue;

   ossimString result(s.str());

   return result;
}

ossimString ossimString::toString(ossim_float32 aValue,
                                  int precision,
                                  bool trimZeroFlag,
                                  bool scientific)
{
   if (aValue == OSSIM_FLT_NAN)
   {
      return ossimString("nan");
   }
   
   std::ostringstream s;

   if (!scientific)
   {
	   s << setiosflags(std::ios::fixed);
   }
   else
   {
	   s << setiosflags(std::ios::scientific);
   }

   s << std::setprecision(precision) << aValue;
   
   ossimString result(s.str());

   if( trimZeroFlag && result.contains(".") )
   {
      result = result.trim('0');
      result = result.trim('.');
   }
   return result;
}

ossimString ossimString::toString(ossim_float64 aValue,
                                  int precision,
                                  bool trimZeroFlag,
                                  bool scientific)
{
   if (aValue == OSSIM_DBL_NAN)
   {
      return ossimString("nan");
   }
   
   std::ostringstream s;
   
   if (!scientific)
   {
	   s << setiosflags(std::ios::fixed);
   }
   else
   {
	   s << setiosflags(std::ios::scientific);
   }

   s << std::setprecision(precision) << aValue;

   ossimString result(s.str());

   if( trimZeroFlag && result.contains(".") )
   {
      result = result.trim('0');
      result = result.trim('.');
   }

   return result;
}

ossimString ossimString::before(const ossimString& str, ossim_uint32 pos)
{
   if(*this == "") return ossimString();
   
   ossim_uint32 last = find(str.c_str(), pos);
   if(last >= npos) return *this;
   
   return substr(0, last);
}

ossimString ossimString::after(const ossimString& str, ossim_uint32 pos)
{
   size_t last = find(str.c_str(), pos);
   if (last >= npos) return ossimString();
   
   return substr(last+str.length());
}

void ossimString::split(std::vector<ossimString>& result,
                        const ossimString& separatorList)const
{
//   result = split(separatorList);
   ossimString copyString = *this;

   char* s = strtok(const_cast<char*>(copyString.c_str()),
                    separatorList.c_str());

   for(ossim_uint32 i = 0; i < separatorList.size(); ++ i)
   {
      if((*(this->begin())) == separatorList.c_str()[i])
      {
         result.push_back("");
      }
   }
   while(s)
   {
      result.push_back(ossimString(s));
      s = strtok(NULL,
                 separatorList.c_str());
   }
   for(ossim_uint32 i = 0; i < separatorList.size(); ++ i)
   {
      if((*(this->end()-1)) == separatorList.c_str()[i])
      {
         result.push_back("");
      }
   }
}

std::vector<ossimString> ossimString:: split(const ossimString& separatorList)const
{
   std::vector<ossimString> result;

   split(result, separatorList);
   
   return result;
}

const ossimString& ossimString::join(const std::vector<ossimString>& stringList,
                                     const ossimString& separator)
{
   *this = "";
   if(stringList.size())
   {
      for(long i = 0; i < ((long)stringList.size()-1); ++i)
      {
         *this += stringList[i];
         *this += separator;
      }
      *this += stringList[stringList.size()-1];
   }

   return *this;
}

ossimString ossimString::getOssimId() const
{
#ifdef OSSIM_ID_ENABLED
   return ossimString(OSSIM_ID);
#endif
   return ossimString("");
}
