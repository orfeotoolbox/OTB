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

#ifndef otbOpenCVUtils_h
#define otbOpenCVUtils_h

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
#include <opencv2/core/core.hpp>
#include <opencv2/core/core_c.h>
#pragma GCC diagnostic pop
#else
#include <opencv2/core/core.hpp>
#include <opencv2/core/core_c.h>
#endif

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#include <opencv2/ml/ml.hpp>
#pragma GCC diagnostic pop
#else
#include <opencv2/ml/ml.hpp>
#endif

#include "otb_opencv_api.h"

#include "OTBSupervisedExport.h"

#include "itkListSample.h"

#ifdef OTB_OPENCV_3
#define CV_TYPE_NAME_ML_SVM         "opencv-ml-svm"
#define CV_TYPE_NAME_ML_RTREES      "opencv-ml-random-trees"
#define CV_TYPE_NAME_ML_BOOSTING    "opencv-ml-boost-tree"
#define CV_TYPE_NAME_ML_ANN_MLP     "opencv-ml-ann-mlp"
#define CV_TYPE_NAME_ML_NBAYES      "opencv-ml-bayesian"
#define CV_TYPE_NAME_ML_TREE        "opencv-ml-tree"

#define CvSVM cv::ml::SVM
#define CvANN_MLP_TrainParams cv::ml::ANN_MLP
#define CvANN_MLP cv::ml::ANN_MLP
#define CvBoost cv::ml::Boost

#define CV_VAR_NUMERICAL cv::ml::VAR_NUMERICAL
#define CV_VAR_CATEGORICAL cv::ml::VAR_CATEGORICAL
#endif

namespace otb
{
  template <class T> void SampleToMat(const T & sample, cv::Mat& output)
  {
    output.create(1,sample.Size(),CV_32FC1);

    // Loop on sample size
    for(unsigned int i = 0; i < sample.Size(); ++i)
      {
      output.at<float>(0,i) = sample[i];
      }
  }


  /** Converts a ListSample of VariableLengthVector to a CvMat. The user
   *  is responsible for freeing the output pointer with the
   *  cvReleaseMat function.  A null pointer is resturned in case the
   *  conversion failed.
   */
  template <class T> void ListSampleToMat(const T * listSample, cv::Mat & output) {
    // Sample index
    unsigned int sampleIdx = 0;

    // Check for valid listSample
    if(listSample != nullptr && listSample->Size() > 0)
      {
       // Retrieve samples count
       unsigned int sampleCount = listSample->Size();

       // Build an iterator
       typename T::ConstIterator sampleIt = listSample->Begin();

       // Retrieve samples size alike
       const unsigned int sampleSize = listSample->GetMeasurementVectorSize();

       // Allocate CvMat
       output.create(sampleCount,sampleSize,CV_32FC1);

       // Fill the cv matrix
       for(; sampleIt!=listSample->End(); ++sampleIt,++sampleIdx)
         {
           // Retrieve sample
           typename T::MeasurementVectorType sample = sampleIt.GetMeasurementVector();

           // Loop on sample size
           for(unsigned int i = 0; i < sampleSize; ++i)
             {
              output.at<float>(sampleIdx,i) = sample[i];
             }
         }
      }
  }

  template <typename T> void ListSampleToMat(typename T::Pointer listSample, cv::Mat & output) {
    return ListSampleToMat(listSample.GetPointer(), output);
  }

  template <typename T> void ListSampleToMat(typename T::ConstPointer listSample, cv::Mat & output ) {
    return ListSampleToMat(listSample.GetPointer(), output);
  }

  template <typename T> typename T::Pointer MatToListSample(const cv::Mat & cvmat)
    {
      // Build output type
      typename T::Pointer output = T::New();

      // Get samples count
      unsigned sampleCount = cvmat.rows;

      // Get samples size
      unsigned int sampleSize = cvmat.cols;

      // Loop on samples
      for(unsigned int i = 0; i < sampleCount; ++i)
       {
         typename T::MeasurementVectorType sample;
         itk::NumericTraits<typename T::MeasurementVectorType>::SetLength(sample, sampleSize);

         unsigned int realSampleSize = sample.Size();

         for(unsigned int j = 0; j < realSampleSize; ++j)
           {
             // Don't forget to cast
             sample[j] = static_cast<typename T::MeasurementVectorType
              ::ValueType>(cvmat.at<float>(i,j));
           }
         // PushBack the new sample
         output->PushBack(sample);
       }
      // return the output
      return output;
    }

}

#endif
