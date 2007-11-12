/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  This code is from a Julien Radoux contribution.
  
  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbLuminanceToReflectanceImageFilter_h
#define __otbLuminanceToReflectanceImageFilter_h

#include "otbUnaryImageFunctorWithVectorImageFilter.h"
#include "otbVectorImage.h"
#include "otbMath.h"
#include "itkNumericTraits.h"
#include "itkVariableLengthVector.h"


namespace otb
{
  namespace Functor
    {
      /** Functor::LuminanceToReflectanceImageFunctor
       *  Multiply by Pi and by an illumination correction coefficient the qutient between the input and the given solar illumination.
       *
       * \ingroup Functor
       */
      template <class TInput, class TOutput>
	class LuminanceToReflectanceImageFunctor
	{
	public:
	  LuminanceToReflectanceImageFunctor() 
	    {
	      m_SolarIllumination = 1.;
	      m_IlluminationCorrectionCoefficient = 1.;
	    };
	  ~LuminanceToReflectanceImageFunctor() {};

	  void SetSolarIllumination(double solarIllumination){ m_SolarIllumination = solarIllumination;};
	  void SetIlluminationCorrectionCoefficient(double coef){ m_IlluminationCorrectionCoefficient = coef;};
	  
	  double GetSolarIllumination(){ return m_SolarIllumination;};
	  double GetIlluminationCorrectionCoefficient(){ return m_IlluminationCorrectionCoefficient;};

	  inline TOutput operator() (const TInput & inPixel) 
	    {
	      TOutput outPixel; 
 	      outPixel = static_cast<TOutput>(inPixel) 
		         * static_cast<TOutput>(M_PI) 
                         * static_cast<TOutput>(m_IlluminationCorrectionCoefficient)
		         / static_cast<TOutput>(m_SolarIllumination); 

	      return outPixel;
	    }

	private:
	  double m_SolarIllumination;
	  double m_IlluminationCorrectionCoefficient;
	};
    }

  /** \class LuminanceToReflectanceImageFilter
   *  \brief Transform a luminance image into the reflectance. For this it uses the functor LuminanceToReflectanceImaeFunctor 
   *   calling for each component of each pixel. 
   *
   * \ingroup ImageToLuminanceImageFunctor
   */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT LuminanceToReflectanceImageFilter : 
public UnaryImageFunctorWithVectorImageFilter< TInputImage,
                                               TOutputImage,
                                               ITK_TYPENAME Functor::LuminanceToReflectanceImageFunctor< ITK_TYPENAME TInputImage::InternalPixelType, 
                                                                                                   ITK_TYPENAME TOutputImage::InternalPixelType > >
{
public:
/** 	Extract input and output images dimensions.*/
  itkStaticConstMacro( InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro( OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** "typedef" to simplify the variables definition and the declaration. */
  typedef TInputImage         InputImageType;
  typedef TOutputImage        OutputImageType;
  typedef typename Functor::LuminanceToReflectanceImageFunctor<ITK_TYPENAME InputImageType::InternalPixelType, 
                                                         ITK_TYPENAME OutputImageType::InternalPixelType> FunctorType;


  /** "typedef" for standard classes. */
  typedef LuminanceToReflectanceImageFilter Self;
  typedef UnaryImageFunctorWithVectorImageFilter< InputImageType, OutputImageType, FunctorType > Superclass; 
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** object factory method. */
  itkNewMacro(Self);

  /** return class name. */
  itkTypeMacro(LuminanceToReflectanceImageFilter, UnaryImageFunctorWithVectorImageFiltermageFilter);
  
  /** Supported images definition. */
  typedef typename InputImageType::PixelType                           InputPixelType;
  typedef typename InputImageType::InternalPixelType                   InputInternalPixelType;
  typedef typename InputImageType::RegionType                          InputImageRegionType;
  typedef typename OutputImageType::PixelType                          OutputPixelType;
  typedef typename OutputImageType::InternalPixelType                  OutputInternalPixelType;
  typedef typename OutputImageType::RegionType                         OutputImageRegionType;


  typedef typename itk::VariableLengthVector<double>                   VectorType;
 
  /** Image size "typedef" definition. */
  typedef typename InputImageType::SizeType SizeType;

   /** Set the solar illumination value. */
  itkSetMacro(SolarIllumination, VectorType);
  /** Give the solar illumination value. */
  itkGetConstReferenceMacro(SolarIllumination, VectorType);

  /** Set the zenithal solar radius. */
  itkSetMacro(ZenithalSolarRadius, double);
  /** Give the zenithal solar radius. */
  itkGetConstReferenceMacro(ZenithalSolarRadius, double);

 /** Set the day. */
  itkSetMacro(Day, int);
  /** Give the day. */
  itkGetConstReferenceMacro(Day, int);

 /** Set the mounth. */
  itkSetMacro(Mounth, int);
  /** Give the mounth. */
  itkGetConstReferenceMacro(Mounth, int);
   
    /** Set the flux normalization coefficient. */
  void SetFluxNormalizationCoefficient(double coef)
    {
      m_FluxNormalizationCoefficient = coef;
      m_IsSetFluxNormalizationCoefficient = true;
      this->Modified();
    };
  /** Give the flux normalization coefficient. */
  itkGetConstReferenceMacro(FluxNormalizationCoefficient, double);

  /** Set the IsSetFluxNormalizationCoefficient boolean. */
  itkSetMacro(IsSetFluxNormalizationCoefficient, bool);
  /** Give the IsSetFluxNormalizationCoefficient boolean. */
  itkGetConstReferenceMacro(IsSetFluxNormalizationCoefficient, bool);

 protected:
  LuminanceToReflectanceImageFilter();
  virtual ~LuminanceToReflectanceImageFilter();

  /** LuminanceToReflectanceImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData() routine
   * which is called for each processing thread. The output image data is
   * allocated automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread, int threadId);
  
                        
private:

  /** Set the zenithal soalr radius. */
  double m_ZenithalSolarRadius;
  /** Flux normalization coefficient. */
  double m_FluxNormalizationCoefficient;
  /* Acquisition day. */
  int m_Day;
  /* Acquisition mounth. */
  int m_Mounth;
  /** Solar illumination value. */
  VectorType m_SolarIllumination;
  /** Used to know if the user has set a value for the FluxNormalizationCoefficient parameter
   * or if the class has to compute it */
  bool m_IsSetFluxNormalizationCoefficient;
  
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLuminanceToReflectanceImageFilter.txx"
#endif

#endif
