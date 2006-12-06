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
#include "otbRCC8VertexWithCompacity.h"

int otbRCC8VertexWithCompacity(int argc, char* argv[])
{
try
  {
    typedef unsigned short LabelType;
    typedef double PrecisionType;

    unsigned int imageIndex = atoi(argv[1]);
    LabelType objectLabel = static_cast<LabelType>(atof(argv[2]));
    PrecisionType compacity = atof(argv[3]);

    typedef otb::RCC8VertexWithCompacity<LabelType,PrecisionType> RCC8VertexType;
    typedef RCC8VertexType::AttributesMapType AttributesMapType;
    // Instantiation
    RCC8VertexType::Pointer vertex1= RCC8VertexType::New();
    // Getters / setters tests
    vertex1->SetSegmentationImageIndex(imageIndex);
    vertex1->SetObjectLabelInImage(objectLabel);
    vertex1->SetCompacity(compacity);

    if(vertex1->GetSegmentationImageIndex()!=imageIndex)
      {
	std::cout<<"Test failed: vertex1->GetSegmentationImageIndex()!=imageIndex"<<std::endl;
	return EXIT_FAILURE;
      }
    if(vertex1->GetObjectLabelInImage()!=objectLabel)
      {
	std::cout<<"Test failed: vertex1->GetSegmentationImageIndex()!=imageIndex"<<std::endl;
	return EXIT_FAILURE;
      }
    if(vertex1->GetCompacity()!=compacity)
      {
	std::cout<<"Test failed: vertex1->GetSegmentationImageIndex()!=imageIndex"<<std::endl;
	return EXIT_FAILURE;
      }

    // attributes vector set test
    AttributesMapType attr1 = vertex1->GetAttributesMap(); 
    if(attr1["SegmentationImageIndex"].compare(std::string(argv[1]))!=0)
      {
	std::cout<<"Test failed: vertex1->GetAttributesMap()[\"SegmentationImageIndex\"]!=std::string(argv[1])"<<std::endl;
	return EXIT_FAILURE;
      }
    if(attr1["ObjectLabelInImage"].compare(std::string(argv[2]))!=0)
      {
	std::cout<<"Test failed: vertex1->GetAttributesMap()[\"ObjectLabelInImage\"]!=std::string(argv[2])"<<std::endl;
	return EXIT_FAILURE;
      }
    if(attr1["Compacity"].compare(std::string(argv[3]))!=0)
      {
	std::cout<<"Test failed: vertex1->GetAttributesMap()[\"Compacity\"]!=std::string(argv[2])"<<std::endl;
	return EXIT_FAILURE;
      }

    // attributes vector get test
    RCC8VertexType::Pointer vertex2 = RCC8VertexType::New();
    vertex2->SetAttributesMap(attr1);
    if(vertex1->GetSegmentationImageIndex()!=vertex2->GetSegmentationImageIndex())
      {
	std::cout<<"Test failed: vertex1->GetSegmentationImageIndex()!=vertex2->GetSegmentationImageIndex()"<<std::endl;
	return EXIT_FAILURE;
      }
    if(vertex1->GetObjectLabelInImage()!=vertex2->GetObjectLabelInImage())
      {
	std::cout<<"Test failed: vertex1->GetObjectLabelInImage()!=vertex2->GetObjectLabelInImage()"<<std::endl;
	return EXIT_FAILURE;
      }
    if(vertex1->GetCompacity()!=vertex2->GetCompacity())
      {
	std::cout<<"Test failed: vertex1->GetCompacity()!=vertex2->GetCompacity()"<<std::endl;
	return EXIT_FAILURE;
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
