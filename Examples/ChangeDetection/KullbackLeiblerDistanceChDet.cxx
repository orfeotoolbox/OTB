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
//    INPUTS: {GomaAvant.png}, {GomaApres.png}
//    OUTPUTS: {KLdistanceChDet.png}
//    35
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
// This example illustrates the class
// \doxygen{otb}{KullbackLeiblerDistanceImageFilter} for detecting changes
// between pairs of images. This filter computes the Kullback-Leibler
// distance between probability density functions (pdfs).
// In fact, the Kullback-Leibler distance is itself approximated through
// a cumulant-based expansion, since the pdfs are approximated through an
// Edgeworth series.
// The Kullback-Leibler distance is evaluated by:
// \begin{multline}\label{eqKLapprox1D}
//    K_{\text{Edgeworth}}(X_1 | X_2) = \frac{1}{12} \frac{\kappa_{X_1; 3}^2}{\kappa_{X_1; 2}^2}
//        + \frac{1}{2} \left( \log \frac{\kappa_{X_2; 2}}{\kappa_{X_1; 2}}
//                             -1+\frac{1}{\kappa_{X_2; 2}}
//                             \left( \kappa_{X_1; 1} - \kappa_{X_2; 1} +  \kappa_{X_1; 2}^{1/2} \right)^2
//                        \right) \\
//        - \left( \kappa_{X_2; 3} \frac{a_1}{6} + \kappa_{X_2; 4} \frac{a_2}{24}
//            + \kappa_{X_2; 3}^2 \frac{a_3}{72} \right)
//        - \frac{1}{2} \frac{ \kappa_{X_2; 3}^2}{36}
//            \left(
//                c_6 - 6 \frac{c_4}{\kappa_{X_1; 2}} + 9 \frac{c_2}{\kappa_{X_2; 2}^2}
//            \right) \\
//        - 10 \frac{\kappa_{X_1; 3} \kappa_{X_2; 3}
//                        \left( \kappa_{X_1; 1} - \kappa_{X_2; 1} \right)
//                        \left( \kappa_{X_1; 2} - \kappa_{X_2; 2} \right)}{\kappa_{X_2; 2}^6} \qquad
// \end{multline}
// where
// \begin{align*}
//    a_1 &= c_3 - 3 \frac{\alpha}{\kappa_{X_2; 2}} \\
//    a_2 &= c_4 - 6 \frac{c_2}{\kappa_{X_2; 2}} + \frac{3}{\kappa_{X_2; 2}^2} \\
//    a_3 &= c_6 - 15\frac{c_4}{\kappa_{X_2; 2}} + 45\frac{c_2}{\kappa_{X_2; 2}^2} -
//            \frac{15}{\kappa_{X_2; 2}^3} \\
//    c_2 &= \alpha^2 + \beta^2 \\
//    c_3 &= \alpha^3 + 3 \alpha \beta^2 \\
//    c_4 &= \alpha^4 + 6 \alpha^2 \beta^2 + 3 \beta^4 \\
//    c_6 &= \alpha^6 + 15\alpha^4 \beta^2 + 45 \alpha^2 \beta^4 + 15 \beta^6 \\
//    \alpha &= \frac{\kappa_{X_1; 1} - \kappa_{X_2; 1}}{\kappa_{X_2; 2}} \\
//    \beta &= \frac{ \kappa_{X_1; 2}^{1/2} }{\kappa_{X_2; 2}}.
// \end{align*}
// $\kappa_{X_i; 1}$, $\kappa_{X_i; 2}$, $\kappa_{X_i; 3}$ and $\kappa_{X_i; 4}$
// are the cumulants up to order 4 of the random variable $X_i$ ($i=1, 2$).
// This example will use the images shown in
// figure~\ref{fig:RATCHDETINIM}. These correspond to 2 Radarsat fine
// mode acquisitions before and after a lava flow resulting from a
// volcanic eruption.
//
// The program itself is very similar to the ratio of means detector,
// implemented in \doxygen{otb}{MeanRatioImageFilter},
// in section~\ref{sec:RatioOfMeans}. Nevertheless
// the corresponding header file has to be used instead.
//
// Software Guide : EndLatex

#include "itkMacro.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

//  Software Guide : BeginCodeSnippet
#include "otbKullbackLeiblerDistanceImageFilter.h"
//  Software Guide : EndCodeSnippet

int main(int argc, char * argv[])
{
  try
    {
    if (argc != 5)
      {
      std::cerr <<
      "Change detection through a Kullback-Leibler measure (which is a distance between local distributions)\n";
      std::cerr <<
      "Kullback-Leibler measure is optimized by a Edgeworth series expansion\n";
      std::cerr << argv[0] << " imgAv imgAp imgResu winSize\n";
      return 1;
      }

    char * fileName1 = argv[1];
    char * fileName2 = argv[2];
    char * fileNameOut = argv[3];
    int    winSize = atoi(argv[4]);

    const unsigned int Dimension = 2;
    typedef double        PixelType;
    typedef unsigned char OutputPixelType;

    typedef otb::Image<PixelType, Dimension>       ImageType;
    typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

    //  Software Guide : BeginLatex
    //
    //  The \doxygen{otb}{KullbackLeiblerDistanceImageFilter} is templated over
    //  the types of the two input images and the type of the generated change
    //  image, in a similar way as the \doxygen{otb}{MeanRatioImageFilter}. It is
    //  the only line to be changed from the ratio of means change detection
    //  example to perform a change detection through a distance between
    //  distributions...
    //
    //  Software Guide : EndLatex

    //  Software Guide : BeginCodeSnippet
    typedef otb::KullbackLeiblerDistanceImageFilter<ImageType,
        ImageType,
        ImageType> FilterType;
    //  Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    //
    //  The different elements of the pipeline can now be instantiated. Follow the
    //  ratio of means change detector example.
    //
    //  Software Guide : EndLatex

    typedef otb::ImageFileReader<ImageType>       ReaderType;
    typedef otb::ImageFileWriter<OutputImageType> WriterType;

    ReaderType::Pointer reader1 = ReaderType::New();
    reader1->SetFileName(fileName1);

    ReaderType::Pointer reader2 = ReaderType::New();
    reader2->SetFileName(fileName2);

    //  Software Guide : BeginLatex
    //
    //  The only parameter for this change detector is the radius of
    //  the window used for computing the cumulants.
    //
    //  Software Guide : EndLatex

    //  Software Guide : BeginCodeSnippet
    FilterType::Pointer filter = FilterType::New();
    filter->SetRadius((winSize - 1) / 2);
    //  Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    //
    //  The pipeline is built by plugging all the elements together.
    //
    //  Software Guide : EndLatex

    //  Software Guide : BeginCodeSnippet
    filter->SetInput1(reader1->GetOutput());
    filter->SetInput2(reader2->GetOutput());
    //  Software Guide : EndCodeSnippet

    typedef itk::RescaleIntensityImageFilter<ImageType,
        OutputImageType> RescaleFilterType;
    RescaleFilterType::Pointer rescaler = RescaleFilterType::New();

    rescaler->SetInput(filter->GetOutput());
    rescaler->SetOutputMinimum(0);
    rescaler->SetOutputMaximum(255);

    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(fileNameOut);
    writer->SetInput(rescaler->GetOutput());
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

  //  Software Guide : BeginLatex
  // Figure \ref{fig:RESKLDCHDET} shows the result of the change
  // detection by computing the Kullback-Leibler distance between
  // local pdf through an Edgeworth approximation.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.35\textwidth]{KLdistanceChDet.eps}
  // \itkcaption[Kullback-Leibler Change Detection Results]{Result of the
  // Kullback-Leibler change detector}
  // \label{fig:RESKLDCHDET}
  // \end{figure}
  //  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
