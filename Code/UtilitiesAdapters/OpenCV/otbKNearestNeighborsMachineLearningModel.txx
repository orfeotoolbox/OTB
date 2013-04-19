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
#ifndef __otbKNearestNeighborsMachineLearningModel_txx
#define __otbKNearestNeighborsMachineLearningModel_txx

#include <boost/lexical_cast.hpp>
#include "otbKNearestNeighborsMachineLearningModel.h"
#include "otbOpenCVUtils.h"

#include <fstream>
#include "itkMacro.h"

namespace otb
{

template <class TInputValue, class TTargetValue>
KNearestNeighborsMachineLearningModel<TInputValue,TTargetValue>
::KNearestNeighborsMachineLearningModel() :
 m_K(10), m_IsRegression(false)
{
  m_KNearestModel = new CvKNearest;
}


template <class TInputValue, class TTargetValue>
KNearestNeighborsMachineLearningModel<TInputValue,TTargetValue>
::~KNearestNeighborsMachineLearningModel()
{
  delete m_KNearestModel;
}

/** Train the machine learning model */
template <class TInputValue, class TTargetValue>
void
KNearestNeighborsMachineLearningModel<TInputValue,TTargetValue>
::Train()
{
  //convert listsample to opencv matrix
  cv::Mat samples;
  otb::ListSampleToMat<InputListSampleType>(this->GetInputListSample(), samples);

  cv::Mat labels;
  otb::ListSampleToMat<TargetListSampleType>(this->GetTargetListSample(),labels);

  //train the KNN model
  m_KNearestModel->train(samples,labels,cv::Mat(),m_IsRegression, m_K,false);
}

template <class TInputValue, class TTargetValue>
typename KNearestNeighborsMachineLearningModel<TInputValue,TTargetValue>
::TargetSampleType
KNearestNeighborsMachineLearningModel<TInputValue,TTargetValue>
::Predict(const InputSampleType & input) const
{
  //convert listsample to Mat
  cv::Mat sample;
  otb::SampleToMat<InputSampleType>(input,sample);

  double result = m_KNearestModel->find_nearest(sample,m_K);

  TargetSampleType target;

  target[0] = static_cast<TTargetValue>(result);

  return target;
}

template <class TInputValue, class TTargetValue>
void
KNearestNeighborsMachineLearningModel<TInputValue,TTargetValue>
::Save(const std::string & filename, const std::string & name)
{
  //there is no m_KNearestModel->save(filename.c_str(), name.c_str()).
  //We need to save the K parameter and IsRegression flag used and the samples.

  std::ofstream ofs(filename.c_str());
  //Save K parameter and IsRegression flag.
  ofs << "K="<< m_K <<"\n";
  ofs << "IsRegression="<<m_IsRegression <<"\n";

  //Save the samples. First column is the Label and other columns are the sample data.
  typename InputListSampleType::ConstIterator sampleIt = this->GetInputListSample()->Begin();
  typename TargetListSampleType::ConstIterator labelIt = this->GetTargetListSample()->Begin();
  const unsigned int sampleSize = this->GetInputListSample()->GetMeasurementVectorSize();
  for(; sampleIt!=this->GetInputListSample()->End(); ++sampleIt,++labelIt)
  {
    // Retrieve sample
    typename InputListSampleType::MeasurementVectorType sample = sampleIt.GetMeasurementVector();
    ofs <<labelIt.GetMeasurementVector()[0];

    // Loop on sample size
    for(unsigned int i = 0; i < sampleSize; ++i)
    {
      ofs << " " << sample[i];
    }
    ofs <<"\n";
  }
  ofs.close();
}

template <class TInputValue, class TTargetValue>
void
KNearestNeighborsMachineLearningModel<TInputValue,TTargetValue>
::Load(const std::string & filename, const std::string & name)
{
  //there is no m_KNearestModel->load(filename.c_str(), name.c_str());
  std::ifstream ifs(filename.c_str());
  if(!ifs)
  {
    itkExceptionMacro(<<"Could not read file "<<filename);
  }
  //first line is the K parameter of this algorithm.
  std::string line;
  std::getline(ifs, line);
  std::string::size_type pos = line.find_first_of("=", 0);
  std::string::size_type nextpos = line.find_first_of(" \n\r", pos+1);
  this->SetK(boost::lexical_cast<int>(line.substr(pos+1, nextpos-pos-1)));
  //second line is the IsRegression parameter
  std::getline(ifs, line);
  pos = line.find_first_of("=", 0);
  nextpos = line.find_first_of(" \n\r", pos+1);
  this->SetIsRegression(boost::lexical_cast<bool>(line.substr(pos+1, nextpos-pos-1)));

  //Clear previous listSample (if any)
  typename InputListSampleType::Pointer samples = InputListSampleType::New();
  typename TargetListSampleType::Pointer labels = TargetListSampleType::New();

  //Read a txt file. First column is the label, other columns are the sample data.
  unsigned int nbFeatures = 0;
  while (!ifs.eof())
  {
    std::getline(ifs, line);

    if(nbFeatures == 0)
      {
        nbFeatures = std::count(line.begin(),line.end(),' ');
      }

    if(line.size()>1)
    {
      // Parse label
      pos = line.find_first_of(" ", 0);
      TargetSampleType label;
      label[0] = static_cast<TargetValueType>(boost::lexical_cast<unsigned int>(line.substr(0, pos)));
      // Parse sample features
      InputSampleType sample(nbFeatures);
      sample.Fill(0);
      unsigned int id = 0;
      nextpos = line.find_first_of(" ", pos+1);
      while(nextpos != std::string::npos)
      {
        nextpos = line.find_first_of(" \n\r", pos+1);
        std::string subline = line.substr(pos+1, nextpos-pos-1);
        //sample[id] = static_cast<InputValueType>(boost::lexical_cast<float>(subline));
        sample[id] = atof(subline.c_str());
        pos = nextpos;
        id++;
      }
      samples->PushBack(sample);
      labels->PushBack(label);
    }
  }
  ifs.close();

  this->SetInputListSample(samples);
  this->SetTargetListSample(labels);
  Train();
}

template <class TInputValue, class TTargetValue>
bool
KNearestNeighborsMachineLearningModel<TInputValue,TTargetValue>
::CanReadFile(const std::string & file)
{
  try
  {
    this->Load(file);
  }
  catch(...)
  {
    return false;
  }
  return true;
}

template <class TInputValue, class TTargetValue>
bool
KNearestNeighborsMachineLearningModel<TInputValue,TTargetValue>
::CanWriteFile(const std::string & file)
{
  return false;
}


template <class TInputValue, class TTargetValue>
void
KNearestNeighborsMachineLearningModel<TInputValue,TTargetValue>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}

} //end namespace otb

#endif
