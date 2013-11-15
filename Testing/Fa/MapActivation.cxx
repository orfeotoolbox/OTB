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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include <iostream>
#include <fstream>
#include "otbVectorImage.h"
#include "otbSOMMap.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "otbImage.h"

#include "otbSOMMap.h"
#include "otbSOM.h"
#include "otbSOMActivationBuilder.h"
#include "itkVectorExpandImageFilter.h"
#include "itkVectorNearestNeighborInterpolateImageFunction.h"

#include "itkExpandImageFilter.h"
#include "itkNearestNeighborInterpolateImageFunction.h"

#include "itkListSample.h"

#include "otbSOMClassifier.h"
#include "itkVariableLengthVector.h"
#include "itkMembershipSample.h"

int main(int argc, char* argv[])
{

  if (argc < 12)
    {
    std::cout << "Usage : " << argv[0] <<
    " inputTabImage size radius NumberOfIterations BetaInit BetaEnd MaxWeight  MinWeight som actMap som" << std::endl;

    return EXIT_FAILURE;
    }

  typedef otb::Image<double, 2> ListImageType;

  typedef otb::ImageFileReader<ListImageType> TabReaderType;
  TabReaderType::Pointer Tabreader = TabReaderType::New();
  Tabreader->SetFileName(argv[1]);
  Tabreader->Update();
  ListImageType::Pointer tabreadImage = Tabreader->GetOutput();

  typedef itk::ImageRegionIterator<ListImageType> IteratorType;
  IteratorType It1(tabreadImage, tabreadImage->GetLargestPossibleRegion());
  const int    nblines = 591; //const int)(tabreadImage->GetLargestPossibleRegion().GetSize()[1]);

  const int nbcolonnes = 9; //(const int)tabreadImage->GetLargestPossibleRegion().GetSize()[0];

  double vectTab[nblines][nbcolonnes];
  std::cout << "lignes = " << nblines << " colonnes = " << nbcolonnes << std::endl;
  for (It1.GoToBegin(); !It1.IsAtEnd(); ++It1)
    {
    vectTab[It1.GetIndex()[1]][It1.GetIndex()[0]] = It1.Get();
    }

  typedef itk::VariableLengthVector<double>                  MeasurementVectorType;
  typedef itk::Statistics::ListSample<MeasurementVectorType> SampleType;
  
  SampleType::Pointer liste = SampleType::New();
  liste->SetMeasurementVectorSize(nbcolonnes);

  for (int j = 0; j < nblines; ++j)
    {
    MeasurementVectorType tab;
    tab.SetSize(nbcolonnes);
    for (int i = 0; i < nbcolonnes; ++i)
      {
      tab[i] = vectTab[j][i];
      }
    liste->PushBack(tab);
    }
  //   std::cout<<"liste: "<<liste->GetMeasurementVectorSize()<< " " << liste->GetMeasurementVector(1)
  //   <<" " <<liste->GetMeasurementVector(2)<< " " <<liste->GetMeasurementVector(3)<<std::endl;

  typedef itk::Statistics::EuclideanDistanceMetric<MeasurementVectorType> DistanceType;
  typedef otb::SOMMap<MeasurementVectorType, DistanceType, 2>       MapType;

  typedef otb::SOM<SampleType, MapType> SOMType;
  SOMType::Pointer som = SOMType::New();
  som->SetListSample(liste);
  SOMType::SizeType som_size;
  som_size[0] = atoi(argv[2]);
  som_size[1] = atoi(argv[2]);
  som->SetMapSize(som_size);
  SOMType::SizeType radius;
  radius[0] = atoi(argv[3]);
  radius[1] = atoi(argv[3]);
  som->SetNeighborhoodSizeInit(radius);

  som->SetNumberOfIterations(atoi(argv[4]));
  som->SetBetaInit(atoi(argv[5]));
  som->SetBetaEnd(atoi(argv[6]));
  som->SetMaxWeight(atoi(argv[7]));
  som->SetMinWeight(atoi(argv[8]));
  som->Update();

  typedef otb::ImageFileWriter<MapType> SomWriterType;
  SomWriterType::Pointer somwriter = SomWriterType::New();
  somwriter->SetFileName(argv[9] /*"som.hd"*/);
  somwriter->SetInput(som->GetOutput());
  somwriter->Update();

  typedef unsigned char OutputPixelType;

  typedef otb::Image<OutputPixelType, 2>                                  OutputImageType;
  typedef otb::ImageFileWriter<OutputImageType>                           ActivationWriterType;
  typedef otb::SOMActivationBuilder<SampleType, MapType, OutputImageType> SOMActivationBuilderType;
  SOMActivationBuilderType::Pointer somAct = SOMActivationBuilderType::New();
  somAct->SetInput(som->GetOutput());
  somAct->SetListSample(liste);

  ActivationWriterType::Pointer actWriter = ActivationWriterType::New();
  actWriter->SetFileName(argv[10] /*"actMap.png"*/);
  actWriter->SetInput(somAct->GetOutput());
  actWriter->Update();

  //Classifier :

  typedef otb::ImageFileReader<MapType>                          SOMReaderType;
  typedef otb::SOMClassifier<SampleType, MapType, unsigned char> ClassifierType;

  SOMReaderType::Pointer somreader = SOMReaderType::New();
  somreader->SetFileName(argv[11] /*"som.mhd"*/);
  somreader->Update();

  ClassifierType::Pointer classifier = ClassifierType::New();

  classifier->SetSample(liste);
  classifier->SetMap(somreader->GetOutput());
  classifier->Update();

  ClassifierType::OutputType* membershipSample = classifier->GetOutput();
  //   std::cout<<"liste: "<<membershipSample->GetMeasurementVectorSize()<< " " << membershipSample->GetMeasurementVector(1)<<std::endl;

  ClassifierType::OutputType::ConstIterator m_iter =  membershipSample->Begin();
  ClassifierType::OutputType::ConstIterator m_last =  membershipSample->End();

  /*int count[16];
  for(int i=0; i<16; ++i)
  {
  count[i]=0;
  }
  */
  while (m_iter != m_last)
    {
//      std::cout<<" classlabel : "<<m_iter.GetClassLabel()<<
//  " ClassSize= "<< membershipSample->GetClassSampleSize(m_iter.GetClassLabel()) <<
//  " instance id = " << m_iter.GetInstanceIdentifier() <<
//  " measurement vector = " << m_iter.GetMeasurementVector() <<
//
//  " class sample: "<< membershipSample->GetClassSample(m_iter.GetClassLabel())<<
//  std::endl;
    //count[m_iter.GetClassLabel()]++;
    ++m_iter;
    }

  /*for(int i=0; i<16; ++i)
    {
    std::cout<<" classe: "<< i <<" nb elements= "<<count[i]<<std::endl;
    }*/

  int nbclass = membershipSample->GetNumberOfClasses();
  std::cout << " nb of classes= " << nbclass << std::endl;


//FIXME methods not available in the new stat framework API
/*
  for (int i = 0; i < nbclass; i++)
    {
    if (membershipSample->GetInternalClassLabel(i) != -1)
      {
      std::cout << "classlabel= " << i << "  classSize= " << membershipSample->GetClassSampleSize(i) << std::endl;
      }
    }
*/

  return EXIT_SUCCESS;
}
