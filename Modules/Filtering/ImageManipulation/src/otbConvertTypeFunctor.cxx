#include "otbConvertTypeFunctor.h"
namespace otb
{
namespace Functor
{
template void FillIn<unsigned char>(unsigned int i, unsigned char const & pix, std::vector < double > & vPix);
template void FillIn<short>(unsigned int i, short const & pix, std::vector < double > & vPix);
template void FillIn<unsigned short>(unsigned int i, unsigned short const & pix, std::vector < double > & vPix);
template void FillIn<int>(unsigned int i, int const & pix, std::vector < double > & vPix);
template void FillIn<unsigned int>(unsigned int i, unsigned int const & pix, std::vector < double > & vPix);
template void FillIn<float>(unsigned int i, float const & pix, std::vector < double > & vPix);
template void FillIn<double>(unsigned int i, double const & pix, std::vector < double > & vPix);

template void FillIn<itk::VariableLengthVector<unsigned char>>(unsigned int i, itk::VariableLengthVector<unsigned char> const & pix, std::vector < double > & vPix);
template void FillIn<itk::VariableLengthVector<short>>(unsigned int i, itk::VariableLengthVector<short> const & pix, std::vector < double > & vPix);
template void FillIn<itk::VariableLengthVector<unsigned short>>(unsigned int i, itk::VariableLengthVector<unsigned short> const & pix, std::vector < double > & vPix);
template void FillIn<itk::VariableLengthVector<int>>(unsigned int i, itk::VariableLengthVector<int> const & pix, std::vector < double > & vPix);
template void FillIn<itk::VariableLengthVector<unsigned int>>(unsigned int i, itk::VariableLengthVector<unsigned int> const & pix, std::vector < double > & vPix);
template void FillIn<itk::VariableLengthVector<float>>(unsigned int i, itk::VariableLengthVector<float> const & pix, std::vector < double > & vPix);
template void FillIn<itk::VariableLengthVector<double>>(unsigned int i, itk::VariableLengthVector<double> const & pix, std::vector < double > & vPix);

template void FillIn<std::complex<short> >(unsigned int i, std::complex<short> const & pix, std::vector < double > & vPix);
template void FillIn<std::complex<int> >(unsigned int i, std::complex<int> const & pix, std::vector < double > & vPix);
template void FillIn<std::complex<float> >(unsigned int i, std::complex<float> const & pix, std::vector < double > & vPix);
template void FillIn<std::complex<double> >(unsigned int i, std::complex<double> const & pix, std::vector < double > & vPix);

template void FillOut<unsigned char>(unsigned int i, unsigned char & pix, std::vector < double > & vPix);
template void FillOut<short>(unsigned int i, short & pix, std::vector < double > & vPix);
template void FillOut<unsigned short>(unsigned int i, unsigned short & pix, std::vector < double > & vPix);
template void FillOut<int>(unsigned int i, int & pix, std::vector < double > & vPix);
template void FillOut<unsigned int>(unsigned int i, unsigned int & pix, std::vector < double > & vPix);
template void FillOut<float>(unsigned int i, float & pix, std::vector < double > & vPix);
template void FillOut<double>(unsigned int i, double & pix, std::vector < double > & vPix);

template void FillOut<itk::VariableLengthVector<unsigned char> >(unsigned int i, itk::VariableLengthVector<unsigned char> & pix, std::vector < double > & vPix);
template void FillOut<itk::VariableLengthVector<short>>(unsigned int i, itk::VariableLengthVector<short> & pix, std::vector < double > & vPix);
template void FillOut<itk::VariableLengthVector<unsigned short>>(unsigned int i, itk::VariableLengthVector<unsigned short> & pix, std::vector < double > & vPix);
template void FillOut<itk::VariableLengthVector<int>>(unsigned int i, itk::VariableLengthVector<int> & pix, std::vector < double > & vPix);
template void FillOut<itk::VariableLengthVector<unsigned int>>(unsigned int i, itk::VariableLengthVector<unsigned int> & pix, std::vector < double > & vPix);
template void FillOut<itk::VariableLengthVector<float>>(unsigned int i, itk::VariableLengthVector<float> & pix, std::vector < double > & vPix);
template void FillOut<itk::VariableLengthVector<double>>(unsigned int i, itk::VariableLengthVector<double> & pix, std::vector < double > & vPix);

template void FillOut<std::complex<short> >(unsigned int i, std::complex<short> & pix, std::vector < double > & vPix);
template void FillOut<std::complex<int> >(unsigned int i, std::complex<int> & pix, std::vector < double > & vPix);
template void FillOut<std::complex<float> >(unsigned int i, std::complex<float> & pix, std::vector < double > & vPix);
template void FillOut<std::complex<double> >(unsigned int i, std::complex<double> & pix, std::vector < double > & vPix);
}
}
