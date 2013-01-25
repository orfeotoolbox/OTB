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
#include <iostream>

#include <otbImageFileReader.h>
#include "otbImageFileWriter.h"

#include "otbNeighborhoodMajorityVotingImageFilter.h"

#include "itkTimeProbe.h"


int otbNeighborhoodMajorityVotingImageFilterTest(int argc, char* argv[])
{
  typedef unsigned char InputLabelPixelType; // 8 bits
  typedef unsigned char OutputLabelPixelType; // 8 bits
  const unsigned int Dimension = 2;
  
  typedef otb::Image<InputLabelPixelType, Dimension> InputLabelImageType;
  typedef otb::Image<OutputLabelPixelType, Dimension> OutputLabelImageType;
  
  typedef otb::ImageFileReader<InputLabelImageType> ReaderType;
  typedef otb::ImageFileWriter<OutputLabelImageType> WriterType;
  
  
  // Neighborhood majority voting filter type
  typedef otb::NeighborhoodMajorityVotingImageFilter<InputLabelImageType, OutputLabelImageType> NeighborhoodMajorityVotingFilterType;
  
  // Binary ball Structuring Element type
  typedef NeighborhoodMajorityVotingFilterType::KernelType StructuringType;
  typedef StructuringType::RadiusType RadiusType;
  
  
  const char * inputFileName = argv[1];
  const char * outputFileName = argv[2];
  
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);
  
  // Neighborhood majority voting filter
  NeighborhoodMajorityVotingFilterType::Pointer NeighMajVotingFilter;
  NeighMajVotingFilter = NeighborhoodMajorityVotingFilterType::New();
  
  NeighMajVotingFilter->SetInput(reader->GetOutput());
  
  StructuringType seBall;
  RadiusType rad;
  

  std::string KeepOriginalLabelBoolStr = argv[3];
  if (KeepOriginalLabelBoolStr.compare("true") == 0)
  {
    NeighMajVotingFilter->SetKeepOriginalLabelBool(true);
  }
  else
  {
    NeighMajVotingFilter->SetKeepOriginalLabelBool(false);
  }
  
  
  if (argc >= 5)
  {
    rad[0] = atoi(argv[4]);
    rad[1] = atoi(argv[5]);
    
    if (argc >= 6)
    {
      NeighMajVotingFilter->SetLabelForNoDataPixels(atoi(argv[6]));
      
      if (argc >= 7)
      {
        NeighMajVotingFilter->SetLabelForUndecidedPixels(atoi(argv[7]));
      }
    }
  }
  else
  {
    rad[0] = 1;
    rad[1] = 1;
  }

  
  seBall.SetRadius(rad);
  seBall.CreateStructuringElement();
  NeighMajVotingFilter->SetKernel(seBall);
  
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput(NeighMajVotingFilter->GetOutput());
  writer->Update();
  
  return EXIT_SUCCESS;
}
