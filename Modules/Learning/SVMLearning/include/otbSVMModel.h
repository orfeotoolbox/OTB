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
#ifndef __otbSVMModel_h
#define __otbSVMModel_h

#include "itkObjectFactory.h"
#include "itkDataObject.h"
#include "itkVariableLengthVector.h"
#include "itkTimeProbe.h"
#include "otb_libsvm.h"

namespace otb
{

/** \class SVMModel
 * \brief Class for SVM models.
 *
 * \TODO update documentation
 *
 * The basic functionality of the SVMModel framework base class is to
 * generate the models used in SVM classification. It requires input
 * images and a training image to be provided by the user.
 * This object supports data handling of multiband images. The object
 * accepts the input image in vector format only, where each pixel is a
 * vector and each element of the vector corresponds to an entry from
 * 1 particular band of a multiband dataset. A single band image is treated
 * as a vector image with a single element for every vector. The classified
 * image is treated as a single band scalar image.
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
 *
 * \ingroup OTBSVMLearning
 */
template <class TValue, class TLabel>
class ITK_EXPORT SVMModel : public itk::DataObject
{
public:
  /** Standard class typedefs. */
  typedef SVMModel                      Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Value type */
  typedef TValue ValueType;
  /** Label Type */
  typedef TLabel                                LabelType;
  typedef std::vector<ValueType>                MeasurementType;
  typedef std::pair<MeasurementType, LabelType> SampleType;
  typedef std::vector<SampleType>               SamplesVectorType;
  /** Cache vector type */
  typedef std::vector<struct svm_node *> CacheVectorType;

  /** Distances vector */
  typedef itk::VariableLengthVector<double> ProbabilitiesVectorType;
  typedef itk::VariableLengthVector<double> DistancesVectorType;

  typedef struct svm_node * NodeCacheType;

  /** Run-time type information (and related methods). */
  itkNewMacro(Self);
  itkTypeMacro(SVMModel, itk::DataObject);

  /** Get the number of classes. */
  unsigned int GetNumberOfClasses(void) const
  {
    return (unsigned int) (m_Model->nr_class);
  }

  /** Get the number of hyperplane. */
  unsigned int GetNumberOfHyperplane(void) const
  {
    return (unsigned int) (m_Model->nr_class * (m_Model->nr_class - 1) / 2);
  }

  /** Set a new model. To avoid pointers holding conflicts, this
   * method actually makes a copy of aModel */
  void SetModel(struct svm_model* aModel);

  /** Gets the model */
  const struct svm_model* GetModel()
  {
    return m_Model;
  }

  /** Gets the parameters */
  struct svm_parameter& GetParameters()
  {
    return m_Parameters;
  }
  /** Gets the parameters */
  const struct svm_parameter& GetParameters() const
  {
    return m_Parameters;
  }

  /** Saves the model to a file */
  void SaveModel(const char* model_file_name) const;
  void SaveModel(const std::string& model_file_name) const
  {
    //implemented in term of const char * version
    this->SaveModel(model_file_name.c_str());
  }

  /** Loads the model from a file */
  void LoadModel(const char* model_file_name);
  void LoadModel(const std::string& model_file_name)
  {
    //implemented in term of const char * version
    this->LoadModel(model_file_name.c_str());
  }

  /** Copy the model */
  Pointer GetCopy() const;

  /** Set the SVM type to C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR */
  void SetSVMType(int svmtype)
  {
    m_Parameters.svm_type = svmtype;
    this->Modified();
  }

  /** Get the SVM type (C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR) */
  int GetSVMType(void) const
  {
    return m_Parameters.svm_type;
  }

  /** Set the kernel type to LINEAR, POLY, RBF, SIGMOID
  linear: u'*v
  polynomial: (gamma*u'*v + coef0)^degree
  radial basis function: exp(-gamma*|u-v|^2)
  sigmoid: tanh(gamma*u'*v + coef0)*/
  void SetKernelType(int kerneltype)
  {
    m_Parameters.kernel_type = kerneltype;
    this->Modified();
  }

  /** Get the kernel type */
  int GetKernelType(void) const
  {
    return m_Parameters.kernel_type;
  }

  /** Set the degree of the polynomial kernel */
  void SetPolynomialKernelDegree(int degree)
  {
    m_Parameters.degree = degree;
    this->Modified();
  }

  /** Get the degree of the polynomial kernel */
  int GetPolynomialKernelDegree(void) const
  {
    return m_Parameters.degree;
  }

  /** Set the gamma parameter for poly/rbf/sigmoid kernels */
  virtual void SetKernelGamma(double gamma)
  {
    m_Parameters.gamma = gamma;
    this->Modified();
  }
  /** Get the gamma parameter for poly/rbf/sigmoid kernels */
  double GetKernelGamma(void) const
  {
    return m_Parameters.gamma;
  }

  /** Set the coef0 parameter for poly/sigmoid kernels */
  void SetKernelCoef0(double coef0)
  {
    m_Parameters.coef0 = coef0;
    this->Modified();
  }

  /** Get the coef0 parameter for poly/sigmoid kernels */
  double GetKernelCoef0(void) const
  {
    //return m_Parameters.coef0;
    return m_Parameters.coef0;
  }

  /** Set the Nu parameter for the training */
  void SetNu(double nu)
  {
    m_Parameters.nu = nu;
    this->Modified();
  }

  /** Set the Nu parameter for the training */
  double GetNu(void) const
  {
    //return m_Parameters.nu;
    return m_Parameters.nu;
  }

  /** Set the cache size in MB for the training */
  void SetCacheSize(int cSize)
  {
    m_Parameters.cache_size = static_cast<double>(cSize);
    this->Modified();
  }

  /** Get the cache size in MB for the training */
  int GetCacheSize(void) const
  {
    return static_cast<int>(m_Parameters.cache_size);
  }

  /** Set the C parameter for the training for C_SVC, EPSILON_SVR and NU_SVR */
  void SetC(double c)
  {
    m_Parameters.C = c;
    this->Modified();
  }

  /** Get the C parameter for the training for C_SVC, EPSILON_SVR and NU_SVR */
  double GetC(void) const
  {
    return m_Parameters.C;
  }

  /** Set the tolerance for the stopping criterion for the training*/
  void SetEpsilon(double eps)
  {
    m_Parameters.eps = eps;
    this->Modified();
  }

  /** Get the tolerance for the stopping criterion for the training*/
  double GetEpsilon(void) const
  {
    return m_Parameters.eps;
  }

  /* Set the value of p for EPSILON_SVR */
  void SetP(double p)
  {
    m_Parameters.p = p;
    this->Modified();
  }

  /* Get the value of p for EPSILON_SVR */
  double GetP(void) const
  {
    return m_Parameters.p;
  }

  /** Use the shrinking heuristics for the training */
  void DoShrinking(bool s)
  {
    m_Parameters.shrinking = static_cast<int>(s);
    this->Modified();
  }

  /** Get Use the shrinking heuristics for the training boolea */
  bool GetDoShrinking(void) const
  {
    return static_cast<bool>(m_Parameters.shrinking);
  }

  /** Do probability estimates */
  void DoProbabilityEstimates(bool prob)
  {
    m_Parameters.probability = static_cast<int>(prob);
    this->Modified();
  }

  /** Get Do probability estimates boolean */
  bool GetDoProbabilityEstimates(void) const
  {
    return static_cast<bool>(m_Parameters.probability);
  }

  /** Get/Set methods for generic kernel functor */
  GenericKernelFunctorBase * GetKernelFunctor(void) const
  {
    return m_Parameters.kernel_generic;
  }

  void SetKernelFunctor(GenericKernelFunctorBase* pGenericKernelFunctor)
  {
    if (pGenericKernelFunctor != NULL)
      m_Parameters.kernel_generic = pGenericKernelFunctor->Clone();
    this->Modified();
  }

  /** Return number of support vectors */
  int GetNumberOfSupportVectors(void) const
  {
    return m_Model->l;
  }

  /** Return rho values */
  double * GetRho(void) const
  {
    return m_Model->rho;
  }
  /** Return the support vectors */
  svm_node ** GetSupportVectors(void)
  {
    return m_Model->SV;
  }
  /** Set the support vectors and changes the l number of support vectors accordind to sv.*/
  void SetSupportVectors(svm_node ** sv, int nbOfSupportVector);

  /** Return the alphas values (SV Coef) */
  double ** GetAlpha(void)
  {
    return m_Model->sv_coef;
  }
  /** Set the alphas values (SV Coef) */
  void SetAlpha(double ** alpha, int nbOfSupportVector);

  /** Return the labels lists */
  int * GetLabels()
  {
    return m_Model->label;
  }

  /** Get the number of SV per classes */
  int * GetNumberOfSVPerClasse()
  {
    return m_Model->nSV;
  }

  struct svm_problem& GetProblem()
  {
    return m_Problem;
  }

  /** Reset ModelUpToDate */
  virtual void Modified()
  {
    Superclass::Modified();
    m_ModelUpToDate = false;
  }

  /** Allocate the problem */
  void BuildProblem();

  /** Check consistency (potentially throws exception) */
  void ConsistencyCheck();

  /** Estimate the model */
  void Train();

  /** Cross validation (returns the accuracy) */
  double CrossValidation(unsigned int nbFolders);

  /** Predict (Please note that due to caching this method is not
* thread safe. If you want to run multiple concurrent instances of
* this method, please consider using the GetCopy() method to clone the
* model.)*/
  LabelType EvaluateLabel(const MeasurementType& measure) const;

  /** Evaluate hyperplan distances (Please note that due to caching this method is not
* thread safe. If you want to run multiple concurrent instances of
* this method, please consider using the GetCopy() method to clone the
* model.)**/
  DistancesVectorType EvaluateHyperplanesDistances(const MeasurementType& measure) const;

  /** Evaluate probabilities of each class. Returns a probability vector ordered
   * by increasing class label value
   * (Please note that due to caching this method is not thread safe.
   * If you want to run multiple concurrent instances of
   * this method, please consider using the GetCopy() method to clone the
   * model.)**/
  ProbabilitiesVectorType EvaluateProbabilities(const MeasurementType& measure) const;

  /** Add a new sample to the list */
  void AddSample(const MeasurementType& measure, const LabelType& label);

  /** Clear all samples */
  void ClearSamples();

  /** Set the samples vector */
  void SetSamples(const SamplesVectorType& samples);

  /** Reset all the model, leaving it in the same state that just
   * before constructor call */
  void Reset();

protected:
  /** Constructor */
  SVMModel();
  /** Destructor */
  virtual ~SVMModel();
  /** Display infos */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

/** Delete any allocated problem */
  void DeleteProblem();

  /** Delete any allocated model */
  void DeleteModel();

  /** Initializes default parameters */
  void Initialize();

private:
  SVMModel(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Container to hold the SVM model itself */
  struct svm_model* m_Model;

  /** True if model is up-to-date */
  bool m_ModelUpToDate;

  /** Container of the SVM problem */
  struct svm_problem m_Problem;

  /** Container of the SVM parameters */
  struct svm_parameter m_Parameters;

  /** true if problem is up-to-date */
  bool m_ProblemUpToDate;

  /** Contains the samples */
  SamplesVectorType m_Samples;
}; // class SVMModel

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSVMModel.txx"
#endif

#endif
