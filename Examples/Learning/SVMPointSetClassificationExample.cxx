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


//  Software Guide : BeginCommandLineArgs
//    INPUTS: {svm_model.svn}
//    OUTPUTS:
//  Software Guide : EndCommandLineArgs

#include "itkMacro.h"
#include "itkPointSet.h"
#include <iostream>
#include <cstdlib>

//  Software Guide : BeginLatex
// This example illustrates the use of the
// \doxygen{otb}{SVMClassifier} class for performing SVM
// classification on pointsets.
// The first thing to do is include the header file for the
// class. Since the \doxygen{otb}{SVMClassifier} takes
// \doxygen{itk}{ListSample}s as input, the class
// \doxygen{itk}{PointSetToListSampleAdaptor} is needed.
//
// We start by including the needed header files.
//
//  Software Guide : EndLatex

//  Software Guide : BeginCodeSnippet
#include "itkPointSetToListSampleAdaptor.h"
#include "itkListSample.h"
#include "otbSVMClassifier.h"
//  Software Guide : EndCodeSnippet

int main(int itkNotUsed(argc), char* argv[])
{
// Software Guide : BeginLatex
//
// In the framework of supervised learning and classification, we will
// always use feature vectors for the characterization of the
// classes. On the other hand, the class labels are scalar
// values. Here, we start by defining the type of the features as the
// \code{PixelType}, which will be used to define the feature
// \code{VectorType}. We also declare the type for the labels.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef float InputPixelType;

  typedef std::vector<InputPixelType> InputVectorType;
  typedef int                         LabelPixelType;
// Software Guide : EndCodeSnippet
  const unsigned int Dimension = 2;

// Software Guide : BeginLatex
//
// We can now proceed to define the point sets used for storing the
// features and the labels.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef itk::PointSet<InputVectorType,  Dimension> MeasurePointSetType;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We will need to get access to the data stored in the point sets, so
// we define the appropriate for the points and the points containers
// used by the point sets (see the section \ref{sec:PointSetSection}
// for more information on how to use point sets).
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef MeasurePointSetType::PointType MeasurePointType;
  typedef MeasurePointSetType::PointsContainer MeasurePointsContainer;

  MeasurePointSetType::Pointer    tPSet = MeasurePointSetType::New();
  MeasurePointsContainer::Pointer tCont = MeasurePointsContainer::New();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We need now to build the test set for the SVM. In this
// simple example, we will build a SVM who classes points depending on
// which side of the line $x=y$ they are located. We start by
// generating 500 random points.
//
// Software Guide : EndLatex

  srand(0);

  unsigned int pointId;
// Software Guide : BeginCodeSnippet
  int lowest = 0;
  int range = 1000;

  for (pointId = 0; pointId < 100; pointId++)
    {

    MeasurePointType tP;

    int x_coord = lowest + static_cast<int>(range * (rand() / (RAND_MAX + 1.0)));
    int y_coord = lowest + static_cast<int>(range * (rand() / (RAND_MAX + 1.0)));

    std::cout << "coords : " << x_coord << " " << y_coord << std::endl;
    tP[0] = x_coord;
    tP[1] = y_coord;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We push the features in the vector after a normalization which is
// useful for SVM convergence.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
    InputVectorType measure;
    measure.push_back(static_cast<InputPixelType>((x_coord * 1.0 -
                                                   lowest) / range));
    measure.push_back(static_cast<InputPixelType>((y_coord * 1.0 -
                                                   lowest) / range));
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// And we insert the points in the points container.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
    tCont->InsertElement(pointId, tP);
    tPSet->SetPointData(pointId, measure);

    }
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// After the loop, we set the points container to the point set.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  tPSet->SetPoints(tCont);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Once the pointset is ready, we must transform it to a sample which
// is compatible with the classification framework. We will use a
// \doxygen{itk}{Statistics::PointSetToListSampleAdaptor} for this
// task. This class is templated over the point set type used for
// storing the measures.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef itk::Statistics::PointSetToListSampleAdaptor<MeasurePointSetType>
  SampleType;
  SampleType::Pointer sample = SampleType::New();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// After instantiation, we can set the point set as an imput of our
// sample adaptor.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  sample->SetPointSet(tPSet);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Now, we need to declare the SVM model which is to be used by the
// classifier. The SVM model is templated over the type of value used
// for the measures and the type of pixel used for the labels.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::SVMModel<SampleType::MeasurementVectorType::ValueType,
      LabelPixelType> ModelType;

  ModelType::Pointer model = ModelType::New();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// After instantiation, we can load a model saved to a file (see
// section \ref{sec:LearningWithPointSets} for an example of model
// estimation and storage to a file).
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  model->LoadModel(argv[1]);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We have now all the elements to create a classifier. The classifier
// is templated over the sample type (the type of the data to be
// classified) and the label type (the type of the output of the classifier).
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::SVMClassifier<SampleType, LabelPixelType> ClassifierType;

  ClassifierType::Pointer classifier = ClassifierType::New();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We set the classifier parameters : number of classes, SVM model,
// the sample data. And we trigger the classification process by
// calling the \code{Update} method.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  int numberOfClasses = model->GetNumberOfClasses();
  classifier->SetNumberOfClasses(numberOfClasses);
  classifier->SetModel(model);
  classifier->SetInput(sample.GetPointer());
  classifier->Update();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// After the classification step, we usually want to get the
// results. The classifier gives an output under the form of a sample
// list. This list supports the classical STL iterators.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ClassifierType::OutputType* membershipSample =
    classifier->GetOutput();

  ClassifierType::OutputType::ConstIterator m_iter =
    membershipSample->Begin();
  ClassifierType::OutputType::ConstIterator m_last =
    membershipSample->End();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We will iterate through the list, get the labels and compute the
// classification error.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  double error = 0.0;
  pointId = 0;
  while (m_iter != m_last)
    {
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We get the label for each point.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
    ClassifierType::ClassLabelType label = m_iter.GetClassLabel();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// And we compare it to the corresponding one of the test set.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
    InputVectorType measure;

    tPSet->GetPointData(pointId, &measure);

    ClassifierType::ClassLabelType expectedLabel;
    if (measure[0] < measure[1]) expectedLabel = -1;
    else expectedLabel = 1;

    double dist = fabs(measure[0] - measure[1]);

    if (label != expectedLabel) error++;

    std::cout << int(label) << "/" << int(expectedLabel) << " --- " << dist <<
    std::endl;

    ++pointId;
    ++m_iter;
    }

  std::cout << "Error = " << error / pointId << " % " << std::endl;
// Software Guide : EndCodeSnippet

  return EXIT_SUCCESS;
}
