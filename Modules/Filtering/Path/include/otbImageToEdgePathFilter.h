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
#ifndef otbImageToEdgePathFilter_h
#define otbImageToEdgePathFilter_h

#include "otbImageToPathFilter.h"

namespace otb
{
/**
 * \class ImageToEdgePathFilter
 * \brief This filter compute the closed edge path of  a labelled object
 *  in an image (for example coming from a segmentation filter). Its output
 * is a Path.
 *
 * The input image is first surrounded by a 1 pixel wide border-line using the
 * ConstantPadImageFilter in order to deal with stuck-to-the-border
 * objects. The padding constant is chosen as regard to the ForegroundValue (label of
 * the object) to avoid confusion on foreground and background values.
 *
 * The edge computation is divided into two steps.
 *
 * First, a linear search is perfomed to detect a first pixel belonging to the edge of
 * the object.
 *
 * From that pixel and until the algorithm comes back to this position, the neighborhood
 * is iteratively and clock-wise searched to detect the next pixel in the edge.
 *
 * In case of linear object (in fact open edge), the algorithm will walk the object twice in
 * opposite directions, thus producing an edge with a null surface and twice the length of the
 * object as perimeter.
 *
 * This leads to consistant result for geometric descriptors (for instance compacity).
 * \sa ImageToPathFilter
 *
 * \ingroup OTBPath
 */

template <class TInputImage, class TOutputPath>
class ITK_EXPORT ImageToEdgePathFilter
  : public ImageToPathFilter<TInputImage, TOutputPath>
{
public:
  /** standards typedefs */
  typedef ImageToEdgePathFilter                       Self;
  typedef ImageToPathFilter<TInputImage, TOutputPath> Superclass;
  typedef itk::SmartPointer<Self>                     Pointer;
  typedef itk::SmartPointer<const Self>               ConstPointer;

  /// Creation through the object factory
  itkNewMacro(Self);
  /// Runtime information
  itkTypeMacro(ImageToEdgePathFilter, ImageToPathFilter);

  /// Template parameters typedef
  typedef typename Superclass::InputImageType          InputImageType;
  typedef typename Superclass::InputImagePointerType   InputImagePointerType;
  typedef typename Superclass::OutputPathType          OutputPathType;
  typedef typename Superclass::OutputPathPointerType   OutputPathPointerType;
  typedef typename OutputPathType::ContinuousIndexType ContinuousIndexType;

  typedef typename InputImageType::PixelType PixelType;

  /** Set and Get foreground value */
  itkSetMacro(ForegroundValue, PixelType);
  itkGetConstMacro(ForegroundValue, PixelType);

protected:
  ImageToEdgePathFilter();
  ~ImageToEdgePathFilter() ITK_OVERRIDE {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;
  void GenerateOutputInformation() ITK_OVERRIDE {}  //does nothing
  void GenerateData() ITK_OVERRIDE;

private:
  ImageToEdgePathFilter(const Self &); // purposely not implemented
  void operator =(const Self&); // purposely not implemented

  PixelType m_ForegroundValue;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToEdgePathFilter.txx"
#endif

#endif
