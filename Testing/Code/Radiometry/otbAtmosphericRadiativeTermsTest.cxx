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


#include <fstream>

#include "itkMacro.h"
#include "otbAtmosphericRadiativeTerms.h"
#include <cstdlib>

int otbAtmosphericRadiativeTermsNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef otb::AtmosphericRadiativeTerms AtmosphericRadiativeTermsType;

  // Instantiating object
  AtmosphericRadiativeTermsType::Pointer object = AtmosphericRadiativeTermsType::New();

  return EXIT_SUCCESS;
}

int otbAtmosphericRadiativeTermsSingleChannelNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef otb::AtmosphericRadiativeTermsSingleChannel AtmosphericRadiativeTermsType;

  // Instantiating object
  AtmosphericRadiativeTermsType::Pointer object = AtmosphericRadiativeTermsType::New();

  std::cout << object << std::endl;
  return EXIT_SUCCESS;
}

std::ostream& operator<<(std::ostream& os, const otb::AtmosphericRadiativeTerms::DataVectorType& values)
{
  os << "[";
  if (values.size() > 0)
    {
    os << values[0];
    }
  for (unsigned int i = 1; i < values.size(); ++i)
    {
    os << ", ";
    os << values[i];
    }
  os << "]\n";
  return os;
}

int otbAtmosphericRadiativeTermsTest(int itkNotUsed(argc), char * argv[])
{
  char * filename = argv[1];
  std::ofstream file;
  file.open(filename);

  typedef otb::AtmosphericRadiativeTerms AtmosphericRadiativeTermsType;

  // Instantiating object
  AtmosphericRadiativeTermsType::Pointer object = AtmosphericRadiativeTermsType::New();
  object->ValuesInitialization(3);

  // Set the values
  AtmosphericRadiativeTermsType::DataVectorType values;
  values.push_back(1.0);
  values.push_back(2.0);
  values.push_back(3.0);

  object->SetIntrinsicAtmosphericReflectances(values);
  object->SetSphericalAlbedos(values);
  object->SetTotalGaseousTransmissions(values);
  object->SetDownwardTransmittances(values);
  object->SetUpwardTransmittances(values);
  object->SetUpwardDiffuseTransmittances(values);
  object->SetUpwardDirectTransmittances(values);
  object->SetUpwardDiffuseTransmittancesForRayleigh(values);
  object->SetUpwardDiffuseTransmittancesForAerosol(values);
  object->SetWavelengthSpectralBand(values);

  file << object << std::endl;

  file << "\n\n TESTING ACCESSOR OUT OF RANGE \n\n";
  object->SetSphericalAlbedo(3, 4.0);
  object->SetTotalGaseousTransmission(5, 5.0);
  object->SetDownwardTransmittance(6, 6.0);
  object->SetUpwardTransmittance(7, 7.0);
  object->SetUpwardDiffuseTransmittance(8, 8.0);
  object->SetUpwardDirectTransmittance(9, 9.0);
  object->SetUpwardDiffuseTransmittanceForRayleigh(10, 10.0);
  object->SetUpwardDiffuseTransmittanceForAerosol(11, 11.0);
  object->SetWavelengthSpectralBand(12, 12.0);

  file << object << std::endl;

  file << "\n\n TESTING ACCESSOR \n\n";
  file << object->GetIntrinsicAtmosphericReflectances() << std::endl;
  file << object->GetSphericalAlbedos() << std::endl;
  file << object->GetTotalGaseousTransmissions() << std::endl;
  file << object->GetDownwardTransmittances() << std::endl;
  file << object->GetUpwardTransmittances() << std::endl;
  file << object->GetUpwardDiffuseTransmittances() << std::endl;
  file << object->GetUpwardDirectTransmittances() << std::endl;
  file << object->GetUpwardDiffuseTransmittancesForRayleigh() << std::endl;
  file << object->GetUpwardDiffuseTransmittancesForAerosol() << std::endl;
  file << object->GetWavelengthSpectralBand() << std::endl;

  file << "\n\n TESTING ACCESSOR WITH INDEX\n\n";
  file << object->GetIntrinsicAtmosphericReflectance(0) << std::endl;
  file << object->GetSphericalAlbedo(5) << std::endl;
  file << object->GetTotalGaseousTransmission(5) << std::endl;
  file << object->GetDownwardTransmittance(5) << std::endl;
  file << object->GetUpwardTransmittance(5) << std::endl;
  file << object->GetUpwardDiffuseTransmittance(5) << std::endl;
  file << object->GetUpwardDirectTransmittance(5) << std::endl;
  file << object->GetUpwardDiffuseTransmittanceForRayleigh(5) << std::endl;
  file << object->GetUpwardDiffuseTransmittanceForAerosol(5) << std::endl;
  file << object->GetWavelengthSpectralBand(12) << std::endl;
  file << std::endl;
  file << object->GetValueByIndex(5) << std::endl;


  file.close();

  return EXIT_SUCCESS;
}
