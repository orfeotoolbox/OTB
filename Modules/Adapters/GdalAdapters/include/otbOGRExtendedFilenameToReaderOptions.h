/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbOGRExtendedFilenameToReaderOptions_h
#define otbOGRExtendedFilenameToReaderOptions_h

#include <set>
#include "otbExtendedFilenameHelper.h"

namespace otb
{

/** \class OGRExtendedFilenameToReaderOptions
 */

class ITK_EXPORT OGRExtendedFilenameToReaderOptions : public ExtendedFilenameHelper
{
public:
  /** Standard class typedefs. */
  typedef OGRExtendedFilenameToReaderOptions Self;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef ExtendedFilenameHelper Superclass;

  typedef Superclass::OptionMapType OptionMapType;
  typedef OptionMapType::iterator MapIteratorType;

  itkTypeMacro(OGRExtendedFilenameToReaderOptions, otb::ExtendedFilenameHelper);
  itkNewMacro(Self);

  /** The creation option structure. */
  struct OptionType
  {
    std::pair< bool , std::string > simpleFileName;
    std::pair< bool , std::string > shapeFileType;
    std::set< std::string > availableOptions;
  };

  /* Set Methods */
  void SetExtendedFileName(const char * extFname) override;

protected:
  OGRExtendedFilenameToReaderOptions();
  ~OGRExtendedFilenameToReaderOptions() override {};

private:
  OGRExtendedFilenameToReaderOptions(const Self &) = delete ;
  void operator =(const Self&) = delete ;

  OptionType m_Options;
};

} //end namespace otb

#endif // otbOGRExtendedFilenameToReaderOptions_h
