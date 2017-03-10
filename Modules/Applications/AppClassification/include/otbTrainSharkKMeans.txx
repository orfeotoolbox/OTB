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
#ifndef otbTrainSharkKMeans_txx
#define otbTrainSharkKMeans_txx

#include "otbLearningApplicationBase.h"

namespace otb
{
namespace Wrapper
{
template<class TInputValue, class TOutputValue>
void LearningApplicationBase<TInputValue, TOutputValue>::InitSharkKMeansParams()
{
  AddChoice( "classifier.sharkkm", "Shark kmeans classifier" );
  SetParameterDescription( "classifier.sharkkm",
                           "This group of parameters allows setting Shark kMeans classifier parameters. "
                                   "See complete documentation here "
                                   "\\url{http://image.diku.dk/shark/sphinx_pages/build/html/rest_sources/tutorials/algorithms/kmeans.html}.\n " );
  //MaxNumberOfIterations
  AddParameter( ParameterType_Int, "classifier.sharkkm.nbmaxiter",
                "Maximum number of iteration for the kmeans algorithm." );
  SetParameterInt( "classifier.sharkkm.nbmaxiter", 10 );
  SetMinimumParameterIntValue( "classifier.sharkkm.nbmaxiter", 0 );
  SetParameterDescription( "classifier.sharkkm.nbmaxiter",
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
  unsigned int nbMaxIter = static_cast<unsigned int>(abs( GetParameterInt( "classifier.sharkkm.nbmaxiter" ) ));
  unsigned int k = static_cast<unsigned int>(abs( GetParameterInt( "classifier.sharkkm.k" ) ));

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
