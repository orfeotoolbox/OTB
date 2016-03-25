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
 *
 *
 *
 * \ingroup OTBLerning
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
     unsigned int required;
     unsigned int tot;
     double rate;
   } tripletType;
   
  typedef std::map<std::string, tripletType > mapRateType;
  
  
  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SamplingRateCalculator, itk::Object);
 

  void setNbofSamplesByClass(std::string);
  void setNbofSamplesAllClasses(std::string);
  void setNbofSamplesAllClasses(unsigned int);
  void setMinimumNbofSamplesByClass(void);
  void write(std::string );
  
  itkGetConstReferenceMacro(RatesbyClass,mapRateType);

  void SetClassCount(const ClassCountMapType& map)
  {
    m_ClassCount = map;
    findAllClasses();
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
  
  
  std::string keyGenerator( unsigned int );
  void findAllClasses();

  int m_NbClasses;  
  ClassCountMapType m_ClassCount;
  ClassCountMapType m_totNbSamplesByClass;
  SetType m_setClassNames;
  
  
  mapRateType m_RatesbyClass;

};
} // End namespace otb

#endif
