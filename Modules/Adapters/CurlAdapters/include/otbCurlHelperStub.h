/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbCurlHelperStub_h
#define otbCurlHelperStub_h

#include "otbCurlHelperInterface.h"

namespace otb
{
/**
 * \class CurlHelperStub
 * \brief Class to test classes using the curl capabilities from OTB
 *
 * This class is available only for testing purposes
 *
 *
 * \ingroup OTBCurlAdapters
 */
class ITK_ABI_EXPORT CurlHelperStub : public CurlHelperInterface
{
public:
  /** Standard class typedefs. */
  typedef CurlHelperStub                Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef CurlHelperInterface           Superclass;

  itkTypeMacro(CurlHelperStub, CurlHelperInterface);

  itkNewMacro(Self);

  bool TestUrlAvailability(const std::string& url) const override;

  int RetrieveUrlInMemory(const std::string& urlString, std::string& output) const override;

  int RetrieveFile(const std::ostringstream& urlStream, std::string filename) const override;

  int RetrieveFile(const std::string& urlString, std::string filename) const override;

  int RetrieveFileMulti(const std::vector<std::string>& listURLs, const std::vector<std::string>& listFiles, int maxConnect) const override;

protected:
  CurlHelperStub()
  {
  }
  ~CurlHelperStub() override
  {
  }

private:
  CurlHelperStub(const Self&) = delete;
  void operator=(const Self&) = delete;
};
}
#endif
