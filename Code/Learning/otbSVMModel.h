/*=========================================================================

  Program :   OTB (ORFEO ToolBox)
  Authors   :   CNES - J. Inglada
  Language  :   C++
  Date      :   26 April 2006
  Version   :   
  Role      :   SVM Model
  $Id$


=========================================================================*/
#ifndef _otbSVMModel_h
#define _otbSVMModel_h

#include "itkDataObject.h"

extern "C"
{
#include "svm.h"
}

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

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
  itkSetMacro(NumberOfClasses, unsigned int);
  
  /** Get the number of classes. */
  itkGetConstReferenceMacro(NumberOfClasses, unsigned int);

  /** Gets the problem */
  struct svm_problem GetProblem()
  {
	  return m_Problem ;
  }

  /** Gets the problem */
  struct svm_node* GetXSpace()
  {
	  return x_space;
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

  int GetNumberOfClasses()
  {
    return m_Model->nr_class;
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
  unsigned int         m_NumberOfClasses;

  /** Container to hold the SVM model itself */
  struct svm_model* m_Model;

  /** Container to hold the SVM model parameters */
  struct svm_parameter m_Parameters;

  struct svm_problem m_Problem;
  struct svm_node* x_space;


}; // class SVMModel


} // namespace otb

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbSVMModel.txx"
#endif



#endif
















