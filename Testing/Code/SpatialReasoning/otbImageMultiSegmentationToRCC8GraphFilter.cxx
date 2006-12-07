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
#include "otbRCC8Graph.h"
#include "otbImage.h"
#include "otbImageList.h"
#include "otbRCC8VertexBase.h"
#include "otbImageMultiSegmentationToRCC8GraphFilter.h"
#include "otbImageFileReader.h"
#include "otbRCC8GraphFileWriter.h"

int otbImageMultiSegmentationToRCC8GraphFilter(int argc, char* argv[])
{
try 
  {
    const unsigned int Dimension = 2;
    char * outputFilename = argv[1];
    unsigned int nbImages = atoi(argv[2]);
    
    // typedefs
    typedef unsigned short LabelPixelType;
    typedef otb::Image<LabelPixelType,Dimension> LabelImageType;
    typedef otb::RCC8VertexBase<LabelPixelType> VertexType;
    typedef otb::RCC8Graph<VertexType> RCC8GraphType;
    typedef otb::ImageMultiSegmentationToRCC8GraphFilter<LabelImageType,RCC8GraphType>
      RCC8GraphFilterType;
    typedef otb::ImageList<LabelImageType> ImageListType;
    typedef otb::ImageFileReader<LabelImageType> ReaderType;
    typedef otb::RCC8GraphFileWriter<RCC8GraphType> GraphWriterType;

    ImageListType::Pointer inputList = ImageListType::New();

    // Reading input images
    for(int i=3;i<nbImages+2;i++)
      {
	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFilename(argv[i]);
	reader->Update();
	inputList->PushBack(reader->GetOutput());
      }

    // Instanatiation
    RCC8GraphFilterType::Pointer filter = RCC8GraphFilterType::New();
    filter->SetInput(inputList);
    
    // Writing output graph
    GraphWriterType::Pointer writer = GraphWriterType::New();
    writer->SetFilename(outputFilename);
    writer->SetInput(filter->GetOutput());
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
