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
#ifndef __otbWindowedSincInterpolateImageFunctionBase_h
#define __otbWindowedSincInterpolateImageFunctionBase_h

#include "otbGenericInterpolateImageFunction.h"
#include "itkSize.h"
#include "otbMath.h"
#include "vnl/vnl_math.h"

namespace otb
{
/**
 * \class WindowedSincInterpolateImageFunctionBase
 * \brief Use the windowed sinc function to interpolate
 *
 * \par THEORY
 *
 * This function is intended to provide an interpolation function that
 * has minimum aliasing artifacts, in contrast to linear interpolation.
 * According to sampling theory, the infinite-support sinc filter,
 * whose Fourier transform is the box filter, is optimal for resampling
 * a function. In practice, the infinite support sinc filter is
 * approximated using a limited support 'windowed' sinc filter.
 * Most of those functions are inspired of the itk::WindowedSincInterpolateImageFunction source code.
 *
 * \par
 * This function is based on the following publication:
 *
 * \par
 * Erik H. W. Meijering, Wiro J. Niessen, Josien P. W. Pluim,
 * Max A. Viergever: Quantitative Comparison of Sinc-Approximating
 * Kernels for Medical Image Interpolation. MICCAI 1999, pp. 210-217
 *
 * \par
 * In this work, several 'windows' are estimated. In two dimensions, the
 * interpolation at a position (x, y) is given by the following
 * expression:
 *
 * \par
 * \f[
 *   I(x, y) =
 *     \sum_{i = \lfloor x \rfloor + 1 - m}^{\lfloor x \rfloor + m}
 *     \sum_{j = \lfloor y \rfloor + 1 - m}^{\lfloor y \rfloor + m}
 *     I_{i, j} K(x-i) K(y-j),
 * \f]
 *
 * \par
 * where m is the 'radius' of the window, (3, 4 are reasonable numbers),
 * and K(t) is the kernel function, composed of the sinc function and
 * one of several possible window functions:
 *
 * \par
 * \f[
 *   K(t) = w(t) \textrm{sinc}(t) = w(t) \frac{\sin(\pi t)}{\pi t}
 * \f]
 *
 * \par
 * Several window functions are provided here in the itk::Function
 * namespace. The conclusions of the referenced paper suggest to use the
 * Welch, Cosine, Kaiser, and Lanczos windows for m = 4, 5. These are based
 * on error in rotating medical images w.r.t. the linear interpolation
 * method. In some cases the results achieve a 20-fold improvement in
 * accuracy.
 *
 * \par USING THIS FILTER
 *
 * Use this filter the way you would use any ImageInterpolationFunction,
 * so for instance, you can plug it into the ResampleImageFilter class.
 * In order to initialize the filter you must choose several template
 * parameters.
 *
 * \par
 * The first (TInputImage) is the image type, that's standard.
 *
 * \par
 * The second (TWindowFunction) is the window function object, which you
 * can choose from about five different functions defined in this
 * header. The default is the Hamming window, which is commonly used
 * but not optimal according to the cited paper.
 *
 * \par
 * The third (TBoundaryCondition) is the boundary condition class used
 * to determine the values of pixels that fall off the image boundary.
 * This class has the same meaning here as in the NeighborhoodItetator
 * classes.
 *
 * \par
 * The fouth (TCoordRep) is again standard for interpolating functions,
 * and should be float or double.
 *
 * \par CAVEATS
 *
 * There are a few improvements that an enthusiasting ITK developer
 * could make to this filter. One issue is with the way that the kernel
 * is applied. The computational expense comes from two sources:
 * computing the kernel weights K(t) and multiplying the pixels in the
 * window by the kernel weights. The first is done more or less
 * efficiently in \f$ 2 m d \f$ operations (where d is the
 * dimensionality of the image). The second can be done
 * better. Presently, each pixel \f$ I(i, j, k) \f$ is multiplied by the
 * weights \f$ K(x-i), K(y-j), K(z-k) \f$ and added to the running
 * total. This results in \f$ d (2m)^d \f$ multiplication
 * operations. However, by keeping intermediate sums, it would be
 * possible to do the operation in \f$ O ( (2m)^d ) \f$
 * operations. This would require some creative coding. In addition, in
 * the case when one of the coordinates is integer, the computation
 * could be reduced by an order of magnitude.
 *
 * \sa GenericInterpolatorImageFunctionBase
 * \sa LinearInterpolateImageFunctionBase ResampleImageFilter
 * \sa Function::HammingWindowFunction
 * \sa Function::CosineWindowFunction
 * \sa Function::WelchWindowFunction
 * \sa Function::LanczosWindowFunction
 * \sa Function::BlackmanWindowFunction
 * \ingroup ImageFunctionBases ImageInterpolators
 *
 * \ingroup OTBInterpolation
 */
template<class TInputImage, class TWindowFunction, class TBoundaryCondition =
      itk::ZeroFluxNeumannBoundaryCondition<TInputImage>, class TCoordRep = double>
class ITK_EXPORT WindowedSincInterpolateImageFunctionBase :
  public GenericInterpolateImageFunction<TInputImage,
      TWindowFunction,
      TBoundaryCondition,
      TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef WindowedSincInterpolateImageFunctionBase                                                     Self;
  typedef GenericInterpolateImageFunction<TInputImage, TWindowFunction, TBoundaryCondition, TCoordRep> Superclass;
  typedef itk::SmartPointer<Self>                                                                      Pointer;
  typedef itk::SmartPointer<const Self>                                                                ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(WindowedSincInterpolateImageFunctionBase, GenericInterpolateImageFunction);

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
  //typedef typename TWindowFunction                                                   FunctionType;
  typedef typename Superclass::FunctionType FunctionType;
  typedef typename std::vector<double>      VectorType;

  /** Compute a resampled profil according to the window size.*/
//   virtual void ComputeResampledWindowedSincProfil();

protected:
  WindowedSincInterpolateImageFunctionBase();
  ~WindowedSincInterpolateImageFunctionBase() ITK_OVERRIDE;
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

private:
  WindowedSincInterpolateImageFunctionBase(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // end namespace itk

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbWindowedSincInterpolateImageFunctionBase.txx"
#endif

#endif
