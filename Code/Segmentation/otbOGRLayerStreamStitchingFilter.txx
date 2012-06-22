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
#ifndef __otbOGRLayerStreamStitchingFilter_txx
#define __otbOGRLayerStreamStitchingFilter_txx

#include "otbOGRLayerStreamStitchingFilter.h"

#include <iomanip>
#include "ogrsf_frmts.h"
#include "itkTimeProbe.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"

namespace otb
{

template<class TImage>
OGRLayerStreamStitchingFilter<TImage>
::OGRLayerStreamStitchingFilter() : m_Radius(2), m_OGRLayer(NULL)
{
   m_StreamSize.Fill(0);
}

template <class TInputImage>
void
OGRLayerStreamStitchingFilter<TInputImage>
::SetInput(const InputImageType *input)
{
  this->Superclass::SetNthInput(0, const_cast<InputImageType *>(input));
}

template <class TInputImage>
const typename OGRLayerStreamStitchingFilter<TInputImage>
::InputImageType *
OGRLayerStreamStitchingFilter<TInputImage>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const InputImageType *>(this->Superclass::GetInput(0));
}

template<class TInputImage>
void
OGRLayerStreamStitchingFilter<TInputImage>
::SetOGRLayer( const OGRLayerType& ogrLayer )
{
  m_OGRLayer = ogrLayer;
  this->Modified();
}

template<class TInputImage>
const typename OGRLayerStreamStitchingFilter<TInputImage>::OGRLayerType &
OGRLayerStreamStitchingFilter<TInputImage>
::GetOGRLayer( void ) const
{
   return m_OGRLayer;
}

template<class TInputImage>
double
OGRLayerStreamStitchingFilter<TInputImage>
::GetLengthOGRGeometryCollection( OGRGeometryCollection * intersection )
{
  double dfLength = 0.0;
  for( int iGeom = 0; iGeom < intersection->getNumGeometries(); iGeom++ )
    {
      OGRGeometry* geom = intersection->getGeometryRef(iGeom);
      switch( wkbFlatten(geom->getGeometryType()) )
       {
       case wkbLinearRing:
       case wkbLineString:
         dfLength += ((OGRCurve *) geom)->get_Length();
         break;
       case wkbGeometryCollection:
         dfLength += GetLengthOGRGeometryCollection(dynamic_cast<OGRGeometryCollection *> (geom));
         break;
       default:
         break;
       }
    }
  return dfLength;
}
template<class TInputImage>
void
OGRLayerStreamStitchingFilter<TInputImage>
::ProcessStreamingLine( bool line )
{
   typename InputImageType::ConstPointer inputImage = this->GetInput();

   //compute the number of stream division in row and column
   SizeType imageSize = this->GetInput()->GetLargestPossibleRegion().GetSize();
   unsigned int nbRowStream = static_cast<unsigned int>(imageSize[1] / m_StreamSize[1] + 1);
   unsigned int nbColStream = static_cast<unsigned int>(imageSize[0] / m_StreamSize[0] + 1);

   itk::ProgressReporter progress(this,0,nbRowStream*nbColStream);

   for(unsigned int x=1; x<=nbColStream; x++)
   {
      m_OGRLayer.ogr().StartTransaction();
      for(unsigned int y=1; y<=nbRowStream; y++)
      {
         //First we get all the feature that intersect the streaming line of the Upper/left stream
         std::vector<FeatureStruct> upperStreamFeatureList;
         upperStreamFeatureList.clear();
         IndexType  UpperLeftCorner;
         IndexType  LowerRightCorner;

         if(!line)
         {
            // Treat Row stream
            //Compute the spatial filter of the upper stream
            UpperLeftCorner[0] = x*m_StreamSize[0] - 1 - m_Radius;
            UpperLeftCorner[1] = m_StreamSize[1]*(y-1) + 1;

            LowerRightCorner[0] = m_StreamSize[0]*x - 1 - 1;
            LowerRightCorner[1] = m_StreamSize[1]*y - 1 - 1;
         }
         else
         {  // Treat Column stream
            //Compute the spatial filter of the left stream
            UpperLeftCorner[0] = (x-1)*m_StreamSize[0] + 1;
            UpperLeftCorner[1] = m_StreamSize[1]*y - 1 - 1 - m_Radius;

            LowerRightCorner[0] = m_StreamSize[0]*x - 1 - 1;
            LowerRightCorner[1] = m_StreamSize[1]*y - 1 - 1; //-1 to stop just before stream line
         }

         OriginType  ulCorner;
         inputImage->TransformIndexToPhysicalPoint(UpperLeftCorner, ulCorner);
         OriginType  lrCorner;
         inputImage->TransformIndexToPhysicalPoint(LowerRightCorner, lrCorner);

         m_OGRLayer.SetSpatialFilterRect(ulCorner[0],lrCorner[1],lrCorner[0],ulCorner[1]);

         OGRLayerType::const_iterator featIt = m_OGRLayer.begin();
         for(; featIt!=m_OGRLayer.end(); ++featIt)
         {
            FeatureStruct s(m_OGRLayer.GetLayerDefn());
            s.feat = *featIt;
            s.fusioned = false;
            upperStreamFeatureList.push_back(s);
         }

         //Do the same thing for the lower/right stream
         std::vector<FeatureStruct> lowerStreamFeatureList;
         lowerStreamFeatureList.clear();

         if(!line)
         {
            //Compute the spatial filter of the lower stream
            UpperLeftCorner[0] = x*m_StreamSize[0] + 1;
            UpperLeftCorner[1] = m_StreamSize[1]*(y-1) + 1;

            LowerRightCorner[0] = m_StreamSize[0]*x + 1 + m_Radius;
            LowerRightCorner[1] = m_StreamSize[1]*y - 1 - 1;
         }
         else
         {
            //Compute the spatial filter of the right stream
            UpperLeftCorner[0] = (x-1)*m_StreamSize[0] + 1;
            UpperLeftCorner[1] = m_StreamSize[1]*y + 1;

            LowerRightCorner[0] = m_StreamSize[0]*x - 1 - 1;
            LowerRightCorner[1] = m_StreamSize[1]*y + 1 + m_Radius;
         }

         inputImage->TransformIndexToPhysicalPoint(UpperLeftCorner, ulCorner);
         inputImage->TransformIndexToPhysicalPoint(LowerRightCorner, lrCorner);

         m_OGRLayer.SetSpatialFilterRect(ulCorner[0],lrCorner[1],lrCorner[0],ulCorner[1]);

         for(featIt = m_OGRLayer.begin(); featIt!=m_OGRLayer.end(); ++featIt)
         {
            FeatureStruct s(m_OGRLayer.GetLayerDefn());
            s.feat = *featIt;
            s.fusioned = false;
            lowerStreamFeatureList.push_back(s);
         }

         unsigned int nbUpperPolygons = upperStreamFeatureList.size();
         unsigned int nbLowerPolygons = lowerStreamFeatureList.size();
         std::vector<FusionStruct> fusionList;
         fusionList.clear();
         for(unsigned int u=0; u<nbUpperPolygons; u++)
         {
            for(unsigned int l=0; l<nbLowerPolygons; l++)
            {
               FeatureStruct upper = upperStreamFeatureList[u];
               FeatureStruct lower = lowerStreamFeatureList[l];
               if (ogr::Intersects(*upper.feat.GetGeometry(), *lower.feat.GetGeometry()))
               {
                  ogr::UniqueGeometryPtr intersection = ogr::Intersection(*upper.feat.GetGeometry(),*lower.feat.GetGeometry());
                  if (intersection)
                  {
                     FusionStruct fusion;
                     fusion.indStream1 = u;
                     fusion.indStream2 = l;
                     fusion.overlap = 0.;

                     if(intersection->getGeometryType() == wkbPolygon)
                     {
                        fusion.overlap = dynamic_cast<OGRPolygon *>(intersection.get())->get_Area();
                     }
                     else if(intersection->getGeometryType() == wkbMultiPolygon)
                     {
                        fusion.overlap = dynamic_cast<OGRMultiPolygon *>(intersection.get())->get_Area();
                     }
                     else if(intersection->getGeometryType() == wkbGeometryCollection)
                     {
                        fusion.overlap = dynamic_cast<OGRGeometryCollection *>(intersection.get())->get_Area();
                     }
                     else if(intersection->getGeometryType() == wkbLineString)
                     {
                        fusion.overlap = dynamic_cast<OGRLineString *>(intersection.get())->get_Length();
                     }
                     else if (intersection->getGeometryType() == wkbMultiLineString)
                     {
                        #if(GDAL_VERSION_NUM < 1800)
                     fusion.overlap = GetLengthOGRGeometryCollection(dynamic_cast<OGRGeometryCollection *> (intersection.get()));
                        #else
                     fusion.overlap = dynamic_cast<OGRMultiLineString *>(intersection.get())->get_Length();
                        #endif
                   }


                     long upperFID = upper.feat.GetFID();
                     long lowerFID = lower.feat.GetFID();
                     fusionList.push_back(fusion);
                  }
               }
            }
         }
         unsigned int fusionListSize = fusionList.size();
         std::sort(fusionList.begin(),fusionList.end(),SortFeature);
         for(unsigned int i=0; i<fusionListSize; i++)
         {
            FeatureStruct upper = upperStreamFeatureList.at(fusionList.at(i).indStream1);
            FeatureStruct lower = lowerStreamFeatureList.at(fusionList.at(i).indStream2);
            if( !upper.fusioned && !lower.fusioned )
            {
               upperStreamFeatureList[fusionList[i].indStream1].fusioned = true;
               lowerStreamFeatureList[fusionList[i].indStream2].fusioned = true;
               ogr::UniqueGeometryPtr fusionPolygon = ogr::Union(*upper.feat.GetGeometry(),*lower.feat.GetGeometry());
               OGRFeatureType fusionFeature(m_OGRLayer.GetLayerDefn());
               fusionFeature.SetGeometry( fusionPolygon.get() );

               ogr::Field field = upper.feat[0];
               try
                 {
                 fusionFeature[0].SetValue(field.GetValue<int>());
                 m_OGRLayer.CreateFeature(fusionFeature);
                 m_OGRLayer.DeleteFeature(lower.feat.GetFID());
                 m_OGRLayer.DeleteFeature(upper.feat.GetFID());
                 }
               catch(itk::ExceptionObject& err)
                 {
                 otbDebugMacro(<<"An exception was caught during fusion: "<<err);
                 }
            }
         }
      } //end for x
      m_OGRLayer.ogr().CommitTransaction();

      // Update progress
      progress.CompletedPixel();
   } //end for y
   m_OGRLayer.ogr().CommitTransaction();

}

template<class TImage>
void
OGRLayerStreamStitchingFilter<TImage>
::GenerateData(void)
{
  if(!m_OGRLayer)
    {
    itkExceptionMacro(<<"Input OGR layer is null!");
    }

   //Process column
   this->ProcessStreamingLine(false);
   //Process row
   this->ProcessStreamingLine(true);
}


} // end namespace otb

#endif
