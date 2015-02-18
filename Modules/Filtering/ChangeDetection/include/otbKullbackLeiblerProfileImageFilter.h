/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are covered by the GET copyright.
  See GETCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbKullbackLeiblerProfileImageFilter_h
#define __otbKullbackLeiblerProfileImageFilter_h

#include <vector>

#include "itkArray.h"
#include "itkArray2D.h"
#include "itkVariableLengthVector.h"

#include "otbBinaryFunctorNeighborhoodVectorImageFilter.h"

namespace otb
{

/** \class CumulantsForEdgeworthProfile
  * \brief Helper class for KullbackLeiblerProfileImageFilter. Please refer to KullbackLeibleProfileImageFilter.
  *
 *
 * \ingroup OTBChangeDetection
  */
template <class TInput>
class CumulantsForEdgeworthProfile
{
public:
  typedef itk::Vector<double, 4>    CumulantType;
  typedef std::vector<CumulantType> CumulantSet;
  typedef CumulantSet::iterator     Iterator;

  CumulantsForEdgeworthProfile (const TInput& input, std::vector<itk::Array2D<int> >& mask);
  virtual ~CumulantsForEdgeworthProfile () {}

  // Kullback-Leibler Profile
  template <class TInput2>
  itk::VariableLengthVector<double> KL_profile(CumulantsForEdgeworthProfile<TInput2>& cumulants);
  // Kullback-Leibler divergence at a given scale
  double KL_profile(const CumulantType& cumulants1, const CumulantType& cumulants2);
  // Access to data
  inline CumulantType& GetCumulantAtScale(int i)
  {
    return this->fCum[i];
  }
  // Check data availability
  inline bool IsDataAvailable() const
  {
    return this->fDataAvailable;
  }

  // debug
  int m_debug;

protected:

  // Momentum Estimation from encapsulated neighborhood
  int  MakeSumAndMoments(const TInput& input, std::vector<itk::Array2D<int> >& mask);
  // momentum estimation from the smaller window
  int InitSumAndMoments(const TInput& input, itk::Array2D<int>& mask);
  //
  int ReInitSumAndMoments(const TInput& input, itk::Array2D<int>& mask, int level);
  // transformation moment -> cumulants (for Edgeworth)
  int MakeCumulants();

  // Internal variables
  double      fSum0, fSum1, fSum2, fSum3, fSum4;
  CumulantSet fMu;
  CumulantSet fCum;

  bool fDataAvailable;

private:
  CumulantsForEdgeworthProfile (); // Not implemented
  CumulantsForEdgeworthProfile (const TInput& input);    // Not implemented

};

namespace Functor
{
/** \class KullbackLeiblerProfile
 * \brief Functor for KullbackLeiblerProfileImageFilter. Please refer to KullbackLeiblerProfileImageFilter.
 *
 *
 * \ingroup OTBChangeDetection
 */
template<class TInput1, class TInput2, class TOutput>
class KullbackLeiblerProfile
{
public:
  KullbackLeiblerProfile ();
  virtual ~KullbackLeiblerProfile () {}
  // Gives the radius min and max of neighborhood
  void SetRadius(const unsigned char& min, const unsigned char& max);
  unsigned char GetRadiusMin(void);
  unsigned char GetRadiusMax(void);
  // Gives the size of the profile
  int  GetNumberOfComponentsPerPixel() const
  {
    return m_mask.size();
  }
  // functor
  TOutput operator ()(const TInput1& it1, const TInput2& it2);
protected:
  // Make the set of masks to play the increase in window size
  void MakeMultiscaleProfile();
  // Internal attributes
  unsigned char                   m_RadiusMin;
  unsigned char                   m_RadiusMax;
  std::vector<itk::Array2D<int> > m_mask;
};
} // Functor

/** \class KullbackLeiblerProfileImageFilter
 * \brief Implements neighborhood-wise the computation of KullbackLeibler profile over Edgeworth approximation.
 *
 * This filter is parametrized over the types of the two
 * input images and the type of the output image.
 *
 * Numeric conversions (castings) are done by the C++ defaults.
 *
 * The filter will walk over all the pixels in the two input images, and for
 * each one of them it will do the following:
 *
 * - cast the input 1 pixel value to \c double
 * - cast the input 2 pixel value to \c double
 * - compute the first four cumulants of the two pixel values
 * - compute the value of the Edgeorth approximation of the KL distance
 * - cast the \c double value resulting to the pixel type of the output image
 * - store the casted value into the output image.
 *
 * The filter expect all images to have the same dimension (all 2D)
 *
 * See article of  Lin Saito et Levine
 * "Edgeworth Approximation of the Kullback-Leibler Distance Towards Problems in Image Analysis"
 * and
 * "Edgeworth Expansions of the Kullback-Leibler Information" (submitted to JASA, nov 25, 1999)
 * http://www.math.ucdavis.edu/~saito/publications
 *
 *  TOutput is expected to be a itk::VariableLengthVector< TPixel > and comes from an otbVectorImage< TPixel, 2 >
 *
 * \ingroup IntensityImageFilters Multithreaded
 *
 * \ingroup OTBChangeDetection
 */
template <class TInputImage1, class TInputImage2, class TOutputImage>
class ITK_EXPORT KullbackLeiblerProfileImageFilter :
  public BinaryFunctorNeighborhoodVectorImageFilter<
      TInputImage1, TInputImage2, TOutputImage,
      Functor::KullbackLeiblerProfile<
          typename itk::ConstNeighborhoodIterator<TInputImage1>,
          typename itk::ConstNeighborhoodIterator<TInputImage2>,
          typename TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef KullbackLeiblerProfileImageFilter Self;
  typedef /*typename*/ BinaryFunctorNeighborhoodVectorImageFilter<
      TInputImage1, TInputImage2, TOutputImage,
      Functor::KullbackLeiblerProfile<
          typename itk::ConstNeighborhoodIterator<TInputImage1>,
          typename itk::ConstNeighborhoodIterator<TInputImage2>,
          typename TOutputImage::PixelType>
      >  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

protected:
  KullbackLeiblerProfileImageFilter() {}
  virtual ~KullbackLeiblerProfileImageFilter() {}

private:
  KullbackLeiblerProfileImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbKullbackLeiblerProfileImageFilter.txx"
#endif

#endif
