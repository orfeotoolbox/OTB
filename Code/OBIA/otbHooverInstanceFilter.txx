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
#ifndef __otbHooverInstanceFilter_txx
#define __otbHooverInstanceFilter_txx

#include "otbHooverInstanceFilter.h"
#include "otbMacro.h"

namespace otb
{
/** Hoover Attribute names */
template <class TLabelMap>
const std::string HooverInstanceFilter<TLabelMap>::ATTRIBUTE_CD="HooverInstance_CD";

template <class TLabelMap>
const std::string HooverInstanceFilter<TLabelMap>::ATTRIBUTE_OS="HooverInstance_OS";

template <class TLabelMap>
const std::string HooverInstanceFilter<TLabelMap>::ATTRIBUTE_US="HooverInstance_US";

template <class TLabelMap>
const std::string HooverInstanceFilter<TLabelMap>::ATTRIBUTE_M="HooverInstance_M";

template <class TLabelMap>
const std::string HooverInstanceFilter<TLabelMap>::ATTRIBUTE_N="HooverInstance_N";

template <class TLabelMap>
const std::string HooverInstanceFilter<TLabelMap>::ATTRIBUTE_RC="HooverInstance_RC";

template <class TLabelMap>
const std::string HooverInstanceFilter<TLabelMap>::ATTRIBUTE_RF="HooverInstance_RF";

template <class TLabelMap>
const std::string HooverInstanceFilter<TLabelMap>::ATTRIBUTE_RA="HooverInstance_RA";

/** Constructor */
template <class TLabelMap>
HooverInstanceFilter<TLabelMap>
::HooverInstanceFilter() : m_NumberOfRegionsGT(0), m_NumberOfRegionsMS(0), m_Threshold(0.8)
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(2);
  typename LabelMapType::Pointer secondOutput = LabelMapType::New();
  this->AddOutput(secondOutput);
  
  m_HooverMatrix.SetSize(0, 0);
  m_CardRegGT.SetSize(0);
  m_CardRegMS.SetSize(0);
  m_LabelsGT.resize(0);
}

/** Set the ground truth label map */
template <class TLabelMap>
void HooverInstanceFilter<TLabelMap>
::SetGroundTruthLabelMap(const LabelMapType *gt)
{
  this->SetInput(0, gt);
}

/** Set the machine segmentation label map */
template <class TLabelMap>
void HooverInstanceFilter<TLabelMap>
::SetMachineSegmentationLabelMap(const LabelMapType *ms)
{
  this->SetInput(1, ms);
}

/** Get the input ground truth label map */
template <class TLabelMap>
const TLabelMap* HooverInstanceFilter<TLabelMap>
::GetGroundTruthLabelMap()
{
  return this->GetInput(0);
}

/** Get the input machine segmentation label map */
template <class TLabelMap>
const TLabelMap* HooverInstanceFilter<TLabelMap>
::GetMachineSegmentationLabelMap()
{
  return this->GetInput(1);
}

/** Get the output ground truth label map */
template <class TLabelMap>
TLabelMap* HooverInstanceFilter<TLabelMap>
::GetOutputGroundTruthLabelMap()
{
  return this->GetOutput(0);
}

/** Get the output machine segmentation label map */
template <class TLabelMap>
TLabelMap* HooverInstanceFilter<TLabelMap>
::GetOutputMachineSegmentationLabelMap()
{
  return this->GetOutput(1);
}

template <class TLabelMap>
void HooverInstanceFilter<TLabelMap>
::AllocateOutputs()
{
  Superclass::AllocateOutputs();
  
  if( this->GetInPlace() && this->CanRunInPlace() )
    {
    LabelMapPointer secondInput = const_cast<TLabelMap *>(this->GetMachineSegmentationLabelMap());
    
    if( secondInput )
      {
      ImageRegionType region = this->GetOutput(1)->GetLargestPossibleRegion();
      this->GraftNthOutput( 1 , secondInput );
      this->GetOutput(1)->SetRegions( region );
      }
    }
  else
    {
    // copying the second input : machine segmentation
    const TLabelMap * inputMS = this->GetInput(1);
    TLabelMap * outputMS = this->GetOutput(1);
    assert( inputMS != NULL );
    assert( outputMS != NULL );
    
    outputMS->SetBackgroundValue( inputMS->GetBackgroundValue() );

    const LabelObjectContainerType & labelObjectContainer = inputMS->GetLabelObjectContainer();

    LabelObjectContainerTypeConstIterator  it = labelObjectContainer.begin();

    while( it != labelObjectContainer.end() )
      {
      const LabelObjectType * labeObject = it->second;

      assert( labeObject != NULL );
      assert( labeObject->GetLabel() == it->first );

      typename LabelObjectType::Pointer newLabelObject = LabelObjectType::New();
      newLabelObject->CopyAllFrom( labeObject );
      
      outputMS->AddLabelObject( newLabelObject );
      it++;
      }
    }
}

template <class TLabelMap>
void HooverInstanceFilter<TLabelMap>
::ReleaseInputs()
{
  Superclass::ReleaseInputs();
  
  if( this->GetInPlace() )
    {
    // Release second input
    TLabelMap * ptr = const_cast<TLabelMap*>( this->GetInput(1) );
    if( ptr )
      {
      ptr->ReleaseData();
      }
    }
}

template <class TLabelMap>
void HooverInstanceFilter<TLabelMap>
::BeforeThreadedGenerateData()
{
  // first : call superclass method
  Superclass::BeforeThreadedGenerateData();
  
  m_NumberOfRegionsGT = this->GetGroundTruthLabelMap()->GetNumberOfLabelObjects();
  m_NumberOfRegionsMS = this->GetMachineSegmentationLabelMap()->GetNumberOfLabelObjects();
  
  if (m_NumberOfRegionsGT == 0 || m_NumberOfRegionsMS == 0)
    {
    itkExceptionMacro("Empty label map");
    }
  
  //Check the matrix size
  if (m_NumberOfRegionsGT != m_HooverMatrix.Rows() || m_NumberOfRegionsMS != m_HooverMatrix.Cols())
    {
    itkExceptionMacro("The given Hoover confusion matrix ("<<m_HooverMatrix.Rows()<<" x "<<m_HooverMatrix.Cols() <<
                      ") doesn't match with the input label maps ("<<m_NumberOfRegionsGT<<" x "<<m_NumberOfRegionsMS<<")");
    }
  
  //Init cardinalities lists
  m_CardRegGT.SetSize(m_NumberOfRegionsGT);
  m_CardRegGT.Fill(0);
  
  m_CardRegMS.SetSize(m_NumberOfRegionsMS);
  m_CardRegMS.Fill(0);
  
  //Fill cardinalities list for MS
  unsigned long i = 0;
  const LabelObjectContainerType & containerMS = this->GetMachineSegmentationLabelMap()->GetLabelObjectContainer();
  LabelObjectContainerTypeConstIterator iter;
  typename LabelObjectType::Pointer blankRegion;
  
  for (iter = containerMS.begin(); iter != containerMS.end(); iter++)
    {
    LabelObjectType *regionMS = iter->second;
    m_CardRegMS[i] = regionMS->Size();
    if (m_CardRegMS[i] == 0)
      {
      otbWarningMacro("Region "<<i<<" in machine segmentation label map is empty");
      }
    // check that no Hoover attribute is already present
    blankRegion = LabelObjectType::New();
    blankRegion->SetLabel(regionMS->GetLabel());
    std::vector< std::string > attKeys = regionMS->GetAvailableAttributes();
    for (unsigned int k=0; k<attKeys.size(); k++)
      {
      if (attKeys[k].find("HooverInstance_") == 0)
        {
        continue;
        }
      else
        {
        blankRegion->SetAttribute(attKeys[k].c_str(), regionMS->GetAttribute(attKeys[k].c_str()));
        }
      }
    regionMS->CopyAttributesFrom(blankRegion);
    i++;
    }
  
  m_LabelsGT = this->GetGroundTruthLabelMap()->GetLabels();
}

template <class TLabelMap>
void HooverInstanceFilter<TLabelMap>
::ThreadedProcessLabelObject( LabelObjectType * labelObject )
{
  // Find the index corresponding to the current label object in GT
  unsigned long currentRegionGT = 0;
  LabelType currentLabelGT = labelObject->GetLabel();
  for (unsigned long k=0; k<m_NumberOfRegionsGT; k++)
    {
    if (currentLabelGT == m_LabelsGT[k])
      {
      currentRegionGT = k;
      break;
      }
    }
  
  m_CardRegGT[currentRegionGT] = labelObject->Size();
  if (m_CardRegGT[currentRegionGT] == 0)
    {
    otbWarningMacro("Region "<<currentRegionGT<<" in ground truth label map is empty");
    }
  
  // check that no Hoover attribute is already present
  typename LabelObjectType::Pointer blankRegion;
  blankRegion = LabelObjectType::New();
  blankRegion->SetLabel(labelObject->GetLabel());
  std::vector< std::string > attKeys = labelObject->GetAvailableAttributes();
  for (unsigned int k=0; k<attKeys.size(); k++)
    {
    if (attKeys[k].find("HooverInstance_") == 0)
      {
      continue;
      }
    else
      {
      blankRegion->SetAttribute(attKeys[k].c_str(), labelObject->GetAttribute(attKeys[k].c_str()));
      }
    }
  labelObject->CopyAttributesFrom(blankRegion);
}

template <class TLabelMap>
void HooverInstanceFilter<TLabelMap>
::AfterThreadedGenerateData()
{
  LabelMapType* outGT = this->GetOutput(0);
  LabelMapType* outMS = this->GetOutput(1);
  
  // Iterators on label object container (to gain efficiency when accessing them)
  const LabelObjectContainerType & containerGT = outGT->GetLabelObjectContainer();
  const LabelObjectContainerType & containerMS = outMS->GetLabelObjectContainer();
  LabelObjectContainerTypeConstIterator iterGT = containerGT.begin();
  LabelObjectContainerTypeConstIterator iterMS;
  
  // Set of classified regions
  RegionSetType GTindices;
  RegionSetType MSindices;
  
  // flags to detect empty rows or columns
  bool IsRowEmpty;
  bool IsColEmpty;
  
  // first pass : loop on GT regions first
  for(unsigned int row=0; row<m_NumberOfRegionsGT; row++, iterGT++)
    {
    double sumOS = 0.0; // sum of coefT for potential over-segmented regions
    double sumScoreRF = 0.0; // temporary sum  of (Tij x (Tij - 1)) terms for the RF score
    RegionSetType     regionsOfMS; // stores region indexes
    ObjectVectorType  objectsOfMS; // stores region pointers
    
    double tGT = static_cast<double>(m_CardRegGT[row]) * m_Threshold; // card Ri x t
    IsRowEmpty = true;
    iterMS = containerMS.begin();
    for(unsigned int col=0; col<m_NumberOfRegionsMS; col++, iterMS++)
      {
      // Tij
      double coefT = static_cast<double>(m_HooverMatrix(row, col));
      if(coefT < 0.5)
        {
        // the regions Ri and ^Rj have an empty intersection : we can jump to the next matrix cell
        continue;
        }
      else
        {
        IsRowEmpty = false;
        }
      
      double tMS = static_cast<double>(m_CardRegMS[col]) * m_Threshold; // card Rj x t
      
      otbDebugMacro(<< "* coef[" << row << "," << col << "]=" << coefT << " #tGT=" << tGT << " #tMS=" << tMS);
      
      // Looking for Correct Detection and Over Segmentation (both can happen for the same GT region)
      if(coefT>=tMS)
        {
        if(coefT>=tGT)
          {
          otbDebugMacro(<< "1 coef[" << row << "," << col << "]=" << coefT << " #tGT=" << tGT << " #tMS=" << tMS << " -> CD");
          
          LabelObjectType *regionGT = iterGT->second;
          LabelObjectType *regionMS = iterMS->second;
          double scoreRC = m_Threshold * (std::min(coefT / tGT, coefT / tMS));
          
          regionGT->SetAttribute(ATTRIBUTE_CD.c_str(), static_cast<AttributesValueType>(regionMS->GetLabel()));
          regionGT->SetAttribute(ATTRIBUTE_RC.c_str(), static_cast<AttributesValueType>(scoreRC));
          
          regionMS->SetAttribute(ATTRIBUTE_CD.c_str(), static_cast<AttributesValueType>(regionGT->GetLabel()));
          regionMS->SetAttribute(ATTRIBUTE_RC.c_str(), static_cast<AttributesValueType>(scoreRC));
          
          GTindices.insert(row);
          MSindices.insert(col);
          }
        else
          {
          otbDebugMacro(<< "2 coef[" << row << "," << col << "]=" << coefT << " #tGT=" << tGT << " #tMS=" << tMS << " -> OSmaybe");
          }
        objectsOfMS.push_back(iterMS->second); // candidate region for over-segmentation
        regionsOfMS.insert(col);
        sumOS += coefT;
        sumScoreRF += coefT*(coefT-1.0);
        }
      } // end of column loop
    
    otbDebugMacro(<< "end of line " << row << "; sumOS=" << sumOS << " " << regionsOfMS.size() << " of MS region");
    if(sumOS>=tGT && sumOS>0)
      {
      // CD
      if(regionsOfMS.size()==1)
        {
        otbDebugMacro(<< "CD only");
        }
      // OS
      else if(regionsOfMS.size()>1)
        {
        otbDebugMacro(<< row << " OS by ");
        LabelObjectType *regionGT = iterGT->second;
        
        double cardRegGT = static_cast<double>(m_CardRegGT[row]);
        double scoreRF = 1.0 - sumScoreRF / (cardRegGT * (cardRegGT - 1.0));
        
        regionGT->SetAttribute(ATTRIBUTE_RF.c_str(), static_cast<AttributesValueType>(scoreRF));
        
        unsigned int indexOS=1;
        for(typename ObjectVectorType::iterator it=objectsOfMS.begin(); it!=objectsOfMS.end(); ++it)
          {
          LabelObjectType *regionMS = *it;
          std::ostringstream attribute;
          attribute << ATTRIBUTE_OS << "_" << indexOS;
          
          regionGT->SetAttribute(attribute.str().c_str(), static_cast<AttributesValueType>(regionMS->GetLabel()));
          
          regionMS->SetAttribute(ATTRIBUTE_OS.c_str(), static_cast<AttributesValueType>(regionGT->GetLabel()));
          regionMS->SetAttribute(ATTRIBUTE_RF.c_str(), static_cast<AttributesValueType>(scoreRF));
          indexOS++;
          }
        
        GTindices.insert(row);
        for(RegionSetType::iterator it=regionsOfMS.begin(); it!=regionsOfMS.end(); ++it)
          {
          MSindices.insert(*it);
          otbDebugMacro(<< *it << " ");
          }
        }
      else
        {
        otbDebugMacro(<< "No MS region present in potential OS instance");
        }
      }
    // check for empty rows : they should be ignored and have no Hoover attribute
    if (IsRowEmpty)
      {
      GTindices.insert(row);
      }
    } // end of line loop

  // second pass : loop on MS regions first
  iterMS = containerMS.begin();
  for(unsigned int col=0; col<m_NumberOfRegionsMS; col++, iterMS++)
    {
    double sumUS = 0.0; // sum of coefT for potential under-segmented regions
    double sumScoreUS = 0.0; // temporary sum of the (Tij x (Tij - 1)) for RA score
    
    RegionSetType regionsOfGT;     // stores region indexes
    ObjectVectorType  objectsOfGT; // stores region pointers
         
    double tMS = static_cast<double>(m_CardRegMS[col]) * m_Threshold;
    IsColEmpty = true;
    iterGT = containerGT.begin();
    for(unsigned int row=0; row<m_NumberOfRegionsGT; row++, iterGT++)
      {
      double coefT = static_cast<double>(m_HooverMatrix(row, col));
      if(coefT < 0.5)
        {
        // the regions Ri and ^Rj have an empty intersection : we can jump to the next matrix cell
        continue;
        }
      else
        {
        IsColEmpty = false;
        }
     
      double tGT = static_cast<double>(m_CardRegGT[row]) * m_Threshold;
      // Looking for Under-Segmented regions
      if(coefT>=tGT)
        {
        otbDebugMacro(<< "3 coef[" << row << "," << col << "]=" << coefT << " #tGT=" << tGT << " #tMS=" << tMS << " -> USmaybe");
        regionsOfGT.insert(row);
        objectsOfGT.push_back(iterGT->second);
        sumUS += coefT;
        sumScoreUS += coefT * (coefT - 1.0);
        }
      } // end of line loop
    
    // US
    if(sumUS>=tMS)
      {
      if(regionsOfGT.size()==1)
        {
        otbDebugMacro(<< "CD already registered");
        }
      else if(regionsOfGT.size()>1) // Under Segmentation
        {
        LabelObjectType *regionMS = iterMS->second;
        double cardTotalUS = sumUS / m_Threshold;
        double scoreRA = 1.0 - sumScoreUS / (cardTotalUS * (cardTotalUS - 1.0));
        
        regionMS->SetAttribute(ATTRIBUTE_RA.c_str(), static_cast<AttributesValueType>(scoreRA));
        
        unsigned int indexUS=1;
        for(typename ObjectVectorType::iterator it=objectsOfGT.begin(); it!=objectsOfGT.end(); ++it)
          {
          LabelObjectType *regionGT = *it;
          std::ostringstream attribute;
          attribute << ATTRIBUTE_US << "_" << indexUS;
          
          regionMS->SetAttribute(attribute.str().c_str(), static_cast<AttributesValueType>(regionGT->GetLabel()));
          
          regionGT->SetAttribute(ATTRIBUTE_US.c_str(), static_cast<AttributesValueType>(regionMS->GetLabel()));
          regionGT->SetAttribute(ATTRIBUTE_RA.c_str(), static_cast<AttributesValueType>(scoreRA));
          indexUS++;
          }
        
        MSindices.insert(col);
        for(RegionSetType::iterator it=regionsOfGT.begin(); it!=regionsOfGT.end(); ++it)
          {
          GTindices.insert(*it);
          otbDebugMacro(<< *it << " ");
          }
        otbDebugMacro(<< "US " << col);
        }
      else
        {
        otbDebugMacro(<< "No GT region present in potential US instance.");
        }
      }
    // check for empty columns (MS region that doesn't intersect any GT region)
    if (IsColEmpty)
      {
      MSindices.insert(col);
      }
    } // end of column loop

  // check for Missed regions (unregistered regions in GT)
  iterGT = containerGT.begin();
  for(unsigned int i=0; i<m_NumberOfRegionsGT; ++i , ++iterGT)
    {
    if(GTindices.count(i)==0)
      {
      otbDebugMacro(<< "M " << i);
      LabelObjectType *regionGT = iterGT->second;
      regionGT->SetAttribute(ATTRIBUTE_M.c_str(), static_cast<AttributesValueType>(regionGT->GetLabel()));
      }
    }

  // check for Noise regions (unregistered regions in MS)
  iterMS = containerMS.begin();
  for(unsigned int i=0; i<m_NumberOfRegionsMS; ++i , ++iterMS)
    {
    if(MSindices.count(i)==0)
      {
      LabelObjectType *regionMS = iterMS->second;
      regionMS->SetAttribute(ATTRIBUTE_N.c_str(), static_cast<AttributesValueType>(regionMS->GetLabel()));
      }
    }
}


}
#endif
