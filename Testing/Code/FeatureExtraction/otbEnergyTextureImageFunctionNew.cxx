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

//#include "otbFunctionWithNeighborhoodToImageFilter.h"
#include "otbEnergyTextureImageFunction.h"
#include "otbImage.h"
//#include "otbImageFileReader.h"
//#include "otbImageFileWriter.h"


int otbEnergyTextureImageFunctionNew(int argc, char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];

  typedef double InputPixelType;
  const int Dimension = 2;
  typedef otb::Image<InputPixelType,Dimension> ImageType;
  typedef ImageType::PixelType PixelType;
  typedef ImageType::OffsetType OffsetType;
  //  typedef otb::ImageFileReader<ImageType>  ReaderType;
  //  typedef otb::ImageFileWriter<ImageType> WriterType;

  //  typedef itk::ConstNeighborhoodIterator<ImageType>   IterType;;
  //  typedef otb::Functor::EnergyTextureFunctor<IterType, IterType, PixelType>  FunctorType;
  typedef otb::EnergyTextureImageFunction<ImageType> EnergyTextureImageFunctionType;
  
  EnergyTextureImageFunctionType::Pointer energy = EnergyTextureImageFunctionType::New();

  // Instantiating object
  /*
  UnaryFunctorNeighborhoodImageFilterType::Pointer object = UnaryFunctorNeighborhoodImageFilterType::New();
  ReaderType::Pointer reader  = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);

  object->SetInput(reader->GetOutput());
  object->SetRadius(atoi(argv[3]));
  OffsetType offset;
  offset[0] =  atoi(argv[4]);
  offset[1] =  atoi(argv[5]);

  object->SetOffset(offset);
  writer->SetInput(object->GetOutput());

  writer->Update();
  */

  return EXIT_SUCCESS;
}
