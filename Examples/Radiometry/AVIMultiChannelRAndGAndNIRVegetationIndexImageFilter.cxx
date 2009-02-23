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
//  INPUTS: {qb_toulouse_sub.tif}
//  OUTPUTS: {AVIMultiChannelRAndGAndNIRVegetationIndex.tif} , {pretty_qb_toulouse_sub.png} , {pretty_AVIMultiChannelRAndGAndNIRVegetationIndex.png}
//  3 2 4 660 560 830
//  Software Guide : EndCommandLineArgs



// Software Guide : BeginLatex
//
// \index{otb::MultiChannelRAndGAndNIRVegetationIndexImageFilter}
// \index{otb::MultiChannelRAndGAndNIRVegetationIndexImageFilter!header}
// \index{otb::VegetationIndex}
// \index{otb::VegetationIndex!header}
//
//
// The following example illustrates the use of the
// otb::MultiChannelRAndGAndNIR VegetationIndexImageFilter with the
// use of the Angular Vegetation Index (AVI).
// The equation for the Angular Vegetation Index involves the gren, red 
// and near infra-red bands. $lambda_1$, $lambda_2$ and $lambda_3$ are the mid-band
// wavelengths for the green, red and NIR bands and $\tan^{-1}$ is the arctangent function.
//
// The AVI expression is
//
// \begin{equation}
// \mathbf{fact1} = \frac{\{lambda_3}-{lambda_2}}{lambda_2}}
// \end{equation}
// \begin{equation}
// \mathbf{fact2} = \frac{\{lambda_2}-{lambda_1}}{lambda_2}}
// \end{equation}
//
// \begin{equation}
// \mathbf{AVI} = \tan^-1 {[\frac{{fact1}{{NIR}-{R}}}} + tan^-1{\frac{{fact2}{{G}-{R}}}}
// \end{equation}
//
// For more details, refer to Plummer work \cite{AVI}.
//
// With the
// \doxygen{otb}{MultiChannelRAndGAndNIRVegetationIndexImageFilter}
// class the input has to be a multi channel image and the user has to
// specify the channel index of the red, green and NIR channel.
//
// Let's look at the minimal code required to use this
// algorithm. First, the following header defining the
// \doxygen{otb}{MultiChannelRAndGAndNIRVegetationIndexImageFilter}
// class must be included.
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbMultiChannelRAndGAndNIRVegetationIndexImageFilter.h"
// Software Guide : EndCodeSnippet


#include "itkExceptionObject.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "otbMultiChannelExtractROI.h"
#include "itkThresholdImageFilter.h"


int main( int argc, char *argv[] )
{
  if ( argc < 11 )
  {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage , outputImage , prettyInput , prettyOutput , redChannel , greenChannel , nirChannel ,";
    std::cerr << " lambdaR, lambdaG, lambdaNIR " << std::endl;
    return 1;
  }

  //  Software Guide : BeginLatex
  //
  // The image types are now defined using pixel types and
  // dimension. The input image is defined as an \doxygen{otb}{VectorImage},
  // the output is a \doxygen{otb}{Image}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  const unsigned int                                  Dimension = 2;
  typedef double                                      InputPixelType;
  typedef float                                       OutputPixelType;
  typedef otb::VectorImage<InputPixelType ,Dimension> InputImageType;
  typedef otb::Image<OutputPixelType,Dimension>       OutputImageType;
  // Software Guide : EndCodeSnippet

  // We instantiate reader and writer types
  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  //  Software Guide : BeginLatex
  //
  // The AVI (Angular Vegetation Index) is
  // instantiated using the image pixel types as template parameters.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef  otb::Functor::AVI< InputPixelType, InputPixelType,
                              InputPixelType,  OutputPixelType > FunctorType;
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  // The
  // \doxygen{otb}{MultiChannelRAndGAndNIRVegetationIndexImageFilter}
  // type is defined using the image types and the AVI functor as
  // template parameters. We then instantiate the filter itself.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::MultiChannelRAndGAndNIRVegetationIndexImageFilter
                           <InputImageType, OutputImageType,FunctorType>
                      MultiChannelRAndGAndNIRVegetationIndexImageFilterType;

  MultiChannelRAndGAndNIRVegetationIndexImageFilterType::Pointer
  filter = MultiChannelRAndGAndNIRVegetationIndexImageFilterType::New();
  // Software Guide : EndCodeSnippet

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  //  Software Guide : BeginLatex
  //
  //  Now the input image is set and a name is given to the output image.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  reader->SetFileName( argv[1] );
  writer->SetFileName( argv[2] );
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // The three used index bands (red, green and NIR) are declared.
  //
  //  Software Guide : EndLatex


  // Software Guide : BeginCodeSnippet
  filter->SetRedIndex(::atoi(argv[5]));
  filter->SetGreenIndex(::atoi(argv[6]));
  filter->SetNIRIndex(::atoi(argv[7]));
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // The $\lambda$ R,G and NIR parameters are set. The
  // \doxygen{otb}{MultiChannelRAndGAndNIRVegetationIndexImageFilter}
  // class sets the default values of $\lambda$ to $660$, $560$ and 
  // $830$. 
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->GetFunctor().SetLambdaR(::atof(argv[8]));
  filter->GetFunctor().SetLambdaG(::atof(argv[9]));
  filter->GetFunctor().SetLambdaNir(::atof(argv[10]));
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // The filter input is linked to the reader output and
  // the filter output is linked to the writer input.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetInput( reader->GetOutput() );

  writer->SetInput( filter->GetOutput() );
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The invocation of the \code{Update()} method on the writer triggers the
  //  execution of the pipeline.  It is recommended to place update calls in a
  //  \code{try/catch} block in case errors occur and exceptions are thrown.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  try
  {
    writer->Update();
  }
  catch ( itk::ExceptionObject & excep )
  {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
  }
  // Software Guide : EndCodeSnippet
  catch ( ... )
  {
    std::cout << "Unknown exception !" << std::endl;
    return EXIT_FAILURE;
  }


  // Pretty image creation for the printing
  typedef otb::Image<unsigned char, Dimension>                                                  OutputPrettyImageType;
  typedef otb::VectorImage<unsigned char, Dimension>                                            OutputVectorPrettyImageType;
  typedef otb::ImageFileWriter<OutputVectorPrettyImageType>                                     WriterVectorPrettyType;
  typedef otb::ImageFileWriter<OutputPrettyImageType>                                           WriterPrettyType;
  typedef itk::RescaleIntensityImageFilter< OutputImageType, OutputPrettyImageType>             RescalerType;
  typedef otb::VectorRescaleIntensityImageFilter< InputImageType ,OutputVectorPrettyImageType > VectorRescalerType;
  typedef otb::MultiChannelExtractROI<unsigned char , unsigned char>                            ChannelExtractorType;


  VectorRescalerType::Pointer vectRescaler         = VectorRescalerType::New();
  ChannelExtractorType::Pointer selecter           = ChannelExtractorType::New();
  WriterVectorPrettyType::Pointer vectPrettyWriter = WriterVectorPrettyType::New();

  OutputVectorPrettyImageType::PixelType minimum,maximum;
  minimum.SetSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  maximum.SetSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  minimum.Fill(0);
  maximum.Fill(255);
  vectRescaler->SetOutputMinimum(minimum);
  vectRescaler->SetOutputMaximum(maximum);
  vectRescaler->SetClampThreshold(0.01);
  vectRescaler->SetInput( reader->GetOutput() );

  selecter->SetInput(vectRescaler->GetOutput());
  selecter->SetChannel(1);
  selecter->SetChannel(2);
  selecter->SetChannel(3);

  vectPrettyWriter->SetFileName( argv[3] );
  vectPrettyWriter->SetInput( selecter->GetOutput() );


  typedef itk::ThresholdImageFilter< OutputImageType >   ThresholderType;

  ThresholderType::Pointer thresholder = ThresholderType::New();
  thresholder->SetInput(  filter->GetOutput() );
  thresholder->SetOutsideValue( 1.0 );
  thresholder->ThresholdOutside( 0.0, 1.0 );
  thresholder->Update();

  RescalerType::Pointer     rescaler     = RescalerType::New();
  WriterPrettyType::Pointer prettyWriter = WriterPrettyType::New();
  rescaler->SetInput( thresholder->GetOutput() );
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  prettyWriter->SetFileName( argv[4] );
  prettyWriter->SetInput( rescaler->GetOutput() );

  try
  {
    prettyWriter->Update();
    vectPrettyWriter->Update();
  }
  catch ( itk::ExceptionObject & excep )
  {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
  }
  catch ( ... )
  {
    std::cout << "Unknown exception !" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;

  // Software Guide : BeginLatex
  //
  // Let's now run this example using as input the image
  // \code{IndexVegetation.hd} and $\gamma$=0.6 provided in the
  // directory \code{Examples/Data}.
  //
  //
  // \begin{figure} \center
  // \includegraphics[width=0.24\textwidth]{pretty_qb_toulouse_sub.eps}
  // \includegraphics[width=0.24\textwidth]{pretty_AVIMultiChannelRAndGAndNIRVegetationIndex.eps}
  // \itkcaption[AVI Example]{AVI result on the right with the left image in input.}
  // \label{fig:AVIMultiChannelRAndGAndNIRVegetationIndexImageFilter}
  // \end{figure}
  //
  //  Software Guide : EndLatex

}

