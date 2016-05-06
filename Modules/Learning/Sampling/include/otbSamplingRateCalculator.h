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
#ifndef __otbSamplingRateCalculator_h
#define __otbSamplingRateCalculator_h

#include "itkImageRegion.h"
#include "itkVectorContainer.h"
#include <set>

namespace otb
{
/** \class SamplingRateCalculator
 *  \brief This class represent a SamplingRateCalculator.
 *
 * \ingroup OTBSampling
 */

class SamplingRateCalculator
  : public itk::Object
{
public:
  /** Standard typedefs */
  typedef SamplingRateCalculator                     Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  
  
  /** typdefs **/
  typedef std::map<std::string, unsigned long>  ClassCountMapType;
  typedef ClassCountMapType::const_iterator constItMapType;
  typedef struct Triplet
   {
     unsigned long Required;
     unsigned long Tot;
     double Rate;
     bool operator==(const struct Triplet  & triplet) const;
   } TripletType;

  typedef std::map<std::string, TripletType > MapRateType;
  
  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SamplingRateCalculator, itk::Object);
 

  void SetNbOfSamplesByClass(const ClassCountMapType &required);
  void SetNbOfSamplesAllClasses(unsigned long);
  void SetMinimumNbOfSamplesByClass(void);
  void Write(std::string filename);
  void Read(std::string filename);

  itkGetConstReferenceMacro(RatesByClass,MapRateType);

  void SetClassCount(const ClassCountMapType& map);

  /** Clear internal data */
  void ClearRates(void);

protected:
  /** Constructor */
  SamplingRateCalculator();

  /** Destructor */
  virtual ~SamplingRateCalculator() {}

  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  SamplingRateCalculator(const Self &);    //purposely not implemented
  void operator =(const Self&);    //purposely not implemented
  
  void UpdateRate(const std::string &name);

  MapRateType m_RatesByClass;
};
} // End namespace otb

#endif
