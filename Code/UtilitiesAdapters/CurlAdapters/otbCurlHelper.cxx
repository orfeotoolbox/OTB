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
#include <itkLightObject.h>

namespace otb
{

#ifdef OTB_USE_CURL

/**
 * Class to handle the CURLcode and CURLMCode and throw exceptions when needed
 */
class CurlHandleError {

public:
  /** Processing CURLcode */
  static void ProcessCURLcode(CURLcode curlCode)
  {
    if(curlCode != CURLE_OK)
       {
       itkGenericExceptionMacro(<<" Curl Error : "<< curl_easy_strerror(curlCode));
       }
  }

  /** Processing CURLMcode */
  static void ProcessCURLcode(CURLMcode curlMCode)
  {
    if(curlMCode != CURLM_OK)
      {
      itkGenericExceptionMacro(<<" CurlM Error : "<< curl_multi_strerror(curlMCode));
      }
  }
};

/**
 * Resource class that create and clean the curl environment proprely
 * in case of a thrown exception
 */
class CurlResource : public itk::LightObject {

public:
  /** Standard class typedefs. */
  typedef CurlResource                  Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef itk::LightObject              Superclass;

  itkTypeMacro(CurlResource, itk::LightObject);
  itkNewMacro(Self);

  /** Get the curl object */
  CURL * GetCurlResource()
  {
    return m_Curl;
  }

protected:
  CurlResource()
  {
    m_Curl = curl_easy_init();

    if (!m_Curl)
      {
      itkExceptionMacro(<<" otbCurlHelper::CurlResource Curl handle init error.");
      }
  }

  virtual ~CurlResource()
  {
    curl_easy_cleanup(m_Curl);
  }

private:
  CURL * m_Curl;
  // prevent copying and assignment; not implemented
  CurlResource (const CurlResource &);
  CurlResource & operator= (const CurlResource &);
};  //end of class CurlResource


#ifdef OTB_CURL_MULTI_AVAILABLE
/**
 * Resource class that create and clean the curl multi environment
 * proprely in case of a thrown exception
 */
class CurlMultiResource : public itk::LightObject {

public:
  /** Standard class typedefs. */
  typedef CurlMultiResource             Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef itk::LightObject              Superclass;

  itkTypeMacro(CurlMultiResource, itk::LightObject);
  itkNewMacro(Self);

  CURLM * GetCurlMultiResource()
  {
    return m_Curl;
  }

protected:
  CurlMultiResource()
  {
    m_Curl = curl_multi_init();

    if (!m_Curl)
      {
      itkExceptionMacro(<<" otbCurlHelper::CurlMultiResource Curl multi handle init error.");
      }
  }

  virtual ~CurlMultiResource()
  {
    curl_multi_cleanup(m_Curl);
  }

private:
  CURLM * m_Curl;
  // prevent copying and assignment; not implemented
  CurlMultiResource (const CurlMultiResource &);
  CurlMultiResource & operator= (const CurlMultiResource &);
}; //end of class CurlMultiResource
#endif

/**
 * Resource class that create FILE * and  close the FILE *  descriptor
 * proprely in case of a thrown exception
 */
class CurlFileDescriptorResource : public itk::LightObject {

public:
  /** Standard class typedefs. */
  typedef CurlFileDescriptorResource    Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef itk::LightObject              Superclass;

  itkTypeMacro(CurlFileDescriptorResource, itk::LightObject);
  itkNewMacro(Self);

  FILE * GetFileResource()
  {
    return m_File;
  }

  void OpenFile(const char* infname)
  {
    m_File = fopen(infname, "wb");

    if (m_File == NULL)
      {
      itkExceptionMacro(<<" otbCurlHelper::FileResource : failed to open the file ."<< infname);
      }
  }

protected:
  CurlFileDescriptorResource(){}

  virtual ~CurlFileDescriptorResource()
  {
    fclose(m_File);
  }

private:
  FILE *      m_File;

  // prevent copying and assignment; not implemented
  CurlFileDescriptorResource (const CurlFileDescriptorResource &);
  CurlFileDescriptorResource & operator= (const CurlFileDescriptorResource &);
}; //end of class FileResource

#endif  // Necessary only if Curl Availabale

bool CurlHelper::TestUrlAvailability(const std::string& url) const
{
#ifdef OTB_USE_CURL
  // Set up a curl resource
  CurlResource::Pointer curlResource = CurlResource::New();

  CurlHandleError::ProcessCURLcode(curl_easy_setopt(curlResource->GetCurlResource(), CURLOPT_USERAGENT, m_Browser.data()));
  CurlHandleError::ProcessCURLcode(curl_easy_setopt(curlResource->GetCurlResource(), CURLOPT_URL, url.data()));
  // Set the dummy write function
  CurlHandleError::ProcessCURLcode(curl_easy_setopt(curlResource->GetCurlResource(), CURLOPT_WRITEFUNCTION,
                               &Self::CallbackWriteDataDummy));
  CurlHandleError::ProcessCURLcode(curl_easy_setopt(curlResource->GetCurlResource(), CURLOPT_MAXFILESIZE, 1));

  // Perform requet
  CURLcode easyPerformResult = curl_easy_perform(curlResource->GetCurlResource());

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

  return false;
#else
  otbMsgDevMacro(<< "Curl is not available, compile with OTB_USE_CURL to ON");
  return false;
#endif
}

bool CurlHelper::IsCurlReturnHttpError(const std::string& url) const
{
#ifdef OTB_USE_CURL
  // Set up a curl resource
  CurlResource::Pointer curlResource = CurlResource::New();

  CurlHandleError::ProcessCURLcode(curl_easy_setopt(curlResource->GetCurlResource(), CURLOPT_USERAGENT, m_Browser.data()));
  CurlHandleError::ProcessCURLcode(curl_easy_setopt(curlResource->GetCurlResource(), CURLOPT_URL, url.data()));
  // Set the dummy write function
  CurlHandleError::ProcessCURLcode(curl_easy_setopt(curlResource->GetCurlResource(), CURLOPT_WRITEFUNCTION,
                               &Self::CallbackWriteDataDummy));
  CurlHandleError::ProcessCURLcode(curl_easy_setopt(curlResource->GetCurlResource(), CURLOPT_MAXFILESIZE, 1));
  CurlHandleError::ProcessCURLcode(curl_easy_setopt(curlResource->GetCurlResource(), CURLOPT_FAILONERROR, 1));

  // Perform requet
  CURLcode easyPerformResult = curl_easy_perform(curlResource->GetCurlResource());
  if ( easyPerformResult == CURLE_HTTP_RETURNED_ERROR )
    {
    return true;
    }

  return false;
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
  CurlResource::Pointer curlResource = CurlResource::New();

  CurlHandleError::ProcessCURLcode(curl_easy_setopt(curlResource->GetCurlResource(), CURLOPT_URL, url.c_str()));

  // Settimeout
  CurlHandleError::ProcessCURLcode(curl_easy_setopt(curlResource->GetCurlResource(), CURLOPT_TIMEOUT, m_Timeout));

  // Use our writing static function to avoid file descriptor
  // pointer crash on windows
  CurlHandleError::ProcessCURLcode(curl_easy_setopt(curlResource->GetCurlResource(), CURLOPT_WRITEFUNCTION,
                               &Self::CallbackWriteDataToStringStream));

  // Say the file where to write the received data
  std::ostringstream* outputStream = new std::ostringstream;
  CurlHandleError::ProcessCURLcode(curl_easy_setopt(curlResource->GetCurlResource(), CURLOPT_WRITEDATA, (void*) outputStream));

  // Perform request
  CurlHandleError::ProcessCURLcode(curl_easy_perform(curlResource->GetCurlResource()));

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
  otbMsgDevMacro(<< "Retrieving: " << urlString);

  CURLcode res = CURLE_OK;

  CurlResource::Pointer curlResource = CurlResource::New();

  CurlFileDescriptorResource::Pointer output_file = CurlFileDescriptorResource::New();
  output_file->OpenFile(filename.c_str());

  char url[256];
  strcpy(url, urlString.c_str());

  CurlHandleError::ProcessCURLcode(curl_easy_setopt(curlResource->GetCurlResource(), CURLOPT_URL, url));

  // Set timeout
  CurlHandleError::ProcessCURLcode(curl_easy_setopt(curlResource->GetCurlResource(), CURLOPT_TIMEOUT, m_Timeout));

  // Use our writing static function to avoid file descriptor
  // pointer crash on windows
  CurlHandleError::ProcessCURLcode(curl_easy_setopt(curlResource->GetCurlResource(), CURLOPT_WRITEFUNCTION,
                               &Self::CallbackWriteDataToFile));
  // Say the file where to write the received data
  CurlHandleError::ProcessCURLcode(curl_easy_setopt(curlResource->GetCurlResource(), CURLOPT_WRITEDATA,
                               (void*) output_file->GetFileResource()));

  CurlHandleError::ProcessCURLcode(curl_easy_perform(curlResource->GetCurlResource()));

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
#if 0
//#ifdef OTB_CURL_MULTI_AVAILABLE
  otbMsgDevMacro(<< "Using curl multi");

  // Initialize curl handle resource
  CurlMultiResource::Pointer  multiHandle = CurlMultiResource::New();

  std::vector<CurlResource::Pointer> listCurlHandles;
  std::vector<CurlFileDescriptorResource::Pointer> listFiles;

  std::vector<std::string>::const_iterator filename;
  filename = listFilename.begin();
  while (filename != listFilename.end())
    {
    CurlFileDescriptorResource::Pointer lOutputFile = CurlFileDescriptorResource::New();
    lOutputFile->OpenFile((*filename).c_str());

    // Add file to vector
    listFiles.push_back(lOutputFile);
    ++filename;
    }

  listCurlHandles.clear();

  std::vector<std::string>::const_iterator url;
  std::vector<CurlFileDescriptorResource::Pointer>::const_iterator      file;
  url = listURLs.begin();
  file = listFiles.begin();
  while ((url != listURLs.end()) && (file != listFiles.end()))
    {
    otbMsgDevMacro(<< "Retrieving: " << (*url).data());
    CurlResource::Pointer lEasyHandle = CurlResource::New();

    // Param easy handle
    CurlHandleError::ProcessCURLcode(curl_easy_setopt(lEasyHandle->GetCurlResource(), CURLOPT_USERAGENT, m_Browser.data()));
    CurlHandleError::ProcessCURLcode(curl_easy_setopt(lEasyHandle->GetCurlResource(), CURLOPT_URL, (*url).data()));
    CurlHandleError::ProcessCURLcode(curl_easy_setopt(lEasyHandle->GetCurlResource(), CURLOPT_WRITEFUNCTION,
                                 &Self::CallbackWriteDataToFile));
    CurlHandleError::ProcessCURLcode(curl_easy_setopt(lEasyHandle->GetCurlResource(), CURLOPT_WRITEDATA,
                                 (void*) (*file)->GetFileResource()));

    // Add easy handle to multi handle
    CurlHandleError::ProcessCURLcode(curl_multi_add_handle(multiHandle->GetCurlMultiResource(), lEasyHandle->GetCurlResource()));

    // Add hanle to vector
    listCurlHandles.push_back(lEasyHandle);
    ++url;
    ++file;
    }

  //fetch tiles
  // Configure multi handle - set the maximum connections
  CurlHandleError::ProcessCURLcode(curl_multi_setopt(multiHandle->GetCurlMultiResource(), CURLMOPT_MAXCONNECTS, maxConnect));
  CurlHandleError::ProcessCURLcode(curl_multi_setopt(multiHandle->GetCurlMultiResource(), CURLMOPT_PIPELINING, 0));

  // Perform
  int lStillRunning;

  while (CURLM_CALL_MULTI_PERFORM == curl_multi_perform(multiHandle->GetCurlMultiResource(), &lStillRunning));

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
    CurlHandleError::ProcessCURLcode(curl_multi_fdset(multiHandle->GetCurlMultiResource(), &fdread, &fdwrite, &fdexcep, &maxfd));

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
        while (CURLM_CALL_MULTI_PERFORM == curl_multi_perform(multiHandle->GetCurlMultiResource(), &lStillRunning));
        break;
      }
    }

  int      remaining_msgs = 1;
  int      error = 0;
  CURLMsg *msg;
  while (remaining_msgs)
    {
    msg = curl_multi_info_read(multiHandle->GetCurlMultiResource(), &remaining_msgs);
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
  listFiles.clear();
  listCurlHandles.clear();

  return 0;
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
