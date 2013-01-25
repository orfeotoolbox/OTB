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
// After having generated a classification map, it is possible to
// regularize such a labeled image in order to obtain more homogeneous
// areas, which makes the interpretation of its classes easier. For this
// purpose, the \doxygen{otb}{NeighborhoodMajorityVotingImageFilter} was
// implemented. Like a morphological filter, this filter uses majority
// voting in a ball shaped neighborhood in order to set each pixel of the
// classification map to the more representative label value in its
// neighborhood.
//
// In this example we will illustrate its use. We start by including the
// appropriate header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbNeighborhoodMajorityVotingImageFilter.h"
// Software Guide : EndCodeSnippet

#include "itkMacro.h"
#include "otbImage.h"
#include <iostream>

#include <otbImageFileReader.h>
#include "otbImageFileWriter.h"

#include "itkTimeProbe.h"


int main(int argc, char * argv[])
{
// Software Guide : BeginLatex
//
// Since the input image is a classification map, we will assume a
// single band input image for which each pixel value is a label coded
// on 8 bits as an integer between 0 and 255.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet  
  typedef unsigned char InputLabelPixelType;
  typedef unsigned char OutputLabelPixelType;
  const unsigned int Dimension = 2;
// Software Guide : EndCodeSnippet
  
// Software Guide : BeginLatex
//
// Thus, both input and output images are single band labeled images,
// which are composed of the same type of pixels in this example
// (unsigned char).
//
// Software Guide : EndLatex
  
// Software Guide : BeginCodeSnippet  
  typedef otb::Image<InputLabelPixelType, Dimension> InputLabelImageType;
  typedef otb::Image<OutputLabelPixelType, Dimension> OutputLabelImageType;
// Software Guide : EndCodeSnippet


// Software Guide : BeginLatex
//
// We can now define the type for the neighborhood majority voting filter,
// which is templated over its input and output images types and over its
// structuring element type.
//
// Software Guide : EndLatex
  
// Software Guide : BeginCodeSnippet   
  // Neighborhood majority voting filter type
  typedef otb::NeighborhoodMajorityVotingImageFilter<InputLabelImageType,
   OutputLabelImageType> NeighborhoodMajorityVotingFilterType;
// Software Guide : EndCodeSnippet


  
// Software Guide : BeginLatex
//
// Since the \doxygen{otb}{NeighborhoodMajorityVotingImageFilter} is a
// neighborhood based image filter, it is necessary to set the structuring
// element which will be used for the majority voting process. By default, the
// structuring element is a ball
// (\doxygen{itk}{BinaryBallStructuringElement}) with a radius defined by two sizes
// (respectively along X and Y). Thus, it is possible to handle anisotropic
// structuring elements such as ovals.
//
// Software Guide : EndLatex
  
// Software Guide : BeginCodeSnippet
  // Binary ball Structuring Element type
  typedef NeighborhoodMajorityVotingFilterType::KernelType StructuringType;
  typedef StructuringType::RadiusType RadiusType;
// Software Guide : EndCodeSnippet



// Software Guide : BeginLatex
//
// Finally, we define the reader and the writer.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<InputLabelImageType> ReaderType;
  typedef otb::ImageFileWriter<OutputLabelImageType> WriterType;
// Software Guide : EndCodeSnippet
  
  
  const char * inputFileName = argv[1];
  const char * outputFileName = argv[2];
  

// Software Guide : BeginLatex
//
// We instantiate the \doxygen{otb}{NeighborhoodMajorityVotingImageFilter} and the 
// reader objects.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  // Neighborhood majority voting filter
  NeighborhoodMajorityVotingFilterType::Pointer NeighMajVotingFilter;
  NeighMajVotingFilter = NeighborhoodMajorityVotingFilterType::New();
  
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);
// Software Guide : EndCodeSnippet
  
  
  std::string KeepOriginalLabelBoolStr = argv[3];
  unsigned int radiusX = atoi(argv[4]);
  unsigned int radiusY = atoi(argv[5]);
  OutputLabelPixelType noDataValue = atoi(argv[6]);
  OutputLabelPixelType undecidedValue = atoi(argv[7]);
  
  
// Software Guide : BeginLatex
//
// The ball shaped structuring element seBall is instantiated and its
// two radii along X and Y are initialized.
//
// Software Guide : EndLatex
  
// Software Guide : BeginCodeSnippet
  StructuringType seBall;
  RadiusType rad;
  
  rad[0] = radiusX;
  rad[1] = radiusY;

  seBall.SetRadius(rad);
  seBall.CreateStructuringElement();
// Software Guide : EndCodeSnippet
  
  
// Software Guide : BeginLatex
//
// Then, this ball shaped neighborhood is used as the kernel structuring element
// for the \doxygen{otb}{NeighborhoodMajorityVotingImageFilter}.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  NeighMajVotingFilter->SetKernel(seBall);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Not classified input pixels are assumed to have the noDataValue label
// and will keep this label in the output image.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  NeighMajVotingFilter->SetLabelForNoDataPixels(noDataValue);
// Software Guide : EndCodeSnippet


// Software Guide : BeginLatex
//
// Moreover, since the majority voting regularization may lead to not unique
// majority labels in the neighborhood, it is important to define which behaviour
// the filter must have in this case. For this purpose, a Boolean parameter is used
// in the filter to choose whether pixels with more than one majority class are set
// to undecidedValue (true), or to their Original labels (false = default value)
// in the output image.
//
// Software Guide : EndLatex


// Software Guide : BeginCodeSnippet
  NeighMajVotingFilter->SetLabelForUndecidedPixels(undecidedValue);
  
  if (KeepOriginalLabelBoolStr.compare("true") == 0)
  {
    NeighMajVotingFilter->SetKeepOriginalLabelBool(true);
  }
  else
  {
    NeighMajVotingFilter->SetKeepOriginalLabelBool(false);
  }
// Software Guide : EndCodeSnippet


// Software Guide : BeginLatex
//
// We plug the pipeline and
// trigger its execution by updating the output of the writer.
//
// Software Guide : EndLatex


// Software Guide : BeginCodeSnippet
  NeighMajVotingFilter->SetInput(reader->GetOutput());
  
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput(NeighMajVotingFilter->GetOutput());
  writer->Update();
// Software Guide : EndCodeSnippet
  
  return EXIT_SUCCESS;
}
