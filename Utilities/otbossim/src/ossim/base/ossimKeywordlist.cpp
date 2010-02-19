//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Description: This class provides capabilities for keywordlists.
//
//********************************************************************
// $Id: ossimKeywordlist.cpp 16563 2010-02-08 22:50:27Z eshirschorn $
#include <algorithm>
#include <fstream>
#include <list>
#include <sstream>

#include <ossim/base/ossimRegExp.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimDirectory.h>

static ossimTrace traceDebug("ossimKeywordlist:debug");
static const ossim_int32 MAX_LINE_LENGTH = 256;
static const char NULL_KEY_NOTICE[]
= "ossimKeywordlist Notice:  Null key passed to method.";


#ifdef OSSIM_ID_ENABLED
static const bool TRACE = false;
static const char OSSIM_ID[] = "$Id: ossimKeywordlist.cpp 16563 2010-02-08 22:50:27Z eshirschorn $";
#endif

ossimKeywordlist::ossimKeywordlist(char delimiter, 
                                   bool expandEnvVars)
:
m_map(),
m_delimiter(delimiter),
m_lineContinuationCharacter('\\'),
m_expandEnvVars(expandEnvVars)
{
#ifdef OSSIM_ID_ENABLED
   if (TRACE) ossimNotify(ossimNotifyLevel_DEBUG) << OSSIM_ID << std::endl;
#endif
}

ossimKeywordlist::ossimKeywordlist(const char* file,
                                   char        delimiter,
                                   bool        ignoreBinaryChars,
                                   bool        expandEnvVars)
:
m_map(),
m_delimiter(delimiter),
m_lineContinuationCharacter('\\'),
m_expandEnvVars(expandEnvVars)
{
   ossimFilename in_file(file);
   
   if (!parseFile(in_file, ignoreBinaryChars))
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
   }
}

ossimKeywordlist::ossimKeywordlist(const ossimFilename& file,
                                   char                 delimiter,
                                   bool                 ignoreBinaryChars,
                                   bool                 expandEnvVars)
:
m_map(),
m_delimiter(delimiter),
m_lineContinuationCharacter('\\'),
m_expandEnvVars(expandEnvVars)

{
   if (!parseFile(file, ignoreBinaryChars))
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
   }
}

ossimKeywordlist::~ossimKeywordlist()
{
   m_map.clear();
}

void ossimKeywordlist::setExpandEnvVarsFlag( bool flag )
{
   m_expandEnvVars = flag;
}

bool ossimKeywordlist::getExpandEnvVarsFlag( void ) const
{
   return m_expandEnvVars;
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
   KeywordMap::const_iterator i = src.m_map.begin();
   
   while (i != src.m_map.end())
   {
      add( (*i).first, (*i).second, overwrite );
      i++;
   }
}

void ossimKeywordlist::add(const ossimKeywordlist& kwl,
                           const char* prefix,
                           bool stripPrefix)
{
   std::map<ossimString, ossimString>::const_iterator iter = kwl.m_map.begin();
   
   ossimRegExp regExp;
   
   // Check for null prefix.
   ossimString tmpPrefix;
   if (prefix) tmpPrefix = prefix;
   
   regExp.compile(("^("+tmpPrefix+")").c_str());
   
   while(iter != kwl.m_map.end())
   {
      ossimString newKey;
      
      if(regExp.find( (*iter).first.c_str()))
      {
         newKey = (*iter).first;
         if(stripPrefix && prefix)
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
   std::map<ossimString, ossimString>::const_iterator iter = kwl.m_map.begin();
   
   while(iter != kwl.m_map.end())
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
      if ( m_expandEnvVars == true )
      {
         v = v.expandEnvironmentVariable();
      }
      
      KeywordMap::iterator i = getMapEntry(k);
      
      if (i == m_map.end())
      {
         m_map.insert(std::make_pair(k, v));
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
      if ( m_expandEnvVars == true )
      {
         v = v.expandEnvironmentVariable();
      }
      
      KeywordMap::iterator i = getMapEntry(k);
      
      if(i == m_map.end())
      {
         m_map.insert(std::make_pair(k, v));
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
      
      if (i == m_map.end())
      {
         m_map.insert(std::make_pair(k, v));
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
      
      if(i == m_map.end())
      {
         m_map.insert(std::make_pair(k, v));
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
      
      if(i == m_map.end())
      {
         m_map.insert(std::make_pair(k, v));
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
      
      if(i == m_map.end())
      {
         m_map.insert(std::make_pair(k, v));
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
      
      if(i == m_map.end())
      {
         m_map.insert(std::make_pair(k, v));
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
      
      if(i == m_map.end())
      {
         m_map.insert(std::make_pair(k, v));
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
      
      if(i == m_map.end())
      {
         m_map.insert(std::make_pair(k, v));
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
      
      if(i == m_map.end())
      {
         m_map.insert(std::make_pair(k, v));
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
      
      if(i == m_map.end())
      {
         m_map.insert(std::make_pair(k, v));
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
      
      if(i == m_map.end())
      {
         m_map.insert(std::make_pair(k, v));
      }
      else if (overwrite)
      {
         (*i).second = v;
      }
   }
}

void ossimKeywordlist::add(const char* key,
                           ossim_int64 value,
                           bool overwrite)
{
   if (key)
   {
      ossimString k = key;
      
      ossimString v = ossimString::toString(value);
      
      KeywordMap::iterator i = getMapEntry(k);
      
      if(i == m_map.end())
      {
         m_map.insert(std::make_pair(k, v));
      }
      else if (overwrite)
      {
         (*i).second = v;
      }
   }
}

void ossimKeywordlist::add(const char*  prefix,
                           const char*  key,
                           ossim_int64 value,
                           bool overwrite)
{
   if (key)
   {
      ossimString k;
      if (prefix) k = prefix;
      k += key;
      
      ossimString v = ossimString::toString(value);
      
      KeywordMap::iterator i = getMapEntry(k);
      
      if(i == m_map.end())
      {
         m_map.insert(std::make_pair(k, v));
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
                           int precision)
{
   if (key)
   {
      ossimString k = key;
      
      ossimString v = ossimString::toString(value,
                                            precision);
      
      KeywordMap::iterator i = getMapEntry(k);
      
      if(i == m_map.end())
      {
         m_map.insert(std::make_pair(k, v));
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
                           int precision)
{
   if (key)
   {
      ossimString k;
      if (prefix) k = prefix;
      k += key;
      
      ossimString v = ossimString::toString(value,
                                            precision);
      
      KeywordMap::iterator i = getMapEntry(k);
      
      if(i == m_map.end())
      {
         m_map.insert(std::make_pair(k, v));
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
                           int precision)
{
   if (key)
   {
      ossimString k = key;
      
      ossimString v = ossimString::toString(value,
                                            precision);
      
      KeywordMap::iterator i = getMapEntry(k);
      
      if(i == m_map.end())
      {
         m_map.insert(std::make_pair(k, v));
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
                           int precision)
{
   if (key)
   {
      ossimString k;
      if (prefix) k = prefix;
      k += key;
      
      ossimString v = ossimString::toString(value,
                                            precision);
      
      KeywordMap::iterator i = getMapEntry(k);
      
      if(i == m_map.end())
      {
         m_map.insert(std::make_pair(k, v));
      }
      else if (overwrite)
      {
         (*i).second = v;
      }
   }
}

bool ossimKeywordlist::write(const char* file, 
                             const char* comment) const
{
   std::ofstream filename(file);
   if (!filename)
   {
      ossimNotify(ossimNotifyLevel_WARN)
      <<"ossimKeywordlist::write, Error opening file:  "
      << file << std::endl;
      return false;
   }

   if ( comment != 0 )
   {
      ossimString commentStr("// ");
      commentStr += comment;

      // Write out the input comment to the first line.
      filename << commentStr.c_str() << std::endl;
   }

   writeToStream(filename);
   
   filename.close();
   
   return true;
}

ossimString ossimKeywordlist::toString()const
{
   std::ostringstream out;
   writeToStream(out);
   
#if 0
   KeywordMap::const_iterator i;
   ossimString result;
   
   for(i = m_map.begin(); i != m_map.end(); i++)
   {
      result += (*i).first;
      result += delimiter_str().c_str();
      result += " ";
      result += (*i).second;
      result += '\n';
   }
#endif
   return ossimString(out.str());
}

void ossimKeywordlist::toString(ossimString& result)const
{
   std::ostringstream out;
   writeToStream(out);
   
   result = out.str();
#if 0
   KeywordMap::const_iterator i;
   
   for(i = m_map.begin(); i != m_map.end(); i++)
   {
      result += (*i).first;
      result += delimiter_str().c_str();
      result += " ";
      result += (*i).second;
      result += '\n';
   }
#endif
}

void ossimKeywordlist::writeToStream(std::ostream& out) const
{
   KeywordMap::const_iterator i;
   ossimString value;
   for(i = m_map.begin(); i != m_map.end(); ++i)
   {
      value = (*i).second;
      value = value.substitute('\n', "\\\n", true);
      out << (*i).first  << delimiter_str().c_str() << "  "
      << value << std::endl;
   }
}


const char* ossimKeywordlist::find(const char* key) const
{
   const char* result = NULL;
   
   if (key)
   {
      ossimString k = key;
      
      KeywordMap::const_iterator i = m_map.find(k.c_str());
      
      if (i != m_map.end())
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
      KeywordMap::const_iterator i = m_map.find(k.c_str());
      if (i != m_map.end())
      {
         result = (*i).second.c_str();
      }
   }
   
   return result;
}


void ossimKeywordlist::remove(const char * key)
{
   ossimString k = key?key:"";
   
   KeywordMap::iterator i = m_map.find(k);
   
   if(i != m_map.end())
   {
      m_map.erase(i);
   }
}

void ossimKeywordlist::remove(const char* prefix, const char * key)
{
   if (key)
   {
      ossimString k;
      if (prefix) k = prefix;
      k += key;
      
      KeywordMap::iterator i = m_map.find(k);
      
      if(i != m_map.end())
      {
         m_map.erase(i);
      }
   }
}

ossim_uint32 ossimKeywordlist::numberOf(const char* str) const
{
   ossim_uint32 count = 0;
   
   if (str)
   {
      KeywordMap::const_iterator i = m_map.begin();
      
      while (i != m_map.end())
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
      KeywordMap::const_iterator i = m_map.begin();
      
      while (i != m_map.end())
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
   m_map.clear();
}

ossimKeywordlist::KeywordMap::iterator
ossimKeywordlist::getMapEntry(const char* key)
{
   if (key)
   {
      ossimString k = key;
      return m_map.find(k);
   }
   else
   {
      return m_map.end();
   }
}

ossimKeywordlist::KeywordMap::iterator
ossimKeywordlist::getMapEntry(const ossimString& key)
{
   return m_map.find(key);
}


//*******************************************************************
// Private Method:
//*******************************************************************
bool ossimKeywordlist::parseFile(const ossimFilename& file,
                                 bool ignoreBinaryChars)
{
   std::ifstream is;
   is.open(file.c_str(), std::ios::in | std::ios::binary);

   if ( !is.is_open() )
   {
      // ESH 07/2008, Trac #234: OSSIM is case sensitive 
      // when using worldfile templates during ingest
      // -- If first you don't succeed with the user-specified
      // filename, try again with the results of a case insensitive search.
      ossimDirectory directory(file.path());
      ossimFilename filename(file.file());

      std::vector<ossimFilename> result;
      bool bSuccess = directory.findCaseInsensitiveEquivalents( filename, result );
      if ( bSuccess == true )
      {
         int numResults = (int)result.size();
         int i;
         for ( i=0; i<numResults && !is.is_open(); ++i )
         {
            is.open( result[i].c_str(), std::ios::in | std::ios::binary );
         }
      }

      if ( !is.is_open() )
      {
         if ( traceDebug() ) 
         {
            // report all errors that aren't existence problems.
            // we want to know about things like permissions, too many open files, etc.
            ossimNotify(ossimNotifyLevel_DEBUG)
            << "Error opening file: " << file.c_str() << std::endl;
         }
         return false;
      }
   }

   bool result = parseStream(is, ignoreBinaryChars);

   is.close();

   return result;
}

bool ossimKeywordlist::parseStream(std::istream& is,
                                   bool ignoreBinaryChars)
{
   if (!is) // Check stream state.
   {
      return false;
   }
   
   ossimString line;
   
   while(!is.eof() && is.good())
   {
      line  = "";
      // get a line and parse it
      ossim_uint8 c = is.get();
      while(!is.eof()&&!is.bad())
      {
         if(c == m_lineContinuationCharacter)
         {
            char peekChar = is.peek();
            if(peekChar == '\n')
            {
               line += (char)is.get();
            }
            else
            {
               line += (char)c;
            }
         }
         else if(c == '\n')
         {
            break;
         }
         else if(c >= 0x20)
         {
            if( (c > 0x7e) )
            {
               return false;
            }
            else if (c <= 0x7e)
            {
               
               line += (char)c;
            }
            else
            {
               line += (char)c;
            }
         }
         c = is.get();
      }
      
      if(line != "")
      {
         // Find the delimiter position.
         std::string::size_type pos = line.find(m_delimiter);
         
         if(pos != line.npos)
         {
            // Found delimiter, get the key.
            ossimString key = line.substr(0, pos);
            key = key.trim();
            
            if ( key.size() ) // Could have a one letter key like: 'a'
            {
               bool addPair = true;
               
               // ESH 06/2008 --
               // Make sure the locations exist before accessing.
               if ( key.size() > 1 ) // Check for c++ comment "//".
               {
                  //---
                  // Might want to require beginning character to be an alpha?
                  // For now only blocked if c++ comment.  (drb)
                  //---
                  if ( (key.begin()[0] == (char)'/') &&
                      (key.begin()[1] == (char)'/') )
                  {
                     addPair = false;
                  }
               }

               if ( addPair )
               {
                  // Get value.
                  ossimString value = line.substr(pos + 1);
                  value = value.trim();

                  // ESH: EarthWhere ticket #656
                  // The ossim_prefs file can now use environment variables. 
                  // The ossimPreferences class enables this feature for its 
                  // internal keywordlist.
                  if ( m_expandEnvVars == true )
                  {
                     ossimString result = value.expandEnvironmentVariable();
                     m_map.insert(std::make_pair(key, result));
                  }
                  else
                  {
                     m_map.insert(std::make_pair(key, value));
                  }
               }
            }
            
         } // End of:  if(pos != line.npos)
         
      } // End of:  if(line != "")
      
   } // End of while(!is.eof() && is.good())
   
   return true;
}

std::vector<ossimString> ossimKeywordlist::findAllKeysThatContains(
                                                                   const ossimString &searchString)const
{
   KeywordMap::const_iterator i;
   std::vector<ossimString> result;
   
   for(i = m_map.begin(); i != m_map.end(); ++i)
   {
      if((*i).first.contains(searchString))
      {
         result.push_back((*i).first);
      }
   }
   
   return result;
}

std::vector<ossimString> ossimKeywordlist::findAllKeysThatMatch(
                                                                const ossimString &regularExpression)const
{
   KeywordMap::const_iterator i;
   std::vector<ossimString> result;
   ossimRegExp regExp;
   
   regExp.compile(regularExpression.c_str());
   
   for(i = m_map.begin(); i != m_map.end(); ++i)
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
   std::vector<ossimString> result;
   ossimRegExp regExp;
   
   regExp.compile(regularExpression.c_str());
   
   for(i = m_map.begin(); i != m_map.end(); ++i)
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
   std::vector<ossimString> result;
   ossimRegExp regExp;
   
   regExp.compile(regularExpression.c_str());
   
   for(i = m_map.begin(); i != m_map.end(); ++i)
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

std::vector<ossimString> ossimKeywordlist::getSubstringKeyList(const ossimString& regularExpression)const
{
   std::vector<ossimString> result;
   getSubstringKeyList(result, regularExpression);
   return result;
}

void ossimKeywordlist::getSubstringKeyList(std::vector<ossimString>& result,
                                           const ossimString& regularExpression)const
{
   KeywordMap::const_iterator i;
   ossimRegExp regExp;
   
   regExp.compile(regularExpression.c_str());
   
   for(i = m_map.begin(); i != m_map.end(); ++i)
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
}

ossim_uint32 ossimKeywordlist::getNumberOfSubstringKeys(const ossimString& regularExpression)const
{
   KeywordMap::const_iterator i;
   std::list<ossimString> currentList;
   ossim_uint32 result = 0;
   ossimRegExp regExp;
   
   regExp.compile(regularExpression.c_str());
   
   for(i = m_map.begin(); i != m_map.end(); ++i)
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
   
   KeywordMap::const_iterator values = tempKwl.m_map.begin();
   
   while(values != tempKwl.m_map.end())
   {
      ossimString newKey = prefix + (*values).first;
      add(newKey,
          (*values).second,
          true);
      ++values;
   }
}

void ossimKeywordlist::addPrefixToKeysThatMatch(const ossimString& prefix,
                                                const ossimString& regularExpression)
{
   ossimKeywordlist tempKwl = *this;
   
   clear();
   
   KeywordMap::const_iterator values = tempKwl.m_map.begin();
   ossimRegExp regExp;
   
   regExp.compile(regularExpression.c_str());
   
   while(values != tempKwl.m_map.end())
   {
      ossimString newKey = prefix+(*values).first;
      if(regExp.find( (*values).first.c_str()))
      {
         
         add(newKey,
             (*values).second,
             true);
      }
      else
      {
         add((*values).first,
             (*values).second,
             true);
      }
      ++values;
   }
}

void ossimKeywordlist::stripPrefixFromAll(const ossimString& regularExpression)
{
   ossimKeywordlist tempKwl = *this;
   
   clear();
   
   KeywordMap::const_iterator values = tempKwl.m_map.begin();
   ossimRegExp regExp;
   
   regExp.compile(regularExpression.c_str());
   
   while(values != tempKwl.m_map.end())
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
   return (ossim_uint32)m_map.size();
}

const ossimKeywordlist::KeywordMap& ossimKeywordlist::getMap()const
{
   return m_map;
}

ossimKeywordlist::KeywordMap& ossimKeywordlist::getMap()
{
   return m_map;
}

void ossimKeywordlist::change_delimiter(char del)
{
   m_delimiter = del;
}

ossimString ossimKeywordlist::delimiter_str() const
{
   char tmp[2];
   tmp[0] = m_delimiter;
   tmp[1] = '\0';
   return ossimString(tmp);
}

//*******************************************************************
// Public Method:
//*******************************************************************
std::ostream& ossimKeywordlist::print(std::ostream& os) const
{
   writeToStream(os);
#if 0
   KeywordMap::const_iterator i;
   
   for(i = m_map.begin(); i != m_map.end(); ++i)
   {
      os << (*i).first << delimiter_str().c_str() << "  "
      << (*i).second << std::endl;
   }
#endif
   return os;
}

//*******************************************************************
// friend function:
//*******************************************************************
OSSIMDLLEXPORT std::ostream& operator<<(std::ostream& os,
                                        const ossimKeywordlist& kwl)
{
   kwl.print(os);
   
   return os;
}


bool ossimKeywordlist::operator ==(ossimKeywordlist& kwl)const
{
   if(this==&kwl) return true;
   std::map<ossimString, ossimString>::const_iterator iter = m_map.begin();
   
   while(iter != m_map.end())
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
   KeywordMap::iterator iter = m_map.begin();
   
   while(iter != m_map.end())
   {
      tempMap.insert(make_pair(ossimString::downcase(iter->first),
                               iter->second));
      
      ++iter;
   }
   m_map = tempMap;
}

void ossimKeywordlist::upcaseKeywords()
{
   KeywordMap tempMap;
   KeywordMap::iterator iter = m_map.begin();
   
   while(iter != m_map.end())
   {
      tempMap.insert(make_pair(ossimString::upcase(iter->first),
                               iter->second));
      
      ++iter;
   }
   m_map = tempMap;
}

