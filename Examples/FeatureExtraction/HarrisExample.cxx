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


#include "itkMacro.h"
#include "otbImage.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {ROISpot5.png}
//    OUTPUTS: {ROISpot5Harris.png}
//    1.5 2 0.1
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the \doxygen{otb}{HarrisImageFilter}.
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbHarrisImageFilter.h"
// Software Guide : EndCodeSnippet
#include "otbHarrisImageToPointSetFilter.h"
#include "itkRescaleIntensityImageFilter.h"

int main(int argc, char *argv[])
{
  if (argc != 6)
    {
    std::cerr << "Usage: " << argv[0] << " inputImageFile ";
    std::cerr << " outputHarrisImageFile sigmaD sigmaI alpha" << std::endl;
    return EXIT_FAILURE;
    }

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  double SigmaD((double) ::atof(argv[3]));
  double SigmaI((double) ::atof(argv[4]));
  double Alpha((double) ::atof(argv[5]));

  typedef float InputPixelType;
  const unsigned int Dimension = 2;
  typedef unsigned char OutputPixelType;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType> ReaderType;

  //  Software Guide : BeginLatex
  //
  //  The \doxygen{otb}{HarrisImageFilter} is templated over the
  //  input and output image types, so we start by
  //  defining:
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::HarrisImageFilter<InputImageType,
      InputImageType>   HarrisFilterType;
  // Software Guide : EndCodeSnippet
  typedef itk::RescaleIntensityImageFilter
  <InputImageType, OutputImageType> RescalerType;

  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  ReaderType::Pointer       reader   = ReaderType::New();
  WriterType::Pointer       writer   = WriterType::New();
  HarrisFilterType::Pointer harris = HarrisFilterType::New();
  RescalerType::Pointer     rescaler = RescalerType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  harris->SetInput(reader->GetOutput());

  //  Software Guide : BeginLatex
  //
  // The \doxygen{otb}{HarrisImageFilter} needs some parameters to
  // operate. The derivative computation is performed by a
  // convolution with the derivative of a Gaussian kernel of
  // variance $\sigma_D$ (derivation scale) and
  // the smoothing of the image is performed by convolving with a
  // Gaussian kernel of variance $\sigma_I$ (integration
  // scale). This allows the computation of the following matrix:
  // \begin{equation}
  // \mu(\mathbf{x},\sigma_I,\sigma_D) = \sigma_D^2 g(\sigma_I)\star
  // \left[\begin{array}{cc} L_x^2(\mathbf{x},\sigma_D) &
  // L_xL_y(\mathbf{x},\sigma_D)\\ L_xL_y(\mathbf{x},\sigma_D)&
  // L_y^2(\mathbf{x},\sigma_D) \end{array}\right]
  // \end{equation}
  // The output of the detector is $$det(\mu) - \alpha trace^2(\mu).$$
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  harris->SetSigmaD(SigmaD);
  harris->SetSigmaI(SigmaI);
  harris->SetAlpha(Alpha);
  // Software Guide : EndCodeSnippet

  rescaler->SetOutputMinimum(itk::NumericTraits<OutputPixelType>::min());
  rescaler->SetOutputMaximum(itk::NumericTraits<OutputPixelType>::max());

  rescaler->SetInput(harris->GetOutput());
  writer->SetInput(rescaler->GetOutput());
  writer->Update();

  //  Software Guide : BeginLatex
  // Figure~\ref{fig:Harris} shows the result of applying the interest
  // point detector to a small patch extracted from a Spot 5 image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.25\textwidth]{ROISpot5.eps}
  // \includegraphics[width=0.25\textwidth]{ROISpot5Harris.eps}
  // \itkcaption[Harris Filter Application]{Result of applying the
  // \doxygen{otb}{HarrisImageFilter} to a Spot 5 image.}
  // \label{fig:Harris}
  // \end{figure}
  //
  // The output of the \doxygen{otb}{HarrisImageFilter} is an image
  // where, for each pixel, we obtain the intensity of the
  // detection. Often, the user may want to get access to the set of
  // points for which the output of the detector is higher than a
  // given threshold. This can be obtained by using the
  // \doxygen{otb}{HarrisImageToPointSetFilter}. This filter is only
  // templated over the input image type, the output being a
  // \doxygen{itk}{PointSet} with pixel type equal to the image pixel type.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::HarrisImageToPointSetFilter<InputImageType> FunctionType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  We declare now the filter and a pointer to the output point set.
  //  Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  typedef FunctionType::OutputPointSetType OutputPointSetType;

  FunctionType::Pointer       harrisPoints    = FunctionType::New();
  OutputPointSetType::Pointer pointSet = OutputPointSetType::New();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The \doxygen{otb}{HarrisImageToPointSetFilter} takes the same
  // parameters as the \doxygen{otb}{HarrisImageFilter} and an
  // additional parameter : the threshold for the point selection.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  harrisPoints->SetInput(0, reader->GetOutput());
  harrisPoints->SetSigmaD(SigmaD);
  harrisPoints->SetSigmaI(SigmaI);
  harrisPoints->SetAlpha(Alpha);
  harrisPoints->SetLowerThreshold(10);
  pointSet = harrisPoints->GetOutput();
  // Software Guide : EndCodeSnippet

  harrisPoints->Update();

  //  Software Guide : BeginLatex
  //
  //  We can now iterate through the obtained pointset and access
  //  the coordinates of the points. We start by accessing the
  //  container of the points which is encapsulated into the point
  //  set (see section \ref{sec:PointSetSection} for more
  //  information on using \doxygen{itk}{PointSet}s) and declaring
  //  an iterator to it.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef OutputPointSetType::PointsContainer ContainerType;
  ContainerType* pointsContainer = pointSet->GetPoints();
  typedef ContainerType::Iterator IteratorType;
  IteratorType itList = pointsContainer->Begin();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  And we get the points coordinates
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  while (itList != pointsContainer->End())
    {
    typedef OutputPointSetType::PointType OutputPointType;
    OutputPointType pCoordinate = (itList.Value());
    std::cout << pCoordinate << std::endl;
    ++itList;
    }
  // Software Guide : EndCodeSnippet

  return EXIT_SUCCESS;
}
