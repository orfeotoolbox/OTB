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
#ifndef __otbSOMImageClassificationFilter_h
#define __otbSOMImageClassificationFilter_h

#include "otbSOMClassifier.h"
#include "itkInPlaceImageFilter.h"
#include "itkFixedArray.h"
#include "itkListSample.h"

namespace otb
{
/** \class SOMImageClassificationFilter
 *  \brief This filter performs the classification of a VectorImage using a given SOM map.
 *
 *  This filter is streamed and threaded, allowing to classify huge images. Because the
 *  internal sample type has to be an itk::FixedArray, one must specify at compilation time
 *  the maximum sample dimension. It is up to the user to specify a MaxSampleDimension sufficiently
 *  high to integrate all its features. This filter internally use one SVMClassifier per thread.
 *
 * \sa SVMClassifier
 * \ingroup Streamed
 * \ingroup Threaded
 */
template <class TInputImage, class TOutputImage, class TSOMMap, class TMaskImage = TOutputImage>
class ITK_EXPORT SOMImageClassificationFilter
  : public itk::InPlaceImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef SOMImageClassificationFilter                       Self;
  typedef itk::InPlaceImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SOMImageClassificationFilter, InPlaceImageFilter);

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

  typedef TSOMMap                        SOMMapType;
  typedef typename SOMMapType::Pointer   SOMMapPointerType;
  typedef typename SOMMapType::PixelType SampleType;

  typedef itk::Statistics::ListSample<SampleType>                   ListSampleType;
  typedef typename ListSampleType::Pointer                          ListSamplePointerType;
  typedef otb::SOMClassifier<ListSampleType, SOMMapType, LabelType> ClassifierType;
  typedef typename ClassifierType::Pointer                          ClassifierPointerType;

  /** Set/Get the svm model */
  itkSetObjectMacro(Map, SOMMapType);
  itkGetObjectMacro(Map, SOMMapType);

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
  SOMImageClassificationFilter();
  /** Destructor */
  virtual ~SOMImageClassificationFilter() {}

  /** Threaded generate data */
  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);
  /** Before threaded generate data */
  virtual void BeforeThreadedGenerateData();
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  SOMImageClassificationFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** The SOM model used for classification */
  SOMMapPointerType m_Map;
  /** Default label for invalid pixels (when using a mask) */
  LabelType m_DefaultLabel;

};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSOMImageClassificationFilter.txx"
#endif

#endif
