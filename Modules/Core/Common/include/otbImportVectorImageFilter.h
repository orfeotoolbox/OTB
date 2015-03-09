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

#ifndef __otbImportVectorImageFilter_h
#define __otbImportVectorImageFilter_h

#include "itkImageSource.h"

namespace otb
{

/** \class ImportVectorImageFilter
 * \brief Import data from a standard C array into an Image Type
 *
 * ImportVectorImageFilter provides a mechanism for importing data into an Image Type.
 * ImportVectorImageFilter is an image source, so it behaves like any other pipeline
 * object.
 *
 * This class is templated over the output Image Type
 *
 *
 * \ingroup OTBCommon
 */

template <typename TOutputImageType>
class ITK_EXPORT ImportVectorImageFilter :
  public itk::ImageSource<TOutputImageType>
{
public:
  /** Typedef for the output image.   */
  typedef TOutputImageType                      OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointer;
  typedef typename OutputImageType::SpacingType SpacingType;
  typedef typename OutputImageType::PointType   OriginType;

  /** Standard class typedefs. */
  typedef ImportVectorImageFilter           Self;
  typedef itk::ImageSource<OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImportVectorImageFilter, itk::ImageSource);

  /** Index typedef support. An index is used to access pixel values. */
  typedef itk::Index<OutputImageType::ImageDimension> IndexType;

  /** Size typedef support. A size is used to define region bounds. */
  typedef itk::Size<OutputImageType::ImageDimension> SizeType;

  /** Region typedef support. A region is used to specify a
   * subset of an image. */
  typedef itk::ImageRegion<OutputImageType::ImageDimension> RegionType;

  /** Type of the output image pixel type. */
  typedef typename OutputImageType::PixelType TOutputPixel;
  typedef typename TOutputPixel::ValueType    TPixel;

  /** Get the pointer from which the image data is imported. */
  TPixel *GetImportPointer();

  /** Set the pointer from which the image data is imported.  "num" is
   * the number of pixels in the block of memory. If
   * "LetFilterManageMemory" is false, then the this filter will
   * not free the memory in its destructor and the application providing the
   * buffer retains the responsibility of freeing the memory for this image
   * data.  If "LetFilterManageMemory" is true, then this class
   * will free the memory when this object is destroyed. */
  virtual void SetImportPointer(TPixel *ptr, unsigned long num,
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
   * \sa GetSpacing() */
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

  itkGetMacro(NumberOfComponents, unsigned int);
  itkSetMacro(NumberOfComponents, unsigned int);

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
  ImportVectorImageFilter();
  virtual ~ImportVectorImageFilter();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** This filter does not actually "produce" any data, rather it "wraps"
   * the user supplied data into an itk::Image.  */
  virtual void GenerateData();

  /** This is a source, so it must set the spacing, size, and largest possible
   * region for the output image that it will produce.
   * \sa ProcessObject::GenerateOutputInformation() */
  virtual void GenerateOutputInformation();

  /** This filter can only produce the amount of data that it is given,
   * so we must override ProcessObject::EnlargeOutputRequestedRegion()
   * (The default implementation of a source produces the amount of
   * data requested.  This source, however, can only produce what it is
   * given.)
   *
   * \sa ProcessObject::EnlargeOutputRequestedRegion() */
  virtual void EnlargeOutputRequestedRegion(itk::DataObject *output);

private:
  ImportVectorImageFilter(const ImportVectorImageFilter &); //purposely not implemented
  void operator =(const ImportVectorImageFilter&); //purposely not implemented

  RegionType    m_Region;
  double        m_Spacing[OutputImageType::ImageDimension];
  double        m_Origin[OutputImageType::ImageDimension];
  DirectionType m_Direction;

  TPixel*       m_ImportPointer;
  bool          m_FilterManageMemory;
  unsigned int  m_NumberOfComponents;
  unsigned long m_Size;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImportVectorImageFilter.txx"
#endif

#endif
