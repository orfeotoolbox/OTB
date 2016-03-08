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
  m_NbImg = 0;
  m_NbClasses = 0; 
}


void
SamplingRateCalculator
::keyInterpretor(std::string key, std::string &className) const
{

      /*std::size_t pos = key.find_first_of("_");
      std::string firstPart = key.substr (0,pos);
      //std::cout << firstPart << std::endl;
      std::size_t posA = firstPart.find_first_of("=");
      imageName = firstPart.substr (posA+1);
      //std::cout << nbImg << std::endl;
      
      std::string secondPart = key.substr (pos);
      //std::cout << secondPart << std::endl;
      std::size_t posB = secondPart.find_first_of("=");
      className = secondPart.substr (posB+1);
      //std::cout << className << std::endl;*/
      
      std::size_t pos = key.find_first_of("=");
      className = key.substr(pos+1);
      std::cout << "className = " << className << std::endl;
}


void
SamplingRateCalculator
::findImagesAndClasses()
{

  std::string imageName,className;
  
  constItMapType constIt = m_map.begin();
  for(; constIt != m_map.end(); ++constIt)
   {
      keyInterpretor(constIt->first,className);
      
      m_setImagesNames.insert( imageName );
        
      m_setClassNames.insert( className ); 
      
      if ( !(m_totNbSamplesByClass.count(className)>0) )
        m_totNbSamplesByClass[className] = 0;
    
      m_totNbSamplesByClass[className] += constIt->second;
        
   }
   
   m_NbImg = m_setImagesNames.size();
   m_NbClasses = m_setClassNames.size();
   
}


/*
 * PrintSelf Method
 */



std::string
SamplingRateCalculator
::keyGenerator(unsigned int nbC)
{

   std::ostringstream  key;
   key << "class=" << nbC;

   return key.str();
}


void 
SamplingRateCalculator
::produceMap(int nbImg, int nbClasses, unsigned long range)
{

   srand((unsigned)time(NULL));

  // for (int i=0; i<nbImg; i++)
        for (int c=0; c<nbClasses; c++)
        {

           std::string key = keyGenerator(c);
           m_map[key] = rand() % (range + 1);
           
        }
    
   findImagesAndClasses();
}

void 
SamplingRateCalculator
::produceMap()
{
/*
   class 1 = 104
   class 2 = 160
   class 3 = 211

 * */
 

    std::string key = keyGenerator(1);
    m_map[key] = 104;
    key = keyGenerator(2);
    m_map[key] = 160;
    key = keyGenerator(3);
    m_map[key] = 211;
           
    findImagesAndClasses();
}


void 
SamplingRateCalculator
::
setMinimumNbofSamplesByClass(void)
{
   unsigned int smallestNbofSamples = itk::NumericTraits<unsigned int>::max();
   std::string miniClass;

   constItMapType itMap = m_totNbSamplesByClass.begin();
   for(; itMap != m_totNbSamplesByClass.end(); ++itMap)
      if (smallestNbofSamples > itMap->second)
      {
          smallestNbofSamples = itMap->second;
          miniClass = itMap->first;
          
      }
    
   std::cout << "miniClass = " << miniClass << " smallestNbofSamples = " << smallestNbofSamples << std::endl;       
   this->setNbofSamplesAllClasses( smallestNbofSamples );      
   
}

void 
SamplingRateCalculator
::setNbofSamplesAllClasses(unsigned int dRequiredNbSamples)
{
   std::ostringstream oss;
   oss << dRequiredNbSamples;
   
   this->setNbofSamplesAllClasses(oss.str());
}

void 
SamplingRateCalculator
::setNbofSamplesAllClasses(std::string RequiredNbSamples)
{
   std::string RequiredNbSamplesByClass;
   constItSetType itClass=m_setClassNames.begin();
   for(; itClass != m_setClassNames.end(); ++itClass)
   {
      RequiredNbSamplesByClass += *itClass + ":" + RequiredNbSamples + " ";
   }
  // std::cout << " RequiredNbSamplesByClass = " << RequiredNbSamplesByClass << std::endl;
  
  this->setNbofSamplesByClass(RequiredNbSamplesByClass);
}

void 
SamplingRateCalculator
::setNbofSamplesByClass(std::string RequiredNbSamplesByClass)
{

   std::string className,nbSamples,imageName;

   std::istringstream issNeenedNbSamples(RequiredNbSamplesByClass);

   std::list<std::string> tokens;
	     copy(std::istream_iterator<std::string>(issNeenedNbSamples),
		 std::istream_iterator<std::string>(),
		 back_inserter(tokens));

   
   if (m_setClassNames.size() != tokens.size() )
      itkExceptionMacro(<< "Please, set the number of required samples for each class : " << tokens.size() << "/" << m_NbClasses << " given." << std::endl
                        << "Follow this pattern classname1:nbRequiredSamples1 classname2:nbRequiredSamples2 ...");
   
   
   
   constItSetType itClass=m_setClassNames.begin();
   std::list<std::string>::iterator tokenIt = tokens.begin();
   
   MapType clVsRequiredNbSamples; //<class name,RequiredNbSamplesByClass>
   
   while( (itClass != m_setClassNames.end()) && (tokenIt != tokens.end()) )
   {
      
   
      std::size_t pos = tokenIt->find_first_of(":");
      
      //std::cout << "token = " << *tokenIt << " token.size = " << tokenIt->size() << " pos = " << pos << std::endl;
      
      if ( (pos == std::string::npos) || (pos+1>=tokenIt->size() ) )
       itkExceptionMacro(<< "Please, follow this pattern --> classname1:nbRequiredSamples1 classname2:nbRequiredSamples2 ...");
       
      className = tokenIt->substr (0,pos);
      nbSamples = tokenIt->substr (pos+1);
      
      //std::cout << "className = " << className  << " nbSamples = " << nbSamples << std::endl;
   
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
      if ( !(m_totNbSamplesByClass.count(itMap->first)>0) )
          itkExceptionMacro(<< "The class " << itMap->first << " is not a element the classes set.");
          
      if (m_totNbSamplesByClass[itMap->first]<itMap->second)
          itkExceptionMacro(<< "The class " << itMap->first << " only contains " << m_totNbSamplesByClass[itMap->first]
                            << " samples, but " << itMap->second << " were required.");
                            
   }
   
   constItSetType itSet = m_setClassNames.begin();
   for(; itSet != m_setClassNames.end(); ++itSet)
   {
      if ( !(clVsRequiredNbSamples.count(*itSet)>0) )
          itkExceptionMacro(<< "Please, set the number of samples Required for the class " << *itSet << ".");
                            
   }
   
  itMap = m_map.begin();
  for(; itMap != m_map.end(); ++itMap)
   {
      keyInterpretor(itMap->first,className);

      double overallRate = static_cast<double>(clVsRequiredNbSamples[className]) / static_cast<double>(m_totNbSamplesByClass[className]);
      //double localRatio = static_cast<double>(itMap->second) / static_cast<double>(m_totNbSamplesByClass[className]);
      
      
      tripletType tpt;
      tpt.required=clVsRequiredNbSamples[className];
      tpt.tot=0;
      tpt.rate=overallRate*100.;
      
      m_RatesbyClass[itMap->first] = tpt;
      
      //std::cout << itMap->first << " " << m_RatesbyClass[itMap->first] << "%" << std::endl;
    
   }
   
}

void 
SamplingRateCalculator
::write(std::string filename)
{
  std::ofstream file(filename.c_str(), std::ios::out | std::ios::trunc);
  
  if (file)
  {
     //file << m_RatesbyClass.str();
     mapRateType::iterator itRates = m_RatesbyClass.begin();
     for(; itRates != m_RatesbyClass.end(); ++itRates)
     {
        tripletType tpt=itRates->second;
        file << itRates->first << " " << tpt.required << " " << tpt.tot << " " << tpt.rate << std::endl;
     }
     
     file.close();
  }
  else
  {
     itkExceptionMacro(<< " Couldn't open " << filename);
  }
  
}


const SamplingRateCalculator::mapRateType& 
SamplingRateCalculator
::GetRatesbyClass()
{
    return m_RatesbyClass;
}

void
SamplingRateCalculator
::PrintSelf(std::ostream& os, itk::Indent indent) const
{

  constItMapType constIt = m_map.begin();
  for(; constIt != m_map.end(); ++constIt)
      os << indent << constIt->first << " " << constIt->second << std::endl;
      
   os << indent << "Nb of images : " << m_NbImg << " nb of classes : " << m_NbClasses << std::endl;
}


} // End namespace otb

