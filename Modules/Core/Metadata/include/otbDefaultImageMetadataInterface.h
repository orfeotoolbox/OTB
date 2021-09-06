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

#ifndef otbDefaultImageMetadataInterface_h
#define otbDefaultImageMetadataInterface_h

#include <algorithm>

#include "OTBMetadataExport.h"
#include "otbImageMetadataInterfaceBase.h"

namespace otb
{
/** \class DefaultImageMetadataInterface
 *
 * \brief Creation of an "otb" DefaultImageMetadataInterface that gets metadata.
 *
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT DefaultImageMetadataInterface : public ImageMetadataInterfaceBase
{
public:
  typedef DefaultImageMetadataInterface Self;
  typedef ImageMetadataInterfaceBase    Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DefaultImageMetadataInterface, ImageMetadataInterfaceBase);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType     ImageKeywordlistType;

  /** Get the solar irradiance from the ossim metadata
   * \deprecated
   */
  VariableLengthVectorType GetSolarIrradiance() const
  {
    itkExceptionMacro("GetSolarIrradiance not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the sat elevation from the ossim metadata
   * \deprecated
   */
  double GetSatElevation() const
  {
    itkExceptionMacro("GetSatElevation not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the sat azimuth from the ossim metadata
   * \deprecated
   */
  double GetSatAzimuth() const
  {
    itkExceptionMacro("GetSatElevation not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the first wavelength for the spectral band definition
   * \deprecated
   */
  VariableLengthVectorType GetFirstWavelengths() const
  {
    itkExceptionMacro("GetFirstWavelengths not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the last wavelength for the spectral band definition
   * \deprecated
   */
  VariableLengthVectorType GetLastWavelengths() const
  {
    itkExceptionMacro("GetLastWavelengths not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the enhanced band names */
  std::vector<std::string> GetEnhancedBandNames() const override
  {
    itkExceptionMacro("GetEnhancedBandNames not implemented in DefaultImageMetadataInterface, no captor type found");
  }


  bool CanRead() const override
  {
    // This class is the default one, it has to be able to call every metadata
    return true;
  }


protected:
  DefaultImageMetadataInterface(){};
  ~DefaultImageMetadataInterface() override
  {
  }

private:
  DefaultImageMetadataInterface(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif
