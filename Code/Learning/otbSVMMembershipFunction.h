/*=========================================================================

  Program :   OTB (ORFEO ToolBox)
  Authors   :   CNES - J. Inglada
  Language  :   C++
  Date      :   26 April 2006
  Version   :   
  Role      :   SVM Membership Function
  $Id$


=========================================================================*/
#ifndef __otbSVMMembershipFunction_h
#define __otbSVMMembershipFunction_h



#include "itkMembershipFunctionBase.h"
#include "otbSVMModel.h"


namespace otb{ 

/** \class SVMMembershipFunction
 * \brief SVMMembershipFunction class represents a SVM Model.
 *
 * This class keeps parameter to define the SVM Density Functionclass and has
 * method to return the distance to the hyperplane for  
 * of an instance.  MeasurementVectorSize is the dimension of measurement space.
 * double is type of measurement. 
 */

template< class TVector >
class ITK_EXPORT SVMMembershipFunction :
    public itk::Statistics::MembershipFunctionBase< TVector >
{
public:
  /** Standard class typedefs */
  typedef SVMMembershipFunction Self;
  typedef itk::Statistics::MembershipFunctionBase< TVector > Superclass ;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Strandard macros */
  itkTypeMacro(SVMMembershipFunction, itk::Statistics::MembershipFunctionBase);
  itkNewMacro(Self);
  
  /** Typedef alias for the measurement vectors */
  typedef TVector MeasurementVectorType ;

  /** Typedef to represent the length of measurement vectors */
  typedef typename Superclass::MeasurementVectorSizeType MeasurementVectorSizeType;


  /**  Set the length of each measurement vector. */
  virtual void SetMeasurementVectorSize( const MeasurementVectorSizeType );
  

  /** Type definitions for the SVM Model. */

  typedef SVMModel< MeasurementVectorType >   SVMModelType;
  typedef typename SVMModelType::Pointer     SVMModelPointer;

  /** Set the model */
  itkSetMacro(Model, SVMModelPointer);

  /** Get the model. */
  itkGetConstReferenceMacro(Model, SVMModelPointer);

  /** Set the number of classes. */
  itkSetMacro(NumberOfClasses, int);
  /** Get the number of classes. */
  itkGetConstReferenceMacro(NumberOfClasses, int);

  /**
   * Method to get the distance to the hyperplane of an instance. */
  double Evaluate(const MeasurementVectorType &measurement) const;
  
protected:
  SVMMembershipFunction(void) ;
  virtual ~SVMMembershipFunction(void) {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:

  long int m_NumberOfSamples;
  long int m_MeasurementVectorSize;

  SVMModelPointer m_Model;
  int m_NumberOfClasses;


};


} // end namespace otb

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbSVMMembershipFunction.txx"
#endif

#endif
