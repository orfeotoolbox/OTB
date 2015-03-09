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

#include "otbStreamingStatisticsImageFilter.h"
#include "itkStatisticsImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImage.h"
#include <fstream>
#include "otbStreamingTraits.h"

int otbStreamingStatisticsImageFilter(int argc, char * argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[2];
  float userIgnoredValue = 0.;
  if (argc>3)
    {
    userIgnoredValue = atof(argv[3]);
    }

  const unsigned int Dimension = 2;
  typedef double PixelType;

  typedef otb::Image<PixelType, Dimension>               ImageType;
  typedef otb::ImageFileReader<ImageType>                ReaderType;
  typedef otb::StreamingStatisticsImageFilter<ImageType> StreamingStatisticsImageFilterType;
  //typedef itk::StatisticsImageFilter<ImageType> StreamingStatisticsImageFilterType;
  // Instantiating object
  StreamingStatisticsImageFilterType::Pointer filter = StreamingStatisticsImageFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  filter->GetStreamer()->SetNumberOfLinesStrippedStreaming( 10 );
  filter->SetInput(reader->GetOutput());
  if (argc>3)
    {
    filter->SetIgnoreUserDefinedValue(true);
    filter->SetUserIgnoredValue(userIgnoredValue);
    }
  filter->Update();

  std::ofstream file;
  file.open(outfname);
  file << "Minimum: " << filter->GetMinimum() << std::endl;
  file << "Maximum: " << filter->GetMaximum() << std::endl;
  file << std::fixed;
  file.precision(5);
  file << "Sum: " << filter->GetSum() << std::endl;
  file << "Mean: " << filter->GetMean() << std::endl;
  file << "Sigma: " << filter->GetSigma() << std::endl;
  file << "Variance: " << filter->GetVariance() << std::endl;
  file.close();

  return EXIT_SUCCESS;
}
