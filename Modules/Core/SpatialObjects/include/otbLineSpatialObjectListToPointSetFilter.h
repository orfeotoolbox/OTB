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

#ifndef otbLineSpatialObjectListToPointSetFilter_h
#define otbLineSpatialObjectListToPointSetFilter_h


#include "itkProcessObject.h"
#include "otbLineSpatialObjectList.h"

namespace otb
{
/** \class LineSpatialObjectListToPointSetFilter
 *  \brief Base class for all process objects that output PointSets And use LineSpatialObjectList
 *         as input.
 *
 *
 *
 * \ingroup OTBSpatialObjects
 */

template <class TLinesList, class TPointSet>
class ITK_EXPORT LineSpatialObjectListToPointSetFilter : public itk::ProcessObject
{
public:

  /** Standard class typedefs. */
  typedef LineSpatialObjectListToPointSetFilter Self;
  typedef itk::ProcessObject                    Superclass;
  typedef itk::SmartPointer<Self>               Pointer;
  typedef itk::SmartPointer<const Self>         ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LineSpatialObjectListToPointSetFilter, itk::ProcessObject);

  /** Some convenient typedefs. */
  typedef TLinesList                       LinesListType;
  typedef typename LinesListType::LineType LineType;

  /** typedef support for output*/
  typedef TPointSet PointSetType;

  typedef itk::ProcessObject ProcessObjectType;

  /** Set/Get the input image */
  using Superclass::SetInput;
  virtual void SetInput(const LinesListType *list);
  virtual const LinesListType* GetInput(void);

  /** Set/Get the list of LineSpatialObject of this process object.  */
  using Superclass::SetOutput;
  virtual void SetOutput(const PointSetType *pointSet);
  virtual PointSetType* GetOutput(void);

protected:
  LineSpatialObjectListToPointSetFilter();
  ~LineSpatialObjectListToPointSetFilter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  LineSpatialObjectListToPointSetFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLineSpatialObjectListToPointSetFilter.hxx"
#endif

#endif
