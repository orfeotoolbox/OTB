//*******************************************************************
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: This class extends the stl's string class.
// 
//********************************************************************
// $Id: ossimString.cpp 12047 2007-11-15 15:44:31Z gpotts $

#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimRegExp.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimHexString.h>

static ossimTrace traceDebug("ossimString:debug");

#ifdef OSSIM_ID_ENABLED
static char OSSIM_ID[] = "$Id: ossimString.cpp 12047 2007-11-15 15:44:31Z gpotts $";
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
   if(this->size() == 0) return *this;
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
      result.erase(pos, npos);
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

int ossimString::toInt()const
{
   int i = 0;
   ossimString os = c_str();
   if (!os.empty())
   {
      std::istringstream is(os);
      is >> i;
   }
   return i;
}

int ossimString::toInt(const ossimString& aString)
{
   int i = 0;
   ossimString os = aString;
   if (!os.empty())
   {
      std::istringstream is(os);
      is >> i;
   }
   return i;
}

ossim_int32 ossimString::toInt32()const
{
   ossim_int32 i = 0;
   ossimString os = c_str();
   if (!os.empty())
   {
      std::istringstream is(os);
      is >> i;
   }
   return i;
}

ossim_int32 ossimString::toInt32(const ossimString& aString)
{
   ossim_int32 i = 0;
   ossimString os = aString;
   if (!os.empty())
   {
      std::istringstream is(os);
      is >> i;
   }
   return i;
}

ossim_uint32 ossimString::toUInt32()const
{
   ossim_uint32 i = 0;
   ossimString os = c_str();
   if (!os.empty())
   {
      std::istringstream is(os);
      is >> i;
   }
   return i;
}

ossim_uint32 ossimString::toUInt32(const ossimString& aString)
{
   ossim_uint32 i = 0;
   ossimString os = aString;
   if (!os.empty())
   {
      std::istringstream is(os);
      is >> i;
   }
   return i;   
}

ossim_int64 ossimString::toInt64()const
{
   ossim_int64 i = 0;
   ossimString os = c_str();
   if (!os.empty())
   {
      std::istringstream is(os);
      is >> i;
   }
   return i;
}

ossim_int64 ossimString::toInt64(const ossimString& aString)
{
   ossim_int64 i = 0;
   ossimString os = aString;
   if (!os.empty())
   {
      std::istringstream is(os);
      is >> i;
   }
   return i;
}

ossim_uint64 ossimString::toUInt64()const
{
   ossim_uint64 i = 0;
   ossimString os = c_str();
   if (!os.empty())
   {
      std::istringstream is(os);
      is >> i;
   }
   return i;
}

ossim_uint64 ossimString::toUInt64(const ossimString& aString)
{
   ossim_int64 i = 0;
   ossimString os = aString;
   if (!os.empty())
   {
      std::istringstream is(os);
      is >> i;
   }
   return i;
}

long ossimString::toLong()const
{
   long i = 0;
   ossimString os = c_str();
   if (!os.empty())
   {
      std::istringstream is(os);
      is >> i;
   }
   return i;
}

long ossimString::toLong(const ossimString& aString)
{
   long i = 0;
   ossimString os = aString;
   if (!os.empty())
   {
      std::istringstream is(os);
      is >> i;
   }
   return i;
}

unsigned long  ossimString::toULong()const
{
   unsigned long i = 0;
   ossimString os = c_str();
   if (!os.empty())
   {
      std::istringstream is(os);
      is >> i;
   }
   return i;
}

unsigned long ossimString::toULong(const ossimString& aString)
{
   unsigned long i = 0;
   ossimString os = aString;
   if (!os.empty())
   {
      std::istringstream is(os);
      is >> i;
   }
   return i;
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
   ossimString os = c_str();
   if (!os.empty())
   {
      std::istringstream is(os);
      is >> d;
   }
#endif
   if(!this->empty())
   {
      d = (ossim_float32)atof(this->c_str());
   }

   return d;
}

ossim_float32 ossimString::toFloat32(const ossimString& aString)
{
   if(aString.contains("nan"))
   {
      return ossim::nan();
   }
   ossim_float32 d = 0.0;
   // this part is core dumping under mingw in ossimPlanet.
   // There is a possibility that this isn't a thread safe implementation
   // in mingw stl.  Let's resort back to atof for now

#if 0
   ossimString os =aString.c_str();
   if (!os.empty())
   {
      std::istringstream is(os);
      is >> d;
   }
#endif
   if(!aString.empty())
   {
      d = (ossim_float32)atof(aString.c_str());
   }

   return d;
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
   ossimString os = ths->c_str();
   if (!os.empty())
   {
      std::istringstream is(os);
      is >> d;
   }
#endif
   if(!this->empty())
   {
      d = (ossim_float64)atof(this->c_str());
   }
   
   return d;
}

ossim_float64 ossimString::toFloat64(const ossimString& aString)
{
   if(aString.contains("nan"))
   {
      return ossim::nan();
   }
   ossim_float64 d = 0.0;
   // this part is core dumping under mingw in ossimPlanet.
   // There is a possibility that this isn't a thread safe implementation
   // in mingw stl.  Let's resort back to atof for now

#if 0
   ossimString os = aString;
   if (!os.empty())
   {
      std::istringstream is(os);
      is >> d;
   }
#endif
   if(!aString.empty())
   {
      d = (ossim_float64)atof(aString.c_str());
   }
   return d;
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

#if 0
   ossimString os = c_str();
   if (!os.empty())
   {
      std::istringstream is(os);
      is >> d;
   }
#endif

   if(!this->empty())
   {
      d = atof(this->c_str());
   }
   
   return d;
}

double ossimString::toDouble(const ossimString& aString)
{
   if(aString.contains("nan"))
   {
      return ossim::nan();
   }
   double d = 0.0;
   // this part is core dumping under mingw in ossimPlanet.
   // There is a possibility that this isn't a thread safe implementation
   // in mingw stl.  Let's resort back to atof for now

#if 0
   ossimString os = c_str();
   if (!os.empty())
   {
      std::istringstream is(os);
      is >> d;
   }
#endif
   if(!aString.empty())
   {
      d = atof(aString.c_str());
   }

   return d;
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
                                  int precision,
                                  bool trimZeroFlag,
                                  bool scientific)
{
   if ( ossim::isnan(aValue) )
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

   s << std::setprecision(precision)  << aValue;
   ossimString result(s.str());
   if( !scientific && trimZeroFlag && result.contains(".") )
   {
      // Trim all zeroes front and back of the '.'.
      result = result.trim('0');
      if(result == ".")
      {
         result = "0";
      }
      else if(*(result.begin()+(result.size()-1)) == '.') // check for 123. now.
      {
         return ossimString(result.begin(),
                            result.begin()+result.size()-1);
      }

#if 0      
      ossim_uint32 size = result.size();
      if (size == 1) // A single '.'
      {
         result = "0";
      }
      else
      {
         ossimString::const_iterator i = result.begin();
         if ((*i) == '.')  // .something like ".129" make "0.129"
         {
            // .something like ".129" make "0.129"
            result.insert(0, 1, '0');
         }
         else
         {
            i = result.end();
            --i;
            if ((*i) == '.') // something. like 129. make 129"
            {
               // something. like 129. make "0.129"
               result.push_back('0');
            }
         }
      }
#endif
   }
   return result;
}

ossimString ossimString::toString(ossim_float64 aValue,
                                  int precision,
                                  bool trimZeroFlag,
                                  bool scientific)
{
   if (aValue == ossim::nan())
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

   if( !scientific && trimZeroFlag && result.contains(".") )
   {
      // Trim all zeroes front and back of the '.'.
      result = result.trim('0');
      if(result == ".")
      {
         result = "0";
      }
      else if(*(result.begin()+(result.size()-1)) == '.') // check for 123. now.
      {
         return ossimString(result.begin(),
                            result.begin()+result.size()-1);
      }
#if 0
      // Trim all zeroes front and back of the '.'.
      result = result.trim('0');

      ossim_uint32 size = result.size();
      if (size == 1) // A single '.'
      {
         result = "0.0";
      }
      else
      {
         ossimString::const_iterator i = result.begin();
         if ((*i) == '.')  // .something like ".129" make "0.129"
         {
            // .something like ".129" make "0.129"
            result.insert(0, 1, '0');
         }
         else
         {
            i = result.end();
            --i;
            if ((*i) == '.') // something. like 129. make "0.129"
            {
               // something. like 129. make "0.129"
               result.push_back('0');
            }
         }
      }
#endif
   }
   return result;
}

ossimString ossimString::before(const ossimString& str,
                                std::string::size_type pos)const
{
   if(*this == "") return ossimString();

   size_type last = find(str.c_str(), pos);
   if(last >= npos) return *this;
   
   return substr(0, last);
}

ossimString ossimString::after(const ossimString& str,
                               std::string::size_type pos)const
{
   size_type last = find(str.c_str(), pos);
   if (last >= npos) return ossimString();
   
   return substr(last+str.length());
}

void ossimString::split(std::vector<ossimString>& result,
                        const ossimString& separatorList)const
{
	if(this->empty()) return;
//   result = split(separatorList);
   ossimString copyString = *this;

   char* s = strtok(const_cast<char*>(copyString.c_str()),
                    separatorList.c_str());

   for(std::string::size_type i = 0; i < separatorList.size(); ++ i)
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
   for(std::string::size_type i = 0; i < separatorList.size(); ++ i)
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

