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
#include <string>
#include <fstream>


namespace otb
{



unsigned int 
OGRDataResampler
::selectN1(double per,unsigned int T)
{
   if (!(T>0))
     itkExceptionMacro(<< "In OGRDataResampler::selectN1 : sizes of the sampling arrays must be non-negative (" << T << ")." << std::endl);

   double dist = itk::NumericTraits<double>::max();
   double d;
   unsigned int N=0;
   double p;

   for(unsigned int n=0;n<=T;n++)
   {
   p=static_cast<double>(n)/static_cast<double>(T)*100.;
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
     itkExceptionMacro(<< "In OGRDataResampler::selectN2 : sizes of the sampling arrays must be non-negative (" << T << ")." << std::endl);

   double dist = itk::NumericTraits<double>::max();
   double d;
   unsigned int N=0;
   double p;

   for(unsigned int n=0;n<=T;n++)
   {
   p=static_cast<double>(n)/static_cast<double>(T)*100.;
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
     itkExceptionMacro(<< "N must be <= to T (aka m_SamplingTabSize)." << std::endl);

   std::vector<bool> res(T,0);
   for(unsigned int i=0; i<N;i++)
       res[i]=1;
   

         
   std::random_shuffle ( res.begin(), res.end() );
   
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
      
   otbWarningMacro(<<"prime number > m_MaxSamplingTabSize (" << tot << ">" << m_MaxSamplingTabSize << ")."<< std::endl );  
   return tot;
}

void
OGRDataResampler::InputSamplingVectors()
{

  m_ClassToBools.clear();
  m_ClassToCurrentIndices.clear();
  
  std::ifstream file(m_InputSamplingVectorsPath.c_str(),std::ios::in);
  std::string line;
  
  if (file)
  {
      std::string className;
      int tabNum=1;
      std::vector<bool> tab1,tab2;
  
      while(getline(file,line))
      {
      
        std::size_t found1 = line.find_first_of("#");
        std::size_t found2 = line.find_first_of(">");
        
        if (found1 != std::string::npos)
        {
          className = line.substr(found1+1);
        }
        if (found2 != std::string::npos)
        {
          if (tabNum==1)
          {
            std::istringstream issBooleans(line.substr(found2+1));
            std::list<std::string> tokens;
	        copy(std::istream_iterator<std::string>(issBooleans),
		    std::istream_iterator<std::string>(),
		    back_inserter(tokens));
          
            std::list<std::string>::iterator it = tokens.begin();
            for(; it != tokens.end(); ++it)
                tab1.push_back( (it->find("1")!=std::string::npos) );
          
            tabNum++;
          }
          else //tabNum==2
          {
          
            std::istringstream issBooleans(line.substr(found2+1));
            std::list<std::string> tokens;
	        copy(std::istream_iterator<std::string>(issBooleans),
		    std::istream_iterator<std::string>(),
		    back_inserter(tokens));
          
            std::list<std::string>::iterator it = tokens.begin();
            for(; it != tokens.end(); ++it)
                tab2.push_back( (it->find("1")!=std::string::npos) );
        
            m_ClassToBools[className] = std::make_pair(tab1,tab2);
            m_ClassToCurrentIndices[className] = std::make_pair(0,0);
          
             tabNum=1;
             className.erase();
             tab1.clear();
             tab2.clear();
          }
        }
      }
  }
  else 
      itkExceptionMacro(<< "Could not open file " << m_InputSamplingVectorsPath << "." << std::endl);
}

void
OGRDataResampler::OutputSamplingVectors()
{
  std::ofstream file(m_OutputSamplingVectorsPath.c_str(), std::ios::out | std::ios::trunc);  
  if(file)  
  {
     ClassToBoolsType::iterator it = m_ClassToBools.begin();
     for(; it!=m_ClassToBools.end(); ++it)
     {
         file << "#" << it->first << std::endl;
         file << ">";
         for(unsigned int i=0; i<it->second.first.size(); i++)
           file << it->second.first[i] << " ";
          file << std::endl;
          
         file << ">"; 
         for(unsigned int i=0; i<it->second.second.size(); i++)
           file << it->second.first[i] << " ";
          file << std::endl;
     }
     file.close();  
  }
  else
     itkExceptionMacro(<< "Could not open file " << m_OutputSamplingVectorsPath << "." << std::endl);
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
         
   if (!m_InputSamplingVectorsPath.empty())
        this->InputSamplingVectors(); 
   else
   {      
       SamplingRateCalculator::mapRateType::iterator itRates = m_RatesbyClass.begin();
       for(;itRates !=m_RatesbyClass.end(); ++itRates)
       {
          //std::cout << "#" << itRates->first << " " << itRates->second.required << " " << itRates->second.tot << std::endl;
          
          unsigned int T1 = findBestSize(itRates->second.tot);
          //std::cout << "*** " << itRates->second.tot << " " << T1 << " " << itRates->second.tot/T1 << std::endl;
         
         
          
          double per = static_cast<double>(itRates->second.required) / static_cast<double>(itRates->second.tot)*100.;
          unsigned int N1 = selectN1(per,T1);

          
          double selected_prct = static_cast<double>(N1)/static_cast<double>(T1)*100.;
          unsigned int taken = static_cast<unsigned int>(selected_prct*static_cast<double>(itRates->second.tot)/100.);
          unsigned int left = itRates->second.required - taken;
          unsigned int newtot = itRates->second.tot - taken;
          
          //std::cout << "##" << N1 << " " << per << " " << selected_prct << " " << taken << " " << left << " " << newtot << std::endl;
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
              
              /*std::cout << "*** " << itRates->second.tot << " " << T1 << " " << itRates->second.tot/T1 << " " << N1 << " " 
              << itRates->second.tot/T1*(T1-N1) << " " << T2 << std::endl;*/
              

              double selected_prct2  = 0.;
              if (T2>0)
              {
                 N2 = selectN2(per2,T2);
                 selected_prct2 = static_cast<double>(N2)/static_cast<double>(T2)*100.;
              }   

              
              //std::cout << "###" << N2 << " " << per2 << " " << selected_prct2 << std::endl;
                
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
          
          
          //itRates->first <=>  className     
          m_ClassToBools[itRates->first] = std::make_pair(tab1,tab2);
          m_ClassToCurrentIndices[itRates->first] = std::make_pair(0,0);
          
          if (!m_OutputSamplingVectorsPath.empty())
            this->OutputSamplingVectors();

       }
   }
         
}
m_alreadyPrepared=true;
}


bool
OGRDataResampler::TakeSample(std::string className)
{
   bool res=false;

   if (m_ElmtsInClass[className] >= m_RatesbyClass[className].required)
     return false;    

   unsigned int &ind1 = m_ClassToCurrentIndices[className].first; //first counter
   std::vector<bool> &tab1 = m_ClassToBools[className].first; // First array
   res = tab1[ind1]; 
   
   ind1++;
   

   
   if (ind1>=tab1.size()) 
     ind1=0;
   
   if(!res)
   {
     unsigned int &ind2 = m_ClassToCurrentIndices[className].second; // second counter
     std::vector<bool> &tab2 = m_ClassToBools[className].second; // second array
     
     if (tab2.size()>0)
     {
         res = tab2[ind2];
         
         ind2++;
         
         if (ind2>=tab2.size()) 
           ind2=0;
     }
       
   }
 
   return res;
}

void
OGRDataResampler
::Reset()
{
  m_ElmtsInClass.clear();
}


} // end of namespace otb
