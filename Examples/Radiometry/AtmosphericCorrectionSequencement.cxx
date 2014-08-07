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


//  Software Guide : BeginCommandLineArgs
//  INPUTS: {Romania_Extract.tif}
//  OUTPUTS: {AtmosphericCorrectionSequencement.tif}
//  ${OTB_DATA_ROOT}/Examples/atmosphericCorrectionSequencement_alpha_beta.txt ${OTB_DATA_ROOT}/Examples/atmosphericCorrectionSequencement_solar_illumination.txt ${OTB_DATA_ROOT}/Examples/atmosphericCorrectionSequencement_wavelength_spectral_bands_spot4_1.txt 27.3 4 12 152.7 2.5 -77.0 1013. 2.48134 0.34400 1 0.199854 2 0.020
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// \index{otb::MultiChannelRAndBAndNIRVegetationIndexImageFilter}
// \index{otb::MultiChannelRAndBAndNIRVegetationIndexImageFilter!header}
// \index{otb::VegetationIndex}
// \index{otb::VegetationIndex!header}
//
//
// The following example illustrates the application of atmospheric corrections to
// an optical multispectral image similar to Pleiades.
// These corrections are made in four steps :
// \begin{itemize}
// \item digital number to luminance correction;
// \item luminance to refletance image conversion;
// \item atmospheric correction for TOA (top of atmosphere) to TOC (top of canopy)
// reflectance estimation;
// \item correction of the adjacency effects taking into account the neighborhood contribution.
// \end{itemize}
//
// The manipulation of each class used for the different steps and the
// link with the 6S radiometry library will be explained.
//
// Let's look at the minimal code required to use this
// algorithm. First, the following header defining the
// \doxygen{otb}{AtmosphericCorrectionSequencement} class must be
// included.  For the numerical to luminance image, luminance to
// refletance image, and reflectance to atmospheric correction image
// corrections and the neighborhood correction, four header files are
// required.
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbImageToLuminanceImageFilter.h"
#include "otbLuminanceToReflectanceImageFilter.h"
#include "otbReflectanceToSurfaceReflectanceImageFilter.h"
#include "otbSurfaceAdjacencyEffectCorrectionSchemeFilter.h"
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
// This chain uses the 6S radiative
// transfer code to compute radiometric parameters. To manipulate 6S
// data, three classes are needed (the first one to store the metadata,
// the second one that calls 6S class and generates the information
// which will be stored in the last one).
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbAtmosphericCorrectionParameters.h"
#include "otbImageMetadataCorrectionParameters.h"
#include "otbRadiometryCorrectionParametersToAtmosphericRadiativeTerms.h"
#include "otbAtmosphericRadiativeTerms.h"
// Software Guide : EndCodeSnippet

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include <string>

int main(int argc, char *argv[])
{
  if (argc != 19)
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0] << std::endl;
    std::cerr <<
    " inputImage outputImage atmosphericCorrectionSequencement_alpha_beta.txt atmosphericCorrectionSequencement_solar_illumination.txt atmosphericCorrectionSequencement_wavelength_spectral_bands_spot4_1.txt SolarZenithalAngle day month SolarAzimuthalAngle ViewingZenithalAngle ViewingAzimuthalAngle AtmosphericPresure WaterVaporAmount OzoneAmount AerosolModel AerosolOpticalThickness WindowRadiusForAdjacencyCorrection PixelSpacing"
              << std::endl;
    std::cerr << std::endl;
    return 1;
    }

  //  Software Guide : BeginLatex
  //
  // Image types are now defined using pixel types and
  // dimension. The input image is defined as an \doxygen{otb}{VectorImage},
  // the output image is a \doxygen{otb}{VectorImage}. To simplify, input and
  // output image types are the same one.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  const unsigned int Dimension = 2;
  typedef double                                 PixelType;
  typedef otb::VectorImage<PixelType, Dimension> ImageType;
  // Software Guide : EndCodeSnippet

  // We instantiate reader and writer types
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;
  ReaderType::Pointer reader  = ReaderType::New();

  // Software Guide : BeginLatex
  //
  // The \code{GenerateOutputInformation()} reader method is called
  // to know the number of component per pixel of the image.  It is
  // recommended to
  // place \code{GenerateOutputInformation} calls in a \code{try/catch} block in case
  // errors occur and exceptions are thrown.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  reader->SetFileName(argv[1]);
  try
    {
    reader->GenerateOutputInformation();
    }
  catch (itk::ExceptionObject& excep)
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    }
  // Software Guide : EndCodeSnippet
  catch (...)
    {
    std::cout << "Unknown exception !" << std::endl;
    return EXIT_FAILURE;
    }

  unsigned int nbOfComponent =
    reader->GetOutput()->GetNumberOfComponentsPerPixel();

  //-------------------------------
  // Software Guide : BeginLatex
  //
  // The \doxygen{otb}{ImageToLuminanceImageFilter}
  // type is defined and instancied. This class uses a functor applied
  // to each component of each pixel ($\mathbf{X^{k}}$) whose formula is:
  //
  // \begin{equation}
  // \mathbf{L_{TOA}^{k}} = \frac{ X^{k} } { \alpha_{k} } + \beta_{k}.
  // \end{equation}
  //
  // Where :
  // \begin{itemize}
  // \item $\mathbf{L_{TOA}^{k}}$ is the incident luminance (in
  // $W.m^{-2}.sr^{-1}.\mu m^{-1}$);
  // \item $\mathbf{X^{k}}$ is the measured digital number (ie. the input image pixel component);
  // \item $\alpha_{k}$ is the absolute calibration gain for the channel k;
  // \item $\beta_{k}$ is the absolute calibration bias for the channel k.
  // \end{itemize}
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageToLuminanceImageFilter<ImageType, ImageType>
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
  fin.open(argv[3]);
  double dalpha(0.), dbeta(0.);
  for (unsigned int i = 0; i < nbOfComponent; ++i)
    {
    fin >> dalpha;
    fin >> dbeta;
    alpha[i] = dalpha;
    beta[i] = dbeta;
    }
  fin.close();

  // Software Guide : BeginLatex
  // Here, $\alpha$ and $\beta$ are read from an ASCII file given in input,
  // stored in a vector and passed to the class.
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filterImageToLuminance->SetAlpha(alpha);
  filterImageToLuminance->SetBeta(beta);
  // Software Guide : EndCodeSnippet

  //-------------------------------
  // Software Guide : BeginLatex
  //
  // The \doxygen{otb}{LuminanceToReflectanceImageFilter}
  // type is defined and instancied.
  // This class used a functor applied to each component of each pixel
  // of the luminance filter output ($\mathbf{L_{TOA}^{k}}$):
  //
  // \begin{equation}
  // \rho_{TOA}^{k} = \frac{ \pi.\mathbf{L_{TOA}^{k}} } { E_{S}^{k}.cos(\theta_{S}).d/d_{0} }.
  // \end{equation}
  //
  // Where :
  // \begin{itemize}
  // \item $\mathbf{\rho_{TOA}^{k}}$ is the reflectance measured by the sensor;
  // \item $\theta_{S}$ is the zenithal solar angle in degrees;
  // \item $E_{S}^{k}$ is the solar illumination out of the atmosphere measured at a distance
  // $d_{0}$ from the Earth;
  // \item $d/d_{0}$ is the ratio between the Earth-Sun distance at
  // the acquisition date and the mean Earth-Sun distance. The ratio can be directly
  // given to the class or computed using a 6S routine.
  // In the last case (that is the one of this example), the user has to precise
  // the month and the day of the acquisition.
  // \end{itemize}
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::LuminanceToReflectanceImageFilter<ImageType, ImageType>
  LuminanceToReflectanceImageFilterType;
  LuminanceToReflectanceImageFilterType::Pointer filterLuminanceToReflectance
    = LuminanceToReflectanceImageFilterType::New();
// Software Guide : EndCodeSnippet

  typedef LuminanceToReflectanceImageFilterType::VectorType VectorType;

  VectorType solarIllumination(nbOfComponent);
  solarIllumination.Fill(0);

  fin.open(argv[4]);
  double dsolarIllumination(0.);
  for (unsigned int i = 0; i < nbOfComponent; ++i)
    {
    fin >> dsolarIllumination;
    solarIllumination[i] = dsolarIllumination;
    }
  fin.close();

  // Software Guide : BeginLatex
  // The solar illumination is read from a ASCII file given in input,
  // stored in a vector
  // and given to the class.
  // Day, month and zenital solar angle are inputs and can be directly given to the class.
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filterLuminanceToReflectance->SetZenithalSolarAngle(
    static_cast<double>(atof(argv[6])));
  filterLuminanceToReflectance->SetDay(atoi(argv[7]));
  filterLuminanceToReflectance->SetMonth(atoi(argv[8]));
  filterLuminanceToReflectance->SetSolarIllumination(solarIllumination);
  // Software Guide : EndCodeSnippet

//-------------------------------
// Software Guide : BeginLatex
// TODO : need an update
// At this step of the chain, radiometric informations are nedeed. Those informations
// will be computed from different parameters stored in a
// \doxygen{otb}{AtmosphericCorrectionParameters} class intance.
// This {\em container} will be given to an
// \doxygen{otb}{RadiometryCorrectionParametersToAtmosphericRadiativeTerms}
// class instance which will call a 6S routine that will compute the needed
// radiometric informations and store them in a
// \doxygen{otb}{AtmosphericRadiativeTerms} class instance.
// For this,
// \doxygen{otb}{RadiometryCorrectionParametersToAtmosphericRadiativeTerms}
// \doxygen{otb}{AtmosphericCorrectionParameters} and
// \doxygen{otb}{AtmosphericRadiativeTerms}
// types are defined and instancied.
//
// Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::RadiometryCorrectionParametersToAtmosphericRadiativeTerms
  RadiometryCorrectionParametersToRadiativeTermsType;

  typedef otb::AtmosphericCorrectionParameters
  AtmosphericCorrectionParametersType;

  typedef otb::ImageMetadataCorrectionParameters
  AcquisitionCorrectionParametersType;

  typedef otb::AtmosphericRadiativeTerms
  AtmosphericRadiativeTermsType;
  // Software Guide : EndCodeSnippet
  typedef AtmosphericCorrectionParametersType::AerosolModelType
  AerosolModelType;
  typedef otb::FilterFunctionValues
  FilterFunctionValuesType;
  typedef FilterFunctionValuesType::ValuesVectorType
  ValuesVectorType;

  AtmosphericCorrectionParametersType::Pointer
    dataAtmosphericCorrectionParameters =
    AtmosphericCorrectionParametersType::New();
  AcquisitionCorrectionParametersType::Pointer
    dataAcquisitionCorrectionParameters =
    AcquisitionCorrectionParametersType::New();
  AtmosphericRadiativeTermsType::Pointer dataAtmosphericRadiativeTerms =
    AtmosphericRadiativeTermsType::New();

  float minSpectralValue(0.);
  float maxSpectralValue(0.);
  float userStep(0.);
  float value(0.);

  unsigned int     nbBands(0);
  unsigned int     nbValuesPerBand(0);
  std::string      sString;
  ValuesVectorType vector;

  fin.open(argv[5]);
  fin >> nbBands;
  for (unsigned int i = 0; i < nbBands; ++i)
    {
    vector.clear();
    fin >> sString;
    fin >> minSpectralValue;
    fin >> maxSpectralValue;
    fin >> userStep;
    fin >> nbValuesPerBand;
    for (unsigned int j = 0; j < nbValuesPerBand; ++j)
      {
      fin >> value;
      vector.push_back(value);
      }
    FilterFunctionValuesType::Pointer functionValues =
      FilterFunctionValuesType::New();
    functionValues->SetFilterFunctionValues(vector);
    functionValues->SetMinSpectralValue(minSpectralValue);
    functionValues->SetMaxSpectralValue(maxSpectralValue);
    functionValues->SetUserStep(userStep);
    dataAcquisitionCorrectionParameters->SetWavelengthSpectralBandWithIndex(
      i,
      functionValues);
    }

  fin.close();

  // Software Guide : BeginLatex
  // TODO : need an update
  // The \doxygen{otb}{AtmosphericCorrectionParameters} class needs several parameters :
  //  \begin{itemize}
  // \item The zenithal and azimutal solar angles that describe the solar incidence
  // configuration (in degrees);
  // \item The zenithal and azimuthal viewing angles that describe the viewing
  // direction (in degrees);
  // \item The month and the day of the acquisition;
  // \item The atmospheric pressure;
  // \item The water vapor amount, that is, the total water vapor content over vertical
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
  dataAcquisitionCorrectionParameters->SetSolarZenithalAngle(
    static_cast<double>(atof(argv[6])));

  dataAcquisitionCorrectionParameters->SetSolarAzimutalAngle(
    static_cast<double>(atof(argv[9])));

  dataAcquisitionCorrectionParameters->SetViewingZenithalAngle(
    static_cast<double>(atof(argv[10])));

  dataAcquisitionCorrectionParameters->SetViewingAzimutalAngle(
    static_cast<double>(atof(argv[11])));

  dataAcquisitionCorrectionParameters->SetMonth(atoi(argv[8]));

  dataAcquisitionCorrectionParameters->SetDay(atoi(argv[7]));

  dataAtmosphericCorrectionParameters->SetAtmosphericPressure(
    static_cast<double>(atof(argv[12])));

  dataAtmosphericCorrectionParameters->SetWaterVaporAmount(
    static_cast<double>(atof(argv[13])));

  dataAtmosphericCorrectionParameters->SetOzoneAmount(
    static_cast<double>(atof(argv[14])));

  AerosolModelType aerosolModel =
    static_cast<AerosolModelType>(::atoi(argv[15]));

  dataAtmosphericCorrectionParameters->SetAerosolModel(aerosolModel);

  dataAtmosphericCorrectionParameters->SetAerosolOptical(
    static_cast<double>(atof(argv[16])));
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Once those parameters are loaded and stored in the AtmosphericCorrectionParameters
  // instance class, it is given in input of an instance of
  // RadiometryCorrectionParametersToAtmosphericRadiativeTerms that will compute
  // the needed radiometric informations.
  //
  // Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  AtmosphericRadiativeTermsType::Pointer atmosphericRadiativeTerms =
    RadiometryCorrectionParametersToRadiativeTermsType::Compute(
      dataAtmosphericCorrectionParameters,
      dataAcquisitionCorrectionParameters);
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
// Atmospheric corrections can now start.
// First, an instance of \doxygen{otb}{ReflectanceToSurfaceReflectanceImageFilter} is created.
// Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ReflectanceToSurfaceReflectanceImageFilter<ImageType,
      ImageType>
  ReflectanceToSurfaceReflectanceImageFilterType;

  ReflectanceToSurfaceReflectanceImageFilterType::Pointer
    filterReflectanceToSurfaceReflectanceImageFilter
    = ReflectanceToSurfaceReflectanceImageFilterType::New();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  // The aim of the atmospheric correction is to invert the surface reflectance
  // (for each pixel of the input image) from the TOA reflectance and from simulations
  // of the atmospheric radiative functions corresponding to the geometrical conditions
  // of the observation and to the atmospheric components.
  // The process required to be applied on each pixel of the image, band by band with
  // the following formula:
  //
  // \begin{equation}
  // \rho_{S}^{unif} = \frac{ \mathbf{A} }{ 1 + Sx\mathbf{A} }
  // \end{equation}
  // Where,
  // \begin{equation}
  // \mathbf{A} = \frac{ \rho_{TOA} - \rho_{atm} }{ T(\mu_{S}).T(\mu_{V}).t_{g}^{all gas} }
  // \end{equation}
  //
  // With :
  // \begin{itemize}
  // \item $\rho_{TOA}$ is the reflectance at the top of the atmosphere;
  // \item $\rho_{S}^{unif}$ is the ground reflectance under assumption
  // of a lambertian surface and an uniform environment;
  // \item $\rho_{atm}$ is the intrinsic atmospheric reflectance;
  // \item $t_{g}^{all gas}$ is the spherical albedo of the atmosphere;
  // \item $T(\mu_{S})$ is the downward transmittance;
  // \item $T(\mu_{V})$ is the upward transmittance.
  // \end{itemize}
  // All those parameters are contained in the RadiometryCorrectionParametersToRadiativeTerms
  // output.
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filterReflectanceToSurfaceReflectanceImageFilter->
    SetAtmosphericRadiativeTerms(atmosphericRadiativeTerms);
  // Software Guide : EndCodeSnippet

//-------------------------------
// Software Guide : BeginLatex
// Next (and last step) is the neighborhood correction.
// For this, the SurfaceAdjacencyEffectCorrectionSchemeFilter class is used.
// The previous surface reflectance inversion is performed under the assumption of a
// homogeneous ground environment. The following step allows to correct the adjacency
// effect on the radiometry of pixels. The method is based on the decomposition of
// the observed signal as the summation of the own  contribution of the target pixel and
// of the contribution of neighbored pixels moderated by their distance to the target pixel.
// A simplified relation may be :
// \begin{equation}
// \rho{S} = \frac{ \rho_{S}^{unif}.T(\mu_{V}) - <\rho{S}>.t_{d}(\mu_{V}) }{ exp(-\delta/\mu_{V}) }
// \end{equation}
// With :
// \begin{itemize}
// \item $\rho_{S}^{unif}$ is the ground reflectance under assumption of an homogeneous environment;
// \item $T(\mu_{V})$ is the upward transmittance;
// \item $t_{d}(\mu_{S})$ is the upward diffus transmittance;
// \item $exp(-\delta/\mu_{V})$ is the upward direct transmittance;
// \item $\rho_{S}$ is the environment contribution to the pixel target reflectance in the total
// observed signal.
// \begin{equation}
// \rho{S} = \sum{j}\sum{i}f(r(i, j))\times \rho_{S}^{unif}(i, j)
// \end{equation}
// where,
// \begin{itemize}
// \item r(i, j) is the distance between the pixel(i, j) and the central pixel of the window in $km$;
// \item f(r) is the global environment function.
// \begin{equation}
// f(r) = \frac{t_{d}^{R}(\mu_{V}).f_{R}(r)+t_{d}^{A}(\mu_{V}).f_{A}(r)}{ t_{d}(\mu_{V}) }
// \end{equation}
// \end{itemize}
// \end{itemize}
// The neighborhood consideration window size is given by the window radius.
// An instance of \doxygen{otb}{SurfaceAdjacencyEffectCorrectionSchemeFilter} is created.
// Software Guide : EndLatex

  //  Software Guide : BeginCodeSnippet
  typedef otb::SurfaceAdjacencyEffectCorrectionSchemeFilter<ImageType,
      ImageType>
  SurfaceAdjacencyEffectCorrectionSchemeFilterType;
  SurfaceAdjacencyEffectCorrectionSchemeFilterType::Pointer
    filterSurfaceAdjacencyEffectCorrectionSchemeFilter
    = SurfaceAdjacencyEffectCorrectionSchemeFilterType::New();
  //  Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The needs four input informations:
  // \begin{itemize}
  // \item Radiometric informations (the output of the RadiometryCorrectionParametersToRadiativeTerms filter);
  // \item The zenithal viewing angle;
  // \item The neighborhood window radius;
  // \item The pixel spacing in kilometers.
  // \end{itemize}
  //
  // Software Guide : EndLatex
  filterSurfaceAdjacencyEffectCorrectionSchemeFilter->
    SetAtmosphericRadiativeTerms(atmosphericRadiativeTerms);
  filterSurfaceAdjacencyEffectCorrectionSchemeFilter->SetZenithalViewingAngle(
    dataAcquisitionCorrectionParameters->GetViewingZenithalAngle());
  filterSurfaceAdjacencyEffectCorrectionSchemeFilter->SetWindowRadius(atoi(argv
                                                                             [17]));
  filterSurfaceAdjacencyEffectCorrectionSchemeFilter->
  SetPixelSpacingInKilometers(static_cast<double>(atof(argv[18])));

//-------------------------------
  WriterType::Pointer writer = WriterType::New();
//  Software Guide : BeginLatex
//
// At this step, each filter of the chain is instancied and every one has its
// input paramters set. A name can be given to the output image and each filter
//  can linked to other to create the final processing chain.
//
//  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  writer->SetFileName(argv[2]);

  filterImageToLuminance->SetInput(reader->GetOutput());
  filterLuminanceToReflectance->SetInput(filterImageToLuminance->GetOutput());
  filterReflectanceToSurfaceReflectanceImageFilter->SetInput(
    filterLuminanceToReflectance->GetOutput());
  filterSurfaceAdjacencyEffectCorrectionSchemeFilter->SetInput(
    filterReflectanceToSurfaceReflectanceImageFilter->GetOutput());

  writer->SetInput(
    filterSurfaceAdjacencyEffectCorrectionSchemeFilter->GetOutput());
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  The invocation of the \code{Update()} method on the writer triggers the
//  execution of the pipeline.  It is recommended to place this call in a
//  \code{try/catch} block in case errors occur and exceptions are thrown.
//
//  Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
  try
    {
    writer->Update();
    }
  catch (itk::ExceptionObject& excep)
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    }
  // Software Guide : EndCodeSnippet
  catch (...)
    {
    std::cout << "Unknown exception !" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
