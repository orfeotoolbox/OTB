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

#ifndef otbExtendedFilenameHelper_h
#define otbExtendedFilenameHelper_h

#include <map>
#include <vector>

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "OTBCommonExport.h"
#include <string>

namespace otb
{

/** \class ExtendedFilenameHelper
 *  \brief Helper to handle extended filenames.
 *
 *  \sa ImageFileReader
 *
 * \ingroup OTBExtendedFilename
 * \ingroup OTBCommon
 */

class OTBCommon_EXPORT ExtendedFilenameHelper : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef ExtendedFilenameHelper        Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef itk::Object                   Superclass;

  itkTypeMacro(ExtendedFilenameHelper, itk::Object);
  itkNewMacro(Self);

  typedef std::map<std::string, std::string> OptionMapType;

  /** \deprecated const char* overload of SetExtendedFileName is deprecated, use std::string instead */
  virtual void SetExtendedFileName(const char* extFname);

  virtual void SetExtendedFileName(const std::string& extFname);

  const OptionMapType& GetOptionMap(void) const;

  itkGetStringMacro(ExtendedFileName);
  itkGetStringMacro(SimpleFileName);

  struct OTBCommon_EXPORT GenericBandRange : std::pair<int, int>
  {
    GenericBandRange()
    {
    }

    GenericBandRange(int a);

    GenericBandRange(const std::pair<int, int>& a);

    GenericBandRange(int a, int b);

    bool SetString(const std::string& str, size_t start = 0, size_t size = std::string::npos);

    void Print(std::ostream& os);
  };

  /**  Decode the string into a list of GenericBandRange, band indexes are
   *  1-based. */
  std::vector<ExtendedFilenameHelper::GenericBandRange> GetGenericBandRange(const std::string& bandRange) const;

  /** Resolve the list of band ranges into real band indexes, according to
   *  a total number of bands in the image. Note that the output indexes are
   *  zero-based (0 is the first component) */
  bool ResolveBandRange(const std::string& bandRange, const unsigned int& nbBands, std::vector<unsigned int>& output) const;

protected:
  ExtendedFilenameHelper()
  {
  }
  ~ExtendedFilenameHelper() override
  {
  }

private:
  ExtendedFilenameHelper(const Self&) = delete;
  void operator=(const Self&) = delete;
  itkSetStringMacro(SimpleFileName);

  std::string   m_ExtendedFileName;
  std::string   m_SimpleFileName;
  OptionMapType m_OptionMap;
};
} // end namespace otb

#endif // otbExtendedFilenameHelper_h
