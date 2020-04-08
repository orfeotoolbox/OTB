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

#ifndef otbWavelengthSpectralBands_h
#define otbWavelengthSpectralBands_h

#include "otbObjectList.h"
#include "otbFilterFunctionValues.h"
#include "OTBOpticalCalibrationExport.h"

namespace otb
{
/** \class WavelengthSpectralBands
 *  \brief This class contains the values of the filter functions
 *
 * \ingroup IO
 *
 * \ingroup OTBOpticalCalibration
 */
class OTBOpticalCalibration_EXPORT WavelengthSpectralBands : public itk::DataObject
{
public:
  /** Standard typedefs */
  typedef WavelengthSpectralBands       Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkTypeMacro(WavelengthSpectralBands, DataObject);

  /** Creation through object factory macro */
  itkNewMacro(Self);

  typedef FilterFunctionValues                            FilterFunctionValuesType;
  typedef ObjectList<FilterFunctionValuesType>            InternalWavelengthSpectralBandListType;
  typedef InternalWavelengthSpectralBandListType::Pointer WavelengthSpectralBandListType;
  // typedef std::map <FilterFunctionValuesType>        FilterFunctionValuesMapType;

  /** Set the number of bands. */
  itkSetMacro(NbBands, unsigned int);
  /** Get the number of bands. */
  itkGetMacro(NbBands, unsigned int);

protected:
  /** Constructor */
  WavelengthSpectralBands();
  /** Destructor */
  ~WavelengthSpectralBands() override
  {
  }

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  WavelengthSpectralBands(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** List that contains the filter function value. */
  WavelengthSpectralBandListType m_WavelengthSpectralBandsList;
  unsigned int                   m_NbBands;
};

} // end namespace

#endif
