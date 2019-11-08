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

#ifndef otbSVMMarginSampler_h
#define otbSVMMarginSampler_h

#include "itkSampleClassifierFilter.h"
#include "itkVectorImage.h"

namespace otb
{

/** \class SVMMarginSampler
 *  \brief Implement one iteration of active learning by margin
 *  \ingroup ActiveLearningFilters
 *
 * \ingroup OTBSupervised
 */

template <class TSample, class TModel>
class ITK_EXPORT SVMMarginSampler : public itk::Statistics::SampleClassifierFilter<TSample>
{
public:
  /** Standard class typedef*/
  typedef SVMMarginSampler                                 Self;
  typedef itk::Statistics::SampleClassifierFilter<TSample> Superclass;
  typedef itk::SmartPointer<Self>                          Pointer;
  typedef itk::SmartPointer<const Self>                    ConstPointer;

  /** Standard macros */
  itkTypeMacro(SVMMarginSampler, itk::Statistics::SampleClassifierFilter);
  itkNewMacro(Self);

  /** Output type for GetClassSample method */
  typedef typename Superclass::MembershipSampleType OutputType;

  /** typedefs from TSample object */
  typedef typename TSample::MeasurementType       MeasurementType;
  typedef typename TSample::MeasurementVectorType MeasurementVectorType;
  typedef std::pair<unsigned int, double> IndexAndDistanceType;
  typedef std::vector<IndexAndDistanceType> IndexAndDistanceVectorType;
  typedef std::vector<unsigned int>         IndexVectorType;


  static bool Compare(const IndexAndDistanceType& l1, const IndexAndDistanceType& l2)
  {
    bool resp = l2.second > l1.second;
    return resp;
  }

  /** Type definitions for the SVM Model. */
  typedef TModel                         SVMModelType;
  typedef typename SVMModelType::Pointer SVMModelPointer;

  itkSetMacro(NumberOfCandidates, unsigned int);
  itkGetMacro(NumberOfCandidates, unsigned int);

  /** Set the model */
  itkSetObjectMacro(Model, SVMModelType);

  /** Get the number of classes. */
  itkGetObjectMacro(Model, SVMModelType);

  const IndexVectorType& GetMarginSamples()
  {
    return m_MarginSamples;
  }

protected:
  SVMMarginSampler();
  ~SVMMarginSampler() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Starts the classification process */
  void         GenerateData() override;
  virtual void DoMarginSampling();

private:
  SVMModelPointer m_Model;

  unsigned int m_NumberOfCandidates;

  IndexVectorType m_MarginSamples;

}; // end of class


} // end of namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSVMMarginSampler.hxx"
#endif

#endif
