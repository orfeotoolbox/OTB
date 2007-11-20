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
#ifndef __otbReflectanceToSurfaceReflectanceImageFilter_h
#define __otbReflectanceToSurfaceReflectanceImageFilter_h


#include "otbAtmosphericRadiativeTerms.h"
#include "otbUnaryImageFunctorWithVectorImageFilter.h"


namespace otb
{
  namespace Functor
    {
      /** Functor::LuminanceToReflectanceImageFunctor
       *  Compute the surface reflectance pixel from a TOA reflectance.
       *
       * \ingroup Functor
       */
      template <class TInput, class TOutput>
	class ReflectanceToSurfaceReflectanceImageFunctor
	{
	public:
	  ReflectanceToSurfaceReflectanceImageFunctor() 
	    {
	     m_Coefficient = 1.;
	     m_Residu = 1.;
	     m_SphericalAlbedo = 1.;
	    };
	  ~ReflectanceToSurfaceReflectanceImageFunctor() {};

	  /**
	   * Set/Get the spherical albedo of the atmosphere.
	   */
	  void SetSphericalAlbedo(double albedo){ m_SphericalAlbedo=albedo;}; 
	  double GetSphericalAlbedo(){ return m_SphericalAlbedo; }; 

	  /**
	   * Set/Get Coefficient, computed from AtmosphericRadiativeTermsPointerType datas.
	   */
	  void SetCoefficient(double coef){ m_Coefficient = coef; }; 
	  double GetCoefficient(){ return m_Coefficient; };
	  
	  /**
	   * Set/Get Residu, computed from AtmosphericRadiativeTermsPointerType datas.
	   */
	  void SetResidu(double res){ m_Residu = res; }; 
	  double GetResidu(){ return m_Residu; };

	  inline TOutput operator() (const TInput & inPixel) 
	    {
	      TOutput outPixel;
	      double temp;
	      temp = static_cast<TOutput>(inPixel)*static_cast<TOutput>(m_Coefficient) + static_cast<TOutput>(m_Residu);
	      outPixel = temp / (1 + static_cast<TOutput>(m_SphericalAlbedo) *  temp);
	
	      return outPixel;
	    }

	private:
	  double m_SphericalAlbedo;
	  double m_Coefficient;
	  double m_Residu;
	};
    }
  /** \class ReflectanceToSurfaceReflectanceImageFilter
   *  \brief Calculates the slope, the orientation incidence and exitance radius values for each pixel.
   *
   * 
   * \ingroup AtmosphericRadiativeTerms
   * \ingroup AtmosphericCorrectionParameters
   * \ingroup LuminanceToReflectanceImageFilter
   * \ingroup ImageToReflectanceImageFilter
   */
  template <class TInputImage, class TOutputImage >
    class ITK_EXPORT ReflectanceToSurfaceReflectanceImageFilter : 
    public UnaryImageFunctorWithVectorImageFilter< TInputImage,
                                                   TOutputImage,
                                                   ITK_TYPENAME Functor::ReflectanceToSurfaceReflectanceImageFunctor< ITK_TYPENAME TInputImage::InternalPixelType, 
                                                                                                                      ITK_TYPENAME TOutputImage::InternalPixelType > >
    {
    public:
      /** Extract input and output images dimensions.*/
      itkStaticConstMacro( InputImageDimension, unsigned int, TInputImage::ImageDimension);
      itkStaticConstMacro( OutputImageDimension, unsigned int, TOutputImage::ImageDimension);
      
      /** "typedef" to simplify the variables definition and the declaration. */
      typedef TInputImage         InputImageType;
      typedef TOutputImage        OutputImageType;
      typedef typename Functor::ReflectanceToSurfaceReflectanceImageFunctor<ITK_TYPENAME InputImageType::InternalPixelType, 
	                                                                    ITK_TYPENAME OutputImageType::InternalPixelType> FunctorType;
      /** "typedef" for standard classes. */
      typedef ReflectanceToSurfaceReflectanceImageFilter                                                     Self;
      typedef UnaryImageFunctorWithVectorImageFilter< InputImageType, OutputImageType, FunctorType >    Superclass; 
      typedef itk::SmartPointer<Self>                                                                        Pointer;
      typedef itk::SmartPointer<const Self>                                                                  ConstPointer;
      
      /** object factory method. */
      itkNewMacro(Self);
      
      /** return class name. */
      itkTypeMacro(ReflectanceToSurfaceReflectanceImageFilter, UnaryImageFunctorWithVectorImageFilter);
      
      /** Supported images definition. */
      typedef typename InputImageType::PixelType                           InputPixelType;
      typedef typename InputImageType::InternalPixelType                   InputInternalPixelType;
      typedef typename InputImageType::RegionType                          InputImageRegionType;
      typedef typename OutputImageType::PixelType                          OutputPixelType;
      typedef typename OutputImageType::InternalPixelType                  OutputInternalPixelType;
      typedef typename OutputImageType::RegionType                         OutputImageRegionType;

      typedef AtmosphericRadiativeTerms::Pointer                           AtmosphericRadiativeTermsPointerType;

      /** Get/Set Atmospheric Radiative Terms. */
      void SetAtmosphericRadiativeTerms(AtmosphericRadiativeTermsPointerType atmo)
	{
	  m_AtmosphericRadiativeTerms = atmo;
	  this->SetNthInput(1, m_AtmosphericRadiativeTerms);
	  this->Modified();
	}
      AtmosphericRadiativeTermsPointerType GetAtmosphericRadiativeTerms()
	{
	  return m_AtmosphericRadiativeTerms;
	}

       
    protected:
      ReflectanceToSurfaceReflectanceImageFilter();
      virtual ~ReflectanceToSurfaceReflectanceImageFilter();
   
      /** Initialize some accumulators before the threads run. */
      void BeforeThreadedGenerateData ();
      /** ImageToLuminanceImageFilter can be implemented as a multithreaded filter.
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
      AtmosphericRadiativeTermsPointerType m_AtmosphericRadiativeTerms;
    };
  
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbReflectanceToSurfaceReflectanceImageFilter.txx"
#endif

#endif
