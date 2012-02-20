//----------------------------------------------------------------------------
//
// License:  LGPL See top level LICENSE.txt file.
//
// File: ossimNmeaMessage.h
//
// Author:  Garrett Potts
//
// Description: Contains a general parser for NMEA messages.
//
//
// $Id$
//----------------------------------------------------------------------------
#ifndef ossimNmeaMessage_HEADER
#define ossimNmeaMessage_HEADER

#include <ossim/base/ossimString.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimException.h>
#include <iostream>
#include <algorithm>
#include <vector>


class OSSIM_DLL ossimNmeaMessage : public ossimReferenced
{
public:
   typedef std::vector<ossimString> FieldListType;
   
   ossimNmeaMessage(const std::string& acceptedStartingCharacters="!$"):m_startChars(acceptedStartingCharacters){}
   
   /**
    * Parses a standard formatted NMEA message.  No exceptions are created for checksums.  The checksum needs
    * to be checked after parsing by calling validCheckSum().
    */
   virtual void parseMessage(std::istream& in)throw(ossimException);
   
   virtual bool valid()const{return validCheckSum();}
   bool validCheckSum()const{return m_validCheckSum;}
   
   
   ossim_uint32 numberOfFields()const{return m_fields.size();}
   
   const std::string& getField(ossim_uint32 idx)
   {
      static ossimString empty="";
      
      if(idx < m_fields.size()) return m_fields[idx];
      
      return empty;
   }
   const std::string& operator [](int idx)const
   {
      static ossimString empty="";
      
      if ( idx < static_cast<int>( m_fields.size() ) ) return m_fields[idx];
      
      return empty;
   }
   void setStartChars(const std::string& acceptedStartingCharacters="!$"){m_startChars = acceptedStartingCharacters;}
   
   virtual void reset()
   {
      m_fields.clear();
      m_message = "";
      m_validCheckSum = false;
   }
   const std::string& message()const{return m_message;}
   
   static ossim_uint32 checksum(std::string::const_iterator start, std::string::const_iterator end);
   
protected:
   virtual void setFields(std::string::const_iterator start, std::string::const_iterator end);
   
   bool isValidStartChar(char c)const;
   
   std::string m_message;
   FieldListType m_fields;
   
   bool m_validCheckSum;
   std::string m_startChars;
};

#endif

