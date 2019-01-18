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

#ifndef otbMapProjectionAdapter_h
#define otbMapProjectionAdapter_h

#include <string>
#include <map>

#include "itkObject.h"
#include "itkObjectFactory.h"

#include "OTBOSSIMAdaptersExport.h"

#include "otbMacro.h"

class ossimProjection;

namespace otb
{

/**
 * \class MapProjectionAdapter
 * \brief Wrapper class to group all dependencies to ossim for map projection
 *
 * This class is NOT intended to be used outside of OTB. Use the
 * GenericMapProjection. If you feel that you need to use it directly,
 * think again!
 *
 * \sa GenericMapProjection
 * \ingroup Projection
 *
 *
 * \ingroup OTBOSSIMAdapters
 **/

class OTBOSSIMAdapters_EXPORT MapProjectionAdapter: public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef MapProjectionAdapter          Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef ossimProjection*       InternalMapProjectionPointer;
  typedef const ossimProjection* InternalMapProjectionConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MapProjectionAdapter, itk::Object);

  InternalMapProjectionPointer GetMapProjection();
  InternalMapProjectionConstPointer GetMapProjection() const;

  std::string GetWkt() const;
  void SetWkt(const std::string& projectionRefWkt);

  void SetParameter(const std::string& key, const std::string& value);
  std::string GetParameter(const std::string& key) const;

  bool InstantiateProjection();

  void InverseTransform(double x, double y, double z,
                        double& lon, double& lat, double& h);
  void ForwardTransform(double lon, double lat, double h,
                        double& x, double& y, double& z);

  void PrintMap() const;

protected:
  MapProjectionAdapter();
  ~MapProjectionAdapter() override;

private:
  MapProjectionAdapter(const Self &) = delete;
  void operator =(const Self&) = delete;

  void ApplyParametersToProjection();

  InternalMapProjectionPointer m_MapProjection;
  std::string                  m_ProjectionRefWkt;

  // Hold the parameters specified directly from the user of the class
  // Consistency is done from ParameterStore -> MapProjection
  // But NOT MapProjection -> ParameterStore
  // so DO NOT use that to read the projection parameters
  typedef std::map<std::string, std::string> StoreType;
  StoreType m_ParameterStore;

  bool m_ReinstanciateProjection;
};

// Some useful free functions related to ossim
namespace Utils
{
int OTBOSSIMAdapters_EXPORT GetZoneFromGeoPoint(double lon, double lat);
}


} // namespace otb

#endif
