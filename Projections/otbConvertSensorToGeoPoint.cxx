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

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif


#include <iostream>
#include <iomanip>

#include "otbCommandLineArgumentParser.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbForwardSensorModel.h"
#include "otbMacro.h"

#include "itkExceptionObject.h"

#include "init/ossimInit.h"


int main(int argc, char* argv[])
{
  try
  {
    ossimInit::instance()->initialize(argc, argv);

    // Parse command line parameters
    typedef otb::CommandLineArgumentParser ParserType;
    ParserType::Pointer parser = ParserType::New();

    parser->SetProgramDescription("Sensor to geographic coordinates conversion. "
        "If x,y are not provided, convert the four corners");
    parser->AddOption("--data","sensor image","-in");
    parser->AddOption("--sample","X value of desired point","-x", 1, false);
    parser->AddOption("--line","Y value of desired point","-y", 1, false);

    typedef otb::CommandLineArgumentParseResult ParserResultType;
    ParserResultType::Pointer  parseResult = ParserResultType::New();

    try
    {
      parser->ParseCommandLine(argc,argv,parseResult);
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

    std::string filename = parseResult->GetParameterString("--data");

    typedef otb::VectorImage<double, 2> ImageType;
    typedef otb::ImageFileReader<ImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(filename);
    reader->GenerateOutputInformation();

    typedef otb::ForwardSensorModel<double> ModelType;
    ModelType::Pointer model = ModelType::New();
    model->SetImageGeometry(reader->GetOutput()->GetImageKeywordlist());

    ImageType::Pointer inputImage = reader->GetOutput();
    if (!model)
      {
      std::cerr << "Unable to create a model" << std::endl;
      return 1;
      }

    typedef itk::Point<double, 2> PointType;
    std::vector<PointType> points;
    if (parseResult->IsOptionPresent("--sample") && parseResult->IsOptionPresent("--line"))
      {
      PointType point;
      point[0] = parseResult->GetParameterInt("--sample");
      point[1] = parseResult->GetParameterInt("--line");
      points.push_back(point);
      }
    else
      {
      // find the four corners
      PointType point;
      point[0] = reader->GetOutput()->GetLargestPossibleRegion().GetIndex()[0];
      point[1] = reader->GetOutput()->GetLargestPossibleRegion().GetIndex()[1];
      points.push_back(point);

      point[0] = reader->GetOutput()->GetLargestPossibleRegion().GetIndex()[0]
               + reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0]-1;
      point[1] = reader->GetOutput()->GetLargestPossibleRegion().GetIndex()[1];
      points.push_back(point);

      point[0] = reader->GetOutput()->GetLargestPossibleRegion().GetIndex()[0];
      point[1] = reader->GetOutput()->GetLargestPossibleRegion().GetIndex()[1]
               + reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1]-1;
      points.push_back(point);

      point[0] = reader->GetOutput()->GetLargestPossibleRegion().GetIndex()[0]
               + reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0]-1;
      point[1] = reader->GetOutput()->GetLargestPossibleRegion().GetIndex()[1]
               + reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1]-1;
      points.push_back(point);

      }

    for (unsigned int i = 0; i < points.size(); ++i)
      {


      ModelType::OutputPointType outputPoint;


      outputPoint = model->TransformPoint(points[i]);

      if (!parseResult->IsOptionPresent("--OTBTesting"))
        {
        std::cout << std::setprecision(10) << "Sensor Point  (x , y)  : (" << points[i][0] << ", " << points[i][1]
            << ")\n";
        std::cout << std::setprecision(10) << "Geographic Point (Lat, Lon) : (" << outputPoint[1] << ", "
            << outputPoint[0] << ")\n\n";
        }
      else
        {
        std::string outputTestFileName = parseResult->GetParameterString("--OTBTesting", 0);

        ofstream outputTestFile;
        outputTestFile.open(outputTestFileName.c_str());

        outputTestFile << std::setprecision(10) << "Sensor Point  (x , y)  : (" << points[i][0] << ", "
            << points[i][1] << ")\n";
        outputTestFile << std::setprecision(10) << "Geographic Point (Lat, Lon) : (" << outputPoint[1] << ", "
            << outputPoint[0] << ")\n\n";
        outputTestFile.close();
        }
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

