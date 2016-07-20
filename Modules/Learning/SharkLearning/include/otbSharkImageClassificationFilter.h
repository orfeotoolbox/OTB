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
#ifndef otbSharkImageClassificationFilter_h
#define otbSharkImageClassificationFilter_h

#include "itkImageToImageFilter.h"
#include "otbSharkRandomForestsMachineLearningModel.h"
#include "otbImage.h"

namespace otb
{
/** \class SharkImageClassificationFilter
 *  \brief This filter performs the classification of a VectorImage using a Model.
 *
 *  This filter is streamed and threaded, allowing to classify huge images
 *  while fully using several core.
 *
 * \sa Classifier
 * \ingroup Streamed
 * \ingroup Threaded
 *
 * \ingroup OTBSupervised
 */
template <class TInputImage, class TOutputImage, class TMaskImage = TOutputImage>
class ITK_EXPORT SharkImageClassificationFilter
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef SharkImageClassificationFilter                       Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SharkImageClassificationFilter, ImageToImageFilter);

  typedef TInputImage                                InputImageType;
  typedef typename InputImageType::ConstPointer      InputImageConstPointerType;
  typedef typename InputImageType::InternalPixelType ValueType;

  typedef TMaskImage                           MaskImageType;
  typedef typename MaskImageType::ConstPointer MaskImageConstPointerType;
  typedef typename MaskImageType::Pointer      MaskImagePointerType;

  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::PixelType  LabelType;

  typedef SharkRandomForestsMachineLearningModel<ValueType, LabelType> ModelType;
  typedef typename ModelType::Pointer    ModelPointerType;

  typedef otb::Image<double>                    ConfidenceImageType;
  typedef typename ConfidenceImageType::Pointer ConfidenceImagePointerType;

  /** Set/Get the model */
  itkSetObjectMacro(Model, ModelType);
  itkGetObjectMacro(Model, ModelType);

  /** Set/Get the default label */
  itkSetMacro(DefaultLabel, LabelType);
  itkGetMacro(DefaultLabel, LabelType);

  /** Set/Get the confidence map flag */
  itkSetMacro(UseConfidenceMap, bool);
  itkGetMacro(UseConfidenceMap, bool);

  itkSetMacro(Batch, bool);
  itkGetMacro(Batch, bool);

  /**
   * If set, only pixels within the mask will be classified.
   * All pixels with a value greater than 0 in the mask, will be classified.
   * \param mask The input mask.
   */
  void SetInputMask(const MaskImageType * mask);

  /**
   * Get the input mask.
   * \return The mask.
   */
  const MaskImageType * GetInputMask(void);

  /**
   * Get the output confidence map
   */
  ConfidenceImageType * GetOutputConfidence(void);

protected:
  /** Constructor */
  SharkImageClassificationFilter();
  /** Destructor */
  ~SharkImageClassificationFilter() ITK_OVERRIDE {}

  /** Threaded generate data */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) ITK_OVERRIDE;
  void ClassicThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);
  void BatchThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);
  /** Before threaded generate data */
  void BeforeThreadedGenerateData() ITK_OVERRIDE;
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

private:
  SharkImageClassificationFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** The model used for classification */
  ModelPointerType m_Model;
  /** Default label for invalid pixels (when using a mask) */
  LabelType m_DefaultLabel;
  /** Flag to produce the confidence map (if the model supports it) */
  bool m_UseConfidenceMap;
  bool m_Batch;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSharkImageClassificationFilter.txx"
#endif

#endif
