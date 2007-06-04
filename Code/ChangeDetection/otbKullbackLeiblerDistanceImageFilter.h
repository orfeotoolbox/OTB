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
#ifndef __otbKullbackLeiblerDistanceImageFilter_h
#define __otbKullbackLeiblerDistanceImageFilter_h

#include "itkVariableLengthVector.h"
#include "otbBinaryFunctorNeighborhoodImageFilter.h"

namespace otb {

template <class TInput>
class CumulantsForEdgeworth 
{
	public :
		CumulantsForEdgeworth ( const TInput & input ); 
		virtual ~CumulantsForEdgeworth () { };

		// Divergence de KL 
		template <class TInput2>
		double KL_divergence ( const CumulantsForEdgeworth<TInput2> & cumulants );
		
		// access to attributes
		inline	double	GetMean ()	const	{ return this->fMean; }
		inline	double	GetVariance	()	const	{ return this->fVariance; }
		inline	double	GetSkewness	()	const	{ return this->fSkewness; }
		inline	double	GetKurtosis	()	const	{ return this->fKurtosis; }
	
	protected :

		// estimation des moments à partir du voisinage initial
		int	MakeSumAndMoments	( const TInput & input );
		// transformation moment -> cumulants (pour Edgeworth)
		int MakeCumulants(); 

		// Attributs internes à la classe
		double	fSum0, fSum1, fSum2, fSum3, fSum4;
		double	fMu1, fMu2, fMu3, fMu4;
		double	fMean, fVariance, fSkewness, fKurtosis;
};



namespace Functor {  

	template < class TInput1, class TInput2, class TOutput >
	class KullbackLeiblerDistance
	{
		public :
			KullbackLeiblerDistance () { }
			virtual ~KullbackLeiblerDistance () { }
			TOutput operator () ( const TInput1 & it1, const TInput2 & it2 ) {
				CumulantsForEdgeworth<TInput1> cum1 ( it1 );
				CumulantsForEdgeworth<TInput2> cum2 ( it2 );
				return static_cast<TOutput> ( cum1.KL_divergence( cum2 ) 
												+ cum2.KL_divergence( cum1 ) );
			}
	};

} // Functor

/** \class KullbackLeiblerDistanceImageFilter
 * \brief Implements neighborhood-wise the computation of KullbackLeibler distance over Edgeworth approximation.
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
 * The filter expect all images to have the same dimension 
 * (e.g. all 2D, or all 3D, or all ND)
 *	
 * See article of  Lin Saito et Levine 
 * "Edgeworth Approximation of the Kullback-Leibler Distance Towards Problems in Image Analysis"
 * and
 * "Edgeworth Expansions of the Kullback-Leibler Information" (submitted to JASA, nov 25, 1999)
 * http://www.math.ucdavis.edu/~saito/publications
 *
 * \ingroup IntensityImageFilters Multithreaded
 */
template <class TInputImage1, class TInputImage2, class TOutputImage>
class ITK_EXPORT KullbackLeiblerDistanceImageFilter :
	public BinaryFunctorNeighborhoodImageFilter<
			TInputImage1,TInputImage2,TOutputImage,
			Functor::KullbackLeiblerDistance<
				typename itk::ConstNeighborhoodIterator<TInputImage1>, 
				typename itk::ConstNeighborhoodIterator<TInputImage2>,
				typename TOutputImage::PixelType> >
{
	public:
		/** Standard class typedefs. */
		typedef KullbackLeiblerDistanceImageFilter Self;
		typedef BinaryFunctorNeighborhoodImageFilter<
								TInputImage1,TInputImage2,TOutputImage, 
								Functor::KullbackLeiblerDistance< 
									typename itk::ConstNeighborhoodIterator<TInputImage1>,
									typename itk::ConstNeighborhoodIterator<TInputImage2>, 
									typename TOutputImage::PixelType>   
								>  Superclass;
		typedef itk::SmartPointer<Self> Pointer;
		typedef itk::SmartPointer<const Self> ConstPointer;

		/** Method for creation through the object factory. */
		itkNewMacro(Self);

	protected:
		KullbackLeiblerDistanceImageFilter() {}
		virtual ~KullbackLeiblerDistanceImageFilter() {}

	private:
		KullbackLeiblerDistanceImageFilter(const Self&); //purposely not implemented
		void operator=(const Self&); //purposely not implemented

};

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbKullbackLeiblerDistanceImageFilter.txx"
#endif

} // namespace otb

#endif


