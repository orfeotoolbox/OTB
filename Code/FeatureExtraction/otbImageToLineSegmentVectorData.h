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
#ifndef __otbImageToLineSegmentVectorData_h
#define __otbImageToLineSegmentVectorData_h

#include "otbPersistentImageFilter.h"
#include "otbObjectList.h"
#include "otbLineSegmentDetector.h"
#include "itkExtractImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "otbVectorData.h"


namespace otb
{

/** \class PersistentLineDetector
 *  \brief Perform line segment computation in a persistent way.
 *
 * This filter allows find and store the segment from a mono channel image.
 * Its output is a LineSpatialObject list.
 * The Reset() method allows to clear the result  path list.
 *
 * \sa PersistentImageFilter
 *
 */
template<class TInputImage, class TPrecision=double >
class ITK_EXPORT PersistentLineSegmentDetector :
public PersistentImageFilter<TInputImage, TInputImage>
{
  public:
  /** Standard Self typedef */
  typedef PersistentLineSegmentDetector Self;
  typedef PersistentImageFilter<TInputImage,TInputImage>        Superclass;
  typedef itk::SmartPointer<Self>                               Pointer;
  typedef itk::SmartPointer<const Self>                         ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentLineSegmentDetector, PersistentImageFilter);
  
  /** Image related typedefs. */
  // First Input
  typedef TInputImage                                     ImageType;
  typedef typename TInputImage::Pointer                   ImagePointer;
  typedef typename TInputImage::RegionType                RegionType;
  typedef typename TInputImage::SizeType                  SizeType;
  typedef typename TInputImage::IndexType                 IndexType;
  typedef typename TInputImage::PixelType                 PixelType;
  typedef typename TInputImage::InternalPixelType         InternalPixelType;

  // Output path type
  typedef LineSegmentDetector<ImageType, TPrecision>           LineDetectorType;
  typedef typename LineDetectorType::Pointer                   LineDetectorPointerType;
  typedef typename LineDetectorType::LineSpatialObjectListType LineSpatialObjectType;
  typedef typename LineSpatialObjectType::Pointer              LineSpatialObjectPointerType;
  typedef ObjectList<LineSpatialObjectType>                    LineSpatialObjectListType;
  typedef typename LineSpatialObjectListType::Pointer          LineSpatialObjectListPointerType;

  typedef itk::ExtractImageFilter<ImageType, ImageType>        ExtractorType;
  typedef typename ExtractorType::Pointer                      ExtractorPointerType;

  typedef ObjectList<ExtractorType>                            ExtractorListType;
  typedef ObjectList<LineDetectorType>                         LineDetectorListType;

  // Vector of region
  typedef std::vector<RegionType>                              RegionListType;

  virtual void Reset(void);
  virtual void Synthetize(void);

  itkGetObjectMacro(LineSpatialObjectList,LineSpatialObjectListType);
  RegionListType GetRegionList()
  {
    return m_RegionList;
  }

  void AllocateOutputs(){};
  void GenerateOutputInformation();
  // Override since the filter needs all the data for the algorithm
  virtual void GenerateInputRequestedRegion();


protected:
  PersistentLineSegmentDetector();
  virtual ~PersistentLineSegmentDetector() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  virtual void  ThreadedGenerateData (const RegionType& outputRegionForThread,int threadId);


private:
  PersistentLineSegmentDetector(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  // line detector filter
  //LineDetectorPointerType m_LineDetector;
  // Region extractor.
  LineSpatialObjectListPointerType m_LineSpatialObjectList;
  // Store requested region for each threads
  RegionListType m_RegionList;
  typename ExtractorListType::Pointer m_Extractor;
  typename LineDetectorListType::Pointer m_LineDetector;

}; // end of class
// end of class PersistentLineSegmentDetector

/**===========================================================================*/

/** \class ImageToLineSegmentVectorData
 * \brief This class streams the whole input image through the PersistentLineSegmentDetector.
 *
 * This way, it allows to compute the inner product of this image. It calls the
 * Reset() method of the PersistentLineSegmentDetector before streaming the image and the
 * Synthetize() method of the PersistentLineSegmentDetector after having streamed the image
 * to compute the statistics. The accessor on the results are wrapping the accessors of the
 * internal PersistentLineSegmentDetector.
 * Its output is a vector data file.
 *
 * \sa PersistentLineSegmentDetector
 * \sa PersistentImageFilter
 * \sa PersistentFilterStreamingDecorator
 * \sa StreamingImageVirtualWriter
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup MathematicalStatisticsImageFilters
 */

template<class TInputImage, class TPrecision=double>
class ITK_EXPORT ImageToLineSegmentVectorData :
      public PersistentFilterStreamingDecorator< PersistentLineSegmentDetector<TInputImage, TPrecision> >
{
  public:
  /** Standard Self typedef */
  typedef ImageToLineSegmentVectorData             Self;
  typedef PersistentFilterStreamingDecorator
  < PersistentLineSegmentDetector<TInputImage, TPrecision> > Superclass;
  typedef itk::SmartPointer<Self>                          Pointer;
  typedef itk::SmartPointer<const Self>                    ConstPointer;


  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ImageToLineSegmentVectorData,PersistentFilterStreamingDecorator);

  typedef TInputImage                                       InputImageType;
  typedef typename TInputImage::SizeType                    SizeType;

  typedef typename Superclass::FilterType                   LSDFilterType;
  /** Type of DataObjects used for scalar outputs */
  typedef typename LSDFilterType::LineSpatialObjectListType LineSpatialObjectListType;
  typedef typename LineSpatialObjectListType::Pointer       LineSpatialObjectListPointerType;
  typedef typename LSDFilterType::LineSpatialObjectType     LineSpatialObjectType;
  typedef typename LSDFilterType::RegionListType            RegionListType;

  typedef VectorData<TPrecision>                            VectorDataType;
  typedef typename VectorDataType::Pointer                  VectorDataPointerType;
  typedef typename VectorDataType::DataNodeType             DataNodeType;
  typedef typename DataNodeType::Pointer                    DataNodePointerType;
  typedef typename DataNodeType::LineType                   LineType;
  typedef typename LineType::VertexType                     VertexType;

  void SetInput(TInputImage * input)
  {
    this->GetFilter()->SetInput(input);
  }
  const TInputImage * GetInput()
  {
    return this->GetFilter()->GetInput();
  }

 
 LineSpatialObjectListPointerType GetLines()
  {
    return this->GetFilter()->GetLineSpatialObjectList();
  };
  RegionListType GetRegionList()
  {
    return this->GetFilter()->GetRegionList();
  }

  /** Transform the linespatialobject list into vector data*/
  VectorDataPointerType GetOutputVectorData();

  itkSetMacro(ThreadDistanceThreshold, double);
  itkGetMacro(ThreadDistanceThreshold, double);

protected:
  /** Constructor */
  ImageToLineSegmentVectorData();
  /** Destructor */
  virtual ~ImageToLineSegmentVectorData() {};

private:
  ImageToLineSegmentVectorData(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** tolerance to fuse 2 lines in 2 threads. */
  double m_ThreadDistanceThreshold;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToLineSegmentVectorData.txx"
#endif

#endif
