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
#ifndef otbSamplingRateCalculatorList_h
#define otbSamplingRateCalculatorList_h

#include "otbSamplingRateCalculator.h"
#include "otbObjectList.h"

namespace otb
{
/** \class SamplingRateCalculatorList
 *  \brief This class is a list of sampling rate calculators.
 *
 * It computes sampling rates for each image and each class, given input
 * statistics on available samples, and a sampling strategy. The sampling
 * strategies are made from the combination of :
 *   - the sampling strategies of otb::SamplingRateCalculator  (mono-image)
 *   - a multi-image behaviour : proportional / constant / custom
 *
 * \ingroup OTBSampling
 */
class ITK_EXPORT SamplingRateCalculatorList
  : public ObjectList<SamplingRateCalculator>
{
public:
  /** Standard typedefs */
  typedef SamplingRateCalculatorList          Self;
  typedef ObjectList<SamplingRateCalculator>  Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  typedef SamplingRateCalculator::ClassCountMapType  ClassCountMapType;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SamplingRateCalculatorList, ObjectList);

protected:
  /** Constructor */
  SamplingRateCalculatorList();

  /** Destructor */
  ~SamplingRateCalculatorList() ITK_OVERRIDE {}

private:
  SamplingRateCalculator(const Self &);    //purposely not implemented
  void operator =(const Self&);    //purposely not implemented

  void UpdateGlobalCounts();

  ClassCountMapType m_GlobalCountMap;

} // end namespace otb

#endif
