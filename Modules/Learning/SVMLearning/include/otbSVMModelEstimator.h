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
#ifndef __otbSVMModelEstimator_h
#define __otbSVMModelEstimator_h

#include "otbSVMModel.h"
#include "itkProcessObject.h"

namespace otb
{

/** \class SVMModelEstimator
 * \brief Class for SVM model estimation from images used for classification.
 *
 *
 * The basic functionality of the SVMModelEstimator framework base class is to
 * generate the models used in SVM classification. It requires input
 * images and a training image to be provided by the user.
 * This object supports data handling of multiband images. The object
 * accepts the input image in vector format only, where each pixel is a
 * vector and each element of the vector corresponds to an entry from
 * 1 particular band of a multiband dataset. A single band image is treated
 * as a vector image with a single element for every vector. The classified
 * image is treated as a single band scalar image.
 *
 * The template parameter is the type of a membership function the
 * ModelEstimator populates.
 *
 * A membership function represents a specific knowledge about
 * a class. In other words, it should tell us how "likely" is that a
 * measurement vector (pattern) belong to the class.
 *
 * As the method name indicates, you can have more than one membership
 * function. One for each classes. The order you put the membership
 * calculator becomes the class label for the class that is represented
 * by the membership calculator.
 *
 *
 * \ingroup ClassificationFilters
 */
template <class InputPixelType, class LabelPixelType>
class ITK_EXPORT SVMModelEstimator : public itk::ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef SVMModelEstimator             Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Model typedef */
  typedef SVMModel<InputPixelType, LabelPixelType> ModelType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Macro defining the type*/
  itkTypeMacro(SVMModelEstimator, SuperClass);

  /** Get the output model */
  ModelType * GetModel();

  /** Get the output model */
  const ModelType * GetModel() const;

  /** Type definitions for the SVM Model. */
  typedef SVMModel<InputPixelType, LabelPixelType> SVMModelType;
  typedef typename SVMModelType::Pointer           SVMModelPointer;

  /** Get the cross validation accuracy measures */
  itkGetMacro(InitialCrossValidationAccuracy, double);
  itkGetMacro(FinalCrossValidationAccuracy, double);

  /** Activate/Deactivate parameters optimization */
  itkSetMacro(ParametersOptimization, bool);
  itkGetMacro(ParametersOptimization, bool);
  itkBooleanMacro(ParametersOptimization);

  /** Set/Get the number of steps for the coarse optimization */
  itkSetMacro(CoarseOptimizationNumberOfSteps, unsigned int);
  itkGetMacro(CoarseOptimizationNumberOfSteps, unsigned int);

  /** Set/Get the number of steps for the fine optimization */
  itkSetMacro(FineOptimizationNumberOfSteps, unsigned int);
  itkGetMacro(FineOptimizationNumberOfSteps, unsigned int);

  /** Set/Get the number of cross validation folders */
  itkSetMacro(NumberOfCrossValidationFolders, unsigned int);
  itkGetMacro(NumberOfCrossValidationFolders, unsigned int);

  /** Get the number of classes */
  unsigned int GetNumberOfClasses()
  {
    return this->GetModel()->GetNumberOfClasses();
  }

  /** Set the SVM type to ONE_CLASS, C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR */
  void SetSVMType(int svmtype)
  {
    this->GetModel()->SetSVMType(svmtype);
    this->Modified();
  }

  /** Get the SVM type (C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR) */
  int GetSVMType(void)
  {
    return this->GetModel()->GetSVMType();
  }

  /** Set the kernel type to LINEAR, POLY, RBF, SIGMOID
   * - linear: \f$ u'*v \f$
   * - polynomial: \f$ (\gamma*u'*v + coef0)^{degree} \f$
   * - radial basis function: \f$ exp(-\gamma*|u-v|^2) \f$
   * - sigmoid: \f$ tanh(\gamma*u'*v + coef0) \f$
   */
  void SetKernelType(int kerneltype)
  {
    this->GetModel()->SetKernelType(kerneltype);
    this->Modified();
  }

  /** Get the kernel type */
  int GetKernelType(void)
  {
    return this->GetModel()->GetKernelType();
  }

  /** Set the degree of the polynomial kernel */
  void SetPolynomialKernelDegree(int degree)
  {
    this->GetModel()->SetPolynomialKernelDegree(degree);
    this->Modified();
  }

  /** Get the degree of the polynomial kernel */
  int GetPolynomialKernelDegree(void)
  {
    return this->GetModel()->GetPolynomialKernelDegree();
  }

  /** Set the gamma parameter for poly/rbf/sigmoid kernels */
  void SetKernelGamma(double gamma)
  {
    this->GetModel()->SetKernelGamma(gamma);
    this->Modified();
  }
  /** Get the gamma parameter for poly/rbf/sigmoid kernels */
  double GetKernelGamma(void)
  {
    return this->GetModel()->GetKernelGamma();
  }

  /** Set the coef0 parameter for poly/sigmoid kernels */
  void SetKernelCoef0(double coef0)
  {
    this->GetModel()->SetKernelCoef0(coef0);
    this->Modified();
  }

  /** Get the coef0 parameter for poly/sigmoid kernels */
  double GetKernelCoef0(void)
  {
    return this->GetModel()->GetKernelCoef0();
  }

  /** Set the Nu parameter for the training */
  void SetNu(double nu)
  {
    this->GetModel()->SetNu(nu);
    this->Modified();
  }

  /** Set the Nu parameter for the training */
  double GetNu(void)
  {
    return this->GetModel()->GetNu();
  }

  /** Set the cache size in MB for the training */
  void SetCacheSize(int cSize)
  {
    this->GetModel()->SetCacheSize(cSize);
    this->Modified();
  }

  /** Get the cache size in MB for the training */
  int GetCacheSize(void)
  {
    return (this->GetModel()->GetCacheSize());
  }

  /** Set the C parameter for the training for C_SVC, EPSILON_SVR and NU_SVR */
  void SetC(double c)
  {
    this->GetModel()->SetC(c);
    this->Modified();
  }

  /** Get the C parameter for the training for C_SVC, EPSILON_SVR and NU_SVR */
  double GetC(void)
  {
    return this->GetModel()->GetC();
  }

  /** Set the tolerance for the stopping criterion for the training*/
  void SetEpsilon(double eps)
  {
    this->GetModel()->SetEpsilon(eps);
    this->Modified();
  }

  /** Get the tolerance for the stopping criterion for the training*/
  double GetEpsilon(void)
  {
    return this->GetModel()->GetEpsilon();
  }

  /* Set the value of p for EPSILON_SVR */
  void SetP(double p)
  {
    //param.svm_type = EPSILON_SVR;
    this->GetModel()->SetP(p);
    this->Modified();
  }

  /* Get the value of p for EPSILON_SVR */
  double GetP(void)
  {
    return this->GetModel()->GetP();
  }

  /** Use the shrinking heuristics for the training */
  void DoShrinking(bool s)
  {
    this->GetModel()->DoShrinking(s);
    this->Modified();
  }

  /** Get Use the shrinking heuristics for the training boolean */
  bool GetDoShrinking(void)
  {
    return (this->GetModel()->GetDoShrinking());
  }

  /** Do probability estimates */
  void DoProbabilityEstimates(bool prob)
  {
    this->GetModel()->DoProbabilityEstimates(prob);
    this->Modified();
  }

  /** Get Do probability estimates boolean */
  bool GetDoProbabilityEstimates(void)
  {
    return (this->GetModel()->GetDoProbabilityEstimates());
  }

  /** Get/Set methods for generic kernel functor */
  virtual GenericKernelFunctorBase * GetKernelFunctor(void) const
  {
    return this->GetModel()->GetKernelFunctor();
  }
  virtual void SetKernelFunctor(GenericKernelFunctorBase* pGenericKernelFunctor)
  {
    this->GetModel()->SetKernelFunctor(pGenericKernelFunctor);
    this->Modified();
  }

  /** Save the model */
  virtual void SaveModel(const char * fname)
  {
    this->GetModel()->SaveModel(fname);
  }
  virtual void SaveModel(const std::string& fname)
  {
    //implemented in term of const char * version
    this->SaveModel(fname.c_str());
  }

protected:
  /** Constructor */
  SVMModelEstimator();
  /** Destructor */
  virtual ~SVMModelEstimator();
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Starts the modelling process */
  void GenerateData();

  /** This virtual function must be implemented in subclasses to
  populate the model with samples */
  virtual void  PrepareData(){}

  /** Optimize parameters */
  void OptimizeParameters();

  /** The number of classes */
  unsigned int m_NumberOfClasses;

private:
  SVMModelEstimator(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  // Initial cross validation accuracy
  double m_InitialCrossValidationAccuracy;

  // Final cross validationa accuracy
  double m_FinalCrossValidationAccuracy;

  // Do parameters optimization, default : false
  bool m_ParametersOptimization;

  // Number of steps for the coarse search
  unsigned int m_CoarseOptimizationNumberOfSteps;

  // Number of steps for the fine search
  unsigned int m_FineOptimizationNumberOfSteps;

  // Number of cross validation folders
  unsigned int m_NumberOfCrossValidationFolders;

}; // class SVMModelEstimator

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSVMModelEstimator.txx"
#endif

#endif
