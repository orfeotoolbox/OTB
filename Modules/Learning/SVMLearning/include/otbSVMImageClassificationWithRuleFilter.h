/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom; Telecom bretagne. All rights reserved.
  See IMTCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef otbSVMImageClassificationWithRuleFilter_h
#define otbSVMImageClassificationWithRuleFilter_h

#include "otbSVMImageClassificationFilter.h"
#include "otbVectorImage.h"

namespace otb {

/** \class SVMImageClassificationWithRuleFilter
 * \brief SVM Image Classification that yield distance to hyperplanes
 *
 * This class comes from SVMImageClassificationFilter that performs
 * SVM classification on image. Here, the class add in ThreadedGenerateData an other output
 * (a VectorImage) that gives the value for \f$ f(x) \f$ without the majority
 * vote decision on class belonging.
 *
 * \sa SVMImageClassificationFilter
 * \ingroup Streamed
 * \ingroup Threaded
 *
 * \ingroup OTBSVMLearning
 */
template <class TInputImage, class TOutputImage, class TMaskImage = TOutputImage>
class ITK_EXPORT SVMImageClassificationWithRuleFilter
  : public SVMImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
{
public:
  /** Standard typedefs */
  typedef SVMImageClassificationWithRuleFilter Self;
  typedef SVMImageClassificationFilter<TInputImage,
      TOutputImage, TMaskImage>              Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SVMImageClassificationWithRuleFilter, SVMImageClassificationFilter);

  typedef typename Superclass::InputImageType             InputImageType;
  typedef typename Superclass::InputImageConstPointerType InputImageConstPointerType;
  typedef typename Superclass::ValueType                  ValueType;

  typedef typename Superclass::MaskImageType             MaskImageType;
  typedef typename Superclass::MaskImageConstPointerType MaskImageConstPointerType;
  typedef typename Superclass::MaskImagePointerType      MaskImagePointerType;

  typedef typename Superclass::OutputImageType        OutputImageType;
  typedef typename Superclass::OutputImagePointerType OutputImagePointerType;
  typedef typename Superclass::OutputImageRegionType  OutputImageRegionType;
  typedef typename Superclass::LabelType              LabelType;

  typedef typename Superclass::ModelType          ModelType;
  typedef typename Superclass::ModelPointerType   ModelPointerType;
  typedef typename ModelType::NodeCacheType       NodeCacheType;
  typedef typename ModelType::MeasurementType     MeasurementType;
  typedef typename ModelType::DistancesVectorType DistanceValueType;
  typedef typename DistanceValueType::ValueType   RuleValueType;

  typedef VectorImage<RuleValueType, OutputImageType::ImageDimension>
  OutputRuleImageType;
  typedef typename OutputRuleImageType::Pointer    OutputRuleImagePointerType;
  typedef typename OutputRuleImageType::RegionType OutputRuleImageRegionType;
  typedef typename OutputRuleImageType::PixelType  RuleType;

  /** Access to the Rule image */
  itkGetObjectMacro(OutputRule, OutputRuleImageType);

protected:
  /** Constructor */
  SVMImageClassificationWithRuleFilter();
  /** Destructor */
  ~SVMImageClassificationWithRuleFilter() ITK_OVERRIDE {}

  /** GenerateOutputInformation
        * Set the number of bands of the output rule image.
        * Copy information from the input image if existing.
        **/
  void GenerateOutputInformation() ITK_OVERRIDE;

  /** AllocateOutputs
   * Output allocation redefinition for VectorImage (used in TOutputRuleImage)
   **/
  void AllocateOutputs() ITK_OVERRIDE;

  /** Threaded generate data */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) ITK_OVERRIDE;

private:
  SVMImageClassificationWithRuleFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  OutputRuleImagePointerType m_OutputRule;

}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSVMImageClassificationWithRuleFilter.txx"
#endif

#endif
