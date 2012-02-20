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
// $Id: ossimString.h 20491 2012-01-23 20:10:06Z dburken $
#ifndef ossimString_HEADER
#define ossimString_HEADER 1

#include <ossim/base/ossimConstants.h>
#include <string>
#include <vector>
#include <iostream>


class OSSIM_DLL ossimString
{
public:

   typedef std::string::const_iterator const_iterator;
   typedef std::string::iterator       iterator;
   typedef std::string::size_type      size_type;

   /** @brief default constructor */
   ossimString(): m_str() {}

   /** @brief constructor that takes a std::string */
   ossimString(const std::string& s):m_str(s) {}

   /**
    * @brief constructor that takes char*
    * NOTE: std::string construtor throws exception given null pointer;
    * hence, the null check.
    */
   ossimString(const char *aString):m_str( aString?aString:"" ){}

   /** @brief copy constructor */
   ossimString(const ossimString& aString):m_str(aString.m_str){}

   /** @brief constructor - constructs with n copies of c */
   ossimString(std::string::size_type n, char c):m_str(n,c){}

   /** @brief constructor - constructs with 1 c. */
   ossimString(char aChar):m_str(1, aChar){}

   template <class Iter>
   ossimString(Iter startIter, Iter endIter):m_str(startIter, endIter){}

   /** @brief destructor */
   ~ossimString(){}

   bool contains(const ossimString& aString) const
   { return m_str.find(aString.m_str)!=std::string::npos; }

   bool contains(const char* aString) const
   { return m_str.find(aString)!=std::string::npos; }

   /** @brief Type conversion operator to a const std::string&. */
   operator const std::string&() const { return m_str; }

   /** @brief Type conversion operator to a std::string&. */
   operator std::string&() { return m_str; }

   /** @brief Type conversion operator to a const char*. */
   operator const char*() const { return m_str.c_str(); }

   /** @brief For backward compatibility. */
   const char* chars() const { return m_str.c_str(); }

   /** @brief Reads s from the input stream is. */
   friend OSSIM_DLL std::istream& operator>>(std::istream&  is, ossimString& s);

   /** @brief Writes s to the output stream os. */
   friend OSSIM_DLL std::ostream& operator<<(std::ostream& os,
                                             const ossimString& s);

   /**
    * @brief Reads a string from the input stream is, stopping when it
    * reaches delim.
    */
   friend OSSIM_DLL std::istream& getline( std::istream& is,
                                           ossimString& str,
                                           char delim );

   /** @brief Reads a single line from the input stream is. */
   friend OSSIM_DLL std::istream& getline( std::istream& is, ossimString& str );

   const ossimString& operator=(const std::string& s)
   {
      m_str = s;
      return *this;
   }

   const ossimString& operator=(const char* s)
   {
      if (s) // std::string::operator= throws exception given null pointer.
      {
         m_str = s;
      }
      else
      {
         m_str = "";
      }
      return *this;
   }

   const ossimString& operator=(char c)
   {
      m_str = c;
      return *this;
   }

   const ossimString& operator=(const ossimString& s)
   {
      if ( this != &s )
      {
         m_str = s.m_str;
      }
      return *this;
   }

   const ossimString& operator+=(const ossimString& s)
   {
      m_str.append(s.m_str);
      return *this;
   }

   const ossimString& operator+=(const std::string& s)
   {
      m_str.append(s);
      return *this;
   }

   const ossimString& operator+=(const char* s)
   {
      if ( s ) m_str.append(s);
      return *this;
   }

   const ossimString& operator+=(char c)
   {
      m_str.append(1, c);
      return *this;
   }

   const ossimString& append(const ossimString& s);

   const ossimString& append(const std::string& s);

   const ossimString& append(const char* s);

   /**
    *  @brief  Provides access to the data contained in the %string.
    *  @param n The index of the character to access.
    *  @return  Read-only (const) reference to the character.
    *  @throw  std::out_of_range  If @a n is an invalid index.
    *
    *  This function provides for safer data access.  The parameter is
    *  first checked that it is in the range of the string.  The function
    *  throws out_of_range if the check fails.
    */
   const char& at(std::string::size_type n) const;

   /**
    *  @brief  Provides access to the data contained in the %string.
    *  @param n The index of the character to access.
    *  @return  Read/write reference to the character.
    *  @throw  std::out_of_range  If @a n is an invalid index.
    *
    *  This function provides for safer data access.  The parameter is
    *  first checked that it is in the range of the string.  The function
    *  throws out_of_range if the check fails.  Success results in
    *  unsharing the string.
    */
   char& at(std::string::size_type n);

   /** @brief Append n copies of c to *this. */
   const ossimString& append(std::string::size_type n, char c);

   ossimString operator+(const ossimString& s)const
   {
      ossimString returnS(*this);
      returnS.m_str.append(s.m_str);
      return returnS;
   }

   ossimString operator+(const std::string& s)const
   {
      ossimString returnS(*this);
      returnS.m_str.append(s);
      return returnS;
   }

   ossimString operator+(const char* s)const
   {
      ossimString returnS(*this);
      if ( s ) returnS.m_str.append(s);
      return returnS;
   }

   ossimString operator+(char c)const
   {
      ossimString returnS(*this);
      returnS.m_str.append(1, c);
      return returnS;
   }

   friend OSSIM_DLL ossimString operator+(const char* s1, const ossimString& s2);

   friend OSSIM_DLL ossimString operator+(const std::string s1, const ossimString& s2);

   friend OSSIM_DLL ossimString operator+(char c, const ossimString& s2);
   friend OSSIM_DLL bool operator==(const char* lhs, const ossimString& rhs);
   friend OSSIM_DLL bool operator==(const std::string& lhs, const ossimString& rhs);

   /**
    *  @brief  Test if this ossimString is equal to another ossimString.
    *  @param rhs ossimString to compare.
    *  @return  True if strings are equal.  False otherwise.
    */
   bool operator==(const ossimString& rhs) const
   {
      return (m_str.compare( rhs.m_str ) == 0);
   }

   /**
    *  @brief  Test if this ossimString is equal to a C sting.
    *  @param rhs C string to compare.
    *  @return  True if strings are equal.
    *  False if rhs is not equal null or null.
    */
   bool operator==(const char* rhs) const
   {
      bool result = false;
      if (rhs)
      {
         result = (m_str.compare(std::string(rhs)) == 0);
      }
      return result;
   }

   /**
    *  @brief  Test if this ossimString is not equal to another ossimString.
    *  @param rhs ossimString to compare.
    *  @return  True if strings are not equal.  False otherwise.
    */
   bool operator!=(const ossimString& rhs) const
   {
      return !(m_str.compare(rhs.m_str) == 0);
   }

   /**
    *  @brief  Test if this ossimString is not equal to a C sting.
    *  @param rhs C string to compare.
    *  @return  True if strings are not equal or rhs is null.
    *  False if rhs equal to this string.
    */
   bool operator!=(const char* rhs) const
   {
      bool result = true;
      if (rhs)
      {
         result = !(m_str.compare(std::string(rhs)) == 0);
      }
      return result;
   }

   char& operator[](std::string::size_type i)
   {
      return m_str[i];
      // return *( const_cast<char*>(m_strc_str())+i);
   }

   const char& operator[](std::string::size_type i)const
   {
      return m_str[i];
   }

   /**
    * @brief If pos > size(), throws out_of_range. Otherwise, equivalent to
    * insert(begin() + pos, s, s + traits::length(s)).
    */
   std::string& insert(std::string::size_type pos, const char* s);

   /** @brief Equivalent to insert(end(), c). */
   void push_back(char c);

   /** @brief Equivalent to basic_string(*this, pos, n). */
   std::string substr(std::string::size_type pos = 0,
                      std::string::size_type n = std::string::npos) const;

   /**
    * this will strip lead and trailing character passed in.
    * So if you want to remove blanks:
    *    ossimString temp("       asdfasdf      ");
    *    ossimString trimmedString = temp.trim(" \n\t\r");
    *
    *    this will now contain "asdfasdf" without the blanks.
    *
    */
   ossimString trim(const ossimString& valueToTrim= ossimString(" \t\n\r"))const;
   ossimString& trim(const ossimString& valueToTrim= ossimString(" \t\n\r"));

   ossimString beforePos(std::string::size_type pos)const;
    ossimString afterPos(std::string::size_type pos)const;

   /**
    *  Substitutes searchKey string with replacementValue and returns a
    *  string.  Will replace all occurrences found if "replaceAll" is true.
    */
   ossimString substitute(const ossimString &searchKey,
                                  const ossimString &replacementValue,
                                  bool replaceAll=false)const;

   /**
    *  Substitutes searchKey string with replacementValue and returns a
    *  reference to *this.  Will replace all occurrences found if
    *  "replaceAll" is true.  (like substitute only works on "this")
    */
   ossimString& gsub(const ossimString &searchKey,
                             const ossimString &replacementValue,
                             bool replaceAll=false);

   std::vector<ossimString> explode(const ossimString& delimeter) const;

   /**
    * If the variable "$(env_var_name)" is found in the string, where
    * "env_var_name" is any system environment variable resolvable by
    * the getenv() function, the variable is expanded in place and the
    * result returned.
    */
   ossimString expandEnvironmentVariable() const;

   /**
    * @param aString String to make an upcased copy of.
    *
    * @return An upcased version of aString.
    */
   static ossimString upcase(const ossimString& aString);

   /**
    * @param aString String to make an downcased copy of.
    *
    * @return A downcased version of aString.
    */
   static ossimString downcase(const ossimString& aString);

   /**
    * Upcases this string.
    *
    * @return Reference to this.
    */
   ossimString& upcase();
   ossimString upcase()const;

   /**
    * Downcases this string.
    *
    * @return Reference to this.
    */
   ossimString& downcase();
   ossimString downcase()const;

   /**
    * @brief Returns a pointer to a null-terminated array of characters
    * representing the string's contents.
    */
   const char* c_str() const { return m_str.c_str(); }

   /**
    * @brief Returns a pointer to an array of characters (not necessarily
    * null-terminated) representing the string's contents.
    */
   const char* data() const { return m_str.data(); }

   /** @return The size of the string. */
   std::string::size_type size() const { return m_str.size(); }

   /** @return The size of the string. */
   std::string::size_type length() const { return m_str.size(); }

   /** @return True if size is 0. */
   bool empty() const { return m_str.empty(); }

   /** @return The underlying std::string container. */
   const std::string& string() const { return m_str; }

   /** @return The underlying std::string container. */
   std::string& string() { return m_str; }

   /** @return An iterator pointing to the beginning of the string. */
   std::string::iterator begin() { return m_str.begin(); }

   /** @return An iterator pointing to the end of the string. */
   std::string::iterator end() { return m_str.end(); }

   /** @return An const_iterator pointing to the beginning of the string. */
   std::string::const_iterator begin() const { return m_str.begin(); }

   /** @return An const_iterator pointing to the end of the string. */
   std::string::const_iterator end() const { return m_str.end(); }

   /** @brief Erases the entire container. */
   void clear() { m_str.clear(); }

   /** @brief Erases the character at position p. */
   std::string::iterator erase(std::string::iterator p);

   /** @brief Erases the range [first, last). */
   std::string::iterator erase(std::string::iterator first,
                               std::string::iterator last);

   /** @brief Erases a range. */
   std::string& erase(std::string::size_type pos = 0,
                      std::string::size_type n = std::string::npos);

   /**
    * @brief Searches for s as a substring of *this, beginning at character
    * pos of *this.
    */
   std::string::size_type find(const std::string& s,
                               std::string::size_type pos = 0) const;

   /**
    * @brief Searches for the first n characters of s as a substring of *this,
    * beginning at character pos of *this.
    */
   std::string::size_type find(const char* s,
                               std::string::size_type pos,
                               std::string::size_type n) const;

   /**
    * @brief Searches for a null-terminated character array as a substring
    * of *this, beginning at character pos of *this.
    */
   std::string::size_type find(const char* s,
                               std::string::size_type pos = 0) const;

   /**
    * @brief Searches for the character c, beginning at character position
    * pos.
    */
   std::string::size_type find(char c, std::string::size_type pos = 0) const;

    /**
    * @brief Searches backward for s as a substring of *this, beginning at
    * character position min(pos, size()).
    */
   std::string::size_type rfind(const std::string& s,
                                std::string::size_type pos = 0) const;

   /**
    * @brief Searches backward for the first n characters of s as a substring
    * of *this, beginning at character position min(pos, size()).
    */
   std::string::size_type rfind(const char* s,
                                std::string::size_type pos,
                                std::string::size_type n) const;

   /**
    * @brief Searches backward for a null-terminated character array as a
    * substring of *this, beginning at character min(pos, size()).
    */
   std::string::size_type rfind(const char* s,
                                std::string::size_type pos = 0) const;

   /**
    * @brief Searches backward for the character c, beginning at character
    * position min(pos, size().
    */
   std::string::size_type rfind(char c, std::string::size_type pos = 0) const;

   /** @brief Equivalent to find(c, pos). */
   std::string::size_type find_first_of(
      char c, std::string::size_type pos = 0) const;

   /**
    * @brief Returns the smallest character position N such that
    * pos <= N < size(), and such that (*this)[N] does not compare equal to c.
    * Returns npos if no such character position exists.
    */
   std::string::size_type find_first_not_of(
      char c, std::string::size_type pos = 0) const;

   /**
    * @brief Equivalent to find_first_of(s, pos, traits::length(s)).
    */
   std::string::size_type find_first_of(
      const char* s, std::string::size_type pos = 0) const;

      /**
    * @brief Equivalent to find_first_not_of(s, pos, traits::length(s)).
    */
   std::string::size_type find_first_not_of(
      const char* s, std::string::size_type pos = 0) const;

   /** @brief Equivalent to rfind(c, pos). */
   std::string::size_type find_last_of(
      char c, std::string::size_type pos = std::string::npos) const;

   /**
    * @brief Erases the existing characters and replaces them by n copies
    * of c.
    */
   std::string& assign(std::string::size_type n, char c);

   /** @brief Replaces a substring of *this with the string s. */
   std::string& replace(std::string::size_type pos,
                        std::string::size_type n,
                        const std::string& s);

   /** brief Equivalent to insert(erase(first, last), s.begin(), s.end()). */
   std::string& replace(std::string::iterator first,
                        std::string::iterator last,
                        const std::string& s);

   /** @brief Replaces a substring of *this with a substring of s. */
   std::string& replace(std::string::size_type pos,
                        std::string::size_type n,
                        const std::string& s,
                        std::string::size_type pos1,
                        std::string::size_type n1);

   /** @brief Requests that the string's capacity be changed. */
   void reserve(std::string::size_type n);

   /**
    * METHOD: before(str, pos)
    * Returns string beginning at pos and ending one before the token str
    * If string not found or pos out of range the whole string will be
    * returned.
    */
   ossimString before(const ossimString& str, std::string::size_type pos=0)const;

   /**
    * METHOD: after(str, pos)
    * Returns string immediately after the token str. The search for str
    * begins at pos.  Returns an empty string if not found or pos out of
    * range.
    */
   ossimString after (const ossimString& str, std::string::size_type pos=0)const;

   char* stringDup()const;

   /**
    * String to numeric methods.
    */
   bool                 toBool()const;
   static bool          toBool(const ossimString& aString);

   ossim_uint8          toUInt8()const;
   static ossim_uint8   toUInt8(const ossimString& aString);

   int                  toInt()const;
   static int           toInt(const ossimString& aString);

   ossim_int16          toInt16()const;
   static ossim_int16   toInt16(const ossimString& aString);
   ossim_uint16         toUInt16()const;
   static ossim_uint16  toUInt16(const ossimString& aString);

   ossim_int32          toInt32()const;
   static ossim_int32   toInt32(const ossimString& aString);
   ossim_uint32         toUInt32()const;
   static ossim_uint32  toUInt32(const ossimString& aString);

   ossim_int64          toInt64()const;
   static ossim_int64   toInt64(const ossimString& aString);
   ossim_uint64         toUInt64()const;
   static ossim_uint64  toUInt64(const ossimString& aString);

   /**
    * toLong's deprecated, please use the toInts...
    */
   long                 toLong()const;
   static long          toLong(const ossimString& aString);
   unsigned long        toULong()const;
   static unsigned long toULong(const ossimString& aString);

   ossim_float32        toFloat32()const;
   static ossim_float32 toFloat32(const ossimString& aString);
   ossim_float64        toFloat64()const;
   static ossim_float64 toFloat64(const ossimString& aString);
   double               toDouble()const;
   static double        toDouble(const ossimString& aString);

   /**
    * Numeric to string methods.
    */
   static ossimString toString(bool aValue);
   static ossimString toString(ossim_int16   aValue);
   static ossimString toString(ossim_uint16  aValue);
   static ossimString toString(ossim_int32   aValue);
   static ossimString toString(ossim_uint32  aValue);
   static ossimString toString(ossim_int64   aValue);
   static ossimString toString(ossim_uint64  aValue);

   /**
    * @param aValue Value to convert to string.
    *
    * @param precision Decimal point precision of the output.
    *
    * @param fixed If true setiosflags(std::ios::fixed) will be called.
    */
   static ossimString toString(ossim_float32 aValue,
                               ossim_int32 precision = 8,
                               bool fixed = false);

   /**
    * @param aValue Value to convert to string.
    *
    * @param precision Decimal point precision of the output.
    *
    * @param fixed If true setiosflags(std::ios::fixed) will be called.
    */
   static ossimString toString(ossim_float64 aValue,
                               ossim_int32 precision = 15,
                               bool fixed = false);

   static ossimString stripLeading(const ossimString &value,
                                   char characterToStrip);


   /**
    * Splits this string into a vector of strings (fields) using the delimiter list specified.
    * If a delimiter is encountered at the beginning or the end of this, or two delimiters are
    * contiguous, a blank field is inserted in the vector, unless skipBlankFields is true.
    */
   void split(std::vector<ossimString>& result,
              const ossimString& separatorList,
              bool skipBlankFields=false)const;

   std::vector<ossimString> split(const ossimString& separatorList,
                                  bool skipBlankFields=false)const;

   const ossimString& join(const std::vector<ossimString>& stringList,
                           const ossimString& separator);

   //---
   // Regular expression pattern utilities
   //---

   /**
    * Returns from start of string up to but not including found pattern.
    * Returns "" if pattern not found.
    */
   ossimString beforeRegExp(const char *regularExpressionPattern) const;

   /**
    * Returns from position of found pattern to end of string.
    * Returns "" if pattern not found.
    */
   ossimString fromRegExp(const char *regularExpressionPattern) const;

   /**
    * Returns from position after found pattern to end of string.
    * Returns "" if pattern not found.
    */
   ossimString afterRegExp(const char *regularExpressionPattern) const;

   /**
    * Returns from found pattern to end of pattern.
    * Returns "" if pattern not found.
    */
   ossimString match(const char *regularExpressionPattern) const;

   ossimString replaceAllThatMatch(const char *regularExpressionPattern,
                                   const char *value="") const;

   ossimString replaceStrThatMatch(const char *regularExpressionPattern,
                                   const char *value="") const;

   ossimString urlEncode()const;

   /**
    * If OSSIM_ID_ENABLED returns the OSSIM_ID which currently is the
    * expanded cvs $Id: ossimString.h 20491 2012-01-23 20:10:06Z dburken $ macro; else, an empty string.
    */
   ossimString getOssimId() const;

protected:

   std::string m_str;
};

inline std::string::iterator ossimString::erase(std::string::iterator p)
{
   return m_str.erase(p);
}

inline std::string::iterator ossimString::erase(std::string::iterator first,
                                                std::string::iterator last)
{
   return m_str.erase(first, last);
}

inline std::string& ossimString::erase(std::string::size_type pos,
                                       std::string::size_type n)
{
   return m_str.erase(pos, n);
}

inline std::string::size_type ossimString::find(
   const std::string& s, std::string::size_type pos) const
{
   return m_str.find(s, pos);
}

inline std::string::size_type ossimString::find(
   const char* s, std::string::size_type pos, std::string::size_type n) const
{
   return m_str.find(s, pos, n);
}

inline std::string::size_type ossimString::find(
   const char* s, std::string::size_type pos) const
{
   return m_str.find(s, pos);
}

inline std::string::size_type ossimString::find(
   char c, std::string::size_type pos) const
{
   return m_str.find(c, pos);
}

inline std::string::size_type ossimString::rfind(
   const std::string& s, std::string::size_type pos) const
{
   return m_str.rfind(s, pos);
}

inline std::string::size_type ossimString::rfind(
   const char* s, std::string::size_type pos, std::string::size_type n) const
{
   return m_str.rfind(s, pos, n);
}

inline std::string::size_type ossimString::rfind(
   const char* s, std::string::size_type pos) const
{
   return m_str.rfind(s, pos);
}

inline std::string::size_type ossimString::rfind(
   char c, std::string::size_type pos) const
{
   return m_str.rfind(c, pos);
}

inline std::string::size_type ossimString::find_first_of(
   char c, std::string::size_type pos) const
{
   return m_str.find_first_of(c, pos);
}

inline std::string::size_type ossimString::find_first_of(
   const char* s, std::string::size_type pos) const
{
   return m_str.find_first_of(s, pos);
}

inline std::string::size_type ossimString::find_first_not_of(
   const char c, std::string::size_type pos) const
{
   return m_str.find_first_not_of(c, pos);
}

inline std::string::size_type ossimString::find_first_not_of(
   const char* s, std::string::size_type pos) const
{
   return m_str.find_first_not_of(s, pos);
}

inline std::string::size_type ossimString::find_last_of(
   char c, std::string::size_type pos) const
{
   return m_str.find_last_of(c, pos);
}

inline const ossimString& ossimString::append(const ossimString& s)
{
   m_str.append(s.m_str);
   return *this;
}

inline const ossimString& ossimString::append(const std::string& s)
{
   m_str.append(s);
   return *this;
}

inline const ossimString& ossimString::append(const char* s)
{
   if ( s ) m_str.append(s);
   return *this;
}

inline const ossimString& ossimString::append(std::string::size_type n, char c)
{
   m_str.append(n, c);
   return *this;
}

inline const char& ossimString::at(std::string::size_type n) const
{
   return m_str.at(n); 
}

inline char& ossimString::at(std::string::size_type n)
{
   return m_str.at(n);
}

inline std::string& ossimString::assign(std::string::size_type n, char c)
{
   return m_str.assign(n, c);
}

inline std::string& ossimString::replace(
   std::string::size_type pos, std::string::size_type n, const std::string& s)
{
   return m_str.replace(pos, n, s);
}

inline std::string& ossimString::replace(std::string::iterator first,
                                         std::string::iterator last,
                                         const std::string& s)
{
   return m_str.replace(first, last, s);
}

inline std::string& ossimString::replace(std::string::size_type pos,
                                         std::string::size_type n,
                                         const std::string& s,
                                         std::string::size_type pos1,
                                         std::string::size_type n1)
{
   return m_str.replace(pos, n, s, pos1, n1);
}

inline void ossimString::reserve(std::string::size_type n)
{
   m_str.reserve(n);
}


inline std::string& ossimString::insert(std::string::size_type pos,
                                        const char* s)
{
   if (s) m_str.insert(pos, s);
   return m_str;
}

inline void ossimString::push_back(char c)
{
   m_str.push_back(c);
}

inline std::string ossimString::substr(std::string::size_type pos,
                                       std::string::size_type n) const
{
   return m_str.substr(pos, n);
}

inline std::istream& getline( std::istream& is, ossimString& str, char delim )
{
   return std::getline( is, str.string(), delim );
}

inline std::istream& getline( std::istream& is, ossimString& str )
{
   // Not sure why getline( is, str.string()) doesn't work. (drb)
   return std::getline( is, str.string(), '\n' );
}

inline std::istream& operator>>(std::istream&  is, ossimString& s)
{
   return is >> s.string();
}

inline std::ostream& operator<<(std::ostream& os, const ossimString& s)
{
   return os << s.string();
}

inline ossimString operator+(const char* s1, const ossimString& s2)
{
   ossimString result;
   if ( s1 ) result = ossimString( s1 + s2.string() );
   else result = s2;
   return result;
}

inline ossimString operator+(const std::string s1, const ossimString& s2)
{
   return ossimString( s1 + s2.string() );
}

inline ossimString operator+(char c, const ossimString& s2)
{
   return ossimString( c + s2.string() );
}

inline bool operator==(const char* lhs, const ossimString& rhs)
{
   return (rhs.operator ==(lhs));
}

inline bool operator==(const std::string& lhs, const ossimString& rhs)
{
   return (rhs.operator ==(lhs));
}

/**
 * @brief For use with std::map as a compare operator:
 * std::map<ossimString, ossimString, ossimStringLtstr>
 *
 * This will allow the find method on map to work.
 */
struct ossimStringLtstr
{
   bool operator()(const ossimString& s1, const ossimString& s2) const
   {
      return s1.string().compare(s2.string()) < 0;
   }
};

#endif /* #ifndef ossimString_HEADER */
