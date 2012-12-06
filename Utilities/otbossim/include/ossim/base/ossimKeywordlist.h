//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Ken Melero
// 
// Description: This class provides capabilities for keywordlists.
//
//********************************************************************
// $Id: ossimKeywordlist.h 21527 2012-08-26 16:50:49Z dburken $

#ifndef ossimKeywordlist_HEADER
#define ossimKeywordlist_HEADER 1

#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimString.h>
#include <iosfwd>
#include <map>
#include <vector>
#include <algorithm>

static const char DEFAULT_DELIMITER = ':';

class ossimFilename;


class OSSIM_DLL ossimKeywordlist : public ossimErrorStatusInterface,
   public ossimReferenced
{
public:

   typedef std::map<std::string, std::string> KeywordMap;

   ossimKeywordlist(const ossimKeywordlist& src);
   ossimKeywordlist(char delimiter = DEFAULT_DELIMITER,
                    bool expandEnvVars = false);

   ossimKeywordlist(const char* file,
                    char        delimiter = DEFAULT_DELIMITER,
                    bool        ignoreBinaryChars = false,
                    bool        expandEnvVars = false );

   ossimKeywordlist(const ossimFilename& fileName,
                    char                 delimiter = DEFAULT_DELIMITER,
                    bool                 ignoreBinaryChars = false,
                    bool                 expandEnvVars = false);

   ~ossimKeywordlist();

   static const std::string NULL_KW;

   /*!
    *  Reads file and adds keywords to the KeywordMap.
    *  Returns true if file was parsed, false on error.
    */
   bool addFile(const char* file);

   /*!
    *  Reads file and adds keywords to the KeywordMap.
    *  Returns true if file was parsed, false on error.
    */
   bool addFile(const ossimFilename& file); 

   /*!
    *  Method to change default delimiter.  Handy when parsing
    *  files similar to a ossimKeywordlist.  (DEFAULT = ':')
    */
   void change_delimiter(char del);

   ossimString delimiter_str() const;

   /*!
    * If set to true, then strings found having the format
    * "$(env_var_name)" are expanded in place.
    */
   void setExpandEnvVarsFlag( bool flag );
   /*!
    * Returns the flag that determines whether or not
    * environment variables are expanded.
    */
   bool getExpandEnvVarsFlag( void ) const;

   void add(const char* prefix,
            const ossimKeywordlist& kwl,
            bool overwrite=true);

   /**
    * This is a generic find method that takes a comparator type and iterates through 
    * the map executing the overloaded operator ().
    * Typical code example format
    <pre>
    typedef std::unary_function<std::pair<ossimString, ossimString>, bool> KwlCompareFunctionType;
    
    class KwlKeyCaseInsensitiveEquals : public KwlCompareFunctionType
    {
    public:
       KwlKeyCaseInsensitiveEquals(const ossimString& key):m_key(key){}
       virtual bool operator()(const KwlComparePairType& rhs)const
       {
          return (m_key == rhs.first.downcase());
       }
       ossimString m_key;
    };

    // now for use case example:
    kwl.findValue(value, KwlKeyCaseInsensitiveEquals("foo"));
    </pre>
    
    This example shows how to supplly your own comparator and do a case insensitive
    search for the key foo and the value is set to the variable value.
    *
    */
   template<class CompareType>
   bool findValue(ossimString& value, const CompareType& compare)const
   {
      KeywordMap::const_iterator iter = std::find_if(m_map.begin(), m_map.end(), compare);
      bool result = (iter != m_map.end());
      if(result) value = iter->second;
      return result;
   }
   
   std::string& operator[](const std::string& key)
   {
      return m_map[key];
   }
   std::string operator[](const std::string& key)const
   {
      ossimString result = find(key.c_str());
      
      return result.c_str();
   }

   // Methods to add keywords to list.
   void addPair(const std::string& key,
                const std::string& value,
                bool               overwrite = true);

   void addPair(const std::string& prefix,
                const std::string& key,
                const std::string& value,
                bool               overwrite = true);
   
   /*!
    * Allows you to extract out a sub keywordlist from another
    * you can also collapse the hieracrchy by setting
    * strip prefix to true.
    */
   void add(const ossimKeywordlist& kwl,
            const char* prefix=0,
            bool stripPrefix=true);
  
   void add(const char*   key,
            const char*   value,
            bool          overwrite = true);

   void add(const char*   prefix,
            const char*   key,
            const char*   value,
            bool          overwrite = true);

   void add(const char*   key,
            char          value,
            bool          overwrite = true);

   void add(const char*   prefix,
            const char*   key,
            char          value,
            bool          overwrite = true);

   void add(const char*   key,
            ossim_int16   value,
            bool          overwrite = true);

   void add(const char*   prefix,
            const char*   key,
            ossim_int16   value,
            bool          overwrite = true);

   void add(const char*   key,
            ossim_uint16  value,
            bool          overwrite = true);

   void add(const char*   prefix,
            const char*   key,
            ossim_uint16  value,
            bool          overwrite = true);

   void add(const char*   key,
            ossim_int32   value,
            bool          overwrite = true);

   void add(const char*   prefix,
            const char*   key,
            ossim_int32   value,
            bool          overwrite = true);

   void add(const char*   key,
            ossim_uint32  value,
            bool          overwrite = true);

   void add(const char*   prefix,
            const char*   key,
            ossim_uint32  value,
            bool          overwrite = true);

   void add(const char*   key,
            ossim_int64   value,
            bool          overwrite = true);

   void add(const char*   prefix,
            const char*   key,
            ossim_int64   value,
            bool          overwrite = true);

   void add(const char*   key,
            ossim_uint64  value,
            bool          overwrite = true);

   void add(const char*   prefix,
            const char*   key,
            ossim_uint64  value,
            bool          overwrite = true);

   /**
    * @param key Key for key-value pair.
    *
    * @param value Value to pair with key.  Note this will be stored as a
    * string.
    * 
    * @param precision Decimal point precision of the output. (default = 8)
    *
    * @param trimZeroFlag If true trailing '0's and any trailing '.' will
    * be trimmed from the converted string.  (default = false)
    *
    * @param scientific If true output will be in scientific notation else
    * fixed is used. (default = false)
    */
   void add(const char*   key,
            ossim_float32 value,
            bool          overwrite    = true,
            int           precision    = 8);

   /**
    * @param key Key for key-value pair.
    *
    * @param value Value to pair with key.  Note this will be stored as a
    * string.
    * 
    * @param precision Decimal point precision of the output. (default = 8)
    *
    * @param trimZeroFlag If true trailing '0's and any trailing '.' will
    * be trimmed from the converted string.  (default = false)
    *
    * @param scientific If true output will be in scientific notation else
    * fixed is used. (default = false)
    */
   void add(const char*   prefix,
            const char*   key,
            ossim_float32 value,
            bool          overwrite    = true,
            int           precision    = 8);

   /**
    * @param key Key for key-value pair.
    *
    * @param value Value to pair with key.  Note this will be stored as a
    * string.
    * 
    * @param precision Decimal point precision of the output. (default = 15)
    *
    * @param trimZeroFlag If true trailing '0's and any trailing '.' will
    * be trimmed from the converted string.  (default = false)
    *
    * @param scientific If true output will be in scientific notation else
    * fixed is used. (default = false)
    */
   void add(const char*   key,
            ossim_float64 value,
            bool          overwrite    = true,
            int           precision    = 15);

   /**
    * @param key Key for key-value pair.
    *
    * @param value Value to pair with key.  Note this will be stored as a
    * string.
    * 
    * @param precision Decimal point precision of the output. (default = 15)
    *
    * @param trimZeroFlag If true trailing '0's and any trailing '.' will
    * be trimmed from the converted string.  (default = false)
    *
    * @param scientific If true output will be in scientific notation else
    * fixed is used. (default = false)
    */
   void add(const char*   prefix,
            const char*   key,
            ossim_float64 value,
            bool          overwrite    = true,
            int           precision    = 15);

   /**
    * @brief Checks for key in map.
    *
    * Note that "find" and findKey will alway return an empty string even if
    * the key in not in the map.
    *
    * @return true if key is in map even if value is empty; false, if not.
    */
   bool hasKey( const std::string& key ) const;
   
   /**
    *  @brief Find methods that take std::string(s).
    *  Searches the map for key(/prefix) and returns the resulting value
    *  or an empty string if the key was not found.
    *  @param key e.g. "number_line"
    *  @param prefix e..g "image0."
    *  @return Reference to string.  This will be emptry if not found or
    *  if value is empty.
    */
   const std::string& findKey(const std::string& key) const;
   const std::string& findKey(const std::string& prefix,
                              const std::string& key) const;
   
   const char* find(const char* key) const;
   const char* find(const char* prefix,
                    const char* key) const;

   void remove(const char * key);
   void remove(const char* prefix, const char * key);

   /*!
    *  Searches the map for the number of keys containing the string.
    */
   ossim_uint32 numberOf(const char* str) const;

   /*!
    *  Searches the map for the number of keys containing the prefix+key.
    *  
    *  Given the keyword list contains:
    *
    *  source.type1: foo
    *  source.type2: you
    *
    *  This:
    *
    *  int number_of_sources = numberOf("source", "type");
    *
    *  number_of_sources equals 2
    */
   ossim_uint32 numberOf(const char* prefix, const char* key) const;

   /**
    * Methods to dump the ossimKeywordlist to a file on disk.
    *
    * @param file Name of output file.
    * @param comment Optional string that will be written to line 1
    * as a C++-style comment. A "//" is prepended to the input string.
    *
    * @return true on success, false on error.
    */
   virtual bool write(const char* file, const char* comment = 0) const;

   virtual ossimString toString()const;
   virtual void toString(ossimString& result)const;

   virtual void writeToStream(std::ostream &out)const;

   virtual std::ostream& print(std::ostream& os) const;
   OSSIMDLLEXPORT friend std::ostream& operator<<(std::ostream& os,
                                                  const ossimKeywordlist& kwl);
   bool operator ==(ossimKeywordlist& kwl)const;

    /*!
     * Clear all contents out of the ossimKeywordlist.
     */

    void clear();

    /*!
     * Add contents of another keyword list to this one.
     *
     * @param src the keyword list to copy items from.
     * @param overwrite true if keys existing in this and src should have
     * their value overwritten by the src value, otherwise false to preserve
     * the original value.  Defaults to true.
     */

   void addList( const ossimKeywordlist &src, bool overwrite = true );

   /** deprecated method */
   virtual bool parseStream(std::istream& is,
                            bool ignoreBinaryChars);
   
   virtual bool parseStream(std::istream& is);
   virtual bool parseString(const std::string& inString);

   /*!
    *  Will return a list of keys that contain the string passed in.
    *  Later we will need to allow a user to specify regular expresion
    *  searches.
    */
   std::vector<ossimString> findAllKeysThatContains(
      const ossimString &searchString)const;

   std::vector<ossimString> findAllKeysThatMatch(
      const ossimString &regularExpression)const;

   void extractKeysThatMatch(ossimKeywordlist& kwl,
                             const ossimString &regularExpression)const;

   void removeKeysThatMatch(const ossimString &regularExpression);
   /*!
    * Will return only the portion of the key that
    * matches the regular expression.
    *
    * example:
    *
    *  source1.source1.a:
    *  source1.source2.a:
    *  source1.source3.a:
    *  source1.source4.a:
    *  source1.source10.a:
    *
    *  kwl.getSubstringKeyList("source1.source[0-9]*\\.");
    *
    *  will return:
    *
    *  source1.source1.
    *  source1.source2.
    *  source1.source3.
    *  source1.source4.
    *  source1.source10.
    *
    */
   std::vector<ossimString> getSubstringKeyList(const ossimString& regularExpression)const;
   void getSubstringKeyList(std::vector<ossimString>& result,
                            const ossimString& regularExpression)const;

   ossim_uint32 getNumberOfSubstringKeys(
      const ossimString& regularExpression)const;

   void addPrefixToAll(const ossimString& prefix);
   void addPrefixToKeysThatMatch(const ossimString& prefix,
                                 const ossimString& regularExpression);
   void stripPrefixFromAll(const ossimString& regularExpression);

   /*!
    * Returns the number of elements.
    */
   ossim_uint32 getSize()const;

   const ossimKeywordlist::KeywordMap& getMap()const;
   ossimKeywordlist::KeywordMap& getMap();
   
   ossimKeywordlist& downcaseKeywords();
   ossimKeywordlist& upcaseKeywords();
   
   ossimKeywordlist& trimAllValues(const ossimString& valueToTrim= ossimString(" \t\n\r"));
   ossimKeywordlist trimAllValues(const ossimString& valueToTrim= ossimString(" \t\n\r"))const;


   //! [OLK, Aug/2008]
   //! Sets the boolean  <rtn_val> depending on value associated with keyword for values = 
   //! (yes|no|true|false|1|0). Returns TRUE if keyword found, otherwise false. Also returns false
   //! if none of the above permitted values are specified (rtn_val left unchanged in this case).
   bool getBoolKeywordValue(bool& rtn_val, 
                            const char* keyword, 
                            const char* prefix=0) const;

protected:
   enum KeywordlistParseState
   {
      KeywordlistParseState_OK         = 0,
      KeywordlistParseState_FAIL       = 1, // just used to say this set of token has failed the rules
      KeywordlistParseState_BAD_STREAM = 2, // Means an error occured that is a mal formed stream for Keywordlist
   };
   /*!
    *  Method to parse files to initialize the list.  Method will error on
    *  binary characters if "ignoreBinaryChars = false".  This is used by
    *  ImageHandler factories that can be passed a binary file inadvertently
    *  by a user.  The "ignoreBinaryChars" flag should be set to true if
    *  a text file contains mixed ascii/binary values.
    *  Returns true if file was parsed, false on error.
    */
   bool parseFile(const ossimFilename& file,
                  bool  ignoreBinaryChars = false);

   bool isValidKeywordlistCharacter(ossim_uint8 c)const;
   void skipWhitespace(std::istream& in)const;
   KeywordlistParseState readComments(ossimString& sequence, std::istream& in)const;
   KeywordlistParseState readKey(ossimString& sequence, std::istream& in)const;
   KeywordlistParseState readValue(ossimString& sequence, std::istream& in)const;
   KeywordlistParseState readKeyAndValuePair(ossimString& key, ossimString& value, std::istream& in)const;
   
   // Method to see if keyword exists in list.
   KeywordMap::iterator getMapEntry(const std::string& key);
   KeywordMap::iterator getMapEntry(const ossimString& key);
   KeywordMap::iterator getMapEntry(const char* key);

   KeywordMap               m_map;
   char                     m_delimiter;
   bool                     m_preserveKeyValues; // enables preserving empty field values, multi lines, ... etc
   bool                     m_expandEnvVars;
};

#endif /* #ifndef ossimKeywordlist_HEADER */

