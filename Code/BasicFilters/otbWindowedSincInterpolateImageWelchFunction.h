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
#ifndef __otbWindowedSincInterpolateImageWelchFunction_h
#define __otbWindowedSincInterpolateImageWelchFunction_h

#include "otbWindowedSincInterpolateImageFunctionBase.h"
#include "otbMath.h"
#include "vnl/vnl_math.h"

namespace otb
{

namespace Function
{
/**
 * \class WelchWindowFunction
 * \brief Window function for sinc interpolation.
 * \f[ w(x) = 1 - ( \frac{x^2}{m^2} ) \f]
 * \sa WindowedSincInterpolateImageWelchFunction
 */
template<class TInput = double, class TOutput = double>
class WelchWindowFunction
{
public:
  void SetRadius(unsigned int radius)
  {
    m_Radius = radius;
    m_Factor = 1 / static_cast<double>(radius * radius);
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
    double temp = 1.0 - x * m_Factor * x;
    return (x == 0.0) ? static_cast<TOutput>(temp) : static_cast<TOutput>(temp * vcl_sin(px) / px);
  }
private:
  // Equal to \f$ \frac{1}{m^2} \f$
  double       m_Factor;
  unsigned int m_Radius;
};
} //namespace Function

/**
 * \class WindowedSincInterpolateImageWelchFunction
 * \brief Use the WindowedSincInterpolateImageFunctionBase with a Welch Function.
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
class ITK_EXPORT WindowedSincInterpolateImageWelchFunction :
  public WindowedSincInterpolateImageFunctionBase<TInputImage,
      typename Function::WelchWindowFunction<TInputInterpolator,
          TOutputInterpolator>,
      TBoundaryCondition,
      TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef WindowedSincInterpolateImageWelchFunction Self;
  typedef WindowedSincInterpolateImageFunctionBase<TInputImage,
      typename Function::WelchWindowFunction<TInputInterpolator,
          TOutputInterpolator>,
      TBoundaryCondition,
      TCoordRep>
  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(WindowedSincInterpolateImageWelchFunction, WindowedSincInterpolateImageFunctionBase);

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
  WindowedSincInterpolateImageWelchFunction() {};
  ~WindowedSincInterpolateImageWelchFunction() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  WindowedSincInterpolateImageWelchFunction(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // end namespace otb

#endif
