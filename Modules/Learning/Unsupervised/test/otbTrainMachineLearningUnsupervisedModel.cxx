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
#include <iostream>

#include <otbConfigure.h>
#include <otbMachineLearningModel.h>

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


#ifdef OTB_USE_SHARK
#include "otbSharkKMeansMachineLearningModel.h"
#include "otb_boost_string_header.h"
#include <chrono>

bool SharkReadDataFile(const std::string & infname, InputListSampleType * samples, TargetListSampleType * labels)
{
  std::ifstream ifs(infname);

  if(!ifs)
    {
    std::cout<<"Could not read file "<<infname<<std::endl;
    return false;
    }

  unsigned int nbfeatures = 0;

  std::string line;
  while (std::getline(ifs, line))
    {
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
      label[0] = std::stoi(line.substr(0, pos).c_str());

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
          id = std::stoi(feature.substr(0,semicolonpos).c_str());
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



int otbSharkKMeansMachineLearningModelTrain(int argc, char *argv[])
{
  if( argc != 3 )
    {
    std::cout << "Wrong number of arguments " << std::endl;
    std::cout << "Usage : sample file, output file " << std::endl;
    return EXIT_FAILURE;
    }

  typedef otb::SharkKMeansMachineLearningModel<InputValueType, TargetValueType> KMeansType;
  InputListSampleType::Pointer samples = InputListSampleType::New();
  TargetListSampleType::Pointer labels = TargetListSampleType::New();

  if( !SharkReadDataFile( argv[1], samples, labels ) )
    {
    std::cout << "Failed to read samples file " << argv[1] << std::endl;
    return EXIT_FAILURE;
    }

  KMeansType::Pointer classifier = KMeansType::New();
  classifier->SetInputListSample( samples );
  classifier->SetTargetListSample( labels );
  classifier->SetRegressionMode( false );
  classifier->SetK( 3 );
  classifier->SetMaximumNumberOfIterations( 0 );
  std::cout << "Train\n";
  classifier->Train();
  std::cout << "Save\n";
  classifier->Save( argv[2] );

  return EXIT_SUCCESS;
}


int otbSharkKMeansMachineLearningModelPredict(int argc, char *argv[])
{
  if( argc != 3 )
    {
    std::cout << "Wrong number of arguments " << std::endl;
    std::cout << "Usage : sample file, input model file " << std::endl;
    return EXIT_FAILURE;
    }

  typedef otb::SharkKMeansMachineLearningModel<InputValueType, TargetValueType> KMeansType;
  InputListSampleType::Pointer samples = InputListSampleType::New();
  TargetListSampleType::Pointer labels = TargetListSampleType::New();

  if( !SharkReadDataFile( argv[1], samples, labels ) )
    {
    std::cout << "Failed to read samples file " << argv[1] << std::endl;
    return EXIT_FAILURE;
    }

  KMeansType::Pointer classifier = KMeansType::New();
  std::cout << "Load\n";
  if(!classifier->CanReadFile(argv[2]))
    {
    std::cerr << "Unable to read model file : " << argv[2] << std::endl;
    return EXIT_FAILURE;
    }
  classifier->Load( argv[2] );
  auto start = std::chrono::system_clock::now();
  classifier->SetInputListSample( samples );
  classifier->SetTargetListSample( labels );
  std::cout << "Predict loaded\n";
  classifier->PredictBatch( samples, NULL );
  using TimeT = std::chrono::milliseconds;
  auto duration = std::chrono::duration_cast<TimeT>( std::chrono::system_clock::now() - start );
  auto elapsed = duration.count();
  std::cout << "PredictAll took " << elapsed << " ms\n";

  return EXIT_SUCCESS;
}
#endif
