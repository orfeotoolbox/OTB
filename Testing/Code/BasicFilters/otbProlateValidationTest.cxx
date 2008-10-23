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
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include <fstream>
#include "otbStreamingImageFileWriter.h"
#include "otbStreamingResampleImageFilter.h"


int otbProlateValidationTest(int argc, char * argv[])
{  
  const char * infname = argv[1];
  const char * outfname = argv[2];
  const unsigned int rad = atoi(argv[3]);
  const double factor= atof(argv[4]);
  //const char * defaultoutfname = argv[6];

  typedef otb::Image<double,2>                             ImageType;
  typedef otb::ImageFileReader<ImageType>                  ReaderType;
  typedef otb::StreamingImageFileWriter<ImageType> WriterType;
  typedef otb::StreamingResampleImageFilter<ImageType,ImageType,double> StreamingResampleImageFilterType;

  typedef otb::ProlateInterpolateImageFunction<ImageType>  InterpolatorType;
  //typedef InterpolatorType::FunctionType FunctionType;
  //typedef itk::NearestNeighborInterpolateImageFunction<ImageType,double> DefaultInterpolatorType;
  //DefaultInterpolatorType::Pointer def = DefaultInterpolatorType::New();

  InterpolatorType::Pointer                 prolate      = InterpolatorType::New();
  ReaderType::Pointer                       reader       = ReaderType::New();
  WriterType::Pointer                       prowriter    = WriterType::New();
  StreamingResampleImageFilterType::Pointer proresampler = StreamingResampleImageFilterType::New();

  reader->SetFileName(infname);
  reader->UpdateOutputInformation();

  ImageType::PointType origin = reader->GetOutput()->GetOrigin();
  ImageType::SpacingType spacing = reader->GetOutput()->GetSpacing();
  ImageType::SpacingType newSpacing;
  newSpacing[0]=spacing[0]*factor;
  newSpacing[1]=spacing[1]*factor;

  prolate->SetInputImage(reader->GetOutput());
  prolate->SetRadius(rad);

 
  std::cout<<"Originalprofilsize: "<< prolate->GetFunction().GetOriginalProfileSize()<<std::endl;
  std::cout<<"Energy : "<<prolate->GetFunction().ComputeEnergy(1/factor)<<std::endl;
  std::cout<<"Radius : "<<prolate->GetRadius()<<std::endl;
  std::cout<<"Factor : "<<factor<<std::endl;

  /*
    otb::Function::ProlateFunction<double,double> function; ///= prolate->GetFunction();
    function.SetRadius(rad);
    std::cout<<"Originalprofilsize: "<< function.GetOriginalProfileSize()<<std::endl;
    std::cout<<"Energy : "<<function.ComputeEnergy(factor)<<std::endl;
    std::cout<<"Radius : "<<function.GetRadius()<<std::endl;
    std::cout<<"Factor : "<<factor<<std::endl;
  */
  proresampler->SetInput(reader->GetOutput());
  proresampler->SetInterpolator(prolate);
  StreamingResampleImageFilterType::SizeType size;
  size[0]=atoi(argv[5]);
  size[1]=atoi(argv[5]);
  proresampler->SetSize(size);
  proresampler->SetOutputOrigin(origin);
  proresampler->SetOutputSpacing(newSpacing);

  prowriter->SetInput(proresampler->GetOutput());
  prowriter->SetFileName(outfname);
  prowriter->Update();

  /*
  prowriter     = WriterType::New();
  proresampler = StreamingResampleImageFilterType::New();
  proresampler->SetSize(size);
  proresampler->SetOutputSpacing(newSpacing);
  proresampler->SetOutputOrigin(origin);
  proresampler->SetInput(reader->GetOutput());
  def->SetInputImage(reader->GetOutput());
  proresampler->SetInterpolator(def);
  prowriter->SetInput(proresampler->GetOutput());
  prowriter->SetFileName(defaultoutfname);
  prowriter->Update();
  */
  return EXIT_SUCCESS;
}
