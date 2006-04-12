/*=========================================================================

  Program   :   OTB (ORFEO ToolBox)
  Authors   :   CNES - J. Inglada
  Language  :   C++
  Date      :   7 April 2006
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


//  Software Guide : BeginCommandLineArgs
//    INPUTS: {ROISpot5.png}
//    2 2
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the \doxygen{otb::ComplexMomentImageFunction}.
// 
// The first step required to use this filter is to include its header file. 
//
// Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
#include "otbComplexMomentImageFunction.h"
// Software Guide : EndCodeSnippet

int main(int argc, char ** argv )
{
    if( argc != 4 )
    {
    std::cerr << "Usage: " << argv[0] << " inputImageFile ";
    std::cerr << " p q" << std::endl;  
    return EXIT_FAILURE;
    }

    const char * inputFilename  = argv[1];
        
    unsigned int P((unsigned char)::atoi(argv[2]));
    unsigned int Q((unsigned char)::atoi(argv[3]));

	        
    typedef unsigned char     InputPixelType;
    const   unsigned int      Dimension = 2;
	
    typedef itk::Image< InputPixelType,  Dimension >   InputImageType;

    typedef otb::ImageFileReader< InputImageType  >    ReaderType;

    ReaderType::Pointer reader = ReaderType::New();

    reader->SetFileName(inputFilename);

    //  Software Guide : BeginLatex
    //
    //  The \doxygen{otb::ComplexMomentImageFunction} is templated over the
    //  input image type and the output complex type value, so we start by
    //  defining:
    //
    //  Software Guide : EndLatex 
    
    // Software Guide : BeginCodeSnippet
    typedef std::complex<float>                             ComplexType;
    typedef otb::ComplexMomentImageFunction<InputImageType,ComplexType>   CMType;

    CMType::Pointer cmFunction =CMType::New();
    
    // Software Guide : EndCodeSnippet

    
    
    //  Software Guide : BeginLatex
    //
    // Next, we plug the input image into the complex moment fucntion
    // and we set its parameters.
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet

    reader->Update();
    cmFunction->SetInputImage( reader->GetOutput() );
    cmFunction->SetQ(Q);
    cmFunction->SetP(P);

    // Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    // We can chose the pixel of the image which will used as center
    // for the moment computation
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet


    InputImageType::IndexType center;
    center[0]=50;
    center[1]=50;

    // Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    // We can also choose the size of the neighborhood around the
    // center pixel for the moment computation.
    //
    //  Software Guide : EndLatex


    cmFunction->SetNeighborhoodRadius( 15 );


    
    //  Software Guide : BeginLatex
    // In order to get the value of the moment, we call the
    // \code{EvaluateAtIndex} method.
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet

    ComplexType Result = cmFunction->EvaluateAtIndex(center);

    std::cout << "The moment of order (" << P << "," << Q << ") is equal to " << Result << std:: endl;

    // Software Guide : EndCodeSnippet



  return EXIT_SUCCESS;
}

