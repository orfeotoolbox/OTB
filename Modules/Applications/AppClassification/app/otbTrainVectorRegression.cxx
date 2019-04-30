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

class TrainVectorRegression : public TrainVectorBase<float, float>
{
public:
  typedef TrainVectorRegression Self;
  typedef TrainVectorBase<float, float> Superclass;
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

    SetDocName( "Train Vector Regression" );
    SetDocLongDescription( "This application trains a regression algorithm based on "
      "a predictor geometries and a list of features to consider for "
      "regression.\nThis application is based on LibSVM, OpenCV Machine "
      "Learning (2.3.1 and later), and Shark ML The output of this application "
      "is a text model file, whose format corresponds to the ML model type "
      "chosen. There is no image nor vector data output.");
    SetDocLimitations("");
    SetDocAuthors( "OTB Team" );
    SetDocSeeAlso( " " );

    SetOfficialDocLink();

    Superclass::DoInit();
    
    AddParameter( ParameterType_Float , "io.mse" , "Mean Square Error" );
    SetParameterDescription( "io.mse" ,
      "Mean square error computed with the validation predictors" );
    SetParameterRole( "io.mse" , Role_Output );
    this->MandatoryOff( "io.mse" );

  }

  void DoUpdateParameters() override
  {
    Superclass::DoUpdateParameters();
  }
  
  double ComputeMSE(TargetListSampleType* list1, TargetListSampleType* list2)
  {
    assert(list1->Size() == list2->Size());
    double mse = 0.;
    for (TargetListSampleType::InstanceIdentifier i=0; i<list1->Size() ; ++i)
    {
      auto elem1 = list1->GetMeasurementVector(i);
      auto elem2 = list2->GetMeasurementVector(i);
      
      mse += (elem1[0] - elem2[0]) * (elem1[0] - elem2[0]);
    }
    mse /= static_cast<double>(list1->Size());
    return mse;
  }
  
  
  void DoExecute() override
  {
    m_FeaturesInfo.SetClassFieldNames( GetChoiceNames( "cfield" ), GetSelectedItems( "cfield" ) );

    if( m_FeaturesInfo.m_SelectedCFieldIdx.empty() && GetClassifierCategory() == Supervised )
      {
      otbAppLogFATAL( << "No field has been selected for data labelling!" );
      }

    Superclass::DoExecute();
    
    otbAppLogINFO("Computing training performances");
    
    auto mse = ComputeMSE(m_ClassificationSamplesWithLabel.labeledListSample.GetPointer(), m_PredictedList.GetPointer() );

    otbAppLogINFO("Mean Square Error = "<<mse);
    this->SetParameterFloat("io.mse",mse);
  }
  
private:

};
}
}

OTB_APPLICATION_EXPORT( otb::Wrapper::TrainVectorRegression )
