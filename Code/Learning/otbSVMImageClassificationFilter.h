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
#ifndef __otbSVMImageClassificationFilter_h
#define __otbSVMImageClassificationFilter_h

#include "vcl_deprecated_header.h"

#include "itkInPlaceImageFilter.h"
#include "otbSVMModel.h"

namespace otb
{
/** \class SVMImageClassificationFilter
 *  \brief This filter performs the classification of a VectorImage using a SVM Model.
 *
 *  This filter is streamed and threaded, allowing to classify huge images
 *  while fully using several core.
 *
 * \deprecated
 *
 * \sa MachineLearningModel
 * \sa LibSVMMachineLearningModel
 * \sa ImageClassificationFilter
 *
 * \ingroup Streamed
 * \ingroup Threaded
 */
template <class TInputImage, class TOutputImage, class TMaskImage = TOutputImage>
class ITK_EXPORT SVMImageClassificationFilter
  : public itk::InPlaceImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef SVMImageClassificationFilter                       Self;
  typedef itk::InPlaceImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SVMImageClassificationFilter, InPlaceImageFilter);

  /** The max dimension of the sample to classify.
   *  This filter internally uses itk::FixedArray as input for the classifier,
   *  so the max sample size has to be fixed at compilation time.
   */

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

  typedef SVMModel<ValueType, LabelType> ModelType;
  typedef typename ModelType::Pointer    ModelPointerType;

  /** Set/Get the svm model */
  itkSetObjectMacro(Model, ModelType);
  itkGetObjectMacro(Model, ModelType);

  /** Set/Get the default label */
  itkSetMacro(DefaultLabel, LabelType);
  itkGetMacro(DefaultLabel, LabelType);

  /**
   * If set, only pixels within the mask will be classified.
   * \param mask The input mask.
   */
  void SetInputMask(const MaskImageType * mask);

  /**
   * Get the input mask.
   * \return The mask.
   */
  const MaskImageType * GetInputMask(void);

protected:
  /** Constructor */
  SVMImageClassificationFilter();
  /** Destructor */
  virtual ~SVMImageClassificationFilter() {}

  /** Threaded generate data */
  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId);
  /** Before threaded generate data */
  virtual void BeforeThreadedGenerateData();
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  SVMImageClassificationFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** The SVM model used for classification */
  ModelPointerType m_Model;
  /** Default label for invalid pixels (when using a mask) */
  LabelType m_DefaultLabel;

};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSVMImageClassificationFilter.txx"
#endif

#endif
