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
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbMapProjections.h"
#include "otbOrthoRectificationFilter.h"
#include "otbImage.h"
#include "otbMacro.h"

#include "itkExceptionObject.h"
#include "itkMacro.h"
#include "itkTransform.h"

#include "init/ossimInit.h"


template<typename TMapProjection>
int generic_main_carto_geo(TMapProjection* mapProjection, otb::CommandLineArgumentParseResult* parseResult) 
{

  try 
  { 
		
    typedef TMapProjection MapProjectionType;
				
    typename MapProjectionType::InputPointType cartoPoint;
    typename MapProjectionType::OutputPointType geoPoint;
  							
    cartoPoint[0]=parseResult->GetParameterDouble("--XCarto");
    cartoPoint[1]=parseResult->GetParameterDouble("--YCarto");
				
    geoPoint = mapProjection->TransformPoint(cartoPoint);

    if(!parseResult->IsOptionPresent("--OTBTesting"))
    {
      std::cout << std::setprecision(10) << "Cartographic Point  (x , y)  : (" << cartoPoint[0] << "," << cartoPoint[1] << ")" << std::endl;
      std::cout << std::setprecision(10) << "Geographic   Point (Lat,Lon) : (" << geoPoint[1] << "," <<	geoPoint[0] << ")" << std::endl;
    }
    else
    {
      std::string outputTestFileName = parseResult->GetParameterString("--OTBTesting",0);
					
      ofstream outputTestFile;
      outputTestFile.open(outputTestFileName.c_str());
					
      outputTestFile << std::setprecision(10) << "Cartographic Point  (x , y)  : (" << cartoPoint[0] << "," << cartoPoint[1] << ")" << std::endl;
      outputTestFile << std::setprecision(10) << "Geographic   Point (Lat,Lon) : (" << geoPoint[1] << "," <<	geoPoint[0] << ")" << std::endl;
					
      outputTestFile.close();
    }

							
  }
  catch( itk::ExceptionObject & err ) 
  { 
    std::cout << "Exception itk::ExceptionObject raised !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
  } 
  catch( std::bad_alloc & err ) 
  { 
    std::cout << "Exception bad_alloc : "<<(char*)err.what()<< std::endl; 
    return EXIT_FAILURE;
  } 
  catch( ... ) 
  { 
    std::cout << "Unknown exception raised !" << std::endl; 
    return EXIT_FAILURE;
  } 
  return EXIT_SUCCESS;

}//Fin main()
				
			


int main(int argc, char* argv[]) 
{
  try
  { 
    ossimInit::instance()->initialize(argc, argv);
				
				// Parse command line parameters
    typedef otb::CommandLineArgumentParser ParserType;	
    ParserType::Pointer parser = ParserType::New();

    parser->SetProgramDescription("Cartographic to geographic coordinates conversion");
    parser->AddOption("--XCarto","X cartographic value of desired point","-x");
    parser->AddOption("--YCarto","Y cartographic value of desired point","-y");
    parser->AddOptionNParams("--MapProjectionType","Type (UTM/LAMBERT/LAMBERT2/LAMBERT93/SINUS/ECKERT4/TRANSMERCATOR/MOLLWEID/SVY21) and parameters of map projection used","-mapProj");				

    typedef otb::CommandLineArgumentParseResult ParserResultType;
    ParserResultType::Pointer  parseResult = ParserResultType::New();
				
    try
    {
      parser->ParseCommandLine(argc,argv,parseResult);
    }
    catch( itk::ExceptionObject & err ) 
    { 
      std::string descriptionException = err.GetDescription();   
      if(descriptionException.find("ParseCommandLine(): Help Parser") != std::string::npos) 
      {
        std::cout << "WARNING : output file pixels are converted in 'unsigned char'" << std::endl;
        return EXIT_SUCCESS;
      }
      if(descriptionException.find("ParseCommandLine(): Version Parser") != std::string::npos) 
      {
        return EXIT_SUCCESS;
      }
      return EXIT_FAILURE;
    }	
				
				// Code
					
    std::string typeMap = parseResult->GetParameterString("--MapProjectionType",0);
    int nbParams = parseResult->GetNumberOfParameters("--MapProjectionType");
    nbParams--;
					
    if ((typeMap == "UTM")&&(nbParams==2))
    {
      int numZone = parseResult->GetParameterUInt("--MapProjectionType",1);
      char hemisphere = parseResult->GetParameterChar("--MapProjectionType",2);
						
      typedef otb::UtmInverseProjection UtmProjectionType;
      UtmProjectionType::Pointer utmProjection = UtmProjectionType::New();
						
      utmProjection->SetZone(numZone);
      utmProjection->SetHemisphere(hemisphere);
					
      return generic_main_carto_geo<UtmProjectionType>(utmProjection, parseResult);
    }
    else
    {
      std::vector<double> parameters ;
					
      for (int i=1; i<nbParams+1; i++)
      {
        parameters.push_back(parseResult->GetParameterDouble("--MapProjectionType",i));
      }
						
      if ((typeMap == "LAMBERT")&&(nbParams==4))
      {
        typedef otb::LambertConformalConicInverseProjection LambertProjectionType;
        LambertProjectionType::Pointer lambertProjection = LambertProjectionType::New();
						
        lambertProjection->SetParameters(parameters[0],parameters[1],parameters[2],parameters[3]);
					
        return generic_main_carto_geo<LambertProjectionType>(lambertProjection, parseResult);
      }
      else if ((typeMap == "LAMBERT2")&&(nbParams==0))
      {
        typedef otb::Lambert2EtenduInverseProjection Lambert2ProjectionType;
        Lambert2ProjectionType::Pointer lambert2Projection = Lambert2ProjectionType::New();
					
        return generic_main_carto_geo<Lambert2ProjectionType>(lambert2Projection, parseResult);
      }
      else if ((typeMap == "LAMBERT93")&&(nbParams==0))
      {
        typedef otb::Lambert93InverseProjection Lambert93ProjectionType;
        Lambert93ProjectionType::Pointer lambert93Projection = Lambert93ProjectionType::New();
					
        return generic_main_carto_geo<Lambert93ProjectionType>(lambert93Projection, parseResult);
      }
      else if ((typeMap == "SINUS")&&(nbParams==2))
      {
        typedef otb::SinusoidalInverseProjection SinusoidalProjectionType;
        SinusoidalProjectionType::Pointer sinusoidalProjection = SinusoidalProjectionType::New();
						
        sinusoidalProjection->SetParameters(parameters[0],parameters[1]);
					
        return generic_main_carto_geo<SinusoidalProjectionType>(sinusoidalProjection, parseResult);
      }
      else if ((typeMap == "ECKERT4")&&(nbParams==2))
      {
        typedef otb::Eckert4InverseProjection Eckert4ProjectionType;
        Eckert4ProjectionType::Pointer eckert4Projection = Eckert4ProjectionType::New();
						
        eckert4Projection->SetParameters(parameters[0],parameters[1]);
					
        return generic_main_carto_geo<Eckert4ProjectionType>(eckert4Projection, parseResult);
      }
      else if ((typeMap == "TRANSMERCATOR")&&(nbParams==3))
      {
        typedef otb::TransMercatorInverseProjection TransMercatorProjectionType;
        TransMercatorProjectionType::Pointer transMercatorProjection = TransMercatorProjectionType::New();
						
        transMercatorProjection->SetParameters(parameters[0],parameters[1],parameters[2]);
					
        return generic_main_carto_geo<TransMercatorProjectionType>(transMercatorProjection, parseResult);
      }
      else if ((typeMap == "MOLLWEID")&&(nbParams==2))
      {
        typedef otb::MollweidInverseProjection MollweidProjectionType;
        MollweidProjectionType::Pointer mollweidProjection = MollweidProjectionType::New();
						
        mollweidProjection->SetParameters(parameters[0],parameters[1]);
					
        return generic_main_carto_geo<MollweidProjectionType>(mollweidProjection, parseResult);
      }
      else if ((typeMap == "SVY21")&&(nbParams==0))
      {
        typedef otb::SVY21InverseProjection SVY21ProjectionType;
        SVY21ProjectionType::Pointer svy21Projection = SVY21ProjectionType::New();
					
        return generic_main_carto_geo<SVY21ProjectionType>(svy21Projection, parseResult);
      }
      else 
      {
        itkGenericExceptionMacro(<< "TypeMap not recognized, choose one with (parameters) : UTM(2), LAMBERT(4), LAMBERT2(0), LAMBERT2(93), SINUS(2), ECKERT4(2), TRANSMERCATOR(3), MOLLWEID(2), SVY21(0)");
      }
					
    }


  }
  catch( itk::ExceptionObject & err ) 
  { 
    std::cout << "Exception itk::ExceptionObject raised !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
  } 
  catch( std::bad_alloc & err ) 
  { 
    std::cout << "Exception bad_alloc : "<<(char*)err.what()<< std::endl; 
    return EXIT_FAILURE;
  } 
  catch( ... ) 
  { 
    std::cout << "Unknown exception raised !" << std::endl; 
    return EXIT_FAILURE;
  } 
  return EXIT_SUCCESS;
}				
		
