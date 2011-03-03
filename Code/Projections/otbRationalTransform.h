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

#include "itkTransform.h"
#include "itkExceptionObject.h"
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
class ITK_EXPORT RationalTransform : public itk::Transform<TScalarType, Dimension, Dimension>
{
public:
  /** Standard class typedefs. */
  typedef itk::Transform<TScalarType, Dimension,
    Dimension>                                      Superclass;
  typedef RationalTransform                         Self;
  typedef itk::SmartPointer<Self>                   Pointer;
  typedef itk::SmartPointer<const Self>             ConstPointer;

  typedef typename Superclass::ScalarType           ScalarType;
  typedef itk::Point<ScalarType, Dimension>         InputPointType;
  typedef itk::Point<ScalarType, Dimension>         OutputPointType;

  typedef typename Superclass::InverseTransformBasePointer InverseTransformBasePointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(RationalTransform, itk::Transform);

  itkStaticConstMacro(SpaceDimension, unsigned int, Dimension);

  /** Set the numerator degree */
  itkSetMacro(NumeratorDegree, unsigned int);

  /** Get the numerator degree */
  itkGetMacro(NumeratorDegree, unsigned int);

  /** Set the numerator degree */
  itkSetMacro(DenominatorDegree, unsigned int);

  /** Get the denominator degree */
  itkGetMacro(DenominatorDegree, unsigned int);


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

    // Build output
    OutputPointType outputPoint;

    unsigned int dimensionStride = (m_DenominatorDegree+1)+(m_NumeratorDegree+1);
    
    // Compute RPC transform
    for(unsigned int dim = 0; dim < SpaceDimension; ++dim)
      {
      // Initialize numerator and denominator
      TScalarType num   = itk::NumericTraits<TScalarType>::Zero;
      TScalarType denom = itk::NumericTraits<TScalarType>::Zero;
      TScalarType currentPower = 1.;
      
      // Compute numerator
      for(unsigned int numDegree = 0; numDegree <= m_NumeratorDegree; ++numDegree)
        {
        num+=this->m_Parameters[dim*dimensionStride+numDegree]*currentPower;
        currentPower*=point[dim];
        }
      
      // Compute denominator
      currentPower = 1.;
      for(unsigned int denomDegree = 0; denomDegree <= m_DenominatorDegree; ++denomDegree)
        {
        denom+=this->m_Parameters[dim*dimensionStride+m_NumeratorDegree+denomDegree+1]*currentPower;
        currentPower*=point[dim];
        }

      // Finally, fill the output
      outputPoint[dim]=num/denom;
      }

    // Return the output point
    return outputPoint;
  }

  // Get the number of parameters
  virtual unsigned int GetNumberOfParameters() const
  {
    return (m_NumeratorDegree +1 + m_DenominatorDegree+1)*SpaceDimension;
  }

  // Set parameter method
  virtual void SetParameters(const typename Superclass::ParametersType & params)
  {
    // Check for the appropriate size
    if(params.Size() != this->GetNumberOfParameters())
      {
      itkExceptionMacro(<<"Wrong number of parameters: found "<<params.Size()<<", expected "<<this->GetNumberOfParameters());
      }
    
    // Set parameters
    this->m_Parameters = params;
  }

protected:
  RationalTransform() : Superclass(SpaceDimension, 16), m_NumeratorDegree(3), m_DenominatorDegree(3)
    {
    this->m_Parameters.SetSize(this->GetNumberOfParameters());
    this->m_Parameters.Fill(0);
    this->m_Parameters[1] = 1.;
    this->m_Parameters[4] = 1.;
    }

  virtual ~RationalTransform() {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
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
