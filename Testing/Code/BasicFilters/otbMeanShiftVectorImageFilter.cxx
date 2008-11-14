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
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMeanShiftVectorImageFilter.h"

int otbMeanShiftVectorImageFilter(int argc, char * argv[])
{
  if(argc != 8)
    {
      std::cerr<<"Usage: "<<argv[0]<<" infname outfname spatialRadius rangeRadius maxNbIterations useImageSpacing convergenceDistanceThreshold"<<std::endl;
      return EXIT_FAILURE;
    }

  const char *       infname         = argv[1];
  const char *       outfname        = argv[2];
  const double       spatialRadius   = atof(argv[3]);
  const double       rangeRadius     = atof(argv[4]);
  const unsigned int maxNbIterations = atoi(argv[5]);
  const bool         useImageSpacing = atoi(argv[6]);
  const double       convergenceTol  = atof(argv[7]);

  const unsigned int Dimension = 2;
  typedef short PixelType;
  typedef otb::VectorImage<PixelType,Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;
  typedef otb::MeanShiftVectorImageFilter<ImageType, ImageType> FilterType;
  
  // Instantiating object
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  
  reader->SetFileName(infname);
  writer->SetFileName(outfname);
  
  filter->SetSpatialRadius(spatialRadius);
  filter->SetRangeRadius(rangeRadius);
  std::cout<<maxNbIterations<<std::endl;
  filter->SetMaxNumberOfIterations(maxNbIterations);
  filter->SetUseImageSpacing(useImageSpacing);
  filter->SetConvergenceDistanceThreshold(convergenceTol);

  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  
  writer->Update();

  /*
  typedef FilterType::ClusterImageType ClusterType;
  typedef otb::ImageFileWriter<ClusterType> Writer2Type;
  Writer2Type::Pointer w2 = Writer2Type::New();
  w2->SetFileName("cluster.tif");
  w2->SetInput(filter->GetClusterImage());
  w2->Update();
  */

  return EXIT_SUCCESS;
}
