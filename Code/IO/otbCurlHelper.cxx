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


#ifdef OTB_USE_CURL
#include <curl/curl.h>
#include <cstring>
#endif

namespace otb
{

int CurlHelper::TestUrlAvailability(const std::string& url) const
{
#ifdef OTB_USE_CURL
  // Set up a curl request
  CURL *   curl;
  CURLcode res = CURL_LAST;
  curl = curl_easy_init();

  // Set up the browser
  std::string browser =
    "Mozilla/5.0 (Windows; U; Windows NT 6.0; en-GB; rv:1.8.1.11) Gecko/20071127 Firefox/2.0.0.11";

  if (curl)
    {
    curl_easy_setopt(curl, CURLOPT_USERAGENT, browser.data());
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
#ifdef OTB_USE_CURL
  CURL *   curl;
  CURLcode res = CURL_LAST;

  FILE* output_file = fopen(filename.c_str(), "w");
  curl = curl_easy_init();

  char url[256];
  strcpy(url, urlStream.str().data());

//   std::cout << url << std::endl;
  if (curl)
    {
    std::vector<char> chunk;
    curl_easy_setopt(curl, CURLOPT_URL, url);

    // Set 5s timeout
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, output_file);
    res = curl_easy_perform(curl);

    fclose(output_file);
    /* always cleanup */
    curl_easy_cleanup(curl);
    }
  return res;
#else
  otbMsgDevMacro(<< "Curl is not available, compile with OTB_USE_CURL to ON");
  return -1;
#endif
}

}

