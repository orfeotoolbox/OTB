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
#include <cstdlib>

// Software Guide : BeginLatex
//
// This example illustrates the use of the
// \doxygen{otb}{SVMPointSetModelEstimator} in order to perform the
// SVM learning from an \doxygen{itk}{PointSet} data structure.
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbSVMPointSetModelEstimator.h"
// Software Guide : EndCodeSnippet

int main(int itkNotUsed(argc), char* itkNotUsed(argv)[])
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
  typedef float                  PixelType;
  typedef std::vector<PixelType> VectorType;
  typedef int                    LabelPixelType;
// Software Guide : EndCodeSnippet
  const unsigned int Dimension = 2;

// Software Guide : BeginLatex
//
// We can now proceed to define the point sets used for storing the
// features and the labels.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef itk::PointSet<VectorType,  Dimension> FeaturePointSetType;

  typedef itk::PointSet<LabelPixelType,  Dimension> LabelPointSetType;

  FeaturePointSetType::Pointer fPSet = FeaturePointSetType::New();
  LabelPointSetType::Pointer   lPSet = LabelPointSetType::New();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We will need to get access to the data stored in the point sets, so
// we define the appropriate for the points and the points containers
// used by the point sets (see the section \ref{sec:PointSetSection}
// for more information oin haw to use point sets).
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef FeaturePointSetType::PointType FeaturePointType;
  typedef LabelPointSetType::PointType   LabelPointType;

  typedef FeaturePointSetType::PointsContainer FeaturePointsContainer;
  typedef LabelPointSetType::PointsContainer   LabelPointsContainer;

  FeaturePointsContainer::Pointer fCont = FeaturePointsContainer::New();
  LabelPointsContainer::Pointer   lCont = LabelPointsContainer::New();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We need now to build the training set for the SVM learning. In this
// simple example, we will build a SVM who classes points depending on
// which side of the line $x=y$ they are located. We start by
// generating 500 random points.
//
// Software Guide : EndLatex

  /* We learn the y>x | y<x boundary*/
  srand(0);

// Software Guide : BeginCodeSnippet
  int lowest = 0;
  int range = 1000;

  for (unsigned int pointId = 0; pointId < 500; pointId++)
    {

    FeaturePointType fP;
    LabelPointType   lP;

    int x_coord = lowest + static_cast<int>(range * (rand() / (RAND_MAX + 1.0)));
    int y_coord = lowest + static_cast<int>(range * (rand() / (RAND_MAX + 1.0)));
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We set the coordinates of the points. They are the same for the
// feature vector and for the label.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
    fP[0] = x_coord;
    fP[1] = y_coord;

    lP[0] = x_coord;
    lP[1] = y_coord;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We push the features in the vector after a normalization which is
// useful for SVM convergence.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
    VectorType feature;
    feature.push_back(static_cast<PixelType>((x_coord * 1.0 - lowest) / range));
    feature.push_back(static_cast<PixelType>((y_coord * 1.0 - lowest) / range));
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We decide on the label for each point.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
    LabelPixelType label;

    if (x_coord < y_coord) label = -1;
    else label = 1;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// And we insert the points in the points containers.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
    fCont->InsertElement(pointId, fP);
    fPSet->SetPointData(pointId, feature);

    lCont->InsertElement(pointId, lP);
    lPSet->SetPointData(pointId, label);

    }
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// After the loop, we set the points containers to the point sets.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  fPSet->SetPoints(fCont);
  lPSet->SetPoints(lCont);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Up to now, we have only prepared the data for the SVM learning. We
// can now create the SVM model estimator. This class is templated
// over the feature and the label point set types.
// \index{otb::SVMPointSetModelEstimator}
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::SVMPointSetModelEstimator<FeaturePointSetType,
      LabelPointSetType>   EstimatorType;

  EstimatorType::Pointer estimator = EstimatorType::New();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// The next step consists in setting the point sets for the estimator
// and the number of classes for the model. The feture point set is
// set using the \code{SetInputPointSet} and the label point set is
// set with the \code{SetTrainingPointSet} method.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  estimator->SetInputPointSet(fPSet);
  estimator->SetTrainingPointSet(lPSet);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// The model estimation is triggered by calling the \code{Update}
// method.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  estimator->Update();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Finally, we can save the result of the learning to a file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  estimator->SaveModel("svm_model.svm");
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// The \doxygen{otb}{otbSVMModel} class provides several accessors in
// order to get some information about the result of the learning
// step. For instance, one can get the number of support vectors kept
// to define the separation surface by using the
// \code{GetNumberOfSupportVectors()}. This can be very useful to
// detect some kind of overlearning (the number of support vectors is
// close to the number of examples). One can also get the SVs
// themselves by calling the \code {GetSupportVectors()}. The $\alpha$
// values for the support vectors can be accessed by using the
// \code{GetAlpha()} method. Finally the \code{Evaluate()} method will
// return the result of the classification of a sample and the
// \code{EvaluateHyperplaneDistance()} will return the distance of
// the sample to the separating surface (or surfaces in the case of
// multi-class problems).
//
// Software Guide : EndLatex

  return EXIT_SUCCESS;
}
