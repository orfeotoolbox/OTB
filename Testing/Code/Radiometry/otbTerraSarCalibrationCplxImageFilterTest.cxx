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

#include "otbRadarFunctors.h"
#include "otbUnaryFunctorWithIndexImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbTerraSarCalibrationCplxImageFilterTest(int argc, char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];
 
  typedef std::complex<double>                                      CplxType;
  typedef otb::Image<CplxType, 2>                                   ImageType;
  typedef otb::ImageFileReader<ImageType>                           ReaderType;
  typedef otb::ImageFileWriter<ImageType>                           WriterType;
  typedef otb::Functor::TerraSarCalibrationComplexImageFunctor< CplxType, CplxType > FunctorType;
  typedef otb::UnaryFunctorWithIndexImageFilter< ImageType,ImageType, FunctorType> FilterType;
  typedef FunctorType::DoubleVectorType                              DoubleVectorType;
  typedef FunctorType::DoubleVectorVectorType                        DoubleVectorVectorType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  FilterType::Pointer filter = FilterType::New();

  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);

  reader->UpdateOutputInformation();
 
  DoubleVectorType coefs;
  coefs.push_back(1.);
  coefs.push_back(0.5);
  coefs.push_back(1.);
  coefs.push_back(0.1);
  DoubleVectorVectorType coefVect(1, coefs);

  coefs.clear();
  coefs.push_back(10.);
  coefs.push_back(5);
  coefs.push_back(10);
  coefs.push_back(1);
  coefVect.push_back( coefs );
  
  coefs.clear();
  coefs.push_back(100);
  coefs.push_back(50);
  coefs.push_back(100);
  coefs.push_back(10);
  coefVect.push_back( coefs );

  filter->GetFunctor().SetNoisePolynomialCoefficientsList(coefVect);

  filter->GetFunctor().SetCalFactor( 10 );
  filter->GetFunctor().SetNoiseRangeValidityMin( 0 );
  filter->GetFunctor().SetNoiseRangeValidityMax( 1 );
  filter->GetFunctor().SetNoiseRangeValidityRef( 0.5 );
  filter->GetFunctor().SetLocalIncidentAngle( 15 );
  filter->GetFunctor().SetLocalIncidentAngle( 15 );

  std::vector<long int> timeUtc;
  timeUtc.push_back(1);
  timeUtc.push_back(2);
  timeUtc.push_back(3);
  filter->GetFunctor().SetTimeUTC(timeUtc);
  filter->GetFunctor().SetPRF(50);
  filter->GetFunctor().SetUseFastCalibrationMethod( false );
  
  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->Update();
  

  return EXIT_SUCCESS;
}
