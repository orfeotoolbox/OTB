//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Description: This class provides capabilities for keywordlists.
//
//********************************************************************
// $Id: ossimKeywordlist.cpp 21527 2012-08-26 16:50:49Z dburken $

#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimRegExp.h>
#include <ossim/base/ossimTrace.h>

#include <algorithm>
#include <fstream>
#include <list>
#include <sstream>
#include <utility>

static ossimTrace traceDebug("ossimKeywordlist:debug");
static const ossim_int32 MAX_LINE_LENGTH = 256;
static const char NULL_KEY_NOTICE[]
= "ossimKeywordlist Notice:  Null key passed to method.";


#ifdef OSSIM_ID_ENABLED
static const bool TRACE = false;
static const char OSSIM_ID[] = "$Id: ossimKeywordlist.cpp 21527 2012-08-26 16:50:49Z dburken $";
#endif

const std::string ossimKeywordlist::NULL_KW = "";

ossimKeywordlist::ossimKeywordlist(const ossimKeywordlist& src)
:m_map(src.m_map),
m_delimiter(src.m_delimiter),
m_preserveKeyValues(src.m_preserveKeyValues),
m_expandEnvVars(src.m_expandEnvVars)
{
}

ossimKeywordlist::ossimKeywordlist(char delimiter, 
                                   bool expandEnvVars)
:
m_map(),
m_delimiter(delimiter),
m_preserveKeyValues(true),
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
m_preserveKeyValues(true),
//m_lineContinuationCharacter('\\'),
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
m_preserveKeyValues(true),
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
      addPair( (*i).first, (*i).second, overwrite );
      i++;
   }
}

void ossimKeywordlist::add(const ossimKeywordlist& kwl,
                           const char* prefix,
                           bool stripPrefix)
{
   std::map<std::string, std::string>::const_iterator iter = kwl.m_map.begin();
   
   ossimRegExp regExp;
   
   // Check for null prefix.
   std::string tmpPrefix;
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
            newKey = newKey.substitute(prefix, "");
            
         }
         
         addPair(newKey.string(), (*iter).second, true);
      }
      ++iter;
   }
}

void ossimKeywordlist::add(const char* prefix,
                           const ossimKeywordlist& kwl,
                           bool overwrite)
{
   std::string p = prefix ? prefix : "";
   std::map<std::string, std::string>::const_iterator iter = kwl.m_map.begin();
   while(iter != kwl.m_map.end())
   {
      std::string k( p + (*iter).first );
      addPair( k, (*iter).second, overwrite );
      ++iter;
   }
}

void ossimKeywordlist::addPair(const std::string& key,
                               const std::string& value,
                               bool               overwrite)
{
   if ( key.size() )
   {
      ossimString v = value;
      if ( m_expandEnvVars == true )
      {
         v = v.expandEnvironmentVariable();
      }
      
      KeywordMap::iterator i = getMapEntry(key);
      
      if (i == m_map.end())
      {
         m_map.insert(std::make_pair(key, v.string()));
      }
      else if (overwrite)
      {
         (*i).second = v.string();
      }
   }
}

void ossimKeywordlist::addPair(const std::string& prefix,
                               const std::string& key,
                               const std::string& value,
                               bool               overwrite)
{
   std::string k(prefix + key);
   addPair(k, value, overwrite);
}

void ossimKeywordlist::add(const char* key,
                           const char* value,
                           bool        overwrite)
{
   if ( key )
   {
      std::string k(key);
      std::string v(value?value:"");
      addPair(k, v, overwrite);
   }
}

void ossimKeywordlist::add(const char* prefix,
                           const char* key,
                           const char* value,
                           bool        overwrite)
{
   if ( key )
   {
      std::string k(prefix ? (std::string(prefix)+std::string(key)) : key);
      std::string v(value ? value : "");
      addPair(k, v, overwrite);
   }
}

void ossimKeywordlist::add(const char* key,
                           char        value,
                           bool        overwrite)
{
   if ( key )
   {
      std::string k(key);
      std::string v(1, value);
      addPair(k, v, overwrite);
   }
}

void ossimKeywordlist::add(const char* prefix,
                           const char* key,
                           char        value,
                           bool        overwrite)
{
   if ( key )
   {
      std::string k(prefix ? (std::string(prefix)+std::string(key)) : key);
      std::string v(1, value);
      addPair(k, v, overwrite);
   }
}

void ossimKeywordlist::add(const char* key,
                           ossim_int16 value,
                           bool overwrite)
{
   if ( key )
   {
      std::string k(key);
      std::string v = ossimString::toString(value).string();
      addPair(k, v, overwrite);
   }
}

void ossimKeywordlist::add(const char* prefix,
                           const char* key,
                           ossim_int16 value,
                           bool overwrite)
{
   if ( key )
   {
      std::string k(prefix ? (std::string(prefix)+std::string(key)) : key);
      std::string v = ossimString::toString(value).string();
      addPair(k, v, overwrite);
   }
}

void ossimKeywordlist::add(const char* key,
                           ossim_uint16 value,
                           bool overwrite)
{
   if ( key )
   {
      std::string k(key);
      std::string v = ossimString::toString(value).string();
      addPair(k, v, overwrite);
   }
}

void ossimKeywordlist::add(const char* prefix,
                           const char* key,
                           ossim_uint16 value,
                           bool overwrite)
{
   if ( key )
   {
      std::string k(prefix ? (std::string(prefix)+std::string(key)) : key);
      std::string v = ossimString::toString(value).string();
      addPair(k, v, overwrite);
   }
}

void ossimKeywordlist::add(const char* key,
                           ossim_int32 value,
                           bool overwrite)
{
   if ( key )
   {
      std::string k(key);
      std::string v = ossimString::toString(value).string();
      addPair(k, v, overwrite);
   }
}

void ossimKeywordlist::add(const char*  prefix,
                           const char*  key,
                           ossim_int32 value,
                           bool overwrite)
{
   if ( key )
   {
      std::string k(prefix ? (std::string(prefix)+std::string(key)) : key);
      std::string v = ossimString::toString(value).string();
      addPair(k, v, overwrite);
   }
}

void ossimKeywordlist::add(const char* key,
                           ossim_uint32 value,
                           bool overwrite)
{
   if ( key )
   {
      std::string k(key);
      std::string v = ossimString::toString(value).string();
      addPair(k, v, overwrite);
   }
}

void ossimKeywordlist::add(const char*  prefix,
                           const char*  key,
                           ossim_uint32 value,
                           bool overwrite)
{
   if ( key )
   {
      std::string k(prefix ? (std::string(prefix)+std::string(key)) : key);
      std::string v = ossimString::toString(value).string();
      addPair(k, v, overwrite);
   }
}

void ossimKeywordlist::add(const char* key,
                           ossim_int64 value,
                           bool overwrite)
{
   if ( key )
   {
      std::string k(key);
      std::string v = ossimString::toString(value).string();
      addPair(k, v, overwrite);
   }
}

void ossimKeywordlist::add(const char*  prefix,
                           const char*  key,
                           ossim_int64 value,
                           bool overwrite)
{
   if ( key )
   {
      std::string k(prefix ? (std::string(prefix)+std::string(key)) : key);
      std::string v = ossimString::toString(value).string();
      addPair(k, v, overwrite);
   }
}

void ossimKeywordlist::add(const char* key,
                           ossim_uint64 value,
                           bool overwrite)
{
   if ( key )
   {
      std::string k(key);
      std::string v = ossimString::toString(value).string();
      addPair(k, v, overwrite);
   }
}

void ossimKeywordlist::add(const char*  prefix,
                           const char*  key,
                           ossim_uint64 value,
                           bool overwrite)
{
   if ( key )
   {
      std::string k(prefix ? (std::string(prefix)+std::string(key)) : key);
      std::string v = ossimString::toString(value).string();
      addPair(k, v, overwrite);
   }
}

void ossimKeywordlist::add(const char* key,
                           ossim_float32 value,
                           bool overwrite,
                           int precision)
{
   if ( key )
   {
      std::string k(key);
      std::string v = ossimString::toString(value, precision).string();
      addPair(k, v, overwrite);
   }
}

void ossimKeywordlist::add(const char* prefix,
                           const char* key,
                           ossim_float32 value,
                           bool overwrite,
                           int precision)
{
   if ( key )
   {
      std::string k(prefix ? (std::string(prefix)+std::string(key)) : key);
      std::string v = ossimString::toString(value, precision).string();
      addPair(k, v, overwrite);
   }
}

void ossimKeywordlist::add(const char* key,
                           ossim_float64 value,
                           bool overwrite,
                           int precision)
{
   if ( key )
   {
      std::string k(key);
      std::string v = ossimString::toString(value, precision).string();
      addPair(k, v, overwrite);
   }
}

void ossimKeywordlist::add(const char* prefix,
                           const char* key,
                           ossim_float64 value,
                           bool overwrite,
                           int precision)
{
   if ( key )
   {
      std::string k(prefix ? (std::string(prefix)+std::string(key)) : key);
      std::string v = ossimString::toString(value, precision).string();
      addPair(k, v, overwrite);
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
   ossimRegExp reg("\n|\r| $|^ ");
   for(i = m_map.begin(); i != m_map.end(); ++i)
   {
      value = (*i).second;
      if(!value.empty())
      {
         if(reg.find(value))
         {
            value = "\"\"\"" + value + "\"\"\"";
         }
      }
      //      value = value.substitute('\n', "\\\n", true);
      out << (*i).first  << delimiter_str().c_str() << "  "
      << value << std::endl;
   }
}

bool ossimKeywordlist::hasKey( const std::string& key ) const
{
   bool result = false;
   KeywordMap::const_iterator i = m_map.find(key);
   if (i != m_map.end())
   {
      result = true;
   }
   return result;
}

const std::string& ossimKeywordlist::findKey(const std::string& key) const
{
   // std::string result;
   KeywordMap::const_iterator i = m_map.find(key);
   if (i != m_map.end())
   {
      // result = (*i).second;
      return (*i).second;
   }
   return ossimKeywordlist::NULL_KW;
}

const std::string& ossimKeywordlist::findKey(const std::string& prefix,
                                             const std::string& key) const
{
   std::string k = prefix+key;
   return findKey(k);
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
         if ( ossimString((*i).first).contains(str) )
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
   if ( key ) // Must have key, sometimes no prefix.
   {
      std::string k(prefix ? (std::string(prefix)+std::string(key)) : key);
      return numberOf(k.c_str());
   }
   return 0;
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
      std::string k = key;
      return m_map.find(k);
   }
   else
   {
      return m_map.end();
   }
}

ossimKeywordlist::KeywordMap::iterator
ossimKeywordlist::getMapEntry(const std::string& key)
{
   return m_map.find(key);
}

ossimKeywordlist::KeywordMap::iterator
ossimKeywordlist::getMapEntry(const ossimString& key)
{
   return m_map.find(key.string());
}


//*******************************************************************
// Private Method:
//*******************************************************************
bool ossimKeywordlist::parseFile(const ossimFilename& file,
                                 bool ignoreBinaryChars)
{
   if(!file.exists()) return false;
   bool result = false;
   std::ifstream is;
   is.open(file.c_str(), std::ios::in | std::ios::binary);
   
   if(!is.fail())
   {
      result = parseStream(is, ignoreBinaryChars);
   }
   
   is.close();
   
   return result;
}

bool ossimKeywordlist::parseStream(std::istream& is, bool /* ignoreBinaryChars */)
{
   return parseStream(is);
}

bool ossimKeywordlist::parseString(const std::string& inString)
{
   std::istringstream in(inString);
   
   return parseStream(in);
}

bool ossimKeywordlist::isValidKeywordlistCharacter(ossim_uint8 c)const
{
   if((c>=0x20&&c<=0x7e))
   {
      return true;
   }
   switch(c)
   {
      case '\n':
      case '\r':
      case '\t':
         return true;
   }
   return false;
}

void ossimKeywordlist::skipWhitespace(std::istream& in)const
{
   int c = in.peek();
   while( !in.fail() &&
         ( (c == ' ') || (c == '\t') || (c == '\n') || (c == '\r') ) )
   {
      in.ignore(1);
      c = in.peek();
   }
}

ossimKeywordlist::KeywordlistParseState ossimKeywordlist::readComments(ossimString& sequence, std::istream& in)const
{
   KeywordlistParseState result = KeywordlistParseState_FAIL;
   char c = (char)in.peek();
   if(c == '/')
   {
      sequence += (char)in.get();
      c = in.peek();
      if(c == '/')
      {
         result = KeywordlistParseState_OK;
         sequence += c;
         while(!in.bad()&&!in.eof())
         {
            c = (char)in.get();
            if(!isValidKeywordlistCharacter(c))
            {
               result = KeywordlistParseState_BAD_STREAM;
               break;
            }
            if((c == '\n')||
               (c == '\r'))
            {
               break;
            }
            sequence += c;
         }
      }
   }
   return result;
}

ossimKeywordlist::KeywordlistParseState ossimKeywordlist::readKey(ossimString& sequence, std::istream& in)const
{
   KeywordlistParseState result = KeywordlistParseState_FAIL;
   if(!sequence.empty())
   {
      if(*(sequence.begin()+(sequence.size()-1)) == m_delimiter)
      {
         sequence = ossimString(sequence.begin(), sequence.begin() + (sequence.size()-1));
         return KeywordlistParseState_OK;
      }
   }
   // not a comment so read til key delimeter
   while(!in.eof() && in.good())
   {
      ossim_uint8 c = in.get();
      if( isValidKeywordlistCharacter(c) )
      {
         if ( (c == '\n') || (c == '\r') ) 
         {
            // Hit end of line with no delimiter.
            if ( in.peek() == EOF )
            {
               //---
               // Allowing on last line only.
               // Note the empty key will trigger parseStream to return true.
               //---
               sequence.clear();
               result = KeywordlistParseState_OK;
               break;
            }
            else // Line with no delimiter.
            {
               // mal formed input stream for keyword list specification
               result = KeywordlistParseState_BAD_STREAM;
               break;
            }
         }
         else if(c != m_delimiter)
         {
            sequence += (char)c;
         }
         else // at m_delimiter
         {
            result = KeywordlistParseState_OK;
            sequence = sequence.trim();
            break;
         }
      }
      else 
      {
         // mal formed input stream for keyword list specification
         result = KeywordlistParseState_BAD_STREAM;
         break;
      }
   }
   // we never found a delimeter so we are mal formed
   if(!sequence.empty()&&(result!=KeywordlistParseState_OK))
   {
      result = KeywordlistParseState_BAD_STREAM;
   }
   return result;
}

ossimKeywordlist::KeywordlistParseState ossimKeywordlist::readValue(ossimString& sequence, std::istream& in)const
{
   KeywordlistParseState result = KeywordlistParseState_OK;
   
   ossim_int32 quoteCount = 0; // mark as not set
   
   // make sure we check for a blank value
   while(!in.eof()&&!in.bad())
   {
      if(in.peek() == ' '||
         in.peek() == '\t')
      {
         in.ignore();
      }
      else if(in.peek() == '\n' ||
              in.peek() == '\r')
      {
         in.ignore();
         return result;
      }
      else 
      {
         break;
      }
   }
   // The ifstream object will end in 'ÿ' (character 255 or -1) if the end-of-file indicator 
   // will not be set(e.g \n). In this case, end-of-file conditions would never be detected. 
   // add EOF (which is actually the integer -1 or 255) check here.
   // Reference link http://www.cplusplus.com/forum/general/33821/
   while(!in.eof()&&!in.bad()&&in.peek()!=EOF)
   {
      ossim_uint8 c = in.get();
      if(isValidKeywordlistCharacter(c))
      {
         if(((c == '\n'||c=='\r') && !quoteCount) || in.eof())
         {
            break;
         }
         sequence += (char)c;
         if(sequence.size() >2)
         {
            if(quoteCount < 1)
            {
               // if quoted
               if(ossimString(sequence.begin(), sequence.begin()+3) == "\"\"\"")
               {
                  ++quoteCount;
               }
            }
            else // check for ending quotes 
            {
               if(ossimString(sequence.begin() + sequence.size()-3, sequence.end()) == "\"\"\"")
               {
                  ++quoteCount;
               }
            }
         }
         if(quoteCount > 1)
         {
            sequence = ossimString(sequence.begin()+3, sequence.begin()+(sequence.size()-3));
            break;
         }
      }
      else 
      {
         result = KeywordlistParseState_BAD_STREAM;
         break;
      }
   }
   return result;
}

ossimKeywordlist::KeywordlistParseState ossimKeywordlist::readKeyAndValuePair(ossimString& key, ossimString& value, std::istream& in)const
{
   ossimKeywordlist::KeywordlistParseState keyState   = readKey(key, in);
   if(keyState & KeywordlistParseState_BAD_STREAM) return keyState;
   ossimKeywordlist::KeywordlistParseState valueState = readValue(value, in);
   return static_cast<ossimKeywordlist::KeywordlistParseState>( (static_cast<int>(keyState) |
                                                                 static_cast<int>(valueState)) );
}

bool ossimKeywordlist::parseStream(std::istream& is)
{
   if (!is) // Check stream state.
   {
      return false;
   }
   ossimString key;
   ossimString value;
   ossimString sequence;
   KeywordlistParseState state = KeywordlistParseState_OK;
   while(!is.eof() && !is.bad())
   {
      skipWhitespace(is);
      if(is.eof() || is.bad()) return true; // we skipped to end so valid keyword list
      state = readComments(sequence, is);
      if(state & KeywordlistParseState_BAD_STREAM) return false;
      // if we failed a comment parse then try key value parse.
      if(state == KeywordlistParseState_FAIL)
      {
         key = sequence; // just in case there is a 1 token look ahead residual for a single slash test.
         ossimKeywordlist::KeywordlistParseState testKeyValueState = readKeyAndValuePair(key, value, is);
         if(testKeyValueState == KeywordlistParseState_OK)
         {
            key = key.trim();
            if(key.empty())
            {
               return true;
            }
            if ( m_expandEnvVars == true )
            {
               ossimString result = value.expandEnvironmentVariable();
               m_map.insert(std::make_pair(key.string(), result.string()));
            }
            else
            {
               m_map.insert(std::make_pair(key.string(), value.string()));
            }
         }
         else if(testKeyValueState & KeywordlistParseState_BAD_STREAM)
         {
            return false;
         }
#if 0
         // Commented out to allow an invalid line in keyword list without
         // erroring out, effectively skipping bad line. drb - 01 Sep. 2001
         else
         {
            return false;
         }
#endif
      }
      else if(state & KeywordlistParseState_BAD_STREAM)
      {
         return false;
      }
      sequence = key = value = "";
   }   
   
   return true;
}

std::vector<ossimString> ossimKeywordlist::findAllKeysThatContains(const ossimString &searchString)const
{
   KeywordMap::const_iterator i;
   std::vector<ossimString> result;
   
   for(i = m_map.begin(); i != m_map.end(); ++i)
   {
      if( ossimString((*i).first).contains(searchString))
      {
         result.push_back((*i).first);
      }
   }
   
   return result;
}

std::vector<ossimString> ossimKeywordlist::findAllKeysThatMatch(const ossimString &regularExpression)const
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
         kwl.addPair((*i).first, (*i).second);
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
      std::string newKey = prefix.string() + (*values).first;
      addPair(newKey, (*values).second, true);
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
      std::string newKey = prefix.string()+(*values).first;
      if(regExp.find( (*values).first.c_str()))
      {
         
         addPair(newKey, (*values).second, true);
      }
      else
      {
         addPair((*values).first, (*values).second, true);
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
      std::string newKey = (*values).first;
      if(regExp.find( (*values).first.c_str()))
      {
         newKey.erase(newKey.begin()+regExp.start(),
                      newKey.begin()+regExp.start()+regExp.end());
         
         addPair(newKey, (*values).second, true);
      }
      else
      {
         addPair(newKey, (*values).second, true);
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
   std::map<std::string, std::string>::const_iterator iter = m_map.begin();
   
   while(iter != m_map.end())
   {
      const char* value = kwl.find((*iter).first.c_str());
      
      if(ossimString(value) != (*iter).second)
      {
         return false;
      }
      ++iter;
   }
   
   return true;
}

ossimKeywordlist&  ossimKeywordlist::downcaseKeywords()
{
   KeywordMap tempMap;
   KeywordMap::iterator iter = m_map.begin();
   
   while(iter != m_map.end())
   {
      ossimString k(iter->first);
      tempMap.insert(std::make_pair(k.downcase().string(), iter->second));
      ++iter;
   }
   m_map = tempMap;
   
   return *this;
}

ossimKeywordlist& ossimKeywordlist::upcaseKeywords()
{
   KeywordMap tempMap;
   KeywordMap::iterator iter = m_map.begin();
   
   while(iter != m_map.end())
   {
      ossimString k(iter->first);
      tempMap.insert(std::make_pair(k.upcase().string(), iter->second));
      ++iter;
   }
   m_map = tempMap;
   
   return *this;
}

ossimKeywordlist& ossimKeywordlist::trimAllValues(const ossimString& valueToTrim)
{
   KeywordMap::iterator iter = m_map.begin();
   
   while(iter != m_map.end())
   {
      iter->second = ossimString(iter->second).trim(valueToTrim).string();
      ++iter;
   }
   
   return *this;
}

ossimKeywordlist ossimKeywordlist::trimAllValues(const ossimString& valueToTrim)const
{
   ossimKeywordlist result(*this);
   result.trimAllValues(valueToTrim);
   return result;
}


//*************************************************************************************************
//! [OLK, Aug/2008]
//! Sets the boolean destination arg depending on value associated with keyword for values = 
//! (yes|no|true|false|1|0). Returns TRUE if keyword found, otherwise false. Also returns false
//! if none of the above permitted values are specified (rtn_val left unchanged in this case).
//*************************************************************************************************
bool ossimKeywordlist::getBoolKeywordValue(bool& rtn_val, 
                                           const char* keyword, 
                                           const char* prefix) const
{
   bool found = true;
   const char* val_str = find(prefix, keyword);
   if (val_str) 
   {
      found = true;
      ossimString yesno (val_str);
      yesno.upcase();
      if ((yesno == "YES") || (yesno == "TRUE") || (yesno == "1"))
         rtn_val = true;
      else if ((yesno == "NO") || (yesno == "FALSE") || (yesno == "0"))
         rtn_val = false;
      else
         found = false;
   }
   else
      found = false;
   
   return found;
}

