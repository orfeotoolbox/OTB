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

  /** Get the radiometric bias from the ossim metadata */
  VariableLengthVectorType GetPhysicalBias() const
  {
    itkExceptionMacro("GetPhysicalBias not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the radiometric gain from the ossim metadata */
  VariableLengthVectorType GetPhysicalGain() const
  {
    itkExceptionMacro("GetPhysicalGain not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the solar irradiance from the ossim metadata */
  VariableLengthVectorType GetSolarIrradiance() const
  {
    itkExceptionMacro("GetSolarIrradiance not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition day from the ossim metadata */
  int GetDay() const override
  {
    itkExceptionMacro("GetDay not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition month from the ossim metadata */
  int GetMonth() const override
  {
    itkExceptionMacro("GetMonth not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition year from the ossim metadata */
  int GetYear() const override
  {
    itkExceptionMacro("GetYear not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition hour from the ossim metadata */
  int GetHour() const override
  {
    itkExceptionMacro("GetHour not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition minute from the ossim metadata */
  int GetMinute() const override
  {
    itkExceptionMacro("GetMinute not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging production day from the ossim metadata */
  int GetProductionDay() const override
  {
    itkExceptionMacro("GetProductionDay not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging production month from the ossim metadata */
  int GetProductionMonth() const override
  {
    itkExceptionMacro("GetProductionMonth not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging production year from the ossim metadata */
  int GetProductionYear() const override
  {
    itkExceptionMacro("GetProductionYear not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the sat elevation from the ossim metadata */
  double GetSatElevation() const
  {
    itkExceptionMacro("GetSatElevation not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the sat azimuth from the ossim metadata */
  double GetSatAzimuth() const
  {
    itkExceptionMacro("GetSatElevation not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the first wavelength for the spectral band definition */
  VariableLengthVectorType GetFirstWavelengths() const
  {
    itkExceptionMacro("GetFirstWavelengths not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the last wavelength for the spectral band definition */
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

  /** Get the 3 spectral band numbers corresponding to the default display for visualization,
   *  in the order R, G, B.
   *
   *  Nota : When two spectral bands are available: first band is given to the R and B channel
   *         When one spectral band is available : the only band is given to the R, G and B channel.
   *
   */
  std::vector<unsigned int> GetDefaultDisplay() const override
  {
    unsigned int              i = 0;
    std::vector<unsigned int> rgb(3);

    if (this->GetNumberOfBands() == 0)
    {
      rgb[0] = 0;
      rgb[1] = 1;
      rgb[2] = 2;
    }
    else
    {
      unsigned int min;
      min = std::min(this->GetNumberOfBands(), static_cast<unsigned int>(3));
      while (i < min)
      {
        rgb[i] = i;
        ++i;
      }
    }
    return rgb;
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
