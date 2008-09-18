//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
// 
//********************************************************************
// $Id: ossimHexString.h 12952 2008-06-01 16:23:19Z dburken $

#include <cstring>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimConstants.h>

class OSSIMDLLEXPORT ossimHexString : public ossimString
{
public:
   ossimHexString():ossimString()
      {}
   template <class Iter>
      ossimHexString(Iter start, Iter end):ossimString(start, end){}
   ossimHexString(const std::string& aString):ossimString(aString)
      {}
   ossimHexString(const char *aString):ossimString(aString?aString:"")
      {}
   ossimHexString(const ossimString& aString):ossimString(aString.c_str())
      {}
   
   ossimHexString(char value):ossimString()
      {
         assign(value);
      }
   ossimHexString(ossim_int16 value):ossimString()
      {
         assign(value);
      }
   ossimHexString(ossim_int32 value):ossimString()
      {
         assign(value);
      }
   ossimHexString(ossim_uint8 value):ossimString()
      {
         assign(value);
      }
   ossimHexString(ossim_uint16 value):ossimString()
      {
         assign(value);
      }
   ossimHexString(ossim_uint32 value):ossimString()
      {
         assign(value);
      }

   bool operator==(const ossimHexString& s) const {return (std::strcmp(this->c_str(),s.c_str())==0);}
   bool operator==(const char* s) const {return (std::strcmp(this->c_str(),s)==0);}
   bool operator!=(const ossimHexString& s) const {return (std::strcmp(this->c_str(),s.c_str())!=0);}
   bool operator!=(const char* s) const {return (std::strcmp(this->c_str(),s)!=0);}
   operator const char*()const{return c_str();}
   const char* chars()const{return c_str();}
   char& operator[](int i)
      {
         return *( const_cast<char*>(c_str())+i);
      }
   const char& operator[](int i)const
      {
         return *(c_str()+i);
      }
   const ossimHexString& operator =(ossim_int16 value)
      {
         assign(value);
         return *this;
      }
   const ossimHexString& operator =(ossim_int32 value)
      {
         assign(value);
         return *this;
      }
   const ossimHexString& operator =(ossim_uint16 value)
      {
         assign(value);
         return *this;
      }
   const ossimHexString& operator =(ossim_uint32 value)
      {
         assign(value);
         return *this;
      }
   const ossimHexString& operator =(ossim_int8 value)
      {
         assign(value);
         return *this;
      }
   const ossimHexString& operator =(ossim_uint8 value)
      {
         assign(value);
         return *this;
      }
   const ossimHexString& operator =(const ossim_int8* value)
      {
         ossimString::operator=(value);
         return *this;
      }
   const ossimHexString& operator =(const ossimString& value)
      {
         ossimString::operator=(value);
         return *this;
      }

   const ossimHexString& operator =(const ossimHexString& value)
      {
         ossimString::operator=(value);
         return *this;
      }

   const ossimHexString& operator =(const std::string& value)
      {
         ossimString::operator=(value);
         return *this;
      }
   
   ossim_int32  toInt32()const;
   ossim_int16  toInt16()const;
   ossim_int8   toChar()const;
   ossim_uint8  toUchar()const;
   ossim_uint32 toUint32()const;
   ossim_uint16 toUint16()const;

   void assign(ossim_int16 value)
      {
         assign((ossim_uint16)value);
      }
   void assign(ossim_int32 value)
      {
         assign((ossim_uint32)value);
      }
   void assign(ossim_int8 value)
      {
         assign((ossim_uint8)value);
      }
   void assign(ossim_uint16 value);
   void assign(ossim_uint32 value);
   void assign(ossim_uint8  value);
};
