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
#ifndef __otbStreamingLineSegmentDetector_h
#define __otbStreamingLineSegmentDetector_h

#include <vector>

#include "itkImageRegion.h"
#include "otbVectorData.h"
#include "itkPreOrderTreeIterator.h"

#include "itkDataObject.h"
#include "itkDataObjectDecorator.h"
#include "itkSimpleDataObjectDecorator.h"

#include "otbLineSegmentDetector.h"
#include "itkExtractImageFilter.h"

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"

namespace otb
{

/** \class PersistentLineSegmentDetector
 *  \brief [internal] Helper class for the implementation of LineSegmentDetector
 *  with streaming capabilities.
 *
 *  This class inherits PersistentImageFilter and provides the Reset/Synthesize functions,
 *  plus the ThreadedGenerateData function implementing the image function evaluation
 *
 *  \sa LineSegmentDetector
 *  
 *  \ingroup Streamed
 *  \ingroup FeatureExtraction
 */
template<class TInputImage, class TPrecision = double>
class ITK_EXPORT PersistentLineSegmentDetector :
  public PersistentImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentLineSegmentDetector                   Self;
  typedef PersistentImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentLineSegmentDetector, PersistentImageFilter);

  /** Image related typedefs. */
  // First Input
  typedef TInputImage                             ImageType;
  typedef typename TInputImage::Pointer           ImagePointerType;
  typedef typename TInputImage::RegionType        RegionType;
  typedef typename TInputImage::SizeType          SizeType;
  typedef typename TInputImage::IndexType         IndexType;
  typedef typename TInputImage::PixelType         PixelType;

  // Smart Pointer type to a DataObject
  typedef typename itk::DataObject::Pointer DataObjectPointer;

  // Container types
  typedef LineSegmentDetector<ImageType, TPrecision>           LineDetectorType;
  typedef typename LineDetectorType::Pointer                   LineDetectorPointerType;

  typedef typename LineDetectorType::VectorDataType            VectorDataType;
  typedef typename VectorDataType::DataNodeType                DataNodeType;
  typedef typename DataNodeType::Pointer                       DataNodePointerType;
  typedef typename DataNodeType::LineType                      LineType;
  typedef itk::PreOrderTreeIterator<typename VectorDataType::DataTreeType> TreeIteratorType;
  
  typedef itk::ExtractImageFilter<ImageType, ImageType> ExtractorType;
  typedef typename ExtractorType::Pointer               ExtractorPointerType;

  typedef std::vector<typename ExtractorType::Pointer>    ExtractorListType;
  typedef std::vector<RegionType>                         RegionListType;
  typedef std::vector<typename LineDetectorType::Pointer> LineDetectorListType;
  typedef std::vector<typename VectorDataType::Pointer>   VectorDataListType;

  /** Set/Get methods */
  itkSetMacro(ThreadDistanceThreshold, double);
  itkGetMacro(ThreadDistanceThreshold, double);
  
  VectorDataType* GetOutputVectorData(void);

  /** Make a DataObject of the correct type to be used as the specified
   * output. */
  itk::DataObject::Pointer MakeOutput(unsigned int idx);

  void AllocateOutputs();

  void GenerateOutputInformation();

  void Reset(void);

  void Synthetize(void);

protected:
  PersistentLineSegmentDetector();
  virtual ~PersistentLineSegmentDetector();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void GenerateInputRequestedRegion();
  
  /** Multi-thread version GenerateData. */
  void  ThreadedGenerateData(const RegionType& outputRegionForThread,
                             int threadId);

private:
  PersistentLineSegmentDetector(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  // region extractor
  ExtractorListType             m_ExtractorList;
  // store requested region for each threads
  RegionListType                m_RegionList;
  // line detector filters
  LineDetectorListType          m_LineDetectorList;
  // store each thread output
  VectorDataListType            m_VectorDataList;
  /** tolerance to fuse 2 lines in 2 threads. */
  double                        m_ThreadDistanceThreshold;

};


/** \class StreamingLineSegmentDetector
 *  \brief This class streams the whole input image through the PersistentLineSegmentDetector.
 *
 *  This way, it allows to compute the inner product of this image. It calls the
 *  Reset() method of the PersistentLineSegmentDetector before streaming the image and the
 *  Synthetize() method of the PersistentLineSegmentDetector after having streamed the image
 *  to compute the statistics. The accessor on the results are wrapping the accessors of the
 *  internal PersistentLineSegmentDetector.
 *  Its output is a vector data file.
 *
 *  \sa PersistentLineSegmentDetector
 *  \sa PersistentImageFilter
 *  \sa PersistentFilterStreamingDecorator
 *  \sa StreamingImageVirtualWriter
 *  \ingroup Streamed
 *  \ingroup Multithreaded
 *  \ingroup MathematicalStatisticsImageFilters
 */

template<class TInputImage, class TPrecision = double>
class ITK_EXPORT StreamingLineSegmentDetector :
  public PersistentFilterStreamingDecorator<PersistentLineSegmentDetector<TInputImage, TPrecision> >
{
public:
  /** Standard Self typedef */
  typedef StreamingLineSegmentDetector                      Self;
  typedef PersistentFilterStreamingDecorator
  <PersistentLineSegmentDetector<TInputImage, TPrecision> > Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(StreamingLineSegmentDetector, PersistentFilterStreamingDecorator);

  typedef TInputImage                       InputImageType;
  typedef typename TInputImage::Pointer     InputImagePointerType;
  typedef typename TInputImage::RegionType  RegionType;
  typedef typename TInputImage::SizeType    SizeType;
  typedef typename TInputImage::IndexType   IndexType;
  typedef typename TInputImage::PixelType   PixelType;

  /** Smart Pointer type to a DataObject. */
  typedef typename itk::DataObject::Pointer DataObjectPointer;

  /** Output Types */
  typedef typename Superclass::FilterType                  PersistentLSDFilterType;
  typedef typename PersistentLSDFilterType::VectorDataType VectorDataType;
  typedef typename VectorDataType::Pointer                 VectorDataPointerType;
  typedef typename VectorDataType::DataNodeType            DataNodeType;
  typedef typename DataNodeType::Pointer                   DataNodePointerType;
  typedef typename DataNodeType::LineType                  LineType;
  typedef typename LineType::VertexType                    VertexType;

  /** Input image to extract feature */
  void SetInput(TInputImage * input)
  {
    this->GetFilter()->SetInput(input);
  }

  /** Detected lines as a VectorData of lines */
  VectorDataType* GetOutputVectorData()
  {
    return this->GetFilter()->GetOutputVectorData();
  }

  otbSetObjectMemberMacro(Filter, ThreadDistanceThreshold, double);
  otbGetObjectMemberMacro(Filter, ThreadDistanceThreshold, double);

protected:
  /** Constructor */
  StreamingLineSegmentDetector();
  /** Destructor */
  virtual ~StreamingLineSegmentDetector();

private:
  StreamingLineSegmentDetector(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingLineSegmentDetector.txx"
#endif

#endif
