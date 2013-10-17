//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Ken Melero (kmelero@remotesensing.org)
// Description: This class provides manipulation of filenames.
//
//*************************************************************************
// $Id: ossimXmlString.cpp 22326 2013-07-25 17:13:55Z gpotts $
#include <ossim/base/ossimXmlString.h>

bool ossimXmlString::containsSpecialXmlCharacters(const ossimString& value)
{
   for(ossimString::const_iterator it = value.begin();
      it != value.end();++it)
   {
      switch(*it)
      {
         case '&':
         case '<':
         case '>':
         case '"':
         case '\'':
         {
            return true;
         }
         default:
         {
            break;
         }
      }

   }
   return false;
}

const ossimXmlString& ossimXmlString::assign(const ossimString& s)
{
   ossimString result = s;
   
   std::string::size_type pos = 0;

   pos = result.find("&");
   // do ampersand first
   //
   while(pos != std::string::npos)
   {
      ossimString s1(result.begin() + pos,
                     result.begin()+pos+5);
      ossimString s2(result.begin() + pos,
                     result.begin()+pos+4);
      ossimString s3(result.begin() + pos,
                     result.begin()+pos+3);
      
      if((s2 != "&amp")&&
         (s1 != "&quot")&&
         (s3 != "&gt")&&
         (s3 != "&lt")&&
         (s1 != "&apos"))
      {
         result.replace(pos, 1, "&amp");
      }
      
      pos = result.find("&", pos+4);
   }

   result = result.substitute("\"",
                              "&quot",
                              true);
   result = result.substitute("<",
                              "&lt",
                              true);
   result = result.substitute(">",
                              "&gt",
                              true);
   result = result.substitute("'",
                              "&apos",
                              true);
                     
   *((ossimString*)this) = result;

   return *this;
}

ossimString ossimXmlString::wrapCDataIfNeeded(const ossimString& input)
{
   if(containsSpecialXmlCharacters(input))
   {
      return wrapCData(input);
   }

   return input;
}
ossimString ossimXmlString::wrapCData(const ossimString& input)
{
   return "<![CDATA[" + input + "]]>";
}

