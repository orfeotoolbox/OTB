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
#include "otbMacro.h"

#ifdef OTB_USE_CURL
#include <curl/curl.h>
#include <cstring>
#endif

#include <cstdio>

namespace otb
{

int CurlHelper::TestUrlAvailability(const std::string& url) const
{
#ifdef OTB_USE_CURL
  // Set up a curl request
  CURL *   curl;
  CURLcode res = CURL_LAST;
  curl = curl_easy_init();

  if (curl)
    {
    curl_easy_setopt(curl, CURLOPT_USERAGENT, m_Browser.data());
    curl_easy_setopt(curl, CURLOPT_URL, url.data());
    // Set the dummy write function
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &Self::curlDummyWriteFunction);
    curl_easy_setopt(curl, CURLOPT_MAXFILESIZE, 1);

    // Perform requet
    res = curl_easy_perform(curl);
    }
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
  CURL *   curl;
  CURLcode res = CURL_LAST;

  FILE* output_file = fopen(filename.c_str(), "wb");

  curl = curl_easy_init();

  char url[256];
  strcpy(url, urlString.c_str());

  if (curl)
    {
    curl_easy_setopt(curl, CURLOPT_URL, url);

    // Set 5s timeout
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);

    // Use our writing static function to avoid file descriptor
    // pointer crash on windows
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &Self::write_data);

    // Say the file where to write the received data
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) output_file);

    res = curl_easy_perform(curl);

    /* always cleanup */
    curl_easy_cleanup(curl);

    fclose(output_file);
    }
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

  CURLM *             multiHandle;
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

  // Initialize curl multi handle
  multiHandle = curl_multi_init();

  if (!multiHandle)
    {
    itkExceptionMacro(<< "otbCurlHelper: Curl multi handle init error.");
    }

  listCurlHandles.clear();

  // Initialize curl handle

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
    curl_easy_setopt(lEasyHandle, CURLOPT_USERAGENT, m_Browser.data());
    curl_easy_setopt(lEasyHandle, CURLOPT_URL, (*url).data());
    curl_easy_setopt(lEasyHandle, CURLOPT_WRITEFUNCTION, &Self::write_data);
    curl_easy_setopt(lEasyHandle, CURLOPT_WRITEDATA, (void*) (*file));

    // Add easy handle to multi handle
    curl_multi_add_handle(multiHandle, lEasyHandle);

    // Add hanle to vector
    listCurlHandles.push_back(lEasyHandle);
    ++url;
    ++file;
    }

  //fetch tiles
  // Configure multi handle - set the maximum connections
  curl_multi_setopt(multiHandle, CURLMOPT_MAXCONNECTS, maxConnect);
  curl_multi_setopt(multiHandle, CURLMOPT_PIPELINING, 0);

  // Perform
  int lStillRunning;

  while (CURLM_CALL_MULTI_PERFORM == curl_multi_perform(multiHandle, &lStillRunning))
;

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
    curl_multi_fdset(multiHandle, &fdread, &fdwrite, &fdexcep, &maxfd);

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
        while (
          CURLM_CALL_MULTI_PERFORM == curl_multi_perform(multiHandle, &lStillRunning)
          )
      ;
        break;
      }
    }

  int      remaining_msgs = 1;
  int      error = 0;
  CURLMsg *msg;
  while (remaining_msgs)
    {
    msg = curl_multi_info_read(multiHandle, &remaining_msgs);
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

  // Cleanup multi handle
  return curl_multi_cleanup(multiHandle);
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

size_t CurlHelper::write_data(void* ptr, size_t size, size_t nmemb, void* data)
{
  size_t written;

  FILE * fDescriptor = (FILE *) (data);

  written = fwrite(ptr, size, nmemb, fDescriptor);

  return written;
}

}
