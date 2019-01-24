/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbCurlHelper_h
#define otbCurlHelper_h

#include "otbCurlHelperInterface.h"
#include <string>

namespace otb
{
/**
 * \class CurlHelper
 * \brief Class to use the curl capabilities from OTB
 *
 * This class is responsible for behaving properly when curl is
 * not available, i.e. the compilation should pass, the runtime should
 * not segfault but of course, the behavior will be different.
 *
 *
 * \ingroup OTBCurlAdapters
 */
class OTBCurlAdapters_EXPORT CurlHelper : public CurlHelperInterface
{
public:
  /** Standard class typedefs. */
  typedef CurlHelper                    Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef CurlHelperInterface           Superclass;

  itkTypeMacro(CurlHelper, CurlHelperInterface);
  itkNewMacro(Self);

  bool TestUrlAvailability(const std::string& url) const override;

  bool IsCurlReturnHttpError(const std::string& url) const;

  int RetrieveFile(const std::ostringstream& urlStream, std::string filename) const override;

  int RetrieveFile(const std::string& urlString, std::string filename) const override;

  int RetrieveUrlInMemory(const std::string& urlString, std::string& output) const override;

  int RetrieveFileMulti(const std::vector<std::string>& listURLs,
                        const std::vector<std::string>& listFiles,
                        int maxConnect) const override;

  itkGetMacro(Timeout,long int);

  itkSetMacro(Timeout,long int);

protected:
  CurlHelper() :
    m_Browser("Mozilla/5.0 (Windows; U; Windows NT 6.0; en-GB; rv:1.8.1.11) "
              "Gecko/20071127 Firefox/2.0.0.11"),
    m_Timeout(10) {}
  ~CurlHelper() override {}

private:
  CurlHelper(const Self &) = delete;
  void operator =(const Self&) = delete;

  // Need to use our writing function to handle windows segfaults
  // Need to be static cause the CURL_OPT is expecting a pure C
  // function or a static c++ method.
  static size_t CallbackWriteDataToFile(void* ptr, size_t size, size_t nmemb, void* data);

  static size_t CallbackWriteDataToStringStream(void* ptr, size_t size, size_t nmemb, void* data);

  static size_t CallbackWriteDataDummy(void* ptr, size_t size, size_t nmemb, void* data);

  // Browser Agent used
  std::string m_Browser;
  long int    m_Timeout;
};
}
#endif
