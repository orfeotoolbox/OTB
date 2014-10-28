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
#ifndef __otbWindowedSincInterpolateImageGaussianFunction_h
#define __otbWindowedSincInterpolateImageGaussianFunction_h

#include "otbWindowedSincInterpolateImageFunctionBase.h"
#include "vnl/vnl_math.h"

namespace otb
{

namespace Function
{
/**
 * \class GaussianWindowFunction
 * \brief Gaussian interpolation windowing function.
 * \f[ w(x) = exp ( -2. \frac{sigma*sigma * \pi } ) \f]
 * \sa otbWindowedSincInterpolateImageFunctionBase
 */
template<class TInput = double, class TOutput = double>
class GaussianWindowFunction
{
public:
  void SetRadius(unsigned int radius)
  {
    m_Radius = radius;
    m_Factor = -2. / (static_cast<double>(m_Radius * m_Radius) * CONST_PI);
  }
  double GetFactor()
  {
    return m_Factor;
  }
  unsigned int GetRadius() const
  {
    return m_Radius;
  }

  inline TOutput operator ()(const TInput& A) const
  {
    double x = static_cast<double>(A);
    double px = CONST_PI * x;
    double temp = vcl_exp(px * px * m_Factor);
    return (x == 0.0) ? static_cast<TOutput>(temp) : static_cast<TOutput>(temp * vcl_sin(px) / px);
  }
private:
  double       m_Factor;
  unsigned int m_Radius;
};

} //namespace Function

/**
 * \class WindowedSincInterpolateImageGaussianFunction
 * \brief Use the WindowedSincInterpolateImageFunctionBase with a Gaussian Function.
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
template<class TInputImage, class TBoundaryCondition = itk::ZeroFluxNeumannBoundaryCondition<TInputImage>, class TCoordRep =
      double, class TInputInterpolator = double, class TOutputInterpolator = double>
class ITK_EXPORT WindowedSincInterpolateImageGaussianFunction :
  public WindowedSincInterpolateImageFunctionBase<TInputImage,
      typename Function::GaussianWindowFunction<TInputInterpolator,
          TOutputInterpolator>,
      TBoundaryCondition,
      TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef WindowedSincInterpolateImageGaussianFunction Self;
  typedef WindowedSincInterpolateImageFunctionBase<TInputImage,
      typename Function::GaussianWindowFunction<TInputInterpolator,
          TOutputInterpolator>,
      TBoundaryCondition,
      TCoordRep>
  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(WindowedSincInterpolateImageGaussianFunction, WindowedSincInterpolateImageFunctionBase);

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
  WindowedSincInterpolateImageGaussianFunction() {};
  ~WindowedSincInterpolateImageGaussianFunction() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  WindowedSincInterpolateImageGaussianFunction(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // end namespace otb

#endif
