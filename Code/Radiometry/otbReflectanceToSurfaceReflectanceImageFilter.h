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
        double temp, temp2;
        temp = static_cast<double>(inPixel)*m_Coefficient + m_Residu;
        temp2 =  temp / (1. + m_SphericalAlbedo *  temp);
        outPixel = static_cast<TOutput>(temp2);


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
      /** Constructor */
      ReflectanceToSurfaceReflectanceImageFilter()
  {
    m_AtmosphericRadiativeTerms = AtmosphericRadiativeTerms::New();
  };
      /** Destructor */
      virtual ~ReflectanceToSurfaceReflectanceImageFilter(){};

      /** Initialize the functor vector */
      void BeforeThreadedGenerateData ()
  {
    this->GetFunctorVector().clear();
    for(unsigned int i = 0;i<this->GetInput()->GetNumberOfComponentsPerPixel();++i)
      {
        double coef;
        double res;
        coef = static_cast<double>(m_AtmosphericRadiativeTerms->GetTotalGaseousTransmission(i)
           * m_AtmosphericRadiativeTerms->GetDownwardTransmittance(i)
           * m_AtmosphericRadiativeTerms->GetUpwardTransmittance(i)     );
        coef = 1. / coef;
        res = -m_AtmosphericRadiativeTerms->GetIntrinsicAtmosphericReflectance(i) * coef;

        FunctorType functor;
        functor.SetCoefficient(coef);
        functor.SetResidu(res);
        functor.SetSphericalAlbedo(static_cast<double>(m_AtmosphericRadiativeTerms->GetSphericalAlbedo(i)));

        this->GetFunctorVector().push_back(functor);
      }
  }

    private:
      AtmosphericRadiativeTermsPointerType m_AtmosphericRadiativeTerms;
    };

} // end namespace otb

#endif
