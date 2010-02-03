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
#ifndef __otbRadiometricAttributesLabelMapFilter_h
#define __otbRadiometricAttributesLabelMapFilter_h

#include "itkInPlaceLabelMapFilter.h"
#include "itkMatrix.h"
#include "itkVector.h"

#include "otbVegetationIndicesFunctor.h"
#include "otbSoilIndicesFunctor.h"
#include "otbWaterIndicesFunctor.h"
#include "otbMultiChannelRAndNIRIndexImageFilter.h"
#include "otbMultiChannelGAndRIndexImageFilter.h"
#include "otbMultiChannelRAndGAndNIRVegetationIndexImageFilter.h"
#include "otbVectorImageToIntensityImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbStatisticsAttributesLabelMapFilter.h"

namespace otb
{
/** \class RadiometricAttributesLabelMapFilter
 *  \brief This filter computes radiometric attributes for each object.
 *
 * Images are supposed to be standard 4-bands image (B,G,R,NIR). The
 * index of each channel can be set via the Set***ChannelIndex()
 * accessors.
 *
 * This filter internally applies the
 * StatisticsAttributesLabelMapFilter to the following features: GEMI,
 * NDVI, IR, IC, IB, NDWI2, Intensity and original B, G, R and NIR channels.
 *
 * The feature name is constructed as:
 * 'STATS' + '::' + RadiometricFeatureName + '::' + StatisticName
 * For example, accessing the mean of NDVI on a LabelObject with the attribute: 'STATS::Ndvi::Mean'    
 *
 * The ReducedAttributesSet flag allows to tell the internal
 * statistics filter to compute only the main attributes.
 *
 * \sa StatisticsAttributesImageFilter AttributesMapLabelObject
 * \sa GEMI NDVI IR IC IB NDWI2
 * \sa VectorIntensityImageFilter
 *
 * \ingroup ImageEnhancement MathematicalMorphologyImageFilters
 */
template<class TImage, class TFeatureImage>
class ITK_EXPORT RadiometricAttributesLabelMapFilter :
    public itk::InPlaceLabelMapFilter<TImage>
{
public:
  /** Standard class typedefs. */
  typedef RadiometricAttributesLabelMapFilter            Self;
  typedef itk::InPlaceLabelMapFilter<TImage>             Superclass;
  typedef itk::SmartPointer<Self>                        Pointer;
  typedef itk::SmartPointer<const Self>                  ConstPointer;

  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int,TImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(RadiometricAttributesLabelMapFilter,InPlaceLabelMapFilter);

  /** Some convenient typedefs. */
  typedef TImage ImageType;
  typedef typename ImageType::Pointer                    ImagePointer;
  typedef typename ImageType::ConstPointer               ImageConstPointer;
  typedef typename ImageType::PixelType                  PixelType;
  typedef typename ImageType::IndexType                  IndexType;
  typedef typename ImageType::PointType                  PointType;
  typedef typename ImageType::LabelObjectType            LabelObjectType;
  typedef TFeatureImage                                  FeatureImageType;
  typedef typename FeatureImageType::Pointer             FeatureImagePointer;
  typedef typename FeatureImageType::ConstPointer        FeatureImageConstPointer;
  typedef typename FeatureImageType::PixelType           FeatureImagePixelType;
  typedef typename FeatureImageType::InternalPixelType   FeatureInternalPixelType;
  typedef double                                         InternalPrecisionType;
  typedef Image<InternalPrecisionType,2>                 InternalImageType;
  
  /// Internal statistics filter typedef
  typedef StatisticsAttributesLabelMapFilter
  <ImageType,InternalImageType>                          StatisticsLabelMapFilterType;
  typedef typename StatisticsLabelMapFilterType::Pointer StatisticsLabelMapFilterPointerType;

  /// Fuctors typedef
  
  // GEMI
  typedef Functor::GEMI <FeatureInternalPixelType,
  FeatureInternalPixelType,InternalPrecisionType>        GEMIFunctorType;
  typedef MultiChannelRAndNIRIndexImageFilter
  <FeatureImageType,InternalImageType, GEMIFunctorType>  GEMIFilterType;
  typedef typename GEMIFilterType::Pointer               GEMIFilterPointerType;
  
  // NDVI
  typedef Functor::NDVI <FeatureInternalPixelType,
  FeatureInternalPixelType,InternalPrecisionType>        NDVIFunctorType;
  typedef MultiChannelRAndNIRIndexImageFilter
  <FeatureImageType,InternalImageType, NDVIFunctorType>  NDVIFilterType;
  typedef typename NDVIFilterType::Pointer               NDVIFilterPointerType;
  
  // IR
  typedef Functor::IR <FeatureInternalPixelType,
  FeatureInternalPixelType,InternalPrecisionType>        IRFunctorType;
  typedef MultiChannelGAndRIndexImageFilter
  <FeatureImageType,InternalImageType, IRFunctorType>    IRFilterType;
  typedef typename IRFilterType::Pointer                 IRFilterPointerType;

  // IC
  typedef Functor::IC <FeatureInternalPixelType,
  FeatureInternalPixelType,InternalPrecisionType>        ICFunctorType;
  typedef MultiChannelGAndRIndexImageFilter
  <FeatureImageType,InternalImageType, ICFunctorType>    ICFilterType;
  typedef typename ICFilterType::Pointer                 ICFilterPointerType;

  // IB
  typedef Functor::IB <FeatureInternalPixelType,
  FeatureInternalPixelType,InternalPrecisionType>        IBFunctorType;
  typedef MultiChannelGAndRIndexImageFilter
  <FeatureImageType,InternalImageType, IBFunctorType>    IBFilterType;
  typedef typename IBFilterType::Pointer                 IBFilterPointerType;

  // NDWI2
  typedef Functor::NDWI2 <FeatureInternalPixelType,
  FeatureInternalPixelType,InternalPrecisionType>        NDWI2FunctorType;
  typedef itk::UnaryFunctorImageFilter
  <FeatureImageType,InternalImageType, NDWI2FunctorType> NDWI2FilterType;
  typedef typename NDWI2FilterType::Pointer              NDWI2FilterPointerType;

  // Intensity
  typedef VectorImageToIntensityImageFilter
  <FeatureImageType,InternalImageType>                   IntensityFilterType;
  typedef typename IntensityFilterType::Pointer          IntensityFilterPointerType;
  
  // Channels
  typedef MultiToMonoChannelExtractROI
  <FeatureInternalPixelType,InternalPrecisionType>       ChannelFilterType;
  typedef typename ChannelFilterType::Pointer            ChannelFilterPointerType;
  

   /** Set the feature image */
  void SetFeatureImage(TFeatureImage *input)
     {
     // Process object is not const-correct so the const casting is required.
     this->SetNthInput( 1, const_cast<TFeatureImage *>(input) );
     }

  /** Get the feature image */
  FeatureImageType * GetFeatureImage()
    {
    return static_cast<FeatureImageType*>(const_cast<itk::DataObject *>(this->itk::ProcessObject::GetInput(1)));
    }

   /** Set the input image */
  void SetInput1(TImage *input)
     {
     this->SetInput( input );
     }

   /** Set the feature image */
  void SetInput2(TFeatureImage *input)
     {
     this->SetFeatureImage( input );
     }

  /** Set/Get the red channel index */
  itkSetMacro(RedChannelIndex,unsigned int);
  itkGetMacro(RedChannelIndex,unsigned int);

  /** Set/Get the blue channel index */
  itkSetMacro(BlueChannelIndex,unsigned int);
  itkGetMacro(BlueChannelIndex,unsigned int);

  /** Set/Get the green channel index */
  itkSetMacro(GreenChannelIndex,unsigned int);
  itkGetMacro(GreenChannelIndex,unsigned int);

  /** Set/Get the near infra-red channel index */
  itkSetMacro(NIRChannelIndex,unsigned int);
  itkGetMacro(NIRChannelIndex,unsigned int);

  /** Set/get the ReducedAttributesSet flag */
  itkSetMacro(ReducedAttributeSet,bool);
  itkGetMacro(ReducedAttributeSet,bool);
  itkBooleanMacro(ReducedAttributeSet);

protected:
  /** Constructor */
  RadiometricAttributesLabelMapFilter();
  /** Destructor */
  ~RadiometricAttributesLabelMapFilter() {};

  /** GenerateData method */
  virtual void GenerateData();
  
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  RadiometricAttributesLabelMapFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** The red channel index */
  unsigned int m_RedChannelIndex;

  /** The green channel index */
  unsigned int m_GreenChannelIndex;

  /** The blue channel index */
  unsigned int m_BlueChannelIndex;

  /** The near infra-red channel index */
  unsigned int m_NIRChannelIndex;
  
  /// Use only a reduced attribute set
  bool m_ReducedAttributeSet;

} ; // end of class

} // end namespace itk
  
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRadiometricAttributesLabelMapFilter.txx"
#endif

#endif


