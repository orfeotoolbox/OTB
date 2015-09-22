/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Mines-Telecom. All rights reserved.
  See IMTCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbWaveletOperator_h
#define __otbWaveletOperator_h

#include "itkMacro.h"
#include "itkNeighborhoodOperator.h"

// This include is needed here to define Wavelet::Wavelet...
#include "otbWaveletGenerator.h"

#include "otbWaveletLowPassOperator.h"
#include "otbWaveletHighPassOperator.h"

namespace otb {

/**
 * \class WaveletOperator
 *
 * \brief A Generic NeighborhoodOperator wavelets filter set defined for templation
 *
 * This class is the general class to be used for template definition of any
 * Wavelet decomposition. It contains only typedef that give access to specific
 * LowPass and HighPass filters in a dyadic decomposition frame.
 *
 * Note: It is not an operator as it stands
 *
 * \sa WaveletOperatorBase
 * \sa WaveletLowPassOperator
 * \sa WaveletHighPassOperator
 *
 * \ingroup OTBWavelet
 */
template <Wavelet::Wavelet TMotherWaveletOperator,
    Wavelet::WaveletDirection TDirectionOfTransformation,
    class TPixel, unsigned int VDimension,
    class TAllocator = itk::NeighborhoodAllocator<TPixel> >
class ITK_EXPORT WaveletOperator
{
public:
  /** Standard typedefs */
  typedef WaveletOperator Self;

  /** Typedefs redirections */
  typedef WaveletLowPassOperator<
      TMotherWaveletOperator, TDirectionOfTransformation,
      TPixel, VDimension, TAllocator>
  LowPassOperator;
  typedef WaveletHighPassOperator<
      TMotherWaveletOperator, TDirectionOfTransformation,
      TPixel, VDimension, TAllocator>
  HighPassOperator;

private:
  /**
   * This class may not be instanciated
   * Intentionally not implemented
   */
  WaveletOperator ();
  WaveletOperator (const Self &);
  ~WaveletOperator();
};

} // end of namespace otb

#endif
