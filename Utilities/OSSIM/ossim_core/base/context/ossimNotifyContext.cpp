//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// Contains class declaration for ossimErrorContext.
//*******************************************************************
//  $Id: ossimNotifyContext.cpp,v 1.15 2005/10/17 18:37:16 gpotts Exp $
#include <iostream>
#include <base/context/ossimNotifyContext.h>
#include <stdio.h>
#include <stdarg.h>
#include <stack>
#ifndef NULL
#include <stddef.h>
#endif





static std::ostream* theOssimFatalStream  = &std::cerr;
static std::ostream* theOssimWarnStream   = &std::cerr;
static std::ostream* theOssimInfoStream   = &std::cout;
static std::ostream* theOssimNoticeStream = &std::cout;
static std::ostream* theOssimDebugStream  = &std::cout;
static std::ostream* theOssimAlwaysStream = &std::cout;

static ossimNotifyFlags theNotifyFlags     = ossimNotifyFlags_ALL;
std::stack<ossimNotifyFlags> theNotifyFlagsStack;

template <class charT, class traits = std::char_traits<charT> >
class ossimNullBufferStream : public std::basic_streambuf<charT, traits>
{
public:
   ossimNullBufferStream(){};


protected:

   std::streamsize xsputn(const charT * pChar, std::streamsize n)
      {
         return 0;
      }

private:
   ossimNullBufferStream(const ossimNullBufferStream&);
   ossimNullBufferStream& operator=(const ossimNullBufferStream&);
};

template <class charT, class traits = std::char_traits<charT> >
class ossimLogFileBufferStream : public std::basic_streambuf<charT, traits>
{
public:
   ossimLogFileBufferStream(){};

   void setLogFilename(const ossimFilename& file)
      {
         theLogFilename = file;
      }
   ossimFilename getLogFilename()const
      {
         return theLogFilename;
      }

protected:
   ossimFilename theLogFilename;
   virtual int overflow(int c)
      {
         if(!traits::eq_int_type(c, traits::eof()))
         {
            tempString = tempString + ossimString(c);
         }

         return c;
      }

   virtual std::streamsize xsputn(const charT * pChar, std::streamsize n)
      {
         tempString = tempString + ossimString(pChar, pChar + n);

         return n;
      }

   virtual int sync()
      {
         std::ofstream outFile(theLogFilename.c_str(),
                          std::ios::app|std::ios::out);
         if(outFile)
         {
            outFile.write(tempString.c_str(), tempString.length());
         }

         tempString = "";
         return 0;
      }

private:
   ossimString tempString;

   ossimLogFileBufferStream(const ossimLogFileBufferStream&);
   ossimLogFileBufferStream& operator=(const ossimLogFileBufferStream&);
};


class ossimNullStream : public std::ostream
{
public:
   ossimNullStream() : std::ostream(&nullBufferStream){}
   virtual ~ossimNullStream()
      {
         nullBufferStream.pubsync();
      }

private:
   ossimNullBufferStream<char> nullBufferStream;
   // Copy & assignment are undefined in iostreams
   ossimNullStream(const ossimNullStream&);
   ossimNullStream & operator=(const ossimNullStream&);
};

class ossimLogFileStream : public std::ostream
{
public:
   ossimLogFileStream() : std::ostream(&theLogFileBufferStream){}
   virtual ~ossimLogFileStream()
      {
         theLogFileBufferStream.pubsync();
      }
   void setLogFilename(const ossimFilename& filename)
      {
         theLogFileBufferStream.setLogFilename(filename);
      }
   ossimFilename getLogFilename()const
      {
         return theLogFileBufferStream.getLogFilename();
      }

private:
   ossimLogFileBufferStream<char> theLogFileBufferStream;
   // Copy & assignment are undefined in iostreams
   ossimLogFileStream(const ossimLogFileStream&);
   ossimLogFileStream & operator=(const ossimLogFileStream&);
};

static ossimNullStream    theOssimNullStream;
static ossimLogFileStream theLogFileStream;

void ossimSetDefaultNotifyHandlers()
{
   theOssimFatalStream  = &std::cerr;
   theOssimWarnStream   = &std::cout;
   theOssimInfoStream   = &std::cout;
   theOssimNoticeStream = &std::cout;
   theOssimDebugStream  = &std::cout;
   theOssimAlwaysStream = &std::cout;
}

void ossimSetNotifyStream(std::ostream* outputStream,
                          ossimNotifyFlags whichLevelsToRedirect)
{
   if(whichLevelsToRedirect&ossimNotifyFlags_FATAL)
   {
      theOssimFatalStream = outputStream;
   }
   if(whichLevelsToRedirect&ossimNotifyFlags_WARN)
   {
      theOssimWarnStream = outputStream;
   }
   if(whichLevelsToRedirect&ossimNotifyFlags_INFO)
   {
      theOssimInfoStream = outputStream;
   }
   if(whichLevelsToRedirect&ossimNotifyFlags_NOTICE)
   {
      theOssimNoticeStream = outputStream;
   }
   if(whichLevelsToRedirect&ossimNotifyFlags_DEBUG)
   {
      theOssimDebugStream = outputStream;
   }
}

std::ostream* ossimGetNotifyStream(ossimNotifyLevel whichLevel)
{
   std::ostream* notifyStream = &theOssimNullStream;

   switch(whichLevel)
   {
      case ossimNotifyLevel_ALWAYS:
      {
         notifyStream = theOssimAlwaysStream;
         break;
      }
      case ossimNotifyLevel_FATAL:
      {
         notifyStream = theOssimFatalStream;
         break;
      }
      case ossimNotifyLevel_WARN:
      {
         notifyStream = theOssimWarnStream;
         break;
      }
      case ossimNotifyLevel_INFO:
      {
         notifyStream = theOssimInfoStream;
         break;
      }
      case ossimNotifyLevel_NOTICE:
      {
         notifyStream = theOssimNoticeStream;
         break;
      }
      case ossimNotifyLevel_DEBUG:
      {
         notifyStream = theOssimDebugStream;
         break;
      }
   }
   return notifyStream;
}

OSSIMDLLEXPORT std::ostream& ossimNotify(ossimNotifyLevel level)
{
   if(ossimIsReportingEnabled())
   {
      if(theLogFileStream.getLogFilename() != "")
      {
         return theLogFileStream;
      }
      else
      {
         bool reportMessageFlag = false;
         switch(level)
         {
            case ossimNotifyLevel_ALWAYS:
            {
               reportMessageFlag = true;
               break;
            }
            case ossimNotifyLevel_FATAL:
            {
               if(theNotifyFlags&ossimNotifyFlags_FATAL)
               {
                  reportMessageFlag = true;
               }
               break;
            }
            case ossimNotifyLevel_WARN:
            {
               if(theNotifyFlags&ossimNotifyFlags_WARN)
               {
                  reportMessageFlag = true;
               }
               break;
            }
            case ossimNotifyLevel_INFO:
            {
               if(theNotifyFlags&ossimNotifyFlags_INFO)
               {
                  reportMessageFlag = true;
               }
               break;
            }
            case ossimNotifyLevel_NOTICE:
            {
               if(theNotifyFlags&ossimNotifyFlags_NOTICE)
               {
                  reportMessageFlag = true;
               }
               break;
            }
            case ossimNotifyLevel_DEBUG:
            {
               if(theNotifyFlags&ossimNotifyFlags_DEBUG)
               {
                  reportMessageFlag = true;
               }
               break;
            }
         }
         if(reportMessageFlag)
         {
            return *ossimGetNotifyStream(level);
         }
      }
   }

   return theOssimNullStream;
}

void ossimSetLogFilename(const ossimFilename& filename)
{
   theLogFileStream.setLogFilename(filename);
}

ossimString ossimErrorV(const char *fmt, va_list args )
{
   char temp[2024];
   if(fmt)
   {
      vsprintf(temp, fmt, args);
   }
   else
   {
      sprintf(temp,"%s", "");
   }

   return temp;
}

void ossimEnableNotify(ossimNotifyFlags flags)
{
   theNotifyFlags = (ossimNotifyFlags)(theNotifyFlags | flags);
}

void ossimDisableNotify(ossimNotifyFlags flags)
{
   theNotifyFlags = (ossimNotifyFlags)((ossimNotifyFlags_ALL^flags)&
                                       theNotifyFlags);
}

void ossimSetNotifyFlag(ossimNotifyFlags notifyFlags)
{
   theNotifyFlags = notifyFlags;
}

void ossimPushNotifyFlags()
{
   theNotifyFlagsStack.push(theNotifyFlags);
}

void ossimPopNotifyFlags()
{
   if(theNotifyFlagsStack.empty())
   {
      return;
   }
   theNotifyFlags = theNotifyFlagsStack.top();
   theNotifyFlagsStack.pop();
}

ossimNotifyFlags ossimGetNotifyFlags()
{
   return theNotifyFlags;
}



bool ossimIsReportingEnabled()
{
   return  (theNotifyFlags != ossimNotifyFlags_NONE);
}


void ossimNotify(ossimString msg,
                 ossimNotifyLevel notifyLevel)
{
   ossimNotify(notifyLevel) << msg << "\n";
}

void ossimSetError( const char *className,
                    ossim_int32 error,
                    const char *fmtString=NULL, ...)
{
    va_list args;

    va_start(args, fmtString);
    ossimString result = ossimErrorV(fmtString, args );
    va_end(args);
    ossimNotify(ossimNotifyLevel_WARN) << result << "\n";
}

void ossimSetInfo( const char *className,
                    const char *fmtString=NULL, ...)
{
    va_list args;

    va_start(args, fmtString);
    ossimString result = ossimErrorV(fmtString, args );
    va_end(args);
    ossimNotify(ossimNotifyLevel_WARN) << result << "\n";
}


