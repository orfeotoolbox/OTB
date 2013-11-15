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
#include "otbVectorImage.h"
#include "itkListSample.h"
#include "otbListSampleToHistogramListGenerator.h"
#include "otbImageFileReader.h"
#include "itkImageRegionIterator.h"

int otbListSampleToHistogramListGenerator(int argc, char * argv[])
{
  typedef double                                       PixelType;
  typedef otb::VectorImage<PixelType>                  VectorImageType;
  typedef VectorImageType::PixelType                   VectorPixelType;
  typedef VectorImageType::SizeType                    SizeType;
  typedef itk::Statistics::ListSample<VectorPixelType> ListSampleType;
  typedef otb::ListSampleToHistogramListGenerator
  <ListSampleType, PixelType>                         HistogramGeneratorType;
  typedef otb::ImageFileReader<VectorImageType> ReaderType;

  // Instantiation
  ReaderType::Pointer             reader    = ReaderType::New();
  HistogramGeneratorType::Pointer generator = HistogramGeneratorType::New();
  ListSampleType::Pointer         ls = ListSampleType::New();

  reader->SetFileName(argv[1]);
  reader->Update();

  // Set the size of the ListSample Measurement Vector
  ls->SetMeasurementVectorSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());

  itk::ImageRegionConstIterator<VectorImageType> it(reader->GetOutput(), reader->GetOutput()->GetLargestPossibleRegion());

  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
    ls->PushBack(it.Get());
    }

  float        mscale = atof(argv[4]);
  unsigned int nbBins = atoi(argv[3]);

  generator->SetListSample(ls);
  generator->SetNumberOfBins(nbBins);
  generator->SetMarginalScale(mscale);
  generator->Update();

  std::ofstream ofs;
  ofs.open(argv[2]);
  for (unsigned int comp = 0; comp < reader->GetOutput()->GetNumberOfComponentsPerPixel(); ++comp)
    {
    ofs << "Channel: " << comp << " histogram: " << std::endl;
    for (unsigned int bin = 0; bin < nbBins; ++bin)
      {
      ofs << generator->GetOutput()->GetNthElement(comp)->GetFrequency(bin) << "\t";
      }
    ofs << std::endl;
    }

  ofs.close();

  return EXIT_SUCCESS;
}
