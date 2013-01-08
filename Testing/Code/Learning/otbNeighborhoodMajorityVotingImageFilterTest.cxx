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


int otbNeighborhoodMajorityVotingImageFilterTest(int argc, char* argv[]) {
       typedef unsigned char LabelPixelType;
       const unsigned int Dimension = 2;

       typedef otb::Image<LabelPixelType, Dimension> LabelImageType;

       typedef otb::ImageFileReader<LabelImageType> ReaderType;
       typedef otb::ImageFileWriter<LabelImageType> WriterType;

       typedef otb::NeighborhoodMajorityVotingImageFilter<LabelImageType> NeighborhoodMajorityVotingFilterType;
       typedef NeighborhoodMajorityVotingFilterType::RadiusType RadiusType;


       const char * inputFileName = argv[1];
       const char * outputFileName = argv[2];

       ReaderType::Pointer reader = ReaderType::New();
       reader->SetFileName(inputFileName);
       
       //NEIGHBORHOOD MAJORITY FILTER
       NeighborhoodMajorityVotingFilterType::Pointer NeighMajVotingFilter;
       NeighMajVotingFilter = NeighborhoodMajorityVotingFilterType::New();
       
       NeighMajVotingFilter->SetInput(reader->GetOutput());
       
       if(argc >= 4)
       {
              RadiusType rad;
              rad[0] = atoi(argv[3]);
              rad[1] = atoi(argv[4]);
              NeighMajVotingFilter->SetRadiusNeighborhood(rad);
              
              if(argc >= 5)
              {
                     NeighMajVotingFilter->SetNoDataValue(atoi(argv[5]));
                     
                     if(argc >= 6)
                     {
                            NeighMajVotingFilter->SetUndefinedValue(atoi(argv[6]));
                     }
              }
       }
       

       WriterType::Pointer writer = WriterType::New();
       writer->SetFileName(outputFileName);
       writer->SetInput(NeighMajVotingFilter->GetOutput());
       writer->Update();

       return EXIT_SUCCESS;
}
