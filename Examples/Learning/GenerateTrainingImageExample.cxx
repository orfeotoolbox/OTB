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
//    INPUTS: {ROI_QB_MUL_4.tif}, {LearningROIs.txt}
//    OUTPUTS: {ROI_QB_MUL_4_training.png}
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates how to generate a training image for an
// image supervised classification.
//
// Software Guide : EndLatex

#include "itkNumericTraits.h"
#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"
#include "itkImageRegionIterator.h"

#include <stdio.h>
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
  if (argc != 4)
    {

    std::cout << "Usage : " << argv[0] <<
    " inputImage roiFile outputTrainingImage" << std::endl;
    return EXIT_FAILURE;

    }

  const char * imageFilename  = argv[1];
  const char * roiFilename  = argv[2];
  const char * outputFilename = argv[3];

  typedef unsigned char InputPixelType;
  typedef unsigned char OutputPixelType;

  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(imageFilename);
  writer->SetFileName(outputFilename);

  reader->Update();

  // We allocate the output image
  OutputImageType::Pointer trainingImage = OutputImageType::New();

  // Declare the type of the index to access images
  typedef itk::Index<Dimension> myIndexType;

  // Declare the type of the size
  typedef itk::Size<Dimension> mySizeType;

  // Declare the type of the Region
  typedef itk::ImageRegion<Dimension> myRegionType;

  // Define their size, and start index
  mySizeType size;
  size[0] = reader->GetOutput()->GetRequestedRegion().GetSize()[0];
  size[1] = reader->GetOutput()->GetRequestedRegion().GetSize()[1];

  myIndexType start;
  start[0] = 0;
  start[1] = 0;

  myRegionType region;
  region.SetIndex(start);
  region.SetSize(size);

  // Initialize Image B
  trainingImage->SetLargestPossibleRegion(region);
  trainingImage->SetBufferedRegion(region);
  trainingImage->SetRequestedRegion(region);
  trainingImage->Allocate();
  OutputPixelType pix;
  pix = itk::NumericTraits<OutputPixelType>::Zero;
  trainingImage->FillBuffer(pix);

  // For each line of the ROIs file, we create a region iterator and
  // assign the value of the labels to the pixels

  std::ifstream roisFile;
  roisFile.open(roiFilename);

  unsigned int nbRois = 0;

  roisFile >> nbRois;

  std::cout << "Nb of ROIS " << int(nbRois) << std::endl;

  while (!roisFile.fail() && (nbRois > 0))
    {

    --nbRois;
    OutputPixelType label = 0;
    unsigned long   xUL, yUL, xBR, yBR, tmp_label = 0;

    roisFile >> tmp_label;
    roisFile >> xUL;
    roisFile >> yUL;
    roisFile >> xBR;
    roisFile >> yBR;

    label = static_cast<OutputPixelType>(tmp_label);

    std::cout << "Label : " << int(label) << std::endl;
    std::cout << "( " << xUL << " , " << yUL << " )" << std::endl;
    std::cout << "( " << xBR << " , " << yBR << " )" << std::endl;

    // Region creation

    start[0] = xUL;
    start[1] = yUL;

    size[0]  = xBR - xUL;
    size[1]  = yBR - yUL;

    region.SetSize(size);
    region.SetIndex(start);

    // Iterator creation
    typedef itk::ImageRegionIterator<OutputImageType> IteratorType;
    IteratorType it(trainingImage, region);

    it.GoToBegin();

    // Iteration and pixel value assignment
    while (!it.IsAtEnd())
      {

      it.Set(static_cast<OutputPixelType>(label));

      //std::cout << (int)static_cast<OutputPixelType>(label) << " -- ";
      //std::cout << (int)it.Get() << std::endl;
      ++it;

      }

    }

  writer->SetInput(trainingImage);

  writer->Update();

  return EXIT_SUCCESS;
}
