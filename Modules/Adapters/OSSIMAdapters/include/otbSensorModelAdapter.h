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

#ifndef otbSensorModelAdapter_h
#define otbSensorModelAdapter_h

#include "otbOssimDEMHandler.h"

class ossimProjection;
class ossimTieGptSet;

namespace otb
{

class ImageKeywordlist;

/**
 * \class SensorModelAdapter
 * \brief Wrapper class to group all dependencies to ossim for sensor models
 *
 * This class is NOT intended to be used outside of OTB. Use the
 * InverseSensorModel and ForwardSensorModel. If you feel that you need to use
 * it directly, think again!
 *
 * \sa InverseSensorModel
 * \sa ForwardSensorModel
 * \ingroup Projection
 *
 *
 * \ingroup OTBOSSIMAdapters
 **/

class OTBOSSIMAdapters_EXPORT SensorModelAdapter : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef SensorModelAdapter            Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef ossimProjection*       InternalMapProjectionPointer;
  typedef const ossimProjection* InternalMapProjectionConstPointer;
  typedef ossimTieGptSet*        InternalTiePointsContainerPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SensorModelAdapter, itk::Object);

  /** Create the projection ( m_Model). Called by the SetImageGeometry methods */
  void CreateProjection(const ImageKeywordlist& image_kwl);
  // FIXME check if it should be protected instead

  /** Forward sensor modelling with elevation (above ellipsoid) provided by the user */
  void ForwardTransformPoint(double x, double y, double z, double& lon, double& lat, double& h) const;

  /** Forward sensor modelling with elevation (above ellipsoid) estimated by the
   *  algorithm */
  void ForwardTransformPoint(double x, double y, double& lon, double& lat, double& h) const;

  /** Inverse sensor modelling with elevation (above ellipsoid) provided by the user */
  void InverseTransformPoint(double lon, double lat, double h, double& x, double& y, double& z) const;

  /** Inverse sensor modelling with elevation (above ellipsoid) from DEMHandler */
  void InverseTransformPoint(double lon, double lat, double& x, double& y, double& z) const;


  /** Add a tie point with elevation (above ellipsoid) provided by the user */
  void AddTiePoint(double x, double y, double z, double lon, double lat);

  /** Add a tie point with elevation (above ellipsoid) from DEMHandler */
  void AddTiePoint(double x, double y, double lon, double lat);

  /** Clear all tie points */
  void ClearTiePoints();

  /** Optimize sensor model with respect to tie points */
  double Optimize();

  /** Is sensor model valid method. return false if the m_SensorModel is null*/
  bool IsValidSensorModel() const;

  /** Read geom file and instantiate sensor model */
  bool ReadGeomFile(const std::string& infile);

  /** Write geom file corresponding to sensor model */
  bool WriteGeomFile(const std::string& outfile);

protected:
  SensorModelAdapter();
  ~SensorModelAdapter() override;

private:
  SensorModelAdapter(const Self&) = delete;
  void operator=(const Self&) = delete;

  InternalMapProjectionPointer m_SensorModel;

  InternalTiePointsContainerPointer m_TiePoints;

  /** Object that read and use DEM */
  OssimDEMHandler::Pointer m_DEMHandler;
};

} // namespace otb

#endif
