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

#ifndef otbQuickBirdImageMetadataInterface_h
#define otbQuickBirdImageMetadataInterface_h

#include "otbOpticalImageMetadataInterface.h"

namespace otb
{
/** \class QuickBirdImageMetadataInterface
 *
 * \brief Creation of an "otb" QuickBirdImageMetadataInterface that gets metadata.
 *
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT QuickBirdImageMetadataInterface : public OpticalImageMetadataInterface
{
public:
  typedef QuickBirdImageMetadataInterface Self;
  typedef ImageMetadataInterfaceBase      Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(QuickBirdImageMetadataInterface, ImageMetadataInterfaceBase);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType     ImageKeywordlistType;

  /** Get the radiometric bias from the ossim metadata */
  VariableLengthVectorType GetPhysicalBias() const override;

  /** Get the radiometric gain from the ossim metadata */
  VariableLengthVectorType GetPhysicalGain() const override;

  /** Get the solar irradiance from the ossim metadata */
  VariableLengthVectorType GetSolarIrradiance() const override;

  /** Get the imaging acquisition day from the ossim metadata : TLCTime metadata value */
  int GetDay() const override;

  /** Get the imaging acquisition month from the ossim metadata : TLCTime metadata value */
  int GetMonth() const override;

  /** Get the imaging acquisition year from the ossim metadata : TLCTime metadata value */
  int GetYear() const override;

  /** Get the imaging acquisition hour from the ossim metadata : TLCTime metadata value */
  int GetHour() const override;

  /** Get the imaging acquisition year from the ossim metadata : TLCTime metadata value */
  int GetMinute() const override;

  /** Get the imaging production day from the ossim metadata : generationTime metadata value */
  int GetProductionDay() const override;

  /** Get the imaging production month from the ossim metadata : generationTime metadata value */
  int GetProductionMonth() const override;

  /** Get the imaging production year from the ossim metadata : generationTime metadata value */
  int GetProductionYear() const override;

  /** Get the sat elevation from the ossim metadata */
  double GetSatElevation() const override;

  /** Get the sat azimuth from the ossim metadata */
  double GetSatAzimuth() const override;

  /** Get the first wavelength for the spectral band definition */
  VariableLengthVectorType GetFirstWavelengths() const override;

  /** Get the last wavelength for the spectral band definition */
  VariableLengthVectorType GetLastWavelengths() const override;

  /** Get the enhanced band names of QuickBird data */
  std::vector<std::string> GetEnhancedBandNames() const override;

  bool CanRead() const override;

  /** Get the 3 spectral band numbers corresponding to the default display for visualization,
   *  in the order R, G, B */
  std::vector<unsigned int> GetDefaultDisplay() const override;

  /** Vector that contains the filter function value in 6S format (step of 0.0025 micro m).
     * There values a computed by 6S. */
  WavelengthSpectralBandVectorType GetSpectralSensitivity() const override;

protected:
  QuickBirdImageMetadataInterface();
  ~QuickBirdImageMetadataInterface() override
  {
  }

private:
  QuickBirdImageMetadataInterface(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif
