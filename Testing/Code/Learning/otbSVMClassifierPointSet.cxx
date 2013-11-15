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



#include <fstream>

#include "itkPoint.h"
#include "itkPointSet.h"

#include "itkPointSetToListSampleAdaptor.h"
#include "itkSubsample.h"
#include "itkListSample.h"
#include "otbSVMClassifier.h"

int otbSVMClassifierPointSet(int argc, char* argv[])
{
  if (argc != 2)
    {
    std::cout << "Usage : " << argv[0] << " modelFile"
              << std::endl;
    return EXIT_FAILURE;
    }

  const char * modelFilename  = argv[1];

  std::cout << "Building the pointset" << std::endl;

  typedef double                      InputPixelType;
  typedef int                         LabelPixelType;
  typedef std::vector<InputPixelType> InputVectorType;
  const unsigned int Dimension = 2;

  typedef itk::PointSet<InputVectorType,  Dimension>
  MeasurePointSetType;

  MeasurePointSetType::Pointer mPSet = MeasurePointSetType::New();

  typedef MeasurePointSetType::PointType MeasurePointType;

  typedef MeasurePointSetType::PointsContainer MeasurePointsContainer;

  MeasurePointsContainer::Pointer mCont = MeasurePointsContainer::New();

  unsigned int pointId;

  for (pointId = 0; pointId < 20; pointId++)
    {

    MeasurePointType mP;

    mP[0] = pointId;
    mP[1] = pointId;

    InputVectorType measure;
    //measure.push_back(vcl_pow(pointId, 2.0));
    measure.push_back(double(2.0 * pointId));
    measure.push_back(double(-10));

    mCont->InsertElement(pointId, mP);
    mPSet->SetPointData(pointId, measure);

    }

  mPSet->SetPoints(mCont);

  std::cout << "PointSet built" << std::endl;

  typedef itk::Statistics::PointSetToListSampleAdaptor<MeasurePointSetType>
  SampleType;
  SampleType::Pointer sample = SampleType::New();
  sample->SetPointSet(mPSet);

  std::cout << "Sample set to Adaptor" << std::endl;

  /** preparing classifier and decision rule object */
  typedef otb::SVMModel<SampleType::MeasurementVectorType::ValueType, LabelPixelType> ModelType;

  ModelType::Pointer model = ModelType::New();

  model->LoadModel(modelFilename);

  std::cout << "Model loaded" << std::endl;

  int numberOfClasses = model->GetNumberOfClasses();

  typedef otb::SVMClassifier<SampleType, LabelPixelType> ClassifierType;

  ClassifierType::Pointer classifier = ClassifierType::New();

  classifier->SetNumberOfClasses(numberOfClasses);
  classifier->SetModel(model);
  classifier->SetInput(sample.GetPointer());
  classifier->Update();

  /* Build the class map */

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

    mPSet->GetPointData(pointId, &measure);

    if (label != ((measure[0] + measure[1]) > 0)) error++;

    std::cout << label << "/" <<
    ((measure[0] + measure[1]) > 0) << std::endl;

    ++pointId;
    ++m_iter;
    }

  std::cout << "Error = " << error / pointId << std::endl;

  return EXIT_SUCCESS;
}
