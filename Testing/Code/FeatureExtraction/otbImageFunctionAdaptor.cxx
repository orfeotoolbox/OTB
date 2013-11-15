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



#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingMinMaxImageFilter.h"

#include "otbImageFunctionAdaptor.h"

#include "otbFourierMellinDescriptorsImageFunction.h"
#include "otbRealMomentsImageFunction.h"
#include "otbComplexMomentsImageFunction.h"
#include "otbFlusserMomentsImageFunction.h"
#include "otbHuMomentsImageFunction.h"
#include "otbRadiometricMomentsImageFunction.h"
#include "otbLocalHistogramImageFunction.h"


int otbImageFunctionAdaptorNew(int argc, char * argv[])
{
  typedef double InputPixelType;
  typedef double PrecisionType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension>                        InputImageType;

  typedef otb::FourierMellinDescriptorsImageFunction<InputImageType>    FMDFunctionType;
  typedef otb::RealMomentsImageFunction<InputImageType>                 RMFunctionType;
  typedef otb::ComplexMomentsImageFunction<InputImageType>              CMFunctionType;
  typedef otb::FlusserMomentsImageFunction<InputImageType>              FMFunctionType;
  typedef otb::HuMomentsImageFunction<InputImageType>                   HMFunctionType;
  typedef otb::RadiometricMomentsImageFunction<InputImageType>          RaMFunctionType;
  typedef otb::LocalHistogramImageFunction<InputImageType>              LHFunctionType;

  typedef otb::ImageFunctionAdaptor<FMDFunctionType, PrecisionType>                    FMDImageFunctionAdaptorType;
  typedef otb::ImageFunctionAdaptor<RMFunctionType, PrecisionType>                     RMImageFunctionAdaptorType;
  typedef otb::ImageFunctionAdaptor<CMFunctionType, PrecisionType>                     CMImageFunctionAdaptorType;
  typedef otb::ImageFunctionAdaptor<FMFunctionType, PrecisionType>                     FMImageFunctionAdaptorType;
  typedef otb::ImageFunctionAdaptor<HMFunctionType, PrecisionType>                     HMImageFunctionAdaptorType;
  typedef otb::ImageFunctionAdaptor<RaMFunctionType, PrecisionType>                    RaMImageFunctionAdaptorType;
  typedef otb::ImageFunctionAdaptor<LHFunctionType, PrecisionType>                     LHImageFunctionAdaptorType;

  // Instantiating objects
  FMDImageFunctionAdaptorType::Pointer FMDadaptedFunction = FMDImageFunctionAdaptorType::New();
  std::cout << FMDadaptedFunction << std::endl;
  RMImageFunctionAdaptorType::Pointer RMadaptedFunction = RMImageFunctionAdaptorType::New();
  std::cout << RMadaptedFunction << std::endl;
  CMImageFunctionAdaptorType::Pointer CMadaptedFunction = CMImageFunctionAdaptorType::New();
  std::cout << CMadaptedFunction << std::endl;
  FMImageFunctionAdaptorType::Pointer FMadaptedFunction = FMImageFunctionAdaptorType::New();
  std::cout << FMadaptedFunction << std::endl;
  HMImageFunctionAdaptorType::Pointer HMadaptedFunction = HMImageFunctionAdaptorType::New();
  std::cout << HMadaptedFunction << std::endl;
  RaMImageFunctionAdaptorType::Pointer RaMadaptedFunction = RaMImageFunctionAdaptorType::New();
  std::cout << RaMadaptedFunction << std::endl;
  LHImageFunctionAdaptorType::Pointer LHadaptedFunction = LHImageFunctionAdaptorType::New();
  std::cout << LHadaptedFunction << std::endl;

  return EXIT_SUCCESS;
}

int otbImageFunctionAdaptor(int argc, char * argv[])
{
  const char * inputFilename  = argv[1];

  typedef double InputPixelType;
  typedef double PrecisionType;
  const unsigned int Dimension = 2;
  unsigned int rsltIdx = 0;

  typedef otb::Image<InputPixelType,  Dimension>                        InputImageType;
  typedef otb::ImageFileReader<InputImageType>                          ReaderType;
  typedef otb::StreamingMinMaxImageFilter<InputImageType>               MinMaxFilterType;

  typedef otb::FourierMellinDescriptorsImageFunction<InputImageType>    FMDFunctionType;
  typedef otb::RealMomentsImageFunction<InputImageType>                 RMFunctionType;
  typedef otb::ComplexMomentsImageFunction<InputImageType>              CMFunctionType;
  typedef otb::FlusserMomentsImageFunction<InputImageType>              FMFunctionType;
  typedef otb::HuMomentsImageFunction<InputImageType>                   HMFunctionType;
  typedef otb::RadiometricMomentsImageFunction<InputImageType>          RaMFunctionType;
  typedef otb::LocalHistogramImageFunction<InputImageType>              LHFunctionType;

  typedef otb::ImageFunctionAdaptor<FMDFunctionType, PrecisionType>                                    FMDImageFunctionAdaptorType;
  typedef otb::ImageFunctionAdaptor<RMFunctionType, PrecisionType>                                     RMImageFunctionAdaptorType;
  typedef otb::ImageFunctionAdaptor<CMFunctionType, PrecisionType>                                     CMImageFunctionAdaptorType;
  typedef otb::ImageFunctionAdaptor<FMFunctionType, PrecisionType>                                     FMImageFunctionAdaptorType;
  typedef otb::ImageFunctionAdaptor<HMFunctionType, PrecisionType>                                     HMImageFunctionAdaptorType;
  typedef otb::ImageFunctionAdaptor<RaMFunctionType, PrecisionType>                                    RaMImageFunctionAdaptorType;
  typedef otb::ImageFunctionAdaptor<LHFunctionType, PrecisionType>                                     LHImageFunctionAdaptorType;

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

  FMDImageFunctionAdaptorType::Pointer FMDadaptedFunction = FMDImageFunctionAdaptorType::New();
  RMImageFunctionAdaptorType::Pointer  RMadaptedFunction  = RMImageFunctionAdaptorType::New();
  CMImageFunctionAdaptorType::Pointer  CMadaptedFunction  = CMImageFunctionAdaptorType::New();
  FMImageFunctionAdaptorType::Pointer  FMadaptedFunction  = FMImageFunctionAdaptorType::New();
  HMImageFunctionAdaptorType::Pointer  HMadaptedFunction  = HMImageFunctionAdaptorType::New();
  RaMImageFunctionAdaptorType::Pointer RaMadaptedFunction = RaMImageFunctionAdaptorType::New();
  LHImageFunctionAdaptorType::Pointer  LHadaptedFunction  = LHImageFunctionAdaptorType::New();

  reader->SetFileName(inputFilename);
  filter->SetInput(reader->GetOutput());
  filter->Update();

  InputImageType::IndexType index;
  index[0] = 100;
  index[1] = 100;

  // Content testing
  double error = 0.0;


  try
    {
    FMDFunction->SetInputImage(reader->GetOutput());
    FMDFunction->SetNeighborhoodRadius(5);
    FMDFunction->SetPmax(5);
    FMDFunction->SetQmax(5);
    FMDFunctionType::OutputType resultFMD = FMDFunction->EvaluateAtIndex(index);

    FMDadaptedFunction->SetInputImage(reader->GetOutput());
    FMDadaptedFunction->GetInternalImageFunction()->SetNeighborhoodRadius(5);
    FMDadaptedFunction->GetInternalImageFunction()->SetPmax(5);
    FMDadaptedFunction->GetInternalImageFunction()->SetQmax(5);
    FMDImageFunctionAdaptorType::OutputType resultAdaptedFMD = FMDadaptedFunction->EvaluateAtIndex(index);

    rsltIdx = 0;
    for (unsigned int i = 0; i <= 5; ++i)
      {
      for (unsigned int j = 0; j <= 5; ++j)
        {
        error += vcl_pow(vcl_abs(resultAdaptedFMD[rsltIdx] - resultFMD.at(i).at(j)), 2);

        std::cout << "resultAdaptedFMD : " << resultAdaptedFMD[rsltIdx]
            << "\t - resultFMD : " << resultFMD.at(i).at(j) << std::endl;
        rsltIdx++;
        }
      }
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for FMDadaptedFunction() !" << std::endl;
    std::cout << err << std::endl;

    return EXIT_FAILURE;
    }


  try
    {
    RMFunction->SetInputImage(reader->GetOutput());
    RMFunction->SetNeighborhoodRadius(5);
    RMFunction->SetPmax(5);
    RMFunction->SetQmax(5);
    RMFunctionType::OutputType resultRM = RMFunction->EvaluateAtIndex(index);

    RMadaptedFunction->SetInputImage(reader->GetOutput());
    RMadaptedFunction->GetInternalImageFunction()->SetNeighborhoodRadius(5);
    RMadaptedFunction->GetInternalImageFunction()->SetPmax(5);
    RMadaptedFunction->GetInternalImageFunction()->SetQmax(5);
    RMImageFunctionAdaptorType::OutputType resultAdaptedRM = RMadaptedFunction->EvaluateAtIndex(index);

    rsltIdx = 0;
    for (unsigned int i = 0; i <= 5; ++i)
      {
      for (unsigned int j = 0; j <= 5; ++j)
        {
        error += vcl_pow(vcl_abs(resultAdaptedRM[rsltIdx] - resultRM.at(i).at(j)), 2);

        std::cout << "resultAdaptedRM : " << resultAdaptedRM[rsltIdx] << "\t - resultRM : " << resultRM.at(i).at(j)
            << std::endl;
        rsltIdx++;
        }
      }
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for RMFunction() !" << std::endl;
    std::cout << err << std::endl;

    return EXIT_FAILURE;
    }

  try
    {
    CMFunction->SetInputImage(reader->GetOutput());
    CMFunction->SetNeighborhoodRadius(5);
    CMFunction->SetPmax(5);
    CMFunction->SetQmax(5);
    CMFunctionType::OutputType resultCM = CMFunction->EvaluateAtIndex(index);

    CMadaptedFunction->SetInputImage(reader->GetOutput());
    CMadaptedFunction->GetInternalImageFunction()->SetNeighborhoodRadius(5);
    CMadaptedFunction->GetInternalImageFunction()->SetPmax(5);
    CMadaptedFunction->GetInternalImageFunction()->SetQmax(5);
    CMImageFunctionAdaptorType::OutputType resultAdaptedCM = CMadaptedFunction->EvaluateAtIndex(index);

    rsltIdx = 0;
    for (unsigned int i = 0; i <= 5; ++i)
      {
      for (unsigned int j = 0; j <= 5; ++j)
        {
        error += vcl_pow(vcl_abs(resultAdaptedCM[rsltIdx] - resultCM.at(i).at(j).real()), 2);
        std::cout << "resultAdaptedCM : (" << resultAdaptedCM[rsltIdx] << "," << resultAdaptedCM[rsltIdx + 1] << ")"
            << "\t - resultCM : " << resultCM.at(i).at(j) << std::endl;
        rsltIdx++;
        error += vcl_pow(vcl_abs(resultAdaptedCM[rsltIdx] - resultCM.at(i).at(j).imag()), 2);
        rsltIdx++;
        }
      }
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for CMadaptedFunction() !" << std::endl;
    std::cout << err << std::endl;

    return EXIT_FAILURE;
    }

  try
    {
    FMFunction->SetInputImage(reader->GetOutput());
    FMFunction->SetNeighborhoodRadius(5);
    FMFunctionType::OutputType resultFM = FMFunction->EvaluateAtIndex(index);

    FMadaptedFunction->SetInputImage(reader->GetOutput());
    FMadaptedFunction->GetInternalImageFunction()->SetNeighborhoodRadius(5);
    FMImageFunctionAdaptorType::OutputType resultAdaptedFM = FMadaptedFunction->EvaluateAtIndex(index);

    rsltIdx = 0;
    for (unsigned int i = 0; i < 11; ++i)
      {
      error += vcl_pow(vcl_abs(resultAdaptedFM[rsltIdx] - resultFM[i]), 2);

      std::cout << "resultAdaptedFM : " << resultAdaptedFM[rsltIdx] << "\t - resultFM : " << resultFM[i] << std::endl;
      rsltIdx++;
      }
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for FMFunction() !" << std::endl;
    std::cout << err << std::endl;

    return EXIT_FAILURE;
    }

  try
    {
    HMFunction->SetInputImage(reader->GetOutput());
    HMFunction->SetNeighborhoodRadius(5);
    HMFunctionType::OutputType resultHM = HMFunction->EvaluateAtIndex(index);

    HMadaptedFunction->SetInputImage(reader->GetOutput());
    HMadaptedFunction->GetInternalImageFunction()->SetNeighborhoodRadius(5);
    HMImageFunctionAdaptorType::OutputType resultAdaptedHM = HMadaptedFunction->EvaluateAtIndex(index);

    rsltIdx = 0;
    for (unsigned int i = 0; i < 7; ++i)
      {
      error += vcl_pow(vcl_abs(resultAdaptedHM[rsltIdx] - resultHM[i]), 2);

      std::cout << "resultAdaptedHM : " << resultAdaptedHM[rsltIdx] << "\t - resultHM : " << resultHM[i] << std::endl;
      rsltIdx++;
      }
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for HMFunction() !" << std::endl;
    std::cout << err << std::endl;

    return EXIT_FAILURE;
    }

  try
    {
    RaMFunction->SetInputImage(reader->GetOutput());
    RaMFunction->SetNeighborhoodRadius(5);
    RaMFunctionType::OutputType resultRaM = RaMFunction->EvaluateAtIndex(index);

    RaMadaptedFunction->SetInputImage(reader->GetOutput());
    RaMadaptedFunction->GetInternalImageFunction()->SetNeighborhoodRadius(5);
    RaMImageFunctionAdaptorType::OutputType resultAdaptedRaM = RaMadaptedFunction->EvaluateAtIndex(index);

    rsltIdx = 0;
    for (unsigned int i = 0; i < 4; ++i)
      {
      error += vcl_pow(vcl_abs(resultAdaptedRaM[rsltIdx] - resultRaM[i]), 2);

      std::cout << "resultAdaptedRaM : " << resultAdaptedRaM[rsltIdx] << "\t - resultRaM : " << resultRaM[i]
          << std::endl;
      rsltIdx++;
      }
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for RaMFunction() !" << std::endl;
    std::cout << err << std::endl;

    return EXIT_FAILURE;
    }

  try
    {
    LHFunction->SetInputImage(reader->GetOutput());
    LHFunction->SetNeighborhoodRadius(5);
    LHFunction->SetNumberOfHistogramBins(64);
    LHFunction->SetHistogramMin(filter->GetMinimum());
    LHFunction->SetHistogramMax(filter->GetMaximum());
    LHFunctionType::OutputType resultLH = LHFunction->EvaluateAtIndex(index);

    LHadaptedFunction->SetInputImage(reader->GetOutput());
    LHadaptedFunction->GetInternalImageFunction()->SetNeighborhoodRadius(5);
    LHadaptedFunction->GetInternalImageFunction()->SetNumberOfHistogramBins(64);
    LHadaptedFunction->GetInternalImageFunction()->SetHistogramMin(filter->GetMinimum());
    LHadaptedFunction->GetInternalImageFunction()->SetHistogramMax(filter->GetMaximum());
    LHImageFunctionAdaptorType::OutputType resultAdaptedLH = LHadaptedFunction->EvaluateAtIndex(index);

    rsltIdx = 0;
    for (unsigned int i = 0; i < 64; ++i)
      {
      error += vcl_pow(vcl_abs(resultAdaptedLH[rsltIdx] - resultLH->GetFrequency(i)), 2);

      std::cout << "resultAdaptedLH : " << resultAdaptedLH[rsltIdx] << "\t - resultLH : " << resultLH->GetFrequency(i)
          << std::endl;
      rsltIdx++;
      }
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for LHFunction() !" << std::endl;
    std::cout << err << std::endl;

    return EXIT_FAILURE;
    }

  error = vcl_sqrt(error);
  std::cout << std::endl << "Error : " << error << std::endl
            << std::endl;

  if (error > 1E-3)
    {
    itkGenericExceptionMacro( << "Error = " << error
                              << "  > 1E-9     -> TEST FAILLED" << std::endl );
    }

  // Testing the use of a user defined InternalImageFunction instead
  // of the build-in InternalImageFunction
  try
    {
    FMDFunctionType::Pointer myFunction = FMDFunctionType::New();
    FMDImageFunctionAdaptorType::Pointer myAdaptedFunction = FMDImageFunctionAdaptorType::New();

    myFunction->SetNeighborhoodRadius(8);
    myFunction->SetPmax(2);
    myFunction->SetQmax(2);

    myAdaptedFunction->SetInputImage(reader->GetOutput());
    myAdaptedFunction->SetInternalImageFunction(myFunction);
    FMDImageFunctionAdaptorType::OutputType myResult = myAdaptedFunction->EvaluateAtIndex(index);

    rsltIdx = 0;
    for (unsigned int i = 0; i <= 2; ++i)
      {
      for (unsigned int j = 0; j <= 2; ++j)
        {
        std::cout << "myResult: " << myResult[rsltIdx] << std::endl;
        rsltIdx++;
        }
      }
    std::cout << myAdaptedFunction << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for FMDFunctionType() !" << std::endl;
    std::cout << err << std::endl;

    return EXIT_FAILURE;
    }


  return EXIT_SUCCESS;
}

