//*******************************************************************
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Contains class definition for ossimNotify.
//*******************************************************************
//  $Id: ossimNotify.cpp 9371 2006-08-02 22:03:44Z dburken $

#include <iostream>
#include <cstdio>
#include <cstdarg>
#include <stack>
#include <cstddef>

#include <ossim/base/ossimNotify.h>

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
         if(theLogFilename != "")
         {
            std::ofstream outFile(theLogFilename.c_str(),
                                  std::ios::app|std::ios::out);
            if(outFile)
            {
               outFile.write(tempString.c_str(), tempString.length());
            }
            
            tempString = "";
         }
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


