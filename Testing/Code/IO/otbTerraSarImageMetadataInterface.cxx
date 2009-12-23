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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"

#include <fstream>
#include <iostream>
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbTerraSarImageMetadataInterface.h"

int otbTerraSarImageMetadataInterface (int argc, char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputFilename  = argv[2];

  typedef otb::VectorImage< double,  2 >                 InputImageType;
  typedef otb::ImageFileReader< InputImageType >         ImageReaderType;

  typedef std::vector<double>                   DoubleVectorType;
  typedef std::vector<DoubleVectorType>         DoubleVectorVectorType;
  typedef std::vector<unsigned int>             UIntVectorType;


  ImageReaderType::Pointer reader = ImageReaderType::New();
  reader->SetFileName( inputFilename  );
  reader->UpdateOutputInformation();

  otb::TerraSarImageMetadataInterface::Pointer lImageMetadata = otb::TerraSarImageMetadataInterface::New();

  lImageMetadata->GetNoise(reader->GetOutput()->GetMetaDataDictionary())->print(std::cout);
  std::cout<<lImageMetadata->GetNumberOfNoiseRecords(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  UIntVectorType deg = lImageMetadata->GetNoisePolynomialDegrees(reader->GetOutput()->GetMetaDataDictionary());
  DoubleVectorVectorType coeffs = lImageMetadata->GetNoisePolynomialCoefficientsList(reader->GetOutput()->GetMetaDataDictionary());

  DoubleVectorType mins = lImageMetadata->GetNoiseValidityRangeMinList(reader->GetOutput()->GetMetaDataDictionary());
  DoubleVectorType maxs = lImageMetadata->GetNoiseValidityRangeMaxList(reader->GetOutput()->GetMetaDataDictionary());
  DoubleVectorType ref = lImageMetadata->GetNoiseReferencePointList(reader->GetOutput()->GetMetaDataDictionary());
  DoubleVectorType time = lImageMetadata->GetNoiseTimeUTCList(reader->GetOutput()->GetMetaDataDictionary());

  for( unsigned int i=0; i<deg.size(); i++ )  
    {
      std::cout<<"~~~~~~~ Polynome "<<i<<" ( degree: "<<deg[i]<<")"<<std::endl;
      for( unsigned int j=0; j<coeffs[i].size(); j++ )  
	{
	  std::cout<<coeffs[i][j]<<"  ";
	}
      std::cout<<std::endl;
      std::cout<<mins[i]<<std::endl;
      std::cout<<maxs[i]<<std::endl;
      std::cout<<ref[i]<<std::endl;
      std::cout<<time[i]<<std::endl;
    }
  std::cout<<lImageMetadata->GetRadarFrequency(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;

  std::ofstream file;
  file.open(outputFilename);
  file<<"GetSensorID:          "<<lImageMetadata->GetSensorID(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  file<<"GetMinute:            "<<lImageMetadata->GetMinute(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  file<<"GetHour:              "<<lImageMetadata->GetHour(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  file<<"GetDay:               "<<lImageMetadata->GetDay(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  file<<"GetMonth:             "<<lImageMetadata->GetMonth(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  file<<"GetYear:              "<<lImageMetadata->GetYear(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  file<<"GetProductionDay:     "<<lImageMetadata->GetProductionDay(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  file<<"GetProductionMonth:   "<<lImageMetadata->GetProductionMonth(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  file<<"GetProductionYear:    "<<lImageMetadata->GetProductionYear(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  file<<"GetCalibrationFactor: "<<lImageMetadata->GetCalibrationFactor(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  file.close();

  return EXIT_SUCCESS;

}
