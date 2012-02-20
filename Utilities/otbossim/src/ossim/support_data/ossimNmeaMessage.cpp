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
#include <ossim/support_data/ossimNmeaMessage.h>
#include <ossim/base/ossimCommon.h>
#include <iomanip>

ossim_uint32 ossimNmeaMessage::checksum(std::string::const_iterator start, std::string::const_iterator end)
{
   ossim_uint32 sum = 0;
   
   while(start!=end&&((*start)!='*'))
   {
      sum ^= ((*start)%128);
      ++start;
   }
   return sum;
}

void ossimNmeaMessage::setFields(std::string::const_iterator start, std::string::const_iterator end)
{
   m_fields.clear();
   ossim_uint32 idx = 0;
   while(start != end)
   {
      m_fields.push_back("");
      
      while((start!=end)&&
            (*start!=','))
      {
         m_fields[idx]+=*start;
         ++start;
      }
      
      if(start!=end)
      {
         ++start;
      }
      ++idx;
   }
}

bool ossimNmeaMessage::isValidStartChar(char c)const
{
   std::string::const_iterator iter = std::find(m_startChars.begin(), m_startChars.end(), c);
   return (iter != m_startChars.end());
}


void ossimNmeaMessage::parseMessage(std::istream& in)throw(ossimException)
{
   ossim::skipws(in);
   m_validCheckSum = false;
   m_message = "";
   if(!isValidStartChar(static_cast<char>(in.peek())))
   {
      throw ossimException(ossimString("Starting NMEA messsage indicator not found, expected one of ") +
                           m_startChars + " but found " +
                           ossimString((char)in.peek()));
   }
   
   char c = static_cast<char>(in.get());
   while(((c!='\n')&&(c!='\r'))&&
         !in.eof()&&!in.bad()) 
   {
      m_message += c;
      c = static_cast<char>(in.get());
   }
   std::string::iterator iter = std::find(m_message.begin(), m_message.end(), '*');
   
   if(iter != m_message.end())
   {
      setFields(m_message.begin()+1, m_message.end());
      ossim_uint32 check = checksum(m_message.begin()+1, iter);
      std::ostringstream out;
      out << std::setw(2) << std::setfill('0') << std::hex << check;
      std::string::iterator endChecksumIter = iter+1;
      while((endChecksumIter!= m_message.end())&&(*endChecksumIter!=',')) ++endChecksumIter;
      if(out.str() == ossimString(iter+1, endChecksumIter).downcase()) 
      {
         m_validCheckSum = true;
      }
   }
   else
   {
      throw ossimException("Terminating * indicator for cbecksum not found in NMEA message format");
   }
}
