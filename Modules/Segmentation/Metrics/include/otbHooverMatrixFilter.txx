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
#ifndef __otbHooverMatrixFilter_txx
#define __otbHooverMatrixFilter_txx

#include "otbHooverMatrixFilter.h"

namespace otb
{

/** Constructor */
template <class TLabelMap>
HooverMatrixFilter<TLabelMap>
::HooverMatrixFilter() : m_NumberOfRegionsGT(0), m_NumberOfRegionsMS(0)
{
  this->SetNumberOfRequiredInputs(2);
  m_Matrix.SetSize(0, 0);
}

/** Set the ground truth label map */
template <class TLabelMap>
void HooverMatrixFilter<TLabelMap>
::SetGroundTruthLabelMap(const LabelMapType *gt)
{
  this->SetInput(0, gt);
}

/** Set the machine segmentation label map */
template <class TLabelMap>
void HooverMatrixFilter<TLabelMap>
::SetMachineSegmentationLabelMap(const LabelMapType *ms)
{
  this->SetInput(1, ms);
}

/** Get the ground truth label map */
template <class TLabelMap>
const TLabelMap* HooverMatrixFilter<TLabelMap>
::GetGroundTruthLabelMap()
{
  return this->GetInput(0);
}

/** Get the machine segmentation label map */
template <class TLabelMap>
const TLabelMap* HooverMatrixFilter<TLabelMap>
::GetMachineSegmentationLabelMap()
{
  return this->GetInput(1);
}

template <class TLabelMap>
void HooverMatrixFilter<TLabelMap>
::BeforeThreadedGenerateData()
{
  // first : call superclass method
  Superclass::BeforeThreadedGenerateData();

  // Set the matrix size
  m_NumberOfRegionsGT = this->GetGroundTruthLabelMap()->GetNumberOfLabelObjects();
  m_NumberOfRegionsMS = this->GetMachineSegmentationLabelMap()->GetNumberOfLabelObjects();
  m_LabelsGT = this->GetGroundTruthLabelMap()->GetLabels();
  m_Matrix.SetSize(m_NumberOfRegionsGT , m_NumberOfRegionsMS);
  m_Matrix.Fill(0);
}

template <class TLabelMap>
void HooverMatrixFilter<TLabelMap>
::ThreadedProcessLabelObject( LabelObjectType * labelObject )
{


  unsigned long currentRegionMS = 0;
  unsigned long currentRegionGT = 0;
  // find the index of the current GT region
  LabelType currentLabelGT = labelObject->GetLabel();
  for (unsigned long k=0; k<m_NumberOfRegionsGT; k++)
    {
    if (currentLabelGT == m_LabelsGT[k])
      {
      currentRegionGT = k;
      break;
      }
    }

  // loop over the given region
  typedef typename LabelObjectType::ConstLineIterator IteratorType;
  IteratorType lit = IteratorType( labelObject );
  while ( !lit.IsAtEnd() )
    {
    IndexType idx = lit.GetLine().GetIndex();
    unsigned long length = lit.GetLine().GetLength();
    for (unsigned long i=0; i<length; i++)
      {
      // find the corresponding label in the ms label map
      // if found : ++ in the cell [n, p]
      for (unsigned long j = currentRegionMS;
           j < (currentRegionMS + m_NumberOfRegionsMS);
           j++)
        {
        // TODO : optimize here : call GetLabelObjet(label) would be faster
        const LabelObjectType *regionMS = this->GetMachineSegmentationLabelMap()->GetNthLabelObject(j%m_NumberOfRegionsMS);
        if (regionMS->HasIndex(idx))
          {
          currentRegionMS = j%m_NumberOfRegionsMS;
          m_Matrix(currentRegionGT, currentRegionMS)++;
          break;
          }
        }
      idx[0]++;
      }
    ++lit;
    }
}

}
#endif
