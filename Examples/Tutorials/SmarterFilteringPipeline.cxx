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
#include "otbCommandLineArgumentParser.h"
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
    // Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    //
    // Now, we can declare the \doxygen{otb}{CommandLineArgumentParser} which is
    // going to parse the command line, select the proper variables, handle the
    // missing compulsory arguments and print an error message if necessary.
    //
    // Let's declare the parser:
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    typedef otb::CommandLineArgumentParser ParserType;
    ParserType::Pointer parser = ParserType::New();
    // Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    //
    //  It's now time to tell the parser what are the options we want. Special
    //options are available for input and output images with the
    // \code{AddInputImage()} and \code{AddOutputImage()} methods.
    //
    //  For the other options, we need to use the \code{AddOption()} method.
    //  This method allows us to specify
    //  \begin{itemize}
    //  \item the name of the option
    //  \item a message to explain the meaning of this option
    //  \item a shortcut for this option
    //  \item the number of expected parameters for this option
    //  \item whether or not this option is compulsory
    //  \end{itemize}
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    parser->SetProgramDescription(
      "This program applies a Harris detector on the input image");
    parser->AddInputImage();
    parser->AddOutputImage();
    parser->AddOption("--SigmaD",
                      "Set the sigmaD parameter. Default is 1.0.",
                      "-d",
                      1,
                      false);
    parser->AddOption("--SigmaI",
                      "Set the sigmaI parameter. Default is 1.0.",
                      "-i",
                      1,
                      false);
    parser->AddOption("--Alpha",
                      "Set the alpha parameter. Default is 1.0.",
                      "-a",
                      1,
                      false);
    // Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    //
    //  Now that the parser has all this information, it can actually look at
    // the command line to parse it. We have to do this within a \code{try} -
    // \code{catch} loop to handle exceptions nicely.
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    typedef otb::CommandLineArgumentParseResult ParserResultType;
    ParserResultType::Pointer parseResult = ParserResultType::New();

    try
      {
      parser->ParseCommandLine(argc, argv, parseResult);
      }

    catch (itk::ExceptionObject& err)
      {
      std::string descriptionException = err.GetDescription();
      if (descriptionException.find("ParseCommandLine(): Help Parser")
          != std::string::npos)
        {
        return EXIT_SUCCESS;
        }
      if (descriptionException.find("ParseCommandLine(): Version Parser")
          != std::string::npos)
        {
        return EXIT_SUCCESS;
        }
      return EXIT_FAILURE;
      }
    // Software Guide : EndCodeSnippet

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
    //  We are getting the filenames for the input and the output
    //  images directly from the parser:
    //
    //  Software Guide : BeginLatex

    // Software Guide : BeginCodeSnippet
    reader->SetFileName(parseResult->GetInputImage().c_str());
    writer->SetFileName(parseResult->GetOutputImage().c_str());
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
    //  We set the filter parameters from the parser. The method
    //  \code{IsOptionPresent()} let us know if an optional option
    //  was provided in the command line.
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    if (parseResult->IsOptionPresent("--SigmaD"))
      filter->SetSigmaD(
        parseResult->GetParameterDouble("--SigmaD"));

    if (parseResult->IsOptionPresent("--SigmaI"))
      filter->SetSigmaI(
        parseResult->GetParameterDouble("--SigmaI"));

    if (parseResult->IsOptionPresent("--Alpha"))
      filter->SetAlpha(
        parseResult->GetParameterDouble("--Alpha"));
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
