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
#include "otbSOM.h"
#include "itkRGBPixel.h"
#include "itkEuclideanDistance.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageToListAdaptor.h"

int otbSOM(int argc, char* argv[])
{
try
  {
    const unsigned int Dimension = 2;
    char * inputFileName = argv[1];
    char * outputFileName = argv[2];
    unsigned int sizeX = atoi(argv[3]);
    unsigned int sizeY = atoi(argv[4]);
    unsigned int neighInitX = atoi(argv[5]);
    unsigned int neighInitY= atoi(argv[6]);
    unsigned int nbIterations= atoi(argv[7]);
    double betaInit = atof(argv[8]);
    double betaEnd= atof(argv[9]);
    double initValue = atof(argv[10]);
    

    typedef double ComponentType;
    typedef itk::RGBPixel<ComponentType> PixelType;
    typedef itk::Statistics::EuclideanDistance<PixelType> DistanceType;
    typedef otb::SOMMap<PixelType,DistanceType,Dimension> MapType;
    typedef otb::Image<PixelType,Dimension> ImageType;
    typedef otb::ImageFileReader<ImageType> ReaderType;
    typedef itk::Statistics::ImageToListAdaptor<ImageType> ListAdaptorType;
    
    typedef otb::SOM<ListAdaptorType,MapType> SOMType;
    typedef otb::ImageFileWriter<MapType> WriterType;

    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(inputFileName);
    reader->Update();

    ListAdaptorType::Pointer adaptor = ListAdaptorType::New();
    adaptor->SetImage(reader->GetOutput());
    
    // Instantiation
    SOMType::Pointer som = SOMType::New();
    som->SetListSample(adaptor);
    SOMType::SizeType size;
    size[0]=sizeX;
    size[1]=sizeY;
    som->SetMapSize(size);
    SOMType::SizeType radius;
    radius[0] = neighInitX;
    radius[1] = neighInitY;
    som->SetNeighborhoodSizeInit(radius);
    som->SetNumberOfIterations(nbIterations);
    som->SetBetaInit(betaInit);
    som->SetBetaEnd(betaEnd);
    som->SetMaxWeight(initValue);
    som->SetRandomInit(false);

    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(outputFileName);
    writer->SetInput(som->GetOutput());
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
