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

#include "otbPatternSampler.h"
#include "otbMath.h"
#include "otbMacro.h"
#include <algorithm>

namespace otb
{

bool
PatternSampler::ParameterType::operator!=(const PatternSampler::ParameterType & param) const
{
  return bool((MaxPatternSize != param.MaxPatternSize)||
              (InputPath != param.InputPath)||
              (OutputPath != param.OutputPath)||
              (Seed != param.Seed));
}

PatternSampler::PatternSampler()
  : m_Index1(0UL)
  , m_Index2(0UL)
{
  this->m_Parameters.MaxPatternSize = 256;
  this->m_Parameters.InputPath = "";
  this->m_Parameters.OutputPath = "";
  this->m_Parameters.Seed = 121212;
}

void
PatternSampler::Reset(void)
{
  Superclass::Reset();
  
  m_Pattern1.clear();
  m_Pattern2.clear();
  m_Index1 = 0UL;
  m_Index2 = 0UL;

  if (!m_Parameters.InputPath.empty())
    {
    //this->InputSamplingVectors();
    // TODO
    }
  else
    {
    unsigned long T1 = FindBestSize(this->GetTotalElements());

      //double per = static_cast<double>(itRates->second.required) / static_cast<double>(itRates->second.tot)*100.;
      //unsigned int N1 = SelectN1(per,T1);
    unsigned long N1 = static_cast<unsigned long>(vcl_floor( this->GetRate() * T1 ));

    double selected_ratio = static_cast<double>(N1)/static_cast<double>(T1);

    unsigned long taken = static_cast<unsigned long>(selected_ratio*static_cast<double>(this->GetTotalElements()));
    unsigned long left = this->GetNeededElements() - taken;
    unsigned long newtot = this->GetTotalElements() - taken;
    
    // useless
    //if ( !(this->GetRate()>=selected_ratio) )
    //  itkGenericExceptionMacro(<< " per < selected_prct !" << std::endl);

    unsigned long T2 = 0;
    unsigned long N2 = 0;
    if (left>0)
      {
      double ratio2 = 0.0;
      if (newtot>0)
        ratio2 = static_cast<double>(left)/static_cast<double>(newtot);

      T2 = FindBestSize(this->GetTotalElements()/T1*(T1-N1));
      //double selected_ratio2  = 0.;
      if (T2>0)
        {
        N2 = static_cast<unsigned long>(vcl_ceil( ratio2 * T2 ));
        //selected_ratio2 = static_cast<double>(N2)/static_cast<double>(T2);
        }   

      // useless
      //if ( !(ratio2<=selected_ratio2) )
      //  itkGenericExceptionMacro(<< "ratio2 > selected_ratio2 !" << std::endl);
      }

    std::srand ( m_Parameters.Seed );
    m_Pattern1 = RandArray(N1,T1);
    if (T2>0)
       m_Pattern2 = RandArray(N2,T2);

    //if (!m_Parameters.OutputPath.empty())
    //  this->OutputSamplingVectors();
    // TODO
    }
}

bool
PatternSampler::TakeSample(void)
{
  bool ret = false;
  this->m_ProcessedElements += 1UL;
  if (this->m_ChosenElements >= this->GetNeededElements())
    return false;    

  // Test selection with first pattern 
  ret = m_Pattern1[m_Index1];
  m_Index1++;
  if (m_Index1 >= m_Pattern1.size()) 
     m_Index1=0UL;

  if(!ret && m_Pattern2.size())
    {
    // Test selection with second pattern
    ret = m_Pattern2[m_Index2];
    m_Index2++;
    if (m_Index2 >= m_Pattern2.size()) 
      m_Index2=0UL;
    }
  if (ret)
    {
    this->m_ChosenElements += 1UL;
    }
  return ret;
}

std::vector<bool>
PatternSampler::RandArray(unsigned long N,unsigned long T)
{
   if (N>T)
     itkGenericExceptionMacro(<< "N must be <= to T (aka m_SamplingTabSize)." << std::endl);

   std::vector<bool> res(T,0);
   for(unsigned long i=0; i<N;i++)
       res[i]=1;

   std::random_shuffle ( res.begin(), res.end() );
   
   return res;
}


unsigned long
PatternSampler::FindBestSize(unsigned long tot)
{
  // handle small values
  if (tot<=m_Parameters.MaxPatternSize)
    return tot;

  // try to find a sub-period in tot, between 20 and MaxPatternSize
  for(unsigned long size = m_Parameters.MaxPatternSize; size >= 20; size--)
    if (tot%size == 0)
      return size;
      
  otbWarningMacro(<<"prime number > m_Parameters.MaxPatternSize (" << tot << ">" << m_Parameters.MaxPatternSize << ")."<< std::endl );
  // fallback : return the maximum size
  return m_Parameters.MaxPatternSize;
}

//void
//PatternSampler::InputSamplingVectors()
//{

  //m_ClassToBools.clear();
  //m_ClassToCurrentIndices.clear();
  
  //std::ifstream file(m_InputSamplingVectorsPath.c_str(),std::ios::in);
  //std::string line;
  
  //if (file)
  //{
      //std::string className;
      //int tabNum=1;
      //std::vector<bool> tab1,tab2;
  
      //while(getline(file,line))
      //{
      
        //std::size_t found1 = line.find_first_of("#");
        //std::size_t found2 = line.find_first_of(">");
        
        //if (found1 != std::string::npos)
        //{
          //className = line.substr(found1+1);
        //}
        //if (found2 != std::string::npos)
        //{
          //if (tabNum==1)
          //{
            //std::istringstream issBooleans(line.substr(found2+1));
            //std::list<std::string> tokens;
	        //copy(std::istream_iterator<std::string>(issBooleans),
		    //std::istream_iterator<std::string>(),
		    //back_inserter(tokens));
          
            //std::list<std::string>::iterator it = tokens.begin();
            //for(; it != tokens.end(); ++it)
                //tab1.push_back( (it->find("1")!=std::string::npos) );
          
            //tabNum++;
          //}
          //else //tabNum==2
          //{
          
            //std::istringstream issBooleans(line.substr(found2+1));
            //std::list<std::string> tokens;
	        //copy(std::istream_iterator<std::string>(issBooleans),
		    //std::istream_iterator<std::string>(),
		    //back_inserter(tokens));
          
            //std::list<std::string>::iterator it = tokens.begin();
            //for(; it != tokens.end(); ++it)
                //tab2.push_back( (it->find("1")!=std::string::npos) );
        
            //m_ClassToBools[className] = std::make_pair(tab1,tab2);
            //m_ClassToCurrentIndices[className] = std::make_pair(0,0);
          
             //tabNum=1;
             //className.erase();
             //tab1.clear();
             //tab2.clear();
          //}
        //}
      //}
  //}
  //else 
      //itkGenericExceptionMacro(<< "Could not open file " << m_InputSamplingVectorsPath << "." << std::endl);
//}

//void
//PatternSampler::OutputSamplingVectors()
//{
  //std::ofstream file(m_OutputSamplingVectorsPath.c_str(), std::ios::out | std::ios::trunc);  
  //if(file)  
  //{
     //ClassToBoolsType::iterator it = m_ClassToBools.begin();
     //for(; it!=m_ClassToBools.end(); ++it)
     //{
         //file << "#" << it->first << std::endl;
         //file << ">";
         //for(unsigned int i=0; i<it->second.first.size(); i++)
           //file << it->second.first[i] << " ";
          //file << std::endl;
          
         //file << ">"; 
         //for(unsigned int i=0; i<it->second.second.size(); i++)
           //file << it->second.second[i] << " ";
          //file << std::endl;
     //}
     //file.close();  
  //}
  //else
     //itkGenericExceptionMacro(<< "Could not open file " << m_OutputSamplingVectorsPath << "." << std::endl);
//}


} // namespace otb
