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
#include "itkLightProcessObject.h"

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
 * EstimateModels() is a pure virtual function making this an abstract class.
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
class ITK_EXPORT SVMModelEstimator : public itk::LightProcessObject
{
public:
  /** Standard class typedefs. */
  typedef SVMModelEstimator   Self;
  typedef itk::LightProcessObject   Superclass;

  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);


  /** Set the number of classes. */
  itkSetMacro(NumberOfClasses, unsigned int);

  /** Get the number of classes. */
  itkGetConstReferenceMacro(NumberOfClasses, unsigned int);

  /** Type definitions for the SVM Model. */
  typedef std::vector< InputPixelType > MeasurementVectorType;
  typedef SVMModel< InputPixelType, LabelPixelType >   SVMModelType;
  typedef typename SVMModelType::Pointer     SVMModelPointer;

  /* Vectors to hold the training data */
  typedef std::vector<MeasurementVectorType> TrainingMeasuresType;
  typedef std::vector<LabelPixelType>        TrainingLabelsType;

  /** Set the model */
  itkGetMacro(Model, SVMModelPointer);

  /** Set/Get the Measures */
  void SetMeasures( TrainingMeasuresType measures )
  {
    m_Measures = measures;
  };
  TrainingMeasuresType GetMeasures()
  {
    return m_Measures;
  };

  /** Set/Get the Labels */
  void SetLabels( TrainingLabelsType labels )
  {
    m_Labels = labels;
  };
  TrainingLabelsType GetLabels()
  {
    return m_Labels;
  };

  /** Get the number of classes. */
  itkGetConstReferenceMacro(Model, SVMModelPointer);

  /** Save the estimated model */
  void SaveModel(const char* model_file_name);

  /** Set the SVM type to ONE_CLASS, C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR */
  void SetSVMType(int svmtype)
  {
    m_Model->SetSVMType(svmtype);
    this->Modified();
  }

  /** Get the SVM type (C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR) */
  int GetSVMType(void)
  {
    return m_Model->GetSVMType();
  }

  /** Set the kernel type to LINEAR, POLY, RBF, SIGMOID
  linear: u'*v
  polynomial: (gamma*u'*v + coef0)^degree
  radial basis function: exp(-gamma*|u-v|^2)
  sigmoid: tanh(gamma*u'*v + coef0)*/
  void SetKernelType(int kerneltype)
  {
    m_Model->SetKernelType(kerneltype);
    this->Modified();
  }

  /** Get the kernel type */
  int GetKernelType(void)
  {
    return m_Model->GetKernelType();
  }

  /** Set the degree of the polynomial kernel */
  void SetPolynomialKernelDegree(int degree)
  {
    m_Model->SetPolynomialKernelDegree(degree);
    this->Modified();
  }


  /** Get the degree of the polynomial kernel */
  int GetPolynomialKernelDegree(void)
  {
    return m_Model->GetPolynomialKernelDegree();
  }

  /** Set the gamma parameter for poly/rbf/sigmoid kernels */
  void SetKernelGamma(double gamma)
  {
    m_Model->SetKernelGamma(gamma);
    this->Modified();
  }
  /** Get the gamma parameter for poly/rbf/sigmoid kernels */
  double GetKernelGamma(void)
  {
    return m_Model->GetKernelGamma();
  }

  /** Set the coef0 parameter for poly/sigmoid kernels */
  void SetKernelCoef0(double coef0)
  {
    m_Model->SetKernelCoef0(coef0);
    this->Modified();
  }

  /** Get the coef0 parameter for poly/sigmoid kernels */
  double GetKernelCoef0(void)
  {
    return m_Model->GetKernelCoef0();
  }

  /** Set the Nu parameter for the training */
  void SetNu(double nu)
  {
    m_Model->SetNu(nu);
    this->Modified();
  }

  /** Set the Nu parameter for the training */
  double GetNu(void)
  {
    return m_Model->GetNu();
  }

  /** Set the cache size in MB for the training */
  void SetCacheSize(int cSize)
  {
    m_Model->SetCacheSize(cSize);
    this->Modified();
  }

  /** Get the cache size in MB for the training */
  int GetCacheSize(void)
  {
    return (m_Model->GetCacheSize());
  }

  /** Set the C parameter for the training for C_SVC, EPSILON_SVR and NU_SVR */
  void SetC(double c)
  {
    m_Model->SetC(c);
    this->Modified();
  }

  /** Get the C parameter for the training for C_SVC, EPSILON_SVR and NU_SVR */
  double GetC(void)
  {
    return m_Model->GetC();
  }

  /** Set the tolerance for the stopping criterion for the training*/
  void SetEpsilon(double eps)
  {
    m_Model->SetEpsilon(eps);
    this->Modified();
  }

  /** Get the tolerance for the stopping criterion for the training*/
  double GetEpsilon(void)
  {
    return m_Model->GetEpsilon();
  }


  /* Set the value of p for EPSILON_SVR */
  void SetP(double p)
  {
    //param.svm_type = EPSILON_SVR;
    m_Model->SetP(p);
    this->Modified();
  }


  /* Get the value of p for EPSILON_SVR */
  double GetP(void)
  {
    return m_Model->GetP();
  }

  /** Use the shrinking heuristics for the training */
  void DoShrinking(bool s)
  {
    m_Model->DoShrinking(s);
    this->Modified();
  }

  /** Get Use the shrinking heuristics for the training boolean */
  bool GetDoShrinking(void)
  {
    return (m_Model->GetDoShrinking());
  }


  /** Do probability estimates */
  void DoProbabilityEstimates(bool prob)
  {
    m_Model->DoProbabilityEstimates(prob);
    this->Modified();
  }

  /** Get Do probability estimates boolean */
  bool GetDoProbabilityEstimates(void)
  {
    return (m_Model->GetDoProbabilityEstimates());
  }

  void Update();

  /** Get/Set methods for generic kernel functor */
  virtual GenericKernelFunctorBase * GetKernelFunctor(void)const
  {
    return m_Model->GetKernelFunctor();
  }
  virtual void SetKernelFunctor(GenericKernelFunctorBase* pGenericKernelFunctor)
  {
    m_Model->SetKernelFunctor(pGenericKernelFunctor);
    this->Modified();
  }

  virtual void  PrepareData();

protected:
  SVMModelEstimator();
  ~SVMModelEstimator();
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Starts the modelling process */
  void GenerateData();

  unsigned int         m_NumberOfClasses;
  TrainingMeasuresType m_Measures;
  TrainingLabelsType   m_Labels;

  /** A function that generates the
   * model based on the training input data
   * Achieves the goal of training the classifier. */
  virtual void EstimateModels();
  virtual void BuildProblem()
  {
  }


  SVMModelPointer m_Model;

  bool m_Done;


private:
  SVMModelEstimator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

}; // class SVMModelEstimator

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSVMModelEstimator.txx"
#endif



#endif

