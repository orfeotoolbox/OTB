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
#include "otbVirtualDimensionality.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingStatisticsVectorImageFilter.h"

const unsigned int Dimension = 2;
typedef double PixelType;

typedef otb::VectorImage<PixelType, Dimension> ImageType;
typedef otb::ImageFileReader<ImageType> ReaderType;
typedef otb::StreamingStatisticsVectorImageFilter<ImageType> StreamingStatisticsVectorImageFilterType;

typedef otb::VirtualDimensionality<double> VDType;

int otbVirtualDimensionalityNewTest(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  VDType::Pointer vd = VDType::New();
  std::cout << vd << std::endl;
  return EXIT_SUCCESS;
}


int otbVirtualDimensionalityTest(int itkNotUsed(argc), char * argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[2];

  StreamingStatisticsVectorImageFilterType::Pointer stats = StreamingStatisticsVectorImageFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  stats->SetInput(reader->GetOutput());
  stats->Update();

  VDType::Pointer vd = VDType::New();
  vd->SetCovariance(stats->GetCovariance().GetVnlMatrix());
  vd->SetCorrelation(stats->GetCorrelation().GetVnlMatrix());
  vd->SetNumberOfPixels(reader->GetOutput()->GetLargestPossibleRegion().GetNumberOfPixels());

  std::ofstream file;
  file.open(outfname);
  file.precision(5);

  for (int i = 2; i < 10; ++i)
    {
    double falseAlarmRate = vcl_pow(static_cast<double>(10), static_cast<double>(-i));
    vd->SetFAR(falseAlarmRate);
    vd->Compute();

    std::cout << "FAR : 1E-" << i << " -> Nb Endmembers: " << vd->GetNumberOfEndmembers() << std::endl;
    file << "FAR : 1E-" << i << " -> Nb Endmembers: " << vd->GetNumberOfEndmembers() << std::endl;
    }

  file.close();

  return EXIT_SUCCESS;
}
