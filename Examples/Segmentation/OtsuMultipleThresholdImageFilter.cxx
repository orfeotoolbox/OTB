/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


//  Software Guide : BeginCommandLineArgs
//    INPUTS: {QB_Suburb.png}
//    OUTPUTS: {OtsuMultipleThresholdsOutput000.png}, {OtsuMultipleThresholdsOutput001.png},  {OtsuMultipleThresholdsOutput002.png}
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
// This example illustrates how to use the \doxygen{itk}{OtsuMultipleThresholdsCalculator}.
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "itkOtsuMultipleThresholdsCalculator.h"
// Software Guide : EndCodeSnippet

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkScalarImageToHistogramGenerator.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkNumericTraits.h"

#include <stdio.h>
int main(int argc, char * argv[])
{
  if (argc < 2)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr <<
    " inputImageFile outputImageFileName1 [OutputImageFilename2 ...] " << std::endl;
    return EXIT_FAILURE;
    }

  //Convenience typedefs
  typedef  unsigned char InputPixelType;
  typedef  unsigned char OutputPixelType;

  typedef otb::Image<InputPixelType,  2> InputImageType;
  typedef otb::Image<OutputPixelType, 2> OutputImageType;

  // Software Guide : BeginLatex
  // OtsuMultipleThresholdsCalculator calculates thresholds for a give histogram
  // so as to maximize the between-class variance. We use
  // ScalarImageToHistogramGenerator to generate histograms
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::Statistics::ScalarImageToHistogramGenerator<InputImageType>
  ScalarImageToHistogramGeneratorType;
  typedef itk::OtsuMultipleThresholdsCalculator<
      ScalarImageToHistogramGeneratorType::HistogramType>   CalculatorType;
  // Software Guide : EndCodeSnippet

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  // Software Guide : BeginLatex
  // Once thresholds are computed we will use BinaryThresholdImageFilter to
  // segment the input image into segments.
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::BinaryThresholdImageFilter<InputImageType, OutputImageType>
  FilterType;
  // Software Guide : EndCodeSnippet

  //Create using static New() method

  // Software Guide : BeginCodeSnippet
  ScalarImageToHistogramGeneratorType::Pointer scalarImageToHistogramGenerator
    =
      ScalarImageToHistogramGeneratorType::New();
  CalculatorType::Pointer calculator = CalculatorType::New();
  FilterType::Pointer     filter = FilterType::New();
  // Software Guide : EndCodeSnippet

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  //Set Properties

  // Software Guide : BeginCodeSnippet
  scalarImageToHistogramGenerator->SetNumberOfBins(128);
  int nbThresholds = argc - 2;
  calculator->SetNumberOfThresholds(nbThresholds);
  // Software Guide : EndCodeSnippet

  const OutputPixelType outsideValue = 0;
  const OutputPixelType insideValue = 255;
  filter->SetOutsideValue(outsideValue);
  filter->SetInsideValue(insideValue);

  //Connect Pipeline
  reader->SetFileName(argv[1]);

  // Software Guide : BeginLatex
  // The pipeline will look as follows:
  // Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  scalarImageToHistogramGenerator->SetInput(reader->GetOutput());
  calculator->SetInputHistogram(scalarImageToHistogramGenerator->GetOutput());
  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  // Software Guide : EndCodeSnippet

  //Invoke pipeline
  try
    {
    reader->Update();
    }
  catch (itk::ExceptionObject& excp)
    {
    std::cerr << "Exception thrown while reading image" << excp << std::endl;
    }
  scalarImageToHistogramGenerator->Compute();

  try
    {
    calculator->Update();
    }
  catch (itk::ExceptionObject& excp)
    {
    std::cerr << "Exception thrown " << excp << std::endl;
    }

  // Software Guide : BeginLatex
  // Thresholds are obtained using the \code{GetOutput} method
  // \index{itk::OtsuMultipleThresholdsCalculator!GetOutput()}
  // Software Guide : EndLatex
  //Get Thresholds
  // Software Guide : BeginCodeSnippet
  const CalculatorType::OutputType& thresholdVector =
    calculator->GetOutput();
  CalculatorType::OutputType::const_iterator itNum = thresholdVector.begin();
  // Software Guide : EndCodeSnippet

  //Threshold into seperate segments and write out as binary images
  double lowerThreshold = 0;
  double upperThreshold;

  unsigned int counter = 0;
  // Software Guide : BeginCodeSnippet
  for (; itNum < thresholdVector.end(); itNum++)
    {
    std::cout << "OtsuThreshold["
              << (int) (itNum - thresholdVector.begin())
              << "] = " <<
    static_cast<itk::NumericTraits<CalculatorType::MeasurementType>::PrintType>
    (*itNum) << std::endl;

    upperThreshold = (*itNum);
    filter->SetLowerThreshold(static_cast<OutputPixelType> (lowerThreshold));
    filter->SetUpperThreshold(static_cast<OutputPixelType> (upperThreshold));
    lowerThreshold = upperThreshold;

    writer->SetFileName(argv[2 + counter]);
    ++counter;
    // Software Guide : EndCodeSnippet
    try
      {
      writer->Update();
      }
    catch (itk::ExceptionObject& excp)
      {
      std::cerr << "Exception thrown " << excp << std::endl;
      }
    // Software Guide : BeginCodeSnippet
    }
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{QB_Suburb.eps}
  // \includegraphics[width=0.44\textwidth]{OtsuMultipleThresholdsOutput000.eps}
  // \includegraphics[width=0.44\textwidth]{OtsuMultipleThresholdsOutput001.eps}
  // \includegraphics[width=0.44\textwidth]{OtsuMultipleThresholdsOutput002.eps}
  // \itkcaption[OtsuThresholdImageFilter output]{Effect of the OtsuMultipleThresholdImageFilter.}
  // \label{fig:OtsuMultipleThresholdImageFilterInputOutput}
  // \end{figure}
  //
  //  Figure \ref{fig:OtsuMultipleThresholdImageFilterInputOutput} illustrates the
  //  effect of this filter.
  //
  //  \relatedClasses
  //  \begin{itemize}
  //  \item \doxygen{itk}{ThresholdImageFilter}
  //  \end{itemize}
  //
  //  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
