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

#include <complex>
#include <sstream>
#include <string>

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include "otbImageIOBase.h"
#include "itkRGBPixel.h"
#include "itkRGBAPixel.h"
#include "itkOffset.h"
#include "itkPoint.h"
#include "itkCovariantVector.h"
#include "itkDiffusionTensor3D.h"

namespace otb
{

ImageIOBase::ImageIOBase() :
  m_PixelType(SCALAR),
  m_ComponentType(UNKNOWNCOMPONENTTYPE),
  m_ByteOrder(OrderNotApplicable),
  m_FileType(TypeNotApplicable),
  m_NumberOfDimensions(0)
{
  Reset(false);
}


void ImageIOBase::Reset(const bool)
{
  m_Initialized = false;
  m_FileName = "";
  m_NumberOfComponents = 1;
  for (unsigned int i=0; i < m_NumberOfDimensions; i++)
    {
    m_Dimensions[i] = 0;
    m_Strides[i] = 0;
    }
  m_NumberOfDimensions = 0;
  m_UseCompression = false;
  m_UseStreamedReading = false;
  m_UseStreamedWriting = false;
}

ImageIOBase::~ImageIOBase()
{
}

const ImageIOBase::ArrayOfExtensionsType &
ImageIOBase::GetSupportedWriteExtensions() const
{
  return this->m_SupportedWriteExtensions;
}

const ImageIOBase::ArrayOfExtensionsType &
ImageIOBase::GetSupportedReadExtensions() const
{
  return this->m_SupportedReadExtensions;
}

void ImageIOBase::AddSupportedReadExtension( const char * extension )
{
  this->m_SupportedReadExtensions.push_back( extension );
}

void ImageIOBase::AddSupportedWriteExtension( const char * extension )
{
  this->m_SupportedWriteExtensions.push_back( extension );
}

void ImageIOBase::Resize(const unsigned int numDimensions,
                         const unsigned int* dimensions)
{
  m_NumberOfDimensions = numDimensions;
  if (dimensions != nullptr)
    {
    for (unsigned int i=0; i < m_NumberOfDimensions; i++)
      {
      m_Dimensions[i] = dimensions[i];
      }
    ComputeStrides();
    }
}

void ImageIOBase::SetDimensions(unsigned int i, unsigned int dim)
{
  if ( i >= m_Dimensions.size() )
    {
    itkWarningMacro("Index: " << i <<
                      " is out of bounds, expected maximum is " <<
                      m_Dimensions.size());
    itkExceptionMacro("Index: " << i <<
                      " is out of bounds, expected maximum is " <<
                      m_Dimensions.size());
    }
  this->Modified();
  m_Dimensions[i] = dim;
}

void ImageIOBase::SetOrigin(unsigned int i, double origin)
{
  if ( i >= m_Origin.size() )
    {
    itkWarningMacro("Index: " << i <<
                      " is out of bounds, expected maximum is " <<
                      m_Origin.size());
    itkExceptionMacro("Index: " << i <<
                      " is out of bounds, expected maximum is " <<
                      m_Origin.size());
    }
  this->Modified();
  m_Origin[i] = origin;
}

void ImageIOBase::SetSpacing(unsigned int i, double spacing)
{
  if (i >= m_Spacing.size() )
    {
    itkWarningMacro("Index: " << i <<
                      " is out of bounds, expected maximum is " <<
                      m_Spacing.size());
    itkExceptionMacro("Index: " << i <<
                      " is out of bounds, expected maximum is " <<
                      m_Spacing.size());
    }
  this->Modified();
  m_Spacing[i] = spacing;
}

void ImageIOBase::SetDirection(unsigned int i, std::vector<double> &direction)
{
  if (i >= m_Direction.size() )
    {
    itkWarningMacro("Index: " << i <<
                      " is out of bounds, expected maximum is " <<
                      m_Direction.size());
    itkExceptionMacro("Index: " << i <<
                      " is out of bounds, expected maximum is " <<
                      m_Direction.size());
    }
  this->Modified();
  m_Direction[i] = direction;
}

void ImageIOBase::SetDirection(unsigned int i, vnl_vector<double> &direction)
{
  if (i >= m_Direction.size() )
    {
    itkWarningMacro("Index: " << i <<
                      " is out of bounds, expected maximum is " <<
                      m_Direction.size());
    itkExceptionMacro("Index: " << i <<
                      " is out of bounds, expected maximum is " <<
                      m_Direction.size());
    }
  this->Modified();
  std::vector<double> v;
  v.resize(m_Direction.size());
  for (unsigned int j=0; j < v.size(); j++)
    {
    v[j] = direction[j];
    }
  m_Direction[i] = v;
}

const std::type_info& ImageIOBase::GetComponentTypeInfo() const
{
  switch(m_ComponentType)
    {
    case UCHAR:
      return typeid(unsigned char);
    case CHAR:
      return typeid(char);
    case USHORT:
      return typeid(unsigned short);
    case SHORT:
      return typeid(short);
    case UINT:
      return typeid(unsigned int);
    case INT:
      return typeid(int);
    case ULONG:
      return typeid(unsigned long);
    case LONG:
      return typeid(long);
    case FLOAT:
      return typeid(float);
    case DOUBLE:
      return typeid(double);
    case CSHORT:
      return typeid(std::complex<short>);
    case CINT:
      return typeid(std::complex<int>);
    case CFLOAT:
      return typeid(std::complex<float>);
    case CDOUBLE:
      return typeid(std::complex<double>);
    case UNKNOWNCOMPONENTTYPE:
    default:
      itkExceptionMacro ("Unknown component type: " << m_ComponentType);
    }
}

//
// This macro enforces pixel type information to be available for all different
// pixel types.
//
template <typename T>
bool
itkSetPixelType(ImageIOBase *This,
                const std::type_info &ptype,
                ImageIOBase::IOComponentType ntype,
                T itkNotUsed( dummy ) )
{
  if( ptype == typeid(T) )
    {
    This->SetNumberOfComponents(1);
    This->SetComponentType(ntype);
    This->SetPixelType(ImageIOBase::SCALAR);
    return true;
    }
  else if ( ptype == typeid(itk::RGBPixel<T>) )
    {
    This->SetNumberOfComponents(3);
    This->SetComponentType(ntype);
    This->SetPixelType(ImageIOBase::RGB);
    return true;
    }
  else if ( ptype == typeid(itk::RGBAPixel<T>) )
    {
    This->SetNumberOfComponents(4);
    This->SetComponentType(ntype);
    This->SetPixelType(ImageIOBase::RGBA);
    return true;
    }
  else if ( ptype == typeid(itk::Vector<T,2>) )
    {
    This->SetNumberOfComponents(2);
    This->SetPixelType(ImageIOBase::VECTOR);
    This->SetComponentType(ntype);
    return true;
    }
  else if ( ptype == typeid(itk::Vector<T,3>) )
    {
    This->SetNumberOfComponents(3);
    This->SetPixelType(ImageIOBase::VECTOR);
    This->SetComponentType(ntype);
    return true;
    }
  else if ( ptype == typeid(itk::Vector<T,4>) )
    {
    This->SetNumberOfComponents(4);
    This->SetPixelType(ImageIOBase::VECTOR);
    This->SetComponentType(ntype);
    return true;
    }
  else if ( ptype == typeid(itk::Vector<T,5>) )
    {
    This->SetNumberOfComponents(5);
    This->SetPixelType(ImageIOBase::VECTOR);
    This->SetComponentType(ntype);
    return true;
    }
  else if ( ptype == typeid(itk::Vector<T,6>) )
    {
    This->SetNumberOfComponents(6);
    This->SetPixelType(ImageIOBase::VECTOR);
    This->SetComponentType(ntype);
    return true;
    }
  else if ( ptype == typeid(itk::Vector<T,7>) )
    {
    This->SetNumberOfComponents(7);
    This->SetPixelType(ImageIOBase::VECTOR);
    This->SetComponentType(ntype);
    return true;
    }
  else if ( ptype == typeid(itk::CovariantVector<T,2>) )
    {
    This->SetNumberOfComponents(2);
    This->SetPixelType(ImageIOBase::COVARIANTVECTOR);
    This->SetComponentType(ntype);
    return true;
    }
  else if ( ptype == typeid(itk::CovariantVector<T,3>) )
    {
    This->SetNumberOfComponents(3);
    This->SetPixelType(ImageIOBase::COVARIANTVECTOR);
    This->SetComponentType(ntype);
    return true;
    }
  else if ( ptype == typeid(itk::CovariantVector<T,4>) )
    {
    This->SetNumberOfComponents(4);
    This->SetPixelType(ImageIOBase::COVARIANTVECTOR);
    This->SetComponentType(ntype);
    return true;
    }
  else if ( ptype == typeid(itk::CovariantVector<T,5>) )
    {
    This->SetNumberOfComponents(5);
    This->SetPixelType(ImageIOBase::COVARIANTVECTOR);
    This->SetComponentType(ntype);
    return true;
    }
  else if ( ptype == typeid(itk::CovariantVector<T,6>) )
    {
    This->SetNumberOfComponents(6);
    This->SetPixelType(ImageIOBase::COVARIANTVECTOR);
    This->SetComponentType(ntype);
    return true;
    }
  else if ( ptype == typeid(itk::CovariantVector<T,7>) )
    {
    This->SetNumberOfComponents(7);
    This->SetPixelType(ImageIOBase::COVARIANTVECTOR);
    This->SetComponentType(ntype);
    return true;
    }
  else if ( ptype == typeid(itk::FixedArray<T,2>) )
    {
    This->SetNumberOfComponents(2);
    This->SetPixelType(ImageIOBase::FIXEDARRAY);
    This->SetComponentType(ntype);
    return true;
    }
  else if ( ptype == typeid(itk::FixedArray<T,3>) )
    {
    This->SetNumberOfComponents(3);
    This->SetPixelType(ImageIOBase::FIXEDARRAY);
    This->SetComponentType(ntype);
    return true;
    }
  else if ( ptype == typeid(itk::FixedArray<T,4>) )
    {
    This->SetNumberOfComponents(4);
    This->SetPixelType(ImageIOBase::FIXEDARRAY);
    This->SetComponentType(ntype);
    return true;
    }
  else if ( ptype == typeid(itk::SymmetricSecondRankTensor<T,2>) )
    {
    This->SetNumberOfComponents(3);
    This->SetPixelType(ImageIOBase::SYMMETRICSECONDRANKTENSOR);
    This->SetComponentType(ntype);
    return true;
    }
  else if ( ptype == typeid(itk::SymmetricSecondRankTensor<T,3>) )
    {
    This->SetNumberOfComponents(6);
    This->SetPixelType(ImageIOBase::SYMMETRICSECONDRANKTENSOR);
    This->SetComponentType(ntype);
    return true;
    }
  else if ( ptype == typeid(itk::SymmetricSecondRankTensor<T,4>) )
    {
    This->SetNumberOfComponents(10);
    This->SetPixelType(ImageIOBase::SYMMETRICSECONDRANKTENSOR);
    This->SetComponentType(ntype);
    return true;
    }
  else if ( ptype == typeid(itk::SymmetricSecondRankTensor<T,5>) )
    {
    This->SetNumberOfComponents(15);
    This->SetPixelType(ImageIOBase::SYMMETRICSECONDRANKTENSOR);
    This->SetComponentType(ntype);
    return true;
    }
  else if ( ptype == typeid(itk::SymmetricSecondRankTensor<T,6>) )
    {
    This->SetNumberOfComponents(21);
    This->SetPixelType(ImageIOBase::SYMMETRICSECONDRANKTENSOR);
    This->SetComponentType(ntype);
    return true;
    }
  else if ( ptype == typeid(itk::DiffusionTensor3D<T>) )
    {
    This->SetNumberOfComponents(6);
    This->SetComponentType(ntype);
    This->SetPixelType(ImageIOBase::DIFFUSIONTENSOR3D);
    return true;
    }
  else if ( ptype == typeid(itk::Matrix<T,2,2>) )
    {
    This->SetNumberOfComponents(4);
    This->SetComponentType(ntype);
    This->SetPixelType(ImageIOBase::MATRIX);
    return true;
    }
  else if ( ptype == typeid(itk::Matrix<T,3,3>) )
    {
    This->SetNumberOfComponents(9);
    This->SetComponentType(ntype);
    This->SetPixelType(ImageIOBase::MATRIX);
    return true;
    }
  else if ( ptype == typeid(itk::Matrix<T,4,4>) )
    {
    This->SetNumberOfComponents(16);
    This->SetComponentType(ntype);
    This->SetPixelType(ImageIOBase::MATRIX);
    return true;
    }
  return false;
}

//
// This macro enforces pixel type information to be available for all different
// pixel types.
//
template <typename T>
bool
itkSetPixelType(ImageIOBase *This,
                const std::type_info &ptype,
                ImageIOBase::IOComponentType itkNotUsed(ntype),
                std::complex<T> itkNotUsed( dummy ) )
{
  if (ptype == typeid(std::complex<short>))
    {
    This->SetNumberOfComponents(1);
    This->SetComponentType(ImageIOBase::CSHORT);
    This->SetPixelType(ImageIOBase::COMPLEX);
    return true;
    }
  if (ptype == typeid(std::complex<int>))
    {
    This->SetNumberOfComponents(1);
    This->SetComponentType(ImageIOBase::CINT);
    This->SetPixelType(ImageIOBase::COMPLEX);
    return true;
    }
  if (ptype == typeid(std::complex<float>))
    {
    This->SetNumberOfComponents(1);
    This->SetComponentType(ImageIOBase::CFLOAT);
    This->SetPixelType(ImageIOBase::COMPLEX);
    return true;
    }
  if (ptype == typeid(std::complex<double>))
    {
    This->SetNumberOfComponents(1);
    This->SetComponentType(ImageIOBase::CDOUBLE);
    This->SetPixelType(ImageIOBase::COMPLEX);
    return true;
    }
  return false;
}


bool ImageIOBase::SetPixelTypeInfo(const std::type_info& ptype)
{
  this->SetNumberOfComponents(1);
  this->SetPixelType(ImageIOBase::UNKNOWNPIXELTYPE);
  this->SetComponentType(ImageIOBase::UNKNOWNCOMPONENTTYPE);

  if (!itkSetPixelType(this,ptype,ImageIOBase::CHAR, char(0) ) &&
      !itkSetPixelType(this,ptype,ImageIOBase::UCHAR, (unsigned char)0) &&
      !itkSetPixelType(this,ptype,ImageIOBase::SHORT,(short)(0)) &&
      !itkSetPixelType(this,ptype,ImageIOBase::USHORT,(unsigned short)(0)) &&
      !itkSetPixelType(this,ptype,ImageIOBase::INT,(int)(0)) &&
      !itkSetPixelType(this,ptype,ImageIOBase::UINT,(unsigned int)(0)) &&
      !itkSetPixelType(this,ptype,ImageIOBase::LONG,(long)(0)) &&
      !itkSetPixelType(this,ptype,ImageIOBase::ULONG,(unsigned long)(0)) &&
      !itkSetPixelType(this,ptype,ImageIOBase::FLOAT,(float)(0)) &&
      !itkSetPixelType(this,ptype,ImageIOBase::DOUBLE,(double)(0)) &&
      !itkSetPixelType(this,ptype,ImageIOBase::CSHORT,(std::complex<short>)(0)) &&
      !itkSetPixelType(this,ptype,ImageIOBase::CINT,(std::complex<int>)(0)) &&
      !itkSetPixelType(this,ptype,ImageIOBase::CFLOAT,(std::complex<float>)(0)) &&
      !itkSetPixelType(this,ptype,ImageIOBase::CDOUBLE,(std::complex<double>)(0)) )
    {
    if ( ptype == typeid(itk::Offset<2>) )
      {
      this->SetNumberOfComponents(2);
      this->SetPixelType(ImageIOBase::OFFSET);
      this->SetComponentType(ImageIOBase::LONG);
      }
    else if ( ptype == typeid(itk::Offset<3>) )
      {
      this->SetNumberOfComponents(3);
      this->SetPixelType(ImageIOBase::OFFSET);
      this->SetComponentType(ImageIOBase::LONG);
      }
    else if ( ptype == typeid(itk::Offset<4>) )
      {
      this->SetNumberOfComponents(4);
      this->SetPixelType(ImageIOBase::OFFSET);
      this->SetComponentType(ImageIOBase::LONG);
      }

    }

  if( this->GetPixelType() == ImageIOBase::UNKNOWNPIXELTYPE )
    {
    itkExceptionMacro("Pixel type currently not supported. typeid.name = " << ptype.name() );
    return false;
    }

  if( this->GetComponentType() == ImageIOBase::UNKNOWNCOMPONENTTYPE )
    {
    itkExceptionMacro("Pixel Component type currently not supported. typeid.name = " << ptype.name() );
    return false;
    }

  return true;
}

void ImageIOBase::ComputeStrides()
{
  unsigned int i;

  m_Strides[0] = this->GetComponentSize();
  m_Strides[1] = m_NumberOfComponents * m_Strides[0];
  for (i = 2; i <= (m_NumberOfDimensions+1); i++)
    {
    m_Strides[i] = m_Dimensions[i-2] * m_Strides[i-1];
    }
}

// Calculates the image size in PIXELS
ImageIOBase::SizeType
ImageIOBase
::GetImageSizeInPixels() const
{
  unsigned int i;
  SizeType numPixels = 1;

  for (i = 0; i < m_NumberOfDimensions; i++)
    {
    numPixels *= m_Dimensions[i];
    }

  return numPixels;
}

ImageIOBase::SizeType
ImageIOBase
::GetImageSizeInComponents() const
{
  return (this->GetImageSizeInPixels() * m_NumberOfComponents);
}

ImageIOBase::SizeType
ImageIOBase
::GetImageSizeInBytes () const
{
  return (this->GetImageSizeInComponents() * this->GetComponentSize());
}

ImageIOBase::SizeType
ImageIOBase
::GetComponentStride() const
{
  return m_Strides[0];
}

ImageIOBase::SizeType
ImageIOBase
::GetPixelStride () const
{
  return m_Strides[1];
}

ImageIOBase::SizeType
ImageIOBase
::GetRowStride () const
{
  return m_Strides[2];
}

ImageIOBase::SizeType
ImageIOBase
::GetSliceStride () const
{
  return m_Strides[3];
}

void ImageIOBase::SetNumberOfDimensions(unsigned int dim)
{
  if(dim != m_NumberOfDimensions)
    {
    m_Origin.resize( dim );
    m_Spacing.resize( dim );
    m_Direction.resize( dim );
    m_Strides.resize( dim+2 );
    m_NumberOfDimensions = dim;
    m_Dimensions.resize( dim );
    m_Direction.resize( dim );
    std::vector<double> axis( dim );
    for (unsigned int i=0; i<dim; i++)
      {
      for (unsigned int j=0; j < dim; j++)
        {
        if (i == j)
          {
          axis[j] = 1.0;
          }
        else
          {
          axis[j] = 0.0;
          }
        }
      this->SetDirection(i, axis);
      this->SetOrigin(i, 0.0);
      this->SetSpacing(i, 1.0);
      }
    this->Modified();
    }
}

bool
ImageIOBase
::ReadBufferAsBinary(std::istream& is, void *buffer, ImageIOBase::SizeType num)
{

  const std::streamsize numberOfBytesToBeRead = itk::Math::CastWithRangeCheck< std::streamsize>( num );

  is.read( static_cast<char *>( buffer ), numberOfBytesToBeRead );

  const std::streamsize numberOfBytesRead = is.gcount();

#ifdef __APPLE_CC__
  // fail() is broken in the Mac. It returns true when reaches eof().
  if ( numberOfBytesRead != numberOfBytesToBeRead )
#else
  if ( ( numberOfBytesRead != numberOfBytesToBeRead )  || is.fail() )
#endif
    {
    return false; // read failed
    }

  return true;

}


unsigned int ImageIOBase::GetPixelSize() const
{
  if (m_ComponentType == UNKNOWNCOMPONENTTYPE
      || m_PixelType == UNKNOWNPIXELTYPE)
    {
    itkExceptionMacro ("Unknown pixel or component type: ("
                       << m_PixelType << ", " << m_ComponentType << ")");
    return 0;
    }

  return this->GetComponentSize() * this->GetNumberOfComponents();
}


unsigned int ImageIOBase::GetComponentSize() const
{
  switch(m_ComponentType)
    {
    case UCHAR:
      return sizeof(unsigned char);
    case CHAR:
      return sizeof(char);
    case USHORT:
      return sizeof(unsigned short);
    case SHORT:
      return sizeof(short);
    case UINT:
      return sizeof(unsigned int);
    case INT:
      return sizeof(int);
    case ULONG:
      return sizeof(unsigned long);
    case LONG:
      return sizeof(long);
    case FLOAT:
      return sizeof(float);
    case DOUBLE:
      return sizeof(double);
    case CSHORT:
      return sizeof(std::complex<short>);
    case CINT:
      return sizeof(std::complex<int>);
    case CFLOAT:
      return sizeof(std::complex<float>);
    case CDOUBLE:
      return sizeof(std::complex<double>);
    case UNKNOWNCOMPONENTTYPE:
    default:
      itkExceptionMacro ("Unknown component type: " << m_ComponentType);
    }

  return 0;
}

std::string ImageIOBase::GetFileTypeAsString(FileType t)
{
  std::string s;
  switch(t)
    {
    case ASCII:
      return s = "ASCII";
    case Binary:
      return s = "Binary";
    case TypeNotApplicable:
    default:
      return s = "TypeNotApplicable";
    }
}

std::string ImageIOBase::GetByteOrderAsString(ByteOrder t)
{
  std::string s;
  switch(t)
    {
    case BigEndian:
      return s = "BigEndian";
    case LittleEndian:
      return s = "LittleEndian";
    case OrderNotApplicable:
    default:
      return s = "OrderNotApplicable";
    }
}

std::string ImageIOBase::GetComponentTypeAsString(IOComponentType t)
{
  std::string s;
  switch(t)
    {
    case UCHAR:
      return (s = "unsigned_char");
    case CHAR:
      return (s = "char");
    case USHORT:
      return (s = "unsigned_short");
    case SHORT:
      return (s = "short");
    case UINT:
      return (s = "unsigned_int");
    case INT:
      return (s = "int");
    case ULONG:
      return (s = "unsigned_long");
    case LONG:
      return (s = "long");
    case FLOAT:
      return (s = "float");
    case DOUBLE:
      return (s = "double");
    case CSHORT:
      return (s = "complex_short");
    case CINT:
      return (s = "complex_int");
    case CFLOAT:
      return (s = "complex_float");
    case CDOUBLE:
      return (s = "complex_double");
    case UNKNOWNCOMPONENTTYPE:
    default:
      return (s = "unknown");
    }
}

std::string ImageIOBase::GetPixelTypeAsString(IOPixelType t)
{
  std::string s;
  switch(t)
    {
    case SCALAR:
      return (s = "scalar");
    case VECTOR:
      return (s = "vector");
    case COVARIANTVECTOR:
      return (s = "covariant_vector");
    case POINT:
      return (s = "point");
    case OFFSET:
      return (s = "offset");
    case RGB:
      return (s = "rgb");
    case RGBA:
      return (s = "rgba");
    case SYMMETRICSECONDRANKTENSOR:
      return (s = "symmetric_second_rank_tensor");
    case DIFFUSIONTENSOR3D:
      return (s = "diffusion_tensor_3D");
    case COMPLEX:
      return (s = "complex");
    case UNKNOWNPIXELTYPE:
    default:
      return (s = "unknown");
    }
}

namespace {
template <class TComponent>
void WriteBuffer(std::ostream& os, const TComponent *buffer, ImageIOBase::SizeType num)
{
  const TComponent *ptr = buffer;
  typedef typename itk::NumericTraits<TComponent>::PrintType PrintType;
  for (ImageIOBase::SizeType i=0; i < num; i++)
    {
    if ( !(i%6) && i )
      os << "\n";
    os << PrintType(*ptr++) << " ";
    }
}
}
void ImageIOBase::WriteBufferAsASCII(std::ostream& os, const void *buffer,
                                     IOComponentType ctype,
                                     ImageIOBase::SizeType numComp)
{
  switch (ctype)
    {
    case UCHAR:
      {
      typedef const unsigned char * Type;
      Type buf = reinterpret_cast<Type>(buffer);
      WriteBuffer(os, buf, numComp);
      }
      break;
    case CHAR:
      {
      typedef const char * Type;
      Type buf = reinterpret_cast<Type>(buffer);
      WriteBuffer(os, buf, numComp);
      }
      break;

    case USHORT:
      {
      typedef const unsigned short * Type;
      Type buf = reinterpret_cast<Type>(buffer);
      WriteBuffer(os, buf, numComp);
      }
      break;

    case SHORT:
      {
      typedef const short * Type;
      Type buf = reinterpret_cast<Type>(buffer);
      WriteBuffer(os, buf, numComp);
      }
      break;

    case UINT:
      {
      typedef const unsigned int * Type;
      Type buf = reinterpret_cast<Type>(buffer);
      WriteBuffer(os, buf, numComp);
      }
      break;

    case INT:
      {
      typedef const int * Type;
      Type buf = reinterpret_cast<Type>(buffer);
      WriteBuffer(os, buf, numComp);
      }
      break;

    case ULONG:
      {
      typedef const unsigned long * Type;
      Type buf = reinterpret_cast<Type>(buffer);
      WriteBuffer(os, buf, numComp);
      }
      break;

    case LONG:
      {
      typedef const long * Type;
      Type buf = reinterpret_cast<Type>(buffer);
      WriteBuffer(os, buf, numComp);
      }
      break;

    case FLOAT:
      {
      typedef const float * Type;
      Type buf = reinterpret_cast<Type>(buffer);
      WriteBuffer(os, buf, numComp);
      }
      break;

    case DOUBLE:
      {
      typedef const double * Type;
      Type buf = reinterpret_cast<Type>(buffer);
      WriteBuffer(os, buf, numComp);
      }
      break;

    case CSHORT:
      {
      typedef const std::complex<short> * Type;
      Type buf = reinterpret_cast<Type>(buffer);
      WriteBuffer(os, buf, numComp);
      }
      break;

    case CINT:
      {
      typedef const std::complex<int> * Type;
      Type buf = reinterpret_cast<Type>(buffer);
      WriteBuffer(os, buf, numComp);
      }
      break;

    case CFLOAT:
      {
      typedef const std::complex<float> * Type;
      Type buf = reinterpret_cast<Type>(buffer);
      WriteBuffer(os, buf, numComp);
      }
      break;

    case CDOUBLE:
      {
      typedef const std::complex<double> * Type;
      Type buf = reinterpret_cast<Type>(buffer);
      WriteBuffer(os, buf, numComp);
      }
      break;

    default:
      break;
    }

}


template <class TComponent>
void ReadBuffer(std::istream& is, TComponent *buffer, ImageIOBase::SizeType num)
{

  typedef typename itk::NumericTraits<TComponent>::PrintType PrintType;
  PrintType temp;
  TComponent *ptr = buffer;
  for( ImageIOBase::SizeType i=0; i < num; i++, ptr++ )
    {
    is >> temp;
    *ptr = static_cast<TComponent>( temp );
    }
}

void ImageIOBase::ReadBufferAsASCII(std::istream& is, void *buffer,
                                    IOComponentType ctype,
                                    ImageIOBase::SizeType numComp)
{
  switch (ctype)
    {
    case UCHAR:
      {
      unsigned char *buf = reinterpret_cast<unsigned char*>(buffer);
      ReadBuffer(is, buf, numComp);
      }
      break;
    case CHAR:
      {
      char *buf = reinterpret_cast<char*>(buffer);
      ReadBuffer(is, buf, numComp);
      }
      break;

    case USHORT:
      {
      unsigned short *buf = reinterpret_cast<unsigned short*>(buffer);
      ReadBuffer(is, buf, numComp);
      }
      break;

    case SHORT:
      {
      short *buf = reinterpret_cast<short*>(buffer);
      ReadBuffer(is, buf, numComp);
      }
      break;

    case UINT:
      {
      unsigned int *buf = reinterpret_cast<unsigned int*>(buffer);
      ReadBuffer(is, buf, numComp);
      }
      break;

    case INT:
      {
      int *buf = reinterpret_cast<int*>(buffer);
      ReadBuffer(is, buf, numComp);
      }
      break;

    case ULONG:
      {
      unsigned long *buf = reinterpret_cast<unsigned long*>(buffer);
      ReadBuffer(is, buf, numComp);
      }
      break;

    case LONG:
      {
      long *buf = reinterpret_cast<long*>(buffer);
      ReadBuffer(is, buf, numComp);
      }
      break;

    case FLOAT:
      {
      float *buf = reinterpret_cast<float*>(buffer);
      ReadBuffer(is, buf, numComp);
      }
      break;

    case DOUBLE:
      {
      double *buf = reinterpret_cast<double*>(buffer);
      ReadBuffer(is, buf, numComp);
      }
      break;

    case CSHORT:
      {
      std::complex<short> *buf = reinterpret_cast<std::complex<short>*>(buffer);
      ReadBuffer(is, buf, numComp);
      }
      break;

    case CINT:
      {
      std::complex<int> *buf = reinterpret_cast<std::complex<int>*>(buffer);
      ReadBuffer(is, buf, numComp);
      }
      break;

    case CFLOAT:
      {
      std::complex<float> *buf = reinterpret_cast<std::complex<float>*>(buffer);
      ReadBuffer(is, buf, numComp);
      }
      break;

    case CDOUBLE:
      {
      std::complex<double> *buf = reinterpret_cast<std::complex<double>*>(buffer);
      ReadBuffer(is, buf, numComp);
      }
      break;


    default:
      break;
    }

}


unsigned int
ImageIOBase::GetActualNumberOfSplitsForWritingCanStreamWrite(unsigned int numberOfRequestedSplits,
                                                             const itk::ImageIORegion &pasteRegion) const
{
  // Code from ImageRegionSplitter:GetNumberOfSplits
  int splitAxis;
  const itk::ImageIORegion::SizeType &regionSize = pasteRegion.GetSize();


  // split on the outermost dimension available
  splitAxis = pasteRegion.GetImageDimension() - 1;
  while (regionSize[splitAxis] == 1)
    {
    --splitAxis;
    if (splitAxis < 0)
      { // cannot split
      itkDebugMacro("  Cannot Split");
      return 1;
      }
    }

  // determine the actual number of pieces that will be generated
  itk::ImageIORegion::SizeType::value_type range = regionSize[splitAxis];
  int valuesPerPiece = itk::Math::Ceil<int>(range/double(numberOfRequestedSplits));
  int maxPieceUsed = itk::Math::Ceil<int>(range/double(valuesPerPiece)) - 1;

  return maxPieceUsed+1;
}

unsigned int
ImageIOBase::GetActualNumberOfSplitsForWriting(unsigned int numberOfRequestedSplits,
                                               const itk::ImageIORegion &pasteRegion,
                                               const itk::ImageIORegion &largestPossibleRegion)
{
  if (this->CanStreamWrite())
    {
      return GetActualNumberOfSplitsForWritingCanStreamWrite(numberOfRequestedSplits, pasteRegion);
    }
  if (pasteRegion != largestPossibleRegion)
    {
      itkExceptionMacro("Pasting is not supported! Can't write:" << this->GetFileName());
    }
  if (numberOfRequestedSplits != 1)
    {
    itkDebugMacro("Requested more then 1 splits for streaming");
    itkDebugMacro("This IO class does not support streaming!");
    }
  return 1;
}

itk::ImageIORegion
ImageIOBase::GetSplitRegionForWritingCanStreamWrite(unsigned int ithPiece,
                                                               unsigned int numberOfActualSplits,
                                                               const itk::ImageIORegion &pasteRegion) const
{
    // Code from ImageRegionSplitter:GetSplit
  int splitAxis;
  itk::ImageIORegion splitRegion;
  itk::ImageIORegion::IndexType splitIndex;
  itk::ImageIORegion::SizeType splitSize, regionSize;

  // Initialize the splitRegion to the requested region
  splitRegion = pasteRegion;
  splitIndex = splitRegion.GetIndex();
  splitSize = splitRegion.GetSize();

  regionSize = pasteRegion.GetSize();

  // split on the outermost dimension available
  splitAxis = pasteRegion.GetImageDimension() - 1;
  while (regionSize[splitAxis] == 1)
    {
    --splitAxis;
    if (splitAxis < 0)
      { // cannot split
      itkDebugMacro("  Cannot Split");
      return splitRegion;
      }
    }

  // determine the actual number of pieces that will be generated
  itk::ImageIORegion::SizeType::value_type range = regionSize[splitAxis];
  int valuesPerPiece = itk::Math::Ceil<int>(range/(double)numberOfActualSplits);
  int maxPieceUsed = itk::Math::Ceil<int>(range/(double)valuesPerPiece) - 1;

  // Split the region
  if ((int) ithPiece < maxPieceUsed)
    {
    splitIndex[splitAxis] += ithPiece*valuesPerPiece;
    splitSize[splitAxis] = valuesPerPiece;
    }
  if ((int) ithPiece == maxPieceUsed)
    {
    splitIndex[splitAxis] += ithPiece*valuesPerPiece;
    // last piece needs to process the "rest" dimension being split
    splitSize[splitAxis] = splitSize[splitAxis] - ithPiece*valuesPerPiece;
    }

  // set the split region ivars
  splitRegion.SetIndex( splitIndex );
  splitRegion.SetSize( splitSize );

  itkDebugMacro("  Split Piece: " << splitRegion );

  return splitRegion;
}

itk::ImageIORegion
ImageIOBase::GetSplitRegionForWriting(unsigned int ithPiece,
                                      unsigned int numberOfActualSplits,
                                      const itk::ImageIORegion &pasteRegion,
                                      const itk::ImageIORegion &largestPossibleRegion)
{
   if (this->CanStreamWrite())
    {
      return GetSplitRegionForWritingCanStreamWrite(ithPiece, numberOfActualSplits, pasteRegion);
    }
  return largestPossibleRegion;
}

/** Given a requested region, determine what could be the region that we can
 * read from the file. This is called the streamable region, which will be
 * smaller than the LargestPossibleRegion and greater or equal to the
 * RequestedRegion */
itk::ImageIORegion
ImageIOBase
::GenerateStreamableReadRegionFromRequestedRegion(
    const itk::ImageIORegion & requested ) const
{
  //
  // The default implementations determines that the streamable region is
  // equal to the minimal size of the image in the file. That is two
  // say the return ImageIORegion::GetImageSizeInPixels() is equal to
  // the number in the file.
  //

  // Since the image in the file may have a lower or higher dimension
  // than the image type over which the ImageFileReader is
  // being instantiated we must choose an image dimension which will
  // represent all the pixels. That is we can trim trailing 1s.

  unsigned int minIODimension = this->m_NumberOfDimensions;
  while (minIODimension)
    {
    if (this->m_Dimensions[minIODimension-1] == 1)
      {
      --minIODimension;
      }
    else
      {
      break;
      }
    }

  // dimension size we use to represent the region
  unsigned int maxDimension =
    minIODimension > requested.GetImageDimension() ? minIODimension : requested.GetImageDimension();

  // First: allocate with the correct dimensions
  itk::ImageIORegion streamableRegion( maxDimension );

  // Second: copy only the number of dimension that the file has.
  for( unsigned int i=0; i < minIODimension; i++ )
    {
    streamableRegion.SetSize( i, this->m_Dimensions[i] );
    streamableRegion.SetIndex( i, 0 );
    }

  // Third: set the rest to the default : start = 0, size = 1
  for( unsigned int j=minIODimension; j<streamableRegion.GetImageDimension(); j++ )
    {
    streamableRegion.SetSize( j, 1 );
    streamableRegion.SetIndex( j, 0 );
    }

  // Finally: return the streamable region
  return streamableRegion;
}

/** Return the directions that this particular ImageIO would use by default
 *  in the case the recipient image dimension is smaller than the dimension
 *  of the image in file. */
std::vector<double>
ImageIOBase
::GetDefaultDirection( unsigned int k ) const
{
  std::vector<double> axis;
  axis.resize( this->GetNumberOfDimensions() );

  // Fill up with the equivalent of a line from an Identity matrix
  for( unsigned int r=0; r<axis.size(); r++ )
    {
    axis[r] = 0.0;
    }

  axis[k] = 1.0;

  return axis;
}

void
ImageIOBase
::DoMapBuffer(void* buffer, size_t numberOfPixels, std::vector<unsigned int>& bandList)
{
  size_t componentSize = this->GetComponentSize();
  size_t inPixelSize = componentSize * this->GetNumberOfComponents();
  size_t outPixelSize = componentSize * bandList.size();
  char* inPos = static_cast<char*>(buffer);
  char* outPos = static_cast<char*>(buffer);
  bool workBackward = (outPixelSize > inPixelSize);
  char *pixBuffer = new char[outPixelSize];

  memset(pixBuffer, 0, outPixelSize);

  if (workBackward)
    {
    inPos = inPos + numberOfPixels*inPixelSize;
    outPos = outPos + numberOfPixels*outPixelSize;
    for (size_t n=0 ; n<numberOfPixels ; n++)
      {
      inPos -= inPixelSize;
      outPos -= outPixelSize;
      for (unsigned int i=0 ; i < bandList.size() ; i++)
        {
        memcpy(pixBuffer + i*componentSize, inPos + bandList[i]*componentSize, componentSize);
        }
      // copy pixBuffer to output
      memcpy(outPos, pixBuffer, outPixelSize);
      }
    }
  else
    {
    for (size_t n=0 ; n<numberOfPixels ; n++)
      {
      for (unsigned int i=0 ; i < bandList.size() ; i++)
        {
        memcpy(pixBuffer + i*componentSize, inPos + bandList[i]*componentSize, componentSize);
        }
      // copy pixBuffer to output
      memcpy(outPos, pixBuffer, outPixelSize);
      inPos += inPixelSize;
      outPos += outPixelSize;
      }
    }

  delete[] pixBuffer;
}

void ImageIOBase::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "FileName: " << m_FileName << std::endl;
  os << indent << "FileType: " << ImageIOBase::GetFileTypeAsString(m_FileType) << std::endl;
  os << indent << "ByteOrder: " << ImageIOBase::GetByteOrderAsString(m_ByteOrder) << std::endl;
  os << indent << "IORegion: " << std::endl;
  m_IORegion.Print(os, indent.GetNextIndent());
  os << indent << "Number of Components/Pixel: " << m_NumberOfComponents << "\n";
  os << indent << "Pixel Type: " << ImageIOBase::GetPixelTypeAsString(m_PixelType) << std::endl;
  os << indent << "Component Type: " << ImageIOBase::GetComponentTypeAsString(m_ComponentType)
     << std::endl;
  os << indent << "Dimensions: ( ";
  for (unsigned int i=0; i < m_NumberOfDimensions; i++)
    {
    os << m_Dimensions[i] << " ";
    }
  os << ")" << std::endl;
  os << indent << "Origin: ( ";
 for (unsigned int i=0; i < m_NumberOfDimensions; i++)
    {
    os << m_Origin[i] << " ";
    }
  os << ")" << std::endl;

  if (m_UseCompression)
    {
    os << indent << "UseCompression: On" << std::endl;
    }
  else
    {
    os << indent << "UseCompression: Off" << std::endl;
    }
  if (m_UseStreamedReading)
    {
    os << indent << "UseStreamedReading: On" << std::endl;
    }
  else
    {
    os << indent << "UseStreamedReading: Off" << std::endl;
    }
  if (m_UseStreamedWriting)
    {
    os << indent << "UseStreamedWriting: On" << std::endl;
    }
  else
    {
    os << indent << "UseStreamedWriting: Off" << std::endl;
    }
}

} //namespace itk
