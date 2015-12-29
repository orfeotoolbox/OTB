/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom; Telecom bretagne. All rights reserved.
  See IMTCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbSOMWithMissingValue_h
#define __otbSOMWithMissingValue_h

#include "otbPeriodicSOM.h"

namespace otb
{
/**
 *  \class SOMWithMissingValue
 *  \brief This class implements SOM training with missing value
 *
 *  The distance between vectors that may have missing components is holded
 *  by the EuclideanDistanceMetricWithMissingValue class in the SOMMap distance
 *  template. Nevertheless, this class re-implements the UpdateMap method to
 *  adapt the evaluation of each component of the 'newNeuron' when dealing
 *  with missing values.
 *
 *  TMap has to be templeted with EuclideanDistanceMetricWithMissingValuePow2
 *
 *  \sa SOMMap
 *  \sa PeriodicSOM
 *  \sa EuclideanDistanceMetricWithMissingValue
 *
 * \ingroup OTBSOM
 */
template <class TListSample, class TMap,
    class TSOMLearningBehaviorFunctor = Functor::CzihoSOMLearningBehaviorFunctor,
    class TSOMNeighborhoodBehaviorFunctor = Functor::CzihoSOMNeighborhoodBehaviorFunctor>
class ITK_EXPORT SOMWithMissingValue
  : public PeriodicSOM<TListSample, TMap, TSOMLearningBehaviorFunctor, TSOMNeighborhoodBehaviorFunctor>
{
public:
  /** Standard typedefs */
  typedef SOMWithMissingValue Self;
  typedef PeriodicSOM<TListSample, TMap,
      TSOMLearningBehaviorFunctor,
      TSOMNeighborhoodBehaviorFunctor> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef TListSample                      ListSampleType;
  typedef typename ListSampleType::Pointer ListSamplePointerType;

  typedef TMap                                  MapType;
  typedef typename MapType::PixelType           NeuronType;
  typedef typename NeuronType::ValueType        ValueType;
  typedef typename MapType::IndexType           IndexType;
  typedef typename MapType::SizeType            SizeType;
  typedef typename MapType::RegionType          RegionType;
  typedef typename MapType::Pointer             MapPointerType;
  typedef typename MapType::DistanceType        DistanceType;
  typedef typename MapType::DistancePointerType DistancePointerType;

  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Runtime information macro */
  itkTypeMacro(SOMWithMissingValue, PeriodicSOM);

protected:
  /** Constructor */
  SOMWithMissingValue ();
  /** Destructor */
  virtual ~SOMWithMissingValue();
  /** Output information redefinition */
  virtual void GenerateOutputInformation()
  {
    Superclass::GenerateOutputInformation ();
  }
  /** Output allocation redefinition */
  virtual void AllocateOutputs()
  {
    Superclass::AllocateOutputs();
  }
  /** Main computation method */
  virtual void GenerateData(void)
  {
    Superclass::GenerateData();
  }
  /**
   * Update the output map with a new sample, depending on the availability of the data
   */
  virtual void UpdateMap(const NeuronType& sample, double beta, SizeType& radius);

  /** Step one iteration. */
  virtual void Step(unsigned int currentIteration)
  {
    Superclass::Step(currentIteration);
  }
  /** PrintSelf method */
void PrintSelf(std::ostream& os, itk::Indent indent) const;


private:
  SOMWithMissingValue (const Self &);    // purposely not implemented
  void operator =(const Self&);  // purposely not implemented

}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSOMWithMissingValue.txx"
#endif

#endif
