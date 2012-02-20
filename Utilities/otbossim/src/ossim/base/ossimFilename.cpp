//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// 
// Description: This class provides manipulation of filenames.
//
//*************************************************************************
// $Id: ossimFilename.cpp 20192 2011-10-25 17:27:25Z dburken $

#include <ossim/ossimConfig.h>  /* to pick up platform defines */

#include <iostream>
#include <fstream>
using namespace std;

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimDate.h>
#include <ossim/base/ossimEnvironmentUtility.h>

#if defined(_WIN32)
#  include <io.h>
#  include <direct.h>
#  include <sys/utime.h>
#  include <windows.h>
#else
#  include <sys/types.h>
#  include <utime.h>
#  include <sys/stat.h>
#  include <unistd.h>
#  include <dirent.h>
#  include <fcntl.h>
#endif

#include <sys/stat.h>

#ifdef __BORLANDC__
#  include <dir.h>
#  include <direct.h>
#  include <stdlib.h>
#  include <io.h>
#endif

#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimRegExp.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimNotifyContext.h>

#if defined(_WIN32)
const char ossimFilename::thePathSeparator = '\\';
#else
const char ossimFilename::thePathSeparator = '/';
#endif


/**
 * This was taken from Wx widgets for performing touch and access date stamps.
 */ 
#if defined(_WIN32)
typedef WIN32_FIND_DATA FIND_STRUCT;
typedef HANDLE FIND_DATA;
typedef DWORD FIND_ATTR;

class ossimFileHandle
{
public:
   enum OpenMode
   {
      Read,
      Write
   };
   
   ossimFileHandle(const ossimString& filename, OpenMode mode)
   {
      m_hFile = ::CreateFile(
         filename.c_str(),              // name
         mode == Read ? GENERIC_READ    // access mask
         : GENERIC_WRITE,
         FILE_SHARE_READ |              // sharing mode
         FILE_SHARE_WRITE,              // (allow everything)
         NULL,                          // no secutity attr
         OPEN_EXISTING,                 // creation disposition
         0,                             // no flags
         NULL                           // no template file
         );
      
      if ( m_hFile == INVALID_HANDLE_VALUE )
      {
//             wxLogSysError(_("Failed to open '%s' for %s"),
//                           filename.c_str(),
//                           mode == Read ? _("reading") : _("writing"));
      }
   }
   
   ~ossimFileHandle()
   {
      if ( m_hFile != INVALID_HANDLE_VALUE )
      {
         if ( !::CloseHandle(m_hFile) )
         {
//                 wxLogSysError(_("Failed to close file handle"));
         }
      }
   }
   
   // return true only if the file could be opened successfully
   bool isOk() const { return m_hFile != INVALID_HANDLE_VALUE; }
   
   // get the handle
   operator HANDLE() const { return m_hFile; }
   
private:
   HANDLE m_hFile;
};

static void convertOssimToFileTime(FILETIME *ft, const ossimDate& dt)
{
   SYSTEMTIME st;
   st.wDay = dt.getDay();
   st.wMonth = (WORD)(dt.getMonth());
   st.wYear = (WORD)dt.getYear();
   st.wHour = dt.getHour();
   st.wMinute = dt.getMin();
   st.wSecond = dt.getSec();
//     st.wMilliseconds = dt.GetMillisecond();

   FILETIME ftLocal;
   if ( !::SystemTimeToFileTime(&st, &ftLocal) )
   {
//         wxLogLastError(_T("SystemTimeToFileTime"));
   }

   if ( !::LocalFileTimeToFileTime(&ftLocal, ft) )
   {
//         wxLogLastError(_T("LocalFileTimeToFileTime"));
   }
}

static void convertFileTimeToOssim(ossimLocalTm &dt, const FILETIME &ft)
{
   FILETIME ftcopy = ft;
   FILETIME ftLocal;
   if ( !::FileTimeToLocalFileTime(&ftcopy, &ftLocal) )
   {
//         wxLogLastError(_T("FileTimeToLocalFileTime"));
   }

   SYSTEMTIME st;
   if ( !::FileTimeToSystemTime(&ftLocal, &st) )
   {
//         wxLogLastError(_T("FileTimeToSystemTime"));
   }

   dt.setDay(st.wDay);
   dt.setMonth(st.wMonth);
   dt.setYear(st.wYear);
   dt.setHour(st.wHour);
   dt.setMin(st.wMinute);
   dt.setSec(st.wSecond);
    
//     dt->Set(st.wDay, wxDateTime::Month(st.wMonth - 1), st.wYear,
//             st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
}

static inline bool IsFindDataOk(FIND_DATA fd)
{
   return fd != INVALID_HANDLE_VALUE;
}

static inline FIND_DATA FindFirst(const ossimString& spec,
                                  FIND_STRUCT *finddata)
{
   return ::FindFirstFile(spec.c_str(), finddata);
}

static bool ossimGetDirectoryTimes(const ossimString& dirname,
                                   FILETIME *ftAccess,
                                   FILETIME *ftCreate,
                                   FILETIME *ftMod)
{

   FIND_STRUCT fs;
   FIND_DATA fd = FindFirst(dirname, &fs);
   if ( !IsFindDataOk(fd) )
   {
      return false;
   }

   *ftAccess = fs.ftLastAccessTime;
   *ftCreate = fs.ftCreationTime;
   *ftMod = fs.ftLastWriteTime;

   FindClose(fd);

   return true;
}
#endif

const ossimFilename ossimFilename::NIL=("");

ossimFilename::ossimFilename()
   : ossimString()
{}

ossimFilename::ossimFilename(const ossimFilename& src)
   : ossimString(src)
{
}

ossimFilename::ossimFilename(const ossimString& src)
   : ossimString(src)
{
   if ( m_str.size() )
   {
      convertToNative();
   }
}

ossimFilename::ossimFilename(const std::string& src)
   : ossimString(src)
{
   if ( m_str.size() )
   {
      convertToNative();
   }
}

ossimFilename::ossimFilename(const char* src)
   : ossimString(src)
{
   if ( m_str.size() )
   {
      convertToNative();
   }
}

template <class Iter> ossimFilename::ossimFilename(Iter s, Iter e)
   : ossimString(s, e)
{
   if ( m_str.size() )
   {
      convertToNative();
   }
}

bool ossimFilename::operator == (const ossimFilename& rhs)const
{
   return ossimString::operator==(rhs);
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
   std::string::iterator currentChar = this->begin();

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
   std::string::iterator currentChar = this->begin();

   while(currentChar != this->end())
   {
      if(*currentChar == '/')
      {
         *currentChar = '\\';
      }
      ++currentChar;
   }
}

bool ossimFilename::setTimes(ossimLocalTm* accessTime,
                             ossimLocalTm* modTime,
#if defined(_WIN32)                             
                             ossimLocalTm* createTime)const
#else
                             ossimLocalTm* /* createTime */ )const
#endif
{
#if defined(_WIN32)
   if(isDir())
   {
      // need to implement this later
      return false;
   }
   else
   {
      ossimFileHandle fh(this->expand(), ossimFileHandle::Write);
      if(fh.isOk())
      {
         FILETIME ftAccess, ftCreate, ftWrite;
         
         if ( createTime )
         {
            convertOssimToFileTime(&ftCreate, *createTime);
         }
         if ( accessTime )
         {
            convertOssimToFileTime(&ftAccess, *accessTime);
         }
         if ( modTime )
         {
            convertOssimToFileTime(&ftWrite, *modTime);
         }
         if ( ::SetFileTime(fh,
                            createTime ? &ftCreate : NULL,
                            accessTime ? &ftAccess : NULL,
                            modTime ? &ftWrite : NULL) )
         {
            return true;
         }
      }
   }
#else
   if ( !accessTime && !modTime )
   {
      // can't modify the creation time anyhow, don't try
      return true;
   }
   utimbuf utm;
   utm.actime = accessTime ? accessTime->getTicks() : modTime->getTicks();
   utm.modtime = modTime ? modTime->getTicks() : accessTime->getTicks();
   if ( utime(expand().c_str(), &utm) == 0 )
   {
      return true;
   }
    
#endif

   return false;
}

bool ossimFilename::getTimes(ossimLocalTm *accessTime,
                             ossimLocalTm *modTime,
                             ossimLocalTm *createTime) const
{
   if(!expand().exists()) return false;
   
#if defined(_WIN32)
   // we must use different methods for the files and directories under
   // Windows as CreateFile(GENERIC_READ) doesn't work for the directories and
   // CreateFile(FILE_FLAG_BACKUP_SEMANTICS) works -- but only under NT and
   // not 9x
   bool ok;
   FILETIME ftAccess, ftCreate, ftWrite;
   if ( isDir() )
   {
      ok = ossimGetDirectoryTimes(expand().c_str(),
                                  &ftAccess, &ftCreate, &ftWrite);
      ok = false;
   }
   else // file
   {
      ossimFileHandle fh(expand().c_str(), ossimFileHandle::Read);
      if ( fh.isOk() )
      {
         ok = ::GetFileTime(fh,
                            createTime ? &ftCreate : NULL,
                            accessTime ? &ftAccess : NULL,
                            modTime ? &ftWrite : NULL) != 0;
      }
      else
      {
         ok = false;
      }
   }

   if ( ok )
   {
      if ( createTime )
      {
         convertFileTimeToOssim(*createTime, ftCreate);
      }
      if ( accessTime )
      {
         convertFileTimeToOssim(*accessTime, ftAccess);
      }
      if ( modTime )
      {
         convertFileTimeToOssim(*modTime, ftWrite);
      }
        
      return true;
   }
#else
   struct stat sbuf;
   stat(c_str(), &sbuf);
   if ( stat( expand().c_str(), &sbuf) == 0 )
   {
      if ( accessTime )
      {
         *accessTime = ossimLocalTm(sbuf.st_atime);
      }
      if ( modTime )
      {
         *modTime = ossimLocalTm(sbuf.st_mtime);
      }
      if ( createTime )
      {
         *createTime = ossimLocalTm(sbuf.st_ctime);
      }
      return true;
   }
#endif // platforms


   return false;
}

bool ossimFilename::touch()const
{
#if defined( _WIN32 )
   ossimDate now;

   return setTimes(&now, &now, 0);
   
#else
   if ( utime(expand().c_str(), NULL) == 0 )
   {
      return true;
   }
   
   return false;
#endif   
}

ossimFilename ossimFilename::expand() const
{
   //---
   // Note:  ossimEnvironmentUtility::getCurrentWorkingDir() is returning
   // a blank string on windows with vs9.  This was resulting in seg faults
   // in this method so added checks were added for size of returned result.
   // (drb  20100113)
   //---
   ossimFilename result = "";
   if ( size() )
   {
      result = *this;
      
      if ( needsExpansion() )
      {

//#if defined(_WIN32)
//         result.convertBackToForwardSlashes();
//#endif

         bool addCwd = false;
         
         if ( (size() > 1) && (*(begin()) == '~') && (*(begin()+1) == thePathSeparator) )
         {
            ossimFilename homeDir =
               ossimEnvironmentUtility::instance()->getUserDir();
            
            ossimFilename s( (result.begin()+2) , result.end());
            result = homeDir.dirCat(s);
         }
         else if( (size() > 1) &&
                  (*(begin()) == '.') && (*(begin()+1) == thePathSeparator) )
         {
            // dot slash i.e. ./foo
            addCwd = true;
         }
         else if ( (size() > 2)  && (*(begin()) == '.')
                   && (*(begin()+1) == '.') && (*(begin()+2) == thePathSeparator) )
         {
            // ../foo
            addCwd = true;
         }
         else if (result == ".")
         {
            result = ossimEnvironmentUtility::instance()->
               getCurrentWorkingDir();
         }

         if (addCwd)
         {
            ossimFilename cwd = ossimEnvironmentUtility::instance()->
               getCurrentWorkingDir();
            result = cwd.dirCat(result);
         }
         else if ( result.isRelative() )
         {
            if ( result.size() && ((*(result.begin())) != '$') )
            {
               ossimFilename cwd = ossimEnvironmentUtility::instance()->
                  getCurrentWorkingDir();
               result = cwd.dirCat(result);
            }
         }
               
         // Check result to see if we're finished.
         if ( result.needsExpansion() )
         {
            // now expand any environment variable substitutions
            
            ossimFilename finalResult;
            const char* tempPtr = result.c_str();
            ossim_int32 startIdx = -1;
            ossim_int32 resultSize = (ossim_uint32)result.size();
            ossim_int32 scanIdx = 0;
            while(scanIdx < resultSize)
            {
               // look for start of substitution pattern
               if(tempPtr[scanIdx] == '$')
               {
                  if(tempPtr[scanIdx+1] == '(')
                  {
                     scanIdx +=2;
                     startIdx = scanIdx;
                  }
               }
               // look for an end pattern and apply if we found a start pattern
               else if(tempPtr[scanIdx] == ')')
               {
                  if(startIdx != -1)
                  {
                     ossimFilename value(
                        ossimEnvironmentUtility::instance()->
                        getEnvironmentVariable(ossimString(tempPtr+startIdx,
                                                           tempPtr+scanIdx)));
#if defined(_WIN32) // do windows style replacment
                     //                    value.convertBackToForwardSlashes();
#endif
                     finalResult += value;
                     // reset start idx indicator to not set so we are ready for next pattern
                     //
                     startIdx = -1;
                  }
                  else // if no start then tack on the )
                  {
                     finalResult += tempPtr[scanIdx];
                  }
                  ++scanIdx;
               }
               else if(startIdx == -1)
               {
                  finalResult += tempPtr[scanIdx];
                  ++scanIdx;
               }
               else
               {
                  ++scanIdx;
               }
            }
#if defined(_WIN32)

#else        
            finalResult.gsub("//", "/", true);
#endif       
            result = finalResult;
         
         } // matches:  if ( result.needsExpansion() )

#if defined(_WIN32)
         //        result.convertForwardToBackSlashes();
#endif        

      } // matches: if ( needsExpansion() )

      //---
      // If we had a size before "expand()" and now we don't something went
      // wrong...
      //---
      if (!result.size())
      {
         result = *this;
      }
      
   } // matches: if ( size() )
   
   return result;
}

bool ossimFilename::exists() const
{
   bool result = false;
#if defined(_WIN32)
   result = (_access(c_str(), ossimFilename::OSSIM_EXIST) == 0);
#else
   result = ((access(c_str(), ossimFilename::OSSIM_EXIST)) == 0);
#endif
   return result;
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
   if ( empty() )
   {
      return false;
   }
   
   ossimFilename temp = c_str();
   const char& lastChar = temp[temp.size()-1];
   if ( lastChar == '/' || lastChar == '\\' )
   {
      temp = temp.beforePos(temp.size() - 1);
   }
	
#if defined(_WIN32)

   struct _stat sbuf;
   if ( _stat(temp.c_str(), &sbuf ) == -1)
      return false;
   return (_S_IFDIR & sbuf.st_mode ? true : false);
#else
   struct stat sbuf;
   if (stat(temp.c_str(), &sbuf) == -1)
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

bool ossimFilename::isWriteable() const
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
   std::string::size_type pos = file.m_str.rfind('.');
   if (pos == std::string::npos)
   {
      return ossimFilename::NIL;
   }

   return ossimFilename(file.m_str.substr(pos+1));
}

ossimFilename ossimFilename::file() const
{
   ossimFilename file = *this;

   //file.convertBackToForwardSlashes();

   std::string::size_type pos = file.m_str.rfind(thePathSeparator);
   if (pos == std::string::npos)
      return *this;
   else
      return ossimFilename(file.m_str.substr(pos+1));
}

ossimFilename ossimFilename::path() const
{
   ossimFilename file = *this;
   //file.convertBackToForwardSlashes();

   // finds the last occurrence of the given string; in this case '/';
   std::string::size_type pos = file.m_str.rfind(thePathSeparator);

   if (pos == 0)
      return ossimFilename(ossimFilename(thePathSeparator));
   if (pos == std::string::npos)
   {
      // We got to the end of the file and did not find a path separator.
      return ossimFilename::NIL;
   }

   return ossimFilename(file.m_str.substr(0, pos));
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
   //f.convertBackToForwardSlashes();

   std::string::size_type dot_pos   = f.m_str.rfind('.');
   std::string::size_type slash_pos = f.m_str.rfind(thePathSeparator);

   if(dot_pos == std::string::npos)
   {
      if(slash_pos == std::string::npos)
      {
         return *this;
      }
      else
      {
         return ossimFilename(this->begin()+slash_pos+1,
                              this->end());
      }
   }
   else if(slash_pos == std::string::npos)
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
   if ( (e.begin() != e.end()) && ((*(e.begin())) == '.') )
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

ossimFilename& ossimFilename::setDrive(const ossimString& d)
{
   ossimString drivePart;
   ossimString pathPart;
   ossimString filePart;
   ossimString extPart;

   split(drivePart,
         pathPart,
         filePart,
         extPart);

   merge(d,
         pathPart,
         filePart,
         extPart);

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
   if (file.empty()) return *this;

   ossimFilename dir      = *this;
   ossimFilename tempFile = file;
      
   // Check the end and see if it already has a "/".
   string::const_iterator i = dir.end();

   --i; // decrement past the trailing null.

   if ( (*i) != thePathSeparator)
   {
      dir += ossimString(thePathSeparator);
   }

   // check for dot slash or just slash: ./foo or /foo   
   std::string::iterator iter = tempFile.begin();
   if (iter != tempFile.end())
   {
      if ((*iter) == thePathSeparator)
      {
         ++iter; // skip slash
      }
      else if (tempFile.size() > 1)
      {
         if ( ((*iter) == '.') &&  ( *(iter + 1) == thePathSeparator) )
         {
            iter = iter + 2; // skip dot slash
         }
      }
   }   

   dir += std::string(iter, tempFile.end());

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

bool ossimFilename::createDirectory( bool recurseFlag,
                                     int perm ) const
{
   if(exists()) return true;

   if ( empty() ) return false;

   if(recurseFlag)
   {
      ossimString tempString = this->expand().c_str();

      vector<ossimString> result;
      tempString.split(result,thePathSeparator);

      if(result.size())
      {
         ossimString current = result[0];

// Reconstruct UNC paths under Windows.
#if defined(_WIN32)
         bool bGotUNC = false;
         if ( current.length() == 0 && tempString.length() > 2 )
         {
            const char* fstr = tempString.c_str();
            const char fstar0 = fstr[0];
            const char fstar1 = fstr[1];
            if ( fstar0=='\\' && fstar1=='\\' )
            {
               bGotUNC = true;
               current = thePathSeparator;
            }
         }
#endif

         for(ossim_uint32 i = 1; i < result.size(); ++i)
         {
            current += (thePathSeparator+result[i]);

#if defined(_WIN32)
            if ( bGotUNC == true && i==1 )
            {
               // The root of the UNC path is assumed to exist.
               continue;
            }
#endif
            
            if(current != thePathSeparator)
            {
               if(!ossimFilename(current).exists())
               {
#if defined(__BORLANDC__)
                  if ( _mkdir(current.c_str()) != 0 )
#elif defined(_WIN32)
                  if ( _mkdir(current.c_str()) != 0 )
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
      if ( _mkdir(c_str()) != 0 )
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

   // Note: not sure if these work on all of the above flavors. drb - 14 Sep. 2011.
   if(pathname.isDir())
   {
      // Note this only removes empty directories.
      result = ( RemoveDirectory( pathname.c_str() ) != 0 );
   }
   else
   {
      result = ( DeleteFile( pathname.c_str() ) != 0 );
   }
#else /* Unix flavor from unistd.h. */
   if(pathname.isDir())
   {
      result = ( rmdir( pathname.c_str() ) == 0 );
   }
   else
   {
      result = ( unlink( pathname.c_str() ) == 0 );
   }
#endif

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
#else
      if (unlink(fileListToRemove[idx]) == -1)
      {
         result = false;
      }
#endif /* HAVE_UNISTD_H */
   }
   return result;
}

bool ossimFilename::rename(const ossimFilename& destFile, bool overwriteDestinationFlag)const
{
   if(!overwriteDestinationFlag)
   {
      if(destFile.exists())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "WARNING: "
            << "ossimFilename::rename WARNING:"
            << "\nDestination File Exists: " << destFile << std::endl;
         return false;
      }
   }
   else if(destFile.exists())
   {
      destFile.remove();
   }
   ::rename(this->c_str(), destFile.c_str());
   
   return true;
}
   
bool ossimFilename::remove()const
{
   return ossimFilename::remove(*this);
}

bool ossimFilename::wildcardRemove()const
{
   return ossimFilename::wildcardRemove(*this);
}

bool ossimFilename::copyFileTo(const ossimFilename& outputFile) const
{
   bool result = false;
   
   std::ifstream is(this->c_str(), std::ios::in|std::ios::binary);
   if ( is.good() )
   {
      ossimFilename f = outputFile;
      if ( f.isDir() )
      {
         f = f.dirCat( this->file() );
      }

      if ( f != *this )
      {
         std::ofstream os( f.c_str(), std::ios::out|std::ios::binary );
         if ( os.good() )
         {
            // Copy the file:
            char c;
            while(is.get(c))
            {
               os.put(c);
            }
            
            if ( is.eof() &&  !os.fail())
            {
               result = true;
            }
            else
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "WARNING: "
                  << "ossimFilename::copyFileTo WARNING:"
                  << "\nError detected writing from file "
                  << this->c_str() << " to file " << f.c_str() << std::endl;
            }
         }
         else
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "WARNING: "
               << "ossimFilename::copyFileTo WARNING:"
               << "\nCannot open: " << f.c_str() << std::endl;
         }
      } //  if ( f != *this )
      else
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "WARNING: "
            << "ossimFilename::copyFileTo WARNING:"
            << "\nFiles the same!" << std::endl;
      }
      
   } // if ( is.good() )
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "WARNING: "
         << "ossimFilename::copyFileTo WARNING:"
         << "\nCannot open: " << this->c_str() << std::endl;
   }
   
   return result;
}

//---
// We will only return false if we are absolutely sure absolutely sure we
// are not relative. No pun intended:)
//---
bool ossimFilename::isRelative() const
{
   bool result = true;
   if (size())
   {
      //---
      // Look for unix "/"...
      // ESH: Look for Windows "\" (with prepending escape character \)
      //---
      if ( (*(begin()) == '/') || (*(begin()) == '\\') )
      {
         result = false;
      }
      else
      {
         // Look for windows drive
         ossimRegExp regEx("^([a-z|A-Z])+:");
         if ( regEx.find(c_str()) == true)
         {
            result = false;
         }
      }
   }
   return result;
}

bool ossimFilename::needsExpansion() const
{
   bool result = false;
   if ( size() )
   {
      result = isRelative();
      if (result == false)
      {
         // Check for '$'
         std::string::size_type pos = m_str.find('$', 0);
         {
            if (pos != std::string::npos)
            {
               // found '$'
               result = true;
            }
         }
      }    
   }
   return result;
}

char ossimFilename::getPathSeparator() const
{
   return thePathSeparator;
}

void ossimFilename::convertToNative()
{
#if defined(_WIN32)
   convertForwardToBackSlashes();
#else
   convertBackToForwardSlashes();
#endif
	
}
