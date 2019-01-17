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

#ifndef otbSpatialObjectSource_h
#define otbSpatialObjectSource_h

#include "otbMacro.h"
#include "itkProcessObject.h"

namespace otb
{
/**
 * \class SpatialObjectSource
 * \brief Base class for filters producing a SpatialObject as output.
 * \ingroup DataSources
 *
 * \ingroup OTBSpatialObjects
 */
template <class TSpatialObject>
class ITK_EXPORT SpatialObjectSource
  : public itk::ProcessObject
{
public:
  /** Standard typedefs */
  typedef SpatialObjectSource           Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(SpatialObjectSource, itk::ProcessObject);

  /** Template parameters typedefs */
  typedef TSpatialObject                      SpatialObjectType;
  typedef typename SpatialObjectType::Pointer SpatialObjectPointerType;

  /** Data object pointer */
  typedef itk::DataObject::Pointer DataObjectPointer;

  /**
   * Get the output spatial object.
   * \return The output spatial object.
   */
  virtual SpatialObjectType * GetOutput(void);

protected:
  /** Constructor */
  SpatialObjectSource();
  /** Destructor */
  ~SpatialObjectSource() override {}
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  SpatialObjectSource(const Self &) = delete;
  void operator =(const Self&) = delete;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSpatialObjectSource.hxx"
#endif

#endif
