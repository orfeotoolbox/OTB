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
./MultivariateAlterationDetector Input/Spot5-Gloucester-before.tif \
                                 Input/Spot5-Gloucester-after.tif \
                                 Output/MADOutput.tif \
                                 Output/mad-input1.png \
                                 Output/mad-input2.png \
                                 Output/mad-output.png
*/

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbPrintableImageFilter.h"
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
  using InputPixelType  = unsigned short;
  using OutputPixelType = float;
  using InputImageType  = otb::VectorImage<InputPixelType, Dimension>;
  using OutputImageType = otb::VectorImage<OutputPixelType, Dimension>;

  // We can now declare the types for the reader. Since the images
  // can be vey large, we will force the pipeline to use
  // streaming. For this purpose, the file writer will be
  // streamed. This is achieved by using the
  // ImageFileWriter class.
  using ReaderType = otb::ImageFileReader<InputImageType>;
  using WriterType = otb::ImageFileWriter<OutputImageType>;

  // This is for rendering in software guide
  using InputPrintFilterType  = otb::PrintableImageFilter<InputImageType, InputImageType>;
  using OutputPrintFilterType = otb::PrintableImageFilter<OutputImageType, OutputImageType>;
  using VisuImageType         = InputPrintFilterType::OutputImageType;
  using VisuWriterType        = otb::ImageFileWriter<VisuImageType>;

  // The MultivariateAlterationDetectorImageFilter is templated over
  // the type of the input images and the type of the generated change
  // image.
  using MADFilterType = otb::MultivariateAlterationDetectorImageFilter<InputImageType, OutputImageType>;

  // The different elements of the pipeline can now be instantiated.
  ReaderType::Pointer    reader1   = ReaderType::New();
  ReaderType::Pointer    reader2   = ReaderType::New();
  WriterType::Pointer    writer    = WriterType::New();
  MADFilterType::Pointer madFilter = MADFilterType::New();

  const char* inputFilename1 = argv[1];
  const char* inputFilename2 = argv[2];
  const char* outputFilename = argv[3];
  const char* in1pretty      = argv[4];
  const char* in2pretty      = argv[5];
  const char* outpretty      = argv[6];

  // We set the parameters of the different elements of the pipeline.
  reader1->SetFileName(inputFilename1);
  reader2->SetFileName(inputFilename2);
  writer->SetFileName(outputFilename);

  // We build the pipeline by plugging all the elements together.
  madFilter->SetInput1(reader1->GetOutput());
  madFilter->SetInput2(reader2->GetOutput());
  writer->SetInput(madFilter->GetOutput());

  // And then we can trigger the pipeline update, as usual.
  writer->Update();

  // Here we generate the figures
  InputPrintFilterType::Pointer  input1PrintFilter = InputPrintFilterType::New();
  InputPrintFilterType::Pointer  input2PrintFilter = InputPrintFilterType::New();
  OutputPrintFilterType::Pointer outputPrintFilter = OutputPrintFilterType::New();
  VisuWriterType::Pointer        input1VisuWriter  = VisuWriterType::New();
  VisuWriterType::Pointer        input2VisuWriter  = VisuWriterType::New();
  VisuWriterType::Pointer        outputVisuWriter  = VisuWriterType::New();

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
}
