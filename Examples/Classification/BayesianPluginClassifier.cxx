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


// Software Guide : BeginLatex
//
// \index{Statistics!Bayesian plugin classifier}
// \index{itk::Statistics::SampleClassifierFilter}
// \index{itk::Statistics::GaussianDensityFunction}
// \index{itk::Statistics::NormalVariateGenerator}
//
// In this example, we present a system that places measurement vectors into
// two Gaussian classes. The Figure~\ref{fig:BayesianPluginClassifier} shows
// all the components of the classifier system and the data flow. This system
// differs with the previous k-means clustering algorithms in several
// ways. The biggest difference is that this classifier uses the
// \subdoxygen{itk}{Statistics}{itkGaussianMembershipFunction} as membership functions
// instead of the \subdoxygen{itk}{Statistics}{EuclideanDistance}. Since the
// membership function is different, the membership function requires a
// different set of parameters, mean vectors and covariance matrices. We
// choose the \subdoxygen{itk}{Statistics}{MeanSampleFilter} (sample mean) and the
// \subdoxygen{itk}{Statistics}{CovarianceSampleFilter} (sample covariance) for the
// estimation algorithms of the two parameters. If we want more robust
// estimation algorithm, we can replace these estimation algorithms with more
// alternatives without changing other components in the classifier system.
//
// It is a bad idea to use the same sample for test and training
// (parameter estimation) of the parameters. However, for simplicity, in
// this example, we use a sample for test and training.
//
// \begin{figure}
//   \centering
//   \includegraphics[width=0.9\textwidth]{BayesianPluginClassifier.eps}
//   \itkcaption[Bayesian plug-in classifier for two Gaussian classes]{Bayesian
//   plug-in classifier for two Gaussian classes.}
//  \protect\label{fig:BayesianPluginClassifier}
// \end{figure}
//
// We use the \subdoxygen{itk}{Statistics}{ListSample} as the sample (test
// and training). The \doxygen{itk}{Vector} is our measurement vector
// class. To store measurement vectors into two separate sample
// containers, we use the \subdoxygen{itk}{Statistics}{Subsample} objects.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "itkVector.h"
#include "itkArray.h"
#include "itkListSample.h"
#include "itkSubsample.h"
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// The following two files provides us the parameter estimation algorithms.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "itkMeanSampleFilter.h"
#include "itkCovarianceSampleFilter.h"
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// The following files define the components required by ITK statistical
// classification framework: the decision rule, the membership
// function, and the classifier.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "itkDecisionRule.h"
#include "itkMaximumRatioDecisionRule.h"
#include "itkGaussianMembershipFunction.h"
#include "itkSampleClassifierFilter.h"
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We will fill the sample with random variables from two normal
// distribution using the \subdoxygen{itk}{Statistics}{NormalVariateGenerator}.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "itkNormalVariateGenerator.h"
// Software Guide : EndCodeSnippet

int main(int,  char *[])
{
  // Software Guide : BeginLatex
  //
  // Since the NormalVariateGenerator class only supports 1-D, we define our
  // measurement vector type as a one component vector. We then, create a
  // ListSample object for data inputs.
  //
  // We also create two Subsample objects that will store
  // the measurement vectors in \code{sample} into two separate
  // sample containers. Each Subsample object stores only the
  // measurement vectors belonging to a single class. This class sample
  // will be used by the parameter estimation algorithms.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::Vector<double,
      1>
  MeasurementVectorType;
  typedef itk::Statistics::ListSample<MeasurementVectorType> SampleType;
  SampleType::Pointer sample = SampleType::New();
  sample->SetMeasurementVectorSize(1);   // length of measurement vectors
  // in the sample.

  typedef itk::Statistics::Subsample<SampleType> ClassSampleType;
  std::vector<ClassSampleType::Pointer> classSamples;
  for (unsigned int i = 0; i < 2; ++i)
    {
    classSamples.push_back(ClassSampleType::New());
    classSamples[i]->SetSample(sample);
    }
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The following code snippet creates a NormalVariateGenerator
  // object. Since the random variable generator returns values according to
  // the standard normal distribution (the mean is zero, and the standard
  // deviation is one) before pushing random values into the \code{sample},
  // we change the mean and standard deviation. We want two normal (Gaussian)
  // distribution data. We have two for loops. Each for loop uses different
  // mean and standard deviation. Before we fill the \code{sample} with the
  // second distribution data, we call \code{Initialize(random seed)} method,
  // to recreate the pool of random variables in the
  // \code{normalGenerator}. In the second for loop, we fill the two class
  // samples with measurement vectors using the \code{AddInstance()} method.
  //
  // To see the probability density plots from the two distributions,
  // refer to Figure~\ref{fig:TwoNormalDensityFunctionPlot}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::Statistics::NormalVariateGenerator NormalGeneratorType;
  NormalGeneratorType::Pointer normalGenerator = NormalGeneratorType::New();

  normalGenerator->Initialize(101);

  MeasurementVectorType          mv;
  double                         mean = 100;
  double                         standardDeviation = 30;
  SampleType::InstanceIdentifier id = 0UL;
  for (unsigned int i = 0; i < 100; ++i)
    {
    mv.Fill((normalGenerator->GetVariate() * standardDeviation) + mean);
    sample->PushBack(mv);
    classSamples[0]->AddInstance(id);
    ++id;
    }

  normalGenerator->Initialize(3024);
  mean = 200;
  standardDeviation = 30;
  for (unsigned int i = 0; i < 100; ++i)
    {
    mv.Fill((normalGenerator->GetVariate() * standardDeviation) + mean);
    sample->PushBack(mv);
    classSamples[1]->AddInstance(id);
    ++id;
    }
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // In the following code snippet, notice that the template argument for the
  // MeanSampleFilter and CovarianceFilter is \code{ClassSampleType} (i.e.,
  // type of Subsample) instead of SampleType (i.e., type of ListSample). This
  // is because the parameter estimation algorithms are applied to the class
  // sample.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::Statistics::MeanSampleFilter<ClassSampleType> MeanEstimatorType;
  typedef itk::Statistics::CovarianceSampleFilter<ClassSampleType>
  CovarianceEstimatorType;

  std::vector<MeanEstimatorType::Pointer>       meanEstimators;
  std::vector<CovarianceEstimatorType::Pointer> covarianceEstimators;

  for (unsigned int i = 0; i < 2; ++i)
    {
    meanEstimators.push_back(MeanEstimatorType::New());
    meanEstimators[i]->SetInput(classSamples[i]);
    meanEstimators[i]->Update();

    covarianceEstimators.push_back(CovarianceEstimatorType::New());
    covarianceEstimators[i]->SetInput(classSamples[i]);
    //covarianceEstimators[i]->SetMean(meanEstimators[i]->GetOutput());
    covarianceEstimators[i]->Update();
    }
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We print out the estimated parameters.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  for (unsigned int i = 0; i < 2; ++i)
    {
    std::cout << "class[" << i << "] " << std::endl;
    std::cout << "    estimated mean : "
              << meanEstimators[i]->GetMean()
              << "    covariance matrix : "
              << covarianceEstimators[i]->GetCovarianceMatrixOutput()->Get() << std::endl;
    }
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // After creating a SampleClassifierFilter object and a
  // MaximumRatioDecisionRule object, we plug in the
  // \code{decisionRule} and the \code{sample} to the classifier. Then,
  // we specify the number of classes that will be considered using
  // the \code{SetNumberOfClasses()} method.
  //
  // The MaximumRatioDecisionRule requires a vector of \emph{a
  // priori} probability values. Such \emph{a priori} probability will
  // be the $P(\omega_{i})$ of the following variation of the Bayes
  // decision rule:
  // \begin{equation}
  //   \textrm{Decide } \omega_{i} \textrm{ if }
  //   \frac{p(\overrightarrow{x}|\omega_{i})}
  //        {p(\overrightarrow{x}|\omega_{j})}
  // > \frac{P(\omega_{j})}{P(\omega_{i})} \textrm{ for all } j \not= i
  //   \label{eq:bayes2}
  // \end{equation}
  //
  // The remainder of the code snippet shows how to use user-specified class
  // labels. The classification result will be stored in a
  // MembershipSample object, and for each measurement vector, its
  // class label will be one of the two class labels, 100 and 200
  // (\code{unsigned int}).
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::Statistics::GaussianMembershipFunction
  <MeasurementVectorType> MembershipFunctionType;
  typedef itk::Statistics::MaximumRatioDecisionRule DecisionRuleType;
  DecisionRuleType::Pointer decisionRule = DecisionRuleType::New();

  DecisionRuleType::PriorProbabilityVectorType aPrioris;
  aPrioris.push_back(classSamples[0]->GetTotalFrequency()
                     / sample->GetTotalFrequency());
  aPrioris.push_back(classSamples[1]->GetTotalFrequency()
                     / sample->GetTotalFrequency());
  decisionRule->SetPriorProbabilities(aPrioris);

  typedef itk::Statistics::SampleClassifierFilter<SampleType> ClassifierType;
  ClassifierType::Pointer classifier = ClassifierType::New();

  classifier->SetDecisionRule(dynamic_cast< itk::Statistics::DecisionRule* >( decisionRule.GetPointer()));
  classifier->SetInput(sample);
  classifier->SetNumberOfClasses(2);

  std::vector<long unsigned int> classLabels;
  classLabels.resize(2);
  classLabels[0] = 100;
  classLabels[1] = 200;

  typedef itk::SimpleDataObjectDecorator<std::vector<long unsigned int> >   ClassLabelDecoratedType;
  ClassLabelDecoratedType::Pointer classLabelsDecorated = ClassLabelDecoratedType::New();
  classLabelsDecorated->Set(classLabels);

  classifier->SetClassLabels(classLabelsDecorated);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The \code{classifier} is almost ready to perform the classification
  // except that it needs two membership functions that represent the two
  // clusters.
  //
  // In this example, we can imagine that the two clusters are modeled by two
  // Euclidean distance functions. The distance function (model) has only one
  // parameter, the mean (centroid) set by the \code{SetOrigin()} method. To
  // plug-in two distance functions, we call the
  // \code{AddMembershipFunction()} method. Then invocation of the
  // \code{Update()} method will perform the classification.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef ClassifierType::MembershipFunctionType                              MembershipFunctionBaseType;
  typedef ClassifierType::MembershipFunctionVectorObjectType::ComponentType   ComponentMembershipType;

  // Vector Containing the membership function used
  ComponentMembershipType     membershipFunctions;

  for (unsigned int i = 0; i < 2; i++)
    {
    MembershipFunctionType::Pointer curMemshpFunction =  MembershipFunctionType::New();
    curMemshpFunction->SetMean(meanEstimators[i]->GetMean());
    curMemshpFunction->SetCovariance(covarianceEstimators[i]->GetCovarianceMatrix());

    // cast the GaussianMembershipFunction in a
    // itk::MembershipFunctionBase
    membershipFunctions.push_back(dynamic_cast<const MembershipFunctionBaseType*  >(curMemshpFunction.GetPointer()));
    }

  ClassifierType::MembershipFunctionVectorObjectPointer membershipVectorObject = ClassifierType::MembershipFunctionVectorObjectType::New();
  membershipVectorObject->Set(membershipFunctions);

  classifier->SetMembershipFunctions(membershipVectorObject);
  classifier->Update();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The following code snippet prints out pairs of a measurement vector and
  // its class label in the \code{sample}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  const ClassifierType::MembershipSampleType* membershipSample = classifier->GetOutput();
  ClassifierType::MembershipSampleType::ConstIterator iter = membershipSample->Begin();

  while (iter != membershipSample->End())
    {
    std::cout << "measurement vector = " << iter.GetMeasurementVector()
              << "class label = " << iter.GetClassLabel() << std::endl;
    ++iter;
    }
  // Software Guide : EndCodeSnippet

  return EXIT_SUCCESS;
}
