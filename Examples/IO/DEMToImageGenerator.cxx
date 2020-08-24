/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


/* Example usage:
./DEMToImageGenerator Output/DEMToImageGenerator.tif Output/pretty_DEMToImageGenerator.png 6.5 45.5 500 500 0.002 -0.002 Input/DEM_srtm
*/


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

#include "otbDEMToImageGenerator.h"

#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkThresholdImageFilter.h"
#include "itkMacro.h"
#include "otbImageFileWriter.h"

int main(int argc, char* argv[])
{
  if (argc < 10)
  {
    std::cout << argv[0]
              << " output filename , pretty output filename , Longitude Output Origin point , Latitude Output Origin point , X Output Size, Y Output size , X "
                 "Spacing , Y Spacing, DEM folder path"
              << std::endl;
    return EXIT_FAILURE;
  }
  // The image type is now defined using pixel type and
  // dimension. The output image is defined as an \doxygen{otb}{Image}.

  char*              folderPath = argv[9];
  char*              outputName = argv[1];
  const unsigned int Dimension  = 2;
  using ImageType               = otb::Image<double, Dimension>;

  // The writer is defined
  using WriterType = otb::ImageFileWriter<ImageType>;

  // The DEMToImageGenerator is defined using the image pixel
  // type as a template parameter. After that, the object can be instancied.

  using DEMToImageGeneratorType = otb::DEMToImageGenerator<ImageType>;

  DEMToImageGeneratorType::Pointer object = DEMToImageGeneratorType::New();

  // Input parameter types are defined to set the value in the \doxygen{otb}{DEMToImageGenerator}.

  using SizeType    = DEMToImageGeneratorType::SizeType;
  using SpacingType = DEMToImageGeneratorType::SpacingType;
  using PointType   = DEMToImageGeneratorType::PointType;

  // Instantiating writer
  WriterType::Pointer writer = WriterType::New();

  // The path to the DEM folder is given to the \doxygen{otb}{DEMHandler}.
  otb::DEMHandler::GetInstance().OpenDEMDirectory(folderPath);

  // The origin (Longitude/Latitude) of the output image in the DEM is given to the filter.
  PointType origin;
  origin[0] = ::atof(argv[3]);
  origin[1] = ::atof(argv[4]);

  object->SetOutputOrigin(origin);

  // The size (in Pixel) of the output image is given to the filter.
  SizeType size;
  size[0] = ::atoi(argv[5]);
  size[1] = ::atoi(argv[6]);

  object->SetOutputSize(size);

  // The spacing (step between to consecutive pixel) is given to the filter.
  // By default, this spacing is set at 0.001.
  SpacingType spacing;
  spacing[0] = ::atof(argv[7]);
  spacing[1] = ::atof(argv[8]);

  object->SetOutputSpacing(spacing);

  // The output image name is given to the writer and
  // the filter output is linked to the writer input.

  writer->SetFileName(outputName);

  writer->SetInput(object->GetOutput());

  //  The invocation of the \code{Update()} method on the writer triggers the
  //  execution of the pipeline.  It is recommended to place update calls in a
  //  \code{try/catch} block in case errors occur and exceptions are thrown.

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
  catch (...)
  {
    std::cout << "Unknown exception thrown !" << std::endl;
    return EXIT_FAILURE;
  }

  // Pretty image creation for the printing
  using OutputPrettyImageType = otb::Image<unsigned char, Dimension>;
  using WriterPrettyType      = otb::ImageFileWriter<OutputPrettyImageType>;
  using RescalerType          = itk::RescaleIntensityImageFilter<ImageType, OutputPrettyImageType>;
  using ThresholderType       = itk::ThresholdImageFilter<ImageType>;

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
}
