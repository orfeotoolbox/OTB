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

#ifndef otbPlatformPositionAdapter_h
#define otbPlatformPositionAdapter_h

#include <vector>

#include "itkObject.h"
#include "itkObjectFactory.h"

#include "OTBOSSIMAdaptersExport.h"

namespace ossimplugins
{
class ossimGeometricSarSensorModel;
class JSDDateTime;
}

namespace otb
{

class ImageKeywordlist;

/**
 * \class PlatformPositionAdapter
 * \brief Wrapper class to access the platform position from ossim
 *
 * This class is intended to be used in an InSAR setting to get
 * information relative to the baseline. If necessary, it could be
 * extended to non SAR models, but the optical ossim classes would
 * need to be modified.
 *
 * \ingroup OTBOSSIMAdapters
 **/

class OTBOSSIMAdapters_EXPORT PlatformPositionAdapter: public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef PlatformPositionAdapter       Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(PlatformPositionAdapter, itk::Object);

  void CreateSensorModel(const ImageKeywordlist& image_kwl);

  /** Get the platform position and speed for a given line. */
  void GetPlatformPosition(
      double line, std::vector<double>& position, std::vector<double>& speed);

  /** Get the platform position at a given date and time. */
  void GetPlatformPositionAtTime(
      ossimplugins::JSDDateTime time, std::vector<double>& position, std::vector<double>& speed);

protected:
  PlatformPositionAdapter();
  ~PlatformPositionAdapter() override;

private:
  PlatformPositionAdapter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  ossimplugins::ossimGeometricSarSensorModel* m_SensorModel;

};

} // namespace otb

#endif
