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


#include "otbImageToLuminanceImageFilter.h"
#include "otbLuminanceToReflectanceImageFilter.h"
#include "otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms.h"
#include "otbAtmosphericCorrectionParameters.h"
#include "otbAtmosphericRadiativeTerms.h"
#include "otbReflectanceToSurfaceReflectanceImageFilter.h"
#include "otbSurfaceAdjencyEffect6SCorrectionSchemeFilter.h"


#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include <string>

int otbAtmosphericCorrectionSequencementTest( int argc, char *argv[] )
{
  if( argc != 19 )
    {
      std::cerr << "Missing Parameters " << std::endl;
      std::cerr << "Usage: " << argv[0];
      std::cerr << "    inputImage , outputImage,"  << std::endl;
      std::cerr << "    alphaBetaFileName , solarZenithalAngle , day, month , solarIlluminationFileName,"  << std::endl;
      std::cerr << "    solarAzimutalAngle , viewingZenithalAngle , viewingAzimutalAngle,"  << std::endl;
      std::cerr << "    atmosphericPressure , waterVaporAmount , ozoneAmount , aerosolModel , AerosolOptical,"  << std::endl;
      std::cerr << "    wavelenghtSpectralBandFileName , adjencyEffect6SCorrectionWindowRadius, pixelSpacingInKilometers" << std::endl;
      std::cerr << std::endl;
      return 1;
    }

  const unsigned int                                    Dimension = 2;
  typedef double                                        PixelType;
  typedef otb::VectorImage<PixelType,Dimension>         ImageType;

  typedef otb::ImageFileReader<ImageType>   ReaderType;
  typedef otb::ImageFileWriter<ImageType>   WriterType;

  ReaderType::Pointer reader  = ReaderType::New();
  reader->SetFileName(argv[1]);
  // Software Guide : BeginLatex
  //  
  // The invocation of the \code{GenerateOutputInformation()} method on the reader
  // to known the number of component per pixel of the image.  It is recommended to 
  // place GenerateOutputInformation calls in a \code{try/catch} block in case 
  // errors occur and exceptions are thrown.
  //
  // Software Guide : EndLatex 
  
  // Software Guide : BeginCodeSnippet
  try
    {
      reader->GenerateOutputInformation();
    }
  catch( itk::ExceptionObject & excep )
    {
      std::cerr << "Exception caught !" << std::endl;
      std::cerr << excep << std::endl;
    }
  // Software Guide : EndCodeSnippet
  catch( ... ) 
    { 
      std::cout << "Unknown exception !" << std::endl; 
      return EXIT_FAILURE;
    } 

  unsigned int nbOfComponent = reader->GetOutput()->GetNumberOfComponentsPerPixel();


//-------------------------------

  typedef otb::ImageToLuminanceImageFilter<ImageType,ImageType> 
                                ImageToLuminanceImageFilterType;
  typedef ImageToLuminanceImageFilterType::VectorType VectorType; 

  VectorType alpha(nbOfComponent);
  VectorType beta(nbOfComponent);
  alpha.Fill(0);
  beta.Fill(0);
  
  std::ifstream fin;
  fin.open(argv[3]);
  double dalpha(0.), dbeta(0.);
  for( unsigned int i=0 ; i < nbOfComponent ; i++)
  {
        fin >> dalpha;
        fin >> dbeta;
        alpha[i] = dalpha;
        beta[i] = dbeta;
  }
  fin.close();

  ImageToLuminanceImageFilterType::Pointer filterImageToLuminance = ImageToLuminanceImageFilterType::New();
  filterImageToLuminance->SetAlpha(alpha);
  filterImageToLuminance->SetBeta(beta);
  filterImageToLuminance->SetInput(reader->GetOutput());


//-------------------------------
  typedef otb::LuminanceToReflectanceImageFilter<ImageType,ImageType> 
                                LuminanceToReflectanceImageFilterType;

  typedef LuminanceToReflectanceImageFilterType::VectorType VectorType; 

  VectorType solarIllumination(nbOfComponent);
  solarIllumination.Fill(0);
  
  fin.open(argv[7]);
  double dsolarIllumination(0.);
  for( unsigned int i=0 ; i < nbOfComponent ; i++)
  {
        fin >> dsolarIllumination;
        solarIllumination[i] = dsolarIllumination;
  }
  fin.close();
  
  
  LuminanceToReflectanceImageFilterType::Pointer filterLuminanceToReflectance = LuminanceToReflectanceImageFilterType::New();
  const int day(atoi(argv[5]));
  const int month(atoi(argv[6]));
  
  filterLuminanceToReflectance->SetZenithalSolarAngle(static_cast<double>(atof(argv[4])));
  filterLuminanceToReflectance->SetDay(day);
  filterLuminanceToReflectance->SetMonth(month); 
  filterLuminanceToReflectance->SetSolarIllumination(solarIllumination);
  filterLuminanceToReflectance->SetInput(filterImageToLuminance->GetOutput());

//-------------------------------
  typedef otb::AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms     
                                AtmosphericCorrectionParametersTo6SRadiativeTermsType;
  typedef otb::AtmosphericCorrectionParameters                                  
                                AtmosphericCorrectionParametersType;
  typedef otb::AtmosphericRadiativeTerms                                        
                                AtmosphericRadiativeTermsType;
  typedef AtmosphericCorrectionParametersType::AerosolModelType                 
                                AerosolModelType;

  typedef otb::FilterFunctionValues                                          
                                FilterFunctionValuesType;
  typedef FilterFunctionValuesType::ValuesVectorType                         
                                ValuesVectorType;


  AtmosphericCorrectionParametersType::Pointer dataAtmosphericCorrectionParameters = AtmosphericCorrectionParametersType::New();
  AtmosphericRadiativeTermsType::Pointer  dataAtmosphericRadiativeTerms = AtmosphericRadiativeTermsType::New();

  float minSpectralValue(0.);
  float maxSpectralValue(0.);
  float userStep(0.);
  float value(0.);

  unsigned int nbBands(0);
  unsigned int nbValuesPerBand(0);
  std::string sString;
  ValuesVectorType vector;
  
  fin.open(argv[16]);
  fin >> nbBands;
  for(unsigned int i=0 ; i<nbBands ; i++)
  {
        vector.clear();
        fin >> sString;
        fin >> minSpectralValue;
        fin >> maxSpectralValue;
        fin >> userStep;
        fin >> nbValuesPerBand;
        for( unsigned int j=0 ; j < nbValuesPerBand ; j++)
        {
                fin >> value;
                vector.push_back(value);
        }
        FilterFunctionValuesType::Pointer functionValues = FilterFunctionValuesType::New();
        functionValues->SetFilterFunctionValues(vector);
        functionValues->SetMinSpectralValue(minSpectralValue);
        functionValues->SetMaxSpectralValue(maxSpectralValue);
        functionValues->SetUserStep( userStep );
        dataAtmosphericCorrectionParameters->SetWavelenghtSpectralBandWithIndex(i, functionValues);
  }

  fin.close();

  
  // Set parameters
  dataAtmosphericCorrectionParameters->SetSolarZenithalAngle(filterLuminanceToReflectance->GetZenithalSolarAngle());
  dataAtmosphericCorrectionParameters->SetSolarAzimutalAngle(static_cast<double>(atof(argv[8])));
  dataAtmosphericCorrectionParameters->SetViewingZenithalAngle(static_cast<double>(atof(argv[9])));
  dataAtmosphericCorrectionParameters->SetViewingAzimutalAngle(static_cast<double>(atof(argv[10])));
  dataAtmosphericCorrectionParameters->SetMonth(month);
  dataAtmosphericCorrectionParameters->SetDay(day);
  dataAtmosphericCorrectionParameters->SetAtmosphericPressure(static_cast<double>(atof(argv[11]))); 
  dataAtmosphericCorrectionParameters->SetWaterVaporAmount(static_cast<double>(atof(argv[12])));
  dataAtmosphericCorrectionParameters->SetOzoneAmount(static_cast<double>(atof(argv[13])));
  AerosolModelType aerosolModel = static_cast<AerosolModelType>(::atoi(argv[14]));
  dataAtmosphericCorrectionParameters->SetAerosolModel(aerosolModel);
  dataAtmosphericCorrectionParameters->SetAerosolOptical(static_cast<double>(atof(argv[15])));

  AtmosphericCorrectionParametersTo6SRadiativeTermsType::Pointer  filterAtmosphericCorrectionParametersTo6SRadiativeTerms = AtmosphericCorrectionParametersTo6SRadiativeTermsType::New();
  filterAtmosphericCorrectionParametersTo6SRadiativeTerms->SetInput( dataAtmosphericCorrectionParameters );


//-------------------------------

  typedef otb::ReflectanceToSurfaceReflectanceImageFilter<ImageType, ImageType> ReflectanceToSurfaceReflectanceImageFilterType;
  ReflectanceToSurfaceReflectanceImageFilterType::Pointer  filterReflectanceToSurfaceReflectanceImageFilter = ReflectanceToSurfaceReflectanceImageFilterType::New();
  
  filterReflectanceToSurfaceReflectanceImageFilter->SetAtmosphericRadiativeTerms( filterAtmosphericCorrectionParametersTo6SRadiativeTerms->GetOutput() );
  filterReflectanceToSurfaceReflectanceImageFilter->SetInput(filterLuminanceToReflectance->GetOutput());

//-------------------------------
  typedef otb::SurfaceAdjencyEffect6SCorrectionSchemeFilter<ImageType, ImageType> SurfaceAdjencyEffect6SCorrectionSchemeFilterType;
  SurfaceAdjencyEffect6SCorrectionSchemeFilterType::Pointer  filterSurfaceAdjencyEffect6SCorrectionSchemeFilter = SurfaceAdjencyEffect6SCorrectionSchemeFilterType::New();

  filterSurfaceAdjencyEffect6SCorrectionSchemeFilter->SetAtmosphericRadiativeTerms(filterAtmosphericCorrectionParametersTo6SRadiativeTerms->GetOutput());
  filterSurfaceAdjencyEffect6SCorrectionSchemeFilter->SetWindowRadius(atoi(argv[17]));
  filterSurfaceAdjencyEffect6SCorrectionSchemeFilter->SetPixelSpacingInKilometers(static_cast<double>(atof(argv[18])));
  filterSurfaceAdjencyEffect6SCorrectionSchemeFilter->SetZenithalViewingAngle(dataAtmosphericCorrectionParameters->GetViewingZenithalAngle());
  filterSurfaceAdjencyEffect6SCorrectionSchemeFilter->SetInput(filterReflectanceToSurfaceReflectanceImageFilter->GetOutput());

//-------------------------------
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(argv[2]);
  writer->SetInput(filterSurfaceAdjencyEffect6SCorrectionSchemeFilter->GetOutput());
  writer->Update();

    
  return EXIT_SUCCESS;
}

