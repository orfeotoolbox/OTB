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
#ifndef __otbBCOInterpolateImageFunction_h
#define __otbBCOInterpolateImageFunction_h

#include "itkInterpolateImageFunction.h"
#include "vnl/vnl_vector.h"

namespace otb
{

/** \class 
 * \brief 
 *
 * 
 * 
 * 
 * 
 *
 * 
 *
 * \warning 
 * 
 *
 * \sa 
 *
 * \ingroup ImageFunctions ImageInterpolators 
 */
template <
  class TInputImage, 
  class TCoordRep = double>
class ITK_EXPORT BCOInterpolateImageFunction : 
  public itk::InterpolateImageFunction<TInputImage,TCoordRep> 
{
public:
  /** Standard class typedefs. */
  typedef BCOInterpolateImageFunction                          Self;
  typedef itk::InterpolateImageFunction<TInputImage,TCoordRep> Superclass;
  typedef itk::SmartPointer<Self>                              Pointer;
  typedef itk::SmartPointer<const Self>                        ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(BCOInterpolateImageFunction, InterpolateImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** OutputType typedef support. */
  typedef typename Superclass::OutputType OutputType;
 
  /** InputImageType typedef support. */
  typedef typename Superclass::InputImageType InputImageType;
  
  /** InputPixelType typedef support. */
  typedef typename Superclass::InputPixelType InputPixelType;
  
  /** RealType typedef support. */
  //typedef typename Superclass::RealType RealType;
  typedef typename itk::NumericTraits<typename TInputImage::PixelType>::RealType RealType;

  /** Dimension underlying input image. */
  itkStaticConstMacro(ImageDimension, unsigned int,Superclass::ImageDimension);

  /** Index typedef support. */
  typedef typename Superclass::IndexType      IndexType;
  typedef typename Superclass::IndexValueType IndexValueType;
  
  /** Point typedef support. */
  typedef typename Superclass::PointType      PointType;

  /** ContinuousIndex typedef support. */
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;

  /** Coeficients container type.*/
  typedef vnl_vector<double> CoefContainerType;

  /** Interpolate the image at a point position
   *
   * Returns the interpolated image intensity at a 
   * specified point position. No bounds checking is done.
   * The point is assume to lie within the image buffer.
   *
   * ImageFunction::IsInsideBuffer() can be used to check bounds before
   * calling the method. */
  //virtual OutputType Evaluate( const PointType& point ) const;//////////////////////////////////////////////////
 
  /** Evaluate the function at a ContinuousIndex position
   *
   * Returns the linearly interpolated image intensity at a 
   * specified point position. No bounds checking is done.
   * The point is assume to lie within the image buffer.
   *
   * ImageFunction::IsInsideBuffer() can be used to check bounds before
   * calling the method. */
  virtual OutputType EvaluateAtContinuousIndex( const ContinuousIndexType & index ) const;/////////////////////
  
  /** Interpolate the image at an index position.
   *
   * Simply returns the image value at the
   * specified index position. No bounds checking is done.
   * The point is assume to lie within the image buffer.
   *
   * ImageFunction::IsInsideBuffer() can be used to check bounds before
   * calling the method. */
  //virtual OutputType EvaluateAtIndex( const IndexType & index ) const;//////////////////////////////////////////

  /** Set/Get the window radius */
  virtual void SetRadius(unsigned int radius);
  virtual unsigned int GetRadius() const;
  
  /** Set/Get the optimisation coefficient (Common values are -0.5, -0.75 or -1.0) */
  virtual void SetAlpha(double alpha);
  virtual double GetAlpha() const;

  /** Compute the BCO coefficients. */
  void EvaluateCoef();

protected:
  BCOInterpolateImageFunction();
  ~BCOInterpolateImageFunction();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  BCOInterpolateImageFunction( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  /** Used radius for the BCO */
  double                 m_Radius;
  /** Optimisation Coefficient */
  double                 m_Alpha;
  /** Used BCO coefficiet */   
  CoefContainerType      m_BCOCoef;
};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBCOInterpolateImageFunction.txx"
#endif

#endif
