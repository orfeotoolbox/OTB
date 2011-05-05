/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE,  See the above copyright notices for more information.

=========================================================================*/

#include <iostream>
#include <fstream>

#include "otbCommandLineArgumentParser.h"
#include "otbMapProjections.h"
#include "otbImage.h"

#include "itkExceptionObject.h"
#include "itkMacro.h"

#include "otbUtils.h"

int main(int argc, char* argv[])
{
  try
  {
    // Parse command line parameters
    typedef otb::CommandLineArgumentParser ParserType;
    ParserType::Pointer parser = ParserType::New();

    parser->SetProgramDescription("UTM zone determination from a geographic point");
    parser->AddOption("--Latitude","Latitude value of desired point","-lat");
    parser->AddOption("--Longitude","Longitude value of desired point","-lon");

    typedef otb::CommandLineArgumentParseResult ParserResultType;
    ParserResultType::Pointer  parseResult = ParserResultType::New();

    try
    {
      parser->ParseCommandLine(argc, argv, parseResult);
    }
    catch ( itk::ExceptionObject & err )
    {
      std::string descriptionException = err.GetDescription();
      if (descriptionException.find("ParseCommandLine(): Help Parser") != std::string::npos)
      {
        std::cout << "WARNING : output file pixels are converted in 'unsigned char'" << std::endl;
        return EXIT_SUCCESS;
      }
      if (descriptionException.find("ParseCommandLine(): Version Parser") != std::string::npos)
      {
        return EXIT_SUCCESS;
      }
      return EXIT_FAILURE;
    }

    // Code
    double lon = parseResult->GetParameterDouble("--Longitude");
    double lat =parseResult->GetParameterDouble("--Latitude");

    int utmZone = otb::Utils::GetZoneFromGeoPoint(lon, lat);

    if (!parseResult->IsOptionPresent("--OTBTesting"))
    {
      std::cout << "Geographic   Point (Lat,Lon) : (" << lat << "," << lon << ")" << std::endl;
      std::cout << "UTM Corresponding Zone       : ==> " << utmZone << " <=="  << std::endl;
    }
    else
    {
      std::string outputTestFileName = parseResult->GetParameterString("--OTBTesting", 0);

      std::ofstream outputTestFile;
      outputTestFile.open(outputTestFileName.c_str());

      outputTestFile << "Geographic   Point (Lat,Lon) : (" << lat << "," << lon << ")" << std::endl;
      outputTestFile << "UTM Corresponding Zone       : ==> " << utmZone << " <=="  << std::endl;

      outputTestFile.close();
    }


  }
  catch ( itk::ExceptionObject & err )
  {
    std::cout << "Exception itk::ExceptionObject raised !" << std::endl;
    std::cout << err << std::endl;
    return EXIT_FAILURE;
  }
  catch ( std::bad_alloc & err )
  {
    std::cout << "Exception bad_alloc : "<<(char*)err.what()<< std::endl;
    return EXIT_FAILURE;
  }
  catch ( ... )
  {
    std::cout << "Unknown exception raised !" << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

