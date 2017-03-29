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

#include "otbCvRTreesWrapper.h"
#include <algorithm>
#include <functional>

namespace otb
{

CvRTreesWrapper::CvRTreesWrapper(){}

CvRTreesWrapper::~CvRTreesWrapper(){}

void CvRTreesWrapper::get_votes(const cv::Mat& sample, 
                                const cv::Mat& missing,
                                CvRTreesWrapper::VotesVectorType& vote_count) const
{
  vote_count.resize(nclasses);
  for( int k = 0; k < ntrees; k++ )
    {
    CvDTreeNode* predicted_node = trees[k]->predict( sample, missing );
    int class_idx = predicted_node->class_idx;
    CV_Assert( 0 <= class_idx && class_idx < nclasses );
    ++vote_count[class_idx];
    }
}

float CvRTreesWrapper::predict_margin(const cv::Mat& sample, 
                                      const cv::Mat& missing) const
{
  // Sanity check (division by ntrees later on)
  if(ntrees == 0)
    {
    return 0.;
    }
  std::vector<unsigned int> classVotes;
  this->get_votes(sample, missing, classVotes);
// We only sort the 2 greatest elements
  std::nth_element(classVotes.begin(), classVotes.begin()+1, 
                   classVotes.end(), std::greater<unsigned int>());
  float margin = static_cast<float>(classVotes[0]-classVotes[1])/ntrees;
  return margin;
}

float CvRTreesWrapper::predict_confidence(const cv::Mat& sample, 
                                  const cv::Mat& missing) const
{
  // Sanity check (division by ntrees later on)
  if(ntrees == 0)
    {
    return 0.;
    }
  std::vector<unsigned int> classVotes;
  this->get_votes(sample, missing, classVotes);
  unsigned int max_votes = *(std::max_element(classVotes.begin(), 
                                              classVotes.end()));
  float confidence = static_cast<float>(max_votes)/ntrees;
  return confidence;
}

}
