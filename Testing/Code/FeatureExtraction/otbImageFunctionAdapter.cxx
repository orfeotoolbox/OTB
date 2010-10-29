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

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingMinMaxImageFilter.h"

#include "otbImageFunctionAdapter.h"

#include "otbFourierMellinDescriptorsImageFunction.h"
#include "otbRealMomentsImageFunction.h"
#include "otbComplexMomentsImageFunction.h"
#include "otbFlusserMomentsImageFunction.h"
#include "otbHuMomentsImageFunction.h"
#include "otbRadiometricMomentsImageFunction.h"
#include "otbLocalHistogramImageFunction.h"


int otbImageFunctionAdapterNew(int argc, char * argv[])
{
  typedef double InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension>                        InputImageType;
  
  typedef otb::FourierMellinDescriptorsImageFunction<InputImageType>    FMDFunctionType;
  typedef otb::RealMomentsImageFunction<InputImageType>                 RMFunctionType;
  typedef otb::ComplexMomentsImageFunction<InputImageType>              CMFunctionType;
  typedef otb::FlusserMomentsImageFunction<InputImageType>              FMFunctionType;
  typedef otb::HuMomentsImageFunction<InputImageType>                   HMFunctionType;
  typedef otb::RadiometricMomentsImageFunction<InputImageType>          RaMFunctionType;
  typedef otb::LocalHistogramImageFunction<InputImageType>              LHFunctionType;

  typedef otb::ImageFunctionAdapter<InputImageType, 
    FMDFunctionType>                                    FMDImageFunctionAdapterType;
  typedef otb::ImageFunctionAdapter<InputImageType, 
    RMFunctionType>                                     RMImageFunctionAdapterType;
  typedef otb::ImageFunctionAdapter<InputImageType, 
    CMFunctionType>                                     CMImageFunctionAdapterType;
  typedef otb::ImageFunctionAdapter<InputImageType, 
    FMFunctionType>                                     FMImageFunctionAdapterType;
  typedef otb::ImageFunctionAdapter<InputImageType, 
    HMFunctionType>                                     HMImageFunctionAdapterType;
  typedef otb::ImageFunctionAdapter<InputImageType, 
    RaMFunctionType>                                    RaMImageFunctionAdapterType;
  typedef otb::ImageFunctionAdapter<InputImageType, 
    LHFunctionType>                                     LHImageFunctionAdapterType;

  // Instantiating objects
  FMDImageFunctionAdapterType::Pointer FMDadaptedFunction = FMDImageFunctionAdapterType::New();
  std::cout << FMDadaptedFunction << std::endl;
  RMImageFunctionAdapterType::Pointer RMadaptedFunction = RMImageFunctionAdapterType::New();
  std::cout << RMadaptedFunction << std::endl;
  CMImageFunctionAdapterType::Pointer CMadaptedFunction = CMImageFunctionAdapterType::New();
  std::cout << CMadaptedFunction << std::endl;
  FMImageFunctionAdapterType::Pointer FMadaptedFunction = FMImageFunctionAdapterType::New();
  std::cout << FMadaptedFunction << std::endl;
  HMImageFunctionAdapterType::Pointer HMadaptedFunction = HMImageFunctionAdapterType::New();
  std::cout << HMadaptedFunction << std::endl;
  RaMImageFunctionAdapterType::Pointer RaMadaptedFunction = RaMImageFunctionAdapterType::New();
  std::cout << RaMadaptedFunction << std::endl;
  LHImageFunctionAdapterType::Pointer LHadaptedFunction = LHImageFunctionAdapterType::New();
  std::cout << LHadaptedFunction << std::endl;
 
  return EXIT_SUCCESS;
}

int otbImageFunctionAdapter(int argc, char * argv[])
{
  const char * inputFilename  = argv[1];
  
  typedef double InputPixelType;
  const unsigned int Dimension = 2;
  unsigned int rsltIdx = 0;

  typedef otb::Image<InputPixelType,  Dimension>                        InputImageType;
  typedef otb::ImageFileReader<InputImageType>                          ReaderType;
  typedef otb::StreamingMinMaxImageFilter<InputImageType>         MinMaxFilterType;
  
  typedef otb::FourierMellinDescriptorsImageFunction<InputImageType>    FMDFunctionType;
  typedef otb::RealMomentsImageFunction<InputImageType>                 RMFunctionType;
  typedef otb::ComplexMomentsImageFunction<InputImageType>              CMFunctionType;
  typedef otb::FlusserMomentsImageFunction<InputImageType>              FMFunctionType;
  typedef otb::HuMomentsImageFunction<InputImageType>                   HMFunctionType;
  typedef otb::RadiometricMomentsImageFunction<InputImageType>          RaMFunctionType;
  typedef otb::LocalHistogramImageFunction<InputImageType>              LHFunctionType;

  typedef otb::ImageFunctionAdapter<InputImageType, 
    FMDFunctionType>                                    FMDImageFunctionAdapterType;
  typedef otb::ImageFunctionAdapter<InputImageType, 
    RMFunctionType>                                     RMImageFunctionAdapterType;
  typedef otb::ImageFunctionAdapter<InputImageType, 
    CMFunctionType>                                     CMImageFunctionAdapterType;
  typedef otb::ImageFunctionAdapter<InputImageType, 
    FMFunctionType>                                     FMImageFunctionAdapterType;
  typedef otb::ImageFunctionAdapter<InputImageType, 
    HMFunctionType>                                     HMImageFunctionAdapterType;
  typedef otb::ImageFunctionAdapter<InputImageType, 
    RaMFunctionType>                                    RaMImageFunctionAdapterType;
  typedef otb::ImageFunctionAdapter<InputImageType, 
    LHFunctionType>                                     LHImageFunctionAdapterType;

  // Instantiating objects
  ReaderType::Pointer  reader = ReaderType::New();
  MinMaxFilterType::Pointer filter   = MinMaxFilterType::New();
  
  FMDFunctionType::Pointer FMDFunction = FMDFunctionType::New();
  RMFunctionType::Pointer  RMFunction  = RMFunctionType::New();
  CMFunctionType::Pointer  CMFunction  = CMFunctionType::New();
  FMFunctionType::Pointer  FMFunction  = FMFunctionType::New();
  HMFunctionType::Pointer  HMFunction  = HMFunctionType::New();
  RaMFunctionType::Pointer RaMFunction = RaMFunctionType::New();
  LHFunctionType::Pointer  LHFunction  = LHFunctionType::New();

  FMDImageFunctionAdapterType::Pointer FMDadaptedFunction = FMDImageFunctionAdapterType::New();
  RMImageFunctionAdapterType::Pointer  RMadaptedFunction  = RMImageFunctionAdapterType::New();
  CMImageFunctionAdapterType::Pointer  CMadaptedFunction  = CMImageFunctionAdapterType::New();
  FMImageFunctionAdapterType::Pointer  FMadaptedFunction  = FMImageFunctionAdapterType::New();
  HMImageFunctionAdapterType::Pointer  HMadaptedFunction  = HMImageFunctionAdapterType::New();
  RaMImageFunctionAdapterType::Pointer RaMadaptedFunction = RaMImageFunctionAdapterType::New();
  LHImageFunctionAdapterType::Pointer  LHadaptedFunction  = LHImageFunctionAdapterType::New();
   
  reader->SetFileName(inputFilename);
  filter->SetInput(reader->GetOutput());
  filter->Update();

  InputImageType::IndexType index;
  index[0] = 100;
  index[1] = 100;
  
  // Content testing 
  double error = 0.0;

  FMDFunction->SetInputImage(reader->GetOutput());
  FMDFunction->SetNeighborhoodRadius(5);
  FMDFunction->SetPmax(5);
  FMDFunction->SetQmax(5);
  FMDFunctionType::OutputType resultFMD = FMDFunction->EvaluateAtIndex(index);

  FMDadaptedFunction->SetInputImage(reader->GetOutput());
  FMDadaptedFunction->GetImageFunction()->SetNeighborhoodRadius(5);
  FMDadaptedFunction->GetImageFunction()->SetPmax(5);
  FMDadaptedFunction->GetImageFunction()->SetQmax(5);
  FMDImageFunctionAdapterType::OutputType resultAdaptedFMD = FMDadaptedFunction->EvaluateAtIndex(index);
  
  rsltIdx = 0;
  for (unsigned int i=0; i<=5; i++)
    {
    for (unsigned int j=0; j<=5; j++)
      {
      error += vcl_pow(vcl_abs(resultAdaptedFMD[rsltIdx] - resultFMD.at(i).at(j)), 2);
    
      std::cout << "resultAdaptedFMD : " << resultAdaptedFMD[rsltIdx] 
                << "\t - resultFMD : " << resultFMD.at(i).at(j) << std::endl;
      rsltIdx ++;
      }
    }

  RMFunction->SetInputImage(reader->GetOutput());
  RMFunction->SetNeighborhoodRadius(5);
  RMFunction->SetPmax(5);
  RMFunction->SetQmax(5);
  RMFunctionType::OutputType resultRM = RMFunction->EvaluateAtIndex(index);

  RMadaptedFunction->SetInputImage(reader->GetOutput());
  RMadaptedFunction->GetImageFunction()->SetNeighborhoodRadius(5);
  RMadaptedFunction->GetImageFunction()->SetPmax(5);
  RMadaptedFunction->GetImageFunction()->SetQmax(5);
  RMImageFunctionAdapterType::OutputType resultAdaptedRM = RMadaptedFunction->EvaluateAtIndex(index);
  
  rsltIdx = 0;
  for (unsigned int i=0; i<=5; i++)
    {
    for (unsigned int j=0; j<=5; j++)
      {
      error += vcl_pow(vcl_abs(resultAdaptedRM[rsltIdx] - resultRM.at(i).at(j)), 2);

      std::cout << "resultAdaptedRM : " << resultAdaptedRM[rsltIdx] 
                << "\t - resultRM : " << resultRM.at(i).at(j) << std::endl;
      rsltIdx ++;
      }
    }

  CMFunction->SetInputImage(reader->GetOutput());
  CMFunction->SetNeighborhoodRadius(5);
  CMFunction->SetPmax(5);
  CMFunction->SetQmax(5);
  CMFunctionType::OutputType resultCM = CMFunction->EvaluateAtIndex(index);

  CMadaptedFunction->SetInputImage(reader->GetOutput());
  CMadaptedFunction->GetImageFunction()->SetNeighborhoodRadius(5);
  CMadaptedFunction->GetImageFunction()->SetPmax(5);
  CMadaptedFunction->GetImageFunction()->SetQmax(5);
  CMImageFunctionAdapterType::OutputType resultAdaptedCM = CMadaptedFunction->EvaluateAtIndex(index);
  
  rsltIdx = 0;
  for (unsigned int i=0; i<=5; i++)
    {
    for (unsigned int j=0; j<=5; j++)
      {
      error += vcl_pow(vcl_abs(resultAdaptedCM[rsltIdx] - resultCM.at(i).at(j).real()), 2);
      std::cout << "resultAdaptedCM : (" << resultAdaptedCM[rsltIdx]
                << "," << resultAdaptedCM[rsltIdx+1] << ")"
                << "\t - resultCM : " << resultCM.at(i).at(j) << std::endl;
      rsltIdx ++;
      error += vcl_pow(vcl_abs(resultAdaptedCM[rsltIdx] - resultCM.at(i).at(j).imag()), 2);
      rsltIdx ++;
      }
    }

  FMFunction->SetInputImage(reader->GetOutput());
  FMFunction->SetNeighborhoodRadius(5);
  FMFunctionType::OutputType resultFM = FMFunction->EvaluateAtIndex(index);

  FMadaptedFunction->SetInputImage(reader->GetOutput());
  FMadaptedFunction->GetImageFunction()->SetNeighborhoodRadius(5);
  FMImageFunctionAdapterType::OutputType resultAdaptedFM = FMadaptedFunction->EvaluateAtIndex(index);
  
  rsltIdx = 0;
  for (unsigned int i=0; i<11; i++)
    {
      error += vcl_pow(vcl_abs(resultAdaptedFM[rsltIdx] - resultFM[i]), 2);

      std::cout << "resultAdaptedFM : " << resultAdaptedFM[rsltIdx] 
                << "\t - resultFM : " << resultFM[i] << std::endl;
      rsltIdx ++;
    }

  HMFunction->SetInputImage(reader->GetOutput());
  HMFunction->SetNeighborhoodRadius(5);
  HMFunctionType::OutputType resultHM = HMFunction->EvaluateAtIndex(index);

  HMadaptedFunction->SetInputImage(reader->GetOutput());
  HMadaptedFunction->GetImageFunction()->SetNeighborhoodRadius(5);
  HMImageFunctionAdapterType::OutputType resultAdaptedHM = HMadaptedFunction->EvaluateAtIndex(index);
  
  rsltIdx = 0;
  for (unsigned int i=0; i<=7; i++)
    {
    error += vcl_pow(vcl_abs(resultAdaptedHM[rsltIdx] - resultHM[i]), 2);

    std::cout << "resultAdaptedHM : " << resultAdaptedHM[rsltIdx] 
              << "\t - resultHM : " << resultHM[i] << std::endl;
    rsltIdx ++;
    }

  RaMFunction->SetInputImage(reader->GetOutput());
  RaMFunction->SetNeighborhoodRadius(5);
  RaMFunctionType::OutputType resultRaM = RaMFunction->EvaluateAtIndex(index);
  
  RaMadaptedFunction->SetInputImage(reader->GetOutput());
  RaMadaptedFunction->GetImageFunction()->SetNeighborhoodRadius(5);
  RaMImageFunctionAdapterType::OutputType resultAdaptedRaM = RaMadaptedFunction->EvaluateAtIndex(index);
  
  rsltIdx = 0;
  for (unsigned int i=0; i<4; i++)
    { 
    error += vcl_pow(vcl_abs(resultAdaptedRaM[rsltIdx] - resultRaM[i]), 2);

    std::cout << "resultAdaptedRaM : " << resultAdaptedRaM[rsltIdx] 
              << "\t - resultRaM : " << resultRaM[i] << std::endl;
    rsltIdx ++;
    }

  LHFunction->SetInputImage(reader->GetOutput());
  LHFunction->SetNeighborhoodRadius(5);
  LHFunction->SetNumberOfHistogramBins(64);
  LHFunction->SetHistogramMin(filter->GetMinimum());
  LHFunction->SetHistogramMax(filter->GetMaximum());
  LHFunctionType::OutputType resultLH = LHFunction->EvaluateAtIndex(index);

  LHadaptedFunction->SetInputImage(reader->GetOutput());
  LHadaptedFunction->GetImageFunction()->SetNeighborhoodRadius(5);
  LHadaptedFunction->GetImageFunction()->SetNumberOfHistogramBins(64);
  LHadaptedFunction->GetImageFunction()->SetHistogramMin(filter->GetMinimum());
  LHadaptedFunction->GetImageFunction()->SetHistogramMax(filter->GetMaximum());
  LHImageFunctionAdapterType::OutputType resultAdaptedLH = LHadaptedFunction->EvaluateAtIndex(index);
  
  rsltIdx = 0;
  for (unsigned int i=0; i<64; i++)
    {
    error += vcl_pow(vcl_abs( resultAdaptedLH[rsltIdx] - resultLH->GetFrequency(i)), 2);

    std::cout << "resultAdaptedLH : " << resultAdaptedLH[rsltIdx]
              << "\t - resultLH : " << resultLH->GetFrequency(i) << std::endl;
    rsltIdx ++;
    }
  
  error = vcl_sqrt(error);
  std::cout << std::endl << "Error : " << error << std::endl
            << std::endl;

 if (error > 1E-3)
    {
    itkGenericExceptionMacro( << "Error = " << error
                              << "  > 1E-9     -> TEST FAILLED" << std::endl );
    }

  return EXIT_SUCCESS;
}
