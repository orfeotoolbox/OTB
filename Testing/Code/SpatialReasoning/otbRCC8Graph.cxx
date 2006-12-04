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

bool fail(bool test, char * reason)
{
  if(test)
    {
      std::cout<<"Test failed: "<<reason<<std::endl;
      return EXIT_FAILURE;
    }
}

int otbRCC8Graph(int argc, char* argv[])
{
try
  {
    const unsigned int Dimension = 2;
    unsigned int nbVertices = atoi(argv[1]);
   
    typedef unsigned short  LabelType;
    typedef otb::RCC8VertexBase<LabelType> VertexType;
    typedef otb::RCC8Graph<VertexType> RCC8GraphType;
    typedef RCC8GraphType::EdgeType EdgeType;
    
    // Instantiation
    RCC8GraphType::Pointer rcc8Graph = RCC8GraphType::New();
    // Setting the number of vertices
    rcc8Graph->SetNumberOfVertices(nbVertices);
    // Call to the build method
    rcc8Graph->Build();
    // Testing the number of vertices getter
    fail(rcc8Graph->GetNumberOfVertices()!=nbVertices,
	 "rcc8Graph->GetNumberOfVertices()!=nbVertices");
    // Testing the set vertex method
    VertexType::Pointer vertex1 = VertexType::New();    
    VertexType::Pointer vertex2 = VertexType::New();
    vertex1->SetSegmentationImageIndex(0);
    vertex1->SetObjectLabelInImage(1);
    vertex2->SetSegmentationImageIndex(1);
    vertex2->SetObjectLabelInImage(2);
    rcc8Graph->SetVertex(0,vertex1);
    rcc8Graph->SetVertex(1,vertex2);
    fail(rcc8Graph->GetVertex(0)->GetSegmentationImageIndex()!=0,
	 "rcc8Graph->GetVertex(0)->GetSegmentationImageIndex()!=0");
    fail(rcc8Graph->GetVertex(0)->GetObjectLabelInImage()!=1,
	 "rcc8Graph->GetVertex(0)->GetObjectLabelInImage()!=1");
    fail(rcc8Graph->GetVertex(1)->GetSegmentationImageIndex()!=1,
	 "rcc8Graph->GetVertex(1)->GetSegmentationImageIndex()!=1");
    fail(rcc8Graph->GetVertex(1)->GetObjectLabelInImage()!=2,
	 "rcc8Graph->GetVertex(1)->GetObjectLabelInImgage()!=2");
    EdgeType::Pointer edge = EdgeType::New();
    edge->SetValue(otb::OTB_RCC8_NTPPI);
    rcc8Graph->AddEdge(0,1,edge);
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
