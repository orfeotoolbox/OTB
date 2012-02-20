//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file
//
// Author: Garrett Potts
//
// Description: This is an initial cut at an http response object.  The HttpResponse is
//              returned from the HttpRequest base object.
//----------------------------------------------------------------------------
// $Id$
#ifndef ossimHttpResponse_HEADER
#define ossimHttpResponse_HEADER

#include <iostream>
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimByteStreamBuffer.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimWebResponse.h>

class OSSIM_DLL ossimHttpResponse : public ossimWebResponse
{
public:
   ossimHttpResponse()
   :m_headerStream(&m_headerBuffer),
   m_bodyStream(&m_bodyBuffer)
   {
      clear();
   }
   
   ossimByteStreamBuffer& headerBuffer(){return m_headerBuffer;}
   const ossimByteStreamBuffer& headerBuffer()const{return m_headerBuffer;}
   ossimByteStreamBuffer& bodyBuffer(){return m_bodyBuffer;}
   const ossimByteStreamBuffer& bodyBuffer()const{return m_bodyBuffer;}
   
   std::iostream& headerStream(){return m_headerStream;}
   std::iostream& bodyStream(){return m_bodyStream;}
   
   virtual std::istream* getInputStream()
         {return static_cast<std::istream*>(&m_bodyStream);}
   
   /**
    * Clears out the Response and prepares for a new response.
    */
   virtual void clear()
   {
      m_headerBuffer.clear();
      m_bodyBuffer.clear();
      m_headerKwl.clear();
      m_statusLine = "";
      m_statusCode = 200;
   }
   
   /**
    * This will parse out the response code from the status line and initialize
    * the header variables into a keywordlist.
    */
   void convertHeaderStreamToKeywordlist();
   virtual void clearLastError(){m_statusCode = 200;m_statusLine="";}
   
   virtual ossimString getLastError()const{return ((m_statusCode == 200)?ossimString(""):m_statusLine);}
   
   ossimKeywordlist& headerKwl(){return m_headerKwl;}
   const ossimKeywordlist& headerKwl()const{return m_headerKwl;}
   
   const ossimString& statusLine()const{return m_statusLine;}
   ossim_uint32 getStatusCode()const{return m_statusCode;}
   
protected:
   ossimKeywordlist      m_headerKwl;
   ossimByteStreamBuffer m_headerBuffer;
   ossimByteStreamBuffer m_bodyBuffer;
   std::iostream         m_headerStream;
   std::iostream         m_bodyStream;
   ossimString           m_statusLine;
   ossim_uint32          m_statusCode;
   
TYPE_DATA;
};
#endif

