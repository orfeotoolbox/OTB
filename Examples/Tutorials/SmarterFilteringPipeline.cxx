/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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


//  Software Guide : BeginCommandLineArgs
//    INPUTS: {QB_Suburb.png}
//    OUTPUTS: {TutorialsSmarterFilteringPipelineOutput.png}
//    -d 1.5 -i 2 -a 0.1
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
//
//
//  We are going to use the \doxygen{otb}{HarrisImageFilter}
// to find the points of interest in one image.
//
// The derivative computation is performed by a
// convolution with the derivative of a Gaussian kernel of
// variance $\sigma_D$ (derivation scale) and
//     the smoothing of the image is performed by convolving with a
//     Gaussian kernel of variance $\sigma_I$ (integration
// scale). This allows the computation of the following matrix:
//
//   \[
//   \mu(\mathbf{x},\sigma_I,\sigma_D) = \sigma_D^2 g(\sigma_I)\star
//   \left[\begin{array}{cc} L_x^2(\mathbf{x},\sigma_D) &
//   L_xL_y^2(\mathbf{x},\sigma_D)\\ L_xL_y^2(\mathbf{x},\sigma_D)&
//   L_y^2(\mathbf{x},\sigma_D) \end{array}\right]
//   \]
//
//     The output of the detector is $\mathrm{det}(\mu) - \alpha
// \mathrm{trace}^2(\mu)$.
//
// We want to set 3 parameters of this filter through the command line:
// $\sigma_D$ (SigmaD), $\sigma_I$ (SigmaI) and $\alpha$ (Alpha).
//
// We are also going to do the things properly and catch the exceptions.
//
// Let first add the two following headers:
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "itkMacro.h"

// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
// The first one is to handle the exceptions, the second one to help us
// parse the command line.
//
// We include the other required headers, without forgetting to add the header
// for the \doxygen{otb}{HarrisImageFilter}. Then we start the usual main
// function.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbHarrisImageFilter.h"

int main(int argc, char * argv[])
{
// Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // To handle the exceptions properly, we need to put all the instructions
  // inside a \code{try}.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  try
    {
    //  Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    // 
    //  First we document the usage of the program, and get the different 
    //  variables $\sigma_D$ (d=), $\sigma_I$ (i=) and $\alpha$ (a=) in 
    //  c++ variables :
    //
    //  Software Guide : EndLatex

    //  Software Guide : BeginCodeSnippet
    if ( argc < 3 )
      {
      std::cerr << "This program applies the Harris detector on the input "
      << "image\n";
      std::cerr << "Usage : " << argv[0] ;
      std::cerr << " input_filename output_filename [d=1.0] [i=1.0] [a=1.0]\n";
      std::cerr << "\"d=\" is for the sigmaD parameter, default value is 1.0\n"
      std::cerr << "\"i=\" is for the sigmaI parameter, default value is 1.0\n"
      std::cerr << "\"a=\" is for the alpha parameter, default value is 1.0\n"
      return EXIT_FAILURE;
      }
    std::string input_filename = argv[1];
    std::string output_filename = argv[2];
    bool is_there_sigma_d(false), is_there_sigma_i(false), 
      is_there_alpha(false);
    double sigma_d(1.0), sigma_i(1.0), alpha(1.0);
    for (auto i = 3 ; i < argc ; i++ )
      {
      std::string temp_arg = argv[i];
      auto pos_sigma_d = temp_arg.find("d=");
      auto pos_sigma_i = temp_arg.find("i=");
      auto pos_alpha = temp_arg.find("a=");
      if ( pos_sigma_d != std::string::npos )
        {
        is_there_sigma_d = true;
        sigma_d = std::stod(temp_arg.substr(pos_sigma_d+1));
        continue;
        }
      if ( pos_sigma_i != std::string::npos )
        {
        is_there_sigma_i = true;
        sigma_i = std::stod(temp_arg.substr(pos_sigma_i+1));
        continue;
        }
      if ( pos_alpha != std::string::npos )
        {
        is_there_alpha = true;
        alpha = std::stod(temp_arg.substr(pos_alpha+1));
        continue;
        }
      }
    //  Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    //
    //  Now, we can declare the image type, the reader and the writer as
    //  before:
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    typedef double                   PixelType;
    typedef otb::Image<PixelType, 2> ImageType;

    typedef unsigned char                  OutputPixelType;
    typedef otb::Image<OutputPixelType, 2> OutputImageType;

    typedef otb::ImageFileReader<ImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();

    typedef otb::ImageFileWriter<OutputImageType> WriterType;
    WriterType::Pointer writer = WriterType::New();
    // Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    //
    //  We are setting the filenames of the input and the output and the
    //  reader and writer respectively:
    //
    //  Software Guide : BeginLatex

    // Software Guide : BeginCodeSnippet
    reader->SetFileName( input_filename );
    writer->SetFileName( output_filename );
    // Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    //
    // Now we have to declare the filter. It is templated with the
    // input image type and the output image type like many filters
    // in OTB. Here we are using the same type for the input and the
    // output images:
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    typedef otb::HarrisImageFilter
    <ImageType, ImageType> FilterType;
    FilterType::Pointer filter = FilterType::New();
    // Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    //
    //  We set the filter parameters from the variables we created from the 
    //  commandline.
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    if ( is_there_sigma_d )
      filter->SetSigmaD( sigma_d );

    if ( is_there_sigma_i )
      filter->SetSigmaI( sigma_i );

    if ( is_there_alpha )
      filter->SetAlpha( alpha );
    // Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    //
    // We add the rescaler filter as before
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    typedef itk::RescaleIntensityImageFilter
    <ImageType, OutputImageType> RescalerType;
    RescalerType::Pointer rescaler = RescalerType::New();

    rescaler->SetOutputMinimum(0);
    rescaler->SetOutputMaximum(255);
    // Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    //
    // Let's plug the pipeline:
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    filter->SetInput(reader->GetOutput());
    rescaler->SetInput(filter->GetOutput());
    writer->SetInput(rescaler->GetOutput());
    // Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    //
    // We trigger the pipeline execution calling the \code{Update()}
    // method on the writer
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    writer->Update();

    }
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Finally, we have to handle exceptions we may have raised before
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  catch (itk::ExceptionObject& err)
    {
    std::cout << "Following otbException catch :" << std::endl;
    std::cout << err << std::endl;
    return EXIT_FAILURE;
    }
  catch (std::bad_alloc& err)
    {
    std::cout << "Exception bad_alloc : " << (char*) err.what() << std::endl;
    return EXIT_FAILURE;
    }
  catch (...)
    {
    std::cout << "Unknown Exception found !" << std::endl;
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
// Software Guide : EndCodeSnippet
