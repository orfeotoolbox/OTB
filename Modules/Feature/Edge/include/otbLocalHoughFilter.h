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
#ifndef __otbLocalHoughFilter_h
#define __otbLocalHoughFilter_h


#include "itkHoughTransform2DLinesImageFilter.h"

#include "otbImageToLineSpatialObjectListFilter.h"
#include "otbExtractROI.h"

namespace otb
{
/** \class LocalHoughFilter
 * \brief Application of Hough filter on a nxm pixel tiling of the image.
 *
 * This class implements a filter that applies the hough transform on nxm
 * pixel tiling of the image. The Hough filter is used to find straight
 * lines in a 2-dimensional image. It detects the best line in each tile
 * and suppresses dubious line detection due to small local structures.
 *
 * Input parameters are : the tile radius, the number of lines we are
 * looking for, the variance of the accumulator blurring (default = 5) and
 * the radius of the disc to remove from the accumulator (default = 10).
 *
 */

template <class TInputImage>
class ITK_EXPORT LocalHoughFilter : public ImageToLineSpatialObjectListFilter<TInputImage>
{
public:

  /** Standard class typedefs. */
  typedef LocalHoughFilter                                Self;
  typedef ImageToLineSpatialObjectListFilter<TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for management of the "object factory". */
  itkNewMacro(Self);

  /** Return the name of the class. */
  itkTypeMacro(LocalHoughFilter, ImageToLineSpatialObjectListFilter);

  /** Definition of the list of lines. */
  typedef typename Superclass::LinesListType     LinesListType;
  typedef typename LinesListType::const_iterator LineIterator;

  typedef typename LinesListType::LineType LineType;
  typedef typename LineType::Pointer       LinePointer;

  typedef typename LineType::PointListType PointListType;
  typedef typename LineType::LinePointType LinePointType;

  /**   Extract dimensions as well of the images of entry of exit. */
  itkStaticConstMacro(InputImageDimension,
                      unsigned int,
                      TInputImage::ImageDimension);

  typedef TInputImage InputImageType;

  //------------------------------------------------------------
  typedef  unsigned char                  OutputPixelType;
  typedef  otb::Image<OutputPixelType, 2> OutputImageType;
  //-----------------------------------------------

  /** Definition of the pixel type of the input and output images */
  typedef typename InputImageType::PixelType InputPixelType;
  typedef   float                            AccumulatorPixelType;

  typedef typename InputImageType::RegionType InputImageRegionType;

  /** Definition of the size of the images. */
  typedef typename InputImageType::SizeType SizeType;

  typedef typename InputImageType::RegionType::IndexType IndexType;

  /** Typedefs to define the extract ROI filter. */
  typedef ExtractROI<InputPixelType, InputPixelType> ROIFilterType;

  /** Set the radius of the local region where to apply the Hough filter. */
  itkSetMacro(Radius, SizeType);

  /** Get the radius of the local region. */
  itkGetConstReferenceMacro(Radius, SizeType);

  /** Set the radius of the overlap of the local region where to apply the Hough filter. */
  itkSetMacro(Overlap, SizeType);

  /** Get the radius of the overlap. */
  itkGetConstReferenceMacro(Overlap, SizeType);

  /** Set the number of lines we are looking for. */
  itkSetMacro(NumberOfLines, unsigned int);

  /** Get the number of lines we are looking for. */
  itkGetConstReferenceMacro(NumberOfLines, unsigned int);

  /** Set/Get the radius of the disc to remove from the accumulator
   *  for each line found */
  itkSetMacro(DiscRadius, float);
  itkGetMacro(DiscRadius, float);

  /** Set/Get the variance of the gaussian bluring for the accumulator */
  itkSetMacro(Variance, float);
  itkGetMacro(Variance, float);

  itkSetMacro(Threshold, float);
  itkGetMacro(Threshold, float);

protected:
  LocalHoughFilter();
  virtual ~LocalHoughFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Definition of the Hough Filter. */
  typedef itk::HoughTransform2DLinesImageFilter<InputPixelType, AccumulatorPixelType> HoughFilterType;

  virtual void GenerateData();

private:
  LocalHoughFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Radius used to define the local region. */
  SizeType m_Radius;

  /** Radius used to define the overlap of local regions. */
  SizeType m_Overlap;

  /** Parameter of the Hough filter : number of lines we are looking for. */
  unsigned int m_NumberOfLines;

  /** Variance of the accumulator blurring (default = 5). */
  float m_Variance;

  /** Radius of the disc to remove from the accumulator (default = 10). */
  float m_DiscRadius;

  /** Threshold abouve which a pixel is consedered as valid */
  float m_Threshold;

  LinePointer LinePointResearch(LineIterator itLines, InputImageType *localImage, IndexType origin);

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLocalHoughFilter.txx"
#endif

#endif
