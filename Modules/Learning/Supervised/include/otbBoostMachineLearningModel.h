/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbBoostMachineLearningModel_h
#define otbBoostMachineLearningModel_h

#include "otbRequiresOpenCVCheck.h"

#include "itkLightObject.h"
#include "itkFixedArray.h"
#include "otbMachineLearningModel.h"

#ifdef OTB_OPENCV_3
#include "otbOpenCVUtils.h"
#else
class CvBoost;
#endif

namespace otb
{
template <class TInputValue, class TTargetValue>
class ITK_EXPORT BoostMachineLearningModel
  : public MachineLearningModel <TInputValue, TTargetValue>
{
public:
  /** Standard class typedefs. */
  typedef BoostMachineLearningModel           Self;
  typedef MachineLearningModel<TInputValue, TTargetValue> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  typedef typename Superclass::InputValueType             InputValueType;
  typedef typename Superclass::InputSampleType            InputSampleType;
  typedef typename Superclass::InputListSampleType        InputListSampleType;
  typedef typename Superclass::TargetValueType            TargetValueType;
  typedef typename Superclass::TargetSampleType           TargetSampleType;
  typedef typename Superclass::TargetListSampleType       TargetListSampleType;
  typedef typename Superclass::ConfidenceValueType        ConfidenceValueType;
  typedef typename Superclass::ProbaSampleType            ProbaSampleType;
  /** Run-time type information (and related methods). */
  itkNewMacro(Self);
  itkTypeMacro(BoostMachineLearningModel, MachineLearningModel);

  /** Setters/Getters to the Boost type
   *  It can be CvBoost::DISCRETE, CvBoost::REAL, CvBoost::LOGIT, CvBoost::GENTLE
   *  Default is CvBoost::REAL.
   *  \see http://docs.opencv.org/modules/ml/doc/boosting.html#cvboostparams-cvboostparams
   */
  itkGetMacro(BoostType, int);
  itkSetMacro(BoostType, int);

  /** Setters/Getters to the split criteria
   *  It can be CvBoost::DEFAULT, CvBoost::GINI, CvBoost::MISCLASS, CvBoost::SQERR
   *  Default is CvBoost::DEFAULT. It uses default value according to \c BoostType
   *  \see http://docs.opencv.org/modules/ml/doc/boosting.html#cvboost-predict
   */
  itkGetMacro(SplitCrit, int);
  itkSetMacro(SplitCrit, int);

  /** Setters/Getters to the number of weak classifiers.
   *  Default is 100.
   *  \see http://docs.opencv.org/modules/ml/doc/boosting.html#cvboostparams-cvboostparams
   */
  itkGetMacro(WeakCount, int);
  itkSetMacro(WeakCount, int);

  /** Setters/Getters to the threshold WeightTrimRate.
   *  A threshold between 0 and 1 used to save computational time.
   *  Samples with summary weight \f$ w \leq 1 - WeightTrimRate \f$ do not participate in the next iteration of training.
   *  Set this parameter to 0 to turn off this functionality.
   *  Default is 0.95
   *  \see http://docs.opencv.org/modules/ml/doc/boosting.html#cvboostparams-cvboostparams
   */
  itkGetMacro(WeightTrimRate, double);
  itkSetMacro(WeightTrimRate, double);

  /** Setters/Getters to the maximum depth of the tree.
   * Default is 1
   * \see http://docs.opencv.org/modules/ml/doc/decision_trees.html#CvDTreeParams::CvDTreeParams%28%29
   */
  itkGetMacro(MaxDepth, int);
  itkSetMacro(MaxDepth, int);

  /** Train the machine learning model */
  void Train() override;

  /** Save the model to file */
  void Save(const std::string & filename, const std::string & name="") override;

  /** Load the model from file */
  void Load(const std::string & filename, const std::string & name="") override;

  /**\name Classification model file compatibility tests */
  //@{
  /** Is the input model file readable and compatible with the corresponding classifier ? */
  bool CanReadFile(const std::string &) override;

  /** Is the input model file writable and compatible with the corresponding classifier ? */
  bool CanWriteFile(const std::string &) override;
  //@}

protected:
  /** Constructor */
  BoostMachineLearningModel();

  /** Destructor */
  ~BoostMachineLearningModel() override;

  /** Predict values using the model */
  TargetSampleType DoPredict(const InputSampleType& input, ConfidenceValueType *quality=nullptr, ProbaSampleType *proba=nullptr) const override;
  
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  BoostMachineLearningModel(const Self &) = delete;
  void operator =(const Self&) = delete;

#ifdef OTB_OPENCV_3
  cv::Ptr<cv::ml::Boost> m_BoostModel;
#else
  CvBoost * m_BoostModel;
#endif
  int m_BoostType;
  int m_WeakCount;
  double m_WeightTrimRate;
  int m_SplitCrit;
  int m_MaxDepth;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBoostMachineLearningModel.hxx"
#endif

#endif
