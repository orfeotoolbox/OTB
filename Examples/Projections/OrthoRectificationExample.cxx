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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif



// iostream is used for general output
#include <iostream>
#include <iterator>
#include <stdlib.h>

#include "otbMacro.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"

#include "otbPerBandVectorImageFilter.h"

#include "init/ossimInit.h"

// Software Guide : BeginLatex
//
// This example demonstrates the use of the
// \doxygen{otb}{OrthoRectificationFilter}. This filter is intended to
// orthorectify images which are in a distributor format with the
// appropriate meta-data describing the sensor model. In this example,
// we will choose to use an UTM projection for the output image.
// 
// The first step toward the use of these filters is to include the
// proper header files: the one for the ortho-rectification filter and
// the one defining the different projections available in OTB.
//
// Software Guide : EndLatex


// Software Guide : BeginCodeSnippet

#include "otbOrthoRectificationFilter.h"
#include "otbMapProjections.h"

// Software Guide : EndCodeSnippet

int main( int argc, char* argv[] )
{


  //ossimInit::instance()->initialize(argc, argv);

  if(argc!=9)
    {
    std::cout << argv[0] <<" <input_filename> <output_filename> <x_ground_upper_left_corner> <y_ground_upper_left_corner> <x_Size> <y_Size> <x_groundSamplingDistance> <y_groundSamplingDistance> (should be negative since origin is upper left)>" 
	      << std::endl;

    return EXIT_FAILURE;
    }

// Software Guide : BeginLatex
//
// We will start by defining the types for the images, the image file
// reader and the image file writer. The writer will be a
// \doxygen{otb}{StreamingImageFileWriter} which will allow us to set
// the number of stream divisions we want to apply when writing the
// output image, which can be very large.
//
// Software Guide : EndLatex


// Software Guide : BeginCodeSnippet



   
  typedef otb::Image<unsigned char, 2>    CharImageType;
  typedef otb::Image<unsigned int, 2>     ImageType;
  typedef otb::VectorImage<unsigned int, 2>     VectorImageType;
  typedef otb::ImageFileReader<VectorImageType>  ReaderType;
  typedef otb::StreamingImageFileWriter<VectorImageType>  WriterType;

					
  ReaderType::Pointer     	reader=ReaderType::New();
  WriterType::Pointer	    	writer=WriterType::New();

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);


// Software Guide : EndCodeSnippet	

// Software Guide : BeginLatex
//
// We can now proceed to declare the type for the ortho-rectification
// filter. The class \doxygen{otb}{OrthoRectificationFilter} is
// templated over the input and the output image types as well as over
// the cartographic projection. We define therefore the
// type of the projection we want, which is an UTM projection for this case.
//
// Software Guide : EndLatex


// Software Guide : BeginCodeSnippet


	
  typedef otb::UtmInverseProjection utmMapProjectionType ;
  typedef otb::OrthoRectificationFilter<ImageType, ImageType,
                              utmMapProjectionType> OrthoRectifFilterType ;

	
  OrthoRectifFilterType::Pointer  orthoRectifFilter =
                                              OrthoRectifFilterType::New();
  
// Software Guide : EndCodeSnippet
  
  
// Software Guide : BeginLatex
//
// Now we need to 
// instanciate the map projection, set the {\em zone} and {\em hemisphere} 
// parameters and pass this projection to the orthorectification filter.
//
// Software Guide : EndLatex
  
// Software Guide : BeginCodeSnippet
  utmMapProjectionType::Pointer utmMapProjection =
                                              utmMapProjectionType::New();
  utmMapProjection->SetZone(31);
  utmMapProjection->SetHemisphere('N');
  orthoRectifFilter->SetMapProjection(utmMapProjection);
				
// Software Guide : EndCodeSnippet	

// Software Guide : BeginLatex
// 
// Wiring the orthorectification filter into a PerBandImageFilter allows 
// to orthrectify images with multiple bands seamlesly.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::PerBandVectorImageFilter<VectorImageType, VectorImageType, OrthoRectifFilterType> PerBandFilterType;
  PerBandFilterType::Pointer perBandFilter=PerBandFilterType::New();
  perBandFilter->SetFilter(orthoRectifFilter);
  perBandFilter->SetInput(reader->GetOutput());
  
// Software Guide : EndCodeSnippet	

// Software Guide : BeginLatex
//
// Using the user-provided information, we define the output region
// for the image generated by the orthorectification filter.
//
// Software Guide : EndLatex


// Software Guide : BeginCodeSnippet
  
  ImageType::IndexType start;
  start[0]=0;
  start[1]=0;
  orthoRectifFilter->SetOutputStartIndex(start);
				
  ImageType::SizeType size;
  size[0]=atoi(argv[5]);
  size[1]=atoi(argv[6]);
  orthoRectifFilter->SetSize(size);
				
  ImageType::SpacingType spacing;
  spacing[0]=atof(argv[7]);
  spacing[1]=atof(argv[8]);
  orthoRectifFilter->SetOutputSpacing(spacing);
				
  ImageType::PointType origin;
  origin[0]=strtod(argv[3], NULL);
  origin[1]=strtod(argv[4], NULL);
  orthoRectifFilter->SetOutputOrigin(origin);


// Software Guide : EndCodeSnippet	

// Software Guide : BeginLatex
//
// We can now set plug the ortho-rectification filter to the writer
// and set the number of tiles we want to split the output image in
// for the writing step.
//
// Software Guide : EndLatex


// Software Guide : BeginCodeSnippet
  
  writer->SetInput(perBandFilter->GetOutput());
				
  writer->SetTilingStreamDivisions();

// Software Guide : EndCodeSnippet	

// Software Guide : BeginLatex
//
// Finally, we trigger the pipeline execution by calling the
// \code{Update()} method on the writer. Please note that the
// ortho-rectification filter is derived from the
// \doxygen{otb}{StreamingResampleImageFilter} in order to be able to
// compute the input image regions which are needed to build the
// output image. Since the resampler applies a geometric
// transformation (scale, rotation, etc.), this region computation is
// not trivial. 
//
// Software Guide : EndLatex


// Software Guide : BeginCodeSnippet
  

  writer->Update();

// Software Guide : EndCodeSnippet	  


  return EXIT_SUCCESS;

}
