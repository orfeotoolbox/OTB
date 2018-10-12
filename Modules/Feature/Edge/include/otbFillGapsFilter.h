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

#ifndef otbFillGapsFilter_h
#define otbFillGapsFilter_h


#include "itkProcessObject.h"
#include "otbLineSpatialObjectList.h"
#include "OTBEdgeExport.h"

namespace otb
{
/** \class FillGapsFilter
 * \brief To be documented
 *
 * \ingroup OTBEdge
 */
class OTBEdge_EXPORT FillGapsFilter : public itk::ProcessObject
{

public:
  /** Standard class typedefs. */
  typedef FillGapsFilter                Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FillGapsFilter, itk::ProcessObject);

  /** Some convenient typedefs. */
  typedef LineSpatialObjectList                LineSpatialObjectListType;
  typedef LineSpatialObjectListType::Pointer   LineSpatialObjectListPointer;
  typedef LineSpatialObjectListType::LineType  LineSpatialObjectType;
  typedef LineSpatialObjectType::PointListType PointListType;
  typedef LineSpatialObjectType::LinePointType PointType;

  typedef itk::ProcessObject ProcessObjectType;

  using Superclass::SetInput;
  void SetInput(const LineSpatialObjectListType * input);
  const LineSpatialObjectListType * GetInput();

  LineSpatialObjectListType * GetOutput();

  itkSetMacro(AngularBeam, double);
  itkGetConstReferenceMacro(AngularBeam, double);
  itkSetMacro(Radius, double);
  itkGetConstReferenceMacro(Radius, double);

protected:
  FillGapsFilter();

  ~FillGapsFilter() override {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void GenerateData() override;

private:
  FillGapsFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  double m_AngularBeam;
  double m_Radius;

};

} // end namespace otb

#endif
