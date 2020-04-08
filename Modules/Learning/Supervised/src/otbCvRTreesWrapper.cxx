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

#include "otbCvRTreesWrapper.h"
#include <algorithm>
#include <functional>

namespace otb
{

CvRTreesWrapper::CvRTreesWrapper()
{
#ifdef OTB_OPENCV_3
  m_Impl = cv::ml::RTrees::create();
#endif
}

CvRTreesWrapper::~CvRTreesWrapper()
{
}

void CvRTreesWrapper::get_votes(const cv::Mat& sample, const cv::Mat& missing, CvRTreesWrapper::VotesVectorType& vote_count) const
{
#ifdef OTB_OPENCV_3
  // missing samples not implemented yet
  (void)missing;

  // Here we have to re-implement a basic "predict_tree()" since the function is
  // not exposed anymore
  const std::vector<cv::ml::DTrees::Node>&  nodes  = m_Impl->getNodes();
  const std::vector<cv::ml::DTrees::Split>& splits = m_Impl->getSplits();
  const std::vector<int>&                   roots  = m_Impl->getRoots();
  int                                       ntrees = roots.size();
  int                                       nodeIdx, prevNodeIdx;
  int                                       predictedClass = -1;
  const float*                              samplePtr      = sample.ptr<float>();
  std::map<int, unsigned int> votes;

  for (int t = 0; t < ntrees; t++)
  {
    nodeIdx     = roots[t];
    prevNodeIdx = nodeIdx;
    while (1)
    {
      prevNodeIdx                         = nodeIdx;
      const cv::ml::DTrees::Node& curNode = nodes[nodeIdx];
      // test if this node is a leaf
      if (curNode.split < 0)
        break;
      const cv::ml::DTrees::Split& split  = splits[curNode.split];
      int                          varIdx = split.varIdx;
      float                        val    = samplePtr[varIdx];
      nodeIdx                             = val <= split.c ? curNode.left : curNode.right;
    }
    predictedClass = nodes[prevNodeIdx].classIdx;
    votes[predictedClass] += 1;
  }

  vote_count.resize(votes.size());
  int pos = 0;
  for (std::map<int, unsigned int>::const_iterator it = votes.begin(); it != votes.end(); ++it)
  {
    vote_count[pos] = it->second;
    pos++;
  }

  if (vote_count.size() == 1)
  {
    // give at least 2 classes
    vote_count.push_back(0);
  }
#else
  vote_count.resize(nclasses);
  for (int k = 0; k < ntrees; k++)
  {
    CvDTreeNode* predicted_node = trees[k]->predict(sample, missing);
    int          class_idx      = predicted_node->class_idx;
    CV_Assert(0 <= class_idx && class_idx < nclasses);
    ++vote_count[class_idx];
  }
#endif
}

float CvRTreesWrapper::predict_margin(const cv::Mat& sample, const cv::Mat& missing) const
{
#ifdef OTB_OPENCV_3
  int ntrees = m_Impl->getRoots().size();
#endif
  // Sanity check (division by ntrees later on)
  if (ntrees == 0)
  {
    return 0.;
  }
  std::vector<unsigned int> classVotes;
  this->get_votes(sample, missing, classVotes);
  // We only sort the 2 greatest elements
  std::nth_element(classVotes.begin(), classVotes.begin() + 1, classVotes.end(), std::greater<unsigned int>());
  float margin = static_cast<float>(classVotes[0] - classVotes[1]) / ntrees;
  return margin;
}

float CvRTreesWrapper::predict_confidence(const cv::Mat& sample, const cv::Mat& missing) const
{
#ifdef OTB_OPENCV_3
  int ntrees = m_Impl->getRoots().size();
#endif
  // Sanity check (division by ntrees later on)
  if (ntrees == 0)
  {
    return 0.;
  }
  std::vector<unsigned int> classVotes;
  this->get_votes(sample, missing, classVotes);
  unsigned int max_votes  = *(std::max_element(classVotes.begin(), classVotes.end()));
  float        confidence = static_cast<float>(max_votes) / ntrees;
  return confidence;
}

#ifdef OTB_OPENCV_3
#define OTB_CV_WRAP_IMPL(type, name)        \
  type CvRTreesWrapper::get##name() const   \
  {                                         \
    return m_Impl->get##name();             \
  }                                         \
  void CvRTreesWrapper::set##name(type val) \
  {                                         \
    m_Impl->set##name(val);                 \
  }

#define OTB_CV_WRAP_IMPL_REF(type, name)           \
  type CvRTreesWrapper::get##name() const          \
  {                                                \
    return m_Impl->get##name();                    \
  }                                                \
  void CvRTreesWrapper::set##name(const type& val) \
  {                                                \
    m_Impl->set##name(val);                        \
  }

#define OTB_CV_WRAP_IMPL_CSTREF_GET(type, name)  \
  const type& CvRTreesWrapper::get##name() const \
  {                                              \
    return m_Impl->get##name();                  \
  }

// TODO : wrap all method used
OTB_CV_WRAP_IMPL(int, MaxCategories)
OTB_CV_WRAP_IMPL(int, MaxDepth)
OTB_CV_WRAP_IMPL(int, MinSampleCount)
OTB_CV_WRAP_IMPL(bool, UseSurrogates)
OTB_CV_WRAP_IMPL(int, CVFolds)
OTB_CV_WRAP_IMPL(bool, Use1SERule)
OTB_CV_WRAP_IMPL(bool, TruncatePrunedTree)
OTB_CV_WRAP_IMPL(float, RegressionAccuracy)
OTB_CV_WRAP_IMPL(bool, CalculateVarImportance)
OTB_CV_WRAP_IMPL(int, ActiveVarCount)
OTB_CV_WRAP_IMPL_REF(cv::Mat, Priors)
OTB_CV_WRAP_IMPL_REF(cv::TermCriteria, TermCriteria)

OTB_CV_WRAP_IMPL_CSTREF_GET(std::vector<int>, Roots)
OTB_CV_WRAP_IMPL_CSTREF_GET(std::vector<cv::ml::DTrees::Node>, Nodes)
OTB_CV_WRAP_IMPL_CSTREF_GET(std::vector<cv::ml::DTrees::Split>, Splits)
OTB_CV_WRAP_IMPL_CSTREF_GET(std::vector<int>, Subsets)

int CvRTreesWrapper::getVarCount() const
{
  return m_Impl->getVarCount();
}

bool CvRTreesWrapper::isTrained() const
{
  return m_Impl->isTrained();
}

bool CvRTreesWrapper::isClassifier() const
{
  return m_Impl->isClassifier();
}

cv::Mat CvRTreesWrapper::getVarImportance() const
{
  return m_Impl->getVarImportance();
}

#ifdef OTB_OPENCV_4
void CvRTreesWrapper::getVotes(cv::InputArray samples, cv::OutputArray results, int flags) const
{
  m_Impl->getVotes(samples, results, flags);
}
#endif

cv::String CvRTreesWrapper::getDefaultName() const
{
  return m_Impl->getDefaultName();
}


void CvRTreesWrapper::read(const cv::FileNode& fn)
{
  m_Impl->read(fn);
}

void CvRTreesWrapper::write(cv::FileStorage& fs) const
{
  m_Impl->write(fs);
}

void CvRTreesWrapper::save(const cv::String& filename) const
{
  m_Impl->save(filename);
}

bool CvRTreesWrapper::train(cv::InputArray samples, int layout, cv::InputArray responses)
{
  return m_Impl->train(samples, layout, responses);
}

bool CvRTreesWrapper::train(const cv::Ptr<cv::ml::TrainData>& trainData, int flags)
{
  return m_Impl->train(trainData, flags);
}

float CvRTreesWrapper::predict(cv::InputArray samples, cv::OutputArray results, int flags) const
{
  return m_Impl->predict(samples, results, flags);
}

cv::Ptr<CvRTreesWrapper> CvRTreesWrapper::create()
{
  return cv::makePtr<CvRTreesWrapper>();
}

#undef OTB_CV_WRAP_IMPL
#undef OTB_CV_WRAP_IMPL_REF
#undef OTB_CV_WRAP_IMPL_CSTREF_GET
#endif
}
