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
//  Software Guide : BeginCommandLineArgs
//  INPUTS: {Romania_Extract.tif}, {atmosphericCorrectionSequencement_alpha_beta.txt}, {atmosphericCorrectionSequencement_solar_illumination.txt}, {atmosphericCorrectionSequencement_wavelenght_spectral_bands_spot4_1.txt}
//  
// 
//  OUTPUTS: {AtmosphericCorrectionSequencement.tif}
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// \index{otb::MultiChannelRAndBAndNIRVegetationIndexImageFilter}
// \index{otb::MultiChannelRAndBAndNIRVegetationIndexImageFilter!header}
// \index{otb::VegetationIndex}
// \index{otb::VegetationIndex!header}
//
//
// The following example illustrates the application of an atmospheric correction to
// a Pleiade image.
// This correction is made using in four steps :
// \begin{itemize}
// \item Numerical to Luminance image changeover;
// \item Luminance to Refletance image changeover;
// \item Reflectance to Atmospheric correction changeover;
// \item Correction of the adjency effects : neighborhood contribution taking into account.
// \end{itemize}
// 
// The manipulation of each class used for the different step and the link with the 6S radiometry 
// library will be explained.
//
// Let's look at the minimal code required to use this algorithm. First, the following header 
// defining the \doxygen{otb}{AtmosphericCorrectionSequencement} 
// class must be included.
// For the numerical to luminance image, luminance to refletance image, and 
// reflectance to atmospheric correction image changeover and the neighborhood 
// correction, four includes are needed. 
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbImageToLuminanceImageFilter.h" 
#include "otbLuminanceToReflectanceImageFilter.h"
#include "otbReflectanceToSurfaceReflectanceImageFilter.h"
#include "otbSurfaceAdjencyEffect6SCorrectionSchemeFilter.h"
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
// This chain use 6S to compute radiometric parameters. To manipulate 6S datas, three classes
// are needed (the first to store the metadatas, the second that
// calls 6S class and generates the asked informations which will be stored in the last class).
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbAtmosphericCorrectionParameters.h"
#include "otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms.h"
#include "otbAtmosphericRadiativeTerms.h"
// Software Guide : EndCodeSnippet

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include <string>

int main( int argc, char *argv[] )
{
  if( argc != 20 )
    {
      std::cerr << "Missing Parameters " << std::endl;
      std::cerr << "Usage: " << argv[0]<< std::endl;
      std::cerr << "    inputImage , alphaBetaFileName , solarIlluminationFileName , wavelenghtSpectralBandFileName ," << std::endl;
      std::cerr << "    outputImage,"  << std::endl;
      std::cerr << "    zenithalSolarRadius , day , month ,"  << std::endl;
      std::cerr << "    solarZenithalAngle , solarAzimutalAngle , viewingZenithalAngle , viewingAzimutalAngle ,"  << std::endl;
      std::cerr << "    atmosphericPressure , waterVaporAmount , ozoneAmount , aerosolModel , AerosolOptical ,"  << std::endl;
      std::cerr << "    adjencyEffect6SCorrectionWindowRadius , pixelSpacingInKilometers" << std::endl;
      std::cerr << std::endl;
      return 1;
    }
  
  //  Software Guide : BeginLatex
  //  
  // The image types are now defined using pixel types and 
  // dimension. The input image is defined as an \doxygen{otb}{VectorImage}, 
  // the output is a \doxygen{otb}{VectorImage}. To simplify, input and 
  // output image type are the same one.
  //
  // Software Guide : EndLatex 
  
  // Software Guide : BeginCodeSnippet
  const unsigned int                                    Dimension = 2;
  typedef double                                        PixelType;
  typedef otb::VectorImage<PixelType,Dimension>         ImageType;
  // Software Guide : EndCodeSnippet
  
  // We instantiate reader and writer types
  typedef otb::ImageFileReader<ImageType>   ReaderType;
  typedef otb::ImageFileWriter<ImageType>   WriterType;
  ReaderType::Pointer reader  = ReaderType::New();

  // Software Guide : BeginLatex
  //  
  // The \code{GenerateOutputInformation()} reader method is invoqued
  // to known the number of component per pixel of the image.  It is recommended to 
  // place GenerateOutputInformation calls in a \code{try/catch} block in case 
  // errors occur and exceptions are thrown.
  //
  // Software Guide : EndLatex 
  
  // Software Guide : BeginCodeSnippet
  reader->SetFileName(argv[1]);
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
  // Software Guide : BeginLatex
  //
  // \doxygen{otb}{ImageToLuminanceImageFilter}
  // type is defined and instancied. This class used a functor applied 
  // to each component of each pixel (\mathbf{X^{k}}) which formula is the following one.
  //
  // \begin{equation}
  // \mathbf{L_{TOA}^{k}} = \frac{ X^{k} } { \alpha_{k} } + \beta_{k}
  // \end{equation}
  //
  // Where :
  // \begin{itemize}
  // \item $\mathbf{L_{TOA}^{k}}$ is the incidente luminance (in W.m^{-2}.sr^{-1}.\mum^{-1}).
  // \item X^{k} is the numeric account measured (ie. the input image pixel component).
  // \item $\alpha_{k}$ is the absolute calibration gain for the channel k.
  // \item $\beta_{k}$ is the absolute calibration bias for the channel k.
  // \end{itemize}
  //
  // Software Guide : EndLatex 
  
  // Software Guide : BeginCodeSnippet
  typedef otb::ImageToLuminanceImageFilter<ImageType,ImageType> 
    ImageToLuminanceImageFilterType;

  ImageToLuminanceImageFilterType::Pointer filterImageToLuminance 
    = ImageToLuminanceImageFilterType::New();

 // Software Guide : EndCodeSnippet
  typedef ImageToLuminanceImageFilterType::VectorType VectorType; 
  VectorType alpha(nbOfComponent);
  VectorType beta(nbOfComponent);
  alpha.Fill(0);
  beta.Fill(0);
  std::ifstream fin;
  fin.open(argv[2]);
  double dalpha(0.), dbeta(0.);
  for( unsigned int i=0 ; i < nbOfComponent ; i++)
    {
      fin >> dalpha;
      fin >> dbeta;
      alpha[i] = dalpha;
      beta[i] = dbeta;
    }
  fin.close();

  // Software Guide : BeginLatex
  // $\alpha$ and $\beta$ are read from a file text given in input, 
  // stored in vector and given to the class. 
  // Software Guide : EndLatex 

   // Software Guide : BeginCodeSnippet
  filterImageToLuminance->SetAlpha(alpha);
  filterImageToLuminance->SetBeta(beta);
  // Software Guide : EndCodeSnippet

  //-------------------------------
  // Software Guide : BeginLatex
  //
  // \doxygen{otb}{LuminanceToReflectanceImageFilter}
  // type is defined and instancied. 
  // This class used a functor applied to each component of each pixel 
  // of the luminance filter output (\mathbf{L_{TOA}^{k}}) the following formula.
  //
  // \begin{equation}
  // \rho_{TOA}^{k} = \frac{ \pi.\mathbf{L_{TOA}^{k}} } { E_{S}^{k}.cos(\theta_{S}).d/d_{0} }
  // \end{equation}
  //
  //// Where :
  // \begin{itemize}
  // \item $\mathbf{rho_{TOA}^{k}}$ is the reflectance measured by at the sensor.
  // \item $\theta_{S}$ is the zenithal solar angle in degrees.
  // \item $E_{S}^{k}$ is the solar illumination out of the atmosphere measured at a distance 
  // d_{0} from the earth.\item d/d_{0} is the ratio between the Earth-Sun distance at 
  // the acquisition date and the mean Earth-Sun distance. The ratio can be directly 
  // given to the class or computed using a 6S routine. 
  // In the last case (that is the one of this example), the user has to precise 
  // the month and the day of the acquisition.
  // \end{itemize}
  //
  // Software Guide : EndLatex 
  
  // Software Guide : BeginCodeSnippet
  typedef otb::LuminanceToReflectanceImageFilter<ImageType,ImageType> 
                                LuminanceToReflectanceImageFilterType;
  LuminanceToReflectanceImageFilterType::Pointer filterLuminanceToReflectance 
    = LuminanceToReflectanceImageFilterType::New();
 // Software Guide : EndCodeSnippet

  typedef LuminanceToReflectanceImageFilterType::VectorType VectorType; 

  VectorType solarIllumination(nbOfComponent);
  solarIllumination.Fill(0);
  
  fin.open(argv[3]);
  double dsolarIllumination(0.);
  for( unsigned int i=0 ; i < nbOfComponent ; i++)
  {
        fin >> dsolarIllumination;
        solarIllumination[i] = dsolarIllumination;
  }
  fin.close();
   
  // Software Guide : BeginLatex
  // The solar illumination is read from a file text given in input, stored in vector 
  // and given to the class.
  // Day, month and zenital solar angle are inputs and can be directly given to the class.   
  // Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  filterLuminanceToReflectance->SetZenithalSolarAngle(static_cast<double>(atof(argv[6])));
  filterLuminanceToReflectance->SetDay(atoi(argv[7]));
  filterLuminanceToReflectance->SetMonth(atoi(argv[8])); 
  filterLuminanceToReflectance->SetSolarIllumination(solarIllumination);
  // Software Guide : EndCodeSnippet

//-------------------------------
  // Software Guide : BeginLatex
  //
  // At this step of the chain, radiometric informations are nedeed. Those informations
  // will be computed from different parameters stored in a 
  // \doxygen{otb}{AtmosphericCorrectionParameters} class intance. 
  // This "container" will be given to an 
  // \doxygen{otb}{AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms}
  // class instance which will call a 6S routine that will compute the needed 
  // radiometric informations and store them in a
  // \doxygen{otb}{AtmosphericRadiativeTerms} class instance.
  // For this,
  // \doxygen{otb}{AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms}
  // \doxygen{otb}{AtmosphericCorrectionParameters}
  // \doxygen{otb}{AtmosphericRadiativeTerms}
  // types are defined and instancied. 
  //
  // Software Guide : EndLatex 
  
  // Software Guide : BeginCodeSnippet
  typedef otb::AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms     
                                AtmosphericCorrectionParametersTo6SRadiativeTermsType;
  typedef otb::AtmosphericCorrectionParameters                                  
                                AtmosphericCorrectionParametersType;
  typedef otb::AtmosphericRadiativeTerms                                        
                                AtmosphericRadiativeTermsType;
  // Software Guide : EndCodeSnippet
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
  
  fin.open(argv[4]);
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
 
  // Software Guide : BeginLatex
  //
  // \doxygen{otb}{AtmosphericCorrectionParameters} class needs some various parameters :
  //  \begin{itemize}
  // \item The zenithal and azimutal solar angles that describe the solar incidence 
  // configuration (in degrees);
  // \item The zenithal and azimutal viewing angles that describe the viewing 
  // direction (in degrees); 
  // \item The month and the day of the acquisition;
  // \item The atmospheric pressure;
  // \item The water vapor amount that is the total water vapor content over vertical 
  // atmospheric column;
  // \item The ozone amount that is the Stratospheric ozone layer content;
  // \item The aerosol model that is the kind of particles (no aerosol, continental,
  // maritime, urban, desertic);
  // \item The aerosol optical thickness at 550 nm that is the is the Radiative impact
  //  of aerosol for the reference wavelength 550 nm;
  // \item The filter function that is the values of the filter function for one 
  // spectral band, from $\lambda_{inf}$ to $\lambda_{sup}$ by step of 2.5 nm.
  // One filter function by channel is required. 
  // This last parameter are read in text files, the other one are directly given to the class.
  // \end{itemize} 
  //
  // Software Guide : EndLatex 
  

  // Set parameters
  // Software Guide : BeginCodeSnippet
  dataAtmosphericCorrectionParameters->SetSolarZenithalAngle(static_cast<double>(atof(argv[9])));
  dataAtmosphericCorrectionParameters->SetSolarAzimutalAngle(static_cast<double>(atof(argv[10])));
  dataAtmosphericCorrectionParameters->SetViewingZenithalAngle(static_cast<double>(atof(argv[11])));
  dataAtmosphericCorrectionParameters->SetViewingAzimutalAngle(static_cast<double>(atof(argv[12])));
  dataAtmosphericCorrectionParameters->SetMonth(atoi(argv[8]));
  dataAtmosphericCorrectionParameters->SetDay(atoi(argv[7]));
  dataAtmosphericCorrectionParameters->SetAtmosphericPressure(static_cast<double>(atof(argv[13]))); 
  dataAtmosphericCorrectionParameters->SetWaterVaporAmount(static_cast<double>(atof(argv[14])));
  dataAtmosphericCorrectionParameters->SetOzoneAmount(static_cast<double>(atof(argv[15])));
  AerosolModelType aerosolModel = static_cast<AerosolModelType>(::atoi(argv[16]));
  dataAtmosphericCorrectionParameters->SetAerosolModel(aerosolModel);
  dataAtmosphericCorrectionParameters->SetAerosolOptical(static_cast<double>(atof(argv[17])));
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Once those parameters are loaded and stored in the AtmosphericCorrectionParameters 
  // instance class, it is given in input of an instance of 
  // AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms that will compute
  // the needed radiometric informations.
  //
  // Software Guide : EndLatex 

 // Software Guide : BeginCodeSnippet
  AtmosphericCorrectionParametersTo6SRadiativeTermsType::Pointer  filterAtmosphericCorrectionParametersTo6SRadiativeTerms = AtmosphericCorrectionParametersTo6SRadiativeTermsType::New();
  filterAtmosphericCorrectionParametersTo6SRadiativeTerms->SetInput( dataAtmosphericCorrectionParameters );
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The output of this class will be an instance of the AtmosphericRadiativeTerms class.
  // This class contains (for each channel of the image)
  // \begin{itemize}
  // \item The Intrinsic atmospheric reflectance that takes into account for the molecular scattering 
  // and the aerosol scattering attenuated by water vapor absorption;
  // \item The spherical albedo of the atmosphere;
  // \item The total gaseous transmission (for all species);
  // \item The total transmittance of the atmosphere from sun to ground (downward transmittance) 
  // and from ground to space sensor (upward transmittance).
  // \end{itemize} 
  //
  // Software Guide : EndLatex 

//-------------------------------
  // Software Guide : BeginLatex
  // The atmospheric correction can now start.
  // An instance of \doxygen{otb}{ReflectanceToSurfaceReflectanceImageFilter} is created.
  // Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef otb::ReflectanceToSurfaceReflectanceImageFilter<ImageType, ImageType> ReflectanceToSurfaceReflectanceImageFilterType;
  ReflectanceToSurfaceReflectanceImageFilterType::Pointer  filterReflectanceToSurfaceReflectanceImageFilter 
  	= ReflectanceToSurfaceReflectanceImageFilterType::New();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  // The aim of the atmospheric correction is to inverse the surface reflectance 
  // (for each pixel of the input image) from the TOA reflectance and from simulations 
  // of the atmospheric radiative functions corresponding to the geometrical conditions 
  // of the observation and to the atmospheric components.
  // The process required to be applied on each pixel of the image, band by band with 
  // the folowing formula.
  // 
  // \begin{equation}
  // \rho_{S}^{unif} = \frac{ \mathbf{A} }{ 1 + Sx\mathbf{A} }
  // \end{equation}
  // Where,
  // \begin{equation}
  // \mathbf{A} = \frac{ \rhO_{TOA} - \rhO_{atm} }{ T(\mu_{S}).T(\mu_{V}).t_{g}^{all gas} }
  // \end{equation}
  //
  // With :
  // \begin{itemize}
  // \item $\rhO_{TOA}$ is the reflectance at the top of the atmosphere;
  // \item $\rho_{S}^{unif}$ is the ground reflectance under asumption
  // of a lambertian surface and an uniform environment;
  // \item $\rhO_{atm}$ is the intrinsic atmospheric reflectance;
  // \item t_{g}^{all gas} is the soherical albedo of the atmosphere;
  // \item T($\mu_{S}$) is the downward transmittance;
  // \item T($\mu_{V}$) is the upward transmittance.
  // \end{itemize} 
  // All those parameters are contained in the AtmosphericCorrectionParametersTo6SRadiativeTerms
  // output.
  // Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  filterReflectanceToSurfaceReflectanceImageFilter->
    SetAtmosphericRadiativeTerms( filterAtmosphericCorrectionParametersTo6SRadiativeTerms->GetOutput() );
  // Software Guide : EndCodeSnippet


//-------------------------------
  // Software Guide : BeginLatex
  // Next (and last step) is the neighborhood correction.
  // For this, the SurfaceAdjencyEffect6SCorrectionSchemeFilter class is used.
  // The previous surface reflectance inversion is performed under the assumption of a 
  // homogeneous ground environment. The following step allows to correct the adjacency 
  // effect on the radiometry of pixels. The method is based on the decomposition of 
  // the observed signal as the summation of the own  contribution of the target pixel and 
  // of the contribution of neighbored pixels moderated by their distance to the target pixel. 
  // A simplified relation may be :
  // \begin{equation}
  // \rho{S} = \frac{ \rho_{S}^{unif}.T(\mu_{V}) - <\rho{S}>.t_{d}(\mu_{v}) }{ exp(-\delta/\mu_{v}) }
  // \end{equation}
  // With :
  // \begin{itemize}
  // \item $\rho_{S}^{unif}$ is the ground reflectance under asumption of an homogeneous environment;
  // \item T($\mu_{V}$) is the upward transmittance;
  // \item t_{d}($\mu_{S}$) is the upward diffus transmittance;
  // \item exp(-$\delta$/$\mu_{v}$) is the upward direct transmittance;
  // \item <$\rho{S}$> is the environment contribution to the pixel target reflectance in the total
  // observed signal.
  // \begin{equation}
  // <\rho{S}> = <math>\sum{j}<math>\sum{i}f(r(i,j))x\rho_{S}^{unif}(i,j)<\math><\math>
  // \end{equation}
  // where, 
  // \begin{itemize}
  // \item r(i,j) is the distance between the pixel(i,j) and the central pixel of the window in km;
  // \item f(r) is the global environment function.
  // \begin{equation}
  // f(r) = \frac{t_{d}^{R}(\mu_{V}).f_{R}(r)+t_{d}^{A}(\mu_{V}).f_{A}(r)}{ t_{d}(\mu_{V}) }
  // \end{equation}
  // \end{itemize} 
  // \end{itemize} 
  // The neighborhood consideration window size is given by the window radius. 
  // An instance of \doxygen{otb}{SurfaceAdjencyEffect6SCorrectionSchemeFilter} is created.
  // Software Guide : EndLatex

  //  Software Guide : BeginLatex
  typedef otb::SurfaceAdjencyEffect6SCorrectionSchemeFilter<ImageType, ImageType> 
    SurfaceAdjencyEffect6SCorrectionSchemeFilterType;
  SurfaceAdjencyEffect6SCorrectionSchemeFilterType::Pointer  filterSurfaceAdjencyEffect6SCorrectionSchemeFilter 
    = SurfaceAdjencyEffect6SCorrectionSchemeFilterType::New();
  //  Software Guide : EndLatex

  // Software Guide : BeginLatex
  //
  // The needs four input informations:
  // \begin{itemize}
  // \item The radiometric informations (the output of the AtmosphericCorrectionParametersTo6SRadiativeTerms filter);
  // \item The zenithal viewing angle;
  // \item The neighborhood window radius;
  // \item The pixel spacing in km.
  // \end{itemize} 
  //
  // Software Guide : EndLatex
  filterSurfaceAdjencyEffect6SCorrectionSchemeFilter->SetAtmosphericRadiativeTerms(filterAtmosphericCorrectionParametersTo6SRadiativeTerms->GetOutput());
  filterSurfaceAdjencyEffect6SCorrectionSchemeFilter->SetZenithalViewingAngle(dataAtmosphericCorrectionParameters->GetViewingZenithalAngle());
  filterSurfaceAdjencyEffect6SCorrectionSchemeFilter->SetWindowRadius(atoi(argv[18]));
  filterSurfaceAdjencyEffect6SCorrectionSchemeFilter->SetPixelSpacingInKilometers(static_cast<double>(atof(argv[19])));
 

//-------------------------------
  WriterType::Pointer writer = WriterType::New();
//  Software Guide : BeginLatex
  //  
  // At this step, each filter of the chain is instancied and every one has its 
  // input paramters set. A name can be given to the output image and each filter
  //  can linked to other to create the final treatment chain.
  //  
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  writer->SetFileName(argv[5]);

  filterImageToLuminance->SetInput(reader->GetOutput());
  filterLuminanceToReflectance->SetInput(filterImageToLuminance->GetOutput());
  filterReflectanceToSurfaceReflectanceImageFilter->SetInput(filterLuminanceToReflectance->GetOutput());
  filterSurfaceAdjencyEffect6SCorrectionSchemeFilter->SetInput(filterReflectanceToSurfaceReflectanceImageFilter->GetOutput());

  writer->SetInput(filterSurfaceAdjencyEffect6SCorrectionSchemeFilter->GetOutput());
 // Software Guide : EndCodeSnippet

 //  Software Guide : BeginLatex
  //  
  //  The invocation of the \code{Update()} method on the writer triggers the
  //  execution of the pipeline.  It is recommended to place update calls in a
  //  \code{try/catch} block in case errors occur and exceptions are thrown.
  //
  //  Software Guide : EndLatex 
  // Software Guide : BeginCodeSnippet
  try
    {
      writer->Update();
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
    
  return EXIT_SUCCESS;
}

