//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// Description: This class extends the stl's string class.
//
//********************************************************************
// $Id: ossimString.h,v 1.42 2005/10/17 18:37:16 gpotts Exp $
#ifndef ossimString_HEADER
#define ossimString_HEADER
#include <string>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <base/common/ossimConstants.h>

class OSSIM_DLL ossimString : public std::string
{
public:
	ossimString():std::string(){}
	ossimString(const std::string& aString):std::string(aString){}
	ossimString(const char *aString):std::string(aString?aString:""){}
	ossimString(const ossimString& aString):std::string(aString.c_str()){}
   ossimString(char aChar);

   /**
    * Create a string with n copies of c.
    */
   ossimString(size_type n, char c);
   
   template <class Iter>
	   ossimString(Iter start, Iter end):std::string(start, end){}


   bool contains(const ossimString& aString) const {return find(aString)!=npos;}
   bool contains(const char*   aString) const {return find(aString)!=npos;}

   const ossimString& operator =(const char* s)
      {
         return (*this = ossimString(s));

      }
   const ossimString& operator =(char c)
      {
         return (*this = ossimString(c));

      }
   const ossimString& operator =(const ossimString& s)
      {
		  std::string::operator =(s.c_str());
         return *this;
      }
   const ossimString& operator +=(const ossimString& s)
      {
         append(s.c_str());
         return *this;
      }
   const ossimString& operator +=(const char* s)
      {
         append(ossimString(s).c_str());
         return *this;
      }
   const ossimString& operator +=(char c)
      {
         append(ossimString(c).c_str());
         return *this;
      }
   ossimString operator +(const ossimString& s)const
      {
         ossimString returnS(*this);

         returnS.append(s.c_str());

         return returnS;
      }
   ossimString operator +(const char* s)const
      {
         ossimString returnS(*this);

         returnS.append(ossimString(s).c_str());

         return returnS;
      }
   ossimString operator +(char c)const
      {
         ossimString returnS(*this);

         returnS.append(ossimString(c).c_str());

         return returnS;
      }
   bool operator==(const ossimString& s) const {return (strcmp(this->c_str(),s.c_str())==0);}
   bool operator==(const char* s) const {return (strcmp(this->c_str(),s)==0);}
   bool operator!=(const ossimString& s) const {return (strcmp(this->c_str(),s.c_str())!=0);}
   bool operator!=(const char* s) const {return (strcmp(this->c_str(),s)!=0);}

   char& operator[](int i)
      {
         return *( const_cast<char*>(c_str())+i);
      }
   const char& operator[](int i)const
      {
         return *(c_str()+i);
      }
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

   ossimString beforePos(ossim_uint32 pos)const;
   ossimString afterPos(ossim_uint32 pos)const;

   /**
    *  Substitudes searchKey string with replacementValue and returns a
    *  string.  Will replace all occurrances found if "replaceAll" is true.
    */
   ossimString substitute(const ossimString &searchKey,
                          const ossimString &replacementValue,
                          bool replaceAll=false)const;

   /**
    *  Substitudes searchKey string with replacementValue and returns a
    *  reference to *this.  Will replace all occurrances found if
    *  "replaceAll" is true.  (like substitute only works on "this")
    */
   ossimString& gsub(const ossimString &searchKey,
                     const ossimString &replacementValue,
                     bool replaceAll=false);

   std::vector<ossimString> explode(const ossimString& delimeter) const;

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
   
   /**
    * Downcases this string.
    * 
    * @return Reference to this.
    */
   ossimString& downcase();

   operator const char*()const{return c_str();}
   const char* chars()const{return c_str();}

   /**
    * METHOD: before(str, pos)
    * Returns string beginning at pos and ending one before the token str
    * If string not found or pos out of range the whole string will be
    * returned.
    */
   ossimString before(const ossimString& str, ossim_uint32 pos=0);

   /**
    * METHOD: after(str, pos)
    * Returns string immediately after the token str. The search for str
    * begins at pos.  Returns an emptry string if not found or pos out of
    * range.
    */
   ossimString after (const ossimString& str, ossim_uint32 pos=0);

   char* stringDup()const;

   /**
    * String to numeric methods.
    */
   bool                 toBool()const;
   static bool          toBool(const ossimString& aString);
   int                  toInt()const;
   static int           toInt(const ossimString& aString);
   ossim_int32          toInt32()const;
   static ossim_int32   toInt32(const ossimString& aString);
   ossim_uint32         toUInt32()const;
   static ossim_uint32  toUInt32(const ossimString& aString);
   long                 toLong()const;
   static long          toLong(const ossimString& aString);
   unsigned long        toULong()const;
   static unsigned long toULong(const ossimString& aString);
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
   static ossimString toString(long          aValue);
   static ossimString toString(unsigned long aValue);

   /**
    * @param aValue Value to convert to string.
    *
    * @param precision Decimal point precision of the output.
    * 
    * @param trimZeroFlag If true trailing '0's and any trailing '.' will
    * be trimmed.
    *
    * @param scientific If true output will be in scientific notation else
    * fixed is used.
    */
   static ossimString toString(ossim_float32 aValue,
                               int  precision    = 8,
                               bool trimZeroFlag = false,
                               bool scientific   = false );
   
   /**
    * @param aValue Value to convert to string.
    *
    * @param precision Decimal point precision of the output.
    *
    * @param trimZeroFlag If true trailing '0's and any trailing '.' will
    * be trimmed.
    *
    * @param scientific If true output will be in scientific notation else
    * fixed is used.
    */
   static ossimString toString(ossim_float64 aValue,
                               int  precision    = 15,
                               bool trimZeroFlag = false,
                               bool scientific   = false);

   static ossimString stripLeading(const ossimString &value,
                                   char characterToStrip);


   void split(std::vector<ossimString>& result,
              const ossimString& separatorList)const;

   std::vector<ossimString> split(const ossimString& separatorList)const;

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

   /**
    * If OSSIM_ID_ENABLED returns the OSSIM_ID which currently is the
    * expanded cvs $Id: ossimString.h,v 1.42 2005/10/17 18:37:16 gpotts Exp $ macro; else, an empty string.
    */
   ossimString getOssimId() const;
};

#endif /* #ifndef ossimString_HEADER */
