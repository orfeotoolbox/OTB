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
#include "itkExceptionObject.h"

#include "otbProlateInterpolateImageFunction.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include <fstream>
#include "otbStreamingImageFileWriter.h"
#include "otbStreamingResampleImageFilter.h"
#include "otbWindowedSincInterpolateImageGaussianFunction.h"
#include "otbWindowedSincInterpolateImageHammingFunction.h"
#include "otbWindowedSincInterpolateImageCosineFunction.h"
#include "itkWindowedSincInterpolateImageFunction.h"
#include "itkDifferenceImageFilter.h"

int otbProlateInterpolateImageFunction(int argc, char * argv[])
{  
  const char * infname = argv[1];
  const char * outfname = argv[2];

  typedef otb::Image<double,2>                             ImageType;
  typedef otb::ProlateInterpolateImageFunction<ImageType>  InterpolatorType;
  typedef InterpolatorType::ContinuousIndexType            ContinuousIndexType;
  typedef otb::ImageFileReader<ImageType>                  ReaderType;

   unsigned int i = 4;

   std::vector<ContinuousIndexType> indicesList;
  while(i<static_cast<unsigned int>(argc) && (i+1)<static_cast<unsigned int>(argc))
    {  
      ContinuousIndexType idx;
      idx[0]=atof(argv[i]);
      idx[1]=atof(argv[i+1]);     
      indicesList.push_back(idx);

      i+=2;
    }

  // Instantiating object
  InterpolatorType::Pointer prolate = InterpolatorType::New();
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->Update();
  prolate->SetInputImage(reader->GetOutput());
  prolate->SetRadius(atoi(argv[3]));

  std::ofstream file;
  file.open(outfname);


  for(std::vector<ContinuousIndexType>::iterator it = indicesList.begin();it!=indicesList.end();++it)
    {
      file<<(*it)<<" -> "<<prolate->EvaluateAtContinuousIndex((*it))<<std::endl;
    }
  file.close();

  /**********************************************************/
  typedef otb::StreamingImageFileWriter<ImageType> WriterType;
  typedef otb::StreamingResampleImageFilter<ImageType,ImageType,double> StreamingResampleImageFilterType;

  unsigned int rad = 1;

  typedef otb::WindowedSincInterpolateImageGaussianFunction<ImageType>        GaussInterpolatorType;
  typedef otb::WindowedSincInterpolateImageHammingFunction<ImageType>         HamInterpolatorType;
  typedef otb::WindowedSincInterpolateImageCosineFunction<ImageType>          CosInterpolatorType;
  typedef itk::Function::HammingWindowFunction<1, double, double>             itkHamType;
  typedef itk::WindowedSincInterpolateImageFunction<ImageType, 1, itkHamType> itkHamInterpolatorType;
  typedef itk::Function::CosineWindowFunction<1, double, double>              itkCosType;
  typedef itk::WindowedSincInterpolateImageFunction<ImageType, 1, itkCosType> itkCosInterpolatorType;
  typedef itk::DifferenceImageFilter<ImageType, ImageType>                    DiffType;
  // Instantiating object
  WriterType::Pointer prowriter     = WriterType::New();
  WriterType::Pointer gausswriter   = WriterType::New();
  WriterType::Pointer hamwriter     = WriterType::New();
  WriterType::Pointer coswriter     = WriterType::New();
  WriterType::Pointer itkhamwriter  = WriterType::New();
  WriterType::Pointer itkcoswriter  = WriterType::New();
  WriterType::Pointer cosdiffwriter = WriterType::New();
  WriterType::Pointer hamdiffwriter = WriterType::New();

  StreamingResampleImageFilterType::Pointer proresampler = StreamingResampleImageFilterType::New();
  StreamingResampleImageFilterType::Pointer gaussresampler = StreamingResampleImageFilterType::New();
  StreamingResampleImageFilterType::Pointer hamresampler = StreamingResampleImageFilterType::New();
  StreamingResampleImageFilterType::Pointer cosresampler = StreamingResampleImageFilterType::New();
  StreamingResampleImageFilterType::Pointer itkhamresampler = StreamingResampleImageFilterType::New();
  StreamingResampleImageFilterType::Pointer itkcosresampler = StreamingResampleImageFilterType::New();
  DiffType::Pointer                         hamdiff = DiffType::New();
  DiffType::Pointer                         cosdiff = DiffType::New();

  InterpolatorType::Pointer        pro     = InterpolatorType::New();
  HamInterpolatorType::Pointer     ham     = HamInterpolatorType::New();
  CosInterpolatorType::Pointer     cos     = CosInterpolatorType::New();
  itkHamInterpolatorType::Pointer  itkham  = itkHamInterpolatorType::New();
  itkCosInterpolatorType::Pointer  itkcos  = itkCosInterpolatorType::New();
  GaussInterpolatorType::Pointer   gauss   = GaussInterpolatorType::New();

  // Resampler connected to input image
  proresampler->SetInput(reader->GetOutput());
  pro->SetRadius(rad);
  proresampler->SetInterpolator(pro);
  proresampler->SetInterpolatorNeighborhoodRadius(30);  

  gaussresampler->SetInput(reader->GetOutput());
  gauss->SetRadius(rad);
  gaussresampler->SetInterpolator(gauss);
  gaussresampler->SetInterpolatorNeighborhoodRadius(rad); 

  hamresampler->SetInput(reader->GetOutput());
  ham->SetRadius(rad);
  hamresampler->SetInterpolator(ham);
  hamresampler->SetInterpolatorNeighborhoodRadius(rad); 

  cosresampler->SetInput(reader->GetOutput());
  cos->SetRadius(rad);
  cosresampler->SetInterpolator(cos);
  cosresampler->SetInterpolatorNeighborhoodRadius(rad); 

  itkhamresampler->SetInput(reader->GetOutput());
  itkhamresampler->SetInterpolator(itkham);
  itkhamresampler->SetInterpolatorNeighborhoodRadius(rad);
 
  itkcosresampler->SetInput(reader->GetOutput());
  itkcosresampler->SetInterpolator(itkcos);
  itkcosresampler->SetInterpolatorNeighborhoodRadius(rad); 

  // Size of output resampler result
  StreamingResampleImageFilterType::SizeType size;
  size[0]=2000;
  size[1]=2000;
  double tutu = 0.1;
  proresampler->SetSize(size);
  proresampler->SetOutputSpacing(tutu);

  gaussresampler->SetSize(size);
  gaussresampler->SetOutputSpacing(tutu);

  hamresampler->SetSize(size);
  hamresampler->SetOutputSpacing(tutu);

  cosresampler->SetSize(size);
  cosresampler->SetOutputSpacing(tutu);

  itkhamresampler->SetSize(size);
  itkhamresampler->SetOutputSpacing(tutu);

  itkcosresampler->SetSize(size);
  itkcosresampler->SetOutputSpacing(tutu);

  // Result of resampler is written
  prowriter->SetInput(proresampler->GetOutput());
  prowriter->SetNumberOfStreamDivisions(1);
  prowriter->SetFileName("proresample.tif");

  gausswriter->SetInput(gaussresampler->GetOutput());
  gausswriter->SetNumberOfStreamDivisions(1);
  gausswriter->SetFileName("gaussresample.tif");
 
  hamwriter->SetInput(hamresampler->GetOutput());
  hamwriter->SetNumberOfStreamDivisions(1);
  hamwriter->SetFileName("hamresample.tif");
  
  coswriter->SetInput(cosresampler->GetOutput());
  coswriter->SetNumberOfStreamDivisions(1);
  coswriter->SetFileName("hamresample.tif");
  
  itkhamwriter->SetInput(itkhamresampler->GetOutput());
  itkhamwriter->SetNumberOfStreamDivisions(1);
  itkhamwriter->SetFileName("itkhamresample.tif");

  itkcoswriter->SetInput(itkcosresampler->GetOutput());
  itkcoswriter->SetNumberOfStreamDivisions(1);
  itkcoswriter->SetFileName("itkcosresample.tif");

  gausswriter->Update();
  prowriter->Update();
  hamwriter->Update();
  coswriter->Update();
  itkhamwriter->Update();
  itkcoswriter->Update();
  
  cosdiff->SetInput(0, cosresampler->GetOutput());
  cosdiff->SetInput(1, itkcosresampler->GetOutput());
  cosdiffwriter->SetNumberOfStreamDivisions(1);
  cosdiffwriter->SetFileName("cosdiff.tif");
  cosdiffwriter->SetInput(cosdiff->GetOutput());
  
  hamdiff->SetInput(0, hamresampler->GetOutput());
  hamdiff->SetInput(1, itkhamresampler->GetOutput());
  hamdiffwriter->SetNumberOfStreamDivisions(1);
  hamdiffwriter->SetFileName("hamdiff.tif");
  hamdiffwriter->SetInput(hamdiff->GetOutput());

  cosdiffwriter->Update();
  hamdiffwriter->Update();


  return EXIT_SUCCESS;
}
