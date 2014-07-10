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
#include "otbSubPixelDisparityImageFilter.h"
#include "otbPixelWiseBlockMatchingImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbStandardWriterWatcher.h"
#include "otbImage.h"


  const unsigned int Dimension = 2;
  typedef float                                                          PixelType;
  typedef otb::Image<PixelType, Dimension>                               FloatImageType;



  typedef otb::Functor::NCCBlockMatching<FloatImageType,FloatImageType> NCCBlockMatchingFunctorType;

  typedef otb::SubPixelDisparityImageFilter<FloatImageType,
                                               FloatImageType,
                                               FloatImageType,
                                               FloatImageType,
                                               NCCBlockMatchingFunctorType> NCCSubPixelDisparityFilterType;

  typedef otb::Functor::SSDBlockMatching<FloatImageType,FloatImageType> SSDBlockMatchingFunctorType;
  typedef otb::SubPixelDisparityImageFilter<FloatImageType,
                                                FloatImageType,
                                                FloatImageType,
                                                FloatImageType,
                                                SSDBlockMatchingFunctorType> SSDSubPixelDisparityFilterType;


  typedef otb::Functor::LPBlockMatching<FloatImageType,FloatImageType> LPBlockMatchingFunctorType;
  typedef otb::SubPixelDisparityImageFilter<FloatImageType,
                                                  FloatImageType,
                                                  FloatImageType,
                                                  FloatImageType,
                                                  LPBlockMatchingFunctorType> LPSubPixelDisparityFilterType;


  typedef otb::ImageFileReader<FloatImageType>               ReaderType;
  typedef otb::ImageFileWriter<FloatImageType>      WriterType;

int otbSubPixelDisparityImageFilterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{

  // Instantiation
  SSDSubPixelDisparityFilterType::Pointer filter = SSDSubPixelDisparityFilterType::New();

   return EXIT_SUCCESS;
}

template<class TImage,class TFunctorType> int ProcessAndWriteOutput(otb::SubPixelDisparityImageFilter<TImage,
                                                       TImage,TImage,TImage,TFunctorType> * subPixFilter,int argc,char* argv[])
{

  ReaderType::Pointer leftReader = ReaderType::New();
  leftReader->SetFileName(argv[1]);

  ReaderType::Pointer rightReader = ReaderType::New();
  rightReader->SetFileName(argv[2]);

  ReaderType::Pointer bmhdispReader = ReaderType::New();
  bmhdispReader->SetFileName(argv[3]);
  // ReaderType::Pointer bmvdispReader = ReaderType::New();
  //          bmvdispReader->SetFileName(argv[3]);

  ReaderType::Pointer metricReader = ReaderType::New();
  metricReader->SetFileName(argv[4]);

  //  subPixFilter->SetVerticalDisparityInput(bmvdispReader->GetVerticalDisparityOutput());


  subPixFilter->SetLeftInput(leftReader->GetOutput());
  subPixFilter->SetRightInput(rightReader->GetOutput());

  subPixFilter->SetHorizontalDisparityInput(bmhdispReader->GetOutput());
  subPixFilter->SetRadius(atoi(argv[10]));

  subPixFilter->SetMinimumHorizontalDisparity(atoi(argv[11]));
  subPixFilter->SetMaximumHorizontalDisparity(atoi(argv[12]));

  subPixFilter->SetMinimumVerticalDisparity(atoi(argv[13]));
  subPixFilter->SetMaximumVerticalDisparity(atoi(argv[14]));

  subPixFilter->SetMetricInput(metricReader->GetOutput());

  ReaderType::Pointer leftMaskReader = ReaderType::New();
  ReaderType::Pointer rightMaskReader = ReaderType::New();
  if (argc > 15)
    {
    leftMaskReader->SetFileName(argv[15]);
    subPixFilter->SetLeftMaskInput(leftMaskReader->GetOutput());
    }

  if (argc > 16)
    {
    rightMaskReader->SetFileName(argv[16]);
    subPixFilter->SetRightMaskInput(rightMaskReader->GetOutput());
    }

  switch (atoi(argv[9]))
    {
    case 1:
      subPixFilter->SetRefineMethod(NCCSubPixelDisparityFilterType::PARABOLIC);
      break;
    case 2:
      subPixFilter->SetRefineMethod(NCCSubPixelDisparityFilterType::TRIANGULAR);
      break;
    case 3:
      subPixFilter->SetRefineMethod(NCCSubPixelDisparityFilterType::DICHOTOMY);
      break;
    default:
      std::cout << "wrong pixel mode" << std::endl;
      return EXIT_FAILURE;
      break;
    }
  otb::StandardWriterWatcher watcher(subPixFilter, "Sub-pixel refinement");
  // writers
  // horizontal disparity writer
  WriterType::Pointer hdispWriter = WriterType::New();
  hdispWriter->SetInput(subPixFilter->GetHorizontalDisparityOutput());
  hdispWriter->SetFileName(argv[5]);
  otb::StandardWriterWatcher watcher1(hdispWriter, subPixFilter, "Computing disparity ");

  hdispWriter->Update();

  // vertical disparity writer
  WriterType::Pointer vdispWriter = WriterType::New();
  vdispWriter->SetInput(subPixFilter->GetVerticalDisparityOutput());
  vdispWriter->SetFileName(argv[6]);

  vdispWriter->Update();

  // metric writer
  // vertical disparity writer
  WriterType::Pointer metricWriter = WriterType::New();
  metricWriter->SetInput(subPixFilter->GetMetricOutput());
  metricWriter->SetFileName(argv[7]);

  metricWriter->Update();

  return EXIT_SUCCESS;
}


int otbSubPixelDisparityImageFilter(int argc, char* argv[])
{

  if ((argc < 15) || (argc > 17))
    {
    std::cerr << "Usage: " << argv[0]
        << " leftinput_fname rightinput_fname blockmatchinginput_fname blockmatchingmetricinput_fname ";
    std::cerr << "hdispoutput_fname vdispoutput_fname metricoutput_fname " << std::endl;
    std::cerr << "metric (0=SSD, 1=NCC, 2=LP pseudo norm) ";
    std::cerr << "subpixelmode (1=parabolic,2=triangular,3=dichotomy)" << std::endl;
    std::cerr << "radius minhdisp maxhdisp minvdisp maxvdisp " << std::endl;
    std::cerr << "(leftmaskinput_fname) (rightmaskinput_fname)" << std::endl;
    return EXIT_FAILURE;
    }

  SSDSubPixelDisparityFilterType::Pointer SSDSubPixFilter = SSDSubPixelDisparityFilterType::New();
  NCCSubPixelDisparityFilterType::Pointer NCCSubPixFilter = NCCSubPixelDisparityFilterType::New();
  LPSubPixelDisparityFilterType::Pointer LPSubPixFilter = LPSubPixelDisparityFilterType::New();

  // Instantiation
  switch (atoi(argv[8]))
    {
    case 0:
      SSDSubPixFilter->MinimizeOn();
      return ProcessAndWriteOutput<FloatImageType, SSDBlockMatchingFunctorType> (SSDSubPixFilter, argc, argv);
      break;
    case 1:
      NCCSubPixFilter->MinimizeOff();
      return ProcessAndWriteOutput<FloatImageType, NCCBlockMatchingFunctorType> (NCCSubPixFilter, argc, argv);
      break;
    case 2:
      LPSubPixFilter->MinimizeOff();
      return ProcessAndWriteOutput<FloatImageType, LPBlockMatchingFunctorType> (LPSubPixFilter, argc, argv);
      break;
    default:
      std::cout << "wrong norm type  (0=SSD, 1=NCC, 2=LP pseudo norm)" << std::endl;
      return EXIT_FAILURE;
      break;
    }

  return EXIT_FAILURE;
}
