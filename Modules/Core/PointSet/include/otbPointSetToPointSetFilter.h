/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbPointSetToPointSetFilter_h
#define otbPointSetToPointSetFilter_h

#include "otbPointSetSource.h"
#include "otbStreamingTraits.h"

namespace otb
{

/** \class PointSetToPointSetFilter
 * \brief Base class to process PointSet data
 *
 * PointSetToPointSetFilter is the base class for all process objects that output
 * PointSet data and require a PointSet as input. Specifically, this class
 * defines the SetInput() method for defining the input to a filter.
 *
 * \ingroup PointSetFilters
 *
 * \ingroup OTBPointSet
 */
template <class TInputPointSet, class TOutputPointSet>
class ITK_EXPORT PointSetToPointSetFilter : public PointSetSource<TOutputPointSet>
{
public:
  /** Standard class typedefs. */
  typedef PointSetToPointSetFilter        Self;
  typedef PointSetSource<TOutputPointSet> Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(PointSetToPointSetFilter, PointSetSource);

  /** Some Image related typedefs. */
  typedef TInputPointSet                           InputPointSetType;
  typedef typename InputPointSetType::Pointer      InputPointSetPointer;
  typedef typename InputPointSetType::ConstPointer InputPointSetConstPointer;

  /** Some PointSet related typedefs. */
  typedef typename Superclass::OutputPointSetType    OutputPointSetType;
  typedef typename Superclass::OutputPointSetPointer OutputPointSetPointer;
  typedef typename Superclass::PointsContainerType   PointsContainerType;
  typedef itk::ProcessObject                         ProcessObjectType;

  /** Set the input image of this process object.  */
  using Superclass::SetInput;
  void SetInput(unsigned int idx, const InputPointSetType* input);
  void SetInput(const InputPointSetType* input);

  /** Get the input image of this process object.  */
  InputPointSetType* GetInput(unsigned int idx);
  InputPointSetType* GetInput();

protected:
  PointSetToPointSetFilter();
  ~PointSetToPointSetFilter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  PointSetToPointSetFilter(const PointSetToPointSetFilter&) = delete;
  void operator=(const PointSetToPointSetFilter&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPointSetToPointSetFilter.hxx"
#endif

#endif
