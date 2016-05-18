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
 *  \brief This class is a sampling rate calculator.
 *
 * It computes sampling rates for each class, given input statistics on
 * available samples, and a sampling strategy.
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

  /** Method to manually set the number of samples required in each class */
  void SetNbOfSamplesByClass(const ClassCountMapType &required);

  /** Method to set the same number of required samples in each class */
  void SetNbOfSamplesAllClasses(unsigned long);

  /** Method to choose a sampling strategy based on the smallest class.
   * The number of samples in each class is set to this minimum size*/
  void SetMinimumNbOfSamplesByClass(void);

  /** Method to export sampling rates in a CSV file.
   * Columns are : class name, required samples, total samples, rate.*/
  void Write(std::string filename);

  /** Method to import sampling rates from a CSV file.
   * Columns are : class name, required samples, total samples, rate.*/
  void Read(std::string filename);

  /** Get macro to the computed sampling rates.*/
  itkGetConstReferenceMacro(RatesByClass,MapRateType);

  /** Set method to input the total number of samples in each class.*/
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

  /** Update the computed rate for a given class.*/
  void UpdateRate(const std::string &name);

  /** Internal sampling rates.*/
  MapRateType m_RatesByClass;
};
} // End namespace otb

#endif
