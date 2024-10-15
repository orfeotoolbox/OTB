/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbCurlHelperInterface_h
#define otbCurlHelperInterface_h

#include <iosfwd>
#include "itkObject.h"
#include "itkObjectFactory.h"
#include "OTBCurlAdaptersExport.h"

namespace otb
{
/**
 * \class CurlHelperInterface
 * \brief Class to use the curl capabilities from OTB
 *
 * This class implement the interface of the curl access allowing for
 * different implementation to be used (include stubs for test).
 *
 *
 * \ingroup OTBCurlAdapters
 */
class OTBCurlAdapters_EXPORT CurlHelperInterface : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef CurlHelperInterface           Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef itk::Object                   Superclass;

  itkTypeMacro(CurlHelperInterface, itk::Object);

  virtual bool TestUrlAvailability(const std::string& url) const = 0;

  virtual int RetrieveFile(const std::ostringstream& urlStream, std::string filename) const = 0;

  virtual int RetrieveFile(const std::string& urlString, std::string filename) const = 0;

  virtual int RetrieveUrlInMemory(const std::string& urlString, std::string& output) const = 0;

  virtual int RetrieveFileMulti(const std::vector<std::string>& listURLs, const std::vector<std::string>& listFiles, int maxConnect) const = 0;

  static bool IsCurlAvailable();
  static bool IsCurlMultiAvailable();

protected:
  CurlHelperInterface()
  {
  }
  ~CurlHelperInterface() override
  {
  }

private:
  CurlHelperInterface(const Self&) = delete;
  void operator=(const Self&) = delete;
};
}
#endif
