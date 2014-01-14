/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbRenderingImageFilter_h
#define __otbRenderingImageFilter_h


//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)
#include "itkImageRegionConstIterator.h"
#include "itkMetaDataDictionary.h"
#include "itkRGBAPixel.h"
#include "itkUnaryFunctorImageFilter.h"

//
// OTB includes (sorted by alphabetic order)
#include "otbImage.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdRenderingFunction.h"
#include "Core/mvdStandardRenderingFunction.h"



namespace mvd
{
namespace Functor
{
/** \class RenderingFunctor
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
 *  \ingroup Visualization
 */
template <class TPixel, class TRGBPixel>
class RenderingFunctor
{
public:
  /** Rendering function typedef */
  typedef mvd::Function::RenderingFunction<TPixel, TRGBPixel> RenderingFunctionType;
  /** Rendering function pointer typedef */
  typedef typename RenderingFunctionType::Pointer RenderingFunctionPointerType;
  /** Default rendering function typedef */
  typedef TPixel                                            PixelType;
  typedef typename itk::NumericTraits<PixelType>::ValueType ScalarPixelType;
  typedef itk::VariableLengthVector<ScalarPixelType>        VectorPixelType;
  typedef itk::RGBPixel<ScalarPixelType>                    RGBPixelType;
  typedef itk::RGBAPixel<ScalarPixelType>                   RGBAPixelType;

  typedef itk::MetaDataDictionary MetaDataDictionaryType;

  /** Pixel operator */
  inline TRGBPixel operator ()(const PixelType& pixel) const
  {
    return m_Function->Evaluate(pixel);
  }

  /** Constructor */
  RenderingFunctor() : m_Function()
  {
    // Default rendering function
//     m_Function = DefaultRenderingFunctionType::New();
  }

  /** Destructor */
  virtual ~RenderingFunctor(){}

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
  void InitializeFunction(const MetaDataDictionaryType& metadatadictionary)
  {
    m_Function->Initialize(metadatadictionary);
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
template <class TInputImage, class TOutputImage = otb::Image<itk::RGBAPixel<unsigned char>, 2> >
class RenderingImageFilter
  : public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage,
      Functor::RenderingFunctor
      <typename TInputImage::PixelType,
          typename TOutputImage::PixelType> >
{
public:
  /** Standard typedefs */
  typedef RenderingImageFilter Self;
  typedef itk::UnaryFunctorImageFilter
  <TInputImage, TOutputImage, Functor::RenderingFunctor
      <typename TInputImage::PixelType,
          typename TOutputImage::PixelType> >          Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(RenderingImageFilter, itk::UnaryFunctorImageFilter);

  /** Rendering function typedef */
  typedef Functor::RenderingFunctor
  <typename TInputImage::PixelType,
      typename TOutputImage::PixelType>                         RenderingFunctorType;
  typedef typename RenderingFunctorType::RenderingFunctionType RenderingFunctionType;

  /**
   * Set the rendering function
   * \param function The rendering function.
   */
  void SetRenderingFunction(RenderingFunctionType * function)
  {
    this->GetFunctor().SetFunction(function);
    this->Modified();
  }

  /**
   * Get the rendering function
   * \return The rendering function.
   */
  RenderingFunctionType * GetRenderingFunction(void)
  {
    if (this->GetFunctor().GetFunction() == NULL)
      {
      this->SetDefaultRenderingFunction();
      }
    return this->GetFunctor().GetFunction();
  }

  /**
   * Initialize the function before any processing
   */
  virtual void BeforeThreadedGenerateData(void)
  {
    // Call the superclass implementation
    Superclass::BeforeThreadedGenerateData();

    if (this->GetFunctor().GetFunction() == NULL)
      {
      otbMsgDevMacro(<< "RenderingFunction set to default");
      this->SetDefaultRenderingFunction();
      }

    // Initialize the rendering function
    this->GetFunctor().InitializeFunction(this->GetInput()->GetMetaDataDictionary());

    otbMsgDevMacro(<< "RenderingImageFilter::BeforeThreadedGenerateData():");
    otbMsgDevMacro(<< " - Output functor size "
                   << (this->GetFunctor().GetFunction())->GetPixelRepresentationSize());
    otbMsgDevMacro(<< "Rendering Funtion:" << this->GetFunctor().GetFunction());

    //Check if the rendering function channels are compatible with the image
    //might want to be more generic here one day.
//     unsigned int numberOfInputChannels = this->GetInput()->GetNumberOfComponentsPerPixel();
    itk::ImageRegionConstIterator<TInputImage> it(this->GetInput(), this->GetInput()->GetBufferedRegion());
    unsigned int                               numberOfInputChannels = VisualizationPixelTraits::PixelSize(it.Get());
    std::vector<unsigned int>                  channels = (this->GetFunctor().GetFunction())->GetChannelList();
    for (unsigned int i = 0; i < channels.size(); ++i)
      {
      if (channels[i] >= numberOfInputChannels)
        {
        itkExceptionMacro(<< "Channels specified as input (" << channels[i] << ") is not compatible "
                          << "with the size of the image: " << numberOfInputChannels);
        }
      }

  }

protected:
  /** Constructor */
  RenderingImageFilter() {}
  /** Destructor */
  virtual ~RenderingImageFilter() {}

  typedef Function::StandardRenderingFunction<
      typename TInputImage::PixelType,
      typename TOutputImage::PixelType> DefaultRenderingFunctionType;

  void SetDefaultRenderingFunction()
  {
    otbMsgDevMacro(<< "WARNING: using the default rendering function");

    this->GetFunctor().SetFunction(DefaultRenderingFunctionType::New());
  }

private:
  RenderingImageFilter(const Self&); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};
} // end namespace mvd

#endif
