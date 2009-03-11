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

#ifndef __otbWPCost_h
#define __otbWPCost_h

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
template < class TImage >
class ITK_EXPORT FullyDecomposedWaveletPacketCost
  : public itk::Object
{
public:
  /** Standard typedefs */
  typedef FullyDecomposedWaveletPacketCost Self;
  typedef itk::Object Superclass;
  typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Type macro */
        itkNewMacro(Self);

        /** Creation through object factory macro */
        itkTypeMacro(FullyDecomposedWaveletPacketCost,Object);

  typedef TImage ImageType;

  /** Acces to the data */
  itkGetMacro(NumberOfAllowedDecompositions,unsigned int);
  itkSetMacro(NumberOfAllowedDecompositions,unsigned int);

  /** Evaluate the cost */
  virtual bool Evaluate ( unsigned int decomposition, ImageType * image )
  {
    return ( decomposition < m_NumberOfAllowedDecompositions );
  }

protected:
  FullyDecomposedWaveletPacketCost ()
  {
    m_NumberOfAllowedDecompositions = 1;
  }
  virtual ~FullyDecomposedWaveletPacketCost() {}

  unsigned int m_NumberOfAllowedDecompositions;
private:
  FullyDecomposedWaveletPacketCost ( const Self & ); // not implemented
  void operator= ( const Self & );

}; // end of class 

} // end of namespace otb

#endif


