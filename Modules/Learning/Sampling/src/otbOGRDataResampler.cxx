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
OGRDataResampler
::keyInterpretor2(std::string key, std::string &className) const
{
      std::size_t pos = key.find_first_of("=");
      className = key.substr(pos+1);
      //std::cout << "className = " << className << std::endl;
}

unsigned int 
OGRDataResampler
::selectN1(double per,unsigned int T)
{
   if (!(T>0))
     itkExceptionMacro(<< "In OGRDataResampler::selectN1 : sizes of the sampling arrays must be non-negative ("<< T << ")." << std::endl);

   double dist = itk::NumericTraits<double>::max();
   double d;
   unsigned int N=0;
   double p;

   for(unsigned int n=0;n<=T;n++)
   {
   p=static_cast<double>(n)/static_cast<double>(T)*100.;
   //std::cout << n << " "<< T << " " << p << std::endl;
       if (p<=per)
       {
          d = fabs(per-p);
          if (d<dist)
          {
              N=n;
              dist=d;
          }
       }
   }
   return N;
}


unsigned int 
OGRDataResampler
::selectN2(double per,unsigned int T)
{
   if (!(T>0))
     itkExceptionMacro(<< "In OGRDataResampler::selectN2 : sizes of the sampling arrays must be non-negative ("<< T << ")." << std::endl);

   double dist = itk::NumericTraits<double>::max();
   double d;
   unsigned int N=0;
   double p;

   for(unsigned int n=0;n<=T;n++)
   {
   p=static_cast<double>(n)/static_cast<double>(T)*100.;
   //std::cout << n << " "<< T << " " << per << " " << p << std::endl;
       if (p>=per)
       {
          d = fabs(per-p);
          if (d<dist)
          {
              N=n;
              dist=d;
          }
       }
   }
   return N;
}



std::vector<bool>
OGRDataResampler::randArray(unsigned int N,unsigned int T)
{

   if (N>T)
     itkExceptionMacro(<< "In otbOGRDataResampler::randArray(unsigned int N, unsigned int T), N must be <= to T (aka m_SamplingTabSize)." << std::endl);

   std::vector<bool> res(T,0);
   for(unsigned int i=0; i<N;i++)
       res[i]=1;
   
   /*std::cout << "-------------------" << std::endl;
   for(int i=0; i<m_SamplingTabSize;i++)
      std::cout << "res[" << i <<  "] = " << res[i] << std::endl; */
         
   std::random_shuffle ( res.begin(), res.end() );
   
   /*std::cout << "-------------------" << std::endl;
   for(int i=0; i<m_SamplingTabSize;i++)
      std::cout << "res[" << i <<  "] = " << res[i] << std::endl; */
   return res;

}


unsigned int
OGRDataResampler::findBestSize(unsigned int tot)
{
   if (tot<m_MaxSamplingTabSize)
      return tot;
   
   for(unsigned int size=m_MaxSamplingTabSize; size>=2; size--)
   if (tot%size == 0)
      return size;
      
   otbWarningMacro(<<"prime number > m_MaxSamplingTabSize (" << tot << "," << m_MaxSamplingTabSize << ")."<< std::endl );  
   return 1;
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
         
   SamplingRateCalculator::mapRateType::iterator itRates = m_RatesbyClass.begin();
   for(;itRates !=m_RatesbyClass.end(); ++itRates)
   {
      std::cout << "#" << itRates->first << " " << itRates->second.required << " " << itRates->second.tot << std::endl;
      
      unsigned int T1 = findBestSize(itRates->second.tot);
      std::cout << "*** " << itRates->second.tot << " " << T1 << " " << itRates->second.tot/T1 << std::endl;
     
     
      
      double per = static_cast<double>(itRates->second.required) / static_cast<double>(itRates->second.tot)*100.;
      unsigned int N1 = selectN1(per,T1);

      
      double selected_prct = static_cast<double>(N1)/static_cast<double>(T1)*100.;
      unsigned int taken = static_cast<unsigned int>(selected_prct*static_cast<double>(itRates->second.tot)/100.);
      unsigned int left = itRates->second.required - taken;
      unsigned int newtot = itRates->second.tot - taken;
      
      std::cout << "##" << N1 << " " << per << " " << selected_prct << " " << taken << " " << left << " " << newtot << std::endl;
      if ( !(per>=selected_prct) )
        itkExceptionMacro(<< "In otb::OGRDataResampler::Prepare, per < selected_prct !" << std::endl);
      
      unsigned int T2 = 0;
      unsigned int N2 = 0;
      if (left>0)
      {
          
          double per2;
          if (newtot>0)
             per2 = static_cast<double>(left)/static_cast<double>(newtot)*100.;
          else
             per2 = 0;
             
          T2 = findBestSize(itRates->second.tot/T1*(T1-N1));
          
          std::cout << "*** " << itRates->second.tot << " " << T1 << " " << itRates->second.tot/T1 << " " << N1 << " " 
          << itRates->second.tot/T1*(T1-N1) << " " << T2 << std::endl;
          

          double selected_prct2  = 0.;
          if (T2>0)
          {
             N2 = selectN2(per2,T2);
             selected_prct2 = static_cast<double>(N2)/static_cast<double>(T2)*100.;
          }   

          
          std::cout << "###" << N2 << " " << per2 << " " << selected_prct2 << std::endl;
            
          if ( !(per2<=selected_prct2) )
            itkExceptionMacro(<< "In otb::OGRDataResampler::Prepare, per2 > selected_prct2 !" << std::endl);
      }
    
      
      std::srand ( unsigned ( time(0) ) );
      std::vector<bool> tab1 = randArray(N1,T1);
      std::vector<bool> tab2;
      if (T2>0)
         tab2 = randArray(N2,T2);
      
      unsigned int N=0;
      for(unsigned int i=0;i<tab1.size();i++)
      if (tab1[i])
        N++;
      if (N!=N1)
        itkExceptionMacro(<< "N != N1." << std::endl);
    
      N=0;
      for(unsigned int i=0;i<tab2.size();i++)
      if (tab2[i])
        N++;
      if (N!=N2)
        itkExceptionMacro(<< "N != N2." << std::endl);
      
      std::string className;
      keyInterpretor2(itRates->first,className);
      
      m_ClassToBools[className] = std::make_pair(tab1,tab2);
      m_ClassToCurrentIndices[className] = std::make_pair(0,0);
   }
         
}
m_alreadyPrepared=true;
}


bool
OGRDataResampler::TakeSample(std::string className)
{
return true;

   bool res=false;

   
   //TODO : remove this awful system
   std::ostringstream  key;
   key << "class=" << className;
   
   if (m_ClassToPhyPositions[className].size()>=m_RatesbyClass[key.str()].required)
     return false;
     

   unsigned int &ind1 = m_ClassToCurrentIndices[className].first; //first counter
   std::vector<bool> &tab1 = m_ClassToBools[className].first; // First array
   res = tab1[ind1]; 
   
   ind1++;
   
   if (className.compare(std::string("2")) == 0)
   std::cout << "ind1 = " << ind1 << std::endl;
   
   if (ind1>=tab1.size()) 
   {  ind1=0;
   if (className.compare(std::string("2")) == 0)
       std::cout << "ooo un tour a été fait pour le tab1, classe : " << className << std::endl;
   }
   
   if(!res)
   {
     unsigned int &ind2 = m_ClassToCurrentIndices[className].second; // second counter
     std::vector<bool> &tab2 = m_ClassToBools[className].second; // second array
     
     if (tab2.size()>0)
     {
         res = tab2[ind2];
         
         ind2++;
         
         if (ind2>=tab2.size()) 
         {   ind2=0;
         if (className.compare(std::string("2")) == 0)
             std::cout << "ooo un tour a été fait pour le tab2, classe : " << className << std::endl;
         }
     }
       
   }
 
   return res;
}

void
OGRDataResampler
::Reset()
{
  m_NbPixelsGlobal = 0UL;
  m_ElmtsInClass.clear();
  m_Polygon.clear();
  m_ClassToPhyPositions.clear();
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
