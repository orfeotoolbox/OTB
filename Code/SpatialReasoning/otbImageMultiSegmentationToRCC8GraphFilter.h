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
#ifndef _otbImageMultiSegmentationToRCC8GraphFilter_h
#define _otbImageMultiSegmentationToRCC8GraphFilter_h

#include "otbImageListToRCC8GraphFilter.h"

namespace otb
{
/**
 * \class ImageMultiSegmentationToRCC8GraphFilter
 * \brief This class takes a list of labelled segmentation images 
 * and build the RCC8 graph of the set of regions it represents.
 */
template <class TInputImage, class TOutputGraph>
class ImageMultiSegmentationToRCC8GraphFilter 
  : public ImageListToRCC8GraphFilter<TInputImage,TOutputGraph>
{
public:
  /** Standard class typedefs. */
  typedef ImageMultiSegmentationToRCC8GraphFilter               Self;
  typedef ImageListToRCC8GraphFilter<TInputImage, TOutputGraph> Superclass;
  typedef itk::SmartPointer<Self>                               Pointer;
  typedef itk::SmartPointer<const Self>                         ConstPointer;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageMultiSegmentationToRCC8GraphFilter,ImageListToRCC8GraphFilter);
  /** Input related typedefs */
  typedef TInputImage InputImageType;
  typedef typename InputImageType::PixelType PixelType;
  typedef typename InputImageType::Pointer InputImagePointerType;
  typedef typename Superclass::InputImageListType InputImageListType;
  typedef typename InputImageListType::Pointer InputImageListPointerType;
  typedef typename InputImageListType::ConstIterator ConstListIteratorType;
  /** Output related typedefs */
  typedef TOutputGraph OutputGraphType;
  typedef typename OutputGraphType::Pointer OutputGraphPointerType;
  typedef typename OutputGraphType::VertexType VertexType;
  typedef typename VertexType::Pointer VertexPointerType;

protected:
  /** Constructor */
  ImageMultiSegmentationToRCC8GraphFilter();
  /** Destructor */
  ~ImageMultiSegmentationToRCC8GraphFilter();
  /** Main computation method */
  virtual void GenerateData();
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageMultiSegmentationToRCC8GraphFilter.txx"
#endif

#endif
