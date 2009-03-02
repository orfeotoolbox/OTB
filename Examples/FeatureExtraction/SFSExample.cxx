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
#include "itkExceptionObject.h"

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {qb_RoadExtract2.tif}
//    OUTPUTS: {SFSLengthOutput.tif}, {SFSWidthOutput.tif}, {SFSMeanOutput.tif}, {SFSRatioOutput.tif}, {SFSSDOutput.tif}, {SFSPsiOutput.tif}
//    20 50 8 4 0.6
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the
// \doxygen{otb}{SFSTexturesImageFilter}. 
// This filter computes the Structural Feature Set as descibed in
// \cite{SFS}. These features are textural parameters which give
// information about the structure of lines passing through each pixel
// of the image.
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbSFSTexturesImageFilter.h"
// Software Guide : EndCodeSnippet

int main(int argc, char * argv[])
{
  typedef double PixelType;
  const unsigned int Dimension =2;

  std::string inName            = argv[1];
  std::string outNameLength     = argv[2];
  std::string outNameWidth      = argv[3];
  std::string outNameWMean      = argv[4];
  std::string outNameRatio      = argv[5];
  std::string outNameSD         = argv[6];
  std::string outNamePsi         = argv[7];
  PixelType spectThresh         = atof(argv[8]);
  unsigned int spatialThresh    = atoi(argv[9]);
  unsigned int dirNb            = atoi(argv[10]);
  unsigned int maxConsideration = atoi(argv[11]);
  double alpha                  = atof(argv[12]);  


  typedef otb::Image<PixelType,Dimension>                   ImageType;
  typedef ImageType::PixelType                              InputPixelType;
  typedef otb::ImageFileReader<ImageType>                   ReaderType;
  typedef otb::ImageFileWriter<ImageType>                   WriterType;
  typedef otb::SFSTexturesImageFilter<ImageType, ImageType> FilterType;

  FilterType::Pointer filter       = FilterType::New(); 
  ReaderType::Pointer reader       = ReaderType::New();
  WriterType::Pointer writerLength = WriterType::New();
  WriterType::Pointer writerWidth  = WriterType::New();
  WriterType::Pointer writerWMean  = WriterType::New();
  WriterType::Pointer writerRatio  = WriterType::New();
  WriterType::Pointer writerSD     = WriterType::New();
  WriterType::Pointer writerPsi     = WriterType::New();

  reader->SetFileName(inName);
  reader->GenerateOutputInformation();
   
  filter->SetSpectralThreshold(spectThresh);
  filter->SetSpatialThreshold(spatialThresh);
  filter->SetNumberOfDirections(dirNb);
  filter->SetRatioMaxConsiderationNumber(maxConsideration);
  filter->SetAlpha(alpha);
  // disable PSI texture
  filter->SetTextureStatus(3, true);
  filter->SetInput( reader->GetOutput() );


  writerLength->SetFileName(outNameLength);
  writerLength->SetInput( filter->GetLengthOutput() );
  writerLength->Update();

  writerWidth->SetFileName(outNameWidth);
  writerWidth->SetInput( filter->GetWidthOutput() );
  writerWidth->Update();

  writerWMean->SetFileName(outNameWMean);
  writerWMean->SetInput( filter->GetWMeanOutput() );
  writerWMean->Update();

  writerRatio->SetFileName(outNameRatio);
  writerRatio->SetInput( filter->GetRatioOutput() );
  writerRatio->Update();

  writerSD->SetFileName(outNameSD);
  writerSD->SetInput( filter->GetSDOutput() );
  writerSD->Update();

  writerPsi->SetFileName(outNamePsi);
  writerPsi->SetInput( filter->GetPSIOutput() );
  writerPsi->Update();


  return EXIT_SUCCESS;
}
