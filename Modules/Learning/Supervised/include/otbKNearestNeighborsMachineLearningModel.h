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

#ifndef otbKNearestNeighborsMachineLearningModel_h
#define otbKNearestNeighborsMachineLearningModel_h

#include "otbRequiresOpenCVCheck.h"

#include "itkLightObject.h"
#include "itkFixedArray.h"
#include "otbMachineLearningModel.h"

#include "otbOpenCVUtils.h"

namespace otb
{
template <class TInputValue, class TTargetValue>
class ITK_EXPORT KNearestNeighborsMachineLearningModel : public MachineLearningModel<TInputValue, TTargetValue>
{
public:
  /** Standard class typedefs. */
  typedef KNearestNeighborsMachineLearningModel Self;
  typedef MachineLearningModel<TInputValue, TTargetValue> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef typename Superclass::InputValueType       InputValueType;
  typedef typename Superclass::InputSampleType      InputSampleType;
  typedef typename Superclass::InputListSampleType  InputListSampleType;
  typedef typename Superclass::TargetValueType      TargetValueType;
  typedef typename Superclass::TargetSampleType     TargetSampleType;
  typedef typename Superclass::TargetListSampleType TargetListSampleType;
  typedef typename Superclass::ConfidenceValueType  ConfidenceValueType;
  typedef typename Superclass::ProbaSampleType      ProbaSampleType;
  /** Run-time type information (and related methods). */
  itkNewMacro(Self);
  itkTypeMacro(KNearestNeighborsMachineLearningModel, MachineLearningModel);

  /** Setters/Getters to the number of neighbors to use
   *  Default is 32
   *  \see http://docs.opencv.org/modules/ml/doc/k_nearest_neighbors.html
   */
  itkGetMacro(K, int);
  itkSetMacro(K, int);

  /** Decision rule once the KNN are found :
   *  [for classification]
   *   - KNN_VOTING : output value with maximum occurrences (for classification)
   *  [for regression]
   *   - KNN_MEAN : output mean value of neighbors
   *   - KNN_MEDIAN : output median value of neighbors
   */
  enum
  {
    KNN_VOTING,
    KNN_MEAN,
    KNN_MEDIAN
  };

  /** Setters/Getters to the decision rule */
  itkGetMacro(DecisionRule, int);
  itkSetMacro(DecisionRule, int);

  /** Train the machine learning model */
  void Train() override;

  /** Save the model to file */
  void Save(const std::string& filename, const std::string& name = "") override;

  /** Load the model from file */
  void Load(const std::string& filename, const std::string& name = "") override;

  /**\name Classification model file compatibility tests */
  //@{
  /** Is the input model file readable and compatible with the corresponding classifier ? */
  bool CanReadFile(const std::string&) override;

  /** Is the input model file writable and compatible with the corresponding classifier ? */
  bool CanWriteFile(const std::string&) override;
  //@}

protected:
  /** Constructor */
  KNearestNeighborsMachineLearningModel();

  /** Destructor */
  ~KNearestNeighborsMachineLearningModel() override;

  /** Predict values using the model */
  TargetSampleType DoPredict(const InputSampleType& input, ConfidenceValueType* quality = nullptr, ProbaSampleType* proba = nullptr) const override;

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  KNearestNeighborsMachineLearningModel(const Self&) = delete;
  void operator=(const Self&) = delete;

  cv::Ptr<cv::ml::KNearest> m_KNearestModel;

  int m_K;

  int m_DecisionRule;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbKNearestNeighborsMachineLearningModel.hxx"
#endif

#endif
