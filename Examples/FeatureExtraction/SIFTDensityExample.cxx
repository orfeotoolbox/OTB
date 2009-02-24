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

#include "itkPointSet.h"
#include "itkVariableLengthVector.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {suburb2.jpeg}
//    OUTPUTS: {SIFTDensityOutput.tif}, {PrettySIFTDensityOutput.png}
//    3 3 7
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the
// \doxygen{otb}{KeyPointDensityImageFilter}. 
// This filter computes a local density of keypoints (SIFT or SURF,
// for instance) on an image and can
// be useful to detect man made objects or urban areas, for
// instance. The filter has been implemented in a generic way, so that
// the way the keypoints are detected can be chosen by the user.
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbKeyPointDensityImageFilter.h"
// Software Guide : EndCodeSnippet
#include "otbImageToSIFTKeyPointSetFilter.h"

int main(int argc, char* argv[] )
{
  const char * infname = argv[1];
  const char * outfname = argv[2];
  const char * prettyfname = argv[3];
  const unsigned int scales = atoi(argv[4]);
  const unsigned int octaves = atoi(argv[5]);
  const unsigned int radius = atoi(argv[6]);

  const   unsigned int         Dimension = 2;
  typedef float                PixelType;

  // Software Guide : BeginLatex
  //
  // As usual, we start by defining the types for the images, the reader
  // and the writer.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::Image< PixelType, Dimension >         ImageType;
  typedef otb::ImageFileReader<ImageType>            ReaderType;
  typedef otb::ImageFileWriter<ImageType>            WriterType;

    // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // We define now the type for the keypoint detection. The keypoints
  // will be stored in vector form (they may contain many descriptors)
  // into a point set. The filter for detecting the SIFT is templated
  // over the input image type and the output pointset type.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  
  typedef itk::VariableLengthVector<PixelType>       RealVectorType;
  typedef itk::PointSet<RealVectorType,Dimension>   PointSetType;
  typedef otb::ImageToSIFTKeyPointSetFilter<ImageType, PointSetType>
                                                        DetectorType;
    // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // We can now define the filter which will compute the SIFT
  // density. It will be templated over the input and output image
  // types and the SIFT detector.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::KeyPointDensityImageFilter<ImageType, ImageType, DetectorType>
                                                                   FilterType;


  /**Instancitation of an object*/
  FilterType::Pointer    filter =     FilterType::New();
  DetectorType::Pointer  detector = DetectorType::New();
  ReaderType::Pointer      reader = ReaderType::New();

  reader->SetFileName(infname);
  reader->GenerateOutputInformation();

  filter->SetInput(reader->GetOutput());
  detector = filter->GetDetector();
  detector ->SetOctavesNumber(octaves);
  detector->SetScalesNumber(scales);


  filter->SetDetector(detector);
  filter->SetNeighborhoodRadius(radius);

  /** Writing the densty Image*/
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outfname);
  writer->SetInput(filter->GetOutput());
  writer->Update();



  return EXIT_SUCCESS;
}

