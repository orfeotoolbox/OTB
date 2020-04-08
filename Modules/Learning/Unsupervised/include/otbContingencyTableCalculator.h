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

#ifndef otbContingencyTableCalculator_h
#define otbContingencyTableCalculator_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "otbContingencyTable.h"
#include <map>

namespace otb
{
/**
 * \brief  ContingencyTableCalculator provide facilities to compute ContingencyTable.
 *
 * The ContingencyTableCalculator can be used with different structure type,
 * the size of the label list should be the same for both list.
 *
 * \tparam TClassLabel the label data type
 * \ingroup OTBUnsupervised
 */
template <class TClassLabel>
class ITK_EXPORT ContingencyTableCalculator : public itk::Object
{
public:
  /** Standard class typedefs */
  typedef ContingencyTableCalculator    Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ContingencyTableCalculator, itk::Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef ContingencyTable<TClassLabel>          ContingencyTableType;
  typedef typename ContingencyTableType::Pointer ContingencyTablePointerType;

  typedef typename std::map<TClassLabel, unsigned long> CountMapType;
  typedef typename std::map<TClassLabel, CountMapType>  MapOfClassesType;

  /** Populate the confusion Matrix for a image iteration.
   *  \tparam TRefListLabel data structure type which contain the reference labels.
   *  \tparam TProdListLabel data structure type which contain the produced labels.
   */
  template <class TRefIterator, class TProdIterator>
  void Compute(TRefIterator itRef, TProdIterator itProd, bool refHasNoData = false, typename TRefIterator::InternalPixelType refNoData = 0,
               bool prodHasNoData = false, typename TProdIterator::InternalPixelType prodNoData = 0);

  /** Populate the confusion Matrix with input which provide GetMeasurementVector()[0] access
   *  \tparam TRefListLabel data structure type which contain the reference labels.
   *  \tparam TProdListLabel data structure type which contain the produced labels.
   */
  template <class TRefIterator, class TProdIterator>
  void Compute(TRefIterator refBegin, TRefIterator refEnd, TProdIterator prodBegin, TProdIterator prodEnd);

  itkGetConstMacro(NumberOfRefClasses, unsigned long);
  itkGetConstMacro(NumberOfProdClasses, unsigned long);
  itkGetConstMacro(NumberOfSamples, unsigned long);

  void                        Clear();
  ContingencyTablePointerType BuildContingencyTable();

protected:
  ContingencyTableCalculator();
  ~ContingencyTableCalculator() override
  {
  }
  // void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ContingencyTableCalculator(const Self&) = delete;
  void operator=(const Self&) = delete;

  MapOfClassesType m_LabelCount;
  unsigned long    m_NumberOfRefClasses;
  unsigned long    m_NumberOfProdClasses;
  unsigned long    m_NumberOfSamples;
};
}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbContingencyTableCalculator.hxx"
#endif

#endif // otbContingencyTableCalculator_h
