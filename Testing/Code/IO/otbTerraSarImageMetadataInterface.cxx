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

  typedef itk::ImageBase< 2 >                   ImageType;
  typedef ImageType::IndexType                  IndexType;
  typedef std::vector<IndexType>                IndexVectorType;
  

  ImageReaderType::Pointer reader = ImageReaderType::New();
  reader->SetFileName( inputFilename  );
  reader->UpdateOutputInformation();

  otb::TerraSarImageMetadataInterface::Pointer lImageMetadata = otb::TerraSarImageMetadataInterface::New();

  std::ofstream file;
  file.open(outputFilename);
  file<<"GetSensorID:             "<<lImageMetadata->GetSensorID(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  file<<"GetMinute:               "<<lImageMetadata->GetMinute(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  file<<"GetHour:                 "<<lImageMetadata->GetHour(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  file<<"GetDay:                  "<<lImageMetadata->GetDay(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  file<<"GetMonth:                "<<lImageMetadata->GetMonth(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  file<<"GetYear:                 "<<lImageMetadata->GetYear(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  file<<"GetProductionDay:        "<<lImageMetadata->GetProductionDay(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  file<<"GetProductionMonth:      "<<lImageMetadata->GetProductionMonth(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  file<<"GetProductionYear:       "<<lImageMetadata->GetProductionYear(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  file<<"GetCalibrationFactor:    "<<lImageMetadata->GetCalibrationFactor(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  file<<"GetRadarFrequency:       "<<lImageMetadata->GetRadarFrequency(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  file<<"GetPRF:                  "<<lImageMetadata->GetPRF(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  file<<std::endl;
  file<<"Noise attributs: "<<std::endl;
  UIntVectorType         deg    = lImageMetadata->GetNoisePolynomialDegrees(reader->GetOutput()->GetMetaDataDictionary());
  DoubleVectorVectorType coeffs = lImageMetadata->GetNoisePolynomialCoefficientsList(reader->GetOutput()->GetMetaDataDictionary());
  DoubleVectorType       mins   = lImageMetadata->GetNoiseValidityRangeMinList(reader->GetOutput()->GetMetaDataDictionary());
  DoubleVectorType       maxs   = lImageMetadata->GetNoiseValidityRangeMaxList(reader->GetOutput()->GetMetaDataDictionary());
  DoubleVectorType       ref    = lImageMetadata->GetNoiseReferencePointList(reader->GetOutput()->GetMetaDataDictionary());
  DoubleVectorType       time   = lImageMetadata->GetNoiseTimeUTCList(reader->GetOutput()->GetMetaDataDictionary());
  file<<"GetNumberOfNoiseRecords: "<< lImageMetadata->GetNumberOfNoiseRecords(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  for( unsigned int i=0; i<deg.size(); i++ )  
    {
      file<<"Noise Polynom "<<i<<" ( degree: "<<deg[i]<<")"<<std::endl;
      file<<"coefficients: ";
      for( unsigned int j=0; j<coeffs[i].size(); j++ )  
       {
         file<<coeffs[i][j]<<"  ";
       }
      file<<std::endl;
      file<<"Min validity range: "<<mins[i]<<std::endl;
      file<<"Min validity range: "<<maxs[i]<<std::endl;
      file<<"Reference point:    "<<ref[i]<<std::endl;
      file<<"Time UTC:           "<<time[i]<<std::endl;
    }
  file<<"Incidence Angles attributs: "<<std::endl;
  IndexType centerIndexType = lImageMetadata->GetCenterIncidenceAngleIndex(reader->GetOutput()->GetMetaDataDictionary());
  file<<"GetCenterIncidenceAngle: Value: "<<lImageMetadata->GetCenterIncidenceAngle(reader->GetOutput()->GetMetaDataDictionary());
  file<<" Index: Row: "<<centerIndexType[0]<<" Column: "<<centerIndexType[1]<<std::endl;
  file<<"GetNumberOfCornerIncidenceAngles: "<<lImageMetadata->GetNumberOfCornerIncidenceAngles(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  DoubleVectorType cornerIncidenceAngles = lImageMetadata->GetCornersIncidenceAngles(reader->GetOutput()->GetMetaDataDictionary());
  std::vector<IndexType> tabIndex = lImageMetadata->GetCornersIncidenceAnglesIndex(reader->GetOutput()->GetMetaDataDictionary());
  file<<"GetCornerIncidenceAngles: "<<std::endl;
  for( unsigned int i=0; i<cornerIncidenceAngles.size(); i++)
  {
    file<<"Incidence Angle "<<i<<": Value: "<<cornerIncidenceAngles[i]<<" Index: Row: "<<tabIndex[i][0]<<" Column: "<<tabIndex[i][1]<<std::endl;
  }
  file<<"GetMeanIncidenceAngles: "<<lImageMetadata->GetMeanIncidenceAngles(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  file.close();

  return EXIT_SUCCESS;

}
