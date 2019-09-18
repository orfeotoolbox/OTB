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

#ifndef otbOGRLayerStreamStitchingFilter_hxx
#define otbOGRLayerStreamStitchingFilter_hxx

#include "otbOGRLayerStreamStitchingFilter.h"
#include "itkContinuousIndex.h"

#include <iomanip>
#include "ogrsf_frmts.h"
#include <set>

namespace otb
{

template <class TImage>
OGRLayerStreamStitchingFilter<TImage>::OGRLayerStreamStitchingFilter() : m_Radius(2), m_OGRLayer(nullptr, false)
{
  m_StreamSize.Fill(0);
}

template <class TInputImage>
void OGRLayerStreamStitchingFilter<TInputImage>::SetInput(const InputImageType* input)
{
  this->Superclass::SetNthInput(0, const_cast<InputImageType*>(input));
}

template <class TInputImage>
const typename OGRLayerStreamStitchingFilter<TInputImage>::InputImageType* OGRLayerStreamStitchingFilter<TInputImage>::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
  {
    return nullptr;
  }

  return static_cast<const InputImageType*>(this->Superclass::GetInput(0));
}

template <class TInputImage>
void OGRLayerStreamStitchingFilter<TInputImage>::SetOGRLayer(const OGRLayerType& ogrLayer)
{
  m_OGRLayer = ogrLayer;
  this->Modified();
}

template <class TInputImage>
const typename OGRLayerStreamStitchingFilter<TInputImage>::OGRLayerType& OGRLayerStreamStitchingFilter<TInputImage>::GetOGRLayer(void) const
{
  return m_OGRLayer;
}

template <class TInputImage>
double OGRLayerStreamStitchingFilter<TInputImage>::GetLengthOGRGeometryCollection(OGRGeometryCollection* intersection)
{
  double dfLength = 0.0;
  for (int iGeom = 0; iGeom < intersection->getNumGeometries(); iGeom++)
  {
    OGRGeometry* geom = intersection->getGeometryRef(iGeom);
    switch (wkbFlatten(geom->getGeometryType()))
    {
    case wkbLinearRing:
    case wkbLineString:
      dfLength += ((OGRCurve*)geom)->get_Length();
      break;
    case wkbGeometryCollection:
      dfLength += GetLengthOGRGeometryCollection(dynamic_cast<OGRGeometryCollection*>(geom));
      break;
    default:
      break;
    }
  }
  return dfLength;
}
template <class TInputImage>
void OGRLayerStreamStitchingFilter<TInputImage>::ProcessStreamingLine(bool line, itk::ProgressReporter& progress)
{
  typename InputImageType::ConstPointer inputImage = this->GetInput();

  // compute the number of stream division in row and column
  SizeType     imageSize   = this->GetInput()->GetLargestPossibleRegion().GetSize();
  unsigned int nbRowStream = static_cast<unsigned int>(imageSize[1] / m_StreamSize[1] + 1);
  unsigned int nbColStream = static_cast<unsigned int>(imageSize[0] / m_StreamSize[0] + 1);

  /*unsigned long startReporter;
  unsigned long stopReporter;
  if (!line)
  {
    startReporter = 0;
    stopReporter = 50;
  }
  else
  {
    startReporter = 50;
    stopReporter = 100;
  }
  itk::ProgressReporter progress(this,0,2*nbRowStream*nbColStream,100,startReporter); */

  for (unsigned int x = 1; x <= nbColStream; x++)
  {
    OGRErr errStart = m_OGRLayer.ogr().StartTransaction();

    if (errStart != OGRERR_NONE)
    {
      itkExceptionMacro(<< "Unable to start transaction for OGR layer " << m_OGRLayer.ogr().GetName() << ".");
    }

    for (unsigned int y = 1; y <= nbRowStream; y++)
    {

      // Compute Stream line
      OGRLineString streamLine;
      itk::ContinuousIndex<double, 2> startIndex;
      itk::ContinuousIndex<double, 2> endIndex;
      if (!line)
      {
        // Treat vertical stream line
        startIndex[0] = static_cast<double>(m_StreamSize[0] * x) - 0.5;
        startIndex[1] = static_cast<double>(m_StreamSize[1] * (y - 1)) - 0.5;
        endIndex      = startIndex;
        endIndex[1] += static_cast<double>(m_StreamSize[1]);
      }
      else
      { // Treat horizontal stream line
        startIndex[0] = static_cast<double>(m_StreamSize[0] * (x - 1)) - 0.5;
        startIndex[1] = static_cast<double>(m_StreamSize[1] * y) - 0.5;
        endIndex      = startIndex;
        endIndex[0] += static_cast<double>(m_StreamSize[0]);
      }
      OriginType startPoint;
      inputImage->TransformContinuousIndexToPhysicalPoint(startIndex, startPoint);
      OriginType endPoint;
      inputImage->TransformContinuousIndexToPhysicalPoint(endIndex, endPoint);
      streamLine.addPoint(startPoint[0], startPoint[1]);
      streamLine.addPoint(endPoint[0], endPoint[1]);


      // First we get all the feature that intersect the streaming line of the Upper/left stream
      std::vector<FeatureStruct> upperStreamFeatureList;
      upperStreamFeatureList.clear();
      IndexType UpperLeftCorner;
      IndexType LowerRightCorner;

      if (!line)
      {
        // Treat Row stream
        // Compute the spatial filter of the upper stream
        UpperLeftCorner[0] = x * m_StreamSize[0] - 1 - m_Radius;
        UpperLeftCorner[1] = m_StreamSize[1] * (y - 1);

        LowerRightCorner[0] = m_StreamSize[0] * x - 1;
        LowerRightCorner[1] = m_StreamSize[1] * y - 1;
      }
      else
      { // Treat Column stream
        // Compute the spatial filter of the left stream
        UpperLeftCorner[0] = (x - 1) * m_StreamSize[0];
        UpperLeftCorner[1] = m_StreamSize[1] * y - 1 - m_Radius;

        LowerRightCorner[0] = m_StreamSize[0] * x - 1;
        LowerRightCorner[1] = m_StreamSize[1] * y - 1; //-1 to stop just before stream line
      }

      OriginType ulCorner;
      inputImage->TransformIndexToPhysicalPoint(UpperLeftCorner, ulCorner);
      OriginType lrCorner;
      inputImage->TransformIndexToPhysicalPoint(LowerRightCorner, lrCorner);

      m_OGRLayer.SetSpatialFilterRect(ulCorner[0], lrCorner[1], lrCorner[0], ulCorner[1]);

      std::set<unsigned int>       upperFIDs;
      OGRLayerType::const_iterator featIt = m_OGRLayer.begin();
      for (; featIt != m_OGRLayer.end(); ++featIt)
      {
        FeatureStruct s(m_OGRLayer.GetLayerDefn());
        s.feat     = *featIt;
        s.fusioned = false;
        upperStreamFeatureList.push_back(s);
        upperFIDs.insert((*featIt).GetFID());
      }

      // Do the same thing for the lower/right stream
      std::vector<FeatureStruct> lowerStreamFeatureList;
      lowerStreamFeatureList.clear();

      if (!line)
      {
        // Compute the spatial filter of the lower stream
        UpperLeftCorner[0] = x * m_StreamSize[0];
        UpperLeftCorner[1] = m_StreamSize[1] * (y - 1);

        LowerRightCorner[0] = m_StreamSize[0] * x + m_Radius;
        LowerRightCorner[1] = m_StreamSize[1] * y - 1;
      }
      else
      {
        // Compute the spatial filter of the right stream
        UpperLeftCorner[0] = (x - 1) * m_StreamSize[0];
        UpperLeftCorner[1] = m_StreamSize[1] * y;

        LowerRightCorner[0] = m_StreamSize[0] * x - 1;
        LowerRightCorner[1] = m_StreamSize[1] * y + m_Radius;
      }

      inputImage->TransformIndexToPhysicalPoint(UpperLeftCorner, ulCorner);
      inputImage->TransformIndexToPhysicalPoint(LowerRightCorner, lrCorner);

      m_OGRLayer.SetSpatialFilterRect(ulCorner[0], lrCorner[1], lrCorner[0], ulCorner[1]);

      for (featIt = m_OGRLayer.begin(); featIt != m_OGRLayer.end(); ++featIt)
      {
        if (upperFIDs.find((*featIt).GetFID()) == upperFIDs.end())
        {
          FeatureStruct s(m_OGRLayer.GetLayerDefn());
          s.feat     = *featIt;
          s.fusioned = false;
          lowerStreamFeatureList.push_back(s);
        }
      }

      unsigned int              nbUpperPolygons = upperStreamFeatureList.size();
      unsigned int              nbLowerPolygons = lowerStreamFeatureList.size();
      std::vector<FusionStruct> fusionList;
      fusionList.clear();
      for (unsigned int u = 0; u < nbUpperPolygons; u++)
      {
        for (unsigned int l = 0; l < nbLowerPolygons; l++)
        {
          FeatureStruct upper = upperStreamFeatureList[u];
          FeatureStruct lower = lowerStreamFeatureList[l];
          if (!(upper.feat == lower.feat))
          {
            if (ogr::Intersects(*upper.feat.GetGeometry(), *lower.feat.GetGeometry()))
            {
              ogr::UniqueGeometryPtr intersection2 = ogr::Intersection(*upper.feat.GetGeometry(), *lower.feat.GetGeometry());
              ogr::UniqueGeometryPtr intersection  = ogr::Intersection(*intersection2, streamLine);
              // ogr::UniqueGeometryPtr intersection = ogr::Intersection(*upper.feat.GetGeometry(),*lower.feat.GetGeometry());
              if (intersection)
              {
                FusionStruct fusion;
                fusion.indStream1 = u;
                fusion.indStream2 = l;
                fusion.overlap    = 0.;

                if (intersection->getGeometryType() == wkbPolygon)
                {
                  fusion.overlap = dynamic_cast<OGRPolygon*>(intersection.get())->get_Area();
                }
                else if (intersection->getGeometryType() == wkbMultiPolygon)
                {
                  fusion.overlap = dynamic_cast<OGRMultiPolygon*>(intersection.get())->get_Area();
                }
                else if (intersection->getGeometryType() == wkbGeometryCollection)
                {
                  fusion.overlap = dynamic_cast<OGRGeometryCollection*>(intersection.get())->get_Area();
                }
                else if (intersection->getGeometryType() == wkbLineString)
                {
                  fusion.overlap = dynamic_cast<OGRLineString*>(intersection.get())->get_Length();
                }
                else if (intersection->getGeometryType() == wkbMultiLineString)
                {
                  fusion.overlap = dynamic_cast<OGRMultiLineString*>(intersection.get())->get_Length();
                }

                /** -Wunused-variable
                long upperFID = upper.feat.GetFID();
                long lowerFID = lower.feat.GetFID();
                **/
                fusionList.push_back(fusion);
              }
            }
          }
        }
      }
      unsigned int fusionListSize = fusionList.size();
      std::sort(fusionList.begin(), fusionList.end(), SortFeature);
      for (unsigned int i = 0; i < fusionListSize; i++)
      {
        FeatureStruct upper = upperStreamFeatureList.at(fusionList.at(i).indStream1);
        FeatureStruct lower = lowerStreamFeatureList.at(fusionList.at(i).indStream2);
        if (!upper.fusioned && !lower.fusioned)
        {
          upperStreamFeatureList[fusionList[i].indStream1].fusioned = true;
          lowerStreamFeatureList[fusionList[i].indStream2].fusioned = true;
          ogr::UniqueGeometryPtr fusionPolygon                      = ogr::Union(*upper.feat.GetGeometry(), *lower.feat.GetGeometry());
          OGRFeatureType         fusionFeature(m_OGRLayer.GetLayerDefn());
          fusionFeature.SetGeometry(fusionPolygon.get());

          ogr::Field field = upper.feat[0];
          try
          {
            switch (field.GetType())
            {
            case OFTInteger64:
            {
              fusionFeature[0].SetValue(field.GetValue<GIntBig>());
              break;
            }
            default:
            {
              fusionFeature[0].SetValue(field.GetValue<int>());
            }
            }
            m_OGRLayer.CreateFeature(fusionFeature);
            m_OGRLayer.DeleteFeature(lower.feat.GetFID());
            m_OGRLayer.DeleteFeature(upper.feat.GetFID());
          }
          catch (itk::ExceptionObject& err)
          {
            otbWarningMacro(<< "An exception was caught during fusion: " << err);
          }
        }
      }

      // Update progress
      progress.CompletedPixel();

    } // end for x

    if (m_OGRLayer.ogr().TestCapability("Transactions"))
    {

      OGRErr errCommitX = m_OGRLayer.ogr().CommitTransaction();
      if (errCommitX != OGRERR_NONE)
      {
        itkExceptionMacro(<< "Unable to commit transaction for OGR layer " << m_OGRLayer.ogr().GetName() << ".");
      }
    }
  } // end for y
}
template <class TImage>
void OGRLayerStreamStitchingFilter<TImage>::GenerateData(void)
{
  if (!m_OGRLayer)
  {
    itkExceptionMacro(<< "Input OGR layer is null!");
  }

  this->InvokeEvent(itk::StartEvent());

  typename InputImageType::ConstPointer inputImage = this->GetInput();

  // compute the number of stream division in row and column
  SizeType     imageSize   = this->GetInput()->GetLargestPossibleRegion().GetSize();
  unsigned int nbRowStream = static_cast<unsigned int>(imageSize[1] / m_StreamSize[1] + 1);
  unsigned int nbColStream = static_cast<unsigned int>(imageSize[0] / m_StreamSize[0] + 1);

  itk::ProgressReporter progress(this, 0, 2 * nbRowStream * nbColStream, 100, 0);
  // Process column
  this->ProcessStreamingLine(false, progress);
  // Process row
  this->ProcessStreamingLine(true, progress);

  this->InvokeEvent(itk::EndEvent());
}


} // end namespace otb

#endif
