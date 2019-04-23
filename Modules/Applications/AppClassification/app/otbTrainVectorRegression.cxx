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

#include "otbTrainVectorBase.h"

namespace otb
{
namespace Wrapper
{

class TrainVectorRegression : public TrainVectorBase
{
public:
  typedef TrainVectorRegression Self;
  typedef TrainVectorBase Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  
  itkNewMacro( Self )
  itkTypeMacro( Self, Superclass )

  typedef Superclass::SampleType SampleType;
  typedef Superclass::ListSampleType ListSampleType;
  typedef Superclass::TargetListSampleType TargetListSampleType;

protected:
  TrainVectorRegression()
  {
    this->m_RegressionFlag = true;
  }
  
  void DoInit() override
  {
    SetName( "TrainVectorRegression" );
    SetDescription( "Train a regression algorithm based on geometries with "
      "list of features to consider and a predictor." );

    SetDocName( "Train Vector Classifier" );
    SetDocLongDescription( "This application trains a classifier based on "
      "labeled geometries and a list of features to consider for "
      "classification.\nThis application is based on LibSVM, OpenCV Machine "
      "Learning (2.3.1 and later), and Shark ML The output of this application "
      "is a text model file, whose format corresponds to the ML model type "
      "chosen. There is no image nor vector data output.");
    SetDocLimitations("");
    SetDocAuthors( "OTB Team" );
    SetDocSeeAlso( " " );

    SetOfficialDocLink();

    Superclass::DoInit();
  }

  void DoUpdateParameters() override
  {
    Superclass::DoUpdateParameters();
  }
  
  void DoExecute() override
  {

    Superclass::DoExecute();
  }
  
private:

};
}
}

OTB_APPLICATION_EXPORT( otb::Wrapper::TrainVectorRegression )
