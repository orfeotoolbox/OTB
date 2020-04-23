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

#ifndef otbEllipsoidAdapter_h
#define otbEllipsoidAdapter_h

#include "itkObject.h"
#include "itkObjectFactory.h"

#include "OTBOSSIMAdaptersExport.h"

class ossimEllipsoid;

namespace otb
{
/** \class EllipsoidAdapter
 *  \brief class providing the interface to the ossimEllipsoid
 *
 * This is a class to be used internally instead of introducing a dependence on
 * ossimEllipsoid
 *
 * \todo{Add the support for different ellipsoid models}
 *
 *
 * \ingroup OTBOSSIMAdapters
 **/

class OTBOSSIMAdapters_EXPORT EllipsoidAdapter : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef EllipsoidAdapter              Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(EllipsoidAdapter, itk::Object);

  /** Convert a XYZ coordinate into a lon, lat, height on the ellipsoid */
  void XYZToLonLatHeight(double x, double y, double z, double& lon, double& lat, double& h) const;

  /** Convert a lon, lat, height on the ellipsoid into a XYZ geocentric system*/
  void LonLatHeightToXYZ(double lon, double lat, double h, double& x, double& y, double& z) const;

protected:
  EllipsoidAdapter();
  ~EllipsoidAdapter() override;

private:
  EllipsoidAdapter(const Self&) = delete;
  void operator=(const Self&) = delete;

  ossimEllipsoid* m_Ellipsoid;
};

} // namespace otb

#endif
