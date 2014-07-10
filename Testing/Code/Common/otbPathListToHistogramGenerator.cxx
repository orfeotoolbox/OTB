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



#include "itkMacro.h"
#include "itkPolyLineParametricPath.h"
#include "otbOrientationPathFunction.h"
#include "otbPathListToHistogramGenerator.h"
#include "otbMath.h"
#include "otbObjectList.h"

int otbPathListToHistogramGenerator(int itkNotUsed(argc), char * argv[])
{
  unsigned int       NbOfBins((unsigned int) ::atoi(argv[1]));
  unsigned int       NbOfPointsPerHistogram((unsigned int) ::atoi(argv[2]));
  const unsigned int Dimension = 2;
  typedef itk::PolyLineParametricPath<Dimension> PathType;
  typedef PathType::Pointer                      PathPointer;
  typedef PathType::ConstPointer                 PathConstPointerType;
  typedef otb::ObjectList<PathType>              PathListType;

  typedef otb::OrientationPathFunction<PathType> FunctionType;

  typedef otb::PathListToHistogramGenerator<PathType, FunctionType> HistogramGeneratorType;

  PathType::ContinuousIndexType cindex;
  int                           NbAngle = NbOfPointsPerHistogram * NbOfBins;

  /* build segments list */
  PathListType::Pointer PathList = PathListType::New();
  PathList->Clear();

  for (int i = 0; i < NbAngle; ++i)
    {
    PathPointer pathElt = PathType::New();
    pathElt->Initialize();
    cindex[0] = 30;
    cindex[1] = 30;
    pathElt->AddVertex(cindex);

    float Theta = 2.0 * static_cast<float>(otb::CONST_PI) * static_cast<float>(i) / static_cast<float>(NbAngle);
    cindex[0] = 30 + vcl_cos(Theta);
    cindex[1] = 30 + vcl_sin(Theta);
    pathElt->AddVertex(cindex);

    PathList->PushBack(pathElt);
    }

  HistogramGeneratorType::Pointer histogramGenerator = HistogramGeneratorType::New();

  typedef HistogramGeneratorType::SizeType HistogramSizeType;
  HistogramSizeType hsize(1);
  hsize[0] = NbOfBins;  // number of bins for the Red   channel

  histogramGenerator->SetInput(PathList);
  histogramGenerator->SetNumberOfBins(hsize);
  histogramGenerator->Update();

  typedef HistogramGeneratorType::HistogramType HistogramType;

  const HistogramType * histogram = histogramGenerator->GetOutput();

  const unsigned int histogramSize = histogram->Size();
  std::cout << "Histogram size " << histogramSize << std::endl;

  for (unsigned int bin = 0; bin < histogramSize; bin++)
    {
    if (histogram->GetFrequency(bin, 0) != NbOfPointsPerHistogram)
      {
      std::cout << "Error in histogram value !" << std::endl;
      return EXIT_FAILURE;
      }
    std::cout << "bin = " << bin << " frequency = ";
    std::cout << histogram->GetFrequency(bin, 0) << std::endl;
    }

  return EXIT_SUCCESS;
}
