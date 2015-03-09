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


// Software Guide : BeginLatex
//
// The Majority Voting fusion filter \doxygen{itk}{LabelVotingImageFilter} used is based on ITK.
// For each pixel, it chooses the more frequent class label among the input classification maps.
// In case of not unique more frequent class labels, the output pixel is set to the \emph{undecidedLabel} value.
// We start by including the appropriate header file.
//
// Software Guide : EndLatex


// Software Guide : BeginCodeSnippet
#include "itkLabelVotingImageFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int main(int argc, char * argv[])
{
// Software Guide : BeginLatex
//
// We will assume unsigned short type input labeled images.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
  const unsigned int     Dimension = 2;
  typedef unsigned short LabelPixelType;
// Software Guide : EndCodeSnippet


  LabelPixelType undecidedLabel = atoi(argv[argc - 2]);
  const char * outfname = argv[argc - 1];

  unsigned int nbParameters = 2;
  unsigned int nbClassificationMaps = (argc - 1 - nbParameters);


// Software Guide : BeginLatex
//
// The input labeled images to be fused are expected to be scalar images.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
  typedef otb::Image<LabelPixelType, Dimension> LabelImageType;
// Software Guide : EndCodeSnippet


// Software Guide : BeginLatex
//
// The Majority Voting fusion filter \doxygen{itk}{LabelVotingImageFilter} based on ITK is templated
// over the input and output labeled image type.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
  // Majority Voting
  typedef itk::LabelVotingImageFilter<LabelImageType, LabelImageType>
                                                             LabelVotingFilterType;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Both reader and writer are defined. Since the images
// to classify can be very big, we will use a streamed writer which
// will trigger the streaming ability of the fusion filter.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<LabelImageType> ReaderType;
  typedef otb::ImageFileWriter<LabelImageType> WriterType;
// Software Guide : EndCodeSnippet


// Software Guide : BeginLatex
//
// The input classification maps to be fused are pushed into
// the \doxygen{itk}{LabelVotingImageFilter}. Moreover, the
// label value for the undecided pixels (in case of not unique
// majority voting) is set too.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
  ReaderType::Pointer reader;
  LabelVotingFilterType::Pointer labelVotingFilter = LabelVotingFilterType::New();
  for (unsigned int itCM = 0; itCM < nbClassificationMaps; ++itCM)
    {
    std::string fileNameClassifiedImage = argv[itCM + 1];

    reader = ReaderType::New();
    reader->SetFileName(fileNameClassifiedImage);
    reader->Update();

    labelVotingFilter->SetInput(itCM, reader->GetOutput());
    }

  labelVotingFilter->SetLabelForUndecidedPixels(undecidedLabel);
// Software Guide : EndCodeSnippet


// Software Guide : BeginLatex
//
// Once it is plugged the pipeline triggers its execution by updating
// the output of the writer.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(labelVotingFilter->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();
// Software Guide : EndCodeSnippet
  return EXIT_SUCCESS;
}
