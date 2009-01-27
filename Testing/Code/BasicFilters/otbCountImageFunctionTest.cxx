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

#include <stdio.h>
#include "otbCountImageFunction.h"
#include "otbSiftFastImageFilter.h"
#include "otbSimplePointCountStrategy.h"
#include "itkPointSet.h"
#include "itkVariableLengthVector.h"

#include <iostream>

int otbCountImageFunctionTest(int argc, char* argv[] )
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
  
  typedef itk::VariableLengthVector<PixelType>              RealVectorType;
  typedef itk::PointSet<RealVectorType,Dimension>           PointSetType;
  typedef otb::SiftFastImageFilter<ImageType,PointSetType>  DetectorType;
  
  typedef otb::Count<PointSetType,unsigned int ,IndexType>  CounterType;
  
  typedef otb::CountImageFunction< ImageType,DetectorType,
                                             CounterType>   FunctionType;


  /** Instanciation of the reader */
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->Update();
  
  ImageType::SizeType                     size;
  size = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
  
  /**Instancitation of an object*/
  FunctionType::Pointer    filter =     FunctionType::New();
  
  /** Instanciation of the detector : */
  DetectorType::Pointer detector = filter->GetDetector();
  detector->SetNumberOfScales(scales);
  

  filter->SetInputImage(reader->GetOutput()); 
  filter->SetNeighborhoodRadius(radius);
  filter->SetDetector(detector);  /*Set the number of scales for the detector**/

  /** First try*/
  ImageType::IndexType index ; 
  index[0] = size[0]/2 ;
  index[1] = size[1]/4;

  std::ofstream outfile(outfname);
  outfile << "At Index: "   << index << std::endl;
  outfile << "\t density :" << filter->EvaluateAtIndex(index) << std::endl;

  /** Second Try*/
  index[0] = size[0]/4 ;
  index[1] = size[1]/4;

  outfile << "At Index: "   << index << std::endl;
  outfile << "\t density :" << filter->EvaluateAtIndex(index) << std::endl;

  /** Third Try*/
  index[0] = size[0]/2 ;
  index[1] = size[1]/2;

  outfile << "At Index: "   << index << std::endl;
  outfile << "\t density :" << filter->EvaluateAtIndex(index) << std::endl;
  
  outfile.close();
  

  return EXIT_SUCCESS;
}

