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
#include "itkPointSet.h"
#include <iostream>

#include "otbSVMPointSetModelEstimator.h"

int otbSVMPointSetModelEstimatorTrain(int itkNotUsed(argc), char* argv[])
{
  const char* outputModelFileName = argv[1];

  typedef std::vector<double> InputPixelType;
  typedef double              LabelPixelType;
  const unsigned int Dimension = 2;

  typedef itk::PointSet<InputPixelType,  Dimension> MeasurePointSetType;
  typedef itk::PointSet<LabelPixelType,  Dimension> LabelPointSetType;
  typedef MeasurePointSetType::PointType            MeasurePointType;
  typedef LabelPointSetType::PointType              LabelPointType;
  typedef MeasurePointSetType::PointsContainer      MeasurePointsContainer;
  typedef LabelPointSetType::PointsContainer        LabelPointsContainer;

  MeasurePointSetType::Pointer    mPSet = MeasurePointSetType::New();
  LabelPointSetType::Pointer      lPSet = LabelPointSetType::New();
  MeasurePointsContainer::Pointer mCont = MeasurePointsContainer::New();
  LabelPointsContainer::Pointer   lCont = LabelPointsContainer::New();

  for (unsigned int pointId = 0; pointId < 20; pointId++)
    {
    MeasurePointType mP;
    LabelPointType   lP;

    mP[0] = pointId;
    mP[1] = pointId;

    lP[0] = pointId;
    lP[1] = pointId;

    InputPixelType measure;
    measure.push_back(double(2.0 * pointId));
    measure.push_back(double(-10));

    LabelPixelType label = static_cast<LabelPixelType>((measure[0] + measure[1]) > 0); //2x-10>0

    mCont->InsertElement(pointId, mP);
    mPSet->SetPointData(pointId, measure);

    lCont->InsertElement(pointId, lP);
    lPSet->SetPointData(pointId, label);
    }

  mPSet->SetPoints(mCont);
  lPSet->SetPoints(lCont);

  typedef otb::SVMPointSetModelEstimator<MeasurePointSetType, LabelPointSetType> EstimatorType;
  EstimatorType::Pointer estimator = EstimatorType::New();

  estimator->SetInputPointSet(mPSet);
  estimator->SetTrainingPointSet(lPSet);
  estimator->ParametersOptimizationOff();

  estimator->Update();

  estimator->GetModel()->SaveModel(outputModelFileName);

  return EXIT_SUCCESS;
}
