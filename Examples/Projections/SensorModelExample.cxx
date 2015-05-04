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


#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkExtractImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkLinearInterpolateImageFunction.h"

// Software Guide : BeginLatex
//
// This example illustrates how to use the sensor model read from
// image meta-data in order to perform ortho-rectification. This is a
// very basic, step-by-step example, so you understand the different
// components involved in the process. When performing real
// ortho-rectifications, you can use the example presented in section
// \ref{sec:OrthorectificationwithOTB}.
//
// We will start by including the header file for the inverse sensor model.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbInverseSensorModel.h"
// Software Guide : EndCodeSnippet

int main(int argc, char* argv[])
{

  if (argc != 8)
    {
    std::cout << argv[0] << " <input_filename> <output_filename>"
              << " <upper_left_corner_longitude> <upper_left_corner_latitude>"
              << " <size_x> <size_y> <number_of_stream_divisions>"
              << std::endl;

    return EXIT_FAILURE;
    }

// Software Guide : BeginLatex
//
// As explained before, the first thing to do is to create the sensor
// model in order to transform the ground coordinates in sensor
// geometry coordinates. The geoetric model will automatically be
// created by the image file reader. So we begin by declaring the
// types for the input image and the image reader.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::Image<unsigned int, 2>     ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  ImageType::Pointer inputImage = reader->GetOutput();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We have just instantiated the reader and set the file name, but the
// image data and meta-data has not yet been accessed by it. Since we
// need the creation of the sensor model and all the image information
// (size, spacing, etc.), but we do not want to read the pixel data --
// it could be huge -- we just ask the reader to generate the output
// information needed.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  reader->GenerateOutputInformation();

  std::cout << "Original input imagine spacing: " <<
  reader->GetOutput()->GetSpacing() << std::endl;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We can now instantiate the sensor model -- an inverse one, since we
// want to convert ground coordinates to sensor geometry. Note that
// the choice of the specific model (SPOT5, Ikonos, etc.) is done by
// the reader and we only need to instantiate a generic model.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::InverseSensorModel<double> ModelType;
  ModelType::Pointer model = ModelType::New();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// The model is parameterized by passing to it the {\em keyword list}
// containing the needed information.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  model->SetImageGeometry(reader->GetOutput()->GetImageKeywordlist());
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Since we can not be sure that the image we read actually has sensor
// model information, we must check for the model validity.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  if (model->IsValidSensorModel() == false)
    {
    std::cerr << "Unable to create a model" << std::endl;
    return 1;
    }
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// The types for the input and output coordinate points can be now
// declared. The same is done for the index types.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ModelType::OutputPointType inputPoint;
  typedef itk::Point <double, 2> PointType;
  PointType outputPoint;

  ImageType::IndexType currentIndex;
  ImageType::IndexType currentIndexBis;
  ImageType::IndexType pixelIndexBis;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We will now create the output image over which we will iterate in
// order to transform ground coordinates to sensor coordinates and get
// the corresponding pixel values.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ImageType::Pointer outputImage = ImageType::New();

  ImageType::PixelType pixelValue;

  ImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;

  ImageType::SizeType size;
  size[0] = atoi(argv[5]);
  size[1] = atoi(argv[6]);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// The spacing in y direction is negative since origin is the upper left corner.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ImageType::SpacingType spacing;
  spacing[0] = 0.00001;
  spacing[1] = -0.00001;

  ImageType::PointType origin;
  origin[0] = strtod(argv[3], NULL);         //longitude
  origin[1] = strtod(argv[4], NULL);         //latitude

  ImageType::RegionType region;

  region.SetSize(size);
  region.SetIndex(start);

  outputImage->SetOrigin(origin);
  outputImage->SetRegions(region);
  outputImage->SetSpacing(spacing);
  outputImage->Allocate();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We will now instantiate an extractor filter in order to get input
// regions by manual tiling.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef itk::ExtractImageFilter<ImageType, ImageType> ExtractType;
  ExtractType::Pointer extract = ExtractType::New();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Since the transformed coordinates in sensor geometry may not be
// integer ones, we will need an interpolator to retrieve the pixel
// values (note that this assumes that the input image was correctly
// sampled by the acquisition system).
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef itk::LinearInterpolateImageFunction<ImageType, double>
  InterpolatorType;
  InterpolatorType::Pointer interpolator = InterpolatorType::New();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We proceed now to create the image writer. We will also use a
// writer plugged to the output of the extractor filter which will
// write the temporary extracted regions. This is just for monitoring
// the process.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
  typedef otb::Image<unsigned char, 2>        CharImageType;
  typedef otb::ImageFileWriter<CharImageType> CharWriterType;
  typedef otb::ImageFileWriter<ImageType>     WriterType;
  WriterType::Pointer     extractorWriter = WriterType::New();
  CharWriterType::Pointer writer = CharWriterType::New();
  extractorWriter->SetFileName("image_temp.jpeg");
  extractorWriter->SetInput(extract->GetOutput());
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// Since the output pixel type and the input pixel type are different,
// we will need to rescale the intensity values before writing them to
// a file.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
  typedef itk::RescaleIntensityImageFilter
  <ImageType, CharImageType> RescalerType;
  RescalerType::Pointer rescaler = RescalerType::New();
  rescaler->SetOutputMinimum(10);
  rescaler->SetOutputMaximum(255);
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// The tricky part starts here. Note that this example is only
// intended for educationnal purposes and that you do not need to proceed
// as this. See the example in section
// \ref{sec:OrthorectificationwithOTB} in order to code
// ortho-rectification chains in a very simple way.
//
//  You want to go on? OK. You have been warned.
//
//  We will start by declaring an image region iterator and some
//  convenience variables.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
  typedef itk::ImageRegionIteratorWithIndex<ImageType> IteratorType;

  unsigned int NumberOfStreamDivisions;
  if (atoi(argv[7]) == 0)
    {
    NumberOfStreamDivisions = 10;
    }
  else
    {
    NumberOfStreamDivisions = atoi(argv[7]);
    }

  unsigned int          count = 0;
  unsigned int          It, j, k;
  int                   max_x, max_y, min_x, min_y;
  ImageType::IndexType  iterationRegionStart;
  ImageType::SizeType   iteratorRegionSize;
  ImageType::RegionType iteratorRegion;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// The loop starts here.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
  for (count = 0; count < NumberOfStreamDivisions; count++)
    {
    iteratorRegionSize[0] = atoi(argv[5]);
    if (count == NumberOfStreamDivisions - 1)
      {
      iteratorRegionSize[1] = (atoi(argv[6])) - ((int) (((atoi(argv[6])) /
                                                         NumberOfStreamDivisions)
                                                        + 0.5)) * (count);
      iterationRegionStart[1] = (atoi(argv[5])) - (iteratorRegionSize[1]);
      }
    else
      {
      iteratorRegionSize[1] = (int) (((atoi(argv[6])) /
                                      NumberOfStreamDivisions) + 0.5);
      iterationRegionStart[1] = count * iteratorRegionSize[1];
      }
    iterationRegionStart[0] = 0;
    iteratorRegion.SetSize(iteratorRegionSize);
    iteratorRegion.SetIndex(iterationRegionStart);
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We create an array for storing the pixel indexes.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
    unsigned int pixelIndexArrayDimension = iteratorRegionSize[0] *
                                            iteratorRegionSize[1] * 2;
    int *pixelIndexArray = new int[pixelIndexArrayDimension];
    int *currentIndexArray = new int[pixelIndexArrayDimension];
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We create an iterator for each piece of the image, and we iterate
// over them.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
    IteratorType outputIt(outputImage, iteratorRegion);

    It = 0;
    for (outputIt.GoToBegin(); !outputIt.IsAtEnd(); ++outputIt)
      {
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We get the current index.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
      currentIndex = outputIt.GetIndex();
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We transform the index to physical coordinates.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
      outputImage->TransformIndexToPhysicalPoint(currentIndex, outputPoint);
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We use the sensor model to get the pixel coordinates in the input
// image and we transform this coodinates to an index. Then we store
// the index in the array. Note that the \code{TransformPoint()}
// method of the model has been overloaded so that it can be used with
// a 3D point when the height of the ground point is known (DEM availability).
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
      inputPoint = model->TransformPoint(outputPoint);

      pixelIndexArray[It] = static_cast<int>(inputPoint[0]);
      pixelIndexArray[It + 1] = static_cast<int>(inputPoint[1]);

      currentIndexArray[It] = static_cast<int>(currentIndex[0]);
      currentIndexArray[It + 1] = static_cast<int>(currentIndex[1]);

      It = It + 2;
      }
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// At this point, we have stored all the indexes we need for the piece
// of image we are processing. We can now compute the bounds of the
// area in the input image we need to extract.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
    max_x = pixelIndexArray[0];
    min_x = pixelIndexArray[0];
    max_y = pixelIndexArray[1];
    min_y = pixelIndexArray[1];

    for (j = 0; j < It; ++j)
      {
      if (j % 2 == 0 && pixelIndexArray[j] > max_x)
        {
        max_x = pixelIndexArray[j];
        }
      if (j % 2 == 0 && pixelIndexArray[j] < min_x)
        {
        min_x = pixelIndexArray[j];
        }
      if (j % 2 != 0 && pixelIndexArray[j] > max_y)
        {
        max_y = pixelIndexArray[j];
        }
      if (j % 2 != 0 && pixelIndexArray[j] < min_y)
        {
        min_y = pixelIndexArray[j];
        }
      }
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We can now set the parameters for the extractor using a little bit
// of margin in order to cope with irregular geometric distortions
// which could be due to topography, for instance.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
    ImageType::RegionType extractRegion;

    ImageType::IndexType extractStart;

    if (min_x < 10 && min_y < 10)
      {
      extractStart[0] = 0;
      extractStart[1] = 0;
      }
    else
      {
      extractStart[0] = min_x - 10;
      extractStart[1] = min_y - 10;
      }

    ImageType::SizeType extractSize;

    extractSize[0] = (max_x - min_x) + 20;
    extractSize[1] = (max_y - min_y) + 20;
    extractRegion.SetSize(extractSize);
    extractRegion.SetIndex(extractStart);

    extract->SetExtractionRegion(extractRegion);
    extract->SetInput(reader->GetOutput());
    extractorWriter->Update();
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We give the input image to the interpolator and we loop through the
// index array in order to get the corresponding pixel values. Note
// that for every point we check whether it is inside the extracted region.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
    interpolator->SetInputImage(extract->GetOutput());

    for (k = 0; k < It / 2; ++k)
      {
      pixelIndexBis[0] = pixelIndexArray[2 * k];
      pixelIndexBis[1] = pixelIndexArray[2 * k + 1];
      currentIndexBis[0] = currentIndexArray[2 * k];
      currentIndexBis[1] = currentIndexArray[2 * k + 1];

      if (interpolator->IsInsideBuffer(pixelIndexBis))
        {
        pixelValue = int(interpolator->EvaluateAtIndex(pixelIndexBis));
        }
      else
        {
        pixelValue = 0;
        }

      outputImage->SetPixel(currentIndexBis, pixelValue);
      }
    delete pixelIndexArray;
    delete currentIndexArray;

    }
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// So we are done. We can now write the output image to a file after
// performing the intensity rescaling.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
  writer->SetFileName(argv[2]);

  rescaler->SetInput(outputImage);

  writer->SetInput(rescaler->GetOutput());
  writer->Update();
// Software Guide : EndCodeSnippet

  return EXIT_SUCCESS;

}
