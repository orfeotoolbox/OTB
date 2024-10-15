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

#ifndef otbFormosatImageMetadataInterface_h
#define otbFormosatImageMetadataInterface_h

#include "otbOpticalImageMetadataInterface.h"
#include <string>

namespace otb
{
/** \class FormosatImageMetadataInterface
 *
 * \brief Creation of an "otb" FormosatImageMetadataInterface that gets metadata.
 *
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT FormosatImageMetadataInterface : public OpticalImageMetadataInterface
{
public:
  typedef FormosatImageMetadataInterface Self;
  typedef ImageMetadataInterfaceBase     Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FormosatImageMetadataInterface, ImageMetadataInterfaceBase);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;

  /** This method is to handle the permutation of the spectral band by some image provider
   * in most cases, this method won't change the value, but for SPOT data, the bands are set up as
   *  2 1 0 3 in the tiff file, this method which is overloaded for SPOT enables to retrieve the
   *  proper band. */
  unsigned int BandIndexToWavelengthPosition(unsigned int i) const override;

  void Parse(ImageMetadata &imd) override;

protected:
  FormosatImageMetadataInterface() = default;
  ~FormosatImageMetadataInterface() = default;

private:
  FormosatImageMetadataInterface(const Self&) = delete;
  void operator=(const Self&) = delete;
  
  void FetchSatAngles(
                    const std::vector<double> & incidenceAngles,
                    const std::vector<double> & alongTrackViewingAngles,
                    const std::vector<double> & acrossTrackViewingAngles,
                    const std::vector<double> & sceneOrientation,
                    const std::string & softwareVersion,
                    double satAzimuth,
                    ImageMetadata & imd);


  /** Vector that contains the filter function value in 6S format (step of 0.0025 micro m).
   * There values a computed by 6S. */
  void FetchSpectralSensitivity(ImageMetadata &);
};

} // end namespace otb

#endif
