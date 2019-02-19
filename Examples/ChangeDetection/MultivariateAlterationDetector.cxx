/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbPrintableImageFilter.h"


/* Example usage:
./MultivariateAlterationDetector Input/Spot5-Gloucester-before.tif \
                                 Input/Spot5-Gloucester-after.tif \
                                 Output/MADOutput.tif \
                                 Output/mad-input1.png \
                                 Output/mad-input2.png \
                                 Output/mad-output.png
*/


// This example illustrates the class
// \doxygen{otb}{MultivariateAlterationChangeDetectorImageFilter},
// which implements the Multivariate Alteration Change Detector
// algorithm \cite{nielsen2007regularized}. This algorihtm allows
// performing change detection from a pair multi-band images, including
// images with different number of bands or modalities. Its output is
// a a multi-band image of change maps, each one being unccorrelated
// with the remaining. The number of bands of the output image is the
// minimum number of bands between the two input images.
//
// The algorithm works as follows. It tries to find two linear
// combinations of bands (one for each input images) which maximize
// correlation, and subtract these two linear combinitation, leading
// to the first change map. Then, it looks for a second set of linear
// combinations which are orthogonal to the first ones, a which
// maximize correlation, and use it as the second change map. This
// process is iterated until no more orthogonal linear combinations
// can be found.
//
// This algorithms has numerous advantages, such as radiometry scaling
// and shifting invariance and absence of parameters, but it can not
// be used on a pair of single band images (in this case the output is
// simply the difference between the two images).
//
// We start by including the corresponding header file.

#include "otbMultivariateAlterationDetectorImageFilter.h"

int main(int argc, char* argv[])
{

  if (argc < 6)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile1 inputImageFile2 outIn1Pretty outIn2Pretty outPretty"
              << "outputImageFile" << std::endl;
    return -1;
    }

  // Define the dimension of the images
  const unsigned int Dimension = 2;

  // We then define the types for the input images and for the
  // change image.

  typedef unsigned short                               InputPixelType;
  typedef float                                        OutputPixelType;
  typedef otb::VectorImage<InputPixelType, Dimension>  InputImageType;
  typedef otb::VectorImage<OutputPixelType, Dimension> OutputImageType;

  //  We can now declare the types for the reader. Since the images
  //  can be vey large, we will force the pipeline to use
  //  streaming. For this purpose, the file writer will be
  //  streamed. This is achieved by using the
  //  \doxygen{otb}{ImageFileWriter} class.

  typedef otb::ImageFileReader<InputImageType>           ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;


  // This is for rendering in software guide
  typedef otb::PrintableImageFilter<InputImageType,InputImageType>   InputPrintFilterType;
  typedef otb::PrintableImageFilter<OutputImageType,OutputImageType> OutputPrintFilterType;
  typedef InputPrintFilterType::OutputImageType                      VisuImageType;
  typedef otb::ImageFileWriter<VisuImageType>               VisuWriterType;

  //  The \doxygen{otb}{MultivariateAlterationDetectorImageFilter} is templated over
  //  the type of the input images and the type of the generated change
  //  image.

  typedef otb::MultivariateAlterationDetectorImageFilter<
      InputImageType,OutputImageType>                   MADFilterType;

  //  The different elements of the pipeline can now be instantiated.

  ReaderType::Pointer    reader1 = ReaderType::New();
  ReaderType::Pointer    reader2 = ReaderType::New();
  WriterType::Pointer    writer = WriterType::New();
  MADFilterType::Pointer madFilter = MADFilterType::New();

  const char * inputFilename1  = argv[1];
  const char * inputFilename2  = argv[2];
  const char * outputFilename  = argv[3];
  const char * in1pretty       = argv[4];
  const char * in2pretty       = argv[5];
  const char * outpretty       = argv[6];
  //  We set the parameters of the different elements of the pipeline.

  reader1->SetFileName(inputFilename1);
  reader2->SetFileName(inputFilename2);
  writer->SetFileName(outputFilename);

  //  We build the pipeline by plugging all the elements together.

  madFilter->SetInput1(reader1->GetOutput());
  madFilter->SetInput2(reader2->GetOutput());
  writer->SetInput(madFilter->GetOutput());

  try
    {
    //  And then we can trigger the pipeline update, as usual.

    writer->Update();

    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught !" << std::endl;
    std::cout << err << std::endl;
    return -1;
    }

  // Here we generate the figures
  InputPrintFilterType::Pointer input1PrintFilter = InputPrintFilterType::New();
  InputPrintFilterType::Pointer input2PrintFilter = InputPrintFilterType::New();
  OutputPrintFilterType::Pointer outputPrintFilter = OutputPrintFilterType::New();
  VisuWriterType::Pointer input1VisuWriter = VisuWriterType::New();
  VisuWriterType::Pointer input2VisuWriter = VisuWriterType::New();
  VisuWriterType::Pointer outputVisuWriter = VisuWriterType::New();

  input1PrintFilter->SetInput(reader1->GetOutput());
  input1PrintFilter->SetChannel(3);
  input1PrintFilter->SetChannel(2);
  input1PrintFilter->SetChannel(1);
  input2PrintFilter->SetInput(reader2->GetOutput());
  input2PrintFilter->SetChannel(3);
  input2PrintFilter->SetChannel(2);
  input2PrintFilter->SetChannel(1);
  outputPrintFilter->SetInput(madFilter->GetOutput());
  outputPrintFilter->SetChannel(3);
  outputPrintFilter->SetChannel(2);
  outputPrintFilter->SetChannel(1);

  input1VisuWriter->SetInput(input1PrintFilter->GetOutput());
  input2VisuWriter->SetInput(input2PrintFilter->GetOutput());
  outputVisuWriter->SetInput(outputPrintFilter->GetOutput());

  input1VisuWriter->SetFileName(in1pretty);
  input2VisuWriter->SetFileName(in2pretty);
  outputVisuWriter->SetFileName(outpretty);

  input1VisuWriter->Update();
  input2VisuWriter->Update();
  outputVisuWriter->Update();

// Figure \ref{fig:MADCHDET} shows the
// results of Multivariate Alteration Detector applied to a pair of
// SPOT5 images before and after a flooding event.
// \begin{figure}
// \center \includegraphics[width=0.32\textwidth]{mad-input1.eps}
// \includegraphics[width=0.32\textwidth]{mad-input2.eps}
// \includegraphics[width=0.32\textwidth]{mad-output.eps}
// \itkcaption[Multivariate Alteration Detection
// Results]{Result of the Multivariate Alteration Detector results on
// SPOT5 data before and after flooding.}  \label{fig:MADCHDET}
// \end{figure}

  return EXIT_SUCCESS;

}
