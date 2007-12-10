/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  
  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbImageToReflectanceImageFilter_h
#define __otbImageToReflectanceImageFilter_h

#include "otbUnaryImageFunctorWithVectorImageFilter.h"
#include "otbImageToLuminanceImageFilter.h"
#include "otbLuminanceToReflectanceImageFilter.h"
#include "otbVectorImage.h"
#include "itkNumericTraits.h"
#include "itkVariableLengthVector.h"


namespace otb
{
  namespace Functor
    {
      /** Functor::ImageToReflectanceImageFunctor
       *  Call the ImageToLuminanceFunctor over the input and the LuminanceToReflectanceFunctor to this result.
       *
       * \ingroup Functor
       * \ingroup ImageToLuminanceFunctor
       * \ingroup LuminanceToReflectanceFunctor
       */
      template <class TInput, class TOutput>
	class ImageToReflectanceImageFunctor
	{
	public:
	  ImageToReflectanceImageFunctor() {};
	  ~ImageToReflectanceImageFunctor() {};

	  typedef Functor::ImageToLuminanceImageFunctor<TInput, TOutput>        ImToLumFunctorType;
	  typedef Functor::LuminanceToReflectanceImageFunctor<TInput, TOutput>  LumToReflecFunctorType;
 
	  void SetAlpha(double alpha){ m_ImToLumFunctor.SetAlpha(alpha); };
	  void SetBeta(double beta){ m_ImToLumFunctor.SetBeta(beta); };
	  void SetSolarIllumination(double solarIllumination){ m_LumToReflecFunctor.SetSolarIllumination(solarIllumination); };
	  void SetIlluminationCorrectionCoefficient(double coef){ m_LumToReflecFunctor.SetIlluminationCorrectionCoefficient(coef); };
	  
	  double GetAlpha(){ return m_ImToLumFunctor.GetAlpha();};
	  double GetBeta(){ return m_ImToLumFunctor.GetBeta();};
	  double GetSolarIllumination(){ return  m_LumToReflecFunctor.GetSolarIllumination();};
	  double GetIlluminationCorrectionCoefficient(){ return m_LumToReflecFunctor.GetIlluminationCorrectionCoefficient();};

	  inline TOutput operator() (const TInput & inPixel) 
	    {
	      TOutput outPixel;
	      TOutput tempPix;
	      tempPix = m_ImToLumFunctor(inPixel);
 	      outPixel = m_LumToReflecFunctor(tempPix); 

	      return outPixel;
	    }

	private:
	  ImToLumFunctorType m_ImToLumFunctor;
	  LumToReflecFunctorType m_LumToReflecFunctor;


	};
    }

/** \class ImageToReflectanceImageFilter
   *  \brief Transform a classical image into the reflectance image. For this it uses the functor ImageToReflectanceFunctor calling for each component of each pixel. 
   *
   *  Transform a classical image into the reflectance image. For this it uses the functor ImageToReflectanceFunctor calling for each component of each pixel.
   *  The flux normalization coefficient (that is the ratio solar distance over mean solar distance) can be directly set or the user can 
   *  give the day and the mounth of the observation and the class will used a coefficient given by a 6S routine that will give the corresponding coefficient. 
   *  To note that in the case, 6S gives the square of the distances ratio.
   *  
   * \ingroup ImageToReflectanceImageFunctor
   * \ingroup ImageToLuminanceImageFilter
   * \ingroup ReflectanceToLuminanceImageFilter
   */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ImageToReflectanceImageFilter : 
public UnaryImageFunctorWithVectorImageFilter< TInputImage,
                                               TOutputImage,
                                               ITK_TYPENAME Functor::ImageToReflectanceImageFunctor< ITK_TYPENAME TInputImage::InternalPixelType, 
                                                                                                   ITK_TYPENAME TOutputImage::InternalPixelType > >
{
public:
/** 	Extract input and output images dimensions.*/
  itkStaticConstMacro( InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro( OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** "typedef" to simplify the variables definition and the declaration. */
  typedef TInputImage         InputImageType;
  typedef TOutputImage        OutputImageType;
  typedef typename Functor::ImageToReflectanceImageFunctor<ITK_TYPENAME InputImageType::InternalPixelType, 
                                                         ITK_TYPENAME OutputImageType::InternalPixelType> FunctorType;


  /** "typedef" for standard classes. */
  typedef ImageToReflectanceImageFilter Self;
  typedef UnaryImageFunctorWithVectorImageFilter< InputImageType, OutputImageType, FunctorType > Superclass; 
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** object factory method. */
  itkNewMacro(Self);

  /** return class name. */
  itkTypeMacro(ImageToReflectanceImageFilter, UnaryImageFunctorWithVectorImageFiltermageFilter);
  
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

   /** Set the absolute calibration gains. */
  itkSetMacro(Alpha, VectorType);
  /** Give the absolute calibration gains. */
  itkGetConstReferenceMacro(Alpha, VectorType);

  /** Set the absolute calibration bias. */
  itkSetMacro(Beta, VectorType);
  /** Give the absolute calibration bias. */
  itkGetConstReferenceMacro(Beta, VectorType);
      /** Set the solar illumination value. */
  itkSetMacro(SolarIllumination, VectorType);
  
  /** Give the solar illumination value. */
  itkGetConstReferenceMacro(SolarIllumination, VectorType);

  /** Set the zenithal solar radius. */
  itkSetMacro(ZenithalSolarRadius, double);
  /** Give the zenithal solar radius. */
  itkGetConstReferenceMacro(ZenithalSolarRadius, double);
   
  /** Set the flux normalization coefficient. */
  void SetFluxNormalizationCoefficient(double coef)
    {
      m_FluxNormalizationCoefficient = coef;
      m_IsSetFluxNormalizationCoefficient = true;
      this->Modified();
    };
  
    /** Set the acquisition day. */
    itkSetMacro(Day, int);
    /** Get the acquisition day. */
    itkGetConstReferenceMacro(Day, int);
    /** Set the acquisition mounth. */
	  itkSetMacro(Month, int);
	  /** Set the  acquisition mounth. */
    itkGetConstReferenceMacro(Month, int);
	  
  
 protected:
  ImageToReflectanceImageFilter()
    {
      m_Alpha.SetSize(1);
      m_Alpha.Fill(0);
      m_Beta.SetSize(1);
      m_Beta.Fill(0);
      m_ZenithalSolarRadius = 1.;
      m_FluxNormalizationCoefficient = 1.;
      m_SolarIllumination.Fill(1.);
      m_IsSetFluxNormalizationCoefficient = false;
      m_Day = 1;
      m_Month = 1;

    };
  virtual ~ImageToReflectanceImageFilter(){};

  virtual void BeforeThreadedGenerateData(void)
    {
      this->GetFunctorVector().clear();
      for(unsigned int i = 0;i<this->GetInput()->GetNumberOfComponentsPerPixel();++i)
	{
	  FunctorType functor;
	  double coefTemp = 0.;  
	  if (!m_IsSetFluxNormalizationCoefficient)
	    {
	      if (m_Day*m_Month != 0 && m_Day<32 && m_Month<12)
		{
		  otb_6s_real dsol = 0.;
		  otb_6s_integer day = static_cast<otb_6s_integer>(m_Day);
		  otb_6s_integer mounth = static_cast<otb_6s_integer>(m_Month);
		  int cr(0);
		  cr = otb_6s_varsol_(&day, &mounth, &dsol);
		  coefTemp = vcl_cos(m_ZenithalSolarRadius*M_PI/180)*static_cast<double>(dsol);
		}
	      else
		{
		  itkExceptionMacro( << "Day has to be included between 1 and 31, Month beetween 1 and 12.");
		}
	    }
	  else
	    {
	      coefTemp = vcl_cos(m_ZenithalSolarRadius*M_PI/180)*m_FluxNormalizationCoefficient*m_FluxNormalizationCoefficient;
	    }
	  functor.SetIlluminationCorrectionCoefficient(1. / coefTemp);
	  functor.SetAlpha(m_Alpha[i]);
	  functor.SetBeta(m_Beta[i]);
	  functor.SetSolarIllumination(m_SolarIllumination[i]);
	  this->GetFunctorVector().push_back(functor);
	}
    }
  
                        
private:
  /** Ponderation declaration*/
  VectorType m_Alpha;
  VectorType m_Beta;
  /** Set the zenithal soalr radius. */
  double m_ZenithalSolarRadius;
  /** Flux normalization coefficient. */
  double m_FluxNormalizationCoefficient;
  /** Solar illumination value. */
  VectorType m_SolarIllumination;
  /** Used to know if the user has set a value for the FluxNormalizationCoefficient parameter
   * or if the class has to compute it */
  bool m_IsSetFluxNormalizationCoefficient;
  /** Acquisition Day*/
  int m_Day;
  /** Acquisition Month*/
  int m_Month;
};

} // end namespace otb

#endif
