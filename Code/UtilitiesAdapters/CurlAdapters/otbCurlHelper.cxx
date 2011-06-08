/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbCurlHelper.h"

#include <sstream>

#include "otbMacro.h"

#ifdef OTB_USE_CURL
#include <curl/curl.h>
#include <cstring>
#endif

#include <cstdio>


/**
 * Macro to handle the curl code errors, and return a string
 * describing the code error
 */
#define otbCurlCall(command)                                               \
  {                                                                        \
    {                                                                      \
    CURLcode curlCode;                                                     \
    curlCode = command;                                                    \
    if(curlCode != CURLE_OK)                                               \
      {                                                                    \
      itkExceptionMacro(<<" Curl Error : "<< curl_easy_strerror(curlCode)) \
      }                                                                    \
    }                                                                      \
  }

/**
 * Macro to handle the multi curl code errors, and return a string
 * describing the code error
 */
#define otbCurlMultiCall(command)                                           \
  {                                                                         \
    {                                                                       \
    CURLMcode curlmultiCode;                                                \
    curlmultiCode = command;                                                \
    if(curlmultiCode != CURLM_OK)                                           \
      {                                                                     \
      itkExceptionMacro(<<" CurlM Error : "<< curl_multi_strerror(curlmultiCode))\
      }                                                                     \
    }                                                                       \
  }

namespace otb
{

/**
 * Resource class that create and clean the curl environment proprely
 * in case of a thrown exception
 */
class CurlResource {

public:
  CurlResource()
  {
    m_Curl = curl_easy_init();
    
    if (!m_Curl) 
      {
      itkExceptionMacro(<<" otbCurlHelper::CurlResource Curl handle init error.");
      }
  }

  ~CurlResource() 
  {
    curl_easy_cleanup(m_Curl);
  }

  // Get Name of Class in order to use the itkExcpetionMacro
  std::string GetNameOfClass(){ return "CurlResource";}

  CURL * GetCurlResource()
  {
    return m_Curl;
  }

private:
  CURL * m_Curl;
  // prevent copying and assignment; not implemented
  CurlResource (const CurlResource &);
  CurlResource & operator= (const CurlResource &);
};  //end of class CurlResource

/**
 * Resource class that create and clean the curl multi environment
 * proprely in case of a thrown exception
 */
class CurlMultiResource {

public:
  CurlMultiResource ()
  {
    m_Curl = curl_multi_init();
    
    if (!m_Curl) 
      {
      itkExceptionMacro(<<" otbCurlHelper::CurlMultiResource Curl multi handle init error.");
      }
  }

  ~CurlMultiResource() 
  {
    curl_multi_cleanup(m_Curl);
  }

  // Get Name of Class in order to use the itkExcpetionMacro
  std::string GetNameOfClass(){ return "CurlMultiResource";}

  CURLM * GetCurlMultiResource()
  {
    return m_Curl;
  }

private:
  CURLM * m_Curl;
  // prevent copying and assignment; not implemented
  CurlMultiResource (const CurlMultiResource &);
  CurlMultiResource & operator= (const CurlMultiResource &);
}; //end of class CurlMultiResource


/**
 * Resource class that create FILE * and  close the FILE *  descriptor
 * proprely in case of a thrown exception
 */
class FileResource {

public:
  FileResource (const char* infname)
  {
    m_File = fopen(infname, "wb");
    
    if (m_File == NULL) 
      {
      itkExceptionMacro(<<" otbCurlHelper::FileResource : failed to open the file ."<< infname);
      }
  }

  ~FileResource() 
  {
    fclose(m_File);
  }

  // Get Name of Class in order to use the itkExcpetionMacro
  std::string GetNameOfClass(){ return "FileResource";}

  FILE * GetFileResource()
  {
    return m_File;
  }

private:
  FILE * m_File;
  // prevent copying and assignment; not implemented
  FileResource (const FileResource &);
  FileResource & operator= (const FileResource &);
}; //end of class FileResource

bool CurlHelper::TestUrlAvailability(const std::string& url) const
{
#ifdef OTB_USE_CURL
  // Set up a curl resource
  CurlResource curlResource; 

  // Boolean to return
  bool res = false;

  otbCurlCall(curl_easy_setopt(curlResource.GetCurlResource(), CURLOPT_USERAGENT, m_Browser.data()));
  otbCurlCall(curl_easy_setopt(curlResource.GetCurlResource(), CURLOPT_URL, url.data()));
  // Set the dummy write function
  otbCurlCall(curl_easy_setopt(curlResource.GetCurlResource(), CURLOPT_WRITEFUNCTION, &Self::CallbackWriteDataDummy));
  otbCurlCall(curl_easy_setopt(curlResource.GetCurlResource(), CURLOPT_MAXFILESIZE, 1));
  
  // Perform requet
  CURLcode easyPerformResult = curl_easy_perform(curlResource.GetCurlResource());
  
  otbMsgDevMacro(<< "CurlHelper::TestUrlAvailability : curl_easy_perform returned "
                 << easyPerformResult << " --> "
                 << curl_easy_strerror(easyPerformResult));

  // Check the curl_easy_perform return code : actually we tests only
  // the availability of the url, so if the file requested size is less
  // than 1 byte, and the url is valid it returns CURLE_OK, or if the
  // url is valid and the file size is greater than 1 byte it returns
  // the error CURLE_FILESIZE_EXCEEDED 
  if ( easyPerformResult == CURLE_OK || easyPerformResult == CURLE_FILESIZE_EXCEEDED ) 
    {
    return true;
    }

  return res;
#else
  otbMsgDevMacro(<< "Curl is not available, compile with OTB_USE_CURL to ON");
  return false;
#endif
}

int CurlHelper::RetrieveUrlInMemory(const std::string& url, std::string& output) const
{
#ifdef OTB_USE_CURL
  otbMsgDevMacro(<< "Retrieving: " << url);
  CURLcode res = CURLE_OK;
  CurlResource curlResource; 
  
  otbCurlCall(curl_easy_setopt(curlResource.GetCurlResource(), CURLOPT_URL, url.c_str()));
  
  // Set 5s timeout
  otbCurlCall(curl_easy_setopt(curlResource.GetCurlResource(), CURLOPT_TIMEOUT, 10));
  
  // Use our writing static function to avoid file descriptor
  // pointer crash on windows
  otbCurlCall(curl_easy_setopt(curlResource.GetCurlResource(), CURLOPT_WRITEFUNCTION, 
                               &Self::CallbackWriteDataToStringStream));
  
  // Say the file where to write the received data
  std::ostringstream* outputStream = new std::ostringstream;
  otbCurlCall(curl_easy_setopt(curlResource.GetCurlResource(), CURLOPT_WRITEDATA, (void*) outputStream));
  
  // Perform request
  otbCurlCall(curl_easy_perform(curlResource.GetCurlResource()));
  
  // Save output
  output = outputStream->str();
  otbMsgDevMacro("curl output : " << output);
  
  // Clean up
  delete outputStream;
  
  otbMsgDevMacro(<< " -> " << res);
  return res;
#else
  otbMsgDevMacro(<< "Curl is not available, compile with OTB_USE_CURL to ON");
  return -1;
#endif
}

int CurlHelper::RetrieveFile(const std::ostringstream& urlStream, std::string filename) const
{
  return RetrieveFile(urlStream.str(), filename);
}

int CurlHelper::RetrieveFile(const std::string& urlString, std::string filename) const
{
#ifdef OTB_USE_CURL
  otbMsgDevMacro(<< "Retrieving: " << url);
  CURLcode res = CURLE_OK;
  
  CurlResource curlResource; 

  FileResource output_file(filename.c_str());

  char url[256];
  strcpy(url, urlString.c_str());

  otbCurlCall(curl_easy_setopt(curlResource.GetCurlResource(), CURLOPT_URL, url));

  // Set 10s timeout
  otbCurlCall(curl_easy_setopt(curlResource.GetCurlResource(), CURLOPT_TIMEOUT, 10));

  // Use our writing static function to avoid file descriptor
  // pointer crash on windows
  otbCurlCall(curl_easy_setopt(curlResource.GetCurlResource(), CURLOPT_WRITEFUNCTION, 
                               &Self::CallbackWriteDataToFile));

  // Say the file where to write the received data
  otbCurlCall(curl_easy_setopt(curlResource.GetCurlResource(), CURLOPT_WRITEDATA, 
                               (void*) output_file.GetFileResource()));

  otbCurlCall(curl_easy_perform(curlResource.GetCurlResource()));
    
  otbMsgDevMacro(<< " -> " << res);
  return res;
#else
  otbMsgDevMacro(<< "Curl is not available, compile with OTB_USE_CURL to ON");
  return -1;
#endif
}

int CurlHelper::RetrieveFileMulti(const std::vector<std::string>& listURLs,
                                  const std::vector<std::string>& listFilename,
                                  int maxConnect) const
{
#ifdef OTB_USE_CURL
#ifdef OTB_CURL_MULTI_AVAILABLE
  otbMsgDevMacro(<< "Using curl multi");

  // Initialize curl handle resource
  CurlMultiResource   multiHandle;
  
  std::vector<CURL *> listCurlHandles;
  std::vector<FILE *> listFiles;

  std::vector<std::string>::const_iterator filename;
  filename = listFilename.begin();
  while (filename != listFilename.end())
    {
    FILE* lOutputFile = fopen((*filename).c_str(), "wb");
    if (lOutputFile == NULL)
      {
      itkExceptionMacro(<< "otbCurlHelper: bad file name: " << (*filename).c_str());
      }

    // Add file to vector
    listFiles.push_back(lOutputFile);
    ++filename;
    }

  listCurlHandles.clear();
  
  std::vector<std::string>::const_iterator url;
  std::vector<FILE *>::const_iterator      file;
  url = listURLs.begin();
  file = listFiles.begin();
  while ((url != listURLs.end()) && (file != listFiles.end()))
    {
    otbMsgDevMacro(<< "Retrieving: " << (*url).data());
    CURL * lEasyHandle;
    lEasyHandle = curl_easy_init();

    if (!lEasyHandle)
      {
      itkExceptionMacro(<< "otbCurlHelper: Curl easy handle init error.");
      }

    // Param easy handle
    // the otbCurlCall is not used here cause we are not using the
    // CurlResource class : Using the CurlResource class in the loop
    // will make the object CurlResource destroyed at the end of each
    // loop, making the multi handle not working.
    curl_easy_setopt(lEasyHandle, CURLOPT_USERAGENT, m_Browser.data());
    curl_easy_setopt(lEasyHandle, CURLOPT_URL, (*url).data());
    curl_easy_setopt(lEasyHandle, CURLOPT_WRITEFUNCTION, &Self::CallbackWriteDataToFile);
    curl_easy_setopt(lEasyHandle, CURLOPT_WRITEDATA, (void*) (*file));

    // Add easy handle to multi handle
    otbCurlMultiCall(curl_multi_add_handle(multiHandle.GetCurlMultiResource(), lEasyHandle));

    // Add hanle to vector
    listCurlHandles.push_back(lEasyHandle);
    ++url;
    ++file;
    }

  //fetch tiles
  // Configure multi handle - set the maximum connections
  otbCurlMultiCall(curl_multi_setopt(multiHandle.GetCurlMultiResource(), CURLMOPT_MAXCONNECTS, maxConnect));
  otbCurlMultiCall(curl_multi_setopt(multiHandle.GetCurlMultiResource(), CURLMOPT_PIPELINING, 0));

  // Perform
  int lStillRunning;

  while (CURLM_CALL_MULTI_PERFORM == curl_multi_perform(multiHandle.GetCurlMultiResource(), &lStillRunning));

  // Now get that URL
  while (lStillRunning)
    {
    struct timeval timeout;
    int            rc; // Return code

    fd_set fdread;
    fd_set fdwrite;
    fd_set fdexcep;
    int    maxfd;

    FD_ZERO(&fdread);
    FD_ZERO(&fdwrite);
    FD_ZERO(&fdexcep);

    /* set a suitable timeout to play around with */
    timeout.tv_sec = 0;
    timeout.tv_usec = 1;

    /* get file descriptors from the transfers */
    otbCurlMultiCall(curl_multi_fdset(multiHandle.GetCurlMultiResource(), &fdread, &fdwrite, &fdexcep, &maxfd)); 

    rc = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout);

    switch (rc)
      {
      case -1:
        /* select error */
        break;
      case 0:
        /* timeout */
      default:
        /* timeout or readable/writable sockets */
        while (CURLM_CALL_MULTI_PERFORM == curl_multi_perform(multiHandle.GetCurlMultiResource(), &lStillRunning));
        break;
      }
    }

  int      remaining_msgs = 1;
  int      error = 0;
  CURLMsg *msg;
  while (remaining_msgs)
    {
    msg = curl_multi_info_read(multiHandle.GetCurlMultiResource(), &remaining_msgs);
    if (msg != NULL)
      {
      if (CURLE_OK != msg->data.result) error = 1;
      }
    }

  if (error != 0)
    {
    itkExceptionMacro(<< "otbCurlHelper: Error occurs while perform Multi handle");
    }

  // Cleanup
  // Close files
  for (unsigned int currentFile = 0; currentFile < listFiles.size(); currentFile++)
    {
    fclose(listFiles[currentFile]);
    }
  listFiles.clear();

  // Cleanup easy handles
  for (unsigned int currentHandle = 0; currentHandle < listCurlHandles.size(); currentHandle++)
    {
    curl_easy_cleanup(listCurlHandles[currentHandle]);
    }
  listCurlHandles.clear();
  
#else
  //fallback on non curl multi
  otbMsgDevMacro(<< "Curl multi is not available, fallback on standard");
  
  std::vector<std::string>::const_iterator url;
  std::vector<std::string>::const_iterator file;
  url = listURLs.begin();
  file = listFilename.begin();
  int res = 0;
  int resTmp = -1;
  while ((url != listURLs.end()) && (file != listFilename.end()))
    {
    resTmp = RetrieveFile(*url, *file);
    if (res == 0) res = resTmp;
    ++url;
    ++file;
    }

  return res;
#endif
#else
  otbMsgDevMacro(<< "Curl is not available, compile with OTB_USE_CURL to ON");
  return -1;
#endif
}

size_t CurlHelper::CallbackWriteDataToFile(void* ptr, size_t size, size_t nmemb, void* data)
{
  size_t written;

  FILE * fDescriptor = (FILE *) (data);

  written = fwrite(ptr, size, nmemb, fDescriptor);

  return written;
}

/*
size_t CurlHelper::CallbackWriteDataToCharVector(void *ptr, size_t size, size_t nmemb, void *data)
{
  register int realsize = (int)(size * nmemb);

  std::vector<char> *vec
    = static_cast<std::vector<char>*>(data);
  const char* chPtr = static_cast<char*>(ptr);
  vec->insert(vec->end(), chPtr, chPtr + realsize);

  return realsize;
}
*/

size_t CurlHelper::CallbackWriteDataToStringStream(void *ptr, size_t size, size_t nmemb, void *data)
{
  std::ostringstream& stream = *reinterpret_cast<std::ostringstream*>(data);
  stream << reinterpret_cast<char*>(ptr);
  return size * nmemb;
}


size_t CurlHelper::CallbackWriteDataDummy(void *ptr, size_t size, size_t nmemb, void *data)
{
  return size * nmemb;
}

}
