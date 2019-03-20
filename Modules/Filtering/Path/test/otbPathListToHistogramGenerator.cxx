/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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




#include "otbMacro.h"
#include "itkPolyLineParametricPath.h"
#include "otbOrientationPathFunction.h"
#include "otbPathListToHistogramGenerator.h"
#include "otbMath.h"

int otbPathListToHistogramGenerator(int itkNotUsed(argc), char * argv[])
{
  unsigned int       NbOfBins((unsigned int) ::atoi(argv[1]));
  unsigned int       NbOfPointsPerHistogram((unsigned int) ::atoi(argv[2]));
  const unsigned int Dimension = 2;
  typedef itk::PolyLineParametricPath<Dimension> PathType;
  typedef PathType::Pointer                      PathPointer;
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
    cindex[0] = 30 + std::cos(Theta);
    cindex[1] = 30 + std::sin(Theta);
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
  otbLogMacro(Info, << "Histogram size " << histogramSize);

  for (unsigned int bin = 0; bin < histogramSize; bin++)
    {
    if (histogram->GetFrequency(bin, 0) != NbOfPointsPerHistogram)
      {
      otbLogMacro(Warning, "Error in histogram value !");
      return EXIT_FAILURE;
      }
    otbLogMacro(Debug, << "bin = " << bin << " frequency = " << histogram->GetFrequency(bin, 0));
    }

  return EXIT_SUCCESS;
}
