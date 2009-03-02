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
//    INPUT: {suburb2.jpeg}
//    OUTPUT: {PanTexOutput.tif}, {pretty_PanTexOutput.png}
//  Software Guide : EndCommandLineArgs

#include "itkExceptionObject.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"



// Software Guide : BeginLatex
//
// This example illustrates the use of the
// \doxygen{otb}{PanTexTextureImageFilter}. This texture parameter was
// first introduced in \cite{PanTex} and is very useful for urban area
// detection. 
// \relatedClasses
//  \begin{itemize}
//  \item \doxygen{otb}{ContrastTextureFunctor}
//  \end{itemize}
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbPanTexTextureImageFilter.h"
// Software Guide : EndCodeSnippet


int main(int argc, char * argv[])
{
  // Parse command line parameters
  if ( argc != 4 )
  {
    std::cerr << "Usage: " << argv[0] << " <inputImage> ";
    std::cerr << " <outputImage> <outputRescaled> ";
    std::cerr << std::endl;
    return EXIT_FAILURE;
  }

  const char* infname   = argv[1];
  const char* outfname  = argv[2];
  const char* outprettyfname  = argv[3];

  const unsigned int radius  =  static_cast<unsigned int>(atoi(argv[4]));
  const unsigned int xOffset =  static_cast<unsigned int>(atoi(argv[5]));
  const unsigned int yOffset =  static_cast<unsigned int>(atoi(argv[6]));


  typedef double PixelType;
  const int Dimension = 2;
  typedef otb::Image<PixelType,Dimension> ImageType;

  // Software Guide : BeginLatex
//
// After defining the types for the pixels and the images used in the
// example, we define the type for the texture functor. It is
// templated by the input and output pixel types.
//
// Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::Functor::ContrastTextureFunctor<PixelType, PixelType>
                                                                FunctorType;
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
//
// The filter for computing the texture features for a complete image
// is templated by the input and output image types and, of course,
// the functor type.
//
// Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::UnaryFunctorNeighborhoodWithOffsetImageFilter<ImageType,
                                          ImageType, FunctorType> FilterType;

  // Software Guide : EndCodeSnippet
  typedef otb::ImageFileReader<ImageType>  ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  ReaderType::Pointer reader  = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(infname);
  writer->SetFileName(outfname);

    // Software Guide : BeginLatex
//
// We can now instatiate the filter.
//
// Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  FilterType::Pointer textureFilter = FilterType::New();
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
//
// The texture filter takes 2 parameters: the radius of the
// neighborhood on which the texture will be computed and the offset
// used. Texture features are bivariate statistics, that is, they are
// computed using pair of pixels. Each texture feature is defined for
// an offset defining the pixel pair.
//
// The radius parameter can be passed to the filter as a scalar
// parameter if the neighborhood is square, or as \code{SizeType} in
// any case.
//
// The offset is always an array of N values, where N is the number of
// dimensions of the image.  
//
// Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet  
  textureFilter->SetRadius(radius);

  typedef ImageType::OffsetType OffsetType;
  OffsetType offset;
  offset[0] =  xOffset;
  offset[1] =  yOffset;

  textureFilter->SetOffset(offset);

  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
//
// We can now plug the pipeline and trigger the execution by calling
// the \code{Update} method of the writer.
//
// Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet    
  textureFilter->SetInput(reader->GetOutput());
  writer->SetInput(textureFilter->GetOutput());

  writer->Update();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  // Figure~\ref{fig:PANTEXFUNCTOR} shows the result of applying
  // the PanTex computation.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.40\textwidth]{suburb2.eps}
  // \includegraphics[width=0.40\textwidth]{pretty_PanTexOutput.eps}
  // \itkcaption[PanTex Functor]{Result of applying the
  // \doxygen{otb}{ContrastTextureFunctor} to an image. From left to right :
  // original image, contrast.}
  // \label{fig:PANTEXFUNCTOR}
  // \end{figure}
  //
  //  Software Guide : EndLatex

 // Pretty image creation for printing

  typedef otb::Image<unsigned char, Dimension>                                           OutputPrettyImageType;
  typedef otb::ImageFileWriter<OutputPrettyImageType>                                    WriterPrettyOutputType;
  typedef itk::RescaleIntensityImageFilter< ImageType, OutputPrettyImageType>      RescalerOutputType;

  RescalerOutputType::Pointer     outputRescaler     = RescalerOutputType::New();
  WriterPrettyOutputType::Pointer prettyOutputWriter = WriterPrettyOutputType::New();
  outputRescaler->SetInput( textureFilter->GetOutput() );
  outputRescaler->SetOutputMinimum(0);
  outputRescaler->SetOutputMaximum(255);
  prettyOutputWriter->SetFileName( outprettyfname );
  prettyOutputWriter->SetInput( outputRescaler->GetOutput() );

  prettyOutputWriter->Update();
  return EXIT_SUCCESS;
}
