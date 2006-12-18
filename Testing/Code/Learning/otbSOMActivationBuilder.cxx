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
#include "otbImage.h"
#include "otbSOMMap.h"
#include "otbSOMActivationBuilder.h"
#include "itkRGBPixel.h"
#include "itkEuclideanDistance.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageToListAdaptor.h"

int otbSOMActivationBuilder(int argc, char* argv[])
{
try
  {
    const unsigned int Dimension = 2;
    char * vectorSetFileName = argv[1];
    char * mapFileName = argv[2];
    char * outputFileName = argv[3];
  
    typedef float ComponentType;
    typedef unsigned char OutputPixelType;
    typedef itk::RGBPixel<ComponentType> PixelType;
    typedef itk::Statistics::EuclideanDistance<PixelType> DistanceType;

    typedef otb::SOMMap<PixelType,DistanceType,Dimension> MapType;
    typedef otb::ImageFileReader<MapType> MapReaderType;

    typedef otb::Image<PixelType,Dimension> InputImageType;
    typedef otb::ImageFileReader<InputImageType> ReaderType;
    typedef itk::Statistics::ImageToListAdaptor<InputImageType> AdaptorType;

    typedef otb::Image<OutputPixelType,Dimension> OutputImageType;
    typedef otb::ImageFileWriter<OutputImageType> WriterType;

    typedef otb::SOMActivationBuilder<AdaptorType,MapType,OutputImageType> SOMActivationBuilderType;

    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(vectorSetFileName);    
    reader->Update();
    AdaptorType::Pointer adaptor = AdaptorType::New();
    adaptor->SetImage(reader->GetOutput());
    
    MapReaderType::Pointer mapReader = MapReaderType::New();
    mapReader->SetFileName(mapFileName);

    SOMActivationBuilderType::Pointer somAct = SOMActivationBuilderType::New();
    somAct->SetInput(mapReader->GetOutput());
    somAct->SetListSample(adaptor);

    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(outputFileName);
    writer->SetInput(somAct->GetOutput());
    writer->Update();
  }
catch( itk::ExceptionObject & err ) 
  { 
    std::cout << "Exception itk::ExceptionObject thrown !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
  } 
catch( ... ) 
  { 
    std::cout << "Unknown exception thrown !" << std::endl; 
    return EXIT_FAILURE;
  } 
 return EXIT_SUCCESS;
}
