/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


/* Example usage:
./ProsailModel 30.0 10.0 0.0 0.015 0.009 1.2 2 50 1 70 0.2 30 0 0 Output/SailReflTest.txt
*/

//
// This example presents how to use PROSAIL (Prospect + Sail) model to generate
// viewing reflectance from leaf parameters, vegetation, and viewing parameters.
// Output can be used to simulate image for example.
//
// Let's look at the minimal code required to use this algorithm. First, the
// following headers must be included.

#include "otbSailModel.h"
#include "otbProspectModel.h"
#include "otbMacro.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int main(int argc, char* argv[])
{
  if (argc < 16)
  {
    std::cerr << "Missing Parameters " << std::endl;
    return EXIT_FAILURE;
  }

  char* OutputName = argv[15];
  //  We now define leaf parameters, which characterize vegetation composition.

  using LeafParametersType = otb::LeafParameters;


  //  Next the parameters variable is created by invoking the \code{New()}~method and
  //  assigning the result to a \doxygen{itk}{SmartPointer}.

  LeafParametersType::Pointer leafParams = LeafParametersType::New();


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

  double Cab    = static_cast<double>(atof(argv[1]));
  double Car    = static_cast<double>(atof(argv[2]));
  double CBrown = static_cast<double>(atof(argv[3]));
  double Cw     = static_cast<double>(atof(argv[4]));
  double Cm     = static_cast<double>(atof(argv[5]));
  double N      = static_cast<double>(atof(argv[6]));

  leafParams->SetCab(Cab);
  leafParams->SetCar(Car);
  leafParams->SetCBrown(CBrown);
  leafParams->SetCw(Cw);
  leafParams->SetCm(Cm);
  leafParams->SetN(N);

  //  Leaf parameters are used as prospect input

  using ProspectType             = otb::ProspectModel;
  ProspectType::Pointer prospect = ProspectType::New();

  prospect->SetInput(leafParams);

  //  Now we use SAIL model to generate transmitance and reflectance spectrum. SAIL model is created by invoking
  //  the \code{New()} method and
  //  assigning the result to a \doxygen{itk}{SmartPointer}.


  //  sail input parameters are :
  //  \begin{itemize}
  //  \item leaf area index (LAI).
  //  \item average leaf angle (Angle) in $\deg$.
  //  \item soil coefficient (PSoil).
  //  \item diffuse/direct radiation (Skyl).
  //  \item hot spot (HSpot).
  //  \item solar zenith angle (TTS) in $\deg$.
  //  \item observer zenith angle (TTO) in $\deg$.
  //  \item azimuth (PSI) in $\deg$.
  //  \end{itemize}


  double LAI   = static_cast<double>(atof(argv[7]));
  double Angle = static_cast<double>(atof(argv[8]));
  double PSoil = static_cast<double>(atof(argv[9]));
  double Skyl  = static_cast<double>(atof(argv[10]));
  double HSpot = static_cast<double>(atof(argv[11]));
  double TTS   = static_cast<double>(atof(argv[12]));
  double TTO   = static_cast<double>(atof(argv[13]));
  double PSI   = static_cast<double>(atof(argv[14]));

  using SailType = otb::SailModel;

  SailType::Pointer sail = SailType::New();

  sail->SetLAI(LAI);
  sail->SetAngl(Angle);
  sail->SetPSoil(PSoil);
  sail->SetSkyl(Skyl);
  sail->SetHSpot(HSpot);
  sail->SetTTS(TTS);
  sail->SetTTO(TTO);
  sail->SetPSI(PSI);

  //  Reflectance and Transmittance  are set with prospect output.

  sail->SetReflectance(prospect->GetReflectance());
  sail->SetTransmittance(prospect->GetTransmittance());


  //  The invocation of the \code{Update()} method triggers the
  //  execution of the pipeline.

  sail->Update();

  //  \emph{GetViewingReflectance} method provides viewing reflectance vector (size $Nx2$, where $N$ is the number of sampled wavelength values, columns
  //  corresponds respectively to wavelength and viewing reflectance) by calling \emph{GetResponse}. \emph{GetHemisphericalReflectance} method provides
  //  hemispherical reflectance vector (size $Nx2$, where $N$ is the number ofsampled wavelength values, columns corresponds to wavelength and hemispherical
  //  reflectance) by calling \emph{GetResponse}.
  //
  // Note that PROSAIL simulation are done for 2100 samples starting from 400nm up to 2500nm

  for (unsigned int i = 0; i < sail->GetViewingReflectance()->Size(); ++i)
  {
    otbLogMacro(Debug, << "wavelength  : " << sail->GetViewingReflectance()->GetResponse()[i].first << ". Viewing reflectance "
                       << sail->GetViewingReflectance()->GetResponse()[i].second << ". Hemispherical reflectance "
                       << sail->GetHemisphericalReflectance()->GetResponse()[i].second);
  }

  std::ofstream outputFile(OutputName, std::ios::out);
  for (unsigned int i = 0; i < sail->GetHemisphericalReflectance()->Size(); ++i)
  {
    outputFile << sail->GetViewingReflectance()->GetResponse()[i].second << " ";
    outputFile << sail->GetHemisphericalReflectance()->GetResponse()[i].second << std::endl;
  }

  //  here you can found example parameters :
  //  \begin{itemize}
  //  \item Cab 30.0
  //  \item Car 10.0
  //  \item CBrown 0.0
  //  \item Cw 0.015
  //  \item Cm 0.009
  //  \item N 1.2
  //  \item LAI 2
  //  \item Angle 50
  //  \item PSoil 1
  //  \item Skyl 70
  //  \item HSpot 0.2
  //  \item TTS 30
  //  \item TTO 0
  //  \item PSI 0
  //  \end{itemize}
  //
  //  More information and data about leaf properties can be found at \emph{St\'{e}phane Jacquemoud}
  //  \href{http://teledetection.ipgp.jussieu.fr/opticleaf/}{OPTICLEAF} website.

  return EXIT_SUCCESS;
}
