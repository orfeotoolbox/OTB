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

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

// Software Guide : BeginLatex
// This example illustrates the use of the \doxygen{otb}{PolarimetricSynthesisFilter}
// in order to perform a synthetic image using fully polarimetric data.
// 
// We will start by including the header file.
//
// Software Guide : EndLatex


// Software Guide : BeginCodeSnippet

#include "otbPolarimetricSynthesisFilter.h"

// Software Guide : EndCodeSnippet

int main( int argc, char* argv[] )
{


  if(argc!=5)
    {
    std::cout << argv[0]
              <<" <input_filename_HH_channel>"
              <<" <input_filename_HV_channel>"
              <<" <input_filename_VH_channel>"
              <<" <input_filename_VV_channel>"
              <<" <output_filename>"
	      << std::endl;

    return EXIT_FAILURE;
    }
   
// Software Guide : BeginLatex
//
// We begin by declaring the types for the input image and the image reader.
//
// Software Guide : EndLatex


// Software Guide : BeginCodeSnippet

  typedef std::complex <double>                   InputPixelType;
  typedef double    	                        OutputPixelType;
  const   unsigned int                            Dimension = 2;

  typedef otb::Image< InputPixelType,  Dimension >  InputImageType;
  typedef otb::Image< OutputPixelType, Dimension >  OutputImageType;

  typedef otb::ImageFileWriter< OutputImageType >   WriterType;

// Software Guide : EndCodeSnippet  

// Software Guide : BeginLatex
//
// The PolarimetricSynthesisFilter can be instantiated
//  using the image types defined above.
//
// Software Guide : EndLatex


// Software Guide : BeginCodeSnippet

  typedef otb::PolarimetricSynthesisFilter<InputImageType, InputImageType,InputImageType,InputImageType,OutputImageType >   FilterType;

// Software Guide : EndCodeSnippet  

//  Software Guide : BeginLatex
//
//  An \doxygen{otb}{ImageFileReader} class is also instantiated in order to read
//  image data from a file. 
//
//  Software Guide : EndLatex   

// Software Guide : BeginCodeSnippet

  typedef otb::ImageFileReader< InputImageType  >   ReaderType;

// Software Guide : EndCodeSnippet  


//  Software Guide : BeginLatex
//  
// An \doxygen{otb}{ImageFileWriter} is instantiated in order to write the
// output image to a file.
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet

  typedef otb::ImageFileWriter< OutputImageType >  WriterType;

// Software Guide : EndCodeSnippet


//  Software Guide : BeginLatex
//
//  The filter, the reader  and the writter are created by invoking their \code{New()}
//  methods and assigning the result to SmartPointers.
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet

  ReaderType::Pointer reader_HH = ReaderType::New();
  ReaderType::Pointer reader_HV = ReaderType::New();
  ReaderType::Pointer reader_VH = ReaderType::New();
  ReaderType::Pointer reader_VV = ReaderType::New();                        
  WriterType::Pointer writer = WriterType::New();
  FilterType::Pointer polarimetricSynthesis = FilterType::New();

  reader_HH->SetFileName( argv[1] );
  reader_HV->SetFileName( argv[2] );
  reader_VH->SetFileName( argv[3] );
  reader_VV->SetFileName( argv[4] );                        
  writer->SetFileName( argv[5] );
  
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//  
//  The image obtained with the reader is passed as input to the
//  \doxygen{otb}{PolarimetricSynthesisFilter}.
//
//  \index{otb::PolarimetricSynthesisFilter!SetInputHH()}
//  \index{otb::PolarimetricSynthesisFilter!SetInputHV()}
//  \index{otb::PolarimetricSynthesisFilter!SetInputVH()}
//  \index{otb::PolarimetricSynthesisFilter!SetInputVV()}
//  \index{otb::FileImageReader!GetOutput()}
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet

  polarimetricSynthesis->SetInputHH( reader_HH->GetOutput() );
  polarimetricSynthesis->SetInputHV( reader_HV->GetOutput() );
  polarimetricSynthesis->SetInputVH( reader_VH->GetOutput() );
  polarimetricSynthesis->SetInputVV( reader_VV->GetOutput() );                        

// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//  
//  The method \code{SetPsiI()} defines the orientation of the emit electromagnetic
//  field to be used for the computation.
//  The method \code{SetKhiI()} defines the ellipticity of the emit electromagnetic
//  field to be used for the computation.
//  The method \code{SetPsiR()} defines the orientation of the receive electromagnetic
//  field to be used for the computation.
//  The method \code{SetKhiR()} defines the ellipticity of the receive electromagnetic
//  field to be used for the computation.
//
//  Hence if we want to emulate a \pi \over 4 data in emission
//  \code{SetPsiI()} is equal to 45.0 and \code{SetKhiI()} to 0.0.
//  In our example, we choose the reveiver to be a linear horizontal polarisation.
//  
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet

  polarimetricSynthesis->SetPsiI( 45. );
  polarimetricSynthesis->SetKhiI(  0. );
  polarimetricSynthesis->SetPsiR(  0. );
  polarimetricSynthesis->SetKhiR(  0. );

// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//  
//  The filter is executed by invoking the \code{Update()} method. If the
//  filter is part of a larger image processing pipeline, calling
//  \code{Update()} on a downstream filter will also trigger update of this
//  filter.
//
//  Software Guide : EndLatex 
 
// Software Guide : BeginCodeSnippet

  polarimetricSynthesis->Update();

// Software Guide : EndCodeSnippet  

  writer->SetInput( polarimetricSynthesis->GetOutput() );
        
  writer->Update(); 
  
  return EXIT_SUCCESS;

}

