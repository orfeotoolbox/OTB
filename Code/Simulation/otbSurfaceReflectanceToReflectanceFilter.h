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
#ifndef __otbSurfaceReflectanceToReflectanceFilter_h
#define __otbSurfaceReflectanceToReflectanceFilter_h


#include "otbUnaryImageFunctorWithVectorImageFilter.h"

#include "otbAtmosphericRadiativeTerms.h"
#include "otbRadiometryCorrectionParametersToAtmosphericRadiativeTerms.h" 
#include "otbImageMetadataCorrectionParameters.h"
#include "itkMetaDataDictionary.h"
// #include "itkFactory.h"
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

  /** "typedef" for standard classes. */
  typedef ReflectanceToSurfaceReflectanceImageFunctor   Self;

  ReflectanceToSurfaceReflectanceImageFunctor()
  {
    m_Coefficient = 1.;
    m_Residu = 1.;
    m_SphericalAlbedo = 1.;
  };
  virtual ~ReflectanceToSurfaceReflectanceImageFunctor() {};

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
   * Set/Get Coefficient, computed from AtmosphericRadiativeTermsPointerType data.
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
   * Set/Get Residu, computed from AtmosphericRadiativeTermsPointerType data.
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
//     std::cout << "m_Coefficient "<< m_Coefficient << std::endl;
//     std::cout << "m_Residu "<< m_Residu << std::endl;

    TOutput outPixel;
//     outPixel.first = inPixel.first;
    double temp, temp1, temp2;
//     temp = static_cast<double>(inPixel)*m_Coefficient + m_Residu;
//     temp2 =  temp / (1. + m_SphericalAlbedo *  temp);

    temp = 1. - (static_cast<double>(inPixel) * m_SphericalAlbedo);
    //temp1 =  1/temp;

//     std::cout << "------------------------------" << std::endl;
//     std::cout << "B "<< temp << std::endl;
//     std::cout << "B*residu "<< temp*m_Residu << std::endl;
//     std::cout << "m_Coefficient*static_cast<double>(inPixel) "<< m_Coefficient*static_cast<double>(inPixel) << std::endl;
//     std::cout << "albedo "<< m_SphericalAlbedo << std::endl;
//     std::cout << "all gaz "<< m_Coefficient << std::endl;
//     std::cout << "residu "<< m_Residu << std::endl;
    //std::cout << "temp2 "<< temp2 << std::endl;
    temp2 = m_Residu;
//     std::cout << "temp2 "<< temp2 << std::endl;
    temp1 = 1. / temp;
//     std::cout << "temp1 "<< temp1 << std::endl;
    temp2 += temp1*m_Coefficient*static_cast<double>(inPixel);
//     std::cout << "1-row*S "<< temp << std::endl;
//     outPixel = vcl_fabs (static_cast<TOutput>(temp2));
    outPixel = static_cast<TOutput>(temp2);

//     std::cout << "in out "<< static_cast<double>(inPixel) << "; " << static_cast<double>(outPixel)<< std::endl;
//     std::cout <<"plot " << m_Residu << "+" << m_Coefficient << "*(x/(1-x*" << m_SphericalAlbedo << "))" << std::endl;
//     std::cout << "------------------------------" << std::endl;
    return outPixel;
  }
  /** Compare two indices. */
  bool operator==(const Self &func) const
  {
    return ( (m_SphericalAlbedo == func.m_SphericalAlbedo) && (m_Coefficient == func.m_Coefficient) && (m_Residu == func.m_Residu) );
  }

  /** Compare two indices. */
  bool operator!=(const Self &func) const
  {
    return ( (m_SphericalAlbedo != func.m_SphericalAlbedo) && (m_Coefficient != func.m_Coefficient) && (m_Residu != func.m_Residu) );
  }

private:
  double m_SphericalAlbedo;
  double m_Coefficient;
  double m_Residu;
};
}
/** \class SurfaceReflectanceToReflectanceFilter
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
class ITK_EXPORT SurfaceReflectanceToReflectanceFilter :
      public UnaryImageFunctorWithVectorImageFilter< TInputImage,
      TOutputImage,
      typename Functor::ReflectanceToSurfaceReflectanceImageFunctor< typename TInputImage::InternalPixelType,
      typename TOutputImage::InternalPixelType > >
{
public:
  /** Extract input and output images dimensions.*/
  itkStaticConstMacro( InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro( OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** "typedef" to simplify the variables definition and the declaration. */
  typedef TInputImage         InputImageType;
  typedef TOutputImage        OutputImageType;
  typedef typename Functor::ReflectanceToSurfaceReflectanceImageFunctor<typename InputImageType::InternalPixelType,
  typename OutputImageType::InternalPixelType> FunctorType;
  /** "typedef" for standard classes. */
  typedef SurfaceReflectanceToReflectanceFilter                                                     Self;
  typedef UnaryImageFunctorWithVectorImageFilter< InputImageType, OutputImageType, FunctorType >    Superclass;
  typedef itk::SmartPointer<Self>                                                                        Pointer;
  typedef itk::SmartPointer<const Self>                                                                  ConstPointer;

  /** object factory method. */
  itkNewMacro(Self);

  /** return class name. */
  itkTypeMacro(SurfaceReflectanceToReflectanceFilter, UnaryImageFunctorWithVectorImageFilter);

  /** Supported images definition. */
  typedef typename InputImageType::PixelType                            InputPixelType;
  typedef typename InputImageType::InternalPixelType                    InputInternalPixelType;
  typedef typename InputImageType::RegionType                           InputImageRegionType;
  typedef typename OutputImageType::PixelType                           OutputPixelType;
  typedef typename OutputImageType::InternalPixelType                   OutputInternalPixelType;
  typedef typename OutputImageType::RegionType                          OutputImageRegionType;

  typedef otb::RadiometryCorrectionParametersToAtmosphericRadiativeTerms     CorrectionParametersToRadiativeTermsType;

  typedef otb::AtmosphericCorrectionParameters                               AtmoCorrectionParametersType;
  typedef typename AtmoCorrectionParametersType::Pointer                    AtmoCorrectionParametersPointerType;

  typedef otb::ImageMetadataCorrectionParameters                             AcquiCorrectionParametersType;
  typedef typename AcquiCorrectionParametersType::Pointer                   AcquiCorrectionParametersPointerType;

  typedef otb::AtmosphericRadiativeTerms                                     AtmosphericRadiativeTermsType;
  typedef typename AtmosphericRadiativeTermsType::Pointer                   AtmosphericRadiativeTermsPointerType;


  typedef otb::FilterFunctionValues                                     FilterFunctionValuesType;
  typedef FilterFunctionValuesType::WavelengthSpectralBandType          ValueType;                //float
  typedef FilterFunctionValuesType::ValuesVectorType                    ValuesVectorType;         //std::vector<float>

  typedef typename AcquiCorrectionParametersType::WavelengthSpectralBandVectorType        WavelengthSpectralBandVectorType;

  typedef itk::MetaDataDictionary MetaDataDictionaryType;


  /** Get/Set Atmospheric Radiative Terms. */
  void SetAtmosphericRadiativeTerms(AtmosphericRadiativeTermsPointerType atmoRadTerms) 
  {
    m_AtmosphericRadiativeTerms = atmoRadTerms;
    this->SetNthInput(1, m_AtmosphericRadiativeTerms); 
    m_IsSetAtmosphericRadiativeTerms = true;
    this->Modified();
  }
  itkGetObjectMacro(AtmosphericRadiativeTerms, AtmosphericRadiativeTermsType);

  /** Get/Set Atmospheric Correction Parameters. */
  void SetAtmoCorrectionParameters(AtmoCorrectionParametersPointerType atmoCorrTerms) 
  {
    m_AtmoCorrectionParameters = atmoCorrTerms;
    this->SetNthInput(2, m_AtmoCorrectionParameters); 
    m_IsSetAtmoCorrectionParameters = true; 
    this->Modified();
  }
  itkGetObjectMacro(AtmoCorrectionParameters, AtmoCorrectionParametersType);

  /** Get/Set Acquisition Correction Parameters. */
  void SetAcquiCorrectionParameters(AcquiCorrectionParametersPointerType acquiCorrTerms) 
  {
    m_AcquiCorrectionParameters = acquiCorrTerms;
    this->SetNthInput(3, m_AcquiCorrectionParameters); 
    m_IsSetAcquiCorrectionParameters = true;
    this->Modified();
  }


  /** Compute radiative terms if necessary and then updtae functors attibuts. */
  void GenerateParameters();

  /** Set/Get UseGenerateParameters. */
  itkSetMacro(UseGenerateParameters, bool);
  itkGetMacro(UseGenerateParameters, bool);

  /** Set/Get IsSetAtmosphericRadiativeTerms */
  itkSetMacro(IsSetAtmosphericRadiativeTerms, bool);
  itkGetMacro(IsSetAtmosphericRadiativeTerms, bool);


protected:
  /** Constructor */
  SurfaceReflectanceToReflectanceFilter();
  /** Destructor */
  virtual ~SurfaceReflectanceToReflectanceFilter() {};


  /** Initialize the functor vector */
  void BeforeThreadedGenerateData();
  /** Fill AtmosphericRadiativeTerms using image metadata*/
  void UpdateAtmosphericRadiativeTerms();
  /** Update Functors parameters */
  void UpdateFunctors();

  /** If modified, we need to compute the functor parameters again */
  virtual void Modified();

private:

  bool m_IsSetAtmosphericRadiativeTerms;
  bool m_IsSetAtmoCorrectionParameters;
  bool m_IsSetAcquiCorrectionParameters;

  /** Radiative terms object */
  AtmosphericRadiativeTermsPointerType     m_AtmosphericRadiativeTerms;
  AtmoCorrectionParametersPointerType      m_AtmoCorrectionParameters;
  AcquiCorrectionParametersPointerType     m_AcquiCorrectionParameters;

  bool m_UseGenerateParameters;

  /** True if the functor parameters have been generated */
  bool m_FunctorParametersHaveBeenComputed;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSurfaceReflectanceToReflectanceFilter.txx"
#endif

#endif
