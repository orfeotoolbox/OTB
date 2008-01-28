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
#include <fstream>
#include "otbPolygon.h"

int otbPolygon(int argc, char * argv[])
{
  try
    {
      typedef otb::Polygon<> PolygonType;
      typedef PolygonType::ContinuousIndexType ContinuousIndexType;
      typedef PolygonType::VertexListType VertexListType;
      typedef VertexListType::ConstIterator IteratorType;


      // Instantiating object
      PolygonType::Pointer polygon1 = PolygonType::New();
      PolygonType::Pointer polygon2 = PolygonType::New();
      
      const char * outfile = argv[1];

      // Reading vertices from command line
      int cpt = 2;
      bool first = true; 
      while ( argv[cpt] != NULL && argv[cpt+1]!= NULL)
      {
	if(argv[cpt][0]=='|')
	  {
	    first = false;
	    ++cpt;
	  }
	else
	  {
	    ContinuousIndexType newVertex; 
	    newVertex[0]=atof(argv[cpt]);
	    newVertex[1]=atof(argv[cpt+1]);
	    if(first)
	      polygon1->AddVertex(newVertex);
	    else
	      polygon2->AddVertex(newVertex);
	    ++cpt;
	    ++cpt;
	  }
      }

      
      IteratorType begin1 = polygon1->GetVertexList()->Begin();
      IteratorType end1   = polygon1->GetVertexList()->End();
      IteratorType begin2 = polygon2->GetVertexList()->Begin();
      IteratorType end2   = polygon2->GetVertexList()->End();
      IteratorType it;
      ContinuousIndexType current,firstVertex;
      
      std::ofstream file;
      file.open(outfile);

      for(it=begin1;it!=end1;++it)
	{
	  file<<"polygon1->IsInside("<<it.Value()<<") = "<<polygon1->IsInside(it.Value())<<std::endl;
	  file<<"polygon1->IsOnEdge("<<it.Value()<<") = "<<polygon1->IsOnEdge(it.Value())<<std::endl;
	  file<<"polygon2->IsInside("<<it.Value()<<") = "<<polygon2->IsInside(it.Value())<<std::endl;
	  file<<"polygon2->IsOnEdge("<<it.Value()<<") = "<<polygon2->IsOnEdge(it.Value())<<std::endl; 
	}
      file<<std::endl<<std::endl;
      for(it=begin2;it!=end2;++it)
	{
	  file<<"polygon1->IsInside("<<it.Value()<<") = "<<polygon1->IsInside(it.Value())<<std::endl;
	  file<<"polygon1->IsOnEdge("<<it.Value()<<") = "<<polygon1->IsOnEdge(it.Value())<<std::endl;
	  file<<"polygon2->IsInside("<<it.Value()<<") = "<<polygon2->IsInside(it.Value())<<std::endl;
	  file<<"polygon2->IsOnEdge("<<it.Value()<<") = "<<polygon2->IsOnEdge(it.Value())<<std::endl; 
	}
      file<<std::endl<<std::endl;

      current = begin1.Value();
      firstVertex=current;
      ++begin1;
      for(it=begin1;it!=end1;++it)
	{
	  file<<"polygon1->NbCrossing("<<current<<", "<<it.Value()<<") = "<<polygon1->NbCrossing(current,it.Value())<<std::endl;
	  file<<"polygon1->NbTouching("<<current<<", "<<it.Value()<<") = "<<polygon1->NbTouching(current,it.Value())<<std::endl;
	  file<<"polygon2->NbCrossing("<<current<<", "<<it.Value()<<") = "<<polygon2->NbCrossing(current,it.Value())<<std::endl;
	  file<<"polygon2->NbTouching("<<current<<", "<<it.Value()<<") = "<<polygon2->NbTouching(current,it.Value())<<std::endl;
	  current = it.Value();
	}
      file<<"polygon1->NbCrossing("<<current<<", "<<firstVertex<<") = "<<polygon1->NbCrossing(current,firstVertex)<<std::endl;
      file<<"polygon1->NbTouching("<<current<<", "<<firstVertex<<") = "<<polygon1->NbTouching(current,firstVertex)<<std::endl;
      file<<"polygon2->NbCrossing("<<current<<", "<<firstVertex<<") = "<<polygon2->NbCrossing(current,firstVertex)<<std::endl;
      file<<"polygon2->NbTouching("<<current<<", "<<firstVertex<<") = "<<polygon2->NbTouching(current,firstVertex)<<std::endl;
      
      file<<std::endl<<std::endl;
      
      current = begin2.Value();
      firstVertex=current;
      ++begin2;
      for(it=begin2;it!=end2;++it)
	{
	  file<<"polygon1->NbCrossing("<<current<<", "<<it.Value()<<") = "<<polygon1->NbCrossing(current,it.Value())<<std::endl;
	  file<<"polygon1->NbTouching("<<current<<", "<<it.Value()<<") = "<<polygon1->NbTouching(current,it.Value())<<std::endl;
	  file<<"polygon2->NbCrossing("<<current<<", "<<it.Value()<<") = "<<polygon2->NbCrossing(current,it.Value())<<std::endl;
	  file<<"polygon2->NbTouching("<<current<<", "<<it.Value()<<") = "<<polygon2->NbTouching(current,it.Value())<<std::endl;
	  current = it.Value();
	}
      file<<"polygon1->NbCrossing("<<current<<", "<<firstVertex<<") = "<<polygon1->NbCrossing(current,firstVertex)<<std::endl;
      file<<"polygon1->NbTouching("<<current<<", "<<firstVertex<<") = "<<polygon1->NbTouching(current,firstVertex)<<std::endl;
      file<<"polygon2->NbCrossing("<<current<<", "<<firstVertex<<") = "<<polygon2->NbCrossing(current,firstVertex)<<std::endl;
      file<<"polygon2->NbTouching("<<current<<", "<<firstVertex<<") = "<<polygon2->NbTouching(current,firstVertex)<<std::endl;
      
      file<<std::endl<<std::endl;
      file<<"Bounding Boxs computation : "<<std::endl;
      polygon1->ComputeBoundingBox();
      file<<"Bounding Box 1"<<std::endl; 
      file<<"Index : "<<polygon1->GetBoundingBoxIndex()<<" , Size : "<<polygon1->GetBoundingBoxSize()<<std::endl;
      polygon2->ComputeBoundingBox();
      file<<"Bounding Box 2"<<std::endl; 
      file<<"Index : "<<polygon2->GetBoundingBoxIndex()<<" , Size : "<<polygon2->GetBoundingBoxSize()<<std::endl;
 

      file.close();
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
