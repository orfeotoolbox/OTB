/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CNES - J.Inglada
  Language  :   C++
  Date      :   14 April 2006
  Version   :   
  $Id$

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif


#include "itkExceptionObject.h"
#include "itkImage.h"
#include "itkImageFileWriter.h"
#include <iostream>

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"


//  Software Guide : BeginCommandLineArgs
//    INPUTS: {amst.png}
//    OUTPUTS: {amstRatioLineDet.png}, {amstRatioLineDir.png} 
//    3 10
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the \doxygen{otb::RatioLineDetector}.
// This filter is used for line detection in SAR images. Its principle
// is described in \cite{tup-98}: a line is detected if two parallel
// edges are present in the images. These edges are detected with the
// ratio of means detector.
//
// The first step required to use this filter is to include its header file. 
//
// Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
#include "otbLineRatioDetector.h"
// Software Guide : EndCodeSnippet



int main( int argc, char* argv[] )
{
    if( argc != 6 )
    {
    std::cerr << "Usage: " << argv[0] << " inputImageFile ";
    std::cerr << " outputLinesImageFile outputDirectionsImageFile lineWidth lineLength" << std::endl;  
    return EXIT_FAILURE;
    }

    const char * inputFilename  = argv[1];
    const char * outputLinesFilename = argv[2];
    const char * outputDirsFilename = argv[3];
    
    // Width of the linear feature = 2*WidthLine+1
    unsigned int  WidthLine((unsigned int)::atoi(argv[4]));
    // Length of the linear feature = 2*LengthLine+1
    unsigned int  LengthLine((unsigned int)::atoi(argv[5]));
    
  
  //  Software Guide : BeginLatex
  //
  //  Then we must decide what pixel type to use for the image. We
  //  choose to make all computations with floating point precision
  //  and rescale the results between 0 and 255 in order to export PNG images.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet

        
    typedef float                    InputPixelType;
    typedef unsigned char            OutputPixelType;
    const   unsigned int             Dimension = 2;

    typedef itk::Image< InputPixelType,  Dimension >        InternalImageType;
    typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

    typedef otb::ImageFileReader< InternalImageType  >         ReaderType;
    typedef otb::ImageFileWriter< OutputImageType >         WriterType;

    // Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    //  
    // The intensity rescaling of the results will be carried out by the
    // \code{itk::RescaleIntensityImageFilter} which is templated by the
    // input and output image types.
    //
    //  Software Guide : EndLatex 

    // Software Guide : BeginCodeSnippet
    typedef itk::RescaleIntensityImageFilter< InternalImageType,
                                            OutputImageType > RescalerType;

    RescalerType::Pointer rescaler = RescalerType::New();

    // Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    //  
    // We can now instantiate the line detector. It is templated over
    // the input and output image types.
    //
    //  Software Guide : EndLatex 

    // Software Guide : BeginCodeSnippet

    typedef otb::LineRatioDetector< InternalImageType, InternalImageType >   FilterType;
	
    FilterType::Pointer filterLineRatio = FilterType::New();
    // Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    //  
    // The line detector needs 2 parameters: the width of the
    // lines that the user wants to detect and the minimal length of
    // the straight segments.
    //
    //  Software Guide : EndLatex 

    // Software Guide : BeginCodeSnippet
        
    filterLineRatio->SetWidthLine( WidthLine );
    filterLineRatio->SetLengthLine( LengthLine );
    // Software Guide : EndCodeSnippet
	
    ReaderType::Pointer reader = ReaderType::New();
    WriterType::Pointer writer = WriterType::New();

    reader->SetFileName( inputFilename  );
    writer->SetFileName( outputLinesFilename );

    //  Software Guide : BeginLatex
    //
    //  The \code{itk::RescaleIntensityImageFilter} needs to know which
    //  is the minimu and maximum values of the output generated
    //  image. Those can be chosen in a generic way by using the
    //  \code{NumericTraits} functions, since they are templated over
    //  the pixel type.
    //  
    //  Software Guide : EndLatex 


    // Software Guide : BeginCodeSnippet  
    rescaler->SetOutputMinimum( itk::NumericTraits< OutputPixelType >::min());
    rescaler->SetOutputMaximum( itk::NumericTraits< OutputPixelType >::max());
    // Software Guide : EndCodeSnippet


    //  Software Guide : BeginLatex
    //  
    // The pipeline is connected as usual using the \code{SetInput}
    // and \code{GetOutput} methods.
    //
    //  Software Guide : EndLatex 

    // Software Guide : BeginCodeSnippet
    
    filterLineRatio->SetInput( reader->GetOutput() );
    rescaler->SetInput( filterLineRatio->GetOutput() );
    writer->SetInput( rescaler->GetOutput() );
        
    writer->Update();

  // Software Guide : EndCodeSnippet

    writer->SetFileName( outputDirsFilename );

    //  Software Guide : BeginLatex
    //  
    // We can also obtain the directions of the lines by calling the
    // \code{GetOutputDirection} method.
    //
    //  Software Guide : EndLatex 

    // Software Guide : BeginCodeSnippet

    rescaler->SetInput( filterLineRatio->GetOutputDirection() );
    writer->Update();

  // Software Guide : EndCodeSnippet

      //  Software Guide : BeginLatex
  // Figure~\ref{fig:RATIO_LINE_FILTER} shows the result of applying
  // the ratio line detector
  // filter to a SAR image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.25\textwidth]{amst.eps}
  // \includegraphics[width=0.25\textwidth]{amstRatioLineDet.eps}
  // \includegraphics[width=0.25\textwidth]{amstRatioLineDir.eps}
  // \itkcaption[Ratio Line Detector Application]{Result of applying the
  // \doxygen{otb::LineRatioDetector} to a SAR image. From left to right :
  // original image, line detection intensity and line orientation.} 
  // \label{fig:RATIO_LINE_FILTER}
  // \end{figure}
  //
  //  Software Guide : EndLatex 


  return EXIT_SUCCESS;
}


