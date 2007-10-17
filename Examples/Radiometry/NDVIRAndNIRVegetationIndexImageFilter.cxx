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

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif


//  Software Guide : BeginCommandLineArgs
//  INPUTS: {NDVI_2.hdr} , {NDVI_3.hdr} 
//  OUTPUTS: {NDVIRAndNIRVegetationIndex.tif} , {pretty_Red.png} , {pretty_NIR.png} , {pretty_NDVIRAndNIRVegetationIndex.png}
//  Software Guide : EndCommandLineArgs


// Software Guide : BeginLatex
//
// \index{otb::RAndNIRVegetationIndexImageFilter}
// \index{otb::VegetationIndex}
// \index{otb::VegetationIndex!header}
//
// The following example illustrates the use of the 
// \doxygen{otb}{RAndNIRVegetationIndexImageFilter} with the use of the Normalized 
// Difference Vegatation Index (NDVI). 
// NDVI computes the difference between the NIR channel, noted $L_{NIR}$, and the red channel,
// noted $L_{r}$ radiances reflected from the surface and transmitted through the atmosphere :
//
// \begin{equation}
// \mathbf{NDVI} = \frac{L_{NIR}-L_{r}}{L_{NIR}+L_{r}}
// \end{equation}
//
// With the \doxygen{otb}{RAndNIRVegetationIndexImageFilter} class the filter
// inputs are one channel images : one inmage represents the NIR channel, the 
// the other the NIR channel.
//
// Let's look at the minimal code required to use this algorithm. First, the following header 
// defining the \doxygen{otb}{RAndNIRVegetationIndexImageFilter} 
// and the \doxygen{otb}{VegetationIndex} (that contains the ARVI) classes must be included.
// Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
#include "otbRAndNIRVegetationIndexImageFilter.h"
#include "otbVegetationIndex.h"
// Software Guide : EndCodeSnippet


#include "itkExceptionObject.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"


int main( int argc, char *argv[] )
{
  if( argc < 6 )
    {
      std::cerr << "Missing Parameters " << std::endl;
      std::cerr << "Usage: " << argv[0];
      std::cerr << " inputImage1 , inputImage2 , outputImage , prettyinputImage1 , prettyinputImage2 , prettyOutput" << std::endl;
      return 1;
    }
  
  //  Software Guide : BeginLatex
  //  
  // The image types are now defined using pixel types and particular
  // dimension. Input and output images are defined as \doxygen{otb}{Image}.
  //
  //  Software Guide : EndLatex 
  
  // Software Guide : BeginCodeSnippet
  const unsigned int                              Dimension = 2;
  typedef double                                  InputPixelType;
  typedef float                                   OutputPixelType;
  typedef otb::Image<InputPixelType,Dimension>    InputRImageType;
  typedef otb::Image<InputPixelType,Dimension>    InputNIRImageType;
  typedef otb::Image<OutputPixelType,Dimension>   OutputImageType;
  // Software Guide : EndCodeSnippet
  
  // We instantiate reader and writer types
  typedef otb::ImageFileReader<InputRImageType>   RReaderType;
  typedef otb::ImageFileReader<InputNIRImageType> NIRReaderType;
  typedef otb::ImageFileWriter<OutputImageType>   WriterType;
  
  //  Software Guide : BeginLatex
  //  
  // The NDVI (Normalized Difference Vegetation Index) is instantiated using 
  // the images pixel type types as template parameters.
  //
  //  Software Guide : EndLatex 
  
  // Software Guide : BeginCodeSnippet
  typedef otb::Functor::NDVI< InputPixelType,
                              InputPixelType,
                              OutputPixelType>   FunctorType;
  // Software Guide : EndCodeSnippet
  
  //  Software Guide : BeginLatex
  //  
  // The \doxygen{otb}{RAndNIRVegetationIndexImageFilter} type is instantiated using the images
  // types and the NDVI functor as template parameters.
  //
  //  Software Guide : EndLatex 
  
  // Software Guide : BeginCodeSnippet
  typedef otb::RAndNIRVegetationIndexImageFilter<InputRImageType,
                                                 InputNIRImageType,
                                                 OutputImageType,   
                                                 FunctorType>       RAndNIRVegetationIndexImageFilterType;
  // Software Guide : EndCodeSnippet
  
  
  // Instantiating object
  RAndNIRVegetationIndexImageFilterType::Pointer filter    = RAndNIRVegetationIndexImageFilterType::New();
  RReaderType::Pointer                           readerR   = RReaderType::New();
  NIRReaderType::Pointer                         readerNIR = NIRReaderType::New();
  WriterType::Pointer                            writer    = WriterType::New();
  
  //  Software Guide : BeginLatex
  //  
  //  Now the input images is set and a name is given to the output image.
  //  
  //  Software Guide : EndLatex 
  
  // Software Guide : BeginCodeSnippet
  readerR->SetFileName( argv[1] );
  readerNIR->SetFileName( argv[2] );
  writer->SetFileName( argv[3]  );
  
  // Software Guide : EndCodeSnippet
  
  // Software Guide : BeginLatex
  //  
  // The filter inputs are linked to readers output and
  // the filter output is linked to the writer input.
  //
  // Software Guide : EndLatex 
  
  // Software Guide : BeginCodeSnippet
  filter->SetInputR( readerR->GetOutput() ); 
  filter->SetInputNIR( readerNIR->GetOutput() ); 
  
  writer->SetInput( filter->GetOutput() );
  // Software Guide : EndCodeSnippet
  
  
  // Software Guide : BeginLatex
  //  
  // The invocation of the \code{Update()} method on the writer triggers the
  // execution of the pipeline.  It is recommended to place update calls in a
  // \code{try/catch} block in case errors occur and exceptions are thrown.
  //
  // Software Guide : EndLatex 
  
  // Software Guide : BeginCodeSnippet
  try
    {
      writer->Update();
    }
  catch( itk::ExceptionObject & excep )
    {
      std::cerr << "Exception caught !" << std::endl;
      std::cerr << excep << std::endl;
    }
  // Software Guide : EndCodeSnippet
  catch( ... ) 
    { 
      std::cout << "Unknown exception !" << std::endl; 
      return EXIT_FAILURE;
    } 
  
  // Pretty image creation for the printing
  typedef otb::Image<unsigned char, Dimension>                                                  OutputPrettyImageType;
  typedef otb::ImageFileWriter<OutputPrettyImageType>                                           WriterPrettyType;
  typedef itk::RescaleIntensityImageFilter< OutputImageType, OutputPrettyImageType>             RescalerType;
  typedef itk::RescaleIntensityImageFilter< InputRImageType, OutputPrettyImageType>             RescalerRType;
  typedef itk::RescaleIntensityImageFilter< InputNIRImageType, OutputPrettyImageType>           RescalerNIRType;


  RescalerType::Pointer     rescaler     = RescalerType::New();                                       
  WriterPrettyType::Pointer prettyWriter = WriterPrettyType::New();
  rescaler->SetInput( filter->GetOutput() );
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  prettyWriter->SetFileName( argv[6] );
  prettyWriter->SetInput( rescaler->GetOutput() );

  RescalerRType::Pointer     rescalerR     = RescalerRType::New();
  RescalerNIRType::Pointer     rescalerNIR     = RescalerNIRType::New();
  WriterPrettyType::Pointer prettyWriterR = WriterPrettyType::New();
  WriterPrettyType::Pointer prettyWriterNIR = WriterPrettyType::New();
  rescalerR->SetInput( readerR->GetOutput() );
  rescalerR->SetOutputMinimum(0);
  rescalerR->SetOutputMaximum(255);
  prettyWriterR->SetFileName( argv[4] );
  prettyWriterR->SetInput( rescalerR->GetOutput() );                                       
 
  rescalerNIR->SetInput( readerNIR->GetOutput() );
  rescalerNIR->SetOutputMinimum(0);
  rescalerNIR->SetOutputMaximum(255);
  prettyWriterNIR->SetFileName( argv[5] );
  prettyWriterNIR->SetInput( rescalerNIR->GetOutput() );

    try
    {
      prettyWriter->Update();
      prettyWriterNIR->Update();
      prettyWriterR->Update();
    }
    catch( itk::ExceptionObject & excep )
      {
	std::cerr << "Exception caught !" << std::endl;
	std::cerr << excep << std::endl;
      }
    catch( ... ) 
      { 
	std::cout << "Unknown exception !" << std::endl; 
	return EXIT_FAILURE;
      } 
    
    
    // Software Guide : BeginLatex
    //
    // Let's now run this example using as input the images
    // \code{NDVI\_3.hdr} and  \code{NDVI\_4.hdr} (images kindly and free of charge given by the SISA and the CNES) 
    // and $\gamma=0.6$ provided in the directory \code{Examples/Data}.
    //
    //
    // \begin{figure} \center
    // \includegraphics[width=0.24\textwidth]{pretty_Red.eps}
    // \includegraphics[width=0.24\textwidth]{pretty_NIR.eps}
    // \includegraphics[width=0.24\textwidth]{pretty_NDVIRAndNIRVegetationIndex.eps}
    // \itkcaption[ARVI Example]{NDVI input images on the right (Red channel and NIR channel), on the left the result of the algorithm.}
    // \label{fig:NDVIRAndNIRVegetationIndex}
    // \end{figure}
    //
    // Software Guide : EndLatex 
    
    return EXIT_SUCCESS;
}

