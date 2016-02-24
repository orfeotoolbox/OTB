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
  typedef std::map<std::string, unsigned long>  MapType;
  typedef std::set<std::string> SetType;
  typedef MapType::const_iterator constItMapType;
  typedef SetType::const_iterator constItSetType;
  
  
  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SamplingRateCalculator, itk::Object);
  
  void produceMap(int nbImg, int nbClasses, unsigned long range);

  //void equalization();
  void setNbofSamplesByClass(std::string);
  void setNbofSamplesAllClasses(std::string);
  void setNbofSamplesAllClasses(unsigned int);
  void setMinimumNbofSamplesByClass(void);
  void write(std::string );


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
  
  
  void keyInterpretor(std::string , std::string &imageName, std::string &className ) const;
  std::string keyGenerator( int  , unsigned int );
  void findImagesAndClasses();

  int m_NbImg;
  int m_NbClasses;  
  MapType m_map;
  MapType m_totNbSamplesByClass;
  SetType m_setClassNames;
  SetType m_setImagesNames;
  std::map<std::string, double>  m_RatesbyClass;

};
} // End namespace otb

#endif
