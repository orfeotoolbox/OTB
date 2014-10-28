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
#ifndef __otbRemoveCarvingPathFilter_h
#define __otbRemoveCarvingPathFilter_h

#include "itkImageAndPathToImageFilter.h"
#include "itkImageSliceIteratorWithIndex.h"

namespace otb
{
/** \class RemoveCarvingPathFilter
 * \brief This class remove pixels given in one path
 *
 *
 *
 * This filter remove pixels found by the otb::ImageToCarvingPathFilter
 * thus decreasing the image size.
  * the image (if Direction is set to 0) or from left to right
  * (if direction is set to 1).
  *
  * This is one component necessary to apply the seam carving algorithm
  * published by Shai Avidan, Ariel Shamir,
  * Seam Carving for Content-Aware Image Resizing,
  * ACM Transactions on Graphics, Volume 26, Number 3,
  * SIGGRAPH 2007
  *
  * \par INPUTS
   * The first input is an image, usually, the same one provided to the
 * otb::ImageToCarvingPathFilter
   * The second input is a path, typically the output of otb::ImageToCarvingPathFilter.
  *
  * \par OUTPUTS
  * The output is an image with a smaller size than the input image. There is one less
 * column if direction is 0 and one less line if direction is 1.
  *
  * \sa ImageToCarvingPathFilter
  *
  *
  * \ingroup PathFilters
  * \ingroup ImageFilters
  */

template <class TInputImage, class TInputPath, class TOutputImage>
class ITK_EXPORT RemoveCarvingPathFilter : public itk::ImageAndPathToImageFilter<TInputImage, TInputPath, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef RemoveCarvingPathFilter                                               Self;
  typedef itk::ImageAndPathToImageFilter<TInputImage, TInputPath, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                                               Pointer;
  typedef itk::SmartPointer<const Self>                                         ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(RemoveCarvingPathFilter, itk::ImageAndPathToImageFilter);

  /** Some convenient typedefs. */
  typedef          TInputImage                   InputImageType;
  typedef typename InputImageType::Pointer       InputImagePointer;
  typedef typename InputImageType::ConstPointer  InputImageConstPointer;
  typedef typename InputImageType::RegionType    InputImageRegionType;
  typedef typename InputImageType::PixelType     InputImagePixelType;
  typedef          TInputPath                    InputPathType;
  typedef typename InputPathType::Pointer        InputPathPointer;
  typedef typename InputPathType::ConstPointer   InputPathConstPointer;
  typedef typename InputPathType::InputType      InputPathInputType;
  typedef typename InputPathType::OutputType     InputPathOutputType;
  typedef typename InputPathType::IndexType      InputPathIndexType;
  typedef typename InputPathType::OffsetType     InputPathOffsetType;
  typedef          TOutputImage                  OutputImageType;
  typedef typename OutputImageType::Pointer      OutputImagePointer;
  typedef typename OutputImageType::ConstPointer OutputImageConstPointer;

  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::PixelType  OutputImagePixelType;
  typedef typename OutputImageType::ValueType  OutputImageValueType;

//   typedef typename itk::ImageRegionConstIterator<InputImageType> InputIteratorType;
//   typedef typename itk::ImageRegionIterator<OutputImageType> OutputIteratorType;
  typedef typename itk::ImageSliceConstIteratorWithIndex<InputImageType> InputIteratorType;
  typedef typename itk::ImageSliceIteratorWithIndex<OutputImageType>     OutputIteratorType;

  /** Set/Get the path input of this process object.
      Note: these methods are adding to get the proper name */
  void SetInputPath(const TInputPath * path);

  const TInputPath * GetInputPath(void);

  itkSetMacro(Value, OutputImagePixelType);
  itkGetConstReferenceMacro(Value, OutputImagePixelType);

  /** Set and Get direction value */
  itkSetMacro(Direction, unsigned int);
  itkGetConstMacro(Direction, unsigned int);

  virtual void GenerateOutputInformation();

protected:
  RemoveCarvingPathFilter();
  virtual ~RemoveCarvingPathFilter() {}

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  virtual void GenerateData();

private:
  RemoveCarvingPathFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  OutputImagePixelType m_Value;
  unsigned int         m_Direction;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRemoveCarvingPathFilter.txx"
#endif

#endif
