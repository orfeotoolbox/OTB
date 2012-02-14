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

#include "otbHorizontalPixelWiseBlockMatchingImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbStandardWriterWatcher.h"

typedef otb::Image<unsigned short>                    ImageType;
typedef otb::Image<float>                             FloatImageType;
typedef otb::ImageFileReader<ImageType>               ReaderType;
typedef otb::StreamingImageFileWriter<FloatImageType> FloatWriterType;

typedef otb::HorizontalPixelWiseBlockMatchingImageFilter<ImageType,FloatImageType,FloatImageType,ImageType> HorizontalPixelWiseBlockMatchingImageFilterType;

typedef otb::Functor::NCCBlockMatching<ImageType,FloatImageType> NCCBlockMatchingFunctorType;

typedef otb::HorizontalPixelWiseBlockMatchingImageFilter<ImageType,FloatImageType,FloatImageType,ImageType, NCCBlockMatchingFunctorType> HorizontalPixelWiseNCCBlockMatchingImageFilterType;

int otbHorizontalPixelWiseBlockMatchingImageFilterNew(int argc, char * argv[])
{
  // Instanciation
  HorizontalPixelWiseBlockMatchingImageFilterType::Pointer bmFilter = HorizontalPixelWiseBlockMatchingImageFilterType::New();

  return EXIT_SUCCESS;
}

int otbHorizontalPixelWiseBlockMatchingImageFilter(int argc, char * argv[])
{
  ReaderType::Pointer leftReader = ReaderType::New();
  leftReader->SetFileName(argv[1]);

  ReaderType::Pointer rightReader = ReaderType::New();
  rightReader->SetFileName(argv[2]);
  
  HorizontalPixelWiseBlockMatchingImageFilterType::Pointer bmFilter = HorizontalPixelWiseBlockMatchingImageFilterType::New();
  bmFilter->SetLeftInput(leftReader->GetOutput());
  bmFilter->SetRightInput(rightReader->GetOutput());
  bmFilter->SetRadius(atoi(argv[5]));
  bmFilter->SetMinimumDisparity(atoi(argv[6]));
  bmFilter->SetMaximumDisparity(atoi(argv[7]));

  ReaderType::Pointer maskReader = ReaderType::New();
  if(argc > 8)
    {
    maskReader->SetFileName(argv[8]);
    bmFilter->SetMaskInput(maskReader->GetOutput());
    }

  FloatWriterType::Pointer dispWriter = FloatWriterType::New();
  dispWriter->SetInput(bmFilter->GetDisparityOutput());
  dispWriter->SetFileName(argv[3]);

  otb::StandardWriterWatcher watcher1(dispWriter,bmFilter,"Computing disparity map");

  dispWriter->Update();

  FloatWriterType::Pointer metricWriter = FloatWriterType::New();
  metricWriter->SetInput(bmFilter->GetMetricOutput());
  metricWriter->SetFileName(argv[4]);

  otb::StandardWriterWatcher watcher2(metricWriter,bmFilter,"Computing metric map");

  metricWriter->Update();


  return EXIT_SUCCESS;
}


int otbHorizontalPixelWiseBlockMatchingImageFilterNCC(int argc, char * argv[])
{
  ReaderType::Pointer leftReader = ReaderType::New();
  leftReader->SetFileName(argv[1]);

  ReaderType::Pointer rightReader = ReaderType::New();
  rightReader->SetFileName(argv[2]);
  
  HorizontalPixelWiseNCCBlockMatchingImageFilterType::Pointer bmFilter = HorizontalPixelWiseNCCBlockMatchingImageFilterType::New();
  bmFilter->SetLeftInput(leftReader->GetOutput());
  bmFilter->SetRightInput(rightReader->GetOutput());
  bmFilter->SetRadius(atoi(argv[5]));
  bmFilter->SetMinimumDisparity(atoi(argv[6]));
  bmFilter->SetMaximumDisparity(atoi(argv[7]));
  bmFilter->MinimizeOff();

  ReaderType::Pointer maskReader = ReaderType::New();
  if(argc > 8)
    {
    maskReader->SetFileName(argv[8]);
    bmFilter->SetMaskInput(maskReader->GetOutput());
    }

  FloatWriterType::Pointer dispWriter = FloatWriterType::New();
  dispWriter->SetInput(bmFilter->GetDisparityOutput());
  dispWriter->SetFileName(argv[3]);

  otb::StandardWriterWatcher watcher1(dispWriter,bmFilter,"Computing disparity map");

  dispWriter->Update();

  FloatWriterType::Pointer metricWriter = FloatWriterType::New();
  metricWriter->SetInput(bmFilter->GetMetricOutput());
  metricWriter->SetFileName(argv[4]);

  otb::StandardWriterWatcher watcher2(metricWriter,bmFilter,"Computing metric map");

  metricWriter->Update();


  return EXIT_SUCCESS;
}
