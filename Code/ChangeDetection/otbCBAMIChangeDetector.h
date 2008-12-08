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
#ifndef __otbCBAMIChangeDetector_h
#define __otbCBAMIChangeDetector_h


#include "otbBinaryFunctorNeighborhoodImageFilter.h"
#include <vector>
#include <stdlib.h>
#include <math.h>


namespace otb
{

/** \class CBAMIChangeDetector
 * \brief Implements neighborhood-wise the computation of the
 * cumulant-based approximation to mutual information.
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
 * - compute the difference of the two pixel values
 * - compute the value of the CBAMI
 * - cast the \c double value resulting to the pixel type of the output image
 * - store the casted value into the output image.
 *
 * The filter expect all images to have the same dimension
 * (e.g. all 2D, or all 3D, or all ND)
 *
 * \ingroup IntensityImageFilters Multithreaded
 */

#define epsilon 0.01

namespace Functor {

template< class TInput1, class TInput2, class TOutput>
class CBAMI
{
public:

  typedef typename std::vector<TOutput> VectorType;
  typedef typename VectorType::iterator IteratorType;
  typedef typename std::vector<VectorType> VectorOfVectorType;
  typedef typename VectorOfVectorType::iterator VecOfVecIteratorType;

  CBAMI() {};
  ~CBAMI() {};
  inline TOutput operator()( const TInput1 & itA,
                             const TInput2 & itB)
  {

    VectorType vecA;
    VectorType vecB;

    for(unsigned long pos = 0; pos< itA.Size(); ++pos)
      {

      vecA.push_back(static_cast<TOutput>(itA.GetPixel(pos)));
      vecB.push_back(static_cast<TOutput>(itB.GetPixel(pos)));

      }

    normalizeInPlace(vecA);
    normalizeInPlace(vecB);

    return static_cast<TOutput>( - vcl_log(PhiMI(vecA, vecB)+epsilon) );
  }

protected:

  inline void normalizeInPlace(VectorType vx)
  {

    TOutput Ex = 0.0;

    IteratorType itx;

    for( itx = vx.begin(); itx < vx.end(); itx++)
      {
      Ex  += (*itx);
      }

    Ex /= (vx.size());

    TOutput Vx = 0.0;

    for( itx = vx.begin(); itx < vx.end(); itx++)
      {
				Vx  += vcl_pow((*itx)-Ex,2);
      }

    Vx /= (vx.size());

    for( itx = vx.begin(); itx < vx.end(); itx++)
      {
      (*itx) = ((*itx)-Ex)/vcl_sqrt(Vx);
      }


  }
  inline TOutput Exyc(VectorType vx, VectorType vy){

    TOutput Exy = 0.0;
    TOutput Ex = 0.0;
    TOutput Ey = 0.0;

    IteratorType itx;
    IteratorType ity;

    for( itx = vx.begin(), ity = vy.begin(); itx < vx.end(); itx++, ity++)
      {
      //Ex  += (*itx);
      //Ey  += (*ity);
      Exy  += (*itx)*(*ity);

      }

    //Ex /= (vx.size());
    //Ey /= (vy.size());
    Exy /= (vx.size());

    return Exy-Ex*Ey;
  }

  inline TOutput Exyztc(VectorType vx, VectorType vy, VectorType vz, VectorType vt){

    TOutput Exyzt = 0.0;

    TOutput Exyz = 0.0;
    TOutput Exyt = 0.0;
    TOutput Exzt = 0.0;
    TOutput Eyzt = 0.0;

    TOutput Exy = 0.0;
    TOutput Exz = 0.0;
    TOutput Ext = 0.0;
    TOutput Eyz = 0.0;
    TOutput Eyt = 0.0;
    TOutput Ezt = 0.0;

    TOutput Ex = 0.0;
    TOutput Ey = 0.0;
    TOutput Ez = 0.0;
    TOutput Et = 0.0;


    IteratorType itx;
    IteratorType ity;
    IteratorType itz;
    IteratorType itt;

    for( itx = vx.begin(),
	   ity = vy.begin(),
	   itz = vz.begin(),
	   itt = vt.begin();
	 itx < vx.end();
	 itx++,
	   ity++,
	   itz++,
	   itt++)
      {
      //Ex  += (*itx);
      //Ey  += (*ity);
      //Ez  += (*itz);
      //Et  += (*itt);

      Exy += (*itx)*(*ity);
      Exz += (*itx)*(*itz);
      Ext += (*itx)*(*itt);
      Eyz += (*ity)*(*itz);
      Eyt += (*ity)*(*itt);
      Ezt += (*itz)*(*itt);

      Exyz += (*itx)*(*ity)*(*itz);
      Exyt += (*itx)*(*ity)*(*itt);
      Exzt += (*itx)*(*itz)*(*itt);
      Eyzt += (*ity)*(*itz)*(*itt);

      Exyzt += (*itx)*(*ity)*(*itz)*(*itt);

      }

    /*Ex  /= (vx.size());
    Ey  /= (vx.size());
    Ez  /= (vx.size());
    Et  /= (vx.size()); */

    Exy /= (vx.size());
    Exz /= (vx.size());
    Ext /= (vx.size());
    Eyz /= (vx.size());
    Eyt /= (vx.size());
    Ezt /= (vx.size());

    Exyz /= (vx.size());
    Exyt /= (vx.size());
    Exzt /= (vx.size());
    Eyzt /= (vx.size());


    TOutput result = Exyzt - Exyz*Et- Exyt*Ez- Exzt*Ey- Eyzt*Ex +
      Exy*Ez*Et + Exz*Et*Ey + Ext*Ey*Ez + Eyz*Et*Ex + Eyt*Ex*Ez + Ezt*Ex*Ey -
      3*Ex*Ey*Ez*Et;

    return result;
  }

  inline TOutput Rxy(VectorType va, VectorType vb)
  {

    return Exyc(va, vb);

  }

  inline TOutput Qxijkl(VectorType va, VectorType vb, VectorType vc, VectorType vd)
  {
//    IteratorType ita;
//    IteratorType itb;
//    IteratorType itc;
//    IteratorType itd;


    TOutput Eabcd_c = Exyztc(va, vb, vc, vd);


    TOutput Eab_c = Exyc(va, vb);
    TOutput Eac_c = Exyc(va, vc);
    TOutput Ead_c = Exyc(va, vd);
    TOutput Ecd_c = Exyc(vc, vd);
    TOutput Ebd_c = Exyc(vb, vd);
    TOutput Ebc_c = Exyc(vb, vc);

    return Eabcd_c - Eab_c*Ecd_c - Eac_c*Ebd_c - Ead_c*Ebc_c;




  }

  inline TOutput PhiMI(VectorType v1, VectorType v2)
  {


    VectorOfVectorType donnees;
    donnees.push_back(v1);
    donnees.push_back(v2);

    VecOfVecIteratorType iti;
    VecOfVecIteratorType itj;
    VecOfVecIteratorType itk;
    VecOfVecIteratorType itl;

    TOutput termeR = 0.0;
    TOutput termeQ = 0.0;

    for( iti = donnees.begin(); iti < donnees.end(); iti++ )
      for( itj = donnees.begin(); itj < donnees.end(); itj++ )
	{
	if(iti != itj)
	  termeR += vcl_pow(Rxy((*iti),(*itj)),2);

	for( itk = donnees.begin(); itk < donnees.end(); itk++ )
	  for( itl = donnees.begin(); itl < donnees.end(); itl++ )
	    {
	    if((iti != itj) || (iti != itk) || (iti != itl))
	      termeQ += vcl_pow( Qxijkl((*iti),(*itj),(*itk),(*itl)),2);
	    }
	}


    return 1.0/4.0*termeR + 1.0/48.0*termeQ;

  }

};
}

template <class TInputImage1, class TInputImage2, class TOutputImage>
class ITK_EXPORT CBAMIChangeDetector :
    public BinaryFunctorNeighborhoodImageFilter<
            TInputImage1,TInputImage2,TOutputImage,
            Functor::CBAMI<
                   ITK_TYPENAME itk::ConstNeighborhoodIterator<TInputImage1>,
                   ITK_TYPENAME itk::ConstNeighborhoodIterator<TInputImage2>,
		   ITK_TYPENAME TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef CBAMIChangeDetector  Self;
  typedef BinaryFunctorNeighborhoodImageFilter<
      TInputImage1,TInputImage2,TOutputImage,
          Functor::CBAMI<
               ITK_TYPENAME itk::ConstNeighborhoodIterator<TInputImage1>,
               ITK_TYPENAME itk::ConstNeighborhoodIterator<TInputImage2>,
               ITK_TYPENAME TOutputImage::PixelType>
  >  Superclass;
  typedef itk::SmartPointer<Self>   Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

protected:
  CBAMIChangeDetector() {}
  virtual ~CBAMIChangeDetector() {}

private:
  CBAMIChangeDetector(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace otb


#endif
