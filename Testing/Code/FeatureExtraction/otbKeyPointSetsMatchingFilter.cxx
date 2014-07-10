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


#include "otbKeyPointSetsMatchingFilter.h"

#include "itkVariableLengthVector.h"
#include "itkPointSet.h"

#include <iostream>
#include <fstream>

int otbKeyPointSetsMatchingFilter(int itkNotUsed(argc), char* argv[])
{

  const char * outfname = argv[1];
  const double thresh        = atof(argv[2]);
  const bool   useBackMatching = atoi(argv[3]);

  typedef itk::VariableLengthVector<double>             PointDataType;
  typedef itk::PointSet<PointDataType, 2>               PointSetType;
  typedef PointSetType::PointType                       PointType;
  typedef otb::KeyPointSetsMatchingFilter<PointSetType> MatchingFilterType;
  typedef MatchingFilterType::LandmarkListType          LandmarkListType;

  // instantiation
  MatchingFilterType::Pointer filter = MatchingFilterType::New();

  filter->SetUseBackMatching(useBackMatching);
  filter->SetDistanceThreshold(thresh);

  // Building two pointsets
  PointSetType::Pointer ps1 = PointSetType::New();
  PointSetType::Pointer ps2 = PointSetType::New();

  PointType p1, p2, p3;

  p1.Fill(1);
  p2.Fill(2);
  p3.Fill(3);

  PointDataType d1(3), d2(3), d3(3), d1b(3), d2b(3), d3b(3);

  d1.Fill(1);
  d1b.Fill(1);

  d2.Fill(0);
  d2[0] = 10;
  d2b.Fill(0);
  d2b[1] = 10;

  d3.Fill(2);
  d3b.Fill(10);

  ps1->SetPoint(0, p1);
  ps1->SetPoint(1, p2);
  ps1->SetPoint(2, p3);

  ps2->SetPoint(0, p1);
  ps2->SetPoint(1, p2);
  ps2->SetPoint(2, p3);

  ps1->SetPointData(0, d1);
  ps1->SetPointData(1, d2);
  ps1->SetPointData(2, d3);

  ps2->SetPointData(0, d1b);
  ps2->SetPointData(1, d2b);
  ps2->SetPointData(2, d3b);

  filter->SetInput1(ps1);
  filter->SetInput2(ps2);

  filter->Update();

  LandmarkListType * matching = filter->GetOutput();

  std::ofstream outfile(outfname);
  outfile << "Matches: " << std::endl;

  for (LandmarkListType::Iterator it = matching->Begin(); it != matching->End(); ++it)
    {
    outfile << "Matching: " << it.Get()->GetPoint1() << " " << it.Get()->GetPointData1() << " <- " <<
    it.Get()->GetLandmarkData() << " -> " << it.Get()->GetPoint2() << " " << it.Get()->GetPointData2() << std::endl;
    }

  outfile.close();

  return EXIT_SUCCESS;
}
