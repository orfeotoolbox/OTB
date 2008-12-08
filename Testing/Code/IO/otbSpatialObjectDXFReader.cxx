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
#include "itkRescaleIntensityImageFilter.h"

int otbSpatialObjectDXFReader(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  char * inputFilename = argv[1];
  char * outputFilename = argv[2];
  const unsigned int outputSize = atoi(argv[4]);

  typedef double PixelType;
  typedef unsigned char OutputPixelType;
  typedef itk::GroupSpatialObject<Dimension> GroupType;
  typedef otb::Image<PixelType,Dimension> ImageType;
  typedef otb::Image<OutputPixelType,Dimension> OutputImageType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  typedef otb::SpatialObjectDXFReader<GroupType> SpatialObjectDXFReaderType;
  typedef otb::SpatialObjectToImageDrawingFilter<GroupType,ImageType> SpatialObjectToImageDrawingFilterType;
  typedef itk::RescaleIntensityImageFilter< ImageType, OutputImageType >     CastFilterType;
  typedef itk::SpatialObject<Dimension> SpatialObjectType;
  // Instantiating object
  SpatialObjectDXFReaderType::Pointer reader = SpatialObjectDXFReaderType::New();
  SpatialObjectToImageDrawingFilterType::Pointer imageGenerator = SpatialObjectToImageDrawingFilterType::New();
  WriterType::Pointer writer = WriterType::New();
  CastFilterType::Pointer castFilter = CastFilterType::New();

  reader->SetFileName(inputFilename);
  reader->SetLayerName(argv[3]);


  reader->Update();
  GroupType::Pointer group = reader->GetOutput();

  if(group->GetNumberOfChildren() != 0)
    {
      /** Writing image  **/
      SpatialObjectType::ChildrenListType* children=group->GetChildren(0);
      SpatialObjectType::ChildrenListType::iterator it = children->begin();
      SpatialObjectType::ChildrenListType::iterator end = children->end();
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
      size[0]=outputSize;
      size[1]=outputSize;
      imageGenerator->SetSize(size);
      ImageType::PointType origin;
      origin[0]=(int) minimum[0];
      origin[1]=(int) minimum[1];
      imageGenerator->SetOrigin(origin);

      group->ComputeBoundingBox();

      maximum[0]=group->GetBoundingBox()->GetMaximum()[0];
      maximum[1]=group->GetBoundingBox()->GetMaximum()[1];

      ImageType::SpacingType spacing;
      spacing[0]=(maximum[0]-origin[0])/size[0];
      spacing[1]=(maximum[1]-origin[1])/size[1];
      imageGenerator->SetSpacing(spacing);


      imageGenerator->SetInput(group);



      imageGenerator->Update();

      castFilter->SetOutputMinimum( 0 );
      castFilter->SetOutputMaximum( 255 );
      castFilter->SetInput( imageGenerator->GetOutput() );
      writer->SetInput(castFilter->GetOutput());

      writer->SetFileName(outputFilename);
      writer->Update();
    }
  else
    {
      std::cout<<"No objects detected."<<std::endl;
      return EXIT_FAILURE;

    }


  return EXIT_SUCCESS;
}
