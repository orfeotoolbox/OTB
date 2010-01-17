//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Ken Melero
// Description: This class defines a keyword.
//              NOTE:  The keywordlist class cleans up all allocated
//                     memory from instantiating a keyword.
// 
//********************************************************************
// $Id: ossimKeyword.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimKeyword_HEADER
#define ossimKeyword_HEADER

#include <ossim/base/ossimString.h>

class OSSIMDLLEXPORT ossimKeyword
{
public:
   ossimKeyword();
   ossimKeyword(const char* key, const char* description);
   ossimKeyword(const char* key, const char* description, const char* value);
   ossimKeyword(const ossimKeyword& src);
   ~ossimKeyword();

   const ossimKeyword& operator=(const ossimKeyword& kw);
   operator const char*() const;

   const char* key()               const;
   const char* description()       const;
   const char* value()             const;
   ossimString keyString()         const;
   ossimString descriptionString() const;
   ossimString valueString()       const;
   
   void setValue(const char* val);
   void setValue(const ossimString& val);
   void setDescription(const char* desc);
   void setDescription(const ossimString& desc);
   

   friend OSSIM_DLL std::ostream& operator<<(std::ostream& os, const ossimKeyword& kw);

private:
   ossimString theKey;
   ossimString theDesc;
   ossimString theValue;
};

#endif
