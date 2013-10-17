//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// Description: This class provides manipulation of filenames.
//
//*************************************************************************
// $Id: ossimXmlString.h 22326 2013-07-25 17:13:55Z gpotts $

#ifndef ossimXmlString_HEADER
#define ossimXmlString_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>

class OSSIM_DLL ossimXmlString : public ossimString
{
public:
   ossimXmlString()
      :
         ossimString()
      {}
   ossimXmlString(const ossimXmlString& src)
      :
         ossimString(src)
      {}
   ossimXmlString(const ossimString& src)
      :
         ossimString("")
      {
         assign(src);

      }
   ossimXmlString(const char* src)
      :
         ossimString("")
      {
         assign(src);
      }
   
   template <class Iter>
   ossimXmlString(Iter s, Iter e)
      :ossimString("")
      {
         assign(ossimString(s, e));
      }

   const ossimXmlString& operator =(const char* s)
      {
         return assign(ossimString(s));
      }
   	  const ossimXmlString& operator =(const ossimString& s)
      {
         return assign(ossimString(s));
      }
	  const ossimXmlString& operator =(const std::string& s)
      {
         return assign(ossimString(s));
      }
   static ossimString wrapCDataIfNeeded(const ossimString& input);
   static ossimString wrapCData(const ossimString& input);
   static bool containsSpecialXmlCharacters(const ossimString& value);
   const ossimXmlString& assign(const ossimString& s);
};

#endif
