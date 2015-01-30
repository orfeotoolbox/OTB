/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights keyerved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include "otbImageFileReader.h"
#include "otbVectorImage.h"
#include "itkMacro.h"
#include <iostream>
#include "otbConfigurationFile.h"
#include <cctype> // for toupper
#include <string>
#include <algorithm>

int otbImageFileReaderTestSensorPixelValue(int argc, char* argv[])
{
  if (argc < 9)
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr <<
    " inputFilename baselineFilename sensorType sensorName sensorMode nbBands indexX indexY"
              << std::endl;
    return 1;
    }
  // Verify the number of parameters in the command line
  const char *      inputFilename    = argv[1];
  const std::string baselineFilename    = argv[2];
  const std::string sensorType    = argv[3];
  const std::string sensorName    = argv[4];
  const std::string sensorMode    = argv[5];

  const unsigned int nbBands    = atoi(argv[6]);
  const unsigned int indexX    = atoi(argv[7]);
  const unsigned int indexY    = atoi(argv[8]);

  typedef double InputPixelType;
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<InputPixelType,  Dimension> ImageType;

  typedef otb::ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(inputFilename);

  reader->UpdateOutputInformation();
  ConfigFile * pixelValuesConfig = NULL;

  try
    {
    pixelValuesConfig = new ConfigFile(baselineFilename);
    }
  catch (ConfigFile::file_not_found& e)
    {
    itkGenericExceptionMacro(<< "Error - File '" << e.filename << "' not found.");
    }
  std::string paramName;

  std::string key;
  key += sensorType;
  key += "_";
  key += sensorName;
  key += "_";
  key += sensorMode;
  key += "_";

  std::stringstream out;
  out << nbBands << "_";
  out << indexX <<  "_";
  out << indexY;
  key += out.str();
  std::transform(key.begin(), key.end(), key.begin(), (int (*)(int))toupper);

  otbGenericMsgDebugMacro(<< "key " << key);

  double baselinePixelValue = 0.;
  try
    {
    baselinePixelValue = pixelValuesConfig->read<double>(key);
    otbGenericMsgDebugMacro(<< "Pixel value in baseline " << baselinePixelValue);
    }
  catch (ConfigFile::key_not_found& e)
    {
    itkGenericExceptionMacro(<< "Error - Key '" << e.key << "' not found.");
    }

  ImageType::IndexType index;
  index[0] = indexX;
  index[1] = indexY;

  ImageType::SizeType size;
  size[0] = 1;
  size[1] = 1;

  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(index);

  reader->GetOutput()->UpdateOutputInformation();
  reader->GetOutput()->SetRequestedRegion(region);
  reader->GetOutput()->PropagateRequestedRegion();
  reader->GetOutput()->UpdateOutputData();

  ImageType::PixelType            pixelValue = reader->GetOutput()->GetPixel(index);
  ImageType::PixelType::ValueType imagePixelValue;
  imagePixelValue = pixelValue[nbBands - 1];

  otbGenericMsgDebugMacro(<< "Pixel value in image " << imagePixelValue);

  double epsilon = 0.0000000001;
  double error = vcl_abs(imagePixelValue - baselinePixelValue);

  if(pixelValuesConfig != NULL)
    {
    delete pixelValuesConfig;
    pixelValuesConfig = NULL;
    }

  if (error < epsilon)
    {
    return EXIT_SUCCESS;
    }
  else
    {
    return EXIT_FAILURE;
    }
}
