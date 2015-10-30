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
#ifndef __otbCvRTreesWrapper_h
#define __otbCvRTreesWrapper_h

#include "otbOpenCVUtils.h"

namespace otb
{

/** \class CvRTreesWrapper
 * \brief Wrapper for OpenCV Random Trees
 *
 * \ingroup OTBSupervised
 */
class CV_EXPORTS_W CvRTreesWrapper : public CvRTrees
{
public:
  CvRTreesWrapper();
  virtual ~CvRTreesWrapper();
  
  /** Predict the confidence of the classifcation by computing the 
      difference in votes between the first and second most voted classes.
      This measure is preferred to the proportion of votes of the majority
      class, since it provides information about the conflict between the
      most likely classes.
  */
  float predict_confidence(const cv::Mat& sample, 
                           const cv::Mat& missing = 
                           cv::Mat()) const;
};

}

#endif
