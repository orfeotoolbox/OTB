/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.

 Some parts of this code are derived from ITK. See ITKCopyright.txt
 for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/

//  Software Guide : BeginCommandLineArgs
//    INPUTS:
//    30.0 10.0 0.0 0.015 0.009 1.2 2 50 1 70 0.2 30 0 0
//    OUTPUTS: SailReflTest.txt
//  Software Guide : EndCommandLineArgs
//
// Software Guide : BeginLatex
//
// This example presents how to use PROSAIL (Prospect + Sail) model to generate
// viewing reflectance from leaf parameters, vegetation, and viewing parameters.
// Output can be used to simulate image for example.
//
// Let's look at the minimal code required to use this algorithm. First, the
// following headers must be included.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbSailModel.h"
#include "otbProspectModel.h"
// Software Guide : EndCodeSnippet


#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int main(int argc, char *argv[])
{
  if (argc < 16)
    {
    std::cerr << "Missing Parameters " << std::endl;
    return EXIT_FAILURE;
    }

  char * OutputName = argv[15];
  //  Software Guide : BeginLatex
  //
  //  We now define leaf parameters, which characterize vegetation composition.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::LeafParameters LeafParametersType;
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  Next the parameters variable is created by invoking the \code{New()}~method and
  //  assigning the result to a \doxygen{itk}{SmartPointer}.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  LeafParametersType::Pointer leafParams = LeafParametersType::New();
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  Leaf characteristics is then set.
  //  Input parameters are :
  //  \begin{itemize}
  //  \item Chlorophyll concentration (Cab) in $\mu g/cm^2$.
  //  \item Carotenoid concentration (Car) in $\mu g/cm^2$.
  //  \item Brown pigment content (CBrown) in arbitrary unit.
  //  \item Water thickness EWT (Cw) in $cm$.
  //  \item Dry matter content LMA (Cm) in $g/cm^2$.
  //  \item Leaf structure parameter (N).
  //  \end{itemize}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  double Cab = static_cast<double> (atof(argv[1]));
  double Car = static_cast<double> (atof(argv[2]));
  double CBrown = static_cast<double> (atof(argv[3]));
  double Cw = static_cast<double> (atof(argv[4]));
  double Cm = static_cast<double> (atof(argv[5]));
  double N = static_cast<double> (atof(argv[6]));

  leafParams->SetCab(Cab);
  leafParams->SetCar(Car);
  leafParams->SetCBrown(CBrown);
  leafParams->SetCw(Cw);
  leafParams->SetCm(Cm);
  leafParams->SetN(N);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Leaf parameters are used as prospect input
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ProspectModel ProspectType;
  ProspectType::Pointer prospect = ProspectType::New();

  prospect->SetInput(leafParams);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Now we use SAIL model to generate transmitance and reflectance spectrum. SAIL model is created by invoking
  //  the \code{New()} method and
  //  assigning the result to a \doxygen{itk}{SmartPointer}.
  //
  //  Software Guide : EndLatex


  //  Software Guide : BeginLatex
  //
  //  sail input parameters are :
  //  \begin{itemize}
  //  \item leaf area index (LAI).
  //  \item average leaf angle (Ang) in $\deg$.
  //  \item soil coefficient (PSoil).
  //  \item diffuse/direct radiation (Skyl).
  //  \item hot spot (HSpot).
  //  \item solar zenith angle (TTS) in $\deg$.
  //  \item observer zenith angle (TTO) in $\deg$.
  //  \item azimuth (PSI) in $\deg$.
  //  \end{itemize}
  //
  //  Software Guide : EndLatex


  // Software Guide : BeginCodeSnippet
  double LAI = static_cast<double> (atof(argv[7]));
  double Angl = static_cast<double> (atof(argv[8]));
  double PSoil = static_cast<double> (atof(argv[9]));
  double Skyl = static_cast<double> (atof(argv[10]));
  double HSpot = static_cast<double> (atof(argv[11]));
  double TTS = static_cast<double> (atof(argv[12]));
  double TTO = static_cast<double> (atof(argv[13]));
  double PSI = static_cast<double> (atof(argv[14]));

  typedef otb::SailModel SailType;

  SailType::Pointer sail = SailType::New();

  sail->SetLAI(LAI);
  sail->SetAngl(Angl);
  sail->SetPSoil(PSoil);
  sail->SetSkyl(Skyl);
  sail->SetHSpot(HSpot);
  sail->SetTTS(TTS);
  sail->SetTTO(TTO);
  sail->SetPSI(PSI);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //  Reflectance and Transmittance  are set with prospect output.
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  sail->SetReflectance(prospect->GetReflectance());
  sail->SetTransmittance(prospect->GetTransmittance());
  // Software Guide : EndCodeSnippet


  // Software Guide : BeginLatex
  //  The invocation of the \code{Update()} method triggers the
  //  execution of the pipeline.
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  sail->Update();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  \emph{GetViewingReflectance} method provides viewing reflectance vector (size $Nx2$, where $N$ is the number of sampled wavelength values, columns corresponds respectively to wavelength and viewing reflectance) by calling \emph{GetResponse}.
  //  \emph{GetHemisphericalReflectance} method provides hemispherical reflectance vector (size $Nx2$, where $N$ is the number ofsampled wavelength values, columns corresponds to wavelength and hemispherical reflectance) by calling \emph{GetResponse}.
  //
  // Note that PROSAIL simulation are done for 2100 samples starting from 400nm up to 2500nm
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  for (unsigned int i = 0; i < sail->GetViewingReflectance()->Size(); ++i)
    {
    std::cout << "wavelength  : ";
    std::cout << sail->GetViewingReflectance()->GetResponse()[i].first;
    std::cout << ". Viewing reflectance ";
    std::cout << sail->GetViewingReflectance()->GetResponse()[i].second;
    std::cout << ". Hemispherical reflectance ";
    std::cout << sail->GetHemisphericalReflectance()->GetResponse()[i].second;
    std::cout << std::endl;
    }
  // Software Guide : EndCodeSnippet

  std::ofstream outputFile(OutputName, std::ios::out);
  for (unsigned int i = 0; i < sail->GetHemisphericalReflectance()->Size(); ++i)
    {
    outputFile << sail->GetViewingReflectance()->GetResponse()[i].second << " ";
    outputFile << sail->GetHemisphericalReflectance()->GetResponse()[i].second<< std::endl;
    }

  //  Software Guide : BeginLatex
  //
  //  here you can found example parameters :
  //  \begin{itemize}
  //  \item Cab 30.0
  //  \item Car 10.0
  //  \item CBrown 0.0
  //  \item Cw 0.015
  //  \item Cm 0.009
  //  \item N 1.2
  //  \item LAI 2
  //  \item Angl 50
  //  \item PSoil 1
  //  \item Skyl 70
  //  \item HSpot 0.2
  //  \item TTS 30
  //  \item TTO 0
  //  \item PSI 0
  //  \end{itemize}
  //
  //  More information and data about leaf properties can be found at \emph{St\'{e}phane Jacquemoud} \href{http://teledetection.ipgp.jussieu.fr/opticleaf/}{OPTICLEAF} website.
  //  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
