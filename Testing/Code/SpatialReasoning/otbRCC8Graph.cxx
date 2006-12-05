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
#include "otbRCC8VertexIterator.h"
#include "otbRCC8EdgeIterator.h"

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
      const unsigned int nbVertices = 3;
      typedef unsigned short  LabelType;
      typedef otb::RCC8VertexBase<LabelType> VertexType;
      typedef otb::RCC8Graph<VertexType> RCC8GraphType;
      typedef RCC8GraphType::EdgeType EdgeType;
      typedef otb::RCC8VertexIterator<RCC8GraphType> VertexIteratorType;
      typedef otb::RCC8EdgeIterator<RCC8GraphType>   EdgeIteratorType;
    
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
      VertexType::Pointer vertex3 = VertexType::New();
      vertex1->SetSegmentationImageIndex(0);
      vertex1->SetObjectLabelInImage(1);
      vertex2->SetSegmentationImageIndex(1);
      vertex2->SetObjectLabelInImage(2);
      vertex3->SetSegmentationImageIndex(2);
      vertex3->SetObjectLabelInImage(3);

      rcc8Graph->SetVertex(0,vertex1);
      rcc8Graph->SetVertex(1,vertex2);
      rcc8Graph->SetVertex(2,vertex3);
      fail(rcc8Graph->GetVertex(0)->GetSegmentationImageIndex()!=0,
	   "rcc8Graph->GetVertex(0)->GetSegmentationImageIndex()!=0");
      fail(rcc8Graph->GetVertex(0)->GetObjectLabelInImage()!=1,
	   "rcc8Graph->GetVertex(0)->GetObjectLabelInImage()!=1");
      fail(rcc8Graph->GetVertex(1)->GetSegmentationImageIndex()!=1,
	   "rcc8Graph->GetVertex(1)->GetSegmentationImageIndex()!=1");
      fail(rcc8Graph->GetVertex(1)->GetObjectLabelInImage()!=2,
	   "rcc8Graph->GetVertex(1)->GetObjectLabelInImgage()!=2");
      fail(rcc8Graph->GetVertex(2)->GetSegmentationImageIndex()!=2,
	   "rcc8Graph->GetVertex(2)->GetSegmentationImageIndex()!=2");
      fail(rcc8Graph->GetVertex(2)->GetObjectLabelInImage()!=3,
	   "rcc8Graph->GetVertex(2)->GetObjectLabelInImgage()!=3");

      // Testing the vertex iterators
      int i=0;
      VertexIteratorType v(rcc8Graph);
      for(v.GoToBegin();!v.IsAtEnd();++v,i++)
	{
	  fail(v.Get()->GetSegmentationImageIndex()!=i,
	       "v.Get()->GetSegmentationImageIndex()!=i");
	  fail(v.Get()->GetObjectLabelInImage()!=(i+1),
	       "v.Get()->GetSegmentationImageIndex()!=i");
	}
    
      // Testing the edge iterator
      rcc8Graph->AddEdge(0,1,otb::OTB_RCC8_NTPPI);
      rcc8Graph->AddEdge(1,2,otb::OTB_RCC8_EC);

      fail(rcc8Graph->GetNumberOfEdges()!=2,
	   "rcc8Graph->GetNumberOfEdges()!=2");

      i = 0;
      EdgeIteratorType e(rcc8Graph);
      for(e.GoToBegin();!e.IsAtEnd();++e,i++)
	{
	  if(i==0)
	    {
	      fail(e.GetValue()!=otb::OTB_RCC8_NTPPI,
		   "e.GetValue()!=otb::OTB_RCC8_NTPPI");
	      fail(e.GetSourceIndex()!=0,
		   "e.GetSourceIndex()!=0");
	      fail(e.GetTargetIndex()!=1,
		   "e.GetTargetIndex()!=1");
	    }
	  else if(i==1)
	    {
	      fail(e.GetValue()!=otb::OTB_RCC8_EC,
		   "e.GetValue()!=otb::OTB_RCC8_EC");
	      fail(e.GetSourceIndex()!=1,
		   "e.GetSourceIndex()!=1");
	      fail(e.GetTargetIndex()!=2,
		   "e.GetTargetIndex()!=2");
	    }
	  else
	    {
	      fail(true,"Edge iterator out of bound."); 
	    }
	}
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
