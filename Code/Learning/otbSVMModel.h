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

  /** Saves the model to a file */
  void SaveModel(const char* model_file_name);


  /** Loads the model from a file */
  void LoadModel(const char* model_file_name);


  /** Get/Set methods for generic kernel functor */
  virtual GenericKernelFunctorBase * GetKernelFunctor(void)const
  {
        return m_Parameters.kernel_generic;// m_GenericKernelFunctor;
  }
  virtual void SetKernelFunctor(GenericKernelFunctorBase* pGenericKernelFunctor)
  {
        //m_GenericKernelFunctor = pGenericKernelFunctor;
        m_Parameters.kernel_generic = pGenericKernelFunctor;
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
  
  /** Get reference */
  struct svm_parameter & GetParameters()
  {
        return (m_Parameters);
  }
  
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
  struct svm_parameter m_Parameters;

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
















