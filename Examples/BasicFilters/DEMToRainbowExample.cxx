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


//  Software Guide : BeginCommandLineArgs
//  OUTPUTS: {DEMToRainbowImageGenerator.png}
//  6.5 45.5 500 500 0.002 -0.002 ${OTB_DATA_ROOT}/Examples/DEM_srtm
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginCommandLineArgs
//  OUTPUTS: {DEMToHotImageGenerator.png}
//  6.5 45.5 500 500 0.002 -0.002 ${OTB_DATA_ROOT}/Examples/DEM_srtm hot
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginCommandLineArgs
//  OUTPUTS: {DEMToReliefImageGenerator.png}
//  6.5 45.5 500 500 0.002 -0.002 ${OTB_DATA_ROOT}/Examples/DEM_srtm relief
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// In some situation, it is desirable to represent a gray level image in color for easier
// interpretation. This is particularly the case if pixel values in the image are used
// to represent some data such as elevation, deformation map,
// interferogram. In this case, it is important to ensure that similar
// values will get similar colors. You can notice how this requirement
// differ from the previous case.
//
// The following example illustrates the use of the \doxygen{otb}{DEMToImageGenerator} class
// combined with the \doxygen{otb}{ScalarToRainbowRGBPixelFunctor}. You can refer to the
// source code or to section \ref{sec:ReadDEM} for the DEM conversion to image,
// we will focus on the color conversion part here.
//
// Software Guide : EndLatex

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkScalarToRGBColormapImageFilter.h"
#include "otbScalarToRainbowRGBPixelFunctor.h"
#include "otbDEMToImageGenerator.h"
#include "otbReliefColormapFunctor.h"

int main(int argc, char * argv[])
{

  if (argc < 9)
    {
    std::cout << argv[0] <<
    " <output_filename> <Longitude Output Origin point>";
    std::cout <<
    " <Latitude Output Origin point> <X Output Size> <Y Output size>";
    std::cout << " <X Spacing> <Y Spacing> <DEM folder path>"  << std::endl;
    return EXIT_FAILURE;
    }

  typedef double                                      PixelType;
  typedef unsigned char                               UCharPixelType;
  typedef itk::RGBPixel<UCharPixelType>               RGBPixelType;
  typedef otb::Image<PixelType, 2>                    ImageType;
  typedef otb::Image<RGBPixelType, 2>                 RGBImageType;
  typedef otb::ImageFileWriter<RGBImageType> WriterType;

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(argv[1]);

  typedef otb::DEMToImageGenerator<ImageType> DEMToImageGeneratorType;

  DEMToImageGeneratorType::Pointer demToImage = DEMToImageGeneratorType::New();

  typedef DEMToImageGeneratorType::SizeType       SizeType;
  typedef DEMToImageGeneratorType::SpacingType    SpacingType;
  typedef DEMToImageGeneratorType::PointType      PointType;

  otb::DEMHandler::Instance()->OpenDEMDirectory(argv[8]);

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
  // As in the previous example the \doxygen{itk}{ScalarToRGBColormapImageFilter} is
  // the filter in charge of calling the functor we specify to do the work for
  // each pixel. Here it is the \doxygen{otb}{ScalarToRainbowRGBPixelFunctor}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::ScalarToRGBColormapImageFilter<ImageType,
      RGBImageType> ColorMapFilterType;
  ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();
  colormapper->UseInputImageExtremaForScalingOff();

  if (argc == 9)
    {
    typedef otb::Functor::ScalarToRainbowRGBPixelFunctor<PixelType,
        RGBPixelType>
    ColorMapFunctorType;
    ColorMapFunctorType::Pointer colormap = ColorMapFunctorType::New();
    colormap->SetMinimumInputValue(0);
    colormap->SetMaximumInputValue(4000);
    colormapper->SetColormap(colormap);
    }
  // Software Guide : EndCodeSnippet

  else
    {
    if (strcmp(argv[9], "hot") == 0)
      {
      typedef itk::Function::HotColormapFunction<PixelType,
          RGBPixelType>
      ColorMapFunctorType;
      ColorMapFunctorType::Pointer colormap = ColorMapFunctorType::New();
      colormap->SetMinimumInputValue(0);
      colormap->SetMaximumInputValue(4000);
      colormapper->SetColormap(colormap);
      }
    else
      {
      typedef otb::Functor::ReliefColormapFunctor<PixelType,
          RGBPixelType>
      ColorMapFunctorType;
      ColorMapFunctorType::Pointer colormap = ColorMapFunctorType::New();
      colormap->SetMinimumInputValue(0);
      colormap->SetMaximumInputValue(4000);
      colormapper->SetColormap(colormap);
      }
    }
  // Software Guide : BeginLatex
  //
  // And we connect the color mapper filter with the filter producing
  // the image of the DEM:
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  colormapper->SetInput(demToImage->GetOutput());
  // Software Guide : EndCodeSnippet

  writer->SetInput(colormapper->GetOutput());

  try
    {
    writer->Update();
    }
  catch (itk::ExceptionObject& excep)
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    }
  catch (...)
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
// \includegraphics[width=0.44\textwidth]{DEMToHotImageGenerator.eps}
// \includegraphics[width=0.44\textwidth]{DEMToReliefImageGenerator.eps}
// \itkcaption[Grayscale to color]{The gray level DEM extracted from SRTM
// data (top-left) and the same area in color representation.}
// \label{fig:RAINBOW_FILTER}
// \end{figure}
//  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
