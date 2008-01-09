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
#ifndef _otbGenericInterpolateImageFunction_h
#define _otbGenericInterpolateImageFunction_h

#include "itkInterpolateImageFunction.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkConstantBoundaryCondition.h"

namespace otb
{
  
/** \class GenericInterpolateImageFunction
 * \brief Generic interpolation of an otb::image.
 *
 * GenericInterpolateImageFunction interpolates image intensity according to a 
 * resampling profil.
 * \ingroup ImageFunctions ImageInterpolators
 */
template <class TInputImage, class TFunction, class TBoundaryCondition = itk::ConstantBoundaryCondition<TInputImage>, class TCoordRep = double>
class ITK_EXPORT GenericInterpolateImageFunction : 
public itk::InterpolateImageFunction<TInputImage,TCoordRep> 
  {
    public:
    /** Standard class typedefs. */
    typedef GenericInterpolateImageFunction Self;
    typedef itk::InterpolateImageFunction<TInputImage,TCoordRep> Superclass;
    typedef itk::SmartPointer<Self> Pointer;
    typedef itk::SmartPointer<const Self>  ConstPointer;
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(GenericInterpolateImageFunction, itk::InterpolateImageFunction);
    
    /** Method for creation through the object factory. */
    itkNewMacro(Self);  
    
    /** Input and output images typedef definition. */
    typedef typename Superclass::OutputType     OutputType;
    typedef typename Superclass::InputImageType InputImageType;
    
    /** Dimension underlying input image. */
    //itkStaticConstMacro(ImageDimension, unsigned int,Superclass::ImageDimension);
    
    /** Index and typedef support. */
    typedef typename Superclass::IndexType                                 IndexType;
    typedef typename InputImageType::SizeType                              SizeType;
    typedef typename Superclass::RealType                                  RealType;
    typedef TFunction                                                      FunctionType;
    typedef itk::ConstNeighborhoodIterator< InputImageType, TBoundaryCondition> IteratorType;

    /** ContinuousIndex typedef support. */
    typedef typename Superclass::ContinuousIndexType ContinuousIndexType;

    virtual void SetInputImage(const InputImageType *image);

    /** Evaluate the function at a ContinuousIndex position
     *
     * Returns the interpolated image intensity at a 
     * specified point position. No bounds checking is done.
     * The point is assume to lie within the image buffer.
     *
     * ImageFunction::IsInsideBuffer() can be used to check bounds before
     * calling the method. */
    virtual OutputType EvaluateAtContinuousIndex( const ContinuousIndexType & index ) const;

    /** Set/Get the window radius*/
    void SetRadius(SizeType rad)
    {
      m_Radius = rad;
      m_WindowSize[0] = m_Radius[0] << 1;
      m_WindowSize[1] = m_Radius[1] << 1;
    };
    void SetRadius(unsigned int radX, unsigned int radY)
    {
      m_Radius[0] = radX;
      m_Radius[1] = radY;
      m_WindowSize[0] = radX << 1;
      m_WindowSize[1] = radY << 1;
    };
    SizeType GetRadius()
    {
      return m_Radius;
    };

    protected:
    GenericInterpolateImageFunction();
    ~GenericInterpolateImageFunction();
    void PrintSelf(std::ostream& os, itk::Indent indent) const;
 

    private:
    GenericInterpolateImageFunction(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented
    /** Store the window radius. */
    SizeType m_Radius;
    // Constant to store twice the radius
    SizeType m_WindowSize;
    /** The offset array, used to keep a list of relevant
     * offsets in the neihborhoodIterator */
    unsigned int *m_OffsetTable;
    /** Used function */
    FunctionType m_Function;
    /** Size of the offset table */
    unsigned int m_OffsetTableSize;
    /** Store the image dimension.*/
    unsigned int m_ImageDimension;  
    /** Index into the weights array for each offset */
    unsigned int **m_WeightOffsetTable;
  };

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbGenericInterpolateImageFunction.txx"
#endif

#endif
