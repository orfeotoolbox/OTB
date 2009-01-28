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

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include <stdio.h>
#include "otbCountImageFilter.h"
#include "otbSiftFastImageFilter.h"
#include "otbSimplePointCountStrategy.h"
#include "itkPointSet.h"
#include "itkVariableLengthVector.h"

#include <iostream>

int otbCountImageFilterTest(int argc, char* argv[] )
{
  const char *  infname = argv[1];            
  const char * outfname = argv[2];
  const unsigned char scales = atoi(argv[3]);
  const unsigned char radius = atoi(argv[4]);
  const   unsigned int                                      Dimension = 2;
  typedef float                                             PixelType; 

  typedef otb::Image< PixelType, Dimension >                ImageType;
  typedef ImageType::IndexType                              IndexType;
  
  typedef otb::ImageFileReader<ImageType>                   ReaderType;
  typedef otb::ImageFileWriter<ImageType>                   WriterType;
  
  typedef itk::VariableLengthVector<PixelType>              RealVectorType;
  typedef itk::PointSet<RealVectorType,Dimension>           PointSetType;
  typedef otb::SiftFastImageFilter<ImageType,PointSetType>  DetectorType;
  
  typedef otb::Count<PointSetType,unsigned int ,IndexType>  CounterType;
  
  typedef otb::CountImageFilter< ImageType,DetectorType,
                                   CounterType, ImageType>  FilterType;


  /** Instanciation of the reader */
  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(infname);
  reader->Update();
  
  ImageType::SizeType                     size;
  size = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
  
  /**Instancitation of an object*/
  FilterType::Pointer    filter =     FilterType::New();
  
  /** Instanciation of the detector : */
  DetectorType::Pointer detector = filter->GetDetector();
  detector->SetNumberOfScales(scales);
  

  filter->SetInput(reader->GetOutput()); 
  filter->SetNeighborhoodRadius(radius);
  filter->SetDetector(detector);  /*Set the number of scales for the detector**/

  
  /** Output*/
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outfname);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}

