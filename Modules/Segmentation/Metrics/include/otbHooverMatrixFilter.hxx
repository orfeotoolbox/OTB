/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbHooverMatrixFilter_hxx
#define otbHooverMatrixFilter_hxx

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
