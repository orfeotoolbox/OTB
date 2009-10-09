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
#ifndef __otbStatisticsAttributesLabelMapFilter_h
#define __otbStatisticsAttributesLabelMapFilter_h

#include "itkInPlaceLabelMapFilter.h"
#include "itkMatrix.h"
#include "itkVector.h"

namespace otb {
/** \class StatisticsAttributesLabelMapFilter
 * \brief This class is a fork of itk::StasticsLabelMapFilter to support AttributesMapLabelObject.
 *
 * To be able to support statistics attributes for multiple features
 * into the same AttributesMapLabelObject, a feature name can be
 * set. The key of the statistics features in the attributes map will
 * be "STATS::FeatureName::StatisticName".
 *
 * The flag ReducedAttributesSet allows to compute only non-redundant
 * statistics and statistics usefull for classification (not spatially
 * dependent statitistics). Reduced set contains only mean, variance,
 * skewness and kurtosis.
 *
 * \sa itk::StatisticsLabelMapFilter
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TImage, class TFeatureImage>
class ITK_EXPORT StatisticsAttributesLabelMapFilter :
    public itk::InPlaceLabelMapFilter<TImage>
{
public:
  /** Standard class typedefs. */
  typedef StatisticsAttributesLabelMapFilter      Self;
  typedef itk::InPlaceLabelMapFilter<TImage>      Superclass;
  typedef itk::SmartPointer<Self>                 Pointer;
  typedef itk::SmartPointer<const Self>           ConstPointer;

  /** Some convenient typedefs. */
  typedef TImage ImageType;
  typedef typename ImageType::Pointer             ImagePointer;
  typedef typename ImageType::ConstPointer        ImageConstPointer;
  typedef typename ImageType::PixelType           PixelType;
  typedef typename ImageType::IndexType           IndexType;
  typedef typename ImageType::PointType           PointType;
  typedef typename ImageType::LabelObjectType     LabelObjectType;
  typedef typename itk::Matrix<double,
  TImage::ImageDimension,TImage::ImageDimension>  MatrixType;
  typedef typename itk::Vector<double,
                         TImage::ImageDimension>  VectorType;
  typedef TFeatureImage                           FeatureImageType;
  typedef typename FeatureImageType::Pointer      FeatureImagePointer;
  typedef typename FeatureImageType::ConstPointer FeatureImageConstPointer;
  typedef typename FeatureImageType::PixelType    FeatureImagePixelType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int,TImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(StatisticsAttributesLabelMapFilter,InPlaceLabelMapFilter);

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

  /** Set/Get the feature name */
  itkSetStringMacro(FeatureName);
  itkGetStringMacro(FeatureName);

  /** Set/get the ReducedAttributesSet flag */
  itkSetMacro(ReducedAttributeSet,bool);
  itkGetMacro(ReducedAttributeSet,bool);
  itkBooleanMacro(ReducedAttributeSet);

protected:
  /** Constructor */
  StatisticsAttributesLabelMapFilter();
  /** Destructor */
  ~StatisticsAttributesLabelMapFilter() {};
  /** Threaded generate data */
  virtual void ThreadedProcessLabelObject( LabelObjectType * labelObject );
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  StatisticsAttributesLabelMapFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Global feature image minimum */
  FeatureImagePixelType m_Minimum;
  /** Global feature image maximum */
  FeatureImagePixelType m_Maximum;

  // Name of the feature for the feature map key
  std::string           m_FeatureName;

  /// Use only a reduced attribute set
  bool m_ReducedAttributeSet;

} ; // end of class

} // end namespace otb
  
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStatisticsAttributesLabelMapFilter.txx"
#endif

#endif


