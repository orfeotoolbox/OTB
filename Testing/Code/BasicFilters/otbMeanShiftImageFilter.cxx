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
#include "otbStreamingImageFileWriter.h"
#include "otbMeanShiftImageFilter.h"

int otbMeanShiftImageFilter(int argc, char * argv[])
{
  if(argc != 8)
    {
      std::cerr<<"Usage: "<<argv[0]<<" infname filteredfname clusteredfname spatialRadius rangeRadius minregionsize scale"<<std::endl;
      return EXIT_FAILURE;
    }

  const char *       infname         = argv[1];
  const char *       filteredfname   = argv[2];
  const char *       clusteredfname  = argv[3];
  const unsigned int spatialRadius   = atoi(argv[4]);
  const double       rangeRadius     = atof(argv[5]);
  const unsigned int minRegionSize   = atoi(argv[6]);
  const double       scale           = atoi(argv[7]);

  const unsigned int Dimension = 2;
  typedef float PixelType;
  typedef otb::Image<PixelType,Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::StreamingImageFileWriter<ImageType> WriterType;
  typedef otb::MeanShiftImageFilter<ImageType,ImageType> FilterType;
  
  // Instantiating object
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer1 = WriterType::New();
  WriterType::Pointer writer2 = WriterType::New();
 
  reader->SetFileName(infname);
  writer1->SetFileName(filteredfname);
  writer2->SetFileName(clusteredfname);
  
  filter->SetSpatialRadius(spatialRadius);
  filter->SetRangeRadius(rangeRadius);
  filter->SetMinimumRegionSize(minRegionSize);
  filter->SetScale(scale);

  filter->SetInput(reader->GetOutput());
  writer1->SetInput(filter->GetOutput());
  writer2->SetInput(filter->GetClusteredOutput());

  writer1->Update();
  writer2->Update();

  return EXIT_SUCCESS;
}
