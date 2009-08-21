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


#include "otbUnaryImageFunctorWithVectorImageFilter.h"

#include "otbAtmosphericRadiativeTerms.h"
#include "otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms.h"
#include "itkMetaDataDictionary.h"

namespace otb
{
namespace Functor
{
/**
   * \class ReflectanceToSurfaceReflectanceImageFunctor
   *  \brief Compute the surface reflectance pixel from a TOA reflectance.
   *
   * \ingroup Functor
   * \ingroup Radiometry
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
  void SetSphericalAlbedo(double albedo)
  {
    m_SphericalAlbedo=albedo;
  };
  double GetSphericalAlbedo()
  {
    return m_SphericalAlbedo;
  };

  /**
   * Set/Get Coefficient, computed from AtmosphericRadiativeTermsPointerType datas.
   */
  void SetCoefficient(double coef)
  {
    m_Coefficient = coef;
  };
  double GetCoefficient()
  {
    return m_Coefficient;
  };

  /**
   * Set/Get Residu, computed from AtmosphericRadiativeTermsPointerType datas.
   */
  void SetResidu(double res)
  {
    m_Residu = res;
  };
  double GetResidu()
  {
    return m_Residu;
  };

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
 * \ingroup Radiometry
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
  typedef typename InputImageType::PixelType                            InputPixelType;
  typedef typename InputImageType::InternalPixelType                    InputInternalPixelType;
  typedef typename InputImageType::RegionType                           InputImageRegionType;
  typedef typename OutputImageType::PixelType                           OutputPixelType;
  typedef typename OutputImageType::InternalPixelType                   OutputInternalPixelType;
  typedef typename OutputImageType::RegionType                          OutputImageRegionType;

  typedef AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms  Parameters2RadiativeTermsType;
  typedef Parameters2RadiativeTermsType::Pointer                        Parameters2RadiativeTermsPointerType;
  typedef AtmosphericCorrectionParameters::Pointer                      CorrectionParametersPointerType;
  typedef AtmosphericRadiativeTerms::Pointer                            AtmosphericRadiativeTermsPointerType;

  typedef itk::MetaDataDictionary                                       MetaDataDictionaryType;

  /** Get/Set Atmospheric Radiative Terms. */
  void SetAtmosphericRadiativeTerms(AtmosphericRadiativeTermsPointerType atmo)
  {
    m_AtmosphericRadiativeTerms = atmo;
    this->SetNthInput(1, m_AtmosphericRadiativeTerms);
    m_IsSetAtmosphericRadiativeTerms = true;
    this->Modified();
  }

  /** Get/Set Atmospheric Correction Parameters. */
  itkGetConstObjectMacro(AtmosphericRadiativeTerms, AtmosphericRadiativeTerms);
  itkGetObjectMacro(CorrectionParameters, AtmosphericCorrectionParameters);

  /** Get/Set Aeronet file name. */
  itkSetMacro(AeronetFileName, std::string);
  itkGetMacro(AeronetFileName, std::string);

protected:
  /** Constructor */
  ReflectanceToSurfaceReflectanceImageFilter();
  /** Destructor */
  virtual ~ReflectanceToSurfaceReflectanceImageFilter() {};

  /** If empty, fill AtmosphericRadiativeTerms using image metadata*/
  void UpdateAtmosphericRadiativeTerms( const MetaDataDictionaryType dict );

  /** Read the aeronet data and extract aerosol optical and water vapor amount. */
  void UpdateAeronetData( const MetaDataDictionaryType dict );

  /** Initialize the functor vector */
  void BeforeThreadedGenerateData();
 
  
private:
  AtmosphericRadiativeTermsPointerType m_AtmosphericRadiativeTerms;
  CorrectionParametersPointerType      m_CorrectionParameters;
  bool m_IsSetAtmosphericRadiativeTerms;
  /** Path to an Aeronet data file, allows to compute aerosol optical and water vapor amounts. */
  std::string m_AeronetFileName;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbReflectanceToSurfaceReflectanceImageFilter.txx"
#endif

#endif
