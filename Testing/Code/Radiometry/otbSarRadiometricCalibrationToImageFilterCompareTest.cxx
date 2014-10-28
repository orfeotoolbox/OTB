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

#include "otbSarRadiometricCalibrationToImageFilter.h"
#include "otbImageFileReader.h"
#include "otbExtractROI.h"
#include "itkComplexToModulusImageFilter.h"
#include  "otbStreamingCompareImageFilter.h"

int otbSarRadiometricCalibrationToImageFilterCompareTest(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef float                                                                         RealType;
  typedef std::complex<RealType>                                                        PixelType;
  typedef otb::Image<PixelType, Dimension>                                              InputImageType;
  typedef otb::Image<RealType, Dimension>                                               OutputImageType;
  typedef otb::ImageFileReader<InputImageType>                                          ReaderType;
  typedef otb::SarRadiometricCalibrationToImageFilter<InputImageType, OutputImageType>  FilterComplexType;
  typedef otb::SarRadiometricCalibrationToImageFilter<OutputImageType, OutputImageType> FilterRealType;
  typedef otb::ExtractROI<RealType, RealType>                                           ExtractorType;
  typedef itk::ComplexToModulusImageFilter<InputImageType, OutputImageType>             ModulusType;
  typedef otb::StreamingCompareImageFilter<OutputImageType>                             CompareFilterType;

  // Instantiating object
  FilterComplexType::Pointer filterComplex = FilterComplexType::New();
  FilterRealType::Pointer filterReal = FilterRealType::New();
  ReaderType::Pointer reader = ReaderType::New();
  ExtractorType::Pointer extractorComplex = ExtractorType::New();
  ExtractorType::Pointer extractorReal = ExtractorType::New();
  ModulusType::Pointer modulus = ModulusType::New();
  CompareFilterType::Pointer compare = CompareFilterType::New();

  reader->SetFileName(argv[1]);
  filterComplex->SetInput(reader->GetOutput());

  modulus->SetInput(reader->GetOutput());
  filterReal->SetInput(modulus->GetOutput());

  filterComplex->UpdateOutputInformation();
  filterReal->UpdateOutputInformation();

  if (argc > 2)
    {
    // Generate an extract from the large input
    OutputImageType::RegionType region;
    OutputImageType::IndexType  id;
    id[0] = atoi(argv[2]);   id[1] = atoi(argv[3]);
    OutputImageType::SizeType size;
    size[0] = atoi(argv[4]);   size[1] = atoi(argv[5]);
    region.SetIndex(id);
    region.SetSize(size);

    extractorComplex->SetExtractionRegion(region);
    extractorComplex->SetInput(filterComplex->GetOutput());

    extractorReal->SetExtractionRegion(region);
    extractorReal->SetInput(filterReal->GetOutput());

    compare->SetInput1(extractorComplex->GetOutput());
    compare->SetInput2(extractorReal->GetOutput());
    }
  else
    {
    compare->SetInput1(filterComplex->GetOutput());
    compare->SetInput2(filterReal->GetOutput());
    }

  compare->Update();
  if(compare->GetMAE() > 0.00000001)
    {
    std::cout << "MAE : " << compare->GetMAE() << std::endl;
    return EXIT_FAILURE;
    }


  return EXIT_SUCCESS;
}
