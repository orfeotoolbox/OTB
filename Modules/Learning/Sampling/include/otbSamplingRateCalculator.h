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
  typedef std::set<std::string> SetType;
  typedef ClassCountMapType::const_iterator constItMapType;
  typedef SetType::const_iterator constItSetType;
  typedef struct
   {
     unsigned int Required;
     unsigned int Tot;
     double Rate;
   } TripletType;
   
  typedef std::map<std::string, TripletType > MapRateType;
  
  
  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SamplingRateCalculator, itk::Object);
 

  void SetNbOfSamplesByClass(std::string);
  void SetNbOfSamplesAllClasses(std::string);
  void SetNbOfSamplesAllClasses(unsigned int);
  void SetMinimumNbOfSamplesByClass(void);
  void Write(std::string filename);

  itkGetConstReferenceMacro(RatesByClass,MapRateType);

  void SetClassCount(const ClassCountMapType& map)
  {
    m_ClassCount = map;
    FindAllClasses();
  }

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

  std::string KeyGenerator( unsigned int );
  void FindAllClasses();

  int m_NbClasses;  
  ClassCountMapType m_ClassCount;
  ClassCountMapType m_TotNbSamplesByClass;
  SetType m_SetClassNames;

  MapRateType m_RatesByClass;
};
} // End namespace otb

#endif
