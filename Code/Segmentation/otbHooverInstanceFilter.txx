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

/** Constructor */
template <class TLabelMap>
HooverInstanceFilter<TLabelMap>
::HooverInstanceFilter() : m_NumberOfRegionsGT(0), m_NumberOfRegionsMS(0), m_Threshold(0.8), m_UseExtendedAttributes(false)
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(2);
  typename LabelMapType::Pointer secondOutput = LabelMapType::New();
  this->AddOutput(secondOutput);
  
  m_HooverMatrix.SetSize(0, 0);
  m_CardRegGT.SetSize(0);
  m_CardRegMS.SetSize(0);
  m_LabelsGT.resize(0);
  
  m_MeanRC = static_cast<AttributesValueType>(0);
  m_MeanRF = static_cast<AttributesValueType>(0);
  m_MeanRA = static_cast<AttributesValueType>(0);
  m_MeanRM = static_cast<AttributesValueType>(0);
  m_MeanRN = static_cast<AttributesValueType>(0);
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
TLabelMap* HooverInstanceFilter<TLabelMap>
::GetMachineSegmentationLabelMap()
{
  return const_cast<TLabelMap*> (this->GetInput(1));
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

    ConstIteratorType  it = ConstIteratorType( inputMS );

    while( !it.IsAtEnd() )
      {
      const LabelObjectType * labeObject = it.GetLabelObject();

      assert( labeObject != NULL );
      assert( labeObject->GetLabel() == it.GetLabel() );

      typename LabelObjectType::Pointer newLabelObject = LabelObjectType::New();
      newLabelObject->CopyAllFrom( labeObject );
      
      outputMS->AddLabelObject( newLabelObject );
      ++it;
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
  
  IteratorType  iter = IteratorType( this->GetMachineSegmentationLabelMap() );
  
  typename LabelObjectType::Pointer blankRegion;
  
  while ( !iter.IsAtEnd() )
    {
    LabelObjectType *regionMS = iter.GetLabelObject();
    m_CardRegMS[i] = regionMS->Size();
    if (m_CardRegMS[i] == 0)
      {
      otbWarningMacro("Region "<<i<<" in machine segmentation label map is empty");
      }
    // reset any Hoover attribute already present
    regionMS->SetAttribute(GetNameFromAttribute(ATTRIBUTE_RC), 0.0);
    regionMS->SetAttribute(GetNameFromAttribute(ATTRIBUTE_RF), 0.0);
    regionMS->SetAttribute(GetNameFromAttribute(ATTRIBUTE_RA), 0.0);
    regionMS->SetAttribute(GetNameFromAttribute(ATTRIBUTE_RN), 0.0);
    
    if (m_UseExtendedAttributes)
      {
      blankRegion = LabelObjectType::New();
      blankRegion->SetLabel(regionMS->GetLabel());
      std::vector< std::string > attKeys = regionMS->GetAvailableAttributes();
      for (unsigned int k=0; k<attKeys.size(); k++)
        {
        if (attKeys[k].find("HooverInstance_Ext_") == 0)
          {
          continue;
          }
        else
          {
          blankRegion->SetAttribute(attKeys[k].c_str(), regionMS->GetAttribute(attKeys[k].c_str()));
          }
        }
      regionMS->CopyAttributesFrom(blankRegion);
      }
    i++;
    ++iter;
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
  
  // reset any Hoover attribute already present
  labelObject->SetAttribute(GetNameFromAttribute(ATTRIBUTE_RC), 0.0);
  labelObject->SetAttribute(GetNameFromAttribute(ATTRIBUTE_RF), 0.0);
  labelObject->SetAttribute(GetNameFromAttribute(ATTRIBUTE_RA), 0.0);
  labelObject->SetAttribute(GetNameFromAttribute(ATTRIBUTE_RM), 0.0);
  
  if (m_UseExtendedAttributes)
    {
    typename LabelObjectType::Pointer blankRegion;
    blankRegion = LabelObjectType::New();
    blankRegion->SetLabel(labelObject->GetLabel());
    std::vector< std::string > attKeys = labelObject->GetAvailableAttributes();
    for (unsigned int k=0; k<attKeys.size(); k++)
      {
      if (attKeys[k].find("HooverInstance_Ext_") == 0)
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
}

template <class TLabelMap>
void HooverInstanceFilter<TLabelMap>
::AfterThreadedGenerateData()
{
  LabelMapType* outGT = this->GetOutput(0);
  LabelMapType* outMS = this->GetOutput(1);
  
  // Iterators on label object container (to gain efficiency when accessing them)
  IteratorType  iterGT = IteratorType( outGT );
  IteratorType  iterMS = IteratorType( outMS );

  // Set of classified regions
  RegionSetType GTindices;
  RegionSetType MSindices;
  
  // flags to detect empty rows or columns
  bool IsRowEmpty;
  bool IsColEmpty;
  
  // temporary buffers to compute average scores
  double bufferRC = 0.0;
  double bufferRF = 0.0;
  double bufferRA = 0.0;
  double bufferRM = 0.0;
  double bufferRN = 0.0;
  double areaGT = 0.0;
  double areaMS = 0.0;
  
  // first pass : loop on GT regions first
  for(unsigned int row=0; row<m_NumberOfRegionsGT; row++, iterGT++)
    {
    double sumOS = 0.0; // sum of coefT for potential over-segmented regions
    double sumScoreRF = 0.0; // temporary sum  of (Tij x (Tij - 1)) terms for the RF score
    RegionSetType     regionsOfMS; // stores region indexes
    ObjectVectorType  objectsOfMS; // stores region pointers
    
    double tGT = static_cast<double>(m_CardRegGT[row]) * m_Threshold; // card Ri x t
    IsRowEmpty = true;
    iterMS.GoToBegin();
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
          
          LabelObjectType *regionGT = iterGT.GetLabelObject();
          LabelObjectType *regionMS = iterMS.GetLabelObject();
          double scoreRC = m_Threshold * (std::min(coefT / tGT, coefT / tMS));
          bufferRC += scoreRC * static_cast<double>(m_CardRegGT[row]);
          
          regionGT->SetAttribute(GetNameFromAttribute(ATTRIBUTE_RC), static_cast<AttributesValueType>(scoreRC));
          regionMS->SetAttribute(GetNameFromAttribute(ATTRIBUTE_RC), static_cast<AttributesValueType>(scoreRC));
          
          if (m_UseExtendedAttributes)
            {
            regionGT->SetAttribute(GetNameFromAttribute(ATTRIBUTE_CD), static_cast<AttributesValueType>(regionMS->GetLabel()));
            regionMS->SetAttribute(GetNameFromAttribute(ATTRIBUTE_CD), static_cast<AttributesValueType>(regionGT->GetLabel()));
            }
          
          GTindices.insert(row);
          MSindices.insert(col);
          }
        else
          {
          otbDebugMacro(<< "2 coef[" << row << "," << col << "]=" << coefT << " #tGT=" << tGT << " #tMS=" << tMS << " -> OSmaybe");
          }
        objectsOfMS.push_back(iterMS.GetLabelObject()); // candidate region for over-segmentation
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
        LabelObjectType *regionGT = iterGT.GetLabelObject();
        
        double cardRegGT = static_cast<double>(m_CardRegGT[row]);
        double scoreRF = 1.0 - sumScoreRF / (cardRegGT * (cardRegGT - 1.0));
        bufferRF += scoreRF * cardRegGT;
        
        regionGT->SetAttribute(GetNameFromAttribute(ATTRIBUTE_RF), static_cast<AttributesValueType>(scoreRF));
        
        unsigned int indexOS=1;
        for(typename ObjectVectorType::iterator it=objectsOfMS.begin(); it!=objectsOfMS.end(); ++it)
          {
          LabelObjectType *regionMS = *it;
          std::ostringstream attribute;
          attribute << ATTRIBUTE_OS << "_" << indexOS;
          
          regionMS->SetAttribute(GetNameFromAttribute(ATTRIBUTE_RF), static_cast<AttributesValueType>(scoreRF));
          
          if (m_UseExtendedAttributes)
            {
            regionGT->SetAttribute(attribute.str().c_str(), static_cast<AttributesValueType>(regionMS->GetLabel()));
            regionMS->SetAttribute(GetNameFromAttribute(ATTRIBUTE_OS), static_cast<AttributesValueType>(regionGT->GetLabel()));
            }
          
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
    else
      {
      areaGT += static_cast<double>(m_CardRegGT[row]);
      }
    } // end of line loop

  // second pass : loop on MS regions first
  iterMS.GoToBegin();
  for(unsigned int col=0; col<m_NumberOfRegionsMS; col++, iterMS++)
    {
    double sumUS = 0.0; // sum of coefT for potential under-segmented regions
    double sumScoreUS = 0.0; // temporary sum of the (Tij x (Tij - 1)) for RA score
    double sumCardUS = 0.0; // temporary sum of under segmented region sizes
    
    RegionSetType regionsOfGT;     // stores region indexes
    ObjectVectorType  objectsOfGT; // stores region pointers
         
    double tMS = static_cast<double>(m_CardRegMS[col]) * m_Threshold;
    IsColEmpty = true;
    iterGT.GoToBegin();
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
        objectsOfGT.push_back(iterGT.GetLabelObject());
        sumUS += coefT;
        sumScoreUS += coefT * (coefT - 1.0);
        sumCardUS += static_cast<double>(m_CardRegGT[row]);
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
        LabelObjectType *regionMS = iterMS.GetLabelObject();
        double scoreRA = 1.0 - sumScoreUS / (sumCardUS * (sumCardUS - 1.0));
        bufferRA += scoreRA * sumCardUS;
        
        regionMS->SetAttribute(GetNameFromAttribute(ATTRIBUTE_RA), static_cast<AttributesValueType>(scoreRA));
        
        unsigned int indexUS=1;
        for(typename ObjectVectorType::iterator it=objectsOfGT.begin(); it!=objectsOfGT.end(); ++it)
          {
          LabelObjectType *regionGT = *it;
          std::ostringstream attribute;
          attribute << ATTRIBUTE_US << "_" << indexUS;
          
          regionGT->SetAttribute(GetNameFromAttribute(ATTRIBUTE_RA), static_cast<AttributesValueType>(scoreRA));
          
          if (m_UseExtendedAttributes)
            {
            regionMS->SetAttribute(attribute.str(), static_cast<AttributesValueType>(regionGT->GetLabel()));
            regionGT->SetAttribute(GetNameFromAttribute(ATTRIBUTE_US), static_cast<AttributesValueType>(regionMS->GetLabel()));
            }
            
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
    else
      {
      areaMS += static_cast<double>(m_CardRegMS[col]);
      }
    } // end of column loop

  // check for Missed regions (unregistered regions in GT)
  iterGT.GoToBegin();
  for(unsigned int i=0; i<m_NumberOfRegionsGT; ++i , ++iterGT)
    {
    if(GTindices.count(i)==0)
      {
      otbDebugMacro(<< "M " << i);
      LabelObjectType *regionGT = iterGT.GetLabelObject();
      
      bufferRM += static_cast<double>(m_CardRegGT[i]);
      
      regionGT->SetAttribute(GetNameFromAttribute(ATTRIBUTE_RM), 1.0);
      
      if (m_UseExtendedAttributes)
        {
        regionGT->SetAttribute(GetNameFromAttribute(ATTRIBUTE_M),
                               static_cast<AttributesValueType>(regionGT->GetLabel()));
        }
      
      }
    }

  // check for Noise regions (unregistered regions in MS)
  iterMS.GoToBegin();
  for(unsigned int i=0; i<m_NumberOfRegionsMS; ++i , ++iterMS)
    {
    if(MSindices.count(i)==0)
      {
      LabelObjectType *regionMS = iterMS.GetLabelObject();
      
      bufferRN += static_cast<double>(m_CardRegMS[i]);
      
      regionMS->SetAttribute(GetNameFromAttribute(ATTRIBUTE_RN), 1.0);
      
      if (m_UseExtendedAttributes)
        {
        regionMS->SetAttribute(GetNameFromAttribute(ATTRIBUTE_N),
                               static_cast<AttributesValueType>(regionMS->GetLabel()));
        }
      
      }
    }
    
  // Compute average scores
  m_MeanRC = static_cast<AttributesValueType>(bufferRC / areaGT);
  m_MeanRF = static_cast<AttributesValueType>(bufferRF / areaGT);
  m_MeanRA = static_cast<AttributesValueType>(bufferRA / areaGT);
  m_MeanRM = static_cast<AttributesValueType>(bufferRM / areaGT);
  m_MeanRN = static_cast<AttributesValueType>(bufferRN / areaMS);
}


}
#endif
