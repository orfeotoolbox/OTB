/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
