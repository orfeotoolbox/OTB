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

#ifndef otbGeometricSarSensorModelAdapter_h
#define otbGeometricSarSensorModelAdapter_h

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
 * \class GeometricSarSensorModelAdapter
 * \brief Wrapper class to access the platform Geometric Sar
 *  Sensor Modelposition from ossim.
 *
 * This class is intended to be used in an InSAR setting to get
 * information relative to the baseline. If necessary, it could be
 * extended to non SAR models, but the optical ossim classes would
 * need to be modified.
 *
 * \ingroup OTBOSSIMAdapters
 **/

class OTBOSSIMAdapters_EXPORT GeometricSarSensorModelAdapter: public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef GeometricSarSensorModelAdapter   Self;
  typedef itk::Object                      Superclass;
  typedef itk::SmartPointer<Self>          Pointer;
  typedef itk::SmartPointer<const Self>    ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GeometricSarSensorModelAdapter, itk::Object);

  void CreateSensorModel(const ImageKeywordlist& image_kwl);

  /** Get the platform position and speed for a given line. */
  void GetPlatformPositionAtLine(
      double line, std::vector<double>& position, std::vector<double>& speed);

  /** Get the platform time for a given line. */
  ossimplugins::JSDDateTime getTime(double line);

protected:
  GeometricSarSensorModelAdapter();
  ~GeometricSarSensorModelAdapter() ITK_OVERRIDE;

private:
  GeometricSarSensorModelAdapter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  ossimplugins::ossimGeometricSarSensorModel* m_SensorModel;

};

} // namespace otb

#endif
