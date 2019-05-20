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


/* Example usage:
./KullbackLeiblerProfileChDet Input/GomaAvant.png Input/GomaApres.png Output/KLProfileChDet.png 5 51 1 12 24
*/


// This example illustrates the class
// \doxygen{otb}{KullbackLeiblerProfileImageFilter} for detecting changes
// between pairs of images, according to a range of window size.
// This example is very similar, in its principle, to all of the change
// detection examples, especially the distance between distributions one
// (section~\ref{sec:KullbackLeiblerDistance}) which uses a fixed window size.
//
// The main differences are:
// \begin{enumerate}
// \item a set of window range instead of a fixed size of window;
// \item an output of type \doxygen{otb}{VectorImage}.
// \end{enumerate}
// Then, the program begins with the \doxygen{otb}{VectorImage} and the
// \doxygen{otb}{KullbackLeiblerProfileImageFilter} header files in addition
// to those already details in the \doxygen{otb}{MeanRatioImageFilter} example.

#include "otbImage.h"
#include "otbMultiChannelExtractROI.h"
#include "otbVectorRescaleIntensityImageFilter.h"

#include "otbKullbackLeiblerProfileImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 9)
    {
      std::cerr << "Change detection based on Kullback-Leibler distance betweenlocal pdf through an Edgeworth approximation\n";
      std::cerr << argv[0] << " imgAv imgAp imgResu winSizeMin winSizeMax outRedIndex outGreenIndex outBlueIndex\n";
      return 1;
    }

    char*        fileName1   = argv[1];
    char*        fileName2   = argv[2];
    char*        fileNameOut = argv[3];
    int          winSizeMin  = atoi(argv[4]);
    int          winSizeMax  = atoi(argv[5]);
    unsigned int ri          = atoi(argv[6]);
    unsigned int gi          = atoi(argv[7]);
    unsigned int bi          = atoi(argv[8]);

    const unsigned int Dimension = 2;
    using PixelType              = double;
    using OutPixelType           = unsigned char;

    //  The \doxygen{otb}{KullbackLeiblerProfileImageFilter} is templated over
    //  the types of the two input images and the type of the generated change
    //  image (which is now of multi-components), in a similar way as the
    //  \doxygen{otb}{KullbackLeiblerDistanceImageFilter}.

    using ImageType       = otb::Image<PixelType, Dimension>;
    using VectorImageType = otb::VectorImage<PixelType, Dimension>;
    using FilterType      = otb::KullbackLeiblerProfileImageFilter<ImageType, ImageType, VectorImageType>;

    using OutVectorImageType  = otb::VectorImage<OutPixelType, Dimension>;
    using ReaderType          = otb::ImageFileReader<ImageType>;
    using WriterType          = otb::ImageFileWriter<OutVectorImageType>;
    using ChannelSelecterType = otb::MultiChannelExtractROI<PixelType, PixelType>;
    using RescalerType        = otb::VectorRescaleIntensityImageFilter<VectorImageType, OutVectorImageType>;

    ReaderType::Pointer reader1 = ReaderType::New();
    reader1->SetFileName(fileName1);

    ReaderType::Pointer reader2 = ReaderType::New();
    reader2->SetFileName(fileName2);

    //  The different elements of the pipeline can now be instantiated in the
    //  same way as the ratio of means change detector example.
    //
    //  Two parameters are now required to give the minimum and the maximum size
    //  of the analysis window. The program will begin by performing change
    //  detection through the smaller window size and then applying moments update
    //  of eq.~\eqref{eqMomentN} by incrementing the radius of the analysis window
    //  (i.e. add a ring of width 1 pixel around the current neightborhood shape).
    //  The process is applied until the larger window size is reached.
    //
    FilterType::Pointer filter = FilterType::New();
    filter->SetRadius((winSizeMin - 1) / 2, (winSizeMax - 1) / 2);
    filter->SetInput1(reader1->GetOutput());
    filter->SetInput2(reader2->GetOutput());

    ChannelSelecterType::Pointer channelSelecter = ChannelSelecterType::New();
    channelSelecter->SetInput(filter->GetOutput());
    channelSelecter->SetChannel(ri);
    channelSelecter->SetChannel(gi);
    channelSelecter->SetChannel(bi);

    RescalerType::Pointer rescaler = RescalerType::New();
    rescaler->SetInput(channelSelecter->GetOutput());
    OutVectorImageType::PixelType min, max;
    min.SetSize(3);
    max.SetSize(3);
    min.Fill(0);
    max.Fill(255);
    rescaler->SetOutputMinimum(min);
    rescaler->SetOutputMaximum(max);

    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(fileNameOut);
    writer->SetInput(rescaler->GetOutput());
    writer->Update();

    // Figure \ref{fig:RESKLPCHDET} shows the result of the change
    // detection by computing the Kullback-Leibler distance between
    // local pdf through an Edgeworth approximation.
    // \begin{figure}
    // \center
    // \includegraphics[width=0.35\textwidth]{KLProfileChDet.eps}
    // \itkcaption[Kullback-Leibler profile Change Detection Results]{Result of the
    // Kullback-Leibler profile change detector, colored composition including the first, 12th and 24th
    // channel of the generated output.}
    // \label{fig:RESKLPCHDET}
    // \end{figure}
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
  return EXIT_SUCCESS;
}
