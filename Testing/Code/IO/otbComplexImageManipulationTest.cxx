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
#include <complex>

#include "otbImageFileReader.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbMacro.h"
#include "itkImageRegionIteratorWithIndex.h"

// Do all comparison in double precision
const double Epsilon = 1.E-6;

template<class TPixel>
bool IsEqual(TPixel output, TPixel expected)
{
  typedef typename itk::NumericTraits<TPixel>::RealType       RealType;

  RealType outputReal   = static_cast<RealType>(output);
  RealType expectedReal = static_cast<RealType>(expected);
  // avoid division by zero
  return outputReal == expectedReal || (vcl_abs(expectedReal - outputReal) / vcl_abs(expectedReal) < Epsilon);
}

template<class TPixel>
bool IsEqual(std::complex<TPixel> output, std::complex<TPixel> expected)
{
  typedef typename itk::NumericTraits<std::complex<TPixel> >::RealType       RealType;
  typedef typename itk::NumericTraits<std::complex<TPixel> >::ScalarRealType ScalarRealType;

  RealType outputReal  ( static_cast<ScalarRealType>(output.real())  , static_cast<ScalarRealType>(output.imag()) );
  RealType expectedReal( static_cast<ScalarRealType>(expected.real()), static_cast<ScalarRealType>(expected.imag()) );

  // avoid division by zero
  return outputReal == expectedReal || (vcl_abs(expectedReal - outputReal) / vcl_abs(expectedReal) < Epsilon);
}

template<class TInternalPixel>
bool IsEqual(itk::VariableLengthVector<TInternalPixel> output, itk::VariableLengthVector<TInternalPixel> expected)
{
  bool result = true;
  for(unsigned int i = 0; i < output.Size(); ++i)
    {
    result = result && IsEqual(output[i], expected[i]);
    }
  return result;
}


template<class TIndex, class TPixel>
bool TestCompare(TIndex idx, TPixel output, TPixel expected)
{
  std::cout << "PIXEL " << idx << "  -->  "
            << "OUTPUT = "   << output
            << "   ||   "
            << "EXPECTED = " << expected
            << std::endl;

  if (!IsEqual(output, expected))
    {
    std::cerr << "ERROR at position " << idx << ". Got " << output << ", expected " << expected << std::endl;
    return false;
    }
  return true;
}

template<class TImagePointerType>
itk::ImageRegionIteratorWithIndex<typename TImagePointerType::ObjectType>
ReadRegion(const char* filename, TImagePointerType& image, unsigned int w, unsigned int h)
{
  typedef TImagePointerType               ImagePointerType;
  typedef typename ImagePointerType::ObjectType    ImageType;
  typedef typename ImageType::RegionType  RegionType;
  typedef typename ImageType::IndexType   IndexType;
  typedef typename ImageType::SizeType    SizeType;

  IndexType idx;
  idx.Fill(0);

  SizeType  size;
  size[0] = w;
  size[1] = h;

  RegionType region;
  region.SetSize(size);
  region.SetIndex(idx);

  typedef otb::ImageFileReader<ImageType> ReaderType;
  typename ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(filename);
  reader->GetOutput()->SetRequestedRegion(region);
  reader->Update();

  image = reader->GetOutput();

  return itk::ImageRegionIteratorWithIndex<ImageType>(image, region);
}

/***********
 * 1.
 * Read Monoband Scalar as Image<complex>
 * out : real = in, imag = 0
 ***********/
template<class InternalType>
int otbMonobandScalarToImageComplexGeneric(int itkNotUsed(argc), char * argv[])
{
  typedef InternalType                        RealType;
  typedef std::complex<RealType>              ComplexType;

  typedef ComplexType                         PixelType;
  typedef otb::Image<PixelType, 2>            ImageType;

  const unsigned int w = 10;
  const unsigned int h = 2;

  typename ImageType::Pointer image;
  itk::ImageRegionIteratorWithIndex<ImageType> it = ReadRegion(argv[1], image, w, h);

  typename ImageType::SizeType largestRegion;
  largestRegion = image->GetLargestPossibleRegion().GetSize();

  unsigned int count = 0;
  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
    count = it.GetIndex()[1] * largestRegion[0] + it.GetIndex()[0];

    if ( !TestCompare(it.GetIndex(), it.Get(), ComplexType(count, 0)) )
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

int otbMonobandScalarToImageComplexFloat(int argc, char * argv[])
{
  return otbMonobandScalarToImageComplexGeneric<float>(argc, argv);
}

int otbMonobandScalarToImageComplexDouble(int argc, char * argv[])
{
  return otbMonobandScalarToImageComplexGeneric<double>(argc, argv);
}

int otbMonobandScalarToImageComplexInt(int argc, char * argv[])
{
  return otbMonobandScalarToImageComplexGeneric<int>(argc, argv);
}

int otbMonobandScalarToImageComplexShort(int argc, char * argv[])
{
  return otbMonobandScalarToImageComplexGeneric<short>(argc, argv);
}

/***********
 * 2.
 * Read MonobandComplex as Image<double>
 * out : norm(in)
 ***********/
template<class InternalType>
int otbMonobandComplexToImageScalarGeneric(int itkNotUsed(argc), char * argv[])
{
  typedef InternalType                                              InputType;
  typedef std::complex<InputType>                                   ComplexType;
  typedef typename itk::NumericTraits<ComplexType>::RealType        RealType;
  typedef typename itk::NumericTraits<ComplexType>::ScalarRealType  ScalarRealType;

  typedef InputType                                                 PixelType;
  typedef otb::Image<PixelType, 2>                                  ImageType;

  const unsigned int w = 10;
  const unsigned int h = 2;

  typename ImageType::Pointer image;
  itk::ImageRegionIteratorWithIndex<ImageType> it = ReadRegion(argv[1], image, w, h);

  typename ImageType::SizeType largestRegion;
  largestRegion = image->GetLargestPossibleRegion().GetSize();

  unsigned int count = 0;
  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
    count = 2 * (it.GetIndex()[1] * largestRegion[0] + it.GetIndex()[0]);

    ComplexType expected = ComplexType(count, count+1);
    RealType    expectedReal( static_cast<ScalarRealType>(expected.real()), static_cast<ScalarRealType>(expected.imag()) );

    if ( !TestCompare(it.GetIndex(), it.Get(), static_cast<InputType>(vcl_abs(expectedReal)) ) )
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

int otbMonobandComplexToImageScalarFloat(int argc, char * argv[])
{
  return otbMonobandComplexToImageScalarGeneric<float>(argc, argv);
}

int otbMonobandComplexToImageScalarDouble(int argc, char * argv[])
{
  return otbMonobandComplexToImageScalarGeneric<double>(argc, argv);
}

int otbMonobandComplexToImageScalarInt(int argc, char * argv[])
{
  return otbMonobandComplexToImageScalarGeneric<int>(argc, argv);
}

int otbMonobandComplexToImageScalarShort(int argc, char * argv[])
{
  return otbMonobandComplexToImageScalarGeneric<short>(argc, argv);
}

/***********
 * 3.
 * Read Monoband Complex as Image<complex>
 * out : in
 ***********/
template<class InternalType>
int otbMonobandComplexToImageComplexGeneric(int itkNotUsed(argc), char * argv[])
{
  typedef InternalType                        RealType;
  typedef std::complex<RealType>              ComplexType;

  typedef ComplexType                         PixelType;
  typedef otb::Image<PixelType, 2>            ImageType;

  const unsigned int w = 10;
  const unsigned int h = 2;

  typename ImageType::Pointer image;
  itk::ImageRegionIteratorWithIndex<ImageType> it = ReadRegion(argv[1], image, w, h);

  typename ImageType::SizeType largestRegion;
  largestRegion = image->GetLargestPossibleRegion().GetSize();

  unsigned int count = 0;
  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
    count = 2 * (it.GetIndex()[1] * largestRegion[0] + it.GetIndex()[0]);
    if ( !TestCompare(it.GetIndex(), it.Get(), ComplexType(count, count+1) ) )
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

int otbMonobandComplexToImageComplexFloat(int argc, char * argv[])
{
  return otbMonobandComplexToImageComplexGeneric<float>(argc, argv);
}

int otbMonobandComplexToImageComplexDouble(int argc, char * argv[])
{
  return otbMonobandComplexToImageComplexGeneric<double>(argc, argv);
}

int otbMonobandComplexToImageComplexInt(int argc, char * argv[])
{
  return otbMonobandComplexToImageComplexGeneric<int>(argc, argv);
}

int otbMonobandComplexToImageComplexShort(int argc, char * argv[])
{
  return otbMonobandComplexToImageComplexGeneric<short>(argc, argv);
}

/***********
 * 4.
 * Read Monoband Complex as VectorImage<double>
 * out : [0]=in.real(), [1]=in.imag()
 ***********/
template<class InternalType>
int otbMonobandComplexToVectorImageScalarGeneric(int itkNotUsed(argc), char * argv[])
{
  typedef InternalType                        RealType;

  typedef otb::VectorImage<RealType, 2>       ImageType;
  typedef typename ImageType::PixelType       PixelType;

  const unsigned int w = 10;
  const unsigned int h = 2;

  typename ImageType::Pointer image;
  itk::ImageRegionIteratorWithIndex<ImageType> it = ReadRegion(argv[1], image, w, h);

  typename ImageType::SizeType largestRegion;
  largestRegion = image->GetLargestPossibleRegion().GetSize();

  unsigned int count = 0;
  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
    count = 2 * (it.GetIndex()[1] * largestRegion[0] + it.GetIndex()[0]);

    PixelType expected(2);
    expected[0] = count;
    expected[1] = count + 1;

    if ( !TestCompare( it.GetIndex(), it.Get(), expected ) )
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

int otbMonobandComplexToVectorImageScalarFloat(int argc, char * argv[])
{
  return otbMonobandComplexToVectorImageScalarGeneric<float>(argc, argv);
}

int otbMonobandComplexToVectorImageScalarDouble(int argc, char * argv[])
{
  return otbMonobandComplexToVectorImageScalarGeneric<double>(argc, argv);
}

int otbMonobandComplexToVectorImageScalarInt(int argc, char * argv[])
{
  return otbMonobandComplexToVectorImageScalarGeneric<int>(argc, argv);
}

int otbMonobandComplexToVectorImageScalarShort(int argc, char * argv[])
{
  return otbMonobandComplexToVectorImageScalarGeneric<short>(argc, argv);
}

/***********
 * 5.
 * Read Monoband Complex as VectorImage<complex>
 * out : [0]=in
 ***********/
template<class InternalType>
int otbMonobandComplexToVectorImageComplexGeneric(int itkNotUsed(argc), char * argv[])
{
  typedef InternalType                        RealType;
  typedef std::complex<RealType>              ComplexType;

  typedef otb::VectorImage<ComplexType, 2>    ImageType;
  typedef typename ImageType::PixelType       PixelType;

  const unsigned int w = 10;
  const unsigned int h = 2;

  typename ImageType::Pointer image;
  itk::ImageRegionIteratorWithIndex<ImageType> it = ReadRegion(argv[1], image, w, h);

  typename ImageType::SizeType largestRegion;
  largestRegion = image->GetLargestPossibleRegion().GetSize();

  unsigned int count = 0;
  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
    count = 2 * (it.GetIndex()[1] * largestRegion[0] + it.GetIndex()[0]);

    PixelType expected(1);
    expected[0] = ComplexType(count, count + 1);

    if ( !TestCompare( it.GetIndex(), it.Get(), expected ) )
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

int otbMonobandComplexToVectorImageComplexFloat(int argc, char * argv[])
{
  return otbMonobandComplexToVectorImageComplexGeneric<float>(argc, argv);
}

int otbMonobandComplexToVectorImageComplexDouble(int argc, char * argv[])
{
  return otbMonobandComplexToVectorImageComplexGeneric<double>(argc, argv);
}

int otbMonobandComplexToVectorImageComplexInt(int argc, char * argv[])
{
  return otbMonobandComplexToVectorImageComplexGeneric<int>(argc, argv);
}

int otbMonobandComplexToVectorImageComplexShort(int argc, char * argv[])
{
  return otbMonobandComplexToVectorImageComplexGeneric<short>(argc, argv);
}

/***********
 * 6.
 * Read Multiband Scalar as Image<complex>
 * out : out.real=in[0], out.imag=in[1]
 ***********/
template<class InternalType>
int otbMultibandScalarToImageComplexGeneric(int itkNotUsed(argc), char * argv[])
{
  typedef InternalType                        RealType;
  typedef std::complex<RealType>              ComplexType;

  typedef otb::Image<ComplexType, 2>          ImageType;
  typedef typename ImageType::PixelType       PixelType;

  const unsigned int w = 10;
  const unsigned int h = 2;

  typename ImageType::Pointer image;
  itk::ImageRegionIteratorWithIndex<ImageType> it = ReadRegion(argv[1], image, w, h);

  typename ImageType::SizeType largestRegion;
  largestRegion = image->GetLargestPossibleRegion().GetSize();

  unsigned int count = 0;
  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
    count = it.GetIndex()[1] * largestRegion[0] + it.GetIndex()[0];

    PixelType expected(count, count + largestRegion[0] * largestRegion[1] );

    if ( !TestCompare( it.GetIndex(), it.Get(), expected ) )
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

int otbMultibandScalarToImageComplexFloat(int argc, char * argv[])
{
  return otbMultibandScalarToImageComplexGeneric<float>(argc, argv);
}

int otbMultibandScalarToImageComplexDouble(int argc, char * argv[])
{
  return otbMultibandScalarToImageComplexGeneric<double>(argc, argv);
}

int otbMultibandScalarToImageComplexInt(int argc, char * argv[])
{
  return otbMultibandScalarToImageComplexGeneric<int>(argc, argv);
}

int otbMultibandScalarToImageComplexShort(int argc, char * argv[])
{
  return otbMultibandScalarToImageComplexGeneric<short>(argc, argv);
}


/***********
 * 7.
 * Read Multiband Scalar as VectorImage<complex>
 * out : (out[0].real=in[0], out[0].imag=0), (out[1].real=in[1], out[1].imag=0), ...
 ***********/
template<class InternalType>
int otbMultibandScalarToVectorImageComplexGeneric(int itkNotUsed(argc), char * argv[])
{
  typedef InternalType                        RealType;
  typedef std::complex<RealType>              ComplexType;

  typedef otb::VectorImage<ComplexType, 2>    ImageType;
  typedef typename ImageType::PixelType       PixelType;

  const unsigned int w = 10;
  const unsigned int h = 2;

  typename ImageType::Pointer image;
  itk::ImageRegionIteratorWithIndex<ImageType> it = ReadRegion(argv[1], image, w, h);

  typename ImageType::SizeType largestRegion;
  largestRegion = image->GetLargestPossibleRegion().GetSize();

  unsigned int count = 0;
  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
    count = it.GetIndex()[1] * largestRegion[0] + it.GetIndex()[0];

    PixelType expected(2);
    expected[0] = ComplexType(count, 0);
    expected[1] = ComplexType(count + largestRegion[0] * largestRegion[1], 0);

    if ( !TestCompare( it.GetIndex(), it.Get(), expected ) )
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

int otbMultibandScalarToVectorImageComplexFloat(int argc, char * argv[])
{
  return otbMultibandScalarToVectorImageComplexGeneric<float>(argc, argv);
}

int otbMultibandScalarToVectorImageComplexDouble(int argc, char * argv[])
{
  return otbMultibandScalarToVectorImageComplexGeneric<double>(argc, argv);
}

int otbMultibandScalarToVectorImageComplexInt(int argc, char * argv[])
{
  return otbMultibandScalarToVectorImageComplexGeneric<int>(argc, argv);
}

int otbMultibandScalarToVectorImageComplexShort(int argc, char * argv[])
{
  return otbMultibandScalarToVectorImageComplexGeneric<short>(argc, argv);
}


/***********
 * 8.
 * Read Multiband Complex as VectorImage<scalar>
 * out : out[0]=in[0].real, out[1]=in[0].imag, out[2]=in[0].real, out[3]=in[0].imag, ...
 ***********/
template<class InternalType>
int otbMultibandComplexToVectorImageScalarGeneric(int itkNotUsed(argc), char * argv[])
{
  typedef InternalType                        RealType;
  typedef otb::VectorImage<RealType, 2>       ImageType;
  typedef typename ImageType::PixelType       PixelType;

  const unsigned int w = 10;
  const unsigned int h = 2;

  typename ImageType::Pointer image;
  itk::ImageRegionIteratorWithIndex<ImageType> it = ReadRegion(argv[1], image, w, h);

  typename ImageType::SizeType largestRegion;
  largestRegion = image->GetLargestPossibleRegion().GetSize();

  const unsigned int nbbands = image->GetNumberOfComponentsPerPixel();

  unsigned int count = 0;
  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
    count = 2 * (it.GetIndex()[1] * largestRegion[0] + it.GetIndex()[0]);

    PixelType expected(nbbands);
    for (unsigned int band = 0; band < nbbands; band += 2 )
      {
      expected[band]     = count +     band * largestRegion[0] * largestRegion[1];
      expected[band + 1] = count + 1 + band * largestRegion[0] * largestRegion[1];
      }

    if ( !TestCompare( it.GetIndex(), it.Get(), expected ) )
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

int otbMultibandComplexToVectorImageScalarFloat(int argc, char * argv[])
{
  return otbMultibandComplexToVectorImageScalarGeneric<float>(argc, argv);
}

int otbMultibandComplexToVectorImageScalarDouble(int argc, char * argv[])
{
  return otbMultibandComplexToVectorImageScalarGeneric<double>(argc, argv);
}

int otbMultibandComplexToVectorImageScalarInt(int argc, char * argv[])
{
  return otbMultibandComplexToVectorImageScalarGeneric<int>(argc, argv);
}

int otbMultibandComplexToVectorImageScalarShort(int argc, char * argv[])
{
  return otbMultibandComplexToVectorImageScalarGeneric<short>(argc, argv);
}

/***********
 * 9.
 * Read Multiband Complex as VectorImage<complex>
 * out : out[0]=in[0], out[1]=in[1], ...
 ***********/
template<class InternalType>
int otbMultibandComplexToVectorImageComplexGeneric(int itkNotUsed(argc), char * argv[])
{
  typedef InternalType                        RealType;
  typedef std::complex<RealType>              ComplexType;

  typedef otb::VectorImage<ComplexType, 2>    ImageType;
  typedef typename ImageType::PixelType       PixelType;

  const unsigned int w = 10;
  const unsigned int h = 2;

  typename ImageType::Pointer image;
  itk::ImageRegionIteratorWithIndex<ImageType> it = ReadRegion(argv[1], image, w, h);

  typename ImageType::SizeType largestRegion;
  largestRegion = image->GetLargestPossibleRegion().GetSize();

  const unsigned int nbbands = image->GetNumberOfComponentsPerPixel();

  unsigned int count = 0;
  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
    count = 2 * (it.GetIndex()[1] * largestRegion[0] + it.GetIndex()[0]);

    PixelType expected(nbbands);
    for (unsigned int band = 0; band < nbbands; ++band )
      {
      expected[band]     = ComplexType(count     + band * 2 * largestRegion[0] * largestRegion[1],
                                       count + 1 + band * 2 * largestRegion[0] * largestRegion[1]);
      }

    if ( !TestCompare( it.GetIndex(), it.Get(), expected ) )
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

int otbMultibandComplexToVectorImageComplexFloat(int argc, char * argv[])
{
  return otbMultibandComplexToVectorImageComplexGeneric<float>(argc, argv);
}

int otbMultibandComplexToVectorImageComplexDouble(int argc, char * argv[])
{
  return otbMultibandComplexToVectorImageComplexGeneric<double>(argc, argv);
}

int otbMultibandComplexToVectorImageComplexShort(int argc, char * argv[])
{
  return otbMultibandComplexToVectorImageComplexGeneric<short>(argc, argv);
}

int otbMultibandComplexToVectorImageComplexInt(int argc, char * argv[])
{
  return otbMultibandComplexToVectorImageComplexGeneric<int>(argc, argv);
}

/***********
 * 10.
 * Read Monoband Scalar as VectorImage<complex>
 * out : out[0].real=in, out[0].imag=0
 ***********/
template<class InternalType>
int otbMonobandScalarToVectorImageComplexGeneric(int itkNotUsed(argc), char * argv[])
{
  typedef InternalType                        RealType;
  typedef std::complex<RealType>              ComplexType;

  typedef otb::VectorImage<ComplexType, 2>    ImageType;
  typedef typename ImageType::PixelType       PixelType;

  const unsigned int w = 10;
  const unsigned int h = 2;

  typename ImageType::Pointer image;
  itk::ImageRegionIteratorWithIndex<ImageType> it = ReadRegion(argv[1], image, w, h);

  typename ImageType::SizeType largestRegion;
  largestRegion = image->GetLargestPossibleRegion().GetSize();

  unsigned int count = 0;
  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
    count = it.GetIndex()[1] * largestRegion[0] + it.GetIndex()[0];

    PixelType expected(1);
    expected[0] = ComplexType(count, 0);

    if ( !TestCompare( it.GetIndex(), it.Get(), expected ) )
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

int otbMonobandScalarToVectorImageComplexFloat(int argc, char * argv[])
{
  return otbMonobandScalarToVectorImageComplexGeneric<float>(argc, argv);
}

int otbMonobandScalarToVectorImageComplexDouble(int argc, char * argv[])
{
  return otbMonobandScalarToVectorImageComplexGeneric<double>(argc, argv);
}

int otbMonobandScalarToVectorImageComplexShort(int argc, char * argv[])
{
  return otbMonobandScalarToVectorImageComplexGeneric<short>(argc, argv);
}

int otbMonobandScalarToVectorImageComplexInt(int argc, char * argv[])
{
  return otbMonobandScalarToVectorImageComplexGeneric<int>(argc, argv);
}

/***********
 * 11.
 * Read MultibandComplex as Image<double>
 * out : ???
 ***********/
template<class InternalType>
int otbMultibandComplexToImageScalarGeneric(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  // This case is not handled yet.
  // We need more time to decide what we want to do with this case. ( perhaps return exception ???)
  std::cout << "This case is not handled yet." \
            << "Need specification : throw exception ?" \
            << std::endl;
  return EXIT_FAILURE;
}

int otbMultibandComplexToImageScalarFloat(int argc, char * argv[])
{
  return otbMultibandComplexToImageScalarGeneric<float>(argc, argv);
}

int otbMultibandComplexToImageScalarDouble(int argc, char * argv[])
{
  return otbMultibandComplexToImageScalarGeneric<double>(argc, argv);
}

int otbMultibandComplexToImageScalarShort(int argc, char * argv[])
{
  return otbMultibandComplexToImageScalarGeneric<short>(argc, argv);
}

int otbMultibandComplexToImageScalarInt(int argc, char * argv[])
{
  return otbMultibandComplexToImageScalarGeneric<int>(argc, argv);
}
