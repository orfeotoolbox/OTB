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
#ifndef __otbBlendingImageFilter_h
#define __otbBlendingImageFilter_h

#include "itkBinaryFunctorImageFilter.h"
#include "otbUniformAlphaBlendingFunction.h"
#include "otbImage.h"
#include "itkRGBPixel.h"

namespace otb
{
namespace Functor
{
/**\class BlendingFunctor
 * \brief 
 * 
 * \sa ImageLayer
 * \sa BlendingImageFilter
 * \sa BlendingFunction
 */
template <class TInputPixel1, class TInputPixel2, class TOutputPixel>
class BlendingFunctor
{
public:
  /** Blending function typedef */
  typedef otb::Function::BlendingFunction<TInputPixel1,
					  TInputPixel2,TOutputPixel>          BlendingFunctionType;
  /** Blending function pointer typedef */
  typedef typename BlendingFunctionType::Pointer                              BlendingFunctionPointerType;
  /** Default rendering function typedef */
  typedef otb::Function::UniformAlphaBlendingFunction<TInputPixel1,
					  TInputPixel2,TOutputPixel>          DefaultBlendingFunctionType;

  /** Scalar pixel operator */
  inline TOutputPixel operator()(const TInputPixel1 & input1, const TInputPixel2 & input2)
  {
    return m_Function->Evaluate(input1,input2);
  }
  
/** Constructor */
  BlendingFunctor()
  {
    // Default rendering function
    m_Function = DefaultBlendingFunctionType::New();
  }
  
  /** Destructor */
  ~BlendingFunctor(){}

  /** Set the rendering function
   *  \param the Blending function.
   */
  void SetFunction(BlendingFunctionType * function)
  {
    m_Function = function;
  }
  
  /** Get the rendering function
   *  \return the Blending function.
   */
  BlendingFunctionType * GetFunction(void)
  {
    return m_Function;
  }
  
private:
  /** The rendering function */
  BlendingFunctionPointerType m_Function;
};
} // end namespace Functor

/** \class BlendingImageFilter
*   \brief This class applies the rendering functor to an image.
*   
*   Please note that the rendering is actually done by a rendering
*   function.
*  
*   \sa BlendingFunction
*
*/
template <class TInputImage1, class TInputImage2 = TInputImage1, class TOutputImage = TInputImage1>
class BlendingImageFilter
  : public itk::BinaryFunctorImageFilter<TInputImage1,TInputImage2,TOutputImage, 
				        Functor::BlendingFunctor
					< typename TInputImage1::PixelType, 
					  typename TInputImage2::PixelType,
					  typename TOutputImage::PixelType > >
{
public:
  /** Standard typedefs */
  typedef BlendingImageFilter                                   Self;
  typedef itk::BinaryFunctorImageFilter
  <TInputImage1,TInputImage2,TOutputImage, Functor::BlendingFunctor
   < typename TInputImage1::PixelType, 
     typename TInputImage2::PixelType,
     typename TOutputImage::PixelType > >                        Superclass;
  typedef itk::SmartPointer<Self>                                Pointer;
  typedef itk::SmartPointer<const Self>                          ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(BlendingImageFilter,itk::BinaryFunctorImageFilter);

  /** Blending function typedef */
  typedef Functor::BlendingFunctor 
  < typename TInputImage1::PixelType,
    typename TInputImage2::PixelType, 
    typename TOutputImage::PixelType >                       BlendingFunctorType;
  typedef typename BlendingFunctorType::BlendingFunctionType BlendingFunctionType;

  /** 
   * Set the rendering function
   * \param function The rendering function.
   */
  void SetBlendingFunction(BlendingFunctionType * function)
  {
    this->GetFunctor().SetFunction(function);
  }

  /** 
   * Get the rendering function
   * \return The rendering function.
   */
  BlendingFunctionType * GetBlendingFunction(void)
  {
    return this->GetFunctor().GetFunction();
  }

 protected:
  /** Constructor */
  BlendingImageFilter() {}
  /** Destructor */
  virtual ~BlendingImageFilter() {}
  
private:
  BlendingImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};
} // end namespace otb

#endif


