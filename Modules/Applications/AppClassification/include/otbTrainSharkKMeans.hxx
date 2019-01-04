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
#ifndef otbTrainSharkKMeans_hxx
#define otbTrainSharkKMeans_hxx

#include "otbLearningApplicationBase.h"
#include "otbSharkKMeansMachineLearningModel.h"

namespace otb
{
namespace Wrapper
{
template<class TInputValue, class TOutputValue>
void LearningApplicationBase<TInputValue, TOutputValue>::InitSharkKMeansParams()
{
  AddChoice( "classifier.sharkkm", "Shark kmeans classifier" );
  SetParameterDescription("classifier.sharkkm", "http://image.diku.dk/shark/sphinx_pages/build/html/rest_sources/tutorials/algorithms/kmeans.html ");
  //MaxNumberOfIterations
  AddParameter( ParameterType_Int, "classifier.sharkkm.maxiter",
                "Maximum number of iteration for the kmeans algorithm." );
  SetParameterInt( "classifier.sharkkm.maxiter", 10 );
  SetMinimumParameterIntValue( "classifier.sharkkm.maxiter", 0 );
  SetParameterDescription( "classifier.sharkkm.maxiter",
                           "The maximum number of iteration for the kmeans algorithm. 0=unlimited" );

  //MaxNumberOfIterations
  AddParameter( ParameterType_Int, "classifier.sharkkm.k", "The number of class used for the kmeans algorithm." );
  SetParameterInt( "classifier.sharkkm.k", 2 );
  SetParameterDescription( "classifier.sharkkm.k",
                           "The number of class used for the kmeans algorithm. Default set to 2 class" );
  SetMinimumParameterIntValue( "classifier.sharkkm.k", 2 );
}

template<class TInputValue, class TOutputValue>
void LearningApplicationBase<TInputValue, TOutputValue>::TrainSharkKMeans(
        typename ListSampleType::Pointer trainingListSample,
        typename TargetListSampleType::Pointer trainingLabeledListSample, std::string modelPath)
{
  unsigned int nbMaxIter = static_cast<unsigned int>(abs( GetParameterInt( "classifier.sharkkm.maxiter" ) ));
  unsigned int k = static_cast<unsigned int>(abs( GetParameterInt( "classifier.sharkkm.k" ) ));

  typedef otb::SharkKMeansMachineLearningModel<InputValueType, OutputValueType> SharkKMeansType;
  typename SharkKMeansType::Pointer classifier = SharkKMeansType::New();
  classifier->SetRegressionMode( this->m_RegressionFlag );
  classifier->SetInputListSample( trainingListSample );
  classifier->SetTargetListSample( trainingLabeledListSample );
  classifier->SetK( k );
  classifier->SetMaximumNumberOfIterations( nbMaxIter );
  classifier->Train();
  classifier->Save( modelPath );
}

} //end namespace wrapper
} //end namespace otb

#endif
