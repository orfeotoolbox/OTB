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

#ifndef otbPointSetExtractROI_h
#define otbPointSetExtractROI_h

#include "otbPointSetToPointSetFilter.h"
#include "itkTransform.h"

namespace otb
{

/** \class PointSetExtractROI
 * \brief Extract a part of the points specified by a region
 *
 * PointSetExtractROI extract all the points of a PointSet that are
 * in a given region.
 *
 * The additional content of the PointSet is passed untouched.
 *
 *
 * \ingroup PointSetFilters
 *
 * \ingroup OTBPointSet
 */
template <class TInputPointSet, class TOutputPointSet>
class ITK_EXPORT PointSetExtractROI : public PointSetToPointSetFilter<TInputPointSet, TOutputPointSet>
{
public:
  /** Standard class typedefs. */
  typedef PointSetExtractROI Self;
  typedef PointSetToPointSetFilter<TInputPointSet, TOutputPointSet> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef TInputPointSet                       InputPointSetType;
  typedef TOutputPointSet                      OutputPointSetType;
  typedef typename InputPointSetType::Pointer  InputPointSetPointer;
  typedef typename OutputPointSetType::Pointer OutputPointSetPointer;

  /** Type for representing coordinates. */
  typedef typename TInputPointSet::CoordRepType CoordRepType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(PointSetExtractROI, PointSetToPointSetFilter);

  /** Set/Get Start methods */
  itkGetMacro(StartX, unsigned long);
  itkGetMacro(StartY, unsigned long);
  itkSetMacro(StartX, unsigned long);
  itkSetMacro(StartY, unsigned long);
  /** Set/Get Size methods */
  itkGetMacro(SizeX, unsigned long);
  itkGetMacro(SizeY, unsigned long);
  itkSetMacro(SizeX, unsigned long);
  itkSetMacro(SizeY, unsigned long);

protected:
  PointSetExtractROI();
  ~PointSetExtractROI() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Generate Requested Data */
  void GenerateData(void) override;

private:
  PointSetExtractROI(const PointSetExtractROI&) = delete;
  void operator=(const PointSetExtractROI&) = delete;

  /** X/Y coordinates of the first point of the region to extract. */
  unsigned long m_StartX;
  unsigned long m_StartY;
  /** Size in X/Y of the region to extract. */
  unsigned long m_SizeX;
  unsigned long m_SizeY;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPointSetExtractROI.hxx"
#endif

#endif
