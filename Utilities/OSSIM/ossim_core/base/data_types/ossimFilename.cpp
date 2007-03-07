//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero
// 
// Description: This class provides manipulation of filenames.
//
//*************************************************************************
// $Id: ossimFilename.cpp,v 1.55 2005/11/25 16:14:08 dburken Exp $

#include <ossimConfig.h>  /* to pick up platform defines */

#include <iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "ossimDirectory.h"

#if defined(_WIN32)
#  include <direct.h>
#else
#   if HAVE_UNISTD_H
#      include <unistd.h>
#   endif
#   if HAVE_DIRENT_H
#      include <dirent.h>
#   endif
#   if HAVE_FCNTL_H
#      include <fcntl.h>
#   endif
#endif

#include <sys/stat.h>

#ifdef __BORLANDC__
#  include <dir.h>
#  include <direct.h>
#  include <stdio.h>
#  include <stdlib.h>
#  include <io.h>
#endif

#include <base/data_types/ossimFilename.h>
#include <base/data_types/ossimRegExp.h>
#include <base/common/ossimConstants.h>
#include <base/context/ossimNotifyContext.h>

#if defined(_WIN32)
const char ossimFilename::thePathSeparator = '\\';
#else
const char ossimFilename::thePathSeparator = '/';
#endif

const ossimFilename ossimFilename::NIL=("");

ossimFilename::ossimFilename()
   : ossimString()
{}

ossimFilename::ossimFilename(const ossimFilename& src)
   : ossimString(src)
{}
ossimFilename::ossimFilename(const ossimString& src)
   : ossimString(src)
{}
ossimFilename::ossimFilename(const char* src)
   : ossimString(src)
{}

template <class Iter> ossimFilename::ossimFilename(Iter s, Iter e)
   : ossimString(s, e)
{}

bool ossimFilename::operator == (const ossimFilename& rhs)const
{
//    ossimFilename tempLeft = *this;
//    ossimFilename tempRight = rhs;

//    tempLeft.convertBackToForwardSlashes();
//    tempRight.convertBackToForwardSlashes();

   return ossimString::operator==(rhs);
//   return (*this==ossimString(rhs));
}

bool ossimFilename::operator == (const ossimString& rhs)const
{
   return ossimString::operator==(rhs);
}

bool ossimFilename::operator == (const char* rhs)const
{
   return ossimString::operator ==(rhs);
}

void ossimFilename::convertBackToForwardSlashes()
{
   ossimFilename::iterator currentChar = this->begin();

   while(currentChar != this->end())
   {
      if(*currentChar == '\\')
      {
         *currentChar = '/';
      }
      ++currentChar;
   }
}

void ossimFilename::convertForwardToBackSlashes()
{
   ossimFilename::iterator currentChar = this->begin();

   while(currentChar != this->end())
   {
      if(*currentChar == '/')
      {
         *currentChar = '\\';
      }
      ++currentChar;
   }
}


bool ossimFilename::exists() const
{
#if defined(_WIN32)

	struct _stat sbuf;
	return( _stat(c_str(), &sbuf ) == 0);
#else
	return ((access(c_str(), ossimFilename::OSSIM_EXIST)) == 0);
#endif
}

bool ossimFilename::isFile() const
{
#if defined(_WIN32)

	struct _stat sbuf;
	if ( _stat(c_str(), &sbuf ) == -1)
		return false;
	return (_S_IFMT & sbuf.st_mode ? true : false);
#else
	struct stat sbuf;

	stat(c_str(), &sbuf);
	return ((sbuf.st_mode & S_IFMT) == S_IFREG);
#endif
}

bool ossimFilename::isDir() const
{
#if defined(_WIN32)

	struct _stat sbuf;
	if ( _stat(c_str(), &sbuf ) == -1)
		return false;
   return (_S_IFDIR & sbuf.st_mode ? true : false);
#else
   struct stat sbuf;
   if (stat(c_str(), &sbuf) == -1)
	   return false;
   return (S_ISDIR(sbuf.st_mode));
#endif
}

bool ossimFilename::isReadable() const
{
#if defined(_WIN32)

	struct _stat sbuf;
	if ( _stat(c_str(), &sbuf ) == -1)
		return false;
	return (_S_IREAD & sbuf.st_mode ? true : false);
#else
	return (access(c_str(), ossimFilename::OSSIM_READ) == 0);
#endif
}

bool ossimFilename::isWritable() const
{
#if defined(_WIN32)

	struct _stat sbuf;
	if ( _stat(c_str(), &sbuf ) == -1)
		return false;
	return (_S_IWRITE & sbuf.st_mode ? true : false);
#else
	return (access(c_str(), ossimFilename::OSSIM_WRITE) == 0);
#endif
}

bool ossimFilename::isExecutable() const
{
#if defined(_WIN32)

	struct _stat sbuf;
	if ( _stat(c_str(), &sbuf ) == -1)
		return false;
	return (_S_IEXEC & sbuf.st_mode ? true : false);
#else
   return (access(c_str(), ossimFilename::OSSIM_EXE) == 0);
#endif
}

ossimString ossimFilename::ext() const
{
   ossimFilename file = *this;
   size_type pos = file.rfind('.');
   if (pos == npos)
   {
      return ossimFilename::NIL;
   }

   return ossimFilename(file.substr(pos+1));
}

ossimFilename ossimFilename::file() const
{
   ossimFilename file = *this;

   file.convertBackToForwardSlashes();

   size_type pos = file.rfind('/');
   if (pos == npos)
      return *this;
   else
      return ossimFilename(file.substr(pos+1));
}

ossimFilename ossimFilename::path() const
{
   ossimFilename file = *this;
   file.convertBackToForwardSlashes();

    size_type pos = file.rfind('/');

    if (pos == 0)
       return ossimFilename(ossimFilename("/"));
    if (pos == npos)
    {
       // We got to the end of the file and did not find a path separator.
       return ossimFilename::NIL;
    }

    return ossimFilename(file.substr(0, pos));
}

ossimFilename ossimFilename::drive()const
{
   ossimFilename tempFile(*this);
   ossimFilename result;

   tempFile.convertForwardToBackSlashes();

   ossimRegExp regEx("^([a-z|A-Z])+:");

   if(regEx.find(tempFile.c_str()))
   {
      result = ossimFilename(ossimString(this->begin() + regEx.start(),
                                         this->begin() + regEx.end()));
   }
   else
   {
      result = "";
   }

   return result;
}

ossimFilename ossimFilename::fileNoExtension()const
{
   ossimFilename f = *this;
   f.convertBackToForwardSlashes();

   size_type dot_pos   = f.rfind('.');
   size_type slash_pos = f.rfind('/');

   if(dot_pos == npos)
   {
      if(slash_pos == npos)
      {
         return *this;
      }
      else
      {
         return ossimFilename(this->begin()+slash_pos+1,
                              this->end());
      }
   }
   else if(slash_pos == npos)
   {
      return ossimFilename(this->begin(), this->begin()+dot_pos);
   }
   else if(slash_pos < dot_pos)
   {
      return ossimFilename(this->begin()+slash_pos+1,
                           this->begin() + dot_pos);
   }

   return ossimFilename(this->begin()+slash_pos+1,
                        this->end());

}
ossimFilename ossimFilename::noExtension()const
{
   ossimString drivePart;
   ossimString pathPart;
   ossimString filePart;
   ossimString extPart;

   split(drivePart, pathPart, filePart, extPart);

   extPart.clear();

   ossimFilename result;
   
   result.merge(drivePart, pathPart, filePart, extPart);
   
   return result;
}

ossimFilename& ossimFilename::setExtension(const ossimString& e)
{
   ossimString newExtPart = e;

   //---
   // If e has a dot "." in the front of it strip it off...
   //---
   if ( (*(e.begin())) == '.')
   {
      newExtPart = ossimString(e.begin() + 1, e.end());
   }

   ossimString drivePart;
   ossimString pathPart;
   ossimString filePart;
   ossimString extPart;

   split(drivePart,
         pathPart,
         filePart,
         extPart);

   merge(drivePart,
         pathPart,
         filePart,
         newExtPart);

   return *this;
}

ossimFilename& ossimFilename::setPath(const ossimString& p)
{
   ossimString drivePart;
   ossimString pathPart;
   ossimString filePart;
   ossimString extPart;

   split(drivePart,
         pathPart,
         filePart,
         extPart);

   merge(drivePart,
         p,
         filePart,
         extPart);

   return *this;
}

ossimFilename& ossimFilename::setFile(const ossimString& f)
{
   ossimString drivePart;
   ossimString pathPart;
   ossimString filePart;
   ossimString extPart;

   split(drivePart,
         pathPart,
         filePart,
         extPart);

   merge(drivePart,
         pathPart,
         f,
         extPart);

   return *this;
}


void ossimFilename::split(ossimString& drivePart,
                          ossimString& pathPart,
                          ossimString& filePart,
                          ossimString& extPart)const
{
   drivePart = drive();
   pathPart  = path();
   if(drivePart != "")
   {
      pathPart = pathPart.substitute(drivePart, "");
   }
   filePart  = fileNoExtension();
   extPart   = ext();
}

void ossimFilename::merge(const ossimString& drivePart,
                          const ossimString& pathPart,
                          const ossimString& filePart,
                          const ossimString& extPart)
{
   ossimFilename result = drivePart;

   if(pathPart != "")
   {
      result = result.dirCat(ossimFilename(pathPart));
   }

   if(filePart!="")
   {
      result = result.dirCat(ossimFilename(filePart));
   }

   if(extPart != "")
   {
      result += ".";
      result += extPart;
   }

   *this = result;
}

ossimFilename ossimFilename::dirCat(const ossimFilename& file) const
{
   // If this string is empty simply return the input file.
   if (empty()) return file;

   ossimFilename dir      = *this;
   ossimFilename tempFile = file;

   dir.convertBackToForwardSlashes();

   // Check the end and see if it already has a "/".
   string::const_iterator i = dir.end();

   --i; // decrement past the trailing null.

   if ( (*i) != '/')
   {
      dir += "/";
   }

   if( (*(tempFile.begin())) == '/') // let's not duplicate a forward slash
   {
      dir += ossimString(tempFile.begin() + 1,
                         tempFile.end());
   }
   else
   {
      dir += tempFile;  // Already has a separator.
   }

   return dir;
}

ossim_int64 ossimFilename::fileSize() const
{
   struct stat sbuf;

#ifndef __BORLANDC__
   if (stat(c_str(), &sbuf) == 0)
   {
      return (ossim_int64)sbuf.st_size;
   }
   else
   {
      ifstream in(c_str());
      if(in)
      {
         in.seekg(SEEK_END);
         return (ossim_int64)in.tellg();
      }
   }
#else
   ifstream in(c_str());
   if(in)
   {
      in.seekg(SEEK_END);
      return (ossim_int64)in.tellg();
   }
#endif
   return 0;
}

bool ossimFilename::createDirectory(bool recurseFlag,
                                  int perm)
{
   if(exists()) return true;

   ossimFilename tempFile = *this;
   tempFile.convertBackToForwardSlashes();
   vector<ossimString> result;


   if(recurseFlag)
   {
      ossimString::split(result,"/");

      if(result.size())
      {
         ossimString current;

         for(ossim_uint32 i = 1; i < result.size(); ++i)
         {
            current += ("/"+result[i]);

            if(current != "/")
            {
               if(!ossimFilename(current).exists())
               {

#if defined(__BORLANDC__)
                  if ( _mkdir(current.c_str()) != 0 )
#elif defined(_WIN32)
                  if ( mkdir(current.c_str()) != 0 )
#else
                  if ( mkdir(current.c_str(), perm) != 0 )
#endif
                  {
                     return false;
                  }
               }
            }
         }
      }
   }
   else
   {
#if defined (__BORLANDC__)
      if ( _mkdir(c_str()) != 0 )

#elif defined(_WIN32)
      if ( mkdir(c_str()) != 0 )
#else
      if ( mkdir(c_str(), perm) != 0 )
#endif
      {
         return false;
      }
      else
      {
         return true;
      }
   }
   return true;
}

bool ossimFilename::remove(const ossimFilename& pathname)
{
   bool result = true;
#if defined(__VISUALC__)  || defined(__BORLANDC__) || defined(__WATCOMC__) || \
    defined(__GNUWIN32__) || defined(_MSC_VER)
   
   if(::remove(pathname.c_str()) != 0)
   {
      result = false;
   }
#elif HAVE_UNISTD_H
   if(pathname.isDir())
   {
      result = (rmdir(pathname) >=0);
   }
   else if (unlink(pathname.c_str()) < 0)
   {
      result = false;
   }
#else
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "WARNING: "
         << "ossimFilename::remove,  Not supported by platform!"
         << endl;
   }
#endif /* HAVE_UNISTD_H */

   return result;
}

bool ossimFilename::wildcardRemove(const ossimFilename& pathname)
{
   std::vector<ossimFilename> fileListToRemove;
   ossimFilename tempPathname = pathname;
   
   if(!tempPathname.isDir())
   {
      ossimFilename file = tempPathname.file();
      ossimFilename path = tempPathname.path();
      if(path == "")
      {
         path = ".";
      }
      ossimDirectory dir;
      if(dir.open(path))
      {
         dir.findAllFilesThatMatch(fileListToRemove,
                                   file.c_str());
      }
      else
      {
      }
   }
   else
   {
      fileListToRemove.push_back(ossimFilename(pathname));
   }
   ossim_uint32 idx = 0;
   bool result = true;
   for(idx = 0; idx < fileListToRemove.size(); ++idx)
   {
#if defined(__VISUALC__)  || defined(__BORLANDC__) || defined(__WATCOMC__) || \
    defined(__GNUWIN32__) || defined(_MSC_VER)
      
      if(remove(fileListToRemove[idx].c_str()) != 0)
      {
         result = false;
      }
#elif HAVE_UNISTD_H
      if (unlink(fileListToRemove[idx]) == -1)
      {
         result = false;
      }
#else
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "WARNING: "
            << "ossimFilename::remove,  Not supported by platform!"
            << endl;
      }
      result = false;
#endif /* HAVE_UNISTD_H */
   }
   return result;
}
   
bool ossimFilename::remove()const
{
   return ossimFilename::remove(*this);
}

bool ossimFilename::wildcardRemove()const
{
   return ossimFilename::wildcardRemove(*this);
}
