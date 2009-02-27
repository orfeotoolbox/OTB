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
#ifndef __otbRenderingImageFilter_h
#define __otbRenderingImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "otbStandardRenderingFunction.h"
#include "otbImage.h"
#include "itkRGBPixel.h"

namespace otb
{
namespace Functor
{
/**\class RenderingFunctor
 * \brief This functor renders the input pixel.
 * This functor renders the input pixel to a printable format
 * (unsigned char RGB).
 * It uses a rendering function which is in charge of the rendering.
 *
 * It defines both operators on scalar and vector pixels, so it can be
 * used either on VectorImage or Image.
 *
 * The operators call the Evaluate() method of the rendering function
 * and returns its result.
 *
 * \sa ImageLayer
 * \sa RenderingImageFilter
 * \sa RenderingFunction
 */
template <class TPixelPrecision, class TRGBPixel>
class RenderingFunctor
{
public:
  /** Rendering function typedef */
  typedef otb::Function::RenderingFunction<TPixelPrecision,TRGBPixel>          RenderingFunctionType;
  /** Rendering function pointer typedef */
  typedef typename RenderingFunctionType::Pointer                              RenderingFunctionPointerType;
  /** Default rendering function typedef */
  typedef otb::Function::StandardRenderingFunction<TPixelPrecision,TRGBPixel> DefaultRenderingFunctionType;
  /** Scalar pixel typedef */
  typedef TPixelPrecision                                                      ScalarPixelType;
  /** Vector pixel typedef */
  typedef itk::VariableLengthVector<TPixelPrecision>                           VectorPixelType;

  /** Scalar pixel operator */
  inline TRGBPixel operator()(const ScalarPixelType & pixel) const
  {
    return m_Function->Evaluate(pixel);
  }
  /** Vector pixel operator */
  inline TRGBPixel operator()(const VectorPixelType & pixel) const
  {
    return m_Function->Evaluate(pixel);
  }

  /** Constructor */
  RenderingFunctor() : m_Function()
  {
    // Default rendering function
    m_Function = DefaultRenderingFunctionType::New();
  }

  /** Destructor */
  ~RenderingFunctor(){}

  /** Set the rendering function
   *  \param function the Rendering function.
   */
  void SetFunction(RenderingFunctionType * function)
  {
    m_Function = function;
  }

  /** Get the rendering function
   *  \return the Rendering function.
   */
  RenderingFunctionType * GetFunction(void)
  {
    return m_Function;
  }

  /**
   * Initialize the rendering function.
   */
  void InitializeFunction(void)
  {
    m_Function->Initialize();
  }

private:
  /** The rendering function */
  RenderingFunctionPointerType m_Function;
};
} // end namespace Functor

/** \class RenderingImageFilter
*   \brief This class applies the rendering functor to an image.
*
*   Please note that the rendering is actually done by a rendering
*   function.
*
*   \sa RenderingFunction
*
*/
template <class TInputImage,class TOutputImage = Image<itk::RGBPixel<unsigned char>, 2 > >
class RenderingImageFilter
  : public itk::UnaryFunctorImageFilter<TInputImage,TOutputImage,
				        Functor::RenderingFunctor
					< typename TInputImage ::InternalPixelType,
					  typename TOutputImage::PixelType > >
{
public:
  /** Standard typedefs */
  typedef RenderingImageFilter                                   Self;
  typedef itk::UnaryFunctorImageFilter
  <TInputImage,TOutputImage, Functor::RenderingFunctor
		 < typename TInputImage ::InternalPixelType,
		   typename TOutputImage::PixelType > >          Superclass;
  typedef itk::SmartPointer<Self>                                Pointer;
  typedef itk::SmartPointer<const Self>                          ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(RenderingImageFilter,itk::UnaryFunctorImageFilter);

  /** Rendering function typedef */
  typedef Functor::RenderingFunctor
  < typename TInputImage ::InternalPixelType,
    typename TOutputImage::PixelType >                         RenderingFunctorType;
  typedef typename RenderingFunctorType::RenderingFunctionType RenderingFunctionType;

  /**
   * Set the rendering function
   * \param function The rendering function.
   */
  void SetRenderingFunction(RenderingFunctionType * function)
  {
    if(function != this->GetFunctor().GetFunction())
      {
      this->GetFunctor().SetFunction(function);
      this->Modified();
      }
  }

  /**
   * Get the rendering function
   * \return The rendering function.
   */
  RenderingFunctionType * GetRenderingFunction(void)
  {
    return this->GetFunctor().GetFunction();
  }

  /**
   * Initialize the function before any processing
   */
  virtual void BeforeThreadedGenerateData(void)
  {
    // Call the superclass implementation
    Superclass::BeforeThreadedGenerateData();
    
    // Initialize the rendering function
    this->GetFunctor().InitializeFunction();
  }

 protected:
  /** Constructor */
  RenderingImageFilter() {}
  /** Destructor */
  virtual ~RenderingImageFilter() {}

private:
  RenderingImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};
} // end namespace otb

#endif


