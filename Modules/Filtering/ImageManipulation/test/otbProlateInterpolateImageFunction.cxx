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
#include "itkMacro.h"

#include "otbProlateInterpolateImageFunction.h"
#include "otbImageFileReader.h"
#include <fstream>
#include "otbImageFileWriter.h"
#include "itkResampleImageFilter.h"
#include "otbImageFileWriter.h"
#include "otbStreamingResampleImageFilter.h"
#include "otbWindowedSincInterpolateImageGaussianFunction.h"
#include "otbWindowedSincInterpolateImageHammingFunction.h"
#include "otbWindowedSincInterpolateImageCosineFunction.h"
#include "itkWindowedSincInterpolateImageFunction.h"

int otbProlateInterpolateImageFunction(int argc, char * argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[2];
  const char * cosfname = argv[3];
  const char * itkcosfname = argv[4];
  const char * profname = argv[5];

  typedef otb::Image<double, 2>                           ImageType;
  typedef otb::ProlateInterpolateImageFunction<ImageType> InterpolatorType;
  typedef InterpolatorType::ContinuousIndexType           ContinuousIndexType;
  typedef otb::ImageFileReader<ImageType>                 ReaderType;

  unsigned int i = 7;

  std::vector<ContinuousIndexType> indicesList;
  while (i < static_cast<unsigned int>(argc) && (i + 1) < static_cast<unsigned int>(argc))
    {
    ContinuousIndexType idx;
    idx[0] = atof(argv[i]);
    idx[1] = atof(argv[i + 1]);
    indicesList.push_back(idx);

    i += 2;
    }

  // Instantiating object
  InterpolatorType::Pointer prolate = InterpolatorType::New();
  ReaderType::Pointer       reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->Update();
  prolate->SetInputImage(reader->GetOutput());
  prolate->SetRadius(atoi(argv[6]));
  prolate->Initialize();

  std::ofstream file;
  file.open(outfname);

  for (std::vector<ContinuousIndexType>::iterator it = indicesList.begin(); it != indicesList.end(); ++it)
    {
    file << (*it) << " -> " << prolate->EvaluateAtContinuousIndex((*it)) << std::endl;
    }
  file.close();

  /**********************************************************/
  //typedef otb::ImageFileWriter<ImageType> WriterType;
  //typedef otb::StreamingResampleImageFilter<ImageType, ImageType, double> StreamingResampleImageFilterType;
  typedef otb::ImageFileWriter<ImageType>                        WriterType;
  typedef itk::ResampleImageFilter<ImageType, ImageType, double> StreamingResampleImageFilterType;
  WriterType::Pointer                       prowriter     = WriterType::New();
  StreamingResampleImageFilterType::Pointer proresampler = StreamingResampleImageFilterType::New();
  InterpolatorType::Pointer                 pro     = InterpolatorType::New();
  // Resampler connected to input image
  proresampler->SetInput(reader->GetOutput());
  pro->SetInputImage(reader->GetOutput());
  pro->SetRadius(atoi(argv[6]));
  pro->Initialize();

  proresampler->SetInterpolator(pro);
  StreamingResampleImageFilterType::SizeType size;
  size[0] = 512;
  size[1] = 512;
  double tutu = 1;
  proresampler->SetSize(size);
  proresampler->SetOutputSpacing(tutu);
  // Result of resampler is written
  prowriter->SetInput(proresampler->GetOutput());
  //prowriter->SetNumberOfDivisionsStrippedStreaming(1);
  prowriter->SetFileName(profname);
  prowriter->Update();

  typedef otb::WindowedSincInterpolateImageCosineFunction<ImageType>          CosInterpolatorType;
  typedef itk::Function::CosineWindowFunction<1, double, double>              itkCosType;
  typedef itk::WindowedSincInterpolateImageFunction<ImageType, 1, itkCosType> itkCosInterpolatorType;

  WriterType::Pointer                       itkcoswriter  = WriterType::New();
  WriterType::Pointer                       coswriter = WriterType::New();
  StreamingResampleImageFilterType::Pointer cosresampler = StreamingResampleImageFilterType::New();
  StreamingResampleImageFilterType::Pointer itkcosresampler = StreamingResampleImageFilterType::New();
  CosInterpolatorType::Pointer              cos     = CosInterpolatorType::New();
  itkCosInterpolatorType::Pointer           itkcos  = itkCosInterpolatorType::New();
  cosresampler->SetSize(size);
  cosresampler->SetOutputSpacing(tutu);
  itkcosresampler->SetSize(size);
  itkcosresampler->SetOutputSpacing(tutu);
  cosresampler->SetInput(reader->GetOutput());
  cos->SetInputImage(reader->GetOutput());
  cos->SetRadius(atoi(argv[6]));
  cos->Initialize();
  cosresampler->SetInterpolator(cos);
  itkcosresampler->SetInput(reader->GetOutput());
  itkcosresampler->SetInterpolator(itkcos);
  coswriter->SetInput(cosresampler->GetOutput());
  coswriter->SetFileName(cosfname);
  itkcoswriter->SetInput(itkcosresampler->GetOutput());
  itkcoswriter->SetFileName(itkcosfname);
  coswriter->Update();
  itkcoswriter->Update();

  return EXIT_SUCCESS;
}
