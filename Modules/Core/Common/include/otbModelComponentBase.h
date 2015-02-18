/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are covered by the GET copyright.
  See GETCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbModelComponentBase_h
#define __otbModelComponentBase_h

#include <vnl/vnl_vector.h>
#include <vnl/vnl_matrix.h>

#include "itkObject.h"
#include "itkArray.h"
#include "itkMembershipFunctionBase.h"

namespace otb
{
namespace Statistics
{

/** \class ModelComponentBase
 * \brief base class for distribution representation that supports
 * analytical way to update the distribution parameters
 *
 * This class expects that its subclasses (distribution components) should
 * have analytical expressions for updating its parameters using only
 * the measurement vectors.
 *
 * The difference from MixtureModelComponentBase is that it does not have
 * the knowledge of a mixture. Hence, no weights have to define.
 *
 * This class can be considered as a macro class that encapsulates the
 * storage for the model (subclasses of MembershipFunctionBase) and
 * model parameter estimators (implementation of analytical expressions).
 *
 * Subclasses of this class should define their own distribution specific
 * membership function. For example, GaussianModelComponent class
 * defines and creates a GaussianDensityFunction object for that matter.
 * Subclasses should also cast such membership function object to
 * MembershipFunctionBase object. By doing that, users can get pointers
 * to membership functions from different distributional model
 *
 * \sa FIXME: StochasticExpectationMaximizationMixtureModelEstimator
 */

template<class TSample>
class ITK_EXPORT ModelComponentBase :
  public itk::Object
{
public:
  /**Standard class typedefs. */
  typedef ModelComponentBase            Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /**Standard Macros */
  itkNewMacro(Self);
  itkTypeMacro(ModelComponentBase, Object);

  typedef typename TSample::MeasurementVectorType     MeasurementVectorType;
  typedef typename TSample::MeasurementVectorSizeType MeasurementVectorSizeType;

  /** typedef for the MembershipFunctionBase */
  typedef itk::Statistics::MembershipFunctionBase<MeasurementVectorType>
  MembershipFunctionType;

  typedef itk::Array<double> ParametersType;

  /** stores the sample pointer (does not update the estimation by itself) */
  virtual void SetSample(const TSample* sample);

  /** returns the sample pointer */
  const TSample* GetSample() const;

  /** returns the pointer to the membership function object.
   * Subclasses of this class are responsible for creating the
   * actual membership function objects and cast them to
   * MembershipFunctionBase objects */
  MembershipFunctionType* GetPdfMembershipFunction();
  MembershipFunctionType* GetCdfMembershipFunction();

  /** stores the pointer to the membership function.
   * subclasses use this function to store their membership function
   * object after dynamic creation, when available (the Pdf part is
   * protected as soon as it is considered to be known for all
   * ModelComponent)*/
  void SetCdfMembershipFunction(MembershipFunctionType* function);

  /** Gets/Sets the parameter(s) required for the component */
  void SetParameters(const ParametersType& parameters);
  ParametersType GetParameters()
  {
    return m_Parameters;
  }

  /** returns the pdf of the "measurements" vector */
  double Evaluate(MeasurementVectorType& measurements);
  double Pdf(MeasurementVectorType& measurements);

  /** returns the cdf of the "measurements" vector */
  double Cdf(MeasurementVectorType& measurements);

  /** Update parameter estimation if necessary
   * (mainly when sample has changed) */
  virtual void Update();

  /** Show the parameters in a minimal form in comparison to PrintSelf */
  virtual void ShowParameters(std::ostream& os, itk::Indent indent) const;

  // TODO: Distance entre distribution via une sorte de fonction membre...

protected:
  ModelComponentBase();
  virtual ~ModelComponentBase() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void GenerateData();

  /** stores the pointer to the membership function.
   * subclasses use this funtion to store their membership function
   * object after dynamic creation, when available */
  void SetPdfMembershipFunction(MembershipFunctionType* function);

  /** Test on new data */
  int IsSampleModified();

  /** Set of parameters necessary for the statistical models */
  ParametersType m_Parameters;

private:
  ModelComponentBase(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
  /** target sample data pointer */
  const TSample* m_Sample;

protected:
  /** SmartPointer to the membership function - here density function */
  MembershipFunctionType* m_PdfFunction;
  /** SmartPointer to the membership function - here cumulative function */
  MembershipFunctionType* m_CdfFunction;
  /** indicative flag of membership function's parameter changes */
  int m_SampleModified;

}; // end of class

} // end of namespace Statistics
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbModelComponentBase.txx"
#endif

#endif
