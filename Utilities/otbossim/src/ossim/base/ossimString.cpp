//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: This class extends the stl's string class.
// 
//********************************************************************
// $Id: ossimString.cpp 22160 2013-02-25 12:09:35Z gpotts $

#include <cctype> /* for toupper */
#include <cstdlib> /* for getenv() */
#include <stack> 
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include <algorithm>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimRegExp.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimHexString.h>

static ossimTrace traceDebug("ossimString:debug");

#ifdef OSSIM_ID_ENABLED
static char OSSIM_ID[] = "$Id: ossimString.cpp 22160 2013-02-25 12:09:35Z gpotts $";
#endif

ossimString ossimString::upcase(const ossimString& aString)
{
   std::string s = aString.string();
   
   std::string::iterator eachCharacter = s.begin();
   while(eachCharacter != s.end())
   {
      *eachCharacter = toupper(*eachCharacter);
      eachCharacter++;
   }

   return s;
}

ossimString ossimString::downcase(const ossimString& aString)
{
   std::string s = aString.m_str;

   std::string::iterator eachCharacter = s.begin();
   while(eachCharacter != s.end())
   {
      *eachCharacter = tolower(*eachCharacter);
      ++eachCharacter;
   }

   return ossimString(s);
}

ossimString& ossimString::upcase()
{
   std::string::iterator eachCharacter = m_str.begin();
   while(eachCharacter != m_str.end())
   {
      *eachCharacter = toupper(*eachCharacter);
      ++eachCharacter;
   }

   return *this;
}

ossimString ossimString::upcase()const
{
   ossimString result(*this);
   result.upcase();
   return result;
}

ossimString& ossimString::downcase()
{
   std::string::iterator eachCharacter = m_str.begin();
   while(eachCharacter != m_str.end())
   {
      *eachCharacter = tolower(*eachCharacter);
      ++eachCharacter;
   }
   
   return *this;
}

ossimString ossimString::downcase()const
{
   ossimString result(*this);
   result.downcase();
   return result;
}

char* ossimString::stringDup()const
{
   char *result = 0;

   if(length() == 0)
   {
      result = new char[1];
      result[0] = '\0';
   }
   else
   {  
      ossim_uint32 index = 0;
      ossim_uint32 len = (ossim_uint32)m_str.length();
      result = new char[len+1];
      const char* sourceString = m_str.c_str();

      while(index < len) 
      {
         result[index] = sourceString[index];
         ++index;
      }
      result[len] = '\0';
   }
   return result;
}

ossimString ossimString::stripLeading(const ossimString &value, char characterToStrip)
{
   std::string s;
   ossimString::const_iterator stringIter = value.m_str.begin();
   
   while((*stringIter == characterToStrip)&&(stringIter!=value.m_str.end()))
   {
      ++stringIter;
   }

   while(stringIter != value.m_str.end())
   {
      s += *stringIter;
      ++stringIter;
   }
   
   return ossimString(s);
}

ossimString ossimString::substitute(const ossimString &searchKey,
                                    const ossimString &replacementValue,
                                    bool replaceAll)const
{
   std::string result = m_str;

   size_type pos = result.find(searchKey.m_str);
   
   if (pos == std::string::npos) return result;  // Search key not found.
   
   if(replaceAll)
   {
      while(pos != std::string::npos)
      {
         result.replace(pos, searchKey.m_str.size(), replacementValue.m_str.c_str());
         pos = result.find(searchKey.m_str, pos+replacementValue.m_str.size());
      }
   }
   else  // Replace only the first instance.
   {
      result.replace(pos, searchKey.m_str.size(), replacementValue.m_str.c_str());
   }

   return ossimString(result);
}

ossimString& ossimString::gsub(const ossimString &searchKey,
                               const ossimString &replacementValue,
                               bool replaceAll)
{
   size_type pos = m_str.find(searchKey.m_str);
   
   if (pos == std::string::npos) return *this;  // Search key not found.
   
   if(replaceAll)
   {
      while(pos < m_str.size())
      {
         m_str.replace(pos, searchKey.m_str.size(), replacementValue.m_str.c_str());
         pos = find(searchKey.m_str, pos+replacementValue.m_str.size());
      }
   }
   else  // Replace only the first instance.
   {
      m_str.replace(pos, searchKey.m_str.size(), replacementValue.m_str.c_str());
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
   if(m_str.size() == 0) return *this;
   if(valueToTrim.empty()) return *this;
   iterator startPos = (*this).begin();
   iterator endPos   = (*this).begin() + ((*this).size()-1);

   while( ( startPos != (*this).end() ) &&
          (std::find(valueToTrim.begin(),
                     valueToTrim.end(),
                     *startPos)!=valueToTrim.end()) ) ++startPos;

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

ossimString ossimString::beforePos(std::string::size_type pos)const
{
   ossimString result = *this;

   if(pos < length())
   {
      result.erase(pos, std::string::npos);
   }

   return result;
}

ossimString ossimString::afterPos(std::string::size_type pos)const
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

   tokenPtr = strtok((char*)exp_str.c_str(), (char*)delimeter.c_str());

   while(tokenPtr != NULL)
   {
      result.push_back(tokenPtr);
      tokenPtr = strtok(NULL, delimeter.c_str());
   }

   return result;
}

ossimString ossimString::expandEnvironmentVariable() const
{
   ossimString result(*this);
   std::stack<ossim_uint32> startChars;
   ossimRegExp regExpStart("\\$\\(");

   if(regExpStart.find(result.c_str()))
   {
      startChars.push(regExpStart.start());
      while(!startChars.empty())
      {
         ossim_uint32 offset = startChars.top() + 2; // skip over the $(
         
         // We will replace like a stack by looking at the right most $(
         //
         if(regExpStart.find(result.c_str()+offset))
         {
            // maintain absolute offset to the original string
            startChars.push(regExpStart.start()+offset);
         }
         else 
         {
            // now look for a closing ) for the stating $(
            ossimRegExp regExpEnd("\\)");
            if(regExpEnd.find(result.c_str()+startChars.top()))
            {
               ossimString envVarStr(result.begin()+startChars.top()+2,
                                     result.begin()+startChars.top()+regExpEnd.start());
               const char* lookup = getenv( envVarStr.c_str() );
               if ( lookup )
               {
                  result.replace(result.begin()+startChars.top(),
                                 result.begin()+startChars.top()+regExpEnd.start()+1,
                                 ossimString(lookup));
               }
               else 
               {
                  if(traceDebug())
                  {
                     ossimNotify(ossimNotifyLevel_WARN)
                     << "In member function ossimString::expandEnvironmentVariable() "
                     << "\n\tERROR: Environment variable("
                     << envVarStr.c_str()
                     << ") not found!"
                     << std::endl;
                  }
                  result.replace(result.begin()+startChars.top(),
                                 result.begin()+startChars.top()+regExpEnd.start()+1,
                                 "");
               }
            }
            startChars.pop();
         }
      }
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
   std::string::size_type offset     = 0;
   std::string::size_type valueLength = ossimString(value).length();
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

bool ossimString::toBool()const
{
   ossimString s = c_str();
   if (s.empty())
   {
      return false;
   }
   
   s = s.downcase();
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
   else if (toInt32())
   {
      return true;
   }

   return false;
}

bool ossimString::toBool(const ossimString& aString)
{
   // Check for true or false, yes or no, y or n, and 1 or 0...
   ossimString s = aString;
   if (s.empty())
   {
      return false;
   }
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
   else if (aString.toInt32())
   {
      return true;
   }

   return false;
}

ossim_uint8 ossimString::toUInt8()const
{
   // Note the std::istringstream::operator>> does not work with unsigned 8 bit.
   ossim_uint16 i = 0;
   if (!empty())
   {
      std::istringstream is(m_str);
      is >> i;
      if(is.fail())
      {
        i = 0;
      }
   }
   return static_cast<ossim_uint8>(i);
}

ossim_uint8 ossimString::toUInt8(const ossimString& aString)
{
   return aString.toUInt8();
}

int ossimString::toInt()const
{
   int i = 0;
   if (!empty())
   {
      std::istringstream is(m_str);
      is >> i;
      if(is.fail())
      {
         i = 0;
      }
   }
   return i;
}

int ossimString::toInt(const ossimString& aString)
{
   return aString.toInt();
}

ossim_int16 ossimString::toInt16()const
{
   ossim_int16 i = 0;
   if (!empty())
   {
      std::istringstream is(m_str);
      is >> i;
      if(is.fail())
      {
         i = 0;
      }
   }
   return i;
}

ossim_int16 ossimString::toInt16(const ossimString& aString)
{
   return aString.toInt16();
}

ossim_uint16 ossimString::toUInt16()const
{
   ossim_uint16 i = 0;
   if (!empty())
   {
      std::istringstream is(m_str);
      is >> i;
      if(is.fail())
      {
         i = 0;
      }
   }
   return i;
}

ossim_uint16 ossimString::toUInt16(const ossimString& aString)
{
   return aString.toUInt16();
}

ossim_int32 ossimString::toInt32()const
{
   ossim_int32 i = 0;
   if (!empty())
   {
      std::istringstream is(m_str);
      is >> i;
      if(is.fail())
      {
         i = 0;
      }
   }
   return i;
}

ossim_int32 ossimString::toInt32(const ossimString& aString)
{
   return aString.toInt32();
}

ossim_uint32 ossimString::toUInt32()const
{
   ossim_uint32 i = 0;
   if (!empty())
   {
      std::istringstream is(m_str);
      is >> i;
      if(is.fail())
      {
         i = 0;
      }
   }
   return i;
}

ossim_uint32 ossimString::toUInt32(const ossimString& aString)
{
   return aString.toUInt32();
}

ossim_int64 ossimString::toInt64()const
{
   ossim_int64 i = 0;
   if (!empty())
   {
      std::istringstream is(m_str);
      is >> i;
      if(is.fail())
      {
         i = 0;
      }
   }
   return i;
}

ossim_int64 ossimString::toInt64(const ossimString& aString)
{
   return aString.toInt64();
}

ossim_uint64 ossimString::toUInt64()const
{
   ossim_uint64 i = 0;
   if (!empty())
   {
      std::istringstream is(m_str);
      is >> i;
      if(is.fail())
      {
         i = 0;
      }
   }
   return i;
}

ossim_uint64 ossimString::toUInt64(const ossimString& aString)
{
   return aString.toUInt64();
}

long ossimString::toLong()const
{
  long i = 0;
   if (!empty())
   {
      std::istringstream is(m_str);
      is >> i;
      if(is.fail())
      {
         i = 0;
      }
   }
   return i;
}

long ossimString::toLong(const ossimString& aString)
{
   return aString.toLong();
}

unsigned long  ossimString::toULong()const
{
   unsigned long i = 0;
   if (!empty())
   {
      std::istringstream is(m_str);
      is >> i;
      if(is.fail())
      {
         i = 0;
      }
   }
   return i;
}

unsigned long ossimString::toULong(const ossimString& aString)
{
   return aString.toULong();
}

ossim_float32 ossimString::toFloat32()const
{
   if(contains("nan"))
   {
      return ossim::nan();
   }
   ossim_float32 d = 0.0;
   // this part is core dumping under mingw in ossimPlanet.
   // There is a possibility that this isn't a thread safe implementation
   // in mingw stl.  Let's resort back to atof for now

#if 0
   d = (ossim_float32)atof(c_str());
#endif
#if 1
   if (!empty())
   {
      std::istringstream is(c_str());
      is >> d;
      if(is.fail())
      {
         d = 0.0;
      }
   }
#endif
   return d;
}

ossim_float32 ossimString::toFloat32(const ossimString& aString)
{
   return aString.toFloat32();
}

ossim_float64 ossimString::toFloat64()const
{
   if(contains("nan"))
   {
      return ossim::nan();
   }
   ossim_float64 d = 0.0;
   // this part is core dumping under mingw in ossimPlanet.
   // There is a possibility that this isn't a thread safe implementation
   // in mingw stl.  Let's resort back to atof for now

#if 0
   d = (ossim_float64)atof(c_str());
#endif
#if 1
   if (!empty())
   {
      std::istringstream is(c_str());
      is >> d;
      if(is.fail())
      {
         d = 0.0;
      }
   }
#endif
   return d;
}

ossim_float64 ossimString::toFloat64(const ossimString& aString)
{
   return aString.toFloat64();
}

double ossimString::toDouble()const
{
   if(contains("nan"))
   {
      return ossim::nan();
   }
   double d = 0.0;

   // this part is core dumping under mingw in ossimPlanet.
   // There is a possibility that this isn't a thread safe implementation
   // in mingw stl.  Let's resort back to atof for now

   if (!empty())
   {
#if 0
      d = atof(c_str());
#endif
#if 1
      if(!empty())
      {
         std::istringstream is(c_str());
         is >> d;
         if(is.fail())
         {
            d = 0.0;
         }
      }
#endif
   }
   return d;
}

double ossimString::toDouble(const ossimString& aString)
{
   return aString.toDouble();
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

ossimString ossimString::toString(ossim_int64 aValue)
{
   std::ostringstream s;
   s << aValue;
   ossimString result(s.str());
   return result;
}

ossimString ossimString::toString(ossim_uint64 aValue)
{
   std::ostringstream s;
   s << aValue;
   ossimString result(s.str());
   return result;
}

ossimString ossimString::toString(ossim_float32 aValue,
                                  ossim_int32 precision,
                                  bool fixed)
{
   if ( ossim::isnan(aValue) )
   {
      return ossimString("nan");
   }

   std::ostringstream s;
   s << std::setprecision(precision);
   
   if (fixed)
   {
      s << std::setiosflags(std::ios::fixed); 
   }
   
   s << aValue;
   
   return ossimString(s.str());
}

ossimString ossimString::toString(ossim_float64 aValue,
                                  ossim_int32 precision,
                                  bool fixed)
{
   if ( ossim::isnan(aValue) )
   {
      return ossimString("nan");
   }
   
   std::ostringstream s;
   s << std::setprecision(precision);
   
   if (fixed)
   {
      s << std::setiosflags(std::ios::fixed); 
   }
   
   s << aValue;
   
   return ossimString(s.str());
}

ossimString ossimString::before(const ossimString& str,
                                std::string::size_type pos)const
{
   if(*this == "") return ossimString();

   size_type last = find(str.c_str(), pos);
   if(last >= std::string::npos) return *this;
   
   return ossimString( substr(0, last) );
}

ossimString ossimString::after(const ossimString& str,
                               std::string::size_type pos)const
{
   size_type last = find(str.c_str(), pos);
   if (last >= std::string::npos) return ossimString();
   
   return ossimString( substr(last+str.length()) );
}

//*************************************************************************************************
// Splits this string into a vector of strings (fields) using the delimiter list specified.
// If a delimiter is encountered at the beginning or the end of this, or two delimiters are 
// contiguous, a blank field is inserted in the vector, unless skipBlankFields is true.
//*************************************************************************************************
void ossimString::split(std::vector<ossimString>& result,
                        const ossimString& separatorList,
                        bool skipBlankFields)const
{
	if(this->empty()) return;
   
   std::string::const_iterator iterStart = m_str.begin();
   std::string::const_iterator iterCurrent = m_str.begin();
   
   while(iterCurrent != m_str.end())
   {
      if(std::find(separatorList.m_str.begin(), separatorList.m_str.end(), *iterCurrent) != separatorList.m_str.end())
      {
         if(iterStart == iterCurrent)
         {
            if(!skipBlankFields)
            {
               result.push_back(ossimString());
            }
         }
         else 
         {
            result.push_back(ossimString(iterStart, iterCurrent));
         }

         ++iterCurrent;
         iterStart = iterCurrent;
      }
      else 
      {
         ++iterCurrent;
      }
   }
   if(iterStart!=iterCurrent)
   {
      result.push_back(ossimString(iterStart, iterCurrent));
   }
   
#if 0   
//   result = split(separatorList);
   ossimString copyString = *this;

   char* s = strtok(const_cast<char*>(copyString.c_str()),
                    separatorList.c_str());

   for(std::string::size_type i = 0; i < separatorList.size(); ++ i)
   {
      if (((*(this->begin())) == separatorList.c_str()[i]) && !skipBlankFields)
         result.push_back("");
   }
   while(s)
   {
      result.push_back(ossimString(s));
      if ((*s != 0) || !skipBlankFields)
         s = strtok(NULL, separatorList.c_str());
   }
   for(std::string::size_type i = 0; i < separatorList.size(); ++ i)
   {
      if (((*(this->end()-1)) == separatorList.c_str()[i]) && !skipBlankFields)
         result.push_back("");
   }
#endif
}

std::vector<ossimString> ossimString:: split(const ossimString& separatorList,
                                             bool skipBlankFields)const
{
   std::vector<ossimString> result;

   split(result, separatorList, skipBlankFields);
   
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

ossimString ossimString::urlEncode()const
{
   ossimString::const_iterator iter = begin();
   ossimString result;
   while(iter != end())
   {
      ossim_uint8 c = *iter;
      
      if(c > 47 && c < 58)
      {
         result += c;
      }
      else if(c > 64 && c < 91)
      {
         result += c;
      }
      else if(c > 96 && c < 123)
      {
         result+=c;
      }
      else if (c == 32)
      {
         result+="+";
      }
      else
      {
         result += ("%" + ossimHexString(c));
      }
      
      ++iter;
   }

  return result;
   
}

ossimString ossimString::getOssimId() const
{
#ifdef OSSIM_ID_ENABLED
   return ossimString(OSSIM_ID);
#endif
   return ossimString("");
}

