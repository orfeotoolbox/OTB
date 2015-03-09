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
//  OUTPUTS: {DEMToImageGenerator.tif}
//  OUTPUTS: {pretty_DEMToImageGenerator.png}
//  6.5 45.5 500 500 0.002 -0.002 ${OTB_DATA_ROOT}/Examples/DEM_srtm
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// \index{otb::DEMToImageGenerator}
// \index{otb::DEMHandler}
//
//
// The following example illustrates the use of the \doxygen{otb}{DEMToImageGenerator} class.
// The aim of this class is to generate an image from the srtm data (precising the start extraction
// latitude and longitude point). Each pixel is a geographic point and its intensity is
// the altitude of the point.
// If srtm doesn't have altitude information for a point, the altitude value is set at -32768 (value of the srtm norm).
//
// Let's look at the minimal code required to use this algorithm. First, the following header
// defining the \doxygen{otb}{DEMToImageGenerator} class must be included.
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbDEMToImageGenerator.h"
// Software Guide : EndCodeSnippet

#include "itkRescaleIntensityImageFilter.h"
#include "itkThresholdImageFilter.h"
#include "itkMacro.h"
#include "otbImageFileWriter.h"

int main(int argc, char * argv[])
{
  if (argc < 10)
    {
    std::cout << argv[0] <<
    " output filename , pretty output filename , Longitude Output Origin point , Latitude Output Origin point , X Output Size, Y Output size , X Spacing , Y Spacing, DEM folder path"
              << std::endl;
    return EXIT_FAILURE;
    }
  //  Software Guide : BeginLatex
  //
  // The image type is now defined using pixel type and
  // dimension. The output image is defined as an \doxygen{otb}{Image}.
  //
  // Software Guide : EndLatex

  char * folderPath = argv[9];
  char * outputName = argv[1];
  // Software Guide : BeginCodeSnippet
  const unsigned int Dimension = 2;
  typedef otb::Image<double, Dimension> ImageType;
  // Software Guide : EndCodeSnippet

  // The writer is defined
  typedef otb::ImageFileWriter<ImageType> WriterType;

  //  Software Guide : BeginLatex
  //
  // The DEMToImageGenerator is defined using the image pixel
  // type as a template parameter. After that, the object can be instancied.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::DEMToImageGenerator<ImageType> DEMToImageGeneratorType;

  DEMToImageGeneratorType::Pointer object = DEMToImageGeneratorType::New();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // Input parameter types are defined to set the value in the \doxygen{otb}{DEMToImageGenerator}.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef DEMToImageGeneratorType::SizeType       SizeType;
  typedef DEMToImageGeneratorType::SpacingType    SpacingType;
  typedef DEMToImageGeneratorType::PointType      PointType;
  // Software Guide : EndCodeSnippet

  // Instantiating writer
  WriterType::Pointer writer = WriterType::New();

  // Software Guide : BeginLatex
  //
  // The path to the DEM folder is given to the \doxygen{otb}{DEMHandler}.
  //
  // Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  otb::DEMHandler::Instance()->OpenDEMDirectory(folderPath);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The origin (Longitude/Latitude) of the output image in the DEM is given to the filter.
  //
  // Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  PointType origin;
  origin[0] = ::atof(argv[3]);
  origin[1] = ::atof(argv[4]);

  object->SetOutputOrigin(origin);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // The size (in Pixel) of the output image is given to the filter.
  //
  // Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  SizeType size;
  size[0] = ::atoi(argv[5]);
  size[1] = ::atoi(argv[6]);

  object->SetOutputSize(size);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The spacing (step between to consecutive pixel) is given to the filter.
  // By default, this spacing is set at 0.001.
  //
  //  Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  SpacingType spacing;
  spacing[0] = ::atof(argv[7]);
  spacing[1] = ::atof(argv[8]);

  object->SetOutputSpacing(spacing);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // The output image name is given to the writer and
  // the filter output is linked to the writer input.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  writer->SetFileName(outputName);

  writer->SetInput(object->GetOutput());
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

  catch (itk::ExceptionObject& err)
    {
    std::cout << "Exception itk::ExceptionObject thrown !" << std::endl;
    std::cout << err << std::endl;
    return EXIT_FAILURE;
    }
  // Software Guide : EndCodeSnippet
  catch (...)
    {
    std::cout << "Unknown exception thrown !" << std::endl;
    return EXIT_FAILURE;
    }

  // Pretty image creation for the printing
  typedef otb::Image<unsigned char,
      Dimension>
  OutputPrettyImageType;
  typedef otb::ImageFileWriter<OutputPrettyImageType>
  WriterPrettyType;
  typedef itk::RescaleIntensityImageFilter<ImageType,
      OutputPrettyImageType> RescalerType;
  typedef itk::ThresholdImageFilter<ImageType>
  ThresholderType;

  ThresholderType::Pointer  thresholder  = ThresholderType::New();
  RescalerType::Pointer     rescaler     = RescalerType::New();
  WriterPrettyType::Pointer prettyWriter = WriterPrettyType::New();

  thresholder->SetInput(object->GetOutput());
  thresholder->SetOutsideValue(0.0);
  thresholder->ThresholdBelow(0.0);
  thresholder->Update();

  rescaler->SetInput(thresholder->GetOutput());
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  prettyWriter->SetFileName(argv[2]);

  prettyWriter->SetInput(rescaler->GetOutput());
  try
    {
    prettyWriter->Update();
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

  return EXIT_SUCCESS;

  // Software Guide : BeginLatex
  //
  // Let's now run this example using as input the SRTM data contained in
  // \code{DEM\_srtm} folder. Figure \ref{fig:DEMToImageGenerator}
  // shows the obtained DEM. Invalid data values -- hidden areas due
  // to SAR shadowing -- are set to zero.
  //
  //
  // \begin{figure} \center
  // \includegraphics[width=0.24\textwidth]{pretty_DEMToImageGenerator.eps}
  // \itkcaption[DEM To Image generator Example]{DEMToImageGenerator image.}
  // \label{fig:DEMToImageGenerator}
  // \end{figure}
  //
  //  Software Guide : EndLatex

}
