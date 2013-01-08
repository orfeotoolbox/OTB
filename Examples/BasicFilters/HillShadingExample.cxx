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
//  OUTPUTS: {HillShadingExample.png}, {HillShadingColorExample.png}
//  6.5 45.5 500 500 0.002 -0.002 ${OTB_DATA_ROOT}/Examples/DEM_srtm
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// Visualization of digital elevation models (DEM) is often more intuitive by simulating a
// lighting source and generating the corresponding shadows. This principle is called
// hill shading.
//
// Using a simple functor \doxygen{otb}{HillShadingFunctor} and the dem image generated
// using the \doxygen{otb}{DEMToImageGenerator} (refer to \ref{sec:ReadDEM}) you can easily
// obtain a representation of the DEM. Better yet, using the
// \doxygen{otb}{ScalarToRainbowRGBPixelFunctor}, combined with the
// \doxygen{otb}{ReliefColormapFunctor} you can easily generate the classic elevation maps.
//
// This example will focus on the shading itself.
//
// Software Guide : EndLatex

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbDEMToImageGenerator.h"
#include "otbHillShadingFilter.h"

#include "itkScalarToRGBColormapImageFilter.h"
#include "otbReliefColormapFunctor.h"
#include "itkMultiplyImageFilter.h"
#include "itkShiftScaleImageFilter.h"
#include "itkBinaryFunctorImageFilter.h"
#include "otbWorldFile.h"

int main(int argc, char * argv[])
{

  if (argc < 10)
    {
    std::cout << argv[0] << " <output_filename> <output_color_filename> "
              << " <Longitude Output Origin point> <Latitude Output Origin point>"
              << " <X Output Size> <Y Output size>"
              << " <X Spacing> <Y Spacing> <DEM folder path>"
              << " [Projection Ref]"
              << std::endl;
    return EXIT_FAILURE;
    }

  typedef double                                         PixelType;
  typedef unsigned char                                  UCharPixelType;
  typedef itk::RGBPixel<UCharPixelType>                  RGBPixelType;
  typedef otb::Image<PixelType, 2>                       ImageType;
  typedef otb::Image<RGBPixelType, 2>                    RGBImageType;
  typedef otb::Image<UCharPixelType, 2>                  ScalarImageType;
  typedef otb::ImageFileWriter<RGBImageType>    WriterType;
  typedef otb::ImageFileWriter<ScalarImageType> ScalarWriterType;

  ScalarWriterType::Pointer writer = ScalarWriterType::New();
  writer->SetFileName(argv[1]);

  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetFileName(argv[2]);

  typedef otb::DEMToImageGenerator<ImageType> DEMToImageGeneratorType;

  DEMToImageGeneratorType::Pointer demToImage = DEMToImageGeneratorType::New();

  typedef DEMToImageGeneratorType::SizeType       SizeType;
  typedef DEMToImageGeneratorType::SpacingType    SpacingType;
  typedef DEMToImageGeneratorType::PointType      PointType;

  demToImage->SetDEMDirectoryPath(argv[9]);

  PointType origin;
  origin[0] = ::atof(argv[3]);
  origin[1] = ::atof(argv[4]);

  demToImage->SetOutputOrigin(origin);

  SizeType size;
  size[0] = ::atoi(argv[5]);
  size[1] = ::atoi(argv[6]);

  demToImage->SetOutputSize(size);

  SpacingType spacing;
  spacing[0] = ::atof(argv[7]);
  spacing[1] = ::atof(argv[8]);

  demToImage->SetOutputSpacing(spacing);
  double res = 0;

  if ( argc > 10)
    {
    demToImage->SetOutputProjectionRef(argv[10]);
    res = spacing[0];
    }
  else
    {
    //Compute the resolution (Vincenty formula)
    double lon1 = origin[0];
    double lon2 = origin[0] + size[0] * spacing[0];
    double lat1 = origin[1];
    double lat2 = origin[1] + size[1] * spacing[1];
    double R = 6371; // km
    double d = vcl_acos(vcl_sin(lat1) * vcl_sin(lat2) +
                        vcl_cos(lat1) * vcl_cos(lat2) * vcl_cos(lon2 - lon1)) * R;
    res = d / vcl_sqrt(2.0);
    }
  // Software Guide : BeginLatex
  //
  // After generating the dem image as in the DEMToImageGenerator example, you can declare
  // the hill shading mechanism. The hill shading is implemented as a functor doing some
  // operations in its neighborhood. A convenient filter called \doxygen{otb}{HillShadingFilter}
  // is defined around this mechanism.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::HillShadingFilter<ImageType, ImageType> HillShadingFilterType;
  HillShadingFilterType::Pointer hillShading = HillShadingFilterType::New();
  hillShading->SetRadius(1);
  hillShading->SetInput(demToImage->GetOutput());
  // Software Guide : EndCodeSnippet

  hillShading->GetFunctor().SetXRes(res);
  hillShading->GetFunctor().SetYRes(res);

  typedef itk::ShiftScaleImageFilter<ImageType, ScalarImageType> RescalerType;
  RescalerType::Pointer rescaler = RescalerType::New();
  rescaler->SetScale(255.0);
  rescaler->SetInput(hillShading->GetOutput());

  writer->SetInput(rescaler->GetOutput());

  typedef itk::ScalarToRGBColormapImageFilter<ImageType,
      RGBImageType> ColorMapFilterType;
  ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();
  colormapper->UseInputImageExtremaForScalingOff();

  typedef otb::Functor::ReliefColormapFunctor<PixelType,
      RGBPixelType> ColorMapFunctorType;
  ColorMapFunctorType::Pointer colormap = ColorMapFunctorType::New();
  colormap->SetMinimumInputValue(0);
  colormap->SetMaximumInputValue(4000);
  colormapper->SetColormap(colormap);

  colormapper->SetInput(demToImage->GetOutput());

  typedef itk::BinaryFunctorImageFilter<RGBImageType, ImageType, RGBImageType,
      otb::Functor::
      HillShadeModulationFunctor<RGBPixelType,
          PixelType,
          RGBPixelType> >
  MultiplyFilterType;

  MultiplyFilterType::Pointer multiply = MultiplyFilterType::New();
  multiply->SetInput1(colormapper->GetOutput());
  multiply->SetInput2(hillShading->GetOutput());

  writer2->SetInput(multiply->GetOutput());

  try
    {
    writer->Update();
    writer2->Update();
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

  otb::WorldFile::Pointer worldFile = otb::WorldFile::New();
  worldFile->SetLonOrigin(origin[0]);
  worldFile->SetLatOrigin(origin[1]);
  worldFile->SetLonSpacing(spacing[0]);
  worldFile->SetLatSpacing(spacing[1]);

  worldFile->SetImageFilename(argv[1]);
  worldFile->Update();
  worldFile->SetImageFilename(argv[2]);
  worldFile->Update();

  // Software Guide : BeginLatex
  //
  // Figure~\ref{fig:HILL_SHADING} shows the hill shading result from SRTM data.
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{HillShadingExample.eps}
  // \includegraphics[width=0.44\textwidth]{HillShadingColorExample.eps}
  // \itkcaption[Hill shading]{Hill shading obtained from SRTM data (left) and combined with
  // the color representation (right)}
  // \label{fig:HILL_SHADING}
  // \end{figure}
  //  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
