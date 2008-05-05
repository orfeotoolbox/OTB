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

#include "otbMRFEnergy.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include <fstream>

int otbMRFEnergy(int argc, char * argv[])
{
  const char * inputImage = argv[1];
  const char * labelImage = argv[2];
  const char * outputFile = argv[3];

  typedef double                                       PixelTypeInput;
  typedef int                                          PixelTypeLabel;
  typedef otb::Image<PixelTypeInput, 2>                ImageType;
  typedef otb::Image<PixelTypeLabel, 2>                LabelType;
  typedef otb::MRFEnergy< ImageType, LabelType>        MRFEnergyType;
  typedef otb::ImageFileReader< ImageType >            ReaderInputType;
  typedef otb::ImageFileReader< LabelType >            ReaderLabelType;
  typedef MRFEnergyType::InputImagePixelType           InputImagePixelType;
  typedef MRFEnergyType::LabelledImagePixelType        LabelledImagePixelType;
  typedef MRFEnergyType::LabelledNeighborhoodIterator  LabelledNeighborhoodIterator;
  typedef MRFEnergyType::InputNeighborhoodIterator     InputNeighborhoodIterator;
  typedef MRFEnergyType::ParametersType                ParametersType;
 
  MRFEnergyType::Pointer object = MRFEnergyType::New();
  ReaderInputType::Pointer     readerIn  = ReaderInputType::New();
  ReaderLabelType::Pointer     readerLab = ReaderLabelType::New();
 
  readerIn->SetFileName( inputImage );
  readerLab->SetFileName( labelImage );
  readerIn->Update();
  readerLab->Update();

  object->SetNumberOfParameters( 4 );

  ParametersType parameters;
  parameters.SetSize(4);
  parameters[0]=10.0; //Class 0 mean
  parameters[1]=10.0; //Class 0 stdev
  parameters[2]=80.0; //Class 1 mean
  parameters[3]=10.0; //Class 1 stdev

  object->SetParameters( parameters );

  ImageType::IndexType idIn;
  LabelType::IndexType idLab;
  idIn[0] = 50; 
  idIn[1] = 50;
  idLab[0] = 100; 
  idLab[1] = 100;
  InputImagePixelType    inPix = readerIn->GetOutput()->GetPixel( idIn );
  LabelledImagePixelType inLab = readerLab->GetOutput()->GetPixel( idLab );

  InputNeighborhoodIterator::RadiusType    radIn;
  radIn.Fill(3);  
  LabelledNeighborhoodIterator::RadiusType radLab;
  radLab.Fill(3);

  InputNeighborhoodIterator    iterIn  = InputNeighborhoodIterator( radIn, readerIn->GetOutput(), readerIn->GetOutput()->GetLargestPossibleRegion());

  LabelledNeighborhoodIterator iterLab = LabelledNeighborhoodIterator( radLab, readerLab->GetOutput(), readerLab->GetOutput()->GetLargestPossibleRegion());

  std::ofstream file;
  file.open(outputFile);
  file<<"Used pixels: (50, 50) -> "<<inPix<<" , (100, 100) -> "<<inLab<<std::endl;
  file<<std::endl;
  file<<"GetSingleValue(const InputImagePixelType,  const LabelledImagePixelType):   "<< object->GetValue(inPix, inLab)<<std::endl;
  file<<"GetValue(const InputImagePixelType, const LabelledImagePixelType):          "<< object->GetValue(inPix, inLab)<<std::endl;
  file<<"GetValue(const LabelledNeighborhoodIterator, const LabelledImagePixelType): "<< object->GetValue(iterLab, inLab)<<std::endl;
  file<<"GetValue(const InputNeighborhoodIterator,  const LabelledImagePixelType):   "<< object->GetValue(iterIn, inLab)<<std::endl;

  file.close();

  return EXIT_SUCCESS;
}

