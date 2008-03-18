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
#ifndef _otbSVMModel_h
#define _otbSVMModel_h

#include "itkDataObject.h"
#include "itkVariableLengthVector.h"

#include "svm.h"

//#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

namespace otb
{

/** \class SVMModel
 * \brief Class for SVM models.
 * 
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
 * EstimateModels() is a pure virtual function making this an abstract class. 
 * The template parameter is the type of a membership function the 
 * Model populates.
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
template <class TInputPixel, class TLabel >
class ITK_EXPORT SVMModel : public itk::DataObject
{
public:
  /** Standard class typedefs. */
  typedef SVMModel   Self;
  typedef itk::DataObject   Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkNewMacro(Self);
  itkTypeMacro(SVMModel,itk::DataObject);




//   /** Set the number of classes. */
//   itkSetMacro(NumberOfModels, unsigned int);

//   /** Get the number of classes. */
//   itkGetConstReferenceMacro(NumberOfModels, unsigned int);


  typedef TInputPixel InputPixelType;
  typedef TLabel LabelType;

  typedef itk::VariableLengthVector<double> ValuesType;


  /** Type definitions for the training image. */
  //typedef typename TTrainingImage::Pointer TrainingImagePointer;

//   /** Set the input image. */
//   itkSetObjectMacro(InputImage,InputImageType);

//   /** Get the input image. */
//   itkGetObjectMacro(InputImage,InputImageType);

//   /** Set the classified image. */
//   void SetMembershipFunctions(MembershipFunctionPointerVector 
//                               membershipFunctions)
//   {
//     m_MembershipFunctions = membershipFunctions;
//   }
  
//   /** Method to get mean */
//   const MembershipFunctionPointerVector GetMembershipFunctions() const
//   {
//     return m_MembershipFunctions;
//   }

//   /** Method to number of membership functions */
//   unsigned int GetNumberOfMembershipFunctions() 
//   {
//     return static_cast<unsigned int>( m_MembershipFunctions.size() );
//   }

//   /** Method to reset the membership fucntion mean */
//   void DeleteAllMembershipFunctions() 
//   {
//     m_MembershipFunctions.resize(0);
//   }

//   /** Stores a MembershipCalculator of a class in its internal vector */
//   unsigned int AddMembershipFunction(MembershipFunctionPointer function);

//   /** Define a virtual function to perform model generation from the input data
//    */
//   void Update() ;

  /** Set the number of classes. */
//  itkSetMacro(NumberOfClasses, unsigned int);
  void SetNumberOfClasses(const unsigned int nr_class)
  {
        m_Model->nr_class = (int)nr_class;
  }
  
  /** Get the number of classes. */
  unsigned int GetNumberOfClasses(void)
  {
        return (unsigned int)(m_Model->nr_class);
  }

  /** Get the number of hyperplane. */
  unsigned int GetNumberOfHyperplane(void)
  {
        return (unsigned int)(m_Model->nr_class - 1);
  }

  /** Gets the problem */
  struct svm_problem & GetProblem()
  {
        return m_Problem ;
  }

  /** Sets the x space */
  void SetXSpace(struct svm_node* x_space)
  {
        m_XSpace = x_space;
  }
  /** Gets the x space */
  struct svm_node* GetXSpace()
  {
        return m_XSpace;
  }

  /** Allocates the problem */
  void AllocateProblem(int l, long int elements);

  
  /** Sets the model */
  void SetModel(struct svm_model* aModel);

  /** Gets the model */
  const struct svm_model* GetModel()
  {
    return m_Model;
  }

  /** Gets the parameters */
  struct svm_parameter & GetParameters()
  {
    //return m_Parameters;
    return m_Model->param;
  }
  /** Gets the parameters */
  const struct svm_parameter & GetParameters() const
  {
    //return m_Parameters;
    return m_Model->param;
  }


  /** Saves the model to a file */
  void SaveModel(const char* model_file_name);


  /** Loads the model from a file */
  void LoadModel(const char* model_file_name);

  /** Set the SVM type to C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR */
  void SetSVMType(int svmtype)
  {
    //m_Parameters.svm_type = svmtype;
    m_Model->param.svm_type = svmtype;
    this->Modified();
  }

  /** Get the SVM type (C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR) */
  int GetSVMType(void)
  {
    //return m_Parameters.svm_type;
    return m_Model->param.svm_type;
  }
  
  /** Set the kernel type to LINEAR, POLY, RBF, SIGMOID
	linear: u'*v
	polynomial: (gamma*u'*v + coef0)^degree
	radial basis function: exp(-gamma*|u-v|^2)
	sigmoid: tanh(gamma*u'*v + coef0)*/
  void SetKernelType(int kerneltype)
  {
    //m_Parameters.kernel_type = kerneltype;
    m_Model->param.kernel_type = kerneltype;
    this->Modified();
  }

  /** Get the kernel type */
  int GetKernelType(void)
  {
    //return m_Parameters.kernel_type;
    return m_Model->param.kernel_type;
  }

  /** Set the degree of the polynomial kernel */
  void SetPolynomialKernelDegree(int degree)
  {
    //m_Parameters.degree = degree;
    m_Model->param.degree = degree;
    this->Modified();
  }


  /** Get the degree of the polynomial kernel */
  int GetPolynomialKernelDegree(void)
  {
    //return m_Parameters.degree;
    return m_Model->param.degree;
  }

  /** Set the gamma parameter for poly/rbf/sigmoid kernels */
  virtual void SetKernelGamma(double gamma)
  {
    //m_Parameters.gamma = gamma;
    m_Model->param.gamma = gamma;
    this->Modified();
  }
 /** Get the gamma parameter for poly/rbf/sigmoid kernels */
  double GetKernelGamma(void)
  {
    //return m_Parameters.gamma;
    return m_Model->param.gamma;
  }

  /** Set the coef0 parameter for poly/sigmoid kernels */
  void SetKernelCoef0(double coef0)
  {
    //m_Parameters.coef0 = coef0;
    m_Model->param.coef0 = coef0;
    this->Modified();
  }

  /** Get the coef0 parameter for poly/sigmoid kernels */
  double GetKernelCoef0(void)
  {
    //return m_Parameters.coef0;
    return m_Model->param.coef0;
  }

  /** Set the Nu parameter for the training */
  void SetNu(double nu)
  {
    //m_Parameters.nu = nu;
     m_Model->param.nu = nu;
    this->Modified();
  }

  /** Set the Nu parameter for the training */
  double GetNu(void)
    {
      //return m_Parameters.nu;
      return m_Model->param.nu;
    }
  
  /** Set the cache size in MB for the training */
  void SetCacheSize(int cSize)
  {
    //m_Parameters.cache_size = static_cast<double>(cSize);
    m_Model->param.cache_size = static_cast<double>(cSize);
    this->Modified();
  }

  /** Get the cache size in MB for the training */
  int GetCacheSize(void)
  {
    //return static_cast<int>(m_Parameters.cache_size);
    return static_cast<int>(m_Model->param.cache_size);
  }

  /** Set the C parameter for the training for C_SVC, EPSILON_SVR and NU_SVR */
  void SetC(double c)
  {
    //m_Parameters.C = c;
    m_Model->param.C = c;
    this->Modified();
  }

/** Get the C parameter for the training for C_SVC, EPSILON_SVR and NU_SVR */
  double GetC(void)
  {
    //return m_Parameters.C;
    return m_Model->param.C;
  }

  /** Set the tolerance for the stopping criterion for the training*/
  void SetEpsilon(double eps)
  {
    //m_Parameters.eps = eps;
    m_Model->param.eps = eps;
    this->Modified();
  }

  /** Get the tolerance for the stopping criterion for the training*/
  double GetEpsilon(void)
  {
    //return m_Parameters.eps;
    return m_Model->param.eps;
  }


  /* Set the value of p for EPSILON_SVR */
  void SetP(double p)
  {
    //param.svm_type = EPSILON_SVR;
    //m_Parameters.p = p;
    m_Model->param.p = p;
    this->Modified();
  }

  
  /* Get the value of p for EPSILON_SVR */
  double GetP(void)
  {
    //return m_Parameters.p;
    return m_Model->param.p;
  }

  /** Use the shrinking heuristics for the training */
  void DoShrinking(bool s)
  {
    //m_Parameters.shrinking = static_cast<int>(s);
    m_Model->param.shrinking = static_cast<int>(s);
    this->Modified();
  }

  /** Get Use the shrinking heuristics for the training boolean */
  bool GetDoShrinking(void)
  {
    //return static_cast<bool>(m_Parameters.shrinking);
    return static_cast<bool>(m_Model->param.shrinking);
  }


  /** Do probability estimates */
  void DoProbabilityEstimates(bool prob)
  {
    //m_Parameters.probability = static_cast<int>(prob);
    m_Model->param.probability = static_cast<int>(prob);
    this->Modified();
  }

  /** Get Do probability estimates boolean */
  bool GetDoProbabilityEstimates(void)
  {
    //return static_cast<bool>(m_Parameters.probability);
    return static_cast<bool>(m_Model->param.probability);
  }


  /** Get/Set methods for generic kernel functor */
  virtual GenericKernelFunctorBase * GetKernelFunctor(void)const
  {
    //return m_Parameters.kernel_generic;// m_GenericKernelFunctor;
    return m_Model->param.kernel_generic;
  }
  virtual void SetKernelFunctor(GenericKernelFunctorBase* pGenericKernelFunctor)
  {
    //m_GenericKernelFunctor = pGenericKernelFunctor;
    //m_Parameters.kernel_generic = pGenericKernelFunctor;
    m_Model->param.kernel_generic = pGenericKernelFunctor;
    this->Modified();
  }
  /** Return number of support vectors */
  int GetNumberOfSupportVectors(void)
  {
        return m_Model->l;
  }
  /** Return rho values */
  double * GetRho(void)
  {
        return m_Model->rho;
  }
  /** Return the support vectors */
  svm_node ** GetSupportVectors(void)
  {
        return m_Model->SV;
  }
  /** Return the alphas values (SV Coef) */
  double ** GetAlpha (void)
  {
	return m_Model->sv_coef;
  }


  /** Evaluate model */
  double Evaluate(void);

  /** Evaluate hyperplane distance model. 
    * Return NumberOfClasses*(NumberOfClasses-1)/2 elements
    */
  ValuesType EvaluateHyperplaneDistance(void);
  
protected:
  SVMModel();
  ~SVMModel();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  //virtual void GenerateData();

private:

  SVMModel(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

//  unsigned int                    m_NumberOfModels;
//  unsigned int         m_NumberOfClasses;

  /** Container to hold the SVM model itself */
  struct svm_model* m_Model;

  /** Container to hold the SVM model parameters */
  //struct svm_parameter m_Parameters;

  struct svm_problem m_Problem;
  struct svm_node* m_XSpace;
  
 
  /** Pointer to generic kernel functor */
//  GenericKernelFunctorBase * m_GenericKernelFunctor;

}; // class SVMModel


} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSVMModel.txx"
#endif

#endif
