/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbAngularProjectionSetImageFilter_h
#define otbAngularProjectionSetImageFilter_h

#include "otbAngularProjectionImageFilter.h"
#include "otbImageToImageListFilter.h"
#include "otbObjectList.h"

namespace otb {
/** \class AngularProjectionSetImageFilter
 * \brief Performs spherical transformation in ND space from a set of angle values
 *
 * This class performs the projections of the set input images to a set of N
 * output images according to N angular values.
 *
 * The list of angular values is given in template. It is supposed to be a list of FixedArray
 * of dimension N-1 (since in speherical coordinate system, there are N-1 angle value in a
 * ND space).
 *
 * Each projection is performed by the AngularProjectionImageFilter class and the
 * output is becoming a ImageList.
 *
 * \sa AngularProjectionImageListFilter
 *
 *
 * \ingroup OTBDimensionalityReduction
 */
template < class TInputImage, class TOutputImage, class TAngleList, class TPrecision >
class ITK_EXPORT AngularProjectionSetImageFilter
  : public ImageToImageListFilter< TInputImage, TOutputImage >
{
public:
  /** Standard typedefs */
  typedef AngularProjectionSetImageFilter    Self;
  typedef ImageToImageListFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>            Pointer;
  typedef itk::SmartPointer<const Self>      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(AngularProjectionSetImageFilter, ImageToImageListFilter);

  /** Dimension */
  itkStaticConstMacro(InputImageDimension,  unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);
  //itkStaticConstMacro(NumberOfInputImages,  unsigned int, TAngleList::MeasurementVectorType::Dimension+1);

  /** Template parameters typedefs */
  typedef TInputImage                         InputImageType;
  typedef typename InputImageType::Pointer    InputImagePointerType;
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename InputImageType::SizeType   InputSizeType;
  typedef typename InputImageType::IndexType  InputIndexType;
  typedef typename InputImageType::PixelType  InputPixelType;

  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;
  typedef typename OutputImageType::PixelType  OutputImagePixelType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  typedef typename Superclass::OutputImageListType OutputImageListType;
  typedef typename OutputImageListType::Pointer    OutputImageListPointerType;
  typedef typename OutputImageListType::Iterator   OutputImageIterator;

  typedef TPrecision PrecisionType;

  typedef TAngleList AngleListType;
  typedef typename AngleListType::Pointer               AngleListPointerType;
  typedef typename AngleListType::MeasurementVectorType MeasurementVectorType;
  typedef typename AngleListType::MeasurementType       MeasurementType;
  typedef MeasurementVectorType AngleArrayType;
  typedef MeasurementType       AngleValueType;

  typedef AngularProjectionImageFilter< InputImageType, OutputImageType, AngleArrayType, PrecisionType > FilterType;
  typedef typename FilterType::Pointer      FilterPointerType;
  typedef ObjectList<FilterType>            FilterListType;
  typedef typename FilterListType::Pointer  FilterListPointerType;
  typedef typename FilterListType::Iterator FilterListIterator;

  itkGetObjectMacro(FilterList, FilterListType);
  FilterType * GetNthFilter(unsigned int idx) const
  {
    if (idx >= m_FilterList->Size())
      {
      itkExceptionMacro(<< "Impossible to GetNthFilter with the index element "
                        << idx << "; this element don't exist, the size of the filter list is "
                        << m_FilterList->Size() << ".");
      }
    return static_cast<FilterType*>(m_FilterList->GetNthElement(idx));
  }

    /** Set/Get Angle List */
  AngleListType * GetAngleList () const {
    return static_cast<AngleListType *>( this->m_AngleList );
  }
  void SetAngleList ( const AngleListType * angles ) {
    this->m_AngleList = const_cast<AngleListType*>( angles );
    this->Modified();
  }

  void SetInput ( unsigned int i, const InputImageType * );
  using Superclass::SetInput;

  const InputImageType * GetInput( unsigned int i ) const;

protected:
  AngularProjectionSetImageFilter();
  ~AngularProjectionSetImageFilter() override { }

  void GenerateData() override;

private:
  AngularProjectionSetImageFilter(const Self&); // not implemented
  void operator=(const Self&); // not implemented

  AngleListPointerType  m_AngleList;
  FilterListPointerType m_FilterList;

}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbAngularProjectionSetImageFilter.hxx"
#endif


#endif

