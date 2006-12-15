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
#ifndef _otbSOMMap_h
#define _otbSOMMap_h

#include "itkFixedArray.h"
#include "itkEuclideanDistance.h"
#include "otbImage.h"

namespace otb
{
/**
 * \class SOMMap
 * \brief
 */
template <class TNeuron=itk::FixedArray<float,3>, 
          class TDistance=itk::Statistics::EuclideanDistance<TNeuron>,unsigned int VMapDimension=2>  
class ITK_EXPORT SOMMap  
: public Image<TNeuron,VMapDimension>
{
  public:
  /** Standard typedefs */
  typedef SOMMap  Self;
  typedef Image<TNeuron,VMapDimension>  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  
  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Runtime informations macro */
  itkTypeMacro(SOMMap,Image);
  
  /** Template parameters related typedefs */
  typedef TNeuron NeuronType;
  typedef TDistance DistanceType;
  typedef typename DistanceType::Pointer DistancePointerType;
  
  /** Superclass related typedefs */
  typedef typename Superclass::IndexType IndexType;
  typedef typename Superclass::SizeType SizeType;
  typedef typename Superclass::DirectionType DirectionType;
  typedef typename Superclass::RegionType RegionType;
  typedef typename Superclass::SpacingType SpacingType;
  typedef typename Superclass::PointType PointType;
  /**
   * Get The index of the winning neuron for a sample.
   * \param the sample.
   * \return The index of the winning neuron.
   */
  IndexType GetWinner(const NeuronType& sample);
  
  protected:
  /** Constructor */
  SOMMap();
  /** Destructor */
  virtual ~SOMMap();
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  
  private:
  SOMMap(const Self&); // purposely not implemented 
  void operator=(const Self&); // purposely not implemented 
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSOMMap.txx"
#endif

#endif
