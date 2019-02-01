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

#ifndef otbLabelMapWithClassLabelToLabeledSampleListFilter_h
#define otbLabelMapWithClassLabelToLabeledSampleListFilter_h

#include "otbLabelMapToSampleListFilter.h"

namespace otb
{
/** \class LabelMapWithClassLabelToLabeledSampleListFilter
 * \brief This class converts a LabelObjectMap with some class labeled
 * objets to a SampleList and a TrainingSampleList suitable for supervised learning,
 * and compatible with otb::SVMSampleListModelEstimator
 *
 * The conversion from label object to measurement vector is made
 * through a functor. This allows supporting any kind of LabelObject
 * via proper redefinition of the functor.
 *
 * \sa otb::SVMSampleListModelEstimator
 *
 * \ingroup OTBLabelMap
 */

template <class TInputLabelMap, class TOutputSampleList, class TOutputTrainingSampleList,
    class TMeasurementFunctor = Functor::AttributesMapMeasurementFunctor
    <typename TInputLabelMap::LabelObjectType, typename TOutputSampleList::MeasurementVectorType > >
class ITK_EXPORT LabelMapWithClassLabelToLabeledSampleListFilter :
    public LabelMapToSampleListFilter<TInputLabelMap, TOutputSampleList,TMeasurementFunctor>
{
public:
  /** Standard class typedefs. */
  typedef LabelMapWithClassLabelToLabeledSampleListFilter Self;
  typedef LabelMapToSampleListFilter
  <TInputLabelMap, TOutputSampleList,TMeasurementFunctor> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LabelMapWithClassLabelToLabeledSampleListFilter, LabelMapToSampleListFilter);

  /** template typedefs */
  typedef TInputLabelMap                              InputLabelMapType;
  typedef typename InputLabelMapType::ConstPointer    InputLabelMapConstPointerType;
  typedef typename InputLabelMapType::LabelObjectType LabelObjectType;
  typedef typename InputLabelMapType::ConstIterator   ConstIteratorType;
  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** Output sample list */
  typedef TOutputSampleList                           OutputSampleListType;
  typedef typename OutputSampleListType::Pointer      OutputSampleListPointerType;
  typedef typename OutputSampleListType
  ::MeasurementVectorType                             MeasurementVectorType;

  /** Output training sample list */
  typedef TOutputTrainingSampleList                      OutputTrainingSampleListType;
  typedef typename OutputTrainingSampleListType::Pointer OutputTrainingSampleListPointerType;
  typedef typename OutputTrainingSampleListType
  ::MeasurementVectorType                                TraningVectorType;

  /** Measurement functor */
  typedef TMeasurementFunctor                            MeasurementFunctorType;

  // DataObject type definition from superclass
  typedef typename Superclass::DataObjectPointerType     DataObjectPointerType;

  // Get the output training ListSample
  const OutputTrainingSampleListType* GetOutputTrainingSampleList();

  // Get a hook on the functor for settings
  void SetMeasurementFunctor(const MeasurementFunctorType& functor)
  {
    m_MeasurementFunctor = functor;
  }

  MeasurementFunctorType & GetMeasurementFunctor()
  {
    return m_MeasurementFunctor;
  }

protected:
  LabelMapWithClassLabelToLabeledSampleListFilter();
  ~LabelMapWithClassLabelToLabeledSampleListFilter() override;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void GenerateData() override;

  /** Make Output */
  DataObjectPointerType MakeOutput(DataObjectPointerArraySizeType idx) override;
  using Superclass::MakeOutput;

private:
  LabelMapWithClassLabelToLabeledSampleListFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** The functor used to build the measurement vector */
  MeasurementFunctorType              m_MeasurementFunctor;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelMapWithClassLabelToLabeledSampleListFilter.hxx"
#endif

#endif
