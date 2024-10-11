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

#ifndef otbDecisionTreeMachineLearningModel_h
#define otbDecisionTreeMachineLearningModel_h

#include "otbRequiresOpenCVCheck.h"

#include "itkLightObject.h"
#include "itkFixedArray.h"
#include "otbMachineLearningModel.h"

#include "otbOpenCVUtils.h"

namespace otb
{
template <class TInputValue, class TTargetValue>
class ITK_EXPORT DecisionTreeMachineLearningModel : public MachineLearningModel<TInputValue, TTargetValue>
{
public:
  /** Standard class typedefs. */
  typedef DecisionTreeMachineLearningModel Self;
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
  itkTypeMacro(DecisionTreeMachineLearningModel, MachineLearningModel);

  /** Setters/Getters to the maximum depth of the tree.
   * The maximum possible depth of the tree. That is the training algorithms attempts to split a node
   * while its depth is less than MaxDepth. The actual depth may be smaller if the other termination criteria
   * are met, and/or if the tree is pruned.
   * Default is INT_MAX
   * \see http://docs.opencv.org/modules/ml/doc/decision_trees.html#CvDTreeParams::CvDTreeParams%28%29
   */
  itkGetMacro(MaxDepth, int);
  itkSetMacro(MaxDepth, int);

  /** Setters/Getters to the minimum number of sample in each node.
   * If the number of samples in a node is less than this parameter then the node will not be split.
   * Default is 10
   * \see http://docs.opencv.org/modules/ml/doc/decision_trees.html#CvDTreeParams::CvDTreeParams%28%29
   */
  itkGetMacro(MinSampleCount, int);
  itkSetMacro(MinSampleCount, int);

  /** Termination Criteria for regression tree.
   * If all absolute differences between an estimated value in a node
   * and values of train samples in this node are less than this parameter
   * then the node will not be split.
   * Default is 0.01
   * \see http://docs.opencv.org/modules/ml/doc/decision_trees.html#CvDTreeParams::CvDTreeParams%28%29
   */
  itkGetMacro(RegressionAccuracy, double);
  itkSetMacro(RegressionAccuracy, double);

  /** If true then surrogate splits will be built.
   * These splits allow working with missing data and compute variable importance correctly.
   * Default is true
   * \see http://docs.opencv.org/modules/ml/doc/decision_trees.html#CvDTreeParams::CvDTreeParams%28%29
   */
  itkGetMacro(UseSurrogates, bool);
  itkSetMacro(UseSurrogates, bool);

  /** Cluster possible values of a categorical variable into \f$ K \leq MaxCategories \f$ clusters to find
   *  a suboptimal split. If a discrete variable, on which the training procedure tries to make a split,
   *  takes more than max_categories values, the precise best subset estimation may take a very long time
   *  because the algorithm is exponential. Instead, many decision trees engines (including ML) try to find
   *  sub-optimal split in this case by clustering all the samples into max_categories clusters
   *  that is some categories are merged together. The clustering is applied only in n>2-class classification problems
   *  for categorical variables with N > max_categories possible values. In case of regression and 2-class classification
   *  the optimal split can be found efficiently without employing clustering, thus the parameter is not used in these cases.
   * Default is 10
   * \see http://docs.opencv.org/modules/ml/doc/decision_trees.html#CvDTreeParams::CvDTreeParams%28%29
   */
  itkGetMacro(MaxCategories, int);
  itkSetMacro(MaxCategories, int);

  /** If true then a pruning will be harsher. This will make a tree more compact and
   * more resistant to the training data noise but a bit less accurate.
   * Default is true
   * \see http://docs.opencv.org/modules/ml/doc/decision_trees.html#CvDTreeParams::CvDTreeParams%28%29
   */
  itkGetMacro(Use1seRule, bool);
  itkSetMacro(Use1seRule, bool);

  /** If true then pruned branches are physically removed from the tree.
   * Otherwise they are retained and it is possible to get results
   * from the original unpruned (or pruned less aggressively) tree by decreasing CvDTree::pruned_tree_idx parameter.
   * Default is true
   * \see http://docs.opencv.org/modules/ml/doc/decision_trees.html#CvDTreeParams::CvDTreeParams%28%29
   */
  itkGetMacro(TruncatePrunedTree, bool);
  itkSetMacro(TruncatePrunedTree, bool);


  /*  The array of a priori class probabilities, sorted by the class label
  * value. The parameter can be used to tune the decision tree preferences toward
  * a certain class. For example, if you want to detect some rare anomaly
  * occurrence, the training base will likely contain much more normal cases than
  * anomalies, so a very good classification performance will be achieved just by
  * considering every case as normal. To avoid this, the priors can be specified,
  * where the anomaly probability is artificially increased (up to 0.5 or even
  * greater), so the weight of the misclassified anomalies becomes much bigger,
  * and the tree is adjusted properly. You can also think about this parameter as
  * weights of prediction categories which determine relative weights that you
  * give to misclassification. That is, if the weight of the first category is 1
  * and the weight of the second category is 10, then each mistake in predicting
  * the second category is equivalent to making 10 mistakes in predicting the
  first category. */

  std::vector<float> GetPriors() const
  {
    return m_Priors;
  }

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
  DecisionTreeMachineLearningModel();

  /** Destructor */
  ~DecisionTreeMachineLearningModel() override = default;

  /** Predict values using the model */
  TargetSampleType DoPredict(const InputSampleType& input, ConfidenceValueType* quality = nullptr, ProbaSampleType* proba = nullptr) const override;

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  DecisionTreeMachineLearningModel(const Self&) = delete;
  void operator=(const Self&) = delete;

  cv::Ptr<cv::ml::DTrees> m_DTreeModel;

  int                m_MaxDepth;
  int                m_MinSampleCount;
  double             m_RegressionAccuracy;
  bool               m_UseSurrogates;
  int                m_MaxCategories;
  bool               m_Use1seRule;
  bool               m_TruncatePrunedTree;
  std::vector<float> m_Priors;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDecisionTreeMachineLearningModel.hxx"
#endif

#endif
