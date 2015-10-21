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
  struct ClassVotes
  {
    unsigned int votes;
    unsigned int class_idx;
  };

  struct MoreVotes
  {
    bool operator()(ClassVotes a, ClassVotes b)
    {
      return (a.votes > b.votes);
    }
  };

  typedef std::vector<ClassVotes> ClassVotesVectorType;

public:
  CV_WRAP CvRTreesWrapper(){};
  virtual ~CvRTreesWrapper(){};
  
  const int get_nclasses() const
  {
    return nclasses;
  };

  //  CV_WRAP virtual float predict(const cv::Mat& sample, const cv::Mat& missing = cv::Mat() ) const{return CvRTrees::predict(sample,missing);};
  
  virtual const float predict_confidence(const cv::Mat& sample, const cv::Mat& missing = cv::Mat()) const
  {
    cv::AutoBuffer<int> _votes(nclasses);
    ClassVotesVectorType classVotes(nclasses);
    for( int k = 0; k < ntrees; k++ )
      {
      CvDTreeNode* predicted_node = trees[k]->predict( sample, missing );
      int class_idx = predicted_node->class_idx;
      CV_Assert( 0 <= class_idx && class_idx < nclasses );
      classVotes[class_idx].votes += 1;
      classVotes[class_idx].class_idx = class_idx;
      }
    std::nth_element(classVotes.begin(), classVotes.begin()+1, 
                     classVotes.end(), MoreVotes());
    unsigned int maxVotes = classVotes[0].votes;
    unsigned int secondVotes = classVotes[2].votes;
    return static_cast<float>(maxVotes-secondVotes)/ntrees;
  };

};



#endif
