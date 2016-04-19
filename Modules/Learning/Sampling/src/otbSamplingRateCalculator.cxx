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


#include "otbSamplingRateCalculator.h"
#include <sstream>
#include <fstream>
#include <iterator>


namespace otb
{

SamplingRateCalculator
::SamplingRateCalculator()
{
  m_NbClasses = 0; 
}

void
SamplingRateCalculator
::FindAllClasses()
{
  std::string imageName,className;
  
  constItMapType constIt = m_ClassCount.begin();
  for(; constIt != m_ClassCount.end(); ++constIt)
    {
    //constIt->first <=> className       
    m_SetClassNames.insert( constIt->first ); 
    }
  m_NbClasses = m_SetClassNames.size();
}


std::string
SamplingRateCalculator
::KeyGenerator(unsigned int nbC)
{
   std::ostringstream  key;
   key << nbC;

   return key.str();
}

void 
SamplingRateCalculator
::
SetMinimumNbOfSamplesByClass(void)
{
   unsigned int smallestNbofSamples = itk::NumericTraits<unsigned int>::max();
   std::string miniClass;

   constItMapType itMap = m_ClassCount.begin();
   for(; itMap != m_ClassCount.end(); ++itMap)
      if (smallestNbofSamples > itMap->second)
      {
          smallestNbofSamples = itMap->second;
          miniClass = itMap->first;
          
      }
    
   this->SetNbOfSamplesAllClasses( smallestNbofSamples );      
   
}

void 
SamplingRateCalculator
::SetNbOfSamplesAllClasses(unsigned int dRequiredNbSamples)
{
   std::ostringstream oss;
   oss << dRequiredNbSamples;
   
   this->SetNbOfSamplesAllClasses(oss.str());
}

void 
SamplingRateCalculator
::SetNbOfSamplesAllClasses(std::string requiredNbSamples)
{
   std::string requiredNbSamplesByClass;
   constItSetType itClass=m_SetClassNames.begin();
   for(; itClass != m_SetClassNames.end(); ++itClass)
   {
      requiredNbSamplesByClass += *itClass + ":" + requiredNbSamples + " ";
   }
  
  this->SetNbOfSamplesByClass(requiredNbSamplesByClass);
}

void 
SamplingRateCalculator
::SetNbOfSamplesByClass(std::string requiredNbSamplesByClass)
{
  std::string className,nbSamples,imageName;

  std::istringstream issNeenedNbSamples(requiredNbSamplesByClass);

  std::list<std::string> tokens;
  copy(std::istream_iterator<std::string>(issNeenedNbSamples),
	  std::istream_iterator<std::string>(),
    back_inserter(tokens));

  if (m_SetClassNames.size() != tokens.size() )
      itkExceptionMacro(<< "Please, set the number of required samples for each class : " << tokens.size() << "/" << m_NbClasses << " given." << std::endl
                        << "Follow this pattern classname1:nbRequiredSamples1 classname2:nbRequiredSamples2 ...");

  constItSetType itClass=m_SetClassNames.begin();
  std::list<std::string>::iterator tokenIt = tokens.begin();

  ClassCountMapType clVsRequiredNbSamples; //<class name,RequiredNbSamplesByClass>
   
  while( (itClass != m_SetClassNames.end()) && (tokenIt != tokens.end()) )
    {
    std::size_t pos = tokenIt->find_first_of(":");

    if ( (pos == std::string::npos) || (pos+1>=tokenIt->size() ) )
     itkExceptionMacro(<< "Please, follow this pattern --> classname1:nbRequiredSamples1 classname2:nbRequiredSamples2 ...");
     
    className = tokenIt->substr (0,pos);
    nbSamples = tokenIt->substr (pos+1);

    std::istringstream issNbSamples(nbSamples);
    unsigned int uiNbSamples;
    
    if (!(issNbSamples >> uiNbSamples))
       itkExceptionMacro(<< "Please, follow this pattern --> classname1:nbRequiredSamples1 classname2:nbRequiredSamples2 ...");
 
    if ( !(uiNbSamples>0) )
       itkExceptionMacro(<< "Please, set a positive number for the number of samples of the class " << className << ".");
 
    clVsRequiredNbSamples[className]=uiNbSamples;
    
    ++itClass;
    ++tokenIt;
    }

  constItMapType itMap = clVsRequiredNbSamples.begin();
  for(; itMap != clVsRequiredNbSamples.end(); ++itMap)
    {
    if ( !(m_ClassCount.count(itMap->first)>0) )
        itkExceptionMacro(<< "The class " << itMap->first << " is not a element the classes set.");
        
    if (m_ClassCount[itMap->first]<itMap->second)
        itkExceptionMacro(<< "The class " << itMap->first << " only contains " << m_ClassCount[itMap->first]
                          << " samples, but " << itMap->second << " were required.");
    }

  constItSetType itSet = m_SetClassNames.begin();
  for(; itSet != m_SetClassNames.end(); ++itSet)
    {
    if ( !(clVsRequiredNbSamples.count(*itSet)>0) )
        itkExceptionMacro(<< "Please, set the number of samples Required for the class " << *itSet << ".");
    }

  itMap = m_ClassCount.begin();
  for(; itMap != m_ClassCount.end(); ++itMap)
    {
    // itMap->first <=> className
    double overallRate = static_cast<double>(clVsRequiredNbSamples[itMap->first]) / static_cast<double>(m_ClassCount[itMap->first]);
    
    
    TripletType tpt;
    tpt.Required=clVsRequiredNbSamples[itMap->first];
    tpt.Tot=m_ClassCount[itMap->first];
    tpt.Rate=overallRate*100.;
    
    m_RatesByClass[itMap->first] = tpt;
    }
}

void 
SamplingRateCalculator
::Write(std::string filename)
{
  std::ofstream file(filename.c_str(), std::ios::out | std::ios::trunc);

  if (file)
    {
    MapRateType::iterator itRates = m_RatesByClass.begin();
    for(; itRates != m_RatesByClass.end(); ++itRates)
      {
      TripletType tpt=itRates->second;
      file << itRates->first << " " << tpt.Required << " " << tpt.Tot << " " << tpt.Rate << std::endl;
      }
    file.close();
    }
  else
    {
    itkExceptionMacro(<< " Couldn't open " << filename);
    }
}

void
SamplingRateCalculator
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  constItMapType constIt = m_ClassCount.begin();
  for(; constIt != m_ClassCount.end(); ++constIt)
      os << indent << constIt->first << " " << constIt->second << std::endl;

   os << indent << "Nb of classes : " << m_NbClasses << std::endl;
}

} // End namespace otb

