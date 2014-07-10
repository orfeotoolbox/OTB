/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbWaveletPacketDecompositionCosts_h
#define __otbWaveletPacketDecompositionCosts_h

#include "itkObject.h"
#include "itkMacro.h"

namespace otb {

/** \class FullyDecomposedWaveletPacketCost
 * \brief Cost evaluation to be used into the Wavelet Packet decomposition class.
 *
 * This class implements the criteria to perform fully decomposed wavelet packet.
 * It is based on the depth of the decomposition only...
 *
 * \sa WaveletPacketForwardTransform
 */
template <class TImage>
class ITK_EXPORT FullyDecomposedWaveletPacketCost
  : public itk::Object
{
public:
  /** Standard typedefs */
  typedef FullyDecomposedWaveletPacketCost Self;
  typedef itk::Object                      Superclass;
  typedef itk::SmartPointer<Self>          Pointer;
  typedef itk::SmartPointer<const Self>    ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(FullyDecomposedWaveletPacketCost, Object);

  typedef TImage ImageType;

  /** Acces to the data */
  static unsigned int NumberOfAllowedDecompositions;

  /** Evaluate the cost */
  bool Evaluate(unsigned int decomposition, const ImageType * itkNotUsed(image))
  {
    return (decomposition < NumberOfAllowedDecompositions);
  }

protected:
  FullyDecomposedWaveletPacketCost () {}
  virtual ~FullyDecomposedWaveletPacketCost() {}

private:
  FullyDecomposedWaveletPacketCost (const Self &);   // not implemented
  void operator =(const Self&);

}; // end of class

template <class TImage>
unsigned int FullyDecomposedWaveletPacketCost<TImage>::NumberOfAllowedDecompositions = 1;

} // end of namespace otb

#endif
