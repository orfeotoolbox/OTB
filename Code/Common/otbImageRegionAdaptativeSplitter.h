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

#ifndef __otbImageRegionAdaptativeSplitter_h
#define __otbImageRegionAdaptativeSplitter_h

#include "itkObject.h"
#include "itkRegion.h"
#include "itkImageRegion.h"
#include "itkImageRegionSplitter.h"
#include "itkObjectFactory.h"
#include "itkIndex.h"
#include "itkSize.h"

namespace otb
{

/** \class ImageRegionSquareTileSplitter
   * \brief Divide a region into several pieces.
   *
   *
   * \sa ImageRegionMultidimensionalSplitter
   *
   * \ingroup ITKSystemObjects
   * \ingroup DataProcessing
 */

template <unsigned int VImageDimension>
class ITK_EXPORT ImageRegionAdaptativeSplitter : public itk::ImageRegionSplitter<VImageDimension>
{
public:
  /** Standard class typedefs. */
  typedef ImageRegionAdaptativeSplitter                Self;
  typedef itk::ImageRegionSplitter<VImageDimension> Superclass;
  typedef itk::SmartPointer<Self>                   Pointer;
  typedef itk::SmartPointer<const Self>             ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageRegionAdaptativeSplitter, itk::Object);

  /** Dimension of the image available at compile time. */
  itkStaticConstMacro(ImageDimension, unsigned int, VImageDimension);

  /** Dimension of the image available at run time. */
  static unsigned int GetImageDimension()
  {
    return VImageDimension;
  }
  
  /** Index typedef support. An index is used to access pixel values. */
  typedef itk::Index<VImageDimension>        IndexType;
  typedef typename IndexType::IndexValueType IndexValueType;

  /** Size typedef support. A size is used to define region bounds. */
  typedef itk::Size<VImageDimension>       SizeType;
  typedef typename SizeType::SizeValueType SizeValueType;

  /** Region typedef support.   */
  typedef itk::ImageRegion<VImageDimension> RegionType;

  typedef std::vector<RegionType> StreamVectorType;

  itkSetMacro(TileHint,SizeType);
  itkGetConstReferenceMacro(TileHint,SizeType);
  
  itkSetMacro(ImageRegion,RegionType);
  itkGetConstReferenceMacro(ImageRegion,RegionType);

  itkSetMacro(RequestedNumberOfSplits,unsigned int);
  itkGetConstReferenceMacro(RequestedNumberOfSplits,unsigned int);


  /** How many pieces can the specified region be split? A given region
   *  cannot always be divided into the requested number of pieces.  For
   *  instance, if the numberOfPieces exceeds the number of pixels along
   *  a certain dimensions, then some splits will not be possible.
   */
  virtual unsigned int GetNumberOfSplits(const RegionType& region,
                                         unsigned int requestedNumber);

  /** Get a region definition that represents the ith piece a specified region.
   * The "numberOfPieces" specified should be less than or equal to what
   * GetNumberOfSplits() returns. */
  virtual RegionType GetSplit(unsigned int i, unsigned int numberOfPieces,
                              const RegionType& region);

  virtual void Modified()
  {
    // Call superclass implementation
    Superclass::Modified();

    // Invalidate up-to-date
    m_IsUpToDate = false;
  }

protected:
  ImageRegionAdaptativeSplitter() : m_TileHint(), 
                                    m_ImageRegion(), 
                                    m_RequestedNumberOfSplits(0), 
                                    m_StreamVector(),
                                    m_IsUpToDate(false)    
                                      {}

  virtual ~ImageRegionAdaptativeSplitter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  // Trigger split map estimation
  void EstimateSplitMap();

  ImageRegionAdaptativeSplitter(const ImageRegionAdaptativeSplitter &); //purposely not implemented
  void operator =(const ImageRegionAdaptativeSplitter&); //purposely not implemented

  // This reflects the input image tiling
  SizeType   m_TileHint;

  // This contains the ImageRegion that is currently beeing split
  RegionType m_ImageRegion;

  // This contains the requested number of splits
  unsigned int m_RequestedNumberOfSplits;

  // This is a vector of all regions which will be split
  StreamVectorType m_StreamVector;

  // Is the splitter up-to-date ?
  bool m_IsUpToDate;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
# include "otbImageRegionAdaptativeSplitter.txx"
#endif

#endif
