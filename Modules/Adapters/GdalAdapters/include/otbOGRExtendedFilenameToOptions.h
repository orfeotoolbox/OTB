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

#ifndef otbOGRExtendedFilenameToOptions_h
#define otbOGRExtendedFilenameToOptions_h

#include <set>
#include "otbExtendedFilenameHelper.h"

namespace otb
{

/** \class OGRExtendedFilenameToOptions
 */

class ITK_EXPORT OGRExtendedFilenameToOptions : public ExtendedFilenameHelper
{
public:
  /** Standard class typedefs. */
  typedef OGRExtendedFilenameToOptions Self;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef ExtendedFilenameHelper Superclass;

  typedef Superclass::OptionMapType OptionMapType;
  typedef OptionMapType::iterator MapIteratorType;

  itkTypeMacro(OGRExtendedFilenameToOptions, otb::ExtendedFilenameHelper);
  itkNewMacro(Self);

  typedef std::vector<std::string> GDALOptionType;

  /** The creation option structure. */
  struct OpenOptionType
  {
    std::pair< bool, std::string  > simpleFileName;
    std::pair< bool, GDALOptionType > gdalOptions;
    std::map< std::string , bool > availableOptions;
  };

  struct CreationOptionType
  {
    std::pair< bool, std::string  > simpleFileName;
    std::pair< bool, GDALOptionType > gdalOptions;
    std::map< std::string , bool > availableOptions;
  };

  struct LayerOptionType
  {
    std::pair< bool, GDALOptionType > gdalOptions;
    std::map< std::string , bool > availableOptions;
  };

  /* Set Methods */
  void SetExtendedFileName(const char * extFname) override;

  GDALOptionType GetGDALOptions( const std::string & type ) const ;

  GDALOptionType GetGDALOpenOptions() const ;
  GDALOptionType GetGDALCreationOptions() const ;
  GDALOptionType GetGDALLayerOptions() const ;



protected:
  OGRExtendedFilenameToOptions();
  ~OGRExtendedFilenameToOptions() override {};

private:
  OGRExtendedFilenameToOptions(const Self &) = delete ;
  void operator =(const Self&) = delete ;

  OpenOptionType m_OpenOptions;
  CreationOptionType m_CreationOptions;
  LayerOptionType m_LayerOptions;
};

} //end namespace otb

#endif // otbOGRExtendedFilenameToOptions_h
