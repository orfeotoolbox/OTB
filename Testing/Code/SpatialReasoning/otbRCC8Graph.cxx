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
#include "otbRCC8InEdgeIterator.h"
#include "otbRCC8OutEdgeIterator.h"

void fail(bool test, char * reason)
{
  if(test)
    {
      std::cout<<"Test failed: "<<reason<<std::endl;
      exit(-1);
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
      typedef otb::RCC8InEdgeIterator<RCC8GraphType> InEdgeIteratorType;
      typedef otb::RCC8OutEdgeIterator<RCC8GraphType> OutEdgeIteratorType;
    
      // Instantiation
      RCC8GraphType::Pointer rcc8Graph = RCC8GraphType::New();
      // Setting the number of vertices
      rcc8Graph->SetNumberOfVertices(nbVertices-1);
      // Call to the build method
      rcc8Graph->Build();
      // Testing the number of vertices getter
      fail(rcc8Graph->GetNumberOfVertices()!=nbVertices-1,
	   "rcc8Graph->GetNumberOfVertices()!=nbVertices-1");
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
      fail(rcc8Graph->GetNumberOfVertices()!=nbVertices,
	   "rcc8Graph->GetNumberOfVertices()!=nbVertices");
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

      // Adding vertices and edges to test the in and out iterators
      VertexType::Pointer vertex4 = VertexType::New();
      VertexType::Pointer vertex5 = VertexType::New();
      vertex4->SetSegmentationImageIndex(3);
      vertex4->SetObjectLabelInImage(3);
      vertex5->SetSegmentationImageIndex(4);
      vertex5->SetObjectLabelInImage(4);
      rcc8Graph->SetVertex(3,vertex4);
      rcc8Graph->SetVertex(4,vertex5);
      rcc8Graph->AddEdge(3,1,otb::OTB_RCC8_NTPP);
      rcc8Graph->AddEdge(1,4,otb::OTB_RCC8_PO);
      
      // Testing the in edge iterator
      int vertexIndex=1;
      InEdgeIteratorType inEdgeIt(vertexIndex,rcc8Graph);
      for(inEdgeIt.GoToBegin();!inEdgeIt.IsAtEnd();++inEdgeIt)
	{
	  std::cout<<"testing inEdgeIt"<<std::endl;
	  fail(!((inEdgeIt.GetSourceIndex()==0)||(inEdgeIt.GetSourceIndex()==3)),
	       "!((inEdgeIt.GetSourceIndex()==0)||(inEdgeIt.GetSourceIndex()==3))");
	}
      // Testing the out edge iterator
      OutEdgeIteratorType outEdgeIt(vertexIndex,rcc8Graph);
      for(outEdgeIt.GoToBegin();!outEdgeIt.IsAtEnd();++outEdgeIt)
	{
	  std::cout<<"testing outEdgeIt"<<std::endl;
	  fail(!((outEdgeIt.GetTargetIndex()==2)||(outEdgeIt.GetTargetIndex()==4)),
	       "!((outEdgeIt.GetTargetIndex()==2)||(outEdgeIt.GetTargetIndex()==4))");
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
