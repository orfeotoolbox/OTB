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
#include "otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms.h"
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

  typedef AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms  Parameters2RadiativeTermsType;
  typedef Parameters2RadiativeTermsType::Pointer                        Parameters2RadiativeTermsPointerType;
  typedef AtmosphericCorrectionParameters::Pointer                      CorrectionParametersPointerType;
  typedef AtmosphericRadiativeTerms::Pointer                            AtmosphericRadiativeTermsPointerType;


  typedef FilterFunctionValues                                          FilterFunctionValuesType;
  typedef FilterFunctionValuesType::ValuesVectorType                    CoefVectorType;
  typedef std::vector<CoefVectorType>                                   FilterFunctionCoefVectorType;

  typedef itk::MetaDataDictionary                                       MetaDataDictionaryType;

  /** Get/Set Atmospheric Radiative Terms. */
  void SetAtmosphericRadiativeTerms(AtmosphericRadiativeTermsPointerType atmo)
  {
    m_AtmosphericRadiativeTerms = atmo;
    this->SetNthInput(1, m_AtmosphericRadiativeTerms);
    m_IsSetAtmosphericRadiativeTerms = true;
    this->Modified();
  }
  itkGetObjectMacro(AtmosphericRadiativeTerms, AtmosphericRadiativeTerms);

  /** Get/Set Atmospheric Correction Parameters. */
  itkSetObjectMacro(CorrectionParameters, AtmosphericCorrectionParameters);
  itkGetObjectMacro(CorrectionParameters, AtmosphericCorrectionParameters);

  /** Get/Set Aeronet file name. */
  itkSetMacro(AeronetFileName, std::string);
  itkGetMacro(AeronetFileName, std::string);

  /** Get/Set Aeronet file name. */
  itkSetMacro(FilterFunctionValuesFileName, std::string);
  itkGetMacro(FilterFunctionValuesFileName, std::string);

  /** Get/Set Filter function coef. */
  void SetFilterFunctionCoef( FilterFunctionCoefVectorType vect )
  {
         m_FilterFunctionCoef = vect;
         this->Modified();
  }
  FilterFunctionCoefVectorType GetFilterFunctionCoef()
  {
         return m_FilterFunctionCoef;
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

  /** Read the aeronet data and extract aerosol optical and water vapor amount. */
  //void UpdateAeronetData( const MetaDataDictionaryType dict );

  /** Initialize the functor vector */
  void GenerateOutputInformation();
  /** Fill AtmosphericRadiativeTerms using image metadata*/
  void UpdateAtmosphericRadiativeTerms();
  /** Update Functors parameters */
  void UpdateFunctors();

private:
  AtmosphericRadiativeTermsPointerType m_AtmosphericRadiativeTerms;
  CorrectionParametersPointerType      m_CorrectionParameters;
  bool m_IsSetAtmosphericRadiativeTerms;
  /** Path to an Aeronet data file, allows to compute aerosol optical and water vapor amounts. */
  std::string m_AeronetFileName;
  /** Path to a filter function values file. */
  std::string m_FilterFunctionValuesFileName;
  /** Contains the filter function values (each element is a vector and represnts the values for each channel) */
  FilterFunctionCoefVectorType m_FilterFunctionCoef;
  /** Enable/Disable GenerateParameters in GenerateOutputInformation.
   *  Usefull for image view that call GenerateOutputInformation each time you move the full area.
   */
  bool m_UseGenerateParameters;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSurfaceReflectanceToReflectanceFilter.txx"
#endif

#endif
