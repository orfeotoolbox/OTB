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
#include "otbRCC8VertexBase.h"
#include "otbRCC8GraphFileWriter.h"

int otbRCC8GraphFileWriter(int argc, char* argv[])
{
try
  {
    const char * outputFile = argv[1];
    
    typedef unsigned char  PixelType;
    typedef otb::RCC8VertexBase<PixelType> VertexType;
    typedef otb::RCC8Graph<VertexType> RCC8GraphType;
    typedef otb::RCC8GraphFileWriter<RCC8GraphType> RCC8GraphFileWriterType;
    
    // Graph instantiation
    RCC8GraphType::Pointer rcc8Graph = RCC8GraphType::New();
    rcc8Graph->SetNumberOfVertices(4);
    rcc8Graph->Build();
    
    // Vertex filling
    VertexType::Pointer vertex1, vertex2, vertex3, vertex4;
    vertex1 = VertexType::New();
    vertex1->SetSegmentationImageIndex(0);
    vertex1->SetObjectLabelInImage(0);
    rcc8Graph->SetVertex(0,vertex1);

    vertex2 = VertexType::New();
    vertex2->SetSegmentationImageIndex(1);
    vertex2->SetObjectLabelInImage(1);
    rcc8Graph->SetVertex(1,vertex2);
    vertex1 = VertexType::New();

    vertex3 = VertexType::New();
    vertex3->SetSegmentationImageIndex(2);
    vertex3->SetObjectLabelInImage(2);
    rcc8Graph->SetVertex(2,vertex3);

    vertex4 = VertexType::New();
    vertex4->SetSegmentationImageIndex(3);
    vertex4->SetObjectLabelInImage(3);
    rcc8Graph->SetVertex(3,vertex4);

    // Edge filling
    rcc8Graph->AddEdge(0,1,otb::OTB_RCC8_EC);
    rcc8Graph->AddEdge(1,2,otb::OTB_RCC8_PO);
    rcc8Graph->AddEdge(2,3,otb::OTB_RCC8_TPP);
    rcc8Graph->AddEdge(0,2,otb::OTB_RCC8_TPPI);
    rcc8Graph->AddEdge(1,3,otb::OTB_RCC8_NTPP);
    rcc8Graph->AddEdge(0,3,otb::OTB_RCC8_NTPPI);

    // Instantiation
    RCC8GraphFileWriterType::Pointer rcc8GraphWriter 
      = RCC8GraphFileWriterType::New();
    rcc8GraphWriter->SetFileName(outputFile);
    rcc8GraphWriter->SetInput(rcc8Graph);
    rcc8GraphWriter->SetWriteSegmentationImages(false);
    rcc8GraphWriter->Update();
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

