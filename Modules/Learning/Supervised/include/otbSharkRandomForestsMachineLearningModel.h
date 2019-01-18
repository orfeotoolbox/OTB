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

#ifndef otbSharkRandomForestsMachineLearningModel_h
#define otbSharkRandomForestsMachineLearningModel_h

#include "itkLightObject.h"
#include "otbMachineLearningModel.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wcast-align"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wheader-guard"
#pragma clang diagnostic ignored "-Wexpansion-to-defined"
#else
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
#endif
#include <shark/Models/Classifier.h>
#include "otb_shark.h"
#include "shark/Algorithms/Trainers/RFTrainer.h"
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif


/** \class SharkRandomForestsMachineLearningModel
 *  \brief Shark version of Random Forests algorithm
 *
 *  This is a specialization of MachineLearningModel class allowing to
 *  use Shark implementation of the Random Forests algorithm.
 *
 *  It is noteworthy that training step is parallel.
 * 
 *  For more information, see
 *  http://image.diku.dk/shark/doxygen_pages/html/classshark_1_1_r_f_trainer.html
 * 
 *  \ingroup OTBSupervised
 */

namespace otb
{
template <class TInputValue, class TTargetValue>
class ITK_EXPORT SharkRandomForestsMachineLearningModel
  : public MachineLearningModel <TInputValue, TTargetValue>
{
public:
  /** Standard class typedefs. */
  typedef SharkRandomForestsMachineLearningModel               Self;
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
  typedef typename Superclass::ConfidenceSampleType       ConfidenceSampleType;
  typedef typename Superclass::ConfidenceListSampleType   ConfidenceListSampleType;
  typedef typename Superclass::ProbaSampleType             ProbaSampleType;
  typedef typename Superclass::ProbaListSampleType        ProbaListSampleType;
  /** Run-time type information (and related methods). */
  itkNewMacro(Self);
  itkTypeMacro(SharkRandomForestsMachineLearningModel, MachineLearningModel);

  /** Train the machine learning model */
  virtual void Train() override;

  /** Save the model to file */
  virtual void Save(const std::string & filename, const std::string & name="") override;

  /** Load the model from file */
  virtual void Load(const std::string & filename, const std::string & name="") override;

  /**\name Classification model file compatibility tests */
  //@{
  /** Is the input model file readable and compatible with the corresponding classifier ? */
  virtual bool CanReadFile(const std::string &) override;

  /** Is the input model file writable and compatible with the corresponding classifier ? */
  virtual bool CanWriteFile(const std::string &) override;
  //@}

  /** From Shark doc: Get the number of trees to grow.*/
  itkGetMacro(NumberOfTrees,unsigned int);
  /** From Shark doc: Set the number of trees to grow.*/
  itkSetMacro(NumberOfTrees,unsigned int);

  /** From Shark doc: Get the number of random attributes to investigate at each node.*/
  itkGetMacro(MTry, unsigned int);
  /** From Shark doc: Set the number of random attributes to investigate at each node.*/
  itkSetMacro(MTry, unsigned int);

  /** From Shark doc: Controls when a node is considered pure. If set
* to 1, a node is pure when it only consists of a single node.
*/
  itkGetMacro(NodeSize, unsigned int);
    /** From Shark doc: Controls when a node is considered pure. If
* set to 1, a node is pure when it only consists of a single node.
 */
  itkSetMacro(NodeSize, unsigned int);

  /** From Shark doc: Get the fraction of the original training
* dataset to use as the out of bag sample. The default value is
* 0.66.*/
  itkGetMacro(OobRatio, float);

  /** From Shark doc: Set the fraction of the original training
* dataset to use as the out of bag sample. The default value is 0.66.
*/
  itkSetMacro(OobRatio, float);

  /** If true, margin confidence value will be computed */
  itkGetMacro(ComputeMargin, bool);
  /** If true, margin confidence value will be computed */
  itkSetMacro(ComputeMargin, bool);

  /** If true, class labels will be normalised in [0 ... nbClasses] */
  itkGetMacro(NormalizeClassLabels, bool);
  itkSetMacro(NormalizeClassLabels, bool);

protected:
  /** Constructor */
  SharkRandomForestsMachineLearningModel();

  /** Destructor */
  virtual ~SharkRandomForestsMachineLearningModel();

  /** Predict values using the model */
  TargetSampleType DoPredict(const InputSampleType& input, ConfidenceValueType *quality=nullptr, ProbaSampleType *proba=nullptr) const override;
  
  void DoPredictBatch(const InputListSampleType *, const unsigned int & startIndex, const unsigned int & size, TargetListSampleType *, ConfidenceListSampleType * = nullptr, ProbaListSampleType * = nullptr) const override;
  
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  SharkRandomForestsMachineLearningModel(const Self &) = delete;
  void operator =(const Self&) = delete;

  shark::RFClassifier<unsigned int> m_RFModel;
  shark::RFTrainer<unsigned int> m_RFTrainer;
  std::vector<unsigned int> m_ClassDictionary;
  bool m_NormalizeClassLabels;

  unsigned int m_NumberOfTrees;
  unsigned int m_MTry;
  unsigned int m_NodeSize;
  float m_OobRatio;
  bool m_ComputeMargin;

  /** Confidence list sample */
  ConfidenceValueType ComputeConfidence(shark::RealVector & probas, 
                                        bool computeMargin) const;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSharkRandomForestsMachineLearningModel.hxx"
#endif

#endif
