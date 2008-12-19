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
//  OUTPUTS: {DEMToRainbowImageGenerator.eps}
//  6.5 45.5 500 500 0.002 -0.002 ${OTB_DATA_ROOT}/Examples/DEM_srtm
//  Software Guide : EndCommandLineArgs


// Software Guide : BeginLatex
//
// In some situation, it is desirable to represent a gray level image in color for easier
// interpretation. This is particularly the case if pixel values in the image are used
// to represent some data such as elevation, deformation map, interferogram.
//
// In this case, it is important to ensure that similar values will get similar colors.
//
// The following example illustrates the use of the \doxygen{otb}{DEMToImageGenerator} class
// combined with the \doxygen{otb}{ScalarToRainbowRGBPixelFunctor}. You can refer to the
// source code or to section \ref{sec:ReadDEM} for the DEM conversion to image,
// we will focus on the color conversion part here.
//
// Software Guide : EndLatex


#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"

#include "itkUnaryFunctorImageFilter.h"
#include "otbScalarToRainbowRGBPixelFunctor.h"
#include "otbDEMToImageGenerator.h"




int main(int argc, char * argv[])
{

  if(argc<9)
  {
    std::cout << argv[0] <<" <output_filename> <Longitude Output Orign point>";
        std::cout << " <Latitude Output Origin point> <X Output Size> <Y Output size>";
        std::cout << " <X Spacing> <Y Spacing> <DEM folder path>"  << std::endl;
    return EXIT_FAILURE;
  }


  typedef double PixelType;
  typedef unsigned char UCharPixelType;
  typedef itk::RGBPixel<UCharPixelType> RGBPixelType;
  typedef otb::Image<PixelType, 2> ImageType;
  typedef otb::Image<RGBPixelType, 2> RGBImageType;
  typedef otb::StreamingImageFileWriter<RGBImageType> WriterType;

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(argv[1]);


  typedef otb::DEMToImageGenerator<ImageType>      DEMToImageGeneratorType;

  DEMToImageGeneratorType::Pointer demToImage = DEMToImageGeneratorType::New();

  typedef DEMToImageGeneratorType::SizeType        SizeType;
  typedef DEMToImageGeneratorType::SpacingType     SpacingType;
  typedef DEMToImageGeneratorType::DEMHandlerType  DEMHandlerType;
  typedef DEMHandlerType::PointType                PointType;

  demToImage->SetDEMDirectoryPath(argv[8]);

  PointType origin;
  origin[0] = ::atof(argv[2]);
  origin[1] = ::atof(argv[3]);

  demToImage->SetOutputOrigin(origin);

  SizeType size;
  size[0] = ::atoi(argv[4]);
  size[1] = ::atoi(argv[5]);

  demToImage->SetOutputSize(size);

  SpacingType spacing;
  spacing[0] = ::atof(argv[6]);
  spacing[1] = ::atof(argv[7]);

  demToImage->SetOutputSpacing(spacing);

  // Software Guide : BeginLatex
  //
  // As in the previous example the \doxygen{itk}{UnaryFunctorImageFilter} is
  // the filter in charge of calling the functor we specify to do the work for
  // each pixel. Here it is the \doxygen{otb}{ScalarToRainbowRGBPixelFunctor}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::Functor::ScalarToRainbowRGBPixelFunctor<PixelType> ColorMapFunctorType;
  typedef itk::UnaryFunctorImageFilter<ImageType,
    RGBImageType, ColorMapFunctorType> ColorMapFilterType;
  ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();
  colormapper->GetFunctor().SetMaximum(4000);
  colormapper->GetFunctor().SetMinimum(0);

  colormapper->SetInput(demToImage->GetOutput());
  // Software Guide : EndCodeSnippet


  writer->SetInput(colormapper->GetOutput());


  try
  {
    writer->Update();
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
  // Figure~\ref{fig:RAINBOW_FILTER} shows effect of applying the filter to
  // a gray level image.
  //
 // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{pretty_DEMToImageGenerator.eps}
  // \includegraphics[width=0.44\textwidth]{DEMToRainbowImageGenerator.eps}
  // \itkcaption[Grayscale to color]{The gray level DEM extracted from SRTM
  // data (left) and the same area in color representation.}
  // \label{fig:RAINBOW_FILTER}
  // \end{figure}
   //  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
