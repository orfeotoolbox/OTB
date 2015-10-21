/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbCvRTrees_h
#define __otbCvRTrees_h

#include "otbOpenCVUtils.h"
#include <vector>

class CV_EXPORTS_W CvRTreesWrapper : public CvRTrees
{
public:
  CvRTreesWrapper(){};
  virtual ~CvRTreesWrapper(){};
  
  float predict_confidence(const cv::Mat& sample, 
                           const cv::Mat& missing = 
                           cv::Mat()) const
  {
    std::vector<unsigned int> classVotes(nclasses);
    for( int k = 0; k < ntrees; k++ )
      {
      CvDTreeNode* predicted_node = trees[k]->predict( sample, missing );
      int class_idx = predicted_node->class_idx;
      CV_Assert( 0 <= class_idx && class_idx < nclasses );
      ++classVotes[class_idx];
      }
    std::nth_element(classVotes.begin(), classVotes.begin()+1, 
                     classVotes.end(), std::greater<>());
    unsigned int maxVotes = classVotes[0];
    unsigned int secondVotes = classVotes[1];
    float confidence = static_cast<float>(maxVotes-secondVotes)/ntrees;
    return confidence;
  };

};



#endif
