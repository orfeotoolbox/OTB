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

#include "otbImageFileReader.h"

//#include "otbVectorDataExtractROI.h"
#include <fstream>
#include <iostream>

#include "otbImage.h"
#include "otbLabelMapToGISTableFilter.h"
#include "otbAttributesMapLabelObject.h"
#include "itkLabelImageToLabelMapFilter.h"
#include "otbPostGISConnectionImplementation.h"
#include "otbPostGISTable.h"

int otbLabelMapToGISTableFilter(int argc, char * argv[])
{

  if (argc != 6)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputLabelImageFile dbName tableName userName userPassword" << std::endl;
    return EXIT_FAILURE;
    }

  const char *      infname = argv[1];
  const std::string dbName = argv[2];
  const std::string tableName = argv[3];
  const std::string userName = argv[4];
  const std::string userPassword = argv[5];

  // Labeled image type
  const unsigned int Dimension                 = 2;
  typedef unsigned short                         LabelType;
  typedef otb::Image<LabelType, Dimension>       LabeledImageType;
  typedef otb::ImageFileReader<LabeledImageType> LabeledReaderType;

  // Label map typedef
  typedef otb::AttributesMapLabelObject<LabelType, Dimension, double>             LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>                                          LabelMapType;
  typedef itk::LabelImageToLabelMapFilter<LabeledImageType, LabelMapType>         LabelMapFilterType;
  typedef otb::Polygon<double>                                                    PolygonType;
  typedef otb::Functor::LabelObjectToPolygonFunctor<LabelObjectType, PolygonType> FunctorType;

  typedef otb::PostGISConnectionImplementation          DbConnection;
  typedef otb::PostGISConnectionImplementation::Pointer DbConnectionPointer;

  DbConnectionPointer myConnection = DbConnection::New();
  myConnection->SetHost("localhost");
  myConnection->SetDBName(dbName);
  myConnection->SetUser(userName);
  myConnection->SetPassword(userPassword);

  LabeledReaderType::Pointer lreader = LabeledReaderType::New();
  lreader->SetFileName(infname);

  LabelMapFilterType::Pointer labelMapFilter = LabelMapFilterType::New();
  labelMapFilter->SetInput(lreader->GetOutput());
  labelMapFilter->SetBackgroundValue(itk::NumericTraits<LabelType>::max());
  labelMapFilter->Update();

  typedef otb::PostGISTable<DbConnection, double, 2>                    PostGISTableType;
  typedef otb::LabelMapToGISTableFilter<LabelMapType, PostGISTableType> LabelMapToGISTableFilterType;

  LabelMapToGISTableFilterType::Pointer myFilter = LabelMapToGISTableFilterType::New();

  myFilter->SetInput(labelMapFilter->GetOutput());

  myFilter->SetInputGISConnection(myConnection);
  myFilter->SetGISTableName(tableName);
  myFilter->SetDropExistingGISTable(true);

  myFilter->Update();

  return EXIT_SUCCESS;

}
