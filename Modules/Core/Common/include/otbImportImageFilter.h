/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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


#ifndef otbImportImageFilter_h
#define otbImportImageFilter_h

#include "itkImageSource.h"

namespace otb
{

/** \class ImportImageFilter
 * \brief Import data from a standard C array into an Image Type
 *
 * ImportImageFilter provides a mechanism for importing data into an Image Type.
 * ImportImageFilter is an image source, so it behaves like any other pipeline
 * object.
 *
 * This class is templated over the output Image Type
 *
 *
 * \ingroup OTBCommon
 */

template <typename TOutputImageType>
class ITK_EXPORT ImportImageFilter :
  public itk::ImageSource<TOutputImageType>
{
public:
  /** Typedef for the output image.   */
  typedef TOutputImageType                      OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointer;
  typedef typename OutputImageType::SpacingType SpacingType;
  typedef typename OutputImageType::PointType   OriginType;

  /** Standard class typedefs. */
  typedef ImportImageFilter                 Self;
  typedef itk::ImageSource<OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImportImageFilter, itk::ImageSource);

  /** Index typedef support. An index is used to access pixel values. */
  typedef itk::Index<OutputImageType::ImageDimension> IndexType;

  /** Size typedef support. A size is used to define region bounds. */
  typedef itk::Size<OutputImageType::ImageDimension> SizeType;

  /** Region typedef support. A region is used to specify a
   * subset of an image. */
  typedef itk::ImageRegion<OutputImageType::ImageDimension> RegionType;

  /** Type of the output image pixel type. */
  typedef typename OutputImageType::PixelType TPixel;

  /** Get the pointer from which the image data is imported. */
  TPixel *GetImportPointer();

  /** Set the pointer from which the image data is imported.  "num" is
   * the number of pixels in the block of memory. If
   * "LetFilterManageMemory" is false, then the this filter will
   * not free the memory in its destructor and the application providing the
   * buffer retains the responsibility of freeing the memory for this image
   * data.  If "LetFilterManageMemory" is true, then this class
   * will free the memory when this object is destroyed. */
  void SetImportPointer(TPixel *ptr, unsigned long num,
                        bool LetFilterManageMemory);

  /** Set the region object that defines the size and starting index
   * for the imported image. This will serve as the LargestPossibleRegion,
   * the BufferedRegion, and the RequestedRegion.
   * \sa ImageRegion */
  void SetRegion(const RegionType& region)
  {
    if (m_Region != region)
      {
      m_Region = region;
      this->Modified();
      }
  }

  /** Get the region object that defines the size and starting index
   * for the imported image. This will serve as the LargestPossibleRegion,
   * the BufferedRegion, and the RequestedRegion.
   * \sa ImageRegion */
  const RegionType& GetRegion() const
  {
    return m_Region;
  }

  /** Set the spacing (size of a pixel) of the image.
   * \sa GetSignedSpacing() */
  itkSetVectorMacro(Spacing, const double, OutputImageType::ImageDimension);
  itkSetVectorMacro(Spacing, const float, OutputImageType::ImageDimension);

  /** Get the spacing (size of a pixel) of the image.
   * \sa SetSpacing() */
  itkGetVectorMacro(Spacing, const double, OutputImageType::ImageDimension);
  void SetSpacing(const SpacingType& spacing);

  /** Set the origin of the image.
   * \sa GetOrigin() */
  itkSetVectorMacro(Origin, const double, OutputImageType::ImageDimension);
  itkSetVectorMacro(Origin, const float, OutputImageType::ImageDimension);
  void SetOrigin(const OriginType& origin);

  /** Get the origin of the image.
   * \sa SetOrigin() */
  itkGetVectorMacro(Origin, const double, OutputImageType::ImageDimension);

  typedef itk::Matrix<double, OutputImageType::ImageDimension, OutputImageType::ImageDimension> DirectionType;

  /** Set the direction of the image
   * \sa GetDirection() */
  virtual void SetDirection(const DirectionType direction);
  /**  Get the direction of the image
   * \sa SetDirection */
  itkGetConstReferenceMacro(Direction, DirectionType);

protected:
  ImportImageFilter();
  ~ImportImageFilter() override;
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** This filter does not actually "produce" any data, rather it "wraps"
   * the user supplied data into an itk::Image.  */
  void GenerateData() override;

  /** This is a source, so it must set the spacing, size, and largest possible
   * region for the output image that it will produce.
   * \sa ProcessObject::GenerateOutputInformation() */
  void GenerateOutputInformation() override;

  /** This filter can only produce the amount of data that it is given,
   * so we must override ProcessObject::EnlargeOutputRequestedRegion()
   * (The default implementation of a source produces the amount of
   * data requested.  This source, however, can only produce what it is
   * given.)
   *
   * \sa ProcessObject::EnlargeOutputRequestedRegion() */
  void EnlargeOutputRequestedRegion(itk::DataObject *output) override;

private:
  ImportImageFilter(const ImportImageFilter &) = delete;
  void operator =(const ImportImageFilter&) = delete;

  RegionType    m_Region;
  double        m_Spacing[OutputImageType::ImageDimension];
  double        m_Origin[OutputImageType::ImageDimension];
  DirectionType m_Direction;

  TPixel*       m_ImportPointer;
  bool          m_FilterManageMemory;
  unsigned long m_Size;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImportImageFilter.hxx"
#endif

#endif
