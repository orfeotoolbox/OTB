//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description: This class provides capabilities for keywordlists.
//
//********************************************************************
// $Id: ossimKeywordlist.cpp,v 1.62 2005/10/17 19:16:33 gpotts Exp $
#include <algorithm>
#include <fstream>
#include <iostream>

#include <base/data_types/ossimRegExp.h>
#include <base/data_types/ossimFilename.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/context/ossimNotifyContext.h>
#include <base/common/ossimTrace.h>

static ossimTrace traceDebug("ossimKeywordlist:debug");
static const ossim_int32 MAX_LINE_LENGTH = 256;
static const char NULL_KEY_NOTICE[]
   = "ossimKeywordlist Notice:  Null key passed to method.";


#ifdef OSSIM_ID_ENABLED
static const bool TRACE = false;
static const char OSSIM_ID[] = "$Id: ossimKeywordlist.cpp,v 1.62 2005/10/17 19:16:33 gpotts Exp $";
#endif

ossimKeywordlist::ossimKeywordlist(char delimiter)
   :
      theMap(),
      theDelimiter(delimiter)
{
#ifdef OSSIM_ID_ENABLED
   if (TRACE) ossimNotify(ossimNotifyLevel_DEBUG) << OSSIM_ID << endl;
#endif
}

ossimKeywordlist::ossimKeywordlist(const char* file,
                         char        delimiter,
                         bool        ignoreBinaryChars)
   :
      theMap(),
      theDelimiter(delimiter)
{
   ossimFilename in_file(file);

   if (!parseFile(in_file, ignoreBinaryChars))
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
   }
}

ossimKeywordlist::ossimKeywordlist(const ossimFilename& file,
                         char            delimiter,
                         bool            ignoreBinaryChars)
   :
      theMap(),
      theDelimiter(delimiter)

{
   if (!parseFile(file, ignoreBinaryChars))
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
   }
}

ossimKeywordlist::~ossimKeywordlist()
{
   theMap.clear();
}

bool ossimKeywordlist::addFile(const char* file)
{
   ossimFilename in_file(file);

   return parseFile(in_file);
}

bool ossimKeywordlist::addFile(const ossimFilename& file)
{
   return parseFile(file);
}

void ossimKeywordlist::addList(const ossimKeywordlist &src, bool overwrite)
{
    KeywordMap::const_iterator i = src.theMap.begin();

    while (i != src.theMap.end())
    {
        add( (*i).first, (*i).second, overwrite );
        i++;
    }
}

void ossimKeywordlist::add(const ossimKeywordlist& kwl,
                           const char* prefix,
                           bool stripPrefix)
{
   map<ossimString, ossimString>::const_iterator iter = kwl.theMap.begin();

   ossimRegExp regExp;

   regExp.compile(("^("+ossimString(prefix)+")").c_str());

   while(iter != kwl.theMap.end())
   {
      ossimString newKey;

      if(regExp.find( (*iter).first.c_str()))
      {
         newKey = (*iter).first;
         if(stripPrefix)
         {
            newKey = newKey.substitute(prefix,
                                       "");

         }

         add(newKey,
             (*iter).second,
            true);
      }
      ++iter;
   }

}

void ossimKeywordlist::add(const char* prefix,
                           const ossimKeywordlist& kwl,
                           bool overwrite)
{
   map<ossimString, ossimString>::const_iterator iter = kwl.theMap.begin();

   while(iter != kwl.theMap.end())
   {
      ossimString valueToAdd = (*iter).second;
      ossimString newKey     = (*iter).first;

      add(prefix,
          newKey,
          valueToAdd,
          overwrite);
      ++iter;
   }
}

void ossimKeywordlist::add(const char* key,
                           const char* value,
                           bool        overwrite)
{
   if (key)
   {
      ossimString k = key;

      ossimString v;
      if (value) v = value;

      KeywordMap::iterator i = getMapEntry(k);

      if (i == theMap.end())
      {
         theMap.insert(std::make_pair(k, v));
      }
      else if (overwrite)
      {
         (*i).second = v;
      }
   }
}

void ossimKeywordlist::add(const char* prefix,
                           const char* key,
                           const char* value,
                           bool        overwrite)
{
   if (key)
   {
      ossimString k;
      if (prefix) k = prefix;
      k += key;

      ossimString v;
      if (value) v = value;

      KeywordMap::iterator i = getMapEntry(k);

      if(i == theMap.end())
      {
         theMap.insert(std::make_pair(k, v));
      }
      else if(overwrite)
      {
         (*i).second = v;
      }
   }
}

void ossimKeywordlist::add(const char* key,
                           char        value,
                           bool        overwrite)
{
   if (key)
   {
      ossimString k = key;

      char tmp[2];
      tmp[0] = value;
      tmp[1] = '\0';
      ossimString v = tmp;

      KeywordMap::iterator i = getMapEntry(k);

      if (i == theMap.end())
      {
         theMap.insert(std::make_pair(k, v));
      }
      else if (overwrite)
      {
         (*i).second = v;
      }
   }
}

void ossimKeywordlist::add(const char* prefix,
                           const char* key,
                           char        value,
                           bool        overwrite)
{
   if (key)
   {
      ossimString k;
      if (prefix) k = prefix;
      k += key;

      char tmp[2];
      tmp[0] = value;
      tmp[1] = '\0';
      ossimString v = tmp;

      KeywordMap::iterator i = getMapEntry(k);

      if(i == theMap.end())
      {
         theMap.insert(std::make_pair(k, v));
      }
      else if(overwrite)
      {
         (*i).second = v;
      }
   }
}

void ossimKeywordlist::add(const char* key,
                           ossim_int16 value,
                           bool overwrite)
{
   if (key)
   {
      ossimString k = key;

      ossimString v = ossimString::toString(value);

      KeywordMap::iterator i = getMapEntry(k);

      if(i == theMap.end())
      {
         theMap.insert(std::make_pair(k, v));
      }
      else if (overwrite)
      {
         (*i).second = v;
      }
   }
}

void ossimKeywordlist::add(const char* prefix,
                           const char* key,
                           ossim_int16 value,
                           bool overwrite)
{
   if (key)
   {
      ossimString k;
      if (prefix) k = prefix;
      k += key;

      ossimString v = ossimString::toString(value);

      KeywordMap::iterator i = getMapEntry(k);

      if(i == theMap.end())
      {
         theMap.insert(std::make_pair(k, v));
      }
      else if (overwrite)
      {
         (*i).second = v;
      }
   }
}

void ossimKeywordlist::add(const char* key,
                           ossim_uint16 value,
                           bool overwrite)
{
   if (key)
   {
      ossimString k = key;

      ossimString v = ossimString::toString(value);

      KeywordMap::iterator i = getMapEntry(k);

      if(i == theMap.end())
      {
         theMap.insert(std::make_pair(k, v));
      }
      else if (overwrite)
      {
         (*i).second = v;
      }
   }
}

void ossimKeywordlist::add(const char* prefix,
                           const char* key,
                           ossim_uint16 value,
                           bool overwrite)
{
   if (key)
   {
      ossimString k;
      if (prefix) k = prefix;
      k += key;

      ossimString v = ossimString::toString(value);

      KeywordMap::iterator i = getMapEntry(k);

      if(i == theMap.end())
      {
         theMap.insert(std::make_pair(k, v));
      }
      else if (overwrite)
      {
         (*i).second = v;
      }
   }
}

void ossimKeywordlist::add(const char* key,
                           ossim_int32 value,
                           bool overwrite)
{
   if (key)
   {
      ossimString k = key;

      ossimString v = ossimString::toString(value);

      KeywordMap::iterator i = getMapEntry(k);

      if(i == theMap.end())
      {
         theMap.insert(std::make_pair(k, v));
      }
      else if (overwrite)
      {
         (*i).second = v;
      }
   }
}

void ossimKeywordlist::add(const char*  prefix,
                           const char*  key,
                           ossim_int32 value,
                           bool overwrite)
{
   if (key)
   {
      ossimString k;
      if (prefix) k = prefix;
      k += key;

      ossimString v = ossimString::toString(value);

      KeywordMap::iterator i = getMapEntry(k);

      if(i == theMap.end())
      {
         theMap.insert(std::make_pair(k, v));
      }
      else if (overwrite)
      {
         (*i).second = v;
      }
   }
}

void ossimKeywordlist::add(const char* key,
                           ossim_uint32 value,
                           bool overwrite)
{
   if (key)
   {
      ossimString k = key;

      ossimString v = ossimString::toString(value);

      KeywordMap::iterator i = getMapEntry(k);

      if(i == theMap.end())
      {
         theMap.insert(std::make_pair(k, v));
      }
      else if (overwrite)
      {
         (*i).second = v;
      }
   }
}

void ossimKeywordlist::add(const char*  prefix,
                           const char*  key,
                           ossim_uint32 value,
                           bool overwrite)
{
   if (key)
   {
      ossimString k;
      if (prefix) k = prefix;
      k += key;

      ossimString v = ossimString::toString(value);

      KeywordMap::iterator i = getMapEntry(k);

      if(i == theMap.end())
      {
         theMap.insert(std::make_pair(k, v));
      }
      else if (overwrite)
      {
         (*i).second = v;
      }
   }
}

void ossimKeywordlist::add(const char* key,
                           long value,
                           bool overwrite)
{
   if (key)
   {
      ossimString k = key;

      ossimString v = ossimString::toString(value);

      KeywordMap::iterator i = getMapEntry(k);

      if(i == theMap.end())
      {
         theMap.insert(std::make_pair(k, v));
      }
      else if (overwrite)
      {
         (*i).second = v;
      }
   }
}

void ossimKeywordlist::add(const char* prefix,
                           const char* key,
                           long value,
                           bool overwrite)
{
   if (key)
   {
      ossimString k;
      if (prefix) k = prefix;
      k += key;

      ossimString v = ossimString::toString(value);

      KeywordMap::iterator i = getMapEntry(k);

      if(i == theMap.end())
      {
         theMap.insert(std::make_pair(k, v));
      }
      else if (overwrite)
      {
         (*i).second = v;
      }
   }
}

void ossimKeywordlist::add(const char* key,
                           unsigned long value,
                           bool overwrite)
{
   if (key)
   {
      ossimString k = key;

      ossimString v = ossimString::toString(value);

      KeywordMap::iterator i = getMapEntry(k);

      if(i == theMap.end())
      {
         theMap.insert(std::make_pair(k, v));
      }
      else if (overwrite)
      {
         (*i).second = v;
      }
   }
}

void ossimKeywordlist::add(const char* prefix,
                           const char* key,
                           unsigned long value,
                           bool overwrite)
{
   if (key)
   {
      ossimString k;
      if (prefix) k = prefix;
      k += key;

      ossimString v = ossimString::toString(value);

      KeywordMap::iterator i = getMapEntry(k);

      if(i == theMap.end())
      {
         theMap.insert(std::make_pair(k, v));
      }
      else if (overwrite)
      {
         (*i).second = v;
      }
   }
}

void ossimKeywordlist::add(const char* key,
                           ossim_float32 value,
                           bool overwrite,
                           int precision,
                           bool trimZeroFlag,
                           bool scientific)
{
   if (key)
   {
      ossimString k = key;

      ossimString v = ossimString::toString(value,
                                            precision,
                                            trimZeroFlag,
                                            scientific);

      KeywordMap::iterator i = getMapEntry(k);

      if(i == theMap.end())
      {
         theMap.insert(std::make_pair(k, v));
      }
      else if (overwrite)
      {
         (*i).second = v;
      }
   }
}

void ossimKeywordlist::add(const char* prefix,
                           const char* key,
                           ossim_float32 value,
                           bool overwrite,
                           int precision,
                           bool trimZeroFlag,
                           bool scientific)
{
   if (key)
   {
      ossimString k;
      if (prefix) k = prefix;
      k += key;

      ossimString v = ossimString::toString(value,
                                            precision,
                                            trimZeroFlag,
                                            scientific);

      KeywordMap::iterator i = getMapEntry(k);

      if(i == theMap.end())
      {
         theMap.insert(std::make_pair(k, v));
      }
      else if (overwrite)
      {
         (*i).second = v;
      }
   }
}

void ossimKeywordlist::add(const char* key,
                           ossim_float64 value,
                           bool overwrite,
                           int precision,
                           bool trimZeroFlag,
                           bool scientific)
{
   if (key)
   {
      ossimString k = key;

      ossimString v = ossimString::toString(value,
                                            precision,
                                            trimZeroFlag,
                                            scientific);

      KeywordMap::iterator i = getMapEntry(k);

      if(i == theMap.end())
      {
         theMap.insert(std::make_pair(k, v));
      }
      else if (overwrite)
      {
         (*i).second = v;
      }
   }
}

void ossimKeywordlist::add(const char* prefix,
                           const char* key,
                           ossim_float64 value,
                           bool overwrite,
                           int precision,
                           bool trimZeroFlag,
                           bool scientific)
{
   if (key)
   {
      ossimString k;
      if (prefix) k = prefix;
      k += key;

      ossimString v = ossimString::toString(value,
                                            precision,
                                            trimZeroFlag,
                                            scientific);

      KeywordMap::iterator i = getMapEntry(k);

      if(i == theMap.end())
      {
         theMap.insert(std::make_pair(k, v));
      }
      else if (overwrite)
      {
         (*i).second = v;
      }
   }
}

bool ossimKeywordlist::write(const char* file) const
{
   ofstream filename(file);
   if (!filename)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         <<"ossimKeywordlist::write, Error opening file:  "
         << file << std::endl;
      return false;
   }

   writeToStream(filename);

   filename.close();

   return true;
}

ossimString ossimKeywordlist::toString()const
{
   KeywordMap::const_iterator i;
   ossimString result;

   for(i = theMap.begin(); i != theMap.end(); i++)
   {
      result += (*i).first;
      result += delimiter_str().c_str();
      result += " ";
      result += (*i).second;
      result += '\n';
   }

   return result;
}

void ossimKeywordlist::toString(ossimString& result)const
{
   KeywordMap::const_iterator i;

   for(i = theMap.begin(); i != theMap.end(); i++)
   {
      result += (*i).first;
      result += delimiter_str().c_str();
      result += " ";
      result += (*i).second;
      result += '\n';
   }
}

void ossimKeywordlist::writeToStream(ostream& out) const
{
   KeywordMap::const_iterator i;

   for(i = theMap.begin(); i != theMap.end(); ++i)
   {
      out << (*i).first  << delimiter_str().c_str() << "  "
          << (*i).second << endl;
   }
}


const char* ossimKeywordlist::find(const char* key) const
{
   const char* result = NULL;

   if (key)
   {
      ossimString k = key;

      KeywordMap::const_iterator i = theMap.find(k.c_str());

      if (i != theMap.end())
      {
         result = (*i).second.c_str();
      }
   }

   return result;
}

const char* ossimKeywordlist::find(const char* prefix,
                              const char* key) const
{
   const char* result = NULL;

   if (key)
   {
      ossimString k;
      if (prefix) k = prefix;
      k += key;
      KeywordMap::const_iterator i = theMap.find(k.c_str());
      if (i != theMap.end())
      {
         result = (*i).second.c_str();
      }
   }

   return result;
}


void ossimKeywordlist::remove(const char * key)
{
   ossimString k = key?key:"";

   KeywordMap::iterator i = theMap.find(k);

   if(i != theMap.end())
   {
      theMap.erase(i);
   }
}

void ossimKeywordlist::remove(const char* prefix, const char * key)
{
   if (key)
   {
      ossimString k;
      if (prefix) k = prefix;
      k += key;

      KeywordMap::iterator i = theMap.find(k);

      if(i != theMap.end())
      {
         theMap.erase(i);
      }
   }
}

ossim_uint32 ossimKeywordlist::numberOf(const char* str) const
{
   ossim_uint32 count = 0;

   if (str)
   {
      KeywordMap::const_iterator i = theMap.begin();

      while (i != theMap.end())
      {
         if ( (*i).first.contains(str) )
         {
            ++count;
         }
         ++i;
      }
   }

   return count;
}

ossim_uint32 ossimKeywordlist::numberOf(const char* prefix,
                                       const char* key) const
{
   if (!prefix && key)  // Must have key, sometimes no prefix.
   {
      return numberOf(key);
   }

   ossim_uint32 count = 0;

   if (prefix && key)
   {
      KeywordMap::const_iterator i = theMap.begin();

      while (i != theMap.end())
      {
         if ( (*i).first.contains(prefix) && (*i).first.contains(key))
         {
            ++count;
         }
         ++i;
      }
   }

   return count;
}

void ossimKeywordlist::clear()
{
   theMap.clear();
}

ossimKeywordlist::KeywordMap::iterator
ossimKeywordlist::getMapEntry(const char* key)
{
   if (key)
   {
      ossimString k = key;
      return theMap.find(k);
   }
   else
   {
      return theMap.end();
   }
}

ossimKeywordlist::KeywordMap::iterator
ossimKeywordlist::getMapEntry(const ossimString& key)
{
   return theMap.find(key);
}


//*******************************************************************
// Private Method:
//*******************************************************************
bool ossimKeywordlist::parseFile(const ossimFilename& file,
                                 bool ignoreBinaryChars)
{
   std::ifstream is;
   is.open(file.c_str(), ios::in | ios::binary);

   if (!is)
   {
      if(traceDebug())
      {
         // report all errors that aren't existance problems.
         // we want to know about things like permissions, too many open files, etc.
         ossimNotify(ossimNotifyLevel_DEBUG) << "Error opening file: " << file.c_str() 
                                             << std::endl;
      }
      return false;
   }

   bool result = parseStream(is, ignoreBinaryChars);

   is.close();

   return result;
}

bool ossimKeywordlist::parseStream(istream &is,
                                   bool ignoreBinaryChars)
{
   ossimString line;

   while(!is.eof() && is.good())
   {
      line  = "";
      // get a line and parse it
      char c = is.get();
      while((c != '\n') &&!is.eof()&&!is.bad())
      {
         if (c >= 0x20)
         {
            if( (c > 0x7e) )
            {
	      return false;
            }
            else if (c <= 0x7e)
            {

               line += c;
            }
            else
            {
               line += c;
            }
         }

         c = is.get();
      }

      if(line != "")
      {
         ossimString key;
         ossimString value;

         // parse out key and value
         ossim_uint32 pos = line.find(theDelimiter);
         key = line.substr(0, pos);
         key = key.trim();
         value = line.substr(pos + 1);
         value = value.trim();

         // if it's not a comment then add it
         if(key.begin()[0] != (char)'/' ||
            key.begin()[1] != (char)'/')
         {
            if(pos != line.npos)
            {
               theMap.insert(std::make_pair(key, value));
            }
         }
      }
   }

   return true;
}

vector<ossimString> ossimKeywordlist::findAllKeysThatContains(const ossimString &searchString)const
{
   KeywordMap::const_iterator i;
   vector<ossimString> result;

   for(i = theMap.begin(); i != theMap.end(); ++i)
   {
      if((*i).first.contains(searchString))
      {
         result.push_back((*i).first);
      }
   }

   return result;
}

vector<ossimString> ossimKeywordlist::findAllKeysThatMatch(const ossimString &regularExpression)const
{
   KeywordMap::const_iterator i;
   vector<ossimString> result;
   ossimRegExp regExp;

   regExp.compile(regularExpression.c_str());

   for(i = theMap.begin(); i != theMap.end(); ++i)
   {
      if(regExp.find( (*i).first.c_str()))
      {
         result.push_back((*i).first);
      }
   }

   return result;
}

void ossimKeywordlist::extractKeysThatMatch(ossimKeywordlist& kwl,
                                            const ossimString &regularExpression)const
{
   KeywordMap::const_iterator i;
   vector<ossimString> result;
   ossimRegExp regExp;

   regExp.compile(regularExpression.c_str());

   for(i = theMap.begin(); i != theMap.end(); ++i)
   {
      if(regExp.find( (*i).first.c_str()))
      {
         kwl.add((*i).first, (*i).second);
      }
   }
}

void ossimKeywordlist::removeKeysThatMatch(const ossimString &regularExpression)
{
   KeywordMap::const_iterator i;
   vector<ossimString> result;
   ossimRegExp regExp;

   regExp.compile(regularExpression.c_str());

   for(i = theMap.begin(); i != theMap.end(); ++i)
   {
      if(regExp.find( (*i).first.c_str()))
      {
         result.push_back((*i).first);
      }
   }
   for(ossim_uint32 i2 = 0; i2 < result.size(); ++i2)
   {
      remove(result[i2]);
   }
}

vector<ossimString> ossimKeywordlist::getSubstringKeyList(const ossimString& regularExpression)const
{
   KeywordMap::const_iterator i;
   vector<ossimString> result;
   ossimRegExp regExp;

   regExp.compile(regularExpression.c_str());

   for(i = theMap.begin(); i != theMap.end(); ++i)
   {
      if(regExp.find( (*i).first.c_str()))
      {
         ossimString value = ossimString((*i).first.begin()+regExp.start(),
                                         (*i).first.begin()+regExp.start()+regExp.end());

         if(std::find(result.begin(), result.end(), value) == result.end())
         {
            result.push_back(value);
         }
      }
   }

   return result;
}

ossim_uint32 ossimKeywordlist::getNumberOfSubstringKeys(const ossimString& regularExpression)const
{
   KeywordMap::const_iterator i;
   list<ossimString> currentList;
   ossim_uint32 result = 0;
   ossimRegExp regExp;

   regExp.compile(regularExpression.c_str());

   for(i = theMap.begin(); i != theMap.end(); ++i)
   {
      if(regExp.find( (*i).first.c_str()))
      {
         ossimString value = ossimString((*i).first.begin()+regExp.start(),
                                         (*i).first.begin()+regExp.start()+regExp.end());

         // make sure we have not already marked it as
         // found
         if(std::find(currentList.begin(),
                      currentList.end(),
                      value) == currentList.end())
         {
            currentList.push_back(value);
            ++result;
         }
      }
   }

   return result;
}

void ossimKeywordlist::addPrefixToAll(const ossimString& prefix)
{
   ossimKeywordlist tempKwl = *this;

   clear();

   KeywordMap::const_iterator values = tempKwl.theMap.begin();

   while(values != tempKwl.theMap.end())
   {
      ossimString newKey = prefix + (*values).first;
      add(newKey,
          (*values).second,
          true);
      ++values;
   }
}

void ossimKeywordlist::stripPrefixFromAll(const ossimString& regularExpression)
{
   ossimKeywordlist tempKwl = *this;

   clear();

   KeywordMap::const_iterator values = tempKwl.theMap.begin();
   ossimRegExp regExp;

   regExp.compile(regularExpression.c_str());

   while(values != tempKwl.theMap.end())
   {
      ossimString newKey = (*values).first;
      if(regExp.find( (*values).first.c_str()))
      {
         newKey.erase(newKey.begin()+regExp.start(),
		      newKey.begin()+regExp.start()+regExp.end());

         add(newKey.c_str(),
             (*values).second,
             true);
      }
      else
      {
         add(newKey.c_str(),
             (*values).second,
             true);
      }
      ++values;
   }
}


ossim_uint32 ossimKeywordlist::getSize()const
{
   return theMap.size();
}

const ossimKeywordlist::KeywordMap& ossimKeywordlist::getMap()const
{
   return theMap;
}

ossimKeywordlist::KeywordMap& ossimKeywordlist::getMap()
{
   return theMap;
}

ossimString ossimKeywordlist::delimiter_str() const
{
   char tmp[2];
   tmp[0] = theDelimiter;
   tmp[1] = '\0';
   return ossimString(tmp);
}

//*******************************************************************
// Public Method:
//*******************************************************************
ostream& ossimKeywordlist::print(ostream& os) const
{
   KeywordMap::const_iterator i;

   for(i = theMap.begin(); i != theMap.end(); ++i)
   {
      os << (*i).first << delimiter_str().c_str() << "  "
         << (*i).second << endl;
   }
   return os;
}

//*******************************************************************
// friend function:
//*******************************************************************
OSSIMDLLEXPORT ostream& operator<<(ostream& os, const ossimKeywordlist& kwl)
{
   kwl.print(os);

   return os;
}


bool ossimKeywordlist::operator ==(ossimKeywordlist& kwl)const
{
   if(this==&kwl) return true;
   map<ossimString, ossimString>::const_iterator iter = theMap.begin();

   while(iter != theMap.end())
   {
      const char* value = kwl.find((*iter).first);

      if(ossimString(value) != (*iter).second)
      {
         return false;
      }
      ++iter;
   }

   return true;
}

void ossimKeywordlist::downcaseKeywords()
{
   KeywordMap tempMap;
   KeywordMap::iterator iter = theMap.begin();

   while(iter != theMap.end())
   {
      tempMap.insert(make_pair(ossimString::downcase(iter->first),
                               iter->second));
                               
      ++iter;
   }
   theMap = tempMap;
}

void ossimKeywordlist::upcaseKeywords()
{
   KeywordMap tempMap;
   KeywordMap::iterator iter = theMap.begin();

   while(iter != theMap.end())
   {
      tempMap.insert(make_pair(ossimString::upcase(iter->first),
                               iter->second));
                               
      ++iter;
   }
   theMap = tempMap;
}

