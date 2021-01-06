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

#ifndef otbWrapperBandParameter_h
#define otbWrapperBandParameter_h

#include <string>
#include "otbWrapperListViewParameter.h"

namespace otb
{
namespace Wrapper
{

/** \class BandParameter
 *  \brief This class represents a band parameter selected from a raster file
 * for the wrapper framework
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT BandParameter : public ListViewParameter
{
public:
  /** Standard class typedef */
  typedef BandParameter                 Self;
  typedef ListViewParameter             Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(BandParameter, ListViewParameter);

  ParameterType GetType() const override
  {
    return ParameterType_Band;
  }

  /** Set raster data name from which we choose the fields */
  void SetRasterData(std::string rasterData)
  {
    m_RasterData = std::move(rasterData);
  }

  /** Get raster data name from which we choose the fields */
  const std::string& GetRasterData() const
  {
    return m_RasterData;
  }

protected:
  /** Constructor */
  BandParameter() = default;

  /** Destructor */
  ~BandParameter() override = default;

  std::string m_RasterData;

private:
  BandParameter(const BandParameter&) = delete;
  void operator=(const BandParameter&) = delete;

}; // End class BandParameter

} // End namespace Wrapper
} // End namespace otb

#endif
