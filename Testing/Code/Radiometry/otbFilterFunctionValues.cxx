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


#include "itkExceptionObject.h"
#include "otbAtmosphericCorrectionParameters.h"
#include "otbSIXSTraits.h"
#include "base/ossimFilename.h"
#include <fstream>
#include <cstdlib>
#include "otbObjectList.h"

int otbFilterFunctionValuesTest(int argc, char * argv[])
{
  char *       outname   = argv[1];
  const double SIXSStepOfWavelengthSpectralBandValues(0.0025);

  typedef otb::FilterFunctionValues FilterFunctionValuesType;
  // Instantiating object
  FilterFunctionValuesType::Pointer          object = FilterFunctionValuesType::New();
  FilterFunctionValuesType::ValuesVectorType vect;

  for (int i = 5; i < argc; i++)
    {
    vect.push_back(atof(argv[i]));
    }

  object->SetMinSpectralValue(atof(argv[2]));
  object->SetMaxSpectralValue(atof(argv[3]));
  object->SetUserStep(atof(argv[4]));
  object->SetFilterFunctionValues(vect);

  // Call interpolate
  otb::SIXSTraits::ComputeWavelengthSpectralBandValuesFor6S(
      SIXSStepOfWavelengthSpectralBandValues,
      object);

  // Writing output file
  std::ofstream file;
  file.open(outname);

  file << "Input Vector :" << std::endl;
  for (unsigned int i = 0; i < vect.size(); i++)
    {
    file << vect[i] << std::endl;
    }
  file << std::endl;
  file << "Output vector :" << std::endl;
  for (unsigned int i = 0; i < object->GetFilterFunctionValues6S().size(); i++)
    {
    file << object->GetFilterFunctionValues6S()[i] << std::endl;
    }
  file << std::endl;
  file << "L_min :" << object->GetMinSpectralValue() << std::endl;
  file << "L_max :" << object->GetMaxSpectralValue() << std::endl;

  file.close();

  return EXIT_SUCCESS;
}

int otbFilterFunctionValuesDigitalGlobeTest(int argc, char * argv[])
{
  const char *       infname   = argv[1];
  const char *       outname   = argv[2];
  const double       userStep  = atof(argv[3]);
  const double wavelengthFactor = atof(argv[4]);
  const double SIXSStepOfWavelengthSpectralBandValues(0.0025);

  typedef otb::FilterFunctionValues FilterFunctionValuesType;
  typedef FilterFunctionValuesType::ValuesVectorType ValuesVectorType;
  typedef otb::ObjectList<FilterFunctionValuesType>          WavelengthSpectralBandVectorType;
  // Instantiate object
  //WavelengthSpectralBandVectorType::Pointer         filterFunctionList  = WavelengthSpectralBandVectorType::New();
  WavelengthSpectralBandVectorType::Pointer wavelengthSpectralBand = WavelengthSpectralBandVectorType::New();

  ossimFilename fname(infname);
  //if (!fname.exists()) itkExceptionMacro(<< infname << " does not exist.");

  std::ifstream file(fname.c_str());
  if (!file) return EXIT_FAILURE;

  std::string line;
  ossimString separator = '\t';
  double      mini = 0.;
  double      maxi = 0.;
  bool        firstLine = true;

  unsigned int nbBands = 0;
  // used to store the coef
  std::vector<ValuesVectorType> valuesVector;

  while (std::getline(file, line))
    {
    ossimString osLine(line);

    // Suppress multiple spaces
    for (unsigned int i = 0; i < osLine.size() - 1; i++)
      {
      if (osLine.compare(i, 1, " ") == 0 && osLine.compare(i + 1, 1, " ") == 0)
        {
        osLine.erase(i + 1, 1);
        i--;
        }
      }

    // if the first character is a space, erase it
    if (osLine.compare(0, 1, " ") == 0) osLine.erase(0, 1);

    std::vector<ossimString> keywordStrings = osLine.split(separator);

    //if (keywordStrings.size() < 3) itkExceptionMacro(<< "Invalid file format");

    // Store min wavelength
    if (firstLine)
      {
      mini = keywordStrings[0].toDouble() * wavelengthFactor;
      nbBands = keywordStrings.size() - 1;
      std::cout << "firstline " << " mini " << mini <<  " nbBands "<< nbBands << std::endl;
      std::cout << "keywordStrings[nbBands] "  << keywordStrings[nbBands] << std::endl;
      for (unsigned int j = 0; j < nbBands; j++)
        {
        wavelengthSpectralBand->PushBack(FilterFunctionValuesType::New());
        ValuesVectorType temp;

        valuesVector.push_back(temp);
        }
      firstLine = false;
      }

    //if (nbBands != keywordStrings.size() - 1) itkExceptionMacro(<< "Invalid file format");


    //std::cout<< "get multi" << std::endl;
    for (unsigned int i = 0; i < nbBands; ++i)
      {
      valuesVector[i].push_back(keywordStrings[i + 1].toDouble());
      //std::cout << "vector values " << keywordStrings[i + 1].toDouble() << " ";
      }


    maxi = keywordStrings[0].toDouble() * wavelengthFactor;
    std::cout << " maxi " << maxi << std::endl;
    std::cout << " vector values[0] size " << valuesVector[0].size() << std::endl;
    }

  // Call interpolate


  // Writing output file
  std::ofstream outputFile;
  outputFile.open(outname);

  //outputFile << "Input Vector filename: " << infname << std::endl;
  //outputFile << std::endl;
  //outputFile << "Output vector :" << std::endl;

  for (unsigned int j = 0; j < nbBands; j++)
    {
    wavelengthSpectralBand->GetNthElement(j)->SetFilterFunctionValues(valuesVector[j]);
    wavelengthSpectralBand->GetNthElement(j)->SetMinSpectralValue(mini);
    wavelengthSpectralBand->GetNthElement(j)->SetMaxSpectralValue(maxi);
    wavelengthSpectralBand->GetNthElement(j)->SetUserStep(userStep);

    otb::SIXSTraits::ComputeWavelengthSpectralBandValuesFor6S(
        SIXSStepOfWavelengthSpectralBandValues,
        wavelengthSpectralBand->GetNthElement(j));

    outputFile << "L_min :" << wavelengthSpectralBand->GetNthElement(j)->GetMinSpectralValue() << std::endl;
    outputFile << "L_max :" << wavelengthSpectralBand->GetNthElement(j)->GetMaxSpectralValue() << std::endl;

    outputFile << std::endl;

    for (unsigned int i = 0; i < wavelengthSpectralBand->GetNthElement(j)->GetFilterFunctionValues6S().size(); i++)
      {
      outputFile << wavelengthSpectralBand->GetNthElement(j)->GetFilterFunctionValues6S()[i] << std::endl;
      }
    outputFile << std::endl;

    }

  outputFile.close();

  return EXIT_SUCCESS;
}
