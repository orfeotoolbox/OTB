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

//  Software Guide : BeginCommandLineArgs
//  INPUTS: {Lalande.dxf}
//  TOIT 1000
//  OUTPUTS: {lalande.png}
//  Software Guide : EndCommandLineArgs




//  Software Guide : BeginLatex
//
//  \label{DXFExample}This example illustrates how to read a DXF file and how to draw objects 
//  on a $2D$ binary image. The graphical DXF objects which can be read are the following : Point, Line
//  Polyline, Circle and 3DFace.
//  The example begins by including the appropriate headers.
//
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
#include "itkExceptionObject.h"
#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbSpatialObjectDXFReader.h"
#include "otbSpatialObjectToImageDrawingFilter.h"
#include "itkRescaleIntensityImageFilter.h"
// Software Guide : EndCodeSnippet


int main(int argc, char * argv[])
{
	  // Verify the number of parameters in the command line
 	   if( argc < 5 )
      {
      std::cerr << "Usage: " << std::endl;
      std::cerr << argv[0] << " inputFilename  LayerName outputSize outputFilename " << std::endl;
      return EXIT_FAILURE;
      }
	  
// Software Guide : BeginLatex
//
//  Then, as usual, we select the pixel types and the image
//  dimension.
// Software Guide : EndLatex 


	  char * inputFilename = argv[1];
      char * outputFilename = argv[4];
      


// Software Guide : BeginCodeSnippet	
      const unsigned int Dimension = 2;
      typedef double PixelType;
      typedef unsigned char OutputPixelType;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// The DXF file reader and the image file writer types are instantiated.
// We can then create objects for both of them.
// Graphical DXF objects will be represented in a GroupSpatialObject.
// Software Guide : EndLatex 
 
// Software Guide : BeginCodeSnippet
      typedef itk::GroupSpatialObject<Dimension> GroupType;
      typedef otb::Image<PixelType,Dimension> ImageType;
      typedef otb::Image<OutputPixelType,Dimension> OutputImageType;
      typedef otb::ImageFileWriter<OutputImageType> WriterType;
      typedef otb::SpatialObjectDXFReader<GroupType>
	                                  SpatialObjectDXFReaderType;
      typedef otb::SpatialObjectToImageDrawingFilter<GroupType,ImageType>
	                       SpatialObjectToImageDrawingFilterType;
      typedef itk::RescaleIntensityImageFilter< ImageType,
                                       OutputImageType >     CastFilterType;
      typedef itk::SpatialObject<Dimension> SpatialObjectType;


      // Instantiating object
      SpatialObjectDXFReaderType::Pointer reader =
	                                   SpatialObjectDXFReaderType::New();
      SpatialObjectToImageDrawingFilterType::Pointer imageGenerator =
	                        SpatialObjectToImageDrawingFilterType::New();
      WriterType::Pointer writer = WriterType::New();
      CastFilterType::Pointer castFilter = CastFilterType::New();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
// Filenames should be provided for both the reader and the writer. In this
// particular example we take those filenames from the command line arguments.
// The size of the output image is also specified. 
// Thanks to the SetLayerName() method, a particular layer can be specified and other layers will not be read. 
// If no layer name is specified, all layers are read.
// Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet	
      reader->SetFileName(inputFilename);
      reader->SetLayerName(argv[2]);
      writer->SetFileName(outputFilename);
      const unsigned int outputSize = atoi(argv[3]);
// Software Guide : EndCodeSnippet
     

// Software Guide : BeginLatex
// The reading of the DXF file is performed with the Update() method. Consequently the group of Spatial Objects is created.
// Software Guide : EndLatex 
// Software Guide : BeginCodeSnippet
      reader->Update();
      GroupType::Pointer group = reader->GetOutput();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
// We check if the group is empty. If it is not the case we will represent the Spatial Object group on the output image.
// To determine the minimum and maximum coordinates of the group we compute the bounding box of each element of the group.
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
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
// Software Guide : EndCodeSnippet	


// Software Guide : BeginLatex
// Origin can be set at the minimum coordinate of the group and the spacing be adapted to the specified output image size in order to represent all 
// Spatial Objects in the output image.
// Software Guide : EndLatex


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
// Software Guide : BeginCodeSnippet
	ImageType::SpacingType spacing;
	spacing[0]=(maximum[0]-origin[0])/size[0];			
	spacing[1]=(maximum[1]-origin[1])/size[1];			
	imageGenerator->SetSpacing(spacing);	
// Software Guide : EndCodeSnippet	
	


// Software Guide : BeginLatex
// The output image is created with previously specified origin, spacing and size.
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
	imageGenerator->SetInput(group);
	imageGenerator->Update();
// Software Guide : EndCodeSnippet
	
// Software Guide : BeginLatex
// The output image is written by calling the Update() method.
// Software Guide : EndLatex
	castFilter->SetOutputMinimum( 0 );
	castFilter->SetOutputMaximum( 255 );
	castFilter->SetInput( imageGenerator->GetOutput() );
	writer->SetInput(castFilter->GetOutput());
	
// Software Guide : BeginCodeSnippet	
	writer->Update();
// Software Guide : EndCodeSnippet
	}
      else
	{
	  std::cout<<"No objects detected."<<std::endl;
	  return EXIT_FAILURE;

	}
//  Software Guide : BeginLatex
//  
// \begin{figure}
// \center
// \includegraphics[width=0.44\textwidth]{lalande.eps}
// \itkcaption[DXF reader]{Representation of a DXF file on an image.}
// \label{fig:lalande}
// \end{figure}
//
//  Figure \ref{fig:lalande} 
//  represents Spatial Objects extracted from a DXF file.
// Software Guide : EndLatex  
  
  return EXIT_SUCCESS;
}
