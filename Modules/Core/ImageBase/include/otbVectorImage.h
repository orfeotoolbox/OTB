/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbVectorImage_h
#define otbVectorImage_h

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "itkVectorImage.h"
#pragma GCC diagnostic pop
#else
#include "itkVectorImage.h"
#endif
#include "otbImageMetadataInterfaceBase.h"
#include "otbImageCommons.h"
#include "OTBImageBaseExport.h"

namespace otb
{
/** \class VectorImage
 * \brief Creation of an "otb" vector image which contains metadata.
 *
 *
 * \ingroup OTBImageBase
 */
template <class TPixel, unsigned int VImageDimension = 2>
class OTBImageBase_EXPORT_TEMPLATE VectorImage
  : public itk::VectorImage<TPixel, VImageDimension>
  , public ImageCommons
{
public:
  /** Standard class typedefs. */
  typedef VectorImage Self;
  typedef itk::VectorImage<TPixel, VImageDimension> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef itk::WeakPointer<const Self>  ConstWeakPointer;

  typedef ImageMetadataInterfaceBase::VectorType           VectorType;
  typedef ImageMetadataInterfaceBase::ImageKeywordlistType ImageKeywordlistType;
  typedef ImageMetadataInterfaceBase::Pointer              ImageMetadataInterfacePointerType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorImage, itk::VectorImage);

  /** Pixel typedef support. Used to declare pixel type in filters
   * or other operations. */
  typedef typename Superclass::PixelType PixelType;

  /** Typedef alias for PixelType */
  typedef typename Superclass::ValueType ValueType;

  /** Internal Pixel representation. Used to maintain a uniform API
   * with Image Adaptors and allow keeping a particular internal
   * representation of data while showing a different external
   * representation. */
  typedef typename Superclass::InternalPixelType InternalPixelType;

  typedef typename Superclass::IOPixelType IOPixelType;

  /** Accessor type that convert data between internal and external
  *  representations.  */
  // typedef itk::DefaultVectorPixelAccessor< InternalPixelType > AccessorType;

  /** Functor to provide a common API between DefaultPixelAccessor and
   * DefaultVectorPixelAccessor */
  typedef itk::DefaultVectorPixelAccessorFunctor<Self> AccessorFunctorType;

  /** Tyepdef for the functor used to access a neighborhood of pixel pointers.*/
  typedef itk::VectorImageNeighborhoodAccessorFunctor<Self> NeighborhoodAccessorFunctorType;

  /** Dimension of the image.  This constant is used by functions that are
   * templated over image type (as opposed to being templated over pixel type
   * and dimension) when they need compile time access to the dimension of
   * the image. */
  itkStaticConstMacro(ImageDimension, unsigned int, VImageDimension);

  /** Container used to store pixels in the image. */
  typedef typename Superclass::PixelContainer PixelContainer;

  /** Index typedef support. An index is used to access pixel values. */
  typedef typename Superclass::IndexType IndexType;

  /** Offset typedef support. An offset is used to access pixel values. */
  typedef typename Superclass::OffsetType OffsetType;

  /** Size typedef support. A size is used to define region bounds. */
  typedef typename Superclass::SizeType SizeType;

  /** Direction typedef support. A matrix of direction cosines. */
  typedef typename Superclass::DirectionType DirectionType;

  /** Region typedef support. A region is used to specify a subset of an image. */
  typedef typename Superclass::RegionType RegionType;

  /** Spacing typedef support.  Spacing holds the size of a pixel.  The
   * spacing is the geometric distance between image samples. */
  typedef typename Superclass::SpacingType SpacingType;

  /** Origin typedef support.  The origin is the geometric coordinates
   * of the index (0, 0). */
  typedef typename Superclass::PointType PointType;

  /** Get the projection coordinate system of the image. */
  virtual std::string GetProjectionRef(void) const;

  virtual void SetProjectionRef(const std::string& wkt);

  /** Get the GCP projection coordinates of the image. */
  virtual std::string GetGCPProjection(void) const;

  virtual unsigned int GetGCPCount(void) const;

  virtual OTB_GCP& GetGCPs(unsigned int GCPnum);
  virtual const OTB_GCP& GetGCPs(unsigned int GCPnum) const;

  virtual std::string GetGCPId(unsigned int GCPnum) const;
  virtual std::string GetGCPInfo(unsigned int GCPnum) const;
  virtual double GetGCPRow(unsigned int GCPnum) const;
  virtual double GetGCPCol(unsigned int GCPnum) const;
  virtual double GetGCPX(unsigned int GCPnum) const;
  virtual double GetGCPY(unsigned int GCPnum) const;
  virtual double GetGCPZ(unsigned int GCPnum) const;

  /** Get the six coefficients of affine geoTtransform. */
  virtual VectorType GetGeoTransform(void) const;

  /** Get signed spacing */
  SpacingType GetSignedSpacing() const;

  /** Set signed spacing */
  virtual void SetSignedSpacing(SpacingType spacing);
  virtual void SetSignedSpacing(double spacing[VImageDimension]);

  /** Get image corners. */
  virtual VectorType GetUpperLeftCorner(void) const;
  virtual VectorType GetUpperRightCorner(void) const;
  virtual VectorType GetLowerLeftCorner(void) const;
  virtual VectorType GetLowerRightCorner(void) const;

  /** Get image keyword list */
  virtual ImageKeywordlistType       GetImageKeywordlist(void);
  virtual const ImageKeywordlistType GetImageKeywordlist(void) const;

  virtual void SetImageKeywordList(const ImageKeywordlistType& kwl);

  /// Copy metadata from a DataObject
  void CopyInformation(const itk::DataObject*) override;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Return the Pixel Accessor object */
  //   AccessorType GetPixelAccessor( void )
  //   {
  //     return AccessorType( this->GetNumberOfComponentsPerPixel() );
  //   }

  //   /** Return the Pixel Accesor object */
  //   const AccessorType GetPixelAccessor( void ) const
  //   {
  //     return AccessorType( this->GetNumberOfComponentsPerPixel() );
  //   }

  /** Return the NeighborhoodAccessor functor */
  NeighborhoodAccessorFunctorType GetNeighborhoodAccessor()
  {
    return NeighborhoodAccessorFunctorType(this->GetNumberOfComponentsPerPixel());
  }

  /** Return the NeighborhoodAccessor functor */
  const NeighborhoodAccessorFunctorType GetNeighborhoodAccessor() const
  {
    return NeighborhoodAccessorFunctorType(this->GetNumberOfComponentsPerPixel());
  }

protected:
  VectorImage();
  ~VectorImage() override
  {
  }

private:
  VectorImage(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Return the ImageMetadataInterfacePointer associated to the data
   *  and creates it on first call
   */
  ImageMetadataInterfacePointerType GetMetaDataInterface() const;

  // The image metadata accessor object. Don't use it directly. Instead use GetMetaDataInterface()
  mutable ImageMetadataInterfacePointerType m_ImageMetadataInterface;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorImage.hxx"
#endif

#include <complex>

namespace otb
{

// Prevent implicit instantiation of common types to improve build performance
// Explicit instanciations are provided in the .cxx
extern template class OTBImageBase_EXPORT_TEMPLATE VectorImage<unsigned int, 2>;
extern template class OTBImageBase_EXPORT_TEMPLATE VectorImage<int, 2>;
extern template class OTBImageBase_EXPORT_TEMPLATE VectorImage<unsigned char, 2>;
extern template class OTBImageBase_EXPORT_TEMPLATE VectorImage<char, 2>;
extern template class OTBImageBase_EXPORT_TEMPLATE VectorImage<unsigned short, 2>;
extern template class OTBImageBase_EXPORT_TEMPLATE VectorImage<short, 2>;
extern template class OTBImageBase_EXPORT_TEMPLATE VectorImage<float, 2>;
extern template class OTBImageBase_EXPORT_TEMPLATE VectorImage<double, 2>;
extern template class OTBImageBase_EXPORT_TEMPLATE VectorImage<std::complex<int>, 2>;
extern template class OTBImageBase_EXPORT_TEMPLATE VectorImage<std::complex<short>, 2>;
extern template class OTBImageBase_EXPORT_TEMPLATE VectorImage<std::complex<float>, 2>;
extern template class OTBImageBase_EXPORT_TEMPLATE VectorImage<std::complex<double>, 2>;
}

#endif
