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

#ifndef otbGradientBoostedTreeMachineLearningModel_h
#define otbGradientBoostedTreeMachineLearningModel_h

#include "otbRequiresOpenCVCheck.h"

#include "itkLightObject.h"
#include "itkFixedArray.h"
#include "otbMachineLearningModel.h"

#ifndef OTB_OPENCV_3

class CvGBTrees;

namespace otb
{
template <class TInputValue, class TTargetValue>
class ITK_EXPORT GradientBoostedTreeMachineLearningModel
  : public MachineLearningModel <TInputValue, TTargetValue>
{
public:
  /** Standard class typedefs. */
  typedef GradientBoostedTreeMachineLearningModel           Self;
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

  /** Run-time type information (and related methods). */
  itkNewMacro(Self);
  itkTypeMacro(GradientBoostedTreeMachineLearningModel, MachineLearningModel);

  /** Type of the loss function used for training.
   * It must be one of the following types: CvGBTrees::SQUARED_LOSS, CvGBTrees::ABSOLUTE_LOSS,
   * CvGBTrees::HUBER_LOSS, CvGBTrees::DEVIANCE_LOSS.
   * The first three types are used for regression problems, and the last one for classification.
   * Default is CvGBTrees::DEVIANCE_LOSS
   * \see http://docs.opencv.org/modules/ml/doc/gradient_boosted_trees.html#cvgbtreesparams-cvgbtreesparams
   */
  itkGetMacro(LossFunctionType, int);
  itkSetMacro(LossFunctionType, int);

  /** Count of boosting algorithm iterations. weak_count*K is the total count of trees in the GBT model,
   * where K is the output classes count (equal to one in case of a regression).
   * Default is 200
   * \see http://docs.opencv.org/modules/ml/doc/gradient_boosted_trees.html#cvgbtreesparams-cvgbtreesparams
   */
  itkGetMacro(WeakCount, int);
  itkSetMacro(WeakCount, int);

  /** Regularization parameter.
   * Default is 0.8
   * \see http://docs.opencv.org/modules/ml/doc/gradient_boosted_trees.html#cvgbtreesparams-cvgbtreesparams
   */
  itkGetMacro(Shrinkage, double);
  itkSetMacro(Shrinkage, double);

  /** Portion of the whole training set used for each algorithm iteration. Subset is generated randomly.
   *  For more information see http://www.salfordsystems.com/doc/StochasticBoostingSS.pdf.
   *  Default is 0.01
   * \see http://docs.opencv.org/modules/ml/doc/gradient_boosted_trees.html#cvgbtreesparams-cvgbtreesparams
   */
  itkGetMacro(SubSamplePortion, double);
  itkSetMacro(SubSamplePortion, double);

  /** Maximum depth of each decision tree.
   * Default is 3
   * \see http://docs.opencv.org/modules/ml/doc/gradient_boosted_trees.html#cvgbtreesparams-cvgbtreesparams
   */
  itkGetMacro(MaxDepth, int);
  itkSetMacro(MaxDepth, int);

  /** If true then surrogate splits will be built.
   * These splits allow working with missing data and compute variable importance correctly.
   * Default is false
   * \see http://docs.opencv.org/modules/ml/doc/gradient_boosted_trees.html#cvgbtreesparams-cvgbtreesparams
   */
  itkGetMacro(UseSurrogates, bool);
  itkSetMacro(UseSurrogates, bool);

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
  GradientBoostedTreeMachineLearningModel();

  /** Destructor */
  ~GradientBoostedTreeMachineLearningModel() override;

    /** Predict values using the model */
  TargetSampleType DoPredict(const InputSampleType& input, ConfidenceValueType *quality=nullptr) const override;

  
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  GradientBoostedTreeMachineLearningModel(const Self &) = delete;
  void operator =(const Self&) = delete;

  CvGBTrees * m_GBTreeModel;

  int m_LossFunctionType;
  int m_WeakCount;
  double m_Shrinkage;
  double m_SubSamplePortion;
  int m_MaxDepth;
  bool m_UseSurrogates;


};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGradientBoostedTreeMachineLearningModel.hxx"
#endif

#endif
#endif
