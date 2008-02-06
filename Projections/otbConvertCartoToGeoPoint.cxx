/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - R. Garrigues
  Language  :   C++
  Date      :   18 Octobre 2007
  Version   :   
  Role      :   Ortho-Rectification Image Application
  $Id$

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
int generic_main_carto_geo(int argc, char* argv[], TMapProjection* mapProjection, otb::CommandLineArgumentParseResult* parseResult) 
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
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
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
    std::cout << "Exception levee inconnue !" << std::endl; 
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

        parser->AddOption("--XCarto","X cartographic value of desired point","-x");
				parser->AddOption("--YCarto","Y cartographic value of desired point","-y");
				parser->AddOptionNParams("--MapProjectionType","Type (UTM/LAMBERT/SINUS/ECKERT4/TRANSMERCATOR/MOLLWEID) and parameters of map projection used","-mapProj");				

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
					
					return generic_main_carto_geo<UtmProjectionType>(argc,argv, utmProjection, parseResult);
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
					
						return generic_main_carto_geo<LambertProjectionType>(argc,argv, lambertProjection, parseResult);
					}
					else if ((typeMap == "SINUS")&&(nbParams==2))
					{
						typedef otb::SinusoidalInverseProjection SinusoidalProjectionType;
						SinusoidalProjectionType::Pointer sinusoidalProjection = SinusoidalProjectionType::New();
						
						sinusoidalProjection->SetParameters(parameters[0],parameters[1]);
					
						return generic_main_carto_geo<SinusoidalProjectionType>(argc,argv, sinusoidalProjection, parseResult);
					}
					else if ((typeMap == "ECKERT4")&&(nbParams==2))
					{
						typedef otb::Eckert4InverseProjection Eckert4ProjectionType;
						Eckert4ProjectionType::Pointer eckert4Projection = Eckert4ProjectionType::New();
						
						eckert4Projection->SetParameters(parameters[0],parameters[1]);
					
						return generic_main_carto_geo<Eckert4ProjectionType>(argc,argv, eckert4Projection, parseResult);
					}
					else if ((typeMap == "TRANSMERCATOR")&&(nbParams==3))
					{
						typedef otb::TransMercatorInverseProjection TransMercatorProjectionType;
						TransMercatorProjectionType::Pointer transMercatorProjection = TransMercatorProjectionType::New();
						
						transMercatorProjection->SetParameters(parameters[0],parameters[1],parameters[2]);
					
						return generic_main_carto_geo<TransMercatorProjectionType>(argc,argv, transMercatorProjection, parseResult);
					}
					else if ((typeMap == "MOLLWEID")&&(nbParams==2))
					{
						typedef otb::MollweidInverseProjection MollweidProjectionType;
						MollweidProjectionType::Pointer mollweidProjection = MollweidProjectionType::New();
						
						mollweidProjection->SetParameters(parameters[0],parameters[1]);
					
						return generic_main_carto_geo<MollweidProjectionType>(argc,argv, mollweidProjection, parseResult);
					}
					else 
					{
						itkGenericExceptionMacro(<< "TypeMap not recognized, choose one with (parameters) : UTM(2), LAMBERT(4), SINUS(2), ECKERT4(2), TRANSMERCATOR(3), MOLLWEID(2)");
					}
					
				}


		}
		catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
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
    std::cout << "Exception levee inconnue !" << std::endl; 
    return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;
}				
		
