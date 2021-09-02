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

#ifndef otbOpticalDefaultImageMetadataInterface_h
#define otbOpticalDefaultImageMetadataInterface_h


#include "otbMacro.h"

#include "otbOpticalImageMetadataInterface.h"

namespace otb
{
/** \class OpticalDefaultImageMetadataInterface
 *
 * \brief Creation of an "otb" DefaultImageMetadataInterface that gets metadata.
 *
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT OpticalDefaultImageMetadataInterface : public OpticalImageMetadataInterface
{
public:
  typedef OpticalDefaultImageMetadataInterface Self;
  typedef OpticalImageMetadataInterface        Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OpticalDefaultImageMetadataInterface, OpticalImageMetadataInterface);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType     ImageKeywordlistType;

  /** Get the radiometric bias from the ossim metadata
   * \deprecated
   */
  VariableLengthVectorType GetPhysicalBias() const override
  {
    itkExceptionMacro("GetPhysicalBias not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the radiometric gain from the ossim metadata
   * \deprecated
   */
  VariableLengthVectorType GetPhysicalGain() const override
  {
    itkExceptionMacro("GetPhysicalGain not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the solar irradiance from the ossim metadata
   * \deprecated
   */
  VariableLengthVectorType GetSolarIrradiance() const override
  {
    itkExceptionMacro("GetSolarIrradiance not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the sat elevation from the ossim metadata
   * \deprecated
   */
  double GetSatElevation() const override
  {
    itkExceptionMacro("GetSatElevation not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the sat azimuth from the ossim metadata
   * \deprecated
   */
  double GetSatAzimuth() const override
  {
    itkExceptionMacro("GetSatElevation not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the first wavelength for the spectral band definition */
  VariableLengthVectorType GetFirstWavelengths() const override
  {
    itkExceptionMacro("GetFirstWavelengths not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the last wavelength for the spectral band definition */
  VariableLengthVectorType GetLastWavelengths() const override
  {
    itkExceptionMacro("GetLastWavelengths not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the enhanced band names (here nothing because the sensor is not identify) */
  std::vector<std::string> GetEnhancedBandNames() const override
  {
    std::vector<std::string> nothing;
    return nothing;
  }

  bool CanRead() const override
  {
    // This class is the default one, it has to be able to call every metadata
    return false;
  }

  /** Get the 3 spectral band numbers corresponding to the default display for visualization,
   *  in the order R, G, B */
  std::vector<unsigned int> GetDefaultDisplay() const override
  {
    std::vector<unsigned int> rgb(3);
    rgb[0] = 0;
    rgb[1] = 1;
    rgb[2] = 2;
    return rgb;
  }

  /** Vector that contains the filter function value in 6S format (step of 0.0025 micro m).
   * There values a computed by 6S. */
  WavelengthSpectralBandVectorType GetSpectralSensitivity() const override
  {
    itkExceptionMacro("GetSpectralSensitivity not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

protected:
  OpticalDefaultImageMetadataInterface(){};
  ~OpticalDefaultImageMetadataInterface() override
  {
  }

private:
  OpticalDefaultImageMetadataInterface(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif
