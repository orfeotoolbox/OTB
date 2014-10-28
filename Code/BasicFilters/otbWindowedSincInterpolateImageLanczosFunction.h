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
#ifndef __otbWindowedSincInterpolateImageLanczosFunction_h
#define __otbWindowedSincInterpolateImageLanczosFunction_h

#include "otbWindowedSincInterpolateImageFunctionBase.h"
#include "otbMath.h"
#include "vnl/vnl_math.h"

namespace otb
{

namespace Function
{
/**
 * \class LanczosWindowFunction
 * \brief Window function for sinc interpolation.
 * \f[ w(x) = \textrm{sinc} ( \frac{x}{m} ) \f]
 * Note: Paper referenced in WindowedSincInterpolateImageFunctionBase gives
 * an incorrect definition of this window function.
 * \sa WindowedSincInterpolateImageLanczosFunction
 */
template<class TInput = double, class TOutput = double>
class LanczosWindowFunction
{
public:
  void SetRadius(unsigned int radius)
  {
    m_Radius = radius;
    m_Factor =  CONST_PI / static_cast<double>(radius);
  }
  unsigned int GetRadius() const
  {
    return m_Radius;
  }
  double GetFactor()
  {
    return m_Factor;
  }

  inline TOutput operator ()(const TInput& A) const
  {
    double x = static_cast<double>(A);
    double px = CONST_PI * x;
    double temp;
    if (x == 0.0)
      {
      temp = 1.0;
      }
    else
      {
      double z = m_Factor * x;
      temp =  vcl_sin(z) / z;
      }
    return (x == 0.0) ? static_cast<TOutput>(temp) : static_cast<TOutput>(temp * vcl_sin(px) / px);
  }
private:
  // Equal to \f$ \frac{\pi}{m} \f$
  double       m_Factor;
  unsigned int m_Radius;
};
} //namespace Function

/**
 * \class WindowedSincInterpolateImageLanczosFunction
 * \brief Use the WindowedSincInterpolateImageFunctionBase with a Lanczos Function.
 *
 * \sa GenericInterpolatorImageFunction
 * \sa Function::GaussianWindowFunction
 * \sa Function::HammingWindowFunction
 * \sa Function::CosineWindowFunction
 * \sa Function::WelchWindowFunction
 * \sa Function::LanczosWindowFunction
 * \sa Function::BlackmanWindowFunction
 * \ingroup ImageFunctionBases ImageInterpolators
 */
template<class TInputImage, class TBoundaryCondition = itk::ConstantBoundaryCondition<TInputImage>, class TCoordRep =
      double, class TInputInterpolator = double, class TOutputInterpolator = double>
class ITK_EXPORT WindowedSincInterpolateImageLanczosFunction :
  public WindowedSincInterpolateImageFunctionBase<TInputImage,
      typename Function::LanczosWindowFunction<TInputInterpolator,
          TOutputInterpolator>,
      TBoundaryCondition,
      TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef WindowedSincInterpolateImageLanczosFunction Self;
  typedef WindowedSincInterpolateImageFunctionBase<TInputImage,
      typename Function::LanczosWindowFunction<TInputInterpolator,
          TOutputInterpolator>,
      TBoundaryCondition,
      TCoordRep>
  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(WindowedSincInterpolateImageLanczosFunction, WindowedSincInterpolateImageFunctionBase);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Input and output images typedef definition. */
  typedef typename Superclass::InputImageType InputImageType;
  typedef typename Superclass::OutputType     OutputType;

  /** Dimension underlying input image. */
  itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);

  /** Superclass typedef inheritance. */
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::SizeType            SizeType;
  typedef typename Superclass::RealType            RealType;
  typedef typename Superclass::IteratorType        IteratorType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;

protected:
  WindowedSincInterpolateImageLanczosFunction() {};
  ~WindowedSincInterpolateImageLanczosFunction() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  WindowedSincInterpolateImageLanczosFunction(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // end namespace otb

#endif
