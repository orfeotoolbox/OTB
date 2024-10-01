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

#ifndef otbOpticalImageMetadataInterface_h
#define otbOpticalImageMetadataInterface_h

#include <string>

#include "otbImageMetadataInterfaceBase.h"
#include "otbFilterFunctionValues.h"
#include "otbObjectList.h"

namespace otb
{
/** \class OpticalImageMetadataInterface
 *
 * \brief Class for Optical captor metadata reading.
 *
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT OpticalImageMetadataInterface : public ImageMetadataInterfaceBase
{
public:
  typedef OpticalImageMetadataInterface Self;
  typedef ImageMetadataInterfaceBase    Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(OpticalImageMetadataInterface, ImageMetadataInterfaceBase);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;

  typedef FilterFunctionValues                              FilterFunctionValuesType;
  typedef otb::ObjectList<FilterFunctionValuesType>         InternalWavelengthSpectralBandVectorType;
  typedef InternalWavelengthSpectralBandVectorType::Pointer WavelengthSpectralBandVectorType;

  /** This method is to handle the permutation of the spectral band by some image provider
   * in most cases, this method won't change the value, but for SPOT data, the bands are set up as
   *  2 1 0 3 in the tiff file, this method which is overloaded for SPOT enables to retrieve the
   *  proper band. */
  virtual unsigned int BandIndexToWavelengthPosition(unsigned int i) const;

protected:
  OpticalImageMetadataInterface() = default;
  ~OpticalImageMetadataInterface() override = default;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;


private:
  OpticalImageMetadataInterface(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif
