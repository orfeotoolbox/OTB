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
#ifndef __otbTileImageFilter_h
#define __otbTileImageFilter_h

#include "itkImageToImageFilter.h"

namespace otb
{
/** \class TileImageFilter
 * \brief TODO
 *
 * \ingroup IntensityImageFilters
 * \ingroup Streamed
 * \ingroup MultiThreaded
 */
template <class TImage>
class ITK_EXPORT TileImageFilter :
    public itk::ImageToImageFilter<TImage, TImage>
{
public:
  /** Standard class typedef */
  typedef TileImageFilter                         Self;
  typedef itk::ImageToImageFilter<TImage, TImage> Superclass;
  typedef itk::SmartPointer<Self>                 Pointer;
  typedef itk::SmartPointer<const Self>           ConstPointer;

  /** Helper typedefs */
  typedef TImage                                  ImageType;
  typedef typename ImageType::Pointer             ImagePointerType;
  typedef typename ImageType::SizeType            SizeType;
  typedef typename ImageType::RegionType          RegionType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(TileImageFilter, ImageToImageFilter);

  itkSetMacro(Layout,SizeType);
  itkGetConstReferenceMacro(Layout,SizeType);

protected:
  /** Constructor */
  TileImageFilter();

  /** Destructor */
  virtual ~TileImageFilter();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Threaded generate data */
  virtual void ThreadedGenerateData(const RegionType& outputRegionForThread, int threadId);

  /** Generate input requested region method */
  virtual void GenerateInputRequestedRegion();

  /** Generate input requested region method */
  virtual void GenerateOutputInformation();
  
private:
  TileImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  // Compute the overlapping region between the output requested
  // region and the nth input tile largest region
  RegionType OutputRegionToInputRegion(unsigned int tileIndex, const RegionType & requestedRegion);

RegionType InputRegionToOutputRegion(unsigned int tileIndex, const RegionType & requestedRegion);

  // Layout of the images
  SizeType m_Layout;

  // Columns sizes
  std::vector<unsigned int> m_ColumnsSizes;

  // Row sizes
  std::vector<unsigned int> m_RowsSizes;
};

} // end namespace itk

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTileImageFilter.txx"
#endif

#endif
