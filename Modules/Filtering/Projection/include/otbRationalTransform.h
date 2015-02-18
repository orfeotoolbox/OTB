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
#ifndef __otbRationalTransform_h
#define __otbRationalTransform_h

#include "otbTransform.h"
#include "itkMacro.h"

namespace otb
{
/** \class RationalTransform
 *  \brief This class implements a rational transfom
 *
 *  A rational transform is a quotient of two polynomial functions.
 *
 *  The degree of the numerator and denominator polynomial functions
 *  can be set using the appropriate setters.
 *
 *  The number of parameters is then the number of dimensions times
 *  the numerator degree plus one times the denominator degree plus
 *  one.
 *
 *  Parameters in the parameters vector are in the following order:
 *  dim0num0 dim0num1 ... dim0numN dim0denom0 dim0denom1
 *  ... dim0denomM ... dim1num0 ... dimDdenomM.
 *
 * \ingroup Transform
 **/

template <class TScalarType = double,
          unsigned int Dimension = 2>
class ITK_EXPORT RationalTransform : public Transform<TScalarType, Dimension, Dimension>
{
public:
  /** Standard class typedefs. */
  typedef Transform<TScalarType, Dimension,
    Dimension>                                      Superclass;
  typedef RationalTransform                         Self;
  typedef itk::SmartPointer<Self>                   Pointer;
  typedef itk::SmartPointer<const Self>             ConstPointer;

  typedef typename Superclass::ScalarType           ScalarType;
  typedef itk::Point<ScalarType, Dimension>         InputPointType;
  typedef itk::Point<ScalarType, Dimension>         OutputPointType;

  typedef itk::Vector<double, Dimension> SpacingType;
  typedef itk::Point<double, Dimension>  OriginType;

  typedef typename Superclass::InverseTransformBasePointer InverseTransformBasePointer;
  typedef typename Superclass::ParametersType              ParametersType;

  typedef typename Superclass::ParametersValueType     ParametersValueType;
  typedef typename Superclass::NumberOfParametersType  NumberOfParametersType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(RationalTransform, Transform);

  itkStaticConstMacro(SpaceDimension, unsigned int, Dimension);

  /** Set the numerator degree */
  void SetNumeratorDegree(unsigned int value)
  {
    this->m_NumeratorDegree = value;
    this->InitalizeParameters();
  }

  /** Get the numerator degree */
  itkGetConstMacro(NumeratorDegree, unsigned int);

  /** Set the numerator degree */
  void SetDenominatorDegree(unsigned int value)
  {
    this->m_DenominatorDegree = value;
    this->InitalizeParameters();
  }

  /** Get the denominator degree */
  itkGetConstMacro(DenominatorDegree, unsigned int);

  /** The transform point method */
  virtual OutputPointType TransformPoint(const InputPointType& point) const
  {
    // Check for consistency
    if(this->GetNumberOfParameters() != this->m_Parameters.size())
      {
      {
      itkExceptionMacro(<<"Wrong number of parameters: found "<<this->m_Parameters.Size()<<", expected "<<this->GetNumberOfParameters());
      }
      }

    InputPointType  inputPoint = point;
    OutputPointType outputPoint;

    unsigned int dimensionStride = (m_DenominatorDegree+1)+(m_NumeratorDegree+1);

    // Compute RPC transform
    for(unsigned int dim = 0; dim < SpaceDimension; ++dim)
      {
      //1) Initialize numerator and denominator
      TScalarType num   = itk::NumericTraits<TScalarType>::Zero;
      TScalarType denom = itk::NumericTraits<TScalarType>::Zero;
      TScalarType currentPower = 1.;

      // 2) Compute numerator
      for(unsigned int numDegree = 0; numDegree <= m_NumeratorDegree; ++numDegree)
        {
        num+=this->m_Parameters[dim*dimensionStride+numDegree]*currentPower;
        currentPower*=inputPoint[dim];
        }

      //3) Compute denominator
      currentPower = 1.;
      for(unsigned int denomDegree = 0; denomDegree <= m_DenominatorDegree; ++denomDegree)
        {
        denom+=this->m_Parameters[dim*dimensionStride+m_NumeratorDegree+denomDegree+1]*currentPower;
        currentPower*=inputPoint[dim];
        }

      //4) Fill the output
      outputPoint[dim]=num/denom;
      }

    // Return the output point
    return outputPoint;
  }

  /** Get the number of parameters */
  virtual NumberOfParametersType GetNumberOfParameters() const
  {
    return (static_cast <NumberOfParametersType> ( (m_NumeratorDegree +1 + m_DenominatorDegree+1)*SpaceDimension ));
  }

  // Set parameter method
  virtual void SetParameters(const typename Superclass::ParametersType & params)
  {
    // Check for the appropriate size
    if(params.Size() != this->GetNumberOfParameters())
      {
      itkExceptionMacro(<<"Wrong number of parameters: found "<<params.Size()<<", expected "<<this->GetNumberOfParameters());
      }

    // Set parametersg
    this->m_Parameters = params;
  }

  /** Initialize Parameters size  */
  void InitalizeParameters()
  {
    this->m_Parameters.SetSize(this->GetNumberOfParameters());
    this->m_Parameters.Fill(0);
    unsigned int dimensionStride = (m_DenominatorDegree+1)+(m_NumeratorDegree+1);

    for(unsigned int dim = 0; dim < SpaceDimension; ++dim)
      {
      this->m_Parameters[ dimensionStride *dim + m_NumeratorDegree+1] = 1.;
      }
  }


protected:
  RationalTransform() : Superclass(16), m_NumeratorDegree(3), m_DenominatorDegree(3)
    {
    this->InitalizeParameters();
    }


  virtual ~RationalTransform() {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
    os << indent << "Numerator Degree : " << m_NumeratorDegree << std::endl;
    os << indent << "Denominator Degree : " << m_DenominatorDegree << std::endl;

  }

private:
  RationalTransform(const Self &);    //purposely not implemented
  void operator =(const Self&);    //purposely not implemented

  // Degree of numerator
  unsigned int m_NumeratorDegree;

  // Degree of denominator
  unsigned int m_DenominatorDegree;
};

} // namespace otb

#endif
