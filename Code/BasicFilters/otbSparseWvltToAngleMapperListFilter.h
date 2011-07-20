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
#ifndef __otbSparseWvltToAngleMapperListFilter_h
#define __otbSparseWvltToAngleMapperListFilter_h

#include "otbBinaryFunctorImageListToSampleListFilter.h"

#include "itkNumericTraits.h"

#include "otbMath.h"

namespace otb {

namespace Functor {

  /** \class SparseWvltToAngleMapperFunctor
   * \brief Functor for SparseWvltToAngleMapperListFilter
   */
  template < class TInput, class TOutput >
  class SparseWvltToAngleMapperFunctor
  {
    public:
      SparseWvltToAngleMapperFunctor ()
      {
        m_Lowerthreshold = itk::NumericTraits< TOutput >::Zero;
      }
      virtual ~SparseWvltToAngleMapperFunctor() { }

      void SetLowerThreshold ( TInput value )
      {
        m_Lowerthreshold = value;
      }

      TInput GetLowerThreshold() const
      {
        return m_Lowerthreshold;
      }

      bool IsToGenerate ( const TInput & x1, const TInput & x2 ) const
      {
        if ( vcl_abs(x1) <= m_Lowerthreshold )
          return false;
        if ( vcl_abs(x2) <= m_Lowerthreshold )
          return false;
        return true;
      }

      TOutput operator() ( const TInput & x1, const TInput & x2 ) const
      {
        return vcl_atan2( x2, x1 );
      }

    protected:
      TInput m_Lowerthreshold;
  }; // end of functor class
} // end of namespace Functor

/** \class SparseWvltToAngleMapperListFilter
 * \brief This class select couple of join-wvlt coeff for sparse unmixing
 *
 * This class performs a kind of scatter plot of wavelet coeff of 2 images and yields
 * the angle values (through atan(y/x)) of the most significant coeff (up to a threashold).
 *
 * This class implements the BinaryFunctorImageListToSampleListFilter with specific functor...
 */
template < class TInputImageList, class TOutputSampleList >
class SparseWvltToAngleMapperListFilter
  : public ITK_EXPORT BinaryFunctorImageListToSampleListFilter <
                        TInputImageList, TOutputSampleList,
                        Functor::SparseWvltToAngleMapperFunctor<
                          typename TInputImageList::ImageType::PixelType,
                          typename TOutputSampleList::MeasurementVectorType::ValueType > >
{
public:
  /** Standard typedefs. */
  typedef SparseWvltToAngleMapperListFilter Self;
  typedef BinaryFunctorImageListToSampleListFilter <
            TInputImageList, TOutputSampleList,
            Functor::SparseWvltToAngleMapperFunctor<
                typename TInputImageList::ImageType::PixelType,
              typename TOutputSampleList::MeasurementVectorType::ValueType > >
          Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SparseWvltToAngleMapperListFilter, BinaryFunctorImageListToSampleListFilter);

protected:
  SparseWvltToAngleMapperListFilter() { }
  virtual ~SparseWvltToAngleMapperListFilter() { }

private:
  SparseWvltToAngleMapperListFilter(const Self &); // not implemented
  void operator=(const Self &);
}; // end of class
                        
} // end of namespace otb

#endif

