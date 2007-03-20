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
#include "otbImageFileWriter.h"
#include "otbSpatialObjectDXFReader.h"
#include "otbSpatialObjectToImageDrawingFilter.h"

int otbSpatialObjectDXFReader(int argc, char * argv[])
{
  try
    {
      std::cout<<"Entering main"<<std::endl;
      const unsigned int Dimension = 2;
      char * inputFilename = argv[1];
      char * outputFilename = argv[2];

      typedef unsigned char PixelType;

      typedef otb::Image<PixelType,Dimension> ImageType;
      typedef otb::ImageFileWriter<ImageType> WriterType;
      typedef otb::SpatialObjectDXFReader<Dimension> SpatialObjectDXFReaderType;
      typedef SpatialObjectDXFReaderType::GroupSpatialObjectType GroupType;
      typedef otb::SpatialObjectToImageDrawingFilter<GroupType,ImageType> SpatialObjectToImageDrawingFilterType;

      // Instantiating object
      SpatialObjectDXFReaderType::Pointer reader = SpatialObjectDXFReaderType::New();
      SpatialObjectToImageDrawingFilterType::Pointer imageGenerator = SpatialObjectToImageDrawingFilterType::New();
      WriterType::Pointer writer = WriterType::New();

      reader->SetFileName(inputFilename);
      //reader->SetLayerName("TOIT");
      reader->GenerateData();
      GroupType::Pointer group = reader->GetOutput();

      std::cout<<"DXF file read."<<std::endl;
      std::cout<<"Number of children: "<<group->GetNumberOfChildren()<<std::endl;

      if(group->GetNumberOfChildren() != 0)
	{
	  GroupType::ChildrenListType* children=group->GetChildren(0);
	  GroupType::ChildrenListType::iterator it = children->begin();
	  GroupType::ChildrenListType::iterator end = children->end();
	  double maximum[Dimension],minimum[Dimension];
	  (*it)->ComputeBoundingBox();
	  minimum[0]=(*it)->GetBoundingBox()->GetMinimum()[0];
	  minimum[1]=(*it)->GetBoundingBox()->GetMinimum()[1];
	  while(it != end)
	    {
 	  (*it)->ComputeBoundingBox();
	  
 	  if ((*it)->GetBoundingBox()->GetMinimum()[0] < minimum[0])
 	    {
 	      minimum[0]=(*it)->GetBoundingBox()->GetMinimum()[0];
 	    }
 	  if ((*it)->GetBoundingBox()->GetMinimum()[1] < minimum[1])
 	    {		
 	      minimum[1]=(*it)->GetBoundingBox()->GetMinimum()[1];
	    }
 	  it++;
	    }
	  
	  ImageType::SizeType size;
	  size[0]=1000;
	  size[1]=1000;
	  imageGenerator->SetSize(size);	
	  ImageType::PointType origin;
	  origin[0]=  minimum[0]; 
	  origin[1]= minimum[1];
	  imageGenerator->SetOrigin(origin);
	  
	  group->ComputeBoundingBox();
	  
	  maximum[0]=group->GetBoundingBox()->GetMaximum()[0];
	  maximum[1]=group->GetBoundingBox()->GetMaximum()[1];
	  
	  ImageType::SpacingType spacing;
	  spacing[0]=(maximum[0]-origin[0])/size[0];			
	  spacing[1]=(maximum[1]-origin[1])/size[1];			
	  imageGenerator->SetSpacing(spacing);    
	  
	  
	  std::cout<<"Origin: "<<origin<<std::endl;
	  std::cout<<"Spacing: "<<spacing << std::endl;

	  imageGenerator->SetInput(group);
	  
	  writer->SetInput(imageGenerator->GetOutput());
	  writer->SetFileName(outputFilename);
	  writer->Update();
	}
      else
	{
	  std::cout<<"No objects detected."<<std::endl;
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
