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


#include "otbMRFSamplerRandomMAP.h"
#include "otbImageFileReader.h"
#include "otbImage.h"
#include "otbMRFEnergyPotts.h"

#include <fstream>

int otbMRFSamplerRandomMAP(int itkNotUsed(argc), char * argv[])
{
  const char * inputImage = argv[1];
  const char * labelImage = argv[2];
  const char * outputFile = argv[3];

  typedef double                                         PixelTypeInput;
  typedef int                                            PixelTypeLabel;
  typedef otb::Image<PixelTypeInput, 2>                  ImageType;
  typedef otb::Image<PixelTypeLabel, 2>                  LabelType;
  typedef otb::ImageFileReader<ImageType>                ReaderInputType;
  typedef otb::ImageFileReader<LabelType>                ReaderLabelType;
  typedef otb::MRFSamplerRandomMAP<ImageType, LabelType> MRFSamplerRandomMAPType;

  typedef otb::MRFEnergyPotts <ImageType, LabelType> EnergyFidelityType;
  typedef otb::MRFEnergyPotts <LabelType, LabelType> EnergyRegularizationType;

  typedef MRFSamplerRandomMAPType::LabelledImageNeighborhoodIterator LabelledNeighborhoodIterator;
  typedef MRFSamplerRandomMAPType::InputImageNeighborhoodIterator    InputNeighborhoodIterator;

  MRFSamplerRandomMAPType::Pointer  object               = MRFSamplerRandomMAPType::New();
  EnergyRegularizationType::Pointer energyRegularization = EnergyRegularizationType::New();
  EnergyFidelityType::Pointer       energyFidelity       = EnergyFidelityType::New();
  ReaderInputType::Pointer          readerIn             = ReaderInputType::New();
  ReaderLabelType::Pointer          readerLab            = ReaderLabelType::New();

  readerIn->SetFileName(inputImage);
  readerLab->SetFileName(labelImage);
  readerIn->Update();
  readerLab->Update();

  object->SetNumberOfClasses(1);
  object->SetEnergyFidelity(energyFidelity);
  object->SetEnergyRegularization(energyRegularization);
  object->InitializeSeed(0); // USED TO OVERPASS RANDOM CALCULATION

  ImageType::IndexType idIn;
  LabelType::IndexType idLab;
  idIn[0] = 50;
  idIn[1] = 50;
  idLab[0] = 70;
  idLab[1] = 70;
  ImageType::PixelType inPix = readerIn->GetOutput()->GetPixel(idIn);
  LabelType::PixelType inLab = readerLab->GetOutput()->GetPixel(idLab);

  InputNeighborhoodIterator::RadiusType    radIn;
  LabelledNeighborhoodIterator::RadiusType radLab;
  radIn.Fill(3);
  radLab.Fill(3);

  InputNeighborhoodIterator iterIn  = InputNeighborhoodIterator(radIn, readerIn->GetOutput(),
                                                                readerIn->GetOutput()->GetLargestPossibleRegion());
  LabelledNeighborhoodIterator iterLab = LabelledNeighborhoodIterator(radLab,
                                                                      readerLab->GetOutput(),
                                                                      readerLab->GetOutput()->GetLargestPossibleRegion());

  std::ofstream file;
  file.open(outputFile);
  file << "Used pixels: (50, 50) -> " << inPix << " , (70, 70) -> " << inLab << std::endl;
  file << std::endl;
  file << "Compute( Compute( const InputImageNeighborhoodIterator, const LabelledImageNeighborhoodIterator) " <<
  object->Compute(iterIn, iterLab) << std::endl;

  // All values (exept m_Value) are null : SingleValue return 0...
  file << "m_EnergyBefore: " << object->GetEnergyBefore() << std::endl;
  file << "m_EnergyAfter: " << object->GetEnergyAfter() << std::endl;
  file << "m_Value: " << object->GetValue() << std::endl;
  file << "m_DeltaEnergy: " << object->GetDeltaEnergy() << std::endl;

  file.close();

  return EXIT_SUCCESS;
}
