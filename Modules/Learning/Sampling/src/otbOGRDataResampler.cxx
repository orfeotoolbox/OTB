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

#include "otbOGRDataResampler.h"

namespace otb
{


void
OGRDataResampler::keyInterpretor(std::string key, std::string &imageName, std::string &className)
{

      std::size_t pos = key.find_first_of("_");
      std::string firstPart = key.substr (0,pos);
      //std::cout << firstPart << std::endl;
      std::size_t posA = firstPart.find_first_of("=");
      imageName = firstPart.substr (posA+1);
      //std::cout << nbImg << std::endl;
      
      std::string secondPart = key.substr (pos);
      //std::cout << secondPart << std::endl;
      std::size_t posB = secondPart.find_first_of("=");
      className = secondPart.substr (posB+1);
      //std::cout << className << std::endl;

}


void
OGRDataResampler::Prepare()
{

if (!m_alreadyPrepared)
{
 
    if (m_RatesbyClass.empty())
    {
         itkGenericExceptionMacro("m_RatesbyClass is empty (otb::OGRDataResampler) ");
    }
         
    std::map<double,std::vector<bool> > mapPercToVect; 

     
    bool mybools1[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0}; // 5%
    bool mybools2[] = {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0}; // 10%
    bool mybools3[] = {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0}; // 15%

    std::vector<bool> vmybools1 (mybools1, mybools1 + sizeof(mybools1) / sizeof(bool) );
    mapPercToVect[5.0] = vmybools1;

    std::vector<bool> vmybools2 (mybools2, mybools2 + sizeof(mybools2) / sizeof(bool) );
    mapPercToVect[10.0] = vmybools2;

    std::vector<bool> vmybools3 (mybools3, mybools3 + sizeof(mybools3) / sizeof(bool) );
    mapPercToVect[15.0] = vmybools3;
    
    //for(int i=0;i<mapPercToVect[15.0].size();i++)
    //std::cout << "### " << mapPercToVect[15.0][i] << std::endl;


    std::map<std::string, double>::iterator it = m_RatesbyClass.begin();
    for(;it !=m_RatesbyClass.end(); ++it)
    {

        double mini=1000;
        std::string imageName,className,bestClassName;
        std::vector<bool> bestBools;
        
        keyInterpretor(it->first,imageName,className);
        
        std::map<double,std::vector<bool> >::iterator it2 = mapPercToVect.begin();
        for(;it2 !=mapPercToVect.end(); ++it2)
          if (fabs(it2->first - it->second)<mini)
          {
               mini=fabs(it2->first - it->second);
               bestClassName = className;
               bestBools = it2->second;
          }
            
        m_ClassToCurrentIndex[bestClassName] = 0;
        m_ClassToBools[bestClassName] = bestBools;
        
    }


}
m_alreadyPrepared=true;
}


bool
OGRDataResampler::TakeSample(std::string className)
{

   bool res=false;
   if (m_ClassToCurrentIndex[className] >= m_ClassToBools[className].size())
   {
       m_ClassToCurrentIndex[className] = 0;
   }
   
   res = m_ClassToBools[className][ m_ClassToCurrentIndex[className] ];
   
   //std::cout << " ------- " << std::endl;
   //std::cout << className << " " << res << " " << m_ClassToCurrentIndex[className] << std::endl;
   
   m_ClassToCurrentIndex[className]++;

   return true;
}

void
OGRDataResampler
::Reset()
{
  m_NbPixelsGlobal = 0UL;
  m_ElmtsInClass.clear();
  m_Polygon.clear();
}

const OGRDataResampler::ClassCountMapType&
OGRDataResampler
::GetClassCountMap()
{
  return m_ElmtsInClass;
}
  
const OGRDataResampler::PolygonSizeMapType&
OGRDataResampler
::GetPolygonSizeMap()
{
  return m_Polygon;
}

unsigned long
OGRDataResampler
::GetNumberOfPixels()
{
  return m_NbPixelsGlobal;
}


const OGRDataResampler::ClassToPhyPosMapType&
OGRDataResampler
::GetClassToPhyPosMap()
{
  return m_ClassToPhyPositions;
}


} // end of namespace otb
