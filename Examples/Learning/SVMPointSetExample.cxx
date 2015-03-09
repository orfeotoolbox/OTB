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
#include <iostream>
#include <cstdlib>

#include "otbSVMPointSetModelEstimator.h"
#include "itkPointSetToListSampleAdaptor.h"
#include "otbSVMClassifier.h"

int main(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{

  typedef float InputPixelType;

  typedef std::vector<InputPixelType> InputVectorType;
  typedef int                         LabelPixelType;
  const unsigned int Dimension = 2;

  typedef itk::PointSet<InputVectorType,  Dimension> MeasurePointSetType;

  typedef itk::PointSet<LabelPixelType,  Dimension> LabelPointSetType;

  MeasurePointSetType::Pointer mPSet = MeasurePointSetType::New();
  LabelPointSetType::Pointer   lPSet = LabelPointSetType::New();

  typedef MeasurePointSetType::PointType MeasurePointType;
  typedef LabelPointSetType::PointType   LabelPointType;

  typedef MeasurePointSetType::PointsContainer MeasurePointsContainer;
  typedef LabelPointSetType::PointsContainer   LabelPointsContainer;

  MeasurePointsContainer::Pointer mCont = MeasurePointsContainer::New();
  LabelPointsContainer::Pointer   lCont = LabelPointsContainer::New();

  /* We learn the y>x | y<x boundary*/
//  srand((unsigned)time(0));
  srand(0);
  int lowest = 0;
  int range = 1000;

  unsigned int pointId;

  for (pointId = 0; pointId < 500; pointId++)
    {

    MeasurePointType mP;
    LabelPointType   lP;

    int x_coord = lowest + static_cast<int>(range * (rand() / (RAND_MAX + 1.0)));
    int y_coord = lowest + static_cast<int>(range * (rand() / (RAND_MAX + 1.0)));

    std::cout << "coords : " << x_coord << " " << y_coord << std::endl;
    mP[0] = x_coord;
    mP[1] = y_coord;

    lP[0] = x_coord;
    lP[1] = y_coord;

    InputVectorType measure;
    measure.push_back(static_cast<InputPixelType>((x_coord * 1.0 -
                                                   lowest) / range));
    measure.push_back(static_cast<InputPixelType>((y_coord * 1.0 -
                                                   lowest) / range));

    LabelPixelType label;

    if (x_coord < y_coord) label = 0;
    else label = 1;

    std::cout << "Label : " << label << std::endl;
    std::cout << "Measures : " << measure[0] << " " << measure[1] << std::endl;

    mCont->InsertElement(pointId, mP);
    mPSet->SetPointData(pointId, measure);

    lCont->InsertElement(pointId, lP);
    lPSet->SetPointData(pointId, label);

    }

  mPSet->SetPoints(mCont);
  lPSet->SetPoints(lCont);

  typedef otb::SVMPointSetModelEstimator<MeasurePointSetType,
      LabelPointSetType>   EstimatorType;

  EstimatorType::Pointer estimator = EstimatorType::New();

  estimator->SetInputPointSet(mPSet);
  estimator->SetTrainingPointSet(lPSet);

  estimator->Update();

  std::cout << "Saving model" << std::endl;
  estimator->SaveModel("model.svm");

  // Build the test set

  MeasurePointSetType::Pointer    tPSet = MeasurePointSetType::New();
  MeasurePointsContainer::Pointer tCont = MeasurePointsContainer::New();

  for (pointId = 0; pointId < 100; pointId++)
    {

    MeasurePointType tP;

    int x_coord = lowest + static_cast<int>(range * (rand() / (RAND_MAX + 1.0)));
    int y_coord = lowest + static_cast<int>(range * (rand() / (RAND_MAX + 1.0)));

    std::cout << "coords : " << x_coord << " " << y_coord << std::endl;
    tP[0] = x_coord;
    tP[1] = y_coord;

    InputVectorType measure;
    measure.push_back(static_cast<InputPixelType>((x_coord * 1.0 -
                                                   lowest) / range));
    measure.push_back(static_cast<InputPixelType>((y_coord * 1.0 -
                                                   lowest) / range));

    std::cout << "Measures : " << measure[0] << " " << measure[1] << std::endl;

    tCont->InsertElement(pointId, tP);
    tPSet->SetPointData(pointId, measure);

    }

  tPSet->SetPoints(tCont);

  // Classify

  typedef itk::Statistics::PointSetToListSampleAdaptor<MeasurePointSetType>
  SampleType;
  SampleType::Pointer sample = SampleType::New();
  sample->SetPointSet(tPSet);

  std::cout << "Sample set to Adaptor" << std::endl;

  /** preparing classifier and decision rule object */
  typedef otb::SVMModel<SampleType::MeasurementVectorType::ValueType,
      LabelPixelType> ModelType;

  ModelType::Pointer model = estimator->GetModel();

  int numberOfClasses = model->GetNumberOfClasses();

  std::cout << "Classification for " << numberOfClasses << " classes " <<
  std::endl;

  typedef otb::SVMClassifier<SampleType, LabelPixelType> ClassifierType;

  ClassifierType::Pointer classifier = ClassifierType::New();

  classifier->SetNumberOfClasses(numberOfClasses);
  classifier->SetModel(model);
  classifier->SetInput(sample.GetPointer());
  classifier->Update();

  /* Build the class map */
  std::cout << "Output image creation" << std::endl;

  std::cout << "classifier get output" << std::endl;
  ClassifierType::OutputType* membershipSample =
    classifier->GetOutput();
  std::cout << "Sample iterators" << std::endl;
  ClassifierType::OutputType::ConstIterator m_iter =
    membershipSample->Begin();
  ClassifierType::OutputType::ConstIterator m_last =
    membershipSample->End();

  double error = 0.0;
  pointId = 0;
  while (m_iter != m_last)
    {
    ClassifierType::ClassLabelType label = m_iter.GetClassLabel();

    InputVectorType measure;

    tPSet->GetPointData(pointId, &measure);

    ClassifierType::ClassLabelType expectedLabel;
    if (measure[0] < measure[1]) expectedLabel = 0;
    else expectedLabel = 1;

    double dist = fabs(measure[0] - measure[1]);

    if (label != expectedLabel) error++;

    std::cout << int(label) << "/" << int(expectedLabel) << " --- " << dist <<
    std::endl;

    ++pointId;
    ++m_iter;
    }

  std::cout << "Error = " << error / pointId << std::endl;

  return EXIT_SUCCESS;
}
