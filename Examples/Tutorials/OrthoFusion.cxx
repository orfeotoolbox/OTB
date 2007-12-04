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

//  Software Guide : BeginLatex
//
//  Start by including some necessary headers and with the 
//  usual \code{main} declaration. Apart from the classical header related to 
// image input and output. We need the headers related to the fusion and the 
// orthorectification. One header is also required to be able to process 
// vector images (the XS one) with the orthorectification.
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet

// iostream is used for general output
// #include <iostream>
// #include <iterator>
// #include <stdlib.h>

#include "otbMacro.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
// #include "otbStreamingResampleImageFilter.h"


#include "itkChangeInformationImageFilter.h"
#include "otbPerBandVectorImageFilter.h"

#include "otbBayesianFusionFilter.h"

#include "init/ossimInit.h"

#include "otbOrthoRectificationFilter.h"
#include "otbMapProjections.h"



int main( int argc, char* argv[] )
{
// Software Guide : EndCodeSnippet

  
  //  Software Guide : BeginLatex
  //
  // We initialize ossim which is required for the orthorectification and we 
  // check that all parameters are provided. Basically, we need
  // \begin{itemize}
  // \item the name of the input PAN image; 
  // \item the name of the input XS image;
  // \item the desired name for the output;
  // \item as the coordinates are given in UTM, we need the UTM zone number;
  // \item of course, we need the UTM coordinates of the final image;
  // \item the size in pixels of the final image;
  // \item and the sampling of the final image.
  // \end{itemize}
  //
  // We check that all those parameters are provided.
  //
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
  ossimInit::instance()->initialize(argc, argv);

  if(argc!=11)
  {
    std::cout << argv[0] <<" <input_pan_filename> <input_xs_filename> <output_filename> <utm zone> <x_ground_upper_left_corner> <y_ground_upper_left_corner> <x_Size> <y_Size> <x_groundSamplingDistance> <y_groundSamplingDistance (should be negative since origin is upper left)>" 
        << std::endl;

    return EXIT_FAILURE;
  }
// Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // We declare the different images, readers and writer:
  //
  //  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
  typedef otb::Image<unsigned int, 2>     ImageType;
  typedef otb::VectorImage<unsigned int, 2>     VectorImageType;
  typedef otb::ImageFileReader<ImageType>  ReaderType;
  typedef otb::ImageFileReader<VectorImageType>  VectorReaderType;
  typedef otb::StreamingImageFileWriter<VectorImageType>  WriterType;

					
  ReaderType::Pointer     	readerPAN=ReaderType::New();
  VectorReaderType::Pointer     readerXS=VectorReaderType::New();
  WriterType::Pointer	    	writer=WriterType::New();

  readerPAN->SetFileName(argv[1]);
  readerXS->SetFileName(argv[2]);
  writer->SetFileName(argv[3]);
// Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  // We declare the projection (here we chose the UTM projection, other choice 
  // are possible). We also declare the orthorectification filter. Note that
  // the \doxygen{otb}{OrthoRectificationFilter} is designed to work with one
  // band images. To be able to process the XS image (which are 
  // \doxygen{otb}{VectorImage}), we need to use the 
  // \doxygen{otb}{PerBandVectorImageFilter} which is going to apply the filter
  // set via the method \code{SetFilter()} to all spectral bands.
  //
  //  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet

	
  typedef otb::UtmInverseProjection utmMapProjectionType ;
  typedef otb::OrthoRectificationFilter<ImageType, ImageType,
  utmMapProjectionType> OrthoRectifFilterType ;
  typedef otb::PerBandVectorImageFilter<VectorImageType, VectorImageType, OrthoRectifFilterType> VectorOrthoRectifFilterType;
	
  OrthoRectifFilterType::Pointer  orthoRectifPAN =
      OrthoRectifFilterType::New();
  OrthoRectifFilterType::Pointer  orthoRectifXS =
      OrthoRectifFilterType::New();
  VectorOrthoRectifFilterType::Pointer  orthoRectifXSVector =
      VectorOrthoRectifFilterType::New();
  orthoRectifXSVector->SetFilter(orthoRectifXS);

  utmMapProjectionType::Pointer utmMapProjection =
      utmMapProjectionType::New();
  utmMapProjection->SetZone(atoi(argv[4]));
  utmMapProjection->SetHemisphere('N');
  orthoRectifPAN->SetMapProjection(utmMapProjection);
  orthoRectifXS->SetMapProjection(utmMapProjection);
// Software Guide : EndCodeSnippet				

  //  Software Guide : BeginLatex
  //
  //  ************** This shouldn't be necessary any more !!!**********
  //
  //  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
  
  ImageType::PointType originNull;
  originNull[0]=0;
  originNull[1]=0;
  
  
  readerPAN->GenerateOutputInformation();
  
  typedef itk::ChangeInformationImageFilter<ImageType > ChangeInfoFilterType;
  ChangeInfoFilterType::Pointer changeInfoPAN = ChangeInfoFilterType::New();
  changeInfoPAN->SetInput(readerPAN->GetOutput());
  changeInfoPAN->ChangeOriginOn();
  changeInfoPAN->SetOutputOrigin(originNull);
  changeInfoPAN->GenerateOutputInformation();
  orthoRectifPAN->SetInput(changeInfoPAN->GetOutput());

  
  readerXS->GenerateOutputInformation();
  
  typedef itk::ChangeInformationImageFilter<VectorImageType > VectorChangeInfoFilterType;
  VectorChangeInfoFilterType::Pointer changeInfoXS = VectorChangeInfoFilterType::New();
  changeInfoXS->SetInput(readerXS->GetOutput());
  changeInfoXS->ChangeOriginOn();
  changeInfoXS->SetOutputOrigin(originNull);
  changeInfoXS->GenerateOutputInformation();
  orthoRectifXSVector->SetInput(changeInfoXS->GetOutput());
  
  // Software Guide : EndCodeSnippet				

  //  Software Guide : BeginLatex
  //
  //  We now pass the orthorectification parameters to the orthorectification 
  // filters:
  //
  //  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
  
  ImageType::IndexType start;
  start[0]=0;
  start[1]=0;
  orthoRectifPAN->SetOutputStartIndex(start);
  orthoRectifXS->SetOutputStartIndex(start);
				
  ImageType::SizeType size;
  size[0]=atoi(argv[7]);
  size[1]=atoi(argv[8]);
  orthoRectifPAN->SetSize(size);
  orthoRectifXS->SetSize(size);
				
  ImageType::SpacingType spacing;
  spacing[0]=atof(argv[9]);
  spacing[1]=atof(argv[10]);
  orthoRectifPAN->SetOutputSpacing(spacing);
  orthoRectifXS->SetOutputSpacing(spacing);
				
  ImageType::PointType origin;
  origin[0]=strtod(argv[5], NULL);
  origin[1]=strtod(argv[6], NULL);
  orthoRectifPAN->SetOutputOrigin(origin);
  orthoRectifXS->SetOutputOrigin(origin);

  // Software Guide : EndCodeSnippet				

  //  Software Guide : BeginLatex
  //
  //  It's time to declare the fusion filter and set its inputs:
  //
  //  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
  
  typedef otb::BayesianFusionFilter< VectorImageType, VectorImageType, ImageType, VectorImageType     >    BayesianFusionFilterType;
  BayesianFusionFilterType::Pointer bayesianFilter = BayesianFusionFilterType::New();
  
  bayesianFilter->SetMultiSpect(orthoRectifXSVector->GetOutput() );
  bayesianFilter->SetMultiSpectInterp(orthoRectifXSVector->GetOutput() );
  bayesianFilter->SetPanchro(orthoRectifPAN->GetOutput() );
  
  
  // Software Guide : EndCodeSnippet				

  //  Software Guide : BeginLatex
  //
  //  And we can plug it to the writer. To be able to process the images by 
  // tiles, we use the \code{SetTilingStreamDivisions()} method of the writer. 
  // We trigger the pipeline execution with the \code{Update()} method.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  
  writer->SetInput(bayesianFilter->GetOutput());
				
  writer->SetTilingStreamDivisions(20);

  writer->Update();

  return EXIT_SUCCESS;

}
    // Software Guide : EndCodeSnippet
	
