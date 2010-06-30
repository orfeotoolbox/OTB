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
//    INPUTS: {ADS40RoiSmall.png}
//    OUTPUTS: {TextureOutput.tif}, {pretty_TextureOutput.png}
//    2 1 1
//  Software Guide : EndCommandLineArgs

#include "itkExceptionObject.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

// Pretty RGB output
#include "otbVectorImage.h"
#include "otbImageToVectorImageCastFilter.h"
#include "otbVectorRescaleIntensityImageFilter.h"

// Software Guide : BeginLatex
//
// This example illustrates the use of the
// \doxygen{otb}{ScalarImageToTexturesFilter},
// which compute the standard Haralick's textural features presented in table~\ref{tab:haralickStandardFeatures}.
//
// \begin{table}
// \begin{tabular}{|c|c|}
// \hline
// Energy & $ f_1 = \sum_{i,j}g(i, j)^2 $ \\
// \hline
// Entropy & $ f_2 = -\sum_{i,j}g(i, j) \log_2 g(i, j)$, or 0 if $g(i, j) = 0$ \\
// \hline
// Correlation & $ f_3 = \sum_{i,j}\frac{(i - \mu)(j - \mu)g(i, j)}{\sigma^2} $ \\
// \hline
// Difference Moment &  $f_4 = \sum_{i,j}\frac{1}{1 + (i - j)^2}g(i, j) $ \\
// \hline
// Inertia (a.k.a. Contrast) & $ f_5 = \sum_{i,j}(i - j)^2g(i, j) $ \\
// \hline
// Cluster Shade & $ f_6 = \sum_{i,j}((i - \mu) + (j - \mu))^3 g(i, j) $ \\
// \hline
// Cluster Prominence & $ f_7 = \sum_{i,j}((i - \mu) + (j - \mu))^4 g(i, j) $ \\
// \hline
// Haralick's Correlation & $ f_8 = \frac{\sum_{i,j}(i, j) g(i, j) -\mu_t^2}{\sigma_t^2} $ \\
// \hline
// \end{tabular}
// \itkcaption[Haralick features available in] {\doxygen{otb}{ScalarImageToTexturesFilter}}
//
// \label{tab:haralickStandardFeatures}
// \end{table}
// where $\mu_t$ and $\sigma_t$ are the mean and standard deviation of the row
// (or column, due to symmetry) sums, $ \mu =  $ (weighted pixel average) 
// $ = \sum_{i,j}i \cdot g(i, j) =\sum_{i,j}j \cdot g(i, j) $ due to matrix summetry, and 
// $ \sigma =  $ (weighted pixel variance) $ = \sum_{i,j}(i - \mu)^2 \cdot g(i, j) =\sum_{i,j}(j - \mu)^2 \cdot g(i, j)  $  
// due to matrix symmetry
//
// More features are available in \doxygen{otb}{ScalarImageToAdvancedTexturesFilter}.
// \relatedClasses
// \begin{itemize}
// \item \doxygen{otb}{ScalarImageToAdvancedTexturesFilter}
// \item \doxygen{otb}{ScalarImageToPanTexTextureFilter}
// \item \doxygen{otb}{MaskedScalarImageToGreyLevelCooccurrenceMatrixGenerator}
// \item \doxygen{itk}{GreyLevelCooccurrenceMatrixTextureCoefficientsCalculator}
// \item \doxygen{otb}{GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator}
// \end{itemize}

// The first step required to use the filter is to include the header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbScalarImageToTexturesFilter.h"
// Software Guide : EndCodeSnippet

int main(int argc, char * argv[])
{
  // Parse command line parameters
  if (argc != 7)
    {
    std::cerr << "Usage: " << argv[0] << " <inputImage> ";
    std::cerr << " <outputImage> <outputRescaled> ";
    std::cerr << " <radius> <xOffset> <yOffset> ";
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
  typedef otb::Image<PixelType, Dimension> ImageType;

  // Software Guide : BeginLatex
  //
  // After defining the types for the pixels and the images used in the
  // example, we define the types for the textures filter. It is
  // templated by the input and output image types.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ScalarImageToTexturesFilter
    <ImageType,ImageType> TexturesFilterType;
  // Software Guide : EndCodeSnippet

  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  ReaderType::Pointer reader  = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(infname);
  writer->SetFileName(outfname);

  // Software Guide : BeginLatex
  //
  // We can now instantiate the filters.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  TexturesFilterType::Pointer texturesFilter
    = TexturesFilterType::New();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The texture filters takes at least 2 parameters: the radius of the
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
  typedef ImageType::SizeType SizeType;
  SizeType sradius;
  sradius.Fill(radius);

  texturesFilter->SetRadius(sradius);

    typedef ImageType::OffsetType OffsetType;
    OffsetType offset;
    offset[0] =  xOffset;
    offset[1] =  yOffset;

    texturesFilter->SetOffset(offset);
    // Software Guide : EndCodeSnippet

    // Software Guide : BeginLatex
    //
    // The textures filter will automatically derive the optimal
    // bin size for co-occurences histogram, but they need to know
    // the input image minimum and maximum. These values can be set
    // like this :
    // Software Guide : EndLatex
    // Software Guide : BeginCodeSnippet
    texturesFilter->SetInputImageMinimum(0);
    texturesFilter->SetInputImageMaximum(255);
    // Software Guide : EndCodeSnippet

    // Software Guide : BeginLatex
    //
    // To tune co-occurence histogram resolution, you can use
    // the SetNumberOfBinsPerAxis() method.
    //
    // Software Guide : EndLatex

    // Software Guide : BeginLatex
    //
    // We can now plug the pipeline.
    //
    // Software Guide : EndLatex
    // Software Guide : BeginCodeSnippet
    texturesFilter->SetInput(reader->GetOutput());

    writer->SetInput(texturesFilter->GetInertiaOutput());
    writer->Update();
    // Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    // Figure~\ref{fig:TEXTUREFUNCTOR} shows the result of applying
    // the contrast texture computation.
    // \begin{figure}
    // \center
    // \includegraphics[width=0.40\textwidth]{ADS40RoiSmall.eps}
    // \includegraphics[width=0.40\textwidth]{pretty_TextureOutput.eps}
    // \itkcaption[Texture Functor]{Result of applying the
    // \doxygen{otb}{ContrastTextureFunctor} to an image. From left to right :
    // original image, contrast.}
    // \label{fig:TEXTUREFUNCTOR}
    // \end{figure}
    //
    //  Software Guide : EndLatex

    // Pretty image creation for printing

    typedef otb::VectorImage<double,2> VectorImageType;
    typedef otb::VectorImage<unsigned char,2 > PrettyVectorImageType;
    typedef otb::ImageFileWriter<PrettyVectorImageType>
    WriterPrettyOutputType;

    typedef otb::ImageToVectorImageCastFilter<ImageType,VectorImageType> VectorCastFilterType;
    typedef otb::VectorRescaleIntensityImageFilter<VectorImageType,PrettyVectorImageType>
    RescalerOutputType;

    VectorCastFilterType::Pointer   vectorCast         = VectorCastFilterType::New();
    RescalerOutputType::Pointer     outputRescaler     = RescalerOutputType::New();
    WriterPrettyOutputType::Pointer prettyOutputWriter =
    WriterPrettyOutputType::New();
    VectorCastFilterType::Pointer vectorCastFilter = VectorCastFilterType::New();
    vectorCastFilter->SetInput(texturesFilter->GetInertiaOutput());
    outputRescaler->SetInput(vectorCastFilter->GetOutput());

    PrettyVectorImageType::PixelType min(1),max(1);
    min.Fill(0);
    max.Fill(255);

    outputRescaler->SetOutputMinimum(min);
    outputRescaler->SetOutputMaximum(max);

    prettyOutputWriter->SetFileName(outprettyfname);
    prettyOutputWriter->SetInput(outputRescaler->GetOutput());

    prettyOutputWriter->Update();
    return EXIT_SUCCESS;
}
