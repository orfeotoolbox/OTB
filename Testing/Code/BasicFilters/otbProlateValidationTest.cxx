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
#include "otbImageFileWriter.h"
#include "itkResampleImageFilter.h"
#include "otbStreamingImageFileWriter.h"
#include "otbStreamingResampleImageFilter.h"
#include "otbWindowedSincInterpolateImageGaussianFunction.h"
#include "otbWindowedSincInterpolateImageHammingFunction.h"
#include "otbWindowedSincInterpolateImageCosineFunction.h"
#include "itkWindowedSincInterpolateImageFunction.h"
#include "itkDifferenceImageFilter.h"

int otbProlateValidationTest(int argc, char * argv[])
{  
  const char * infname = argv[1];
  const char * outfname = argv[2];
  const unsigned int rad = atoi(argv[3]);
  const float spacing = atof(argv[4]);

  typedef otb::Image<double,2>                             ImageType;
  typedef otb::ImageFileReader<ImageType>                  ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;
  typedef itk::ResampleImageFilter<ImageType,ImageType,double> StreamingResampleImageFilterType;

  typedef otb::ProlateInterpolateImageFunction<ImageType>  InterpolatorType;


  InterpolatorType::Pointer prolate = InterpolatorType::New();
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer prowriter     = WriterType::New();
  StreamingResampleImageFilterType::Pointer proresampler = StreamingResampleImageFilterType::New();

  reader->SetFileName(infname);
  prolate->SetInputImage(reader->GetOutput());
  prolate->SetRadius(rad);
  proresampler->SetInput(reader->GetOutput());
  proresampler->SetInterpolator(prolate);
  StreamingResampleImageFilterType::SizeType size;
  size[0]=atoi(argv[5]);
  size[1]=atoi(argv[5]);
  proresampler->SetSize(size);
  proresampler->SetOutputSpacing(spacing);

  prowriter->SetInput(proresampler->GetOutput());
  prowriter->SetFileName(outfname);
  prowriter->Update();

 

  /**********************************************************/
  //typedef otb::StreamingImageFileWriter<ImageType> WriterType;
  //typedef otb::StreamingResampleImageFilter<ImageType,ImageType,double> StreamingResampleImageFilterType;
 
 /*
 typedef otb::WindowedSincInterpolateImageCosineFunction<ImageType>          CosInterpolatorType;
 typedef itk::Function::CosineWindowFunction<1, double, double>              itkCosType;
 typedef itk::WindowedSincInterpolateImageFunction<ImageType, 1, itkCosType> itkCosInterpolatorType;
 
 WriterType::Pointer itkcoswriter  = WriterType::New();
 WriterType::Pointer coswriter = WriterType::New();
 StreamingResampleImageFilterType::Pointer cosresampler = StreamingResampleImageFilterType::New();
 StreamingResampleImageFilterType::Pointer itkcosresampler = StreamingResampleImageFilterType::New();
 CosInterpolatorType::Pointer     cos     = CosInterpolatorType::New();
 itkCosInterpolatorType::Pointer  itkcos  = itkCosInterpolatorType::New();
 cosresampler->SetSize(size);
 cosresampler->SetOutputSpacing(tutu);
 itkcosresampler->SetSize(size);
 itkcosresampler->SetOutputSpacing(tutu);
 cosresampler->SetInput(reader->GetOutput());
 cos->SetRadius(atoi(argv[3]));
 cosresampler->SetInterpolator(cos);
 itkcosresampler->SetInput(reader->GetOutput());
 itkcosresampler->SetInterpolator(itkcos);
 coswriter->SetInput(cosresampler->GetOutput());
 coswriter->SetFileName("cosresample.tif");
 itkcoswriter->SetInput(itkcosresampler->GetOutput());
 itkcoswriter->SetFileName("itkcosresample.tif");
 coswriter->Update();
 itkcoswriter->Update();
 */
  /*
  unsigned int rad = 10;
  
  typedef otb::WindowedSincInterpolateImageGaussianFunction<ImageType>        GaussInterpolatorType;
  typedef otb::WindowedSincInterpolateImageHammingFunction<ImageType>         HamInterpolatorType;
  typedef otb::WindowedSincInterpolateImageCosineFunction<ImageType>          CosInterpolatorType;
  typedef itk::Function::HammingWindowFunction<10, double, double>             itkHamType;
  typedef itk::WindowedSincInterpolateImageFunction<ImageType, 10, itkHamType> itkHamInterpolatorType;
  typedef itk::Function::CosineWindowFunction<10, double, double>              itkCosType;
  typedef itk::WindowedSincInterpolateImageFunction<ImageType, 10, itkCosType> itkCosInterpolatorType;
  typedef itk::DifferenceImageFilter<ImageType, ImageType>                    DiffType;
  */
  // Instantiating object

  /*
  WriterType::Pointer gausswriter   = WriterType::New();
  WriterType::Pointer hamwriter     = WriterType::New();
  WriterType::Pointer coswriter     = WriterType::New();
  WriterType::Pointer itkhamwriter  = WriterType::New();
  WriterType::Pointer itkcoswriter  = WriterType::New();
  WriterType::Pointer cosdiffwriter = WriterType::New();
  WriterType::Pointer hamdiffwriter = WriterType::New();
  */

  /*
  StreamingResampleImageFilterType::Pointer gaussresampler = StreamingResampleImageFilterType::New();
  StreamingResampleImageFilterType::Pointer hamresampler = StreamingResampleImageFilterType::New();
  StreamingResampleImageFilterType::Pointer cosresampler = StreamingResampleImageFilterType::New();
  StreamingResampleImageFilterType::Pointer itkhamresampler = StreamingResampleImageFilterType::New();
  StreamingResampleImageFilterType::Pointer itkcosresampler = StreamingResampleImageFilterType::New();
  DiffType::Pointer                         hamdiff = DiffType::New();
  DiffType::Pointer                         cosdiff = DiffType::New();
  */


  /*
  HamInterpolatorType::Pointer     ham     = HamInterpolatorType::New();
  CosInterpolatorType::Pointer     cos     = CosInterpolatorType::New();
  itkHamInterpolatorType::Pointer  itkham  = itkHamInterpolatorType::New();
  itkCosInterpolatorType::Pointer  itkcos  = itkCosInterpolatorType::New();
  GaussInterpolatorType::Pointer   gauss   = GaussInterpolatorType::New();
  */

  //proresampler->SetInterpolatorNeighborhoodRadius(rad);  
  /*
  gaussresampler->SetInput(reader->GetOutput());
  gauss->SetRadius(rad);
  gaussresampler->SetInterpolator(gauss);
  //gaussresampler->SetInterpolatorNeighborhoodRadius(rad); 

  hamresampler->SetInput(reader->GetOutput());
  ham->SetRadius(rad);
  hamresampler->SetInterpolator(ham);
  //hamresampler->SetInterpolatorNeighborhoodRadius(rad); 

  cosresampler->SetInput(reader->GetOutput());
  cos->SetRadius(rad);
  cosresampler->SetInterpolator(cos);
  //cosresampler->SetInterpolatorNeighborhoodRadius(30); 

  itkhamresampler->SetInput(reader->GetOutput());
  itkhamresampler->SetInterpolator(itkham);
  //itkhamresampler->SetInterpolatorNeighborhoodRadius(rad);
 
  itkcosresampler->SetInput(reader->GetOutput());
  itkcosresampler->SetInterpolator(itkcos);
  //itkcosresampler->SetInterpolatorNeighborhoodRadius(30); 
  */
  // Size of output resampler result
 
  /*
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
  */

  /*
  gausswriter->SetInput(gaussresampler->GetOutput());
  //gausswriter->SetNumberOfStreamDivisions(1);
  gausswriter->SetFileName("gaussresample.tif");
 
  hamwriter->SetInput(hamresampler->GetOutput());
  //hamwriter->SetNumberOfStreamDivisions(1);
  hamwriter->SetFileName("hamresample.tif");
  
  coswriter->SetInput(cosresampler->GetOutput());
  //coswriter->SetNumberOfStreamDivisions(1);
  coswriter->SetFileName("cosresample.tif");
  
  itkhamwriter->SetInput(itkhamresampler->GetOutput());
  //itkhamwriter->SetNumberOfStreamDivisions(1);
  itkhamwriter->SetFileName("itkhamresample.tif");

  itkcoswriter->SetInput(itkcosresampler->GetOutput());
  //itkcoswriter->SetNumberOfStreamDivisions(1);
  itkcoswriter->SetFileName("itkcosresample.tif");

  gausswriter->Update();
  hamwriter->Update();
  coswriter->Update();
  itkhamwriter->Update();
  itkcoswriter->Update();
  
  cosdiff->SetInput(0, cosresampler->GetOutput());
  cosdiff->SetInput(1, itkcosresampler->GetOutput());
  //cosdiffwriter->SetNumberOfStreamDivisions(1);
  cosdiffwriter->SetFileName("cosdiff.tif");
  cosdiffwriter->SetInput(cosdiff->GetOutput());
  
  hamdiff->SetInput(0, hamresampler->GetOutput());
  hamdiff->SetInput(1, itkhamresampler->GetOutput());
  //hamdiffwriter->SetNumberOfStreamDivisions(1);
  hamdiffwriter->SetFileName("hamdiff.tif");
  hamdiffwriter->SetInput(hamdiff->GetOutput());

  cosdiffwriter->Update();
  hamdiffwriter->Update();
  */
 

  return EXIT_SUCCESS;
}
