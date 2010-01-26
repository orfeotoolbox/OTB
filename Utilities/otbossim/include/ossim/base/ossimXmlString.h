//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// Description: This class provides manipulation of filenames.
//
//*************************************************************************
// $Id: ossimXmlString.h 9968 2006-11-29 14:01:53Z gpotts $

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

   const ossimXmlString& assign(const ossimString& s);
};

#endif
