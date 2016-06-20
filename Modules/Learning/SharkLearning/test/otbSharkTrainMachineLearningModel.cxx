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

#include <fstream>
#include <string>
#include <algorithm>

#include <otbMachineLearningModel.h>
#include "otbConfusionMatrixCalculator.h"

#include "otb_boost_string_header.h"

typedef otb::MachineLearningModel<float,short>         MachineLearningModelType;
typedef MachineLearningModelType::InputValueType       InputValueType;
typedef MachineLearningModelType::InputSampleType      InputSampleType;
typedef MachineLearningModelType::InputListSampleType  InputListSampleType;
typedef MachineLearningModelType::TargetValueType      TargetValueType;
typedef MachineLearningModelType::TargetSampleType     TargetSampleType;
typedef MachineLearningModelType::TargetListSampleType TargetListSampleType;

typedef otb::MachineLearningModel<float,float>                   MachineLearningModelRegressionType;
typedef MachineLearningModelRegressionType::InputValueType       InputValueRegressionType;
typedef MachineLearningModelRegressionType::InputSampleType      InputSampleRegressionType;
typedef MachineLearningModelRegressionType::InputListSampleType  InputListSampleRegressionType;
typedef MachineLearningModelRegressionType::TargetValueType      TargetValueRegressionType;
typedef MachineLearningModelRegressionType::TargetSampleType     TargetSampleRegressionType;
typedef MachineLearningModelRegressionType::TargetListSampleType TargetListSampleRegressionType;

typedef otb::ConfusionMatrixCalculator<TargetListSampleType, TargetListSampleType> ConfusionMatrixCalculatorType;

bool SharkReadDataFile(const std::string & infname, InputListSampleType * samples, TargetListSampleType * labels)
{
  std::ifstream ifs;
  ifs.open(infname.c_str());

  if(!ifs)
    {
    std::cout<<"Could not read file "<<infname<<std::endl;
    return false;
    }

  unsigned int nbfeatures = 0;

  while (!ifs.eof())
    {
    std::string line;
    std::getline(ifs, line);
    boost::algorithm::trim(line);

    if(nbfeatures == 0)
      {
      nbfeatures = std::count(line.begin(),line.end(),' ');
      }

    if(line.size()>1)
      {
      InputSampleType sample(nbfeatures);
      sample.Fill(0);

      std::string::size_type pos = line.find_first_of(" ", 0);

      // Parse label
      TargetSampleType label;
      label[0] = atoi(line.substr(0, pos).c_str());

      bool endOfLine = false;
      unsigned int id = 0;

      while(!endOfLine)
        {
        std::string::size_type nextpos = line.find_first_of(" ", pos+1);

        if(pos == std::string::npos)
          {
          endOfLine = true;
          nextpos = line.size()-1;
          }
        else
          {
          std::string feature = line.substr(pos,nextpos-pos);
          std::string::size_type semicolonpos = feature.find_first_of(":");
          id = atoi(feature.substr(0,semicolonpos).c_str());
          sample[id - 1] = atof(feature.substr(semicolonpos+1,feature.size()-semicolonpos).c_str());
          pos = nextpos;
          }

        }
      samples->SetMeasurementVectorSize(itk::NumericTraits<InputSampleType>::GetLength(sample));
      samples->PushBack(sample);
      labels->PushBack(label);
      }
    }

  //std::cout<<"Retrieved "<<samples->Size()<<" samples"<<std::endl;
  ifs.close();
  return true;
}

bool SharkReadDataRegressionFile(const std::string & infname, InputListSampleRegressionType * samples, TargetListSampleRegressionType * labels)
{
  std::ifstream ifs;
  ifs.open(infname.c_str());

  if(!ifs)
    {
    std::cout<<"Could not read file "<<infname<<std::endl;
    return false;
    }

  unsigned int nbfeatures = 0;

  while (!ifs.eof())
    {
    std::string line;
    std::getline(ifs, line);

    if(nbfeatures == 0)
      {
      nbfeatures = std::count(line.begin(),line.end(),' ')-1;
      //std::cout<<"Found "<<nbfeatures<<" features per samples"<<std::endl;
      }

    if(line.size()>1)
      {
      InputSampleRegressionType sample(nbfeatures);
      sample.Fill(0);

      std::string::size_type pos = line.find_first_of(" ", 0);

      // Parse label
      TargetSampleRegressionType label;
      label[0] = atof(line.substr(0, pos).c_str());

      bool endOfLine = false;
      unsigned int id = 0;

      while(!endOfLine)
        {
        std::string::size_type nextpos = line.find_first_of(" ", pos+1);

        if(nextpos == std::string::npos)
          {
          endOfLine = true;
          nextpos = line.size()-1;
          }
        else
          {
          std::string feature = line.substr(pos,nextpos-pos);
          std::string::size_type semicolonpos = feature.find_first_of(":");
          id = atoi(feature.substr(0,semicolonpos).c_str());
          sample[id - 1] = atof(feature.substr(semicolonpos+1,feature.size()-semicolonpos).c_str());
          pos = nextpos;
          }

        }
      samples->SetMeasurementVectorSize(itk::NumericTraits<InputSampleRegressionType>::GetLength(sample));
      samples->PushBack(sample);
      labels->PushBack(label);
      }
    }

  //std::cout<<"Retrieved "<<samples->Size()<<" samples"<<std::endl;
  ifs.close();
  return true;
}


#ifdef OTB_USE_SHARK
#include "otbSharkRandomForestsMachineLearningModel.h"
int otbSharkRFMachineLearningModelNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::SharkRandomForestsMachineLearningModel<InputValueType,TargetValueType> SharkRFType;
  SharkRFType::Pointer classifier = SharkRFType::New();
  return EXIT_SUCCESS;
}

int otbSharkRFMachineLearningModel(int argc, char * argv[])
{
  if (argc != 3 )
    {
    std::cout<<"Wrong number of arguments "<<std::endl;
    std::cout<<"Usage : sample file, output file "<<std::endl;
    return EXIT_FAILURE;
    }

  typedef otb::SharkRandomForestsMachineLearningModel<InputValueType,TargetValueType> RandomForestType;
  InputListSampleType::Pointer samples = InputListSampleType::New();
  TargetListSampleType::Pointer labels = TargetListSampleType::New();
  TargetListSampleType::Pointer predicted = TargetListSampleType::New();

  if(!SharkReadDataFile(argv[1],samples,labels))
    {
    std::cout<<"Failed to read samples file "<<argv[1]<<std::endl;
    return EXIT_FAILURE;
    }


  RandomForestType::Pointer classifier = RandomForestType::New();
  classifier->SetInputListSample(samples);
  classifier->SetTargetListSample(labels);
  classifier->SetRegressionMode(false);
  classifier->SetNumberOfTrees(100);
  classifier->SetMTry(0);
  classifier->SetNodeSize(25);
  classifier->SetOobRatio(0.3);
  std::cout << "Train\n";
  classifier->Train();
  std::cout << "Save\n";
  classifier->Save(argv[2]); 
  
  std::cout << "Predict\n";
  classifier->SetTargetListSample(predicted);
  classifier->PredictAll();

  ConfusionMatrixCalculatorType::Pointer cmCalculator = ConfusionMatrixCalculatorType::New();

  cmCalculator->SetProducedLabels(predicted);
  cmCalculator->SetReferenceLabels(labels);
  cmCalculator->Compute();

  std::cout<<"Confusion matrix: "<<std::endl;
  std::cout<<cmCalculator->GetConfusionMatrix()<<std::endl;
  const float kappaIdx = cmCalculator->GetKappaIndex();
  std::cout<<"Kappa: "<<kappaIdx<<std::endl;
  std::cout<<"Overall Accuracy: "<<cmCalculator->GetOverallAccuracy()<<std::endl;
  
  //Predict single samples
  std::cout << "Predict single samples\n";
  double oa{0};
  auto sIt = samples->Begin();
  auto lIt = labels->Begin();
  for(; sIt != samples->End(); ++sIt, ++lIt)
    {
    auto p = classifier->Predict(sIt.GetMeasurementVector())[0];
    oa += (p==(lIt.GetMeasurementVector())[0])?1.0/samples->Size():0;
    }
  std::cout << "Single sample OA = " << oa << '\n';
//Load Model to new RF
  TargetListSampleType::Pointer predictedLoad = TargetListSampleType::New();
  RandomForestType::Pointer classifierLoad = RandomForestType::New();

  std::cout << "Load\n";
  classifierLoad->Load(argv[2]);
  classifierLoad->SetInputListSample(samples);
  classifierLoad->SetTargetListSample(predictedLoad);
  std::cout << "Predict loaded\n";
  classifierLoad->PredictAll();

  ConfusionMatrixCalculatorType::Pointer cmCalculatorLoad = ConfusionMatrixCalculatorType::New();

  cmCalculatorLoad->SetProducedLabels(predictedLoad);
  cmCalculatorLoad->SetReferenceLabels(labels);
  cmCalculatorLoad->Compute();

  std::cout<<"Confusion matrix: "<<std::endl;
  std::cout<<cmCalculatorLoad->GetConfusionMatrix()<<std::endl;
  const float kappaIdxLoad = cmCalculatorLoad->GetKappaIndex();
  std::cout<<"Kappa: "<<kappaIdxLoad<<std::endl;
  std::cout<<"Overall Accuracy: "<<cmCalculatorLoad->GetOverallAccuracy()<<std::endl;


  if ( vcl_abs(kappaIdxLoad - kappaIdx) < 0.00000001)
    {
    return EXIT_SUCCESS;
    }
  else
    {
    return EXIT_FAILURE;
    }
  
    return EXIT_SUCCESS;
}
#endif
