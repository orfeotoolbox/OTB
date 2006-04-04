/*=========================================================================

  Program   :   OTB (ORFEO ToolBox)
  Authors   :   CNES - J. Inglada
  Language  :   C++
  Date      :   29 march 2006
  Version   :   
  Role      :   
  $Id:$

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include "itkImage.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {ROISpot5.png}
//    OUTPUTS: {ROISpot5Harris.png}
//    1.5 2 0.1
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the \doxygen{HarrisImageFilter}.
// 
// The first step required to use this filter is to include its header file. 
//
// Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
#include "otbHarrisImageFilter.h"
// Software Guide : EndCodeSnippet
#include "itkRescaleIntensityImageFilter.h"

int main(int argc, char ** argv )
{
    if( argc != 6 )
    {
    std::cerr << "Usage: " << argv[0] << " inputImageFile ";
    std::cerr << " outputHarrisImageFile sigmaD sigmaI alpha" << std::endl;  
    return EXIT_FAILURE;
    }

    const char * inputFilename  = argv[1];
    const char * outputFilename = argv[2];
        
    double SigmaD((double)::atof(argv[3]));
    double SigmaI((double)::atof(argv[4]));
    double Alpha((double)::atof(argv[5]));
	        
    typedef float                                   InputPixelType;
    const   unsigned int        	                        Dimension = 2;
    typedef unsigned char   	                        OutputPixelType;
	
    typedef itk::Image< InputPixelType,  Dimension >   InputImageType;
    typedef itk::Image< OutputPixelType, Dimension >   OutputImageType;

    typedef otb::ImageFileReader< InputImageType  >    ReaderType;

    //  Software Guide : BeginLatex
    //
    //  The \doxygen{HarrisImageFilter} is templated over the
    //  input and output image types, so we start by
    //  defining:
    //
    //  Software Guide : EndLatex 
    
    // Software Guide : BeginCodeSnippet
    typedef otb::HarrisImageFilter<InputImageType,
                                     InputImageType>   HarrisFilterType;

    // Software Guide : EndCodeSnippet
    typedef itk::RescaleIntensityImageFilter
                   < InputImageType, OutputImageType > RescalerType; 

    typedef otb::ImageFileWriter< OutputImageType >    WriterType;
  

    ReaderType::Pointer reader   = ReaderType::New();
    WriterType::Pointer writer   = WriterType::New();
    HarrisFilterType::Pointer harris = HarrisFilterType::New();
    RescalerType::Pointer rescaler = RescalerType::New();
	
    reader->SetFileName( inputFilename  );
    writer->SetFileName( outputFilename );

    harris->SetInput( reader->GetOutput() );
    
    //  Software Guide : BeginLatex
    //
    // The \doxygen{HarrisImageFilter} needs some parameters to
    // operate. The derivative computation is performed by a
    // convolution with the derivative of a Gaussian kernel of
    // variance $\sigma_D$ (derivation scale) and 
    // the smoothing of the image is performed by convolving with a
    // Gaussian kernel of variance $\sigma_I$ (integration
    // scale). This allows the coputation of the following matrix:
    // \begin{equation}
    // \mu(\mathbf{x},\sigma_I,\sigma_D) = \sigma_D^2 g(\sigma_I)\star
    // \left[\begin{array}{cc} L_x^2(\mathbf{x},\sigma_D) &
    // L_xL_y^2(\mathbf{x},\sigma_D)\\ L_xL_y^2(\mathbf{x},\sigma_D)&
    // L_y^2(\mathbf{x},\sigma_D) \end{array}\right] \end{equation}
    // The output of the detector is $$det(\mu) - \alpha trace^2(\mu)$$.
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    harris->SetSigmaD( SigmaD );
    harris->SetSigmaI( SigmaI );
    harris->SetAlpha( Alpha );
    // Software Guide : EndCodeSnippet
    
    rescaler->SetOutputMinimum( itk::NumericTraits< OutputPixelType >::min());
    rescaler->SetOutputMaximum( itk::NumericTraits< OutputPixelType >::max());

    rescaler->SetInput( harris->GetOutput() );
    writer->SetInput( rescaler->GetOutput() );
    writer->Update(); 

    //  Software Guide : BeginLatex
  // Figure~\ref{fig:Harris} shows the result of applying the interest
  // point detector to a small patch extracted from a Spot 5 image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.25\textwidth]{ROISpot5.eps}
  // \includegraphics[width=0.25\textwidth]{ROISpot5Harris.eps}
  // \itkcaption[Lee Filter Application]{Result of applying the
  // \doxygen{HarrisImageFilter} to a Spot 5 image.} 
  // \label{fig:Harris}
  // \end{figure}
  //
  //  Software Guide : EndLatex 


  return EXIT_SUCCESS;
}

