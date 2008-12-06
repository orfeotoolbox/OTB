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
#ifndef __otbImageToPointSetFilter_h
#define __otbImageToPointSetFilter_h

#include "otbPointSetSource.h"

namespace otb
{

/** \class ImageToPointSetFilter
 * \brief Base class to output PointSet data with image data as input
 *
 * ImageToPointSetFilter is the base class for all process objects that output
 * Point Set data and require image data as input. Specifically, this class
 * defines the SetInput() method for defining the input to a filter.
 *
 * \ingroup ImageFilters
 */
template <class TInputImage, class TOutputPointSet>
class ITK_EXPORT ImageToPointSetFilter : public PointSetSource<TOutputPointSet>
{
public:
  /** Standard class typedefs. */
  typedef ImageToPointSetFilter            Self;
  typedef PointSetSource<TOutputPointSet>  Superclass;
  typedef itk::SmartPointer<Self>          Pointer;
  typedef itk::SmartPointer<const Self>    ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToPointSetFilter, PointSetSource);

  /** Some Image related typedefs. */
  typedef   TInputImage                             InputImageType;
  typedef   typename InputImageType::Pointer        InputImagePointer;
  typedef   typename InputImageType::ConstPointer   InputImageConstPointer;
  typedef   typename InputImageType::RegionType     InputImageRegionType;
  typedef   typename InputImageType::PixelType      InputImagePixelType;

  /** Some PointSet related typedefs. */
  typedef   typename Superclass::OutputPointSetType     OutputPointSetType;
  typedef   typename Superclass::OutputPointSetPointer  OutputPointSetPointer;

  typedef   itk::ProcessObject			    ProcessObjectType;

  /** Set the input image of this process object.  */
  void SetInput(unsigned int idx, const InputImageType *input);

  /** Get the input image of this process object.  */
  const InputImageType * GetInput(unsigned int idx);

  /** Prepare the output */
  void GenerateOutputInformation(void);

protected:
  ImageToPointSetFilter();
  ~ImageToPointSetFilter();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ImageToPointSetFilter(const ImageToPointSetFilter&); //purposely not implemented
  void operator=(const ImageToPointSetFilter&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToPointSetFilter.txx"
#endif

#endif
