//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author: Ken Melero
// 
// Description: This class provides capabilities for keywordlists.
//
//********************************************************************
// $Id: ossimKeywordlist.h 16542 2010-02-05 13:26:37Z gpotts $

#ifndef ossimKeywordlist_HEADER
#define ossimKeywordlist_HEADER

#include <map>
#include <iosfwd>
#include <vector>

#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimString.h>

static const char DEFAULT_DELIMITER = ':';

class ossimFilename;


class OSSIM_DLL ossimKeywordlist : public ossimErrorStatusInterface,
   public ossimReferenced
{
public:

   typedef std::map<ossimString, ossimString> KeywordMap;

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


   /*!
    * Allows you to extract out a sub keywordlist from another
    * you can also collapse the hieracrchy by setting
    * strip prefix to true.
    */
   void add(const ossimKeywordlist& kwl,
            const char* prefix=0,
            bool stripPrefix=true);

   // Methods to add keywords to list.
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

   /*!
    *  find methods
    *  Searches the map for key(/prefix) and returns the resulting value
    *  or a null string if the key was not found.
    */
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
    *  Searches the map for the number of keys containing the prefix
    *  and the string.
    *
    *  Given the keyword list contains:
    *
    *  source1.type: foo
    *  source2.type: you
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

   virtual bool parseStream(std::istream& is,
                            bool ignoreBinaryChars = false);

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
   void downcaseKeywords();
   void upcaseKeywords();
protected:

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


   // Method to see if keyword exists in list.
   KeywordMap::iterator getMapEntry(const ossimString& key);
   KeywordMap::iterator getMapEntry(const char* key);

   KeywordMap               m_map;
   char                     m_delimiter;
   char                     m_lineContinuationCharacter;
   bool                     m_expandEnvVars;
};

#endif /* #ifndef ossimKeywordlist_HEADER */

