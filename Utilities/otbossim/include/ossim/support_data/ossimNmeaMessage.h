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
   
   ossimNmeaMessage(const ossimString& acceptedStartingCharacters="!$"):m_startChars(acceptedStartingCharacters){}
   
   /**
    * Parses a standard formatted NMEA message.  No exceptions are created for checksums.  The checksum needs
    * to be checked after parsing by calling validCheckSum().
    */
   void parseMessage(std::istream& in)throw(ossimException);
   
   /**
    * After parsing a valid checksum can be called to see if the messages is valid.
    */
   bool validCheckSum()const{return m_validCheckSum;}
   
   
   ossim_uint32 numberOfFields()const{return m_fields.size();}
   
   const ossimString& getField(ossim_uint32 idx)
   {
      static ossimString empty="";
      
      if(idx < m_fields.size()) return m_fields[idx];
      
      return empty;
   }
   const ossimString& operator [](int idx)const
   {
      static ossimString empty="";
      
      if ( idx < static_cast<int>( m_fields.size() ) ) return m_fields[idx];
      
      return empty;
   }
   void setStartChars(const ossimString& acceptedStartingCharacters="!$"){m_startChars = acceptedStartingCharacters;}
   
   void reset()
   {
      m_fields.clear();
      m_message = "";
      m_validCheckSum = false;
   }
   const ossimString& message()const{return m_message;}
   
   static ossim_uint32 checksum(ossimString::const_iterator start, ossimString::const_iterator end);
   
protected:
   void setFields(ossimString::const_iterator start, ossimString::const_iterator end);
   
   bool isValidStartChar(char c)const;
   
   ossimString m_message;
   FieldListType m_fields;
   
   bool m_validCheckSum;
   ossimString m_startChars;
};

#endif

