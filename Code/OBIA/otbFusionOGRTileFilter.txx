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
#ifndef __otbFusionOGRTileFilter_txx
#define __otbFusionOGRTileFilter_txx

#include "otbFusionOGRTileFilter.h"

#include "ogrsf_frmts.h"
#include <iomanip>
#include "itkTimeProbe.h"


namespace otb
{

template<class TImage>
FusionOGRTileFilter<TImage>
::FusionOGRTileFilter() : m_InputFileName("")
{
   OGRRegisterAll();
   m_StreamSize.Fill(0);
   m_Radius = 2;

}

template <class TInputImage>
void
FusionOGRTileFilter<TInputImage>
::SetInput(const InputImageType *input)
{
  this->Superclass::SetNthInput(0, const_cast<InputImageType *>(input));
}

template <class TInputImage>
const typename FusionOGRTileFilter<TInputImage>
::InputImageType *
FusionOGRTileFilter<TInputImage>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const InputImageType *>(this->Superclass::GetInput(0));
}

template<class TImage>
void
FusionOGRTileFilter<TImage>
::GenerateData(void)
{
   
   typename InputImageType::ConstPointer inputImage = this->GetInput();

   OGRDataSource * inputDataSource = OGRSFDriverRegistrar::Open(this->m_InputFileName.c_str(), TRUE);
   OGRLayer * inputLayer = inputDataSource->GetLayer(0);

   unsigned int nbFeature = 0;
   unsigned int f = 0;
   
   //compute the number of stream division in row and column
   SizeType imageSize = this->GetInput()->GetLargestPossibleRegion().GetSize();
   unsigned int nbRowStream = static_cast<unsigned int>(imageSize[1] / m_StreamSize[1] + 1);
   unsigned int nbColStream = static_cast<unsigned int>(imageSize[0] / m_StreamSize[0] + 1);
   
   //process column
   unsigned int nbTransaction = 0;
   inputLayer->StartTransaction();
   for(unsigned int x=1; x<=nbColStream; x++)
   {
      for(unsigned int y=1; y<=nbRowStream; y++)
      {
         /*std::cout<< " column tile number : " << x*y <<std::endl;
         itk::TimeProbe chrono;
         chrono.Start(); */
  
         //First compute the intersection between polygons and the streaming line (upper stream)
         std::vector<FeatureStruct> upperStreamFeatureList;
         upperStreamFeatureList.clear();
         
         //Compute the spatial filter of the upper stream
         IndexType  UpperLeftCorner;
         UpperLeftCorner[0] = x*m_StreamSize[0] - 1 - m_Radius;
         UpperLeftCorner[1] = m_StreamSize[1]*(y-1) + 1;
         
         IndexType  LowerRightCorner;
         LowerRightCorner[0] = m_StreamSize[0]*x - 1 - 1;
         LowerRightCorner[1] = m_StreamSize[1]*y - 1 - 1;

         OriginType  ulCorner;
         inputImage->TransformIndexToPhysicalPoint(UpperLeftCorner, ulCorner);
         OriginType  lrCorner;
         inputImage->TransformIndexToPhysicalPoint(LowerRightCorner, lrCorner);
         
         inputLayer->SetSpatialFilterRect(ulCorner[0],lrCorner[1],lrCorner[0],ulCorner[1]);
         
         inputLayer->ResetReading();
         nbFeature = inputLayer->GetFeatureCount(true);
         
         f = 0;
         while (f<nbFeature)
         {
            OGRFeature * feature = NULL;
            feature = inputLayer->GetNextFeature();
            
            FeatureStruct s;
            s.feat = feature;
            s.fusioned = false;
            upperStreamFeatureList.push_back(s);

            f++;
            
         }
         
         //Do the same thing for the lower stream
         std::vector<FeatureStruct> lowerStreamFeatureList;
         lowerStreamFeatureList.clear();
         
         //Compute the spatial filter of the lower stream
         UpperLeftCorner[0] = x*m_StreamSize[0] + 1;
         UpperLeftCorner[1] = m_StreamSize[1]*(y-1) + 1;
         
         LowerRightCorner[0] = m_StreamSize[0]*x + 1 + m_Radius;
         LowerRightCorner[1] = m_StreamSize[1]*y - 1 - 1;
         
         inputImage->TransformIndexToPhysicalPoint(UpperLeftCorner, ulCorner);
         inputImage->TransformIndexToPhysicalPoint(LowerRightCorner, lrCorner);
         
         inputLayer->SetSpatialFilterRect(ulCorner[0],lrCorner[1],lrCorner[0],ulCorner[1]);
         
         inputLayer->ResetReading();
         nbFeature = inputLayer->GetFeatureCount(true);
         
         f = 0;
         while (f<nbFeature)
         {
            OGRFeature * feature = NULL;
            feature = inputLayer->GetNextFeature();

            FeatureStruct s;
            s.feat = feature;
            s.fusioned = false;
            lowerStreamFeatureList.push_back(s);

            f++;
         }
      
         unsigned int nbUpperPolygons = upperStreamFeatureList.size();
         unsigned int nbLowerPolygons = lowerStreamFeatureList.size();
         std::vector<FusionStruct> fusionList;
         fusionList.clear();
         for(unsigned int u=0; u<nbUpperPolygons; u++)
         {
            for(unsigned int l=0; l<nbLowerPolygons; l++)
            {
               FeatureStruct upper = upperStreamFeatureList.at(u);
               FeatureStruct lower = lowerStreamFeatureList.at(l);
               if (upper.feat->GetGeometryRef()->Intersects(lower.feat->GetGeometryRef()))
               {

                  OGRGeometry * intersection;
                  intersection = upper.feat->GetGeometryRef()->Intersection(lower.feat->GetGeometryRef());
                  if (intersection != NULL)
                  {
                     FusionStruct fusion;
                     fusion.indStream1 = u;
                     fusion.indStream2 = l;
                     fusion.overlap = 0.;
                     
                     if(intersection->getGeometryType() == wkbPolygon)
                     {
                        fusion.overlap = dynamic_cast<OGRPolygon *>(intersection)->get_Area();
                     }
                     else if(intersection->getGeometryType() == wkbMultiPolygon)
                     {
                        fusion.overlap = dynamic_cast<OGRMultiPolygon *>(intersection)->get_Area();
                     }
                     else if(intersection->getGeometryType() == wkbGeometryCollection)
                     {
                        fusion.overlap = dynamic_cast<OGRGeometryCollection *>(intersection)->get_Area();
                     }
                     else if(intersection->getGeometryType() == wkbLineString)
                     {
                        fusion.overlap = dynamic_cast<OGRLineString *>(intersection)->get_Length();
                     }
                     else if (intersection->getGeometryType() == wkbMultiLineString)
                     {
                        fusion.overlap = dynamic_cast<OGRMultiLineString *>(intersection)->get_Length();
                     }
                     
                     long upperFID = upper.feat->GetFID();
                     long lowerFID = lower.feat->GetFID();
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
               upperStreamFeatureList.at(fusionList.at(i).indStream1).fusioned = true;
               lowerStreamFeatureList.at(fusionList.at(i).indStream2).fusioned = true;
               OGRGeometry * fusionPolygon;
               fusionPolygon = upper.feat->GetGeometryRef()->Union(lower.feat->GetGeometryRef());
               upper.feat->SetGeometry(fusionPolygon);
               
               //inputLayer->SetFeature(upper.feat);
               OGRFeature * fusionFeature;
               fusionFeature = OGRFeature::CreateFeature( inputLayer->GetLayerDefn() );
               fusionFeature->SetGeometry( fusionPolygon );
               fusionFeature->SetField(0,upper.feat->GetFieldAsInteger(0));
               inputLayer->CreateFeature(fusionFeature);
               inputLayer->DeleteFeature(lower.feat->GetFID());
               inputLayer->DeleteFeature(upper.feat->GetFID());
               OGRFeature::DestroyFeature( fusionFeature );
               if(++nbTransaction == 65536)
               {
                  inputLayer->CommitTransaction();
                  inputLayer->StartTransaction();
               }
            }
         }
         
         //free memory
         for(unsigned int u=0; u<nbUpperPolygons; u++)
         {
            OGRFeature::DestroyFeature( upperStreamFeatureList.at(u).feat );
         }
         for(unsigned int l=0; l<nbLowerPolygons; l++)
         {
            OGRFeature::DestroyFeature( lowerStreamFeatureList.at(l).feat );
         }
      
      /*chrono.Stop();
      std::cout<< "Column fusion tile took " << chrono.GetTotal() << " sec"<<std::endl; */
  
      } //end for x
   } //end for y
   inputLayer->CommitTransaction();

   //Process line
   nbTransaction = 0;
   inputLayer->StartTransaction();
   for(unsigned int y=1; y<=nbRowStream; y++)
   {
      for(unsigned int x=1; x<=nbColStream; x++)
      {
         /*std::cout<< " line tile number : " << x*y <<std::endl;
         itk::TimeProbe chrono;
         chrono.Start(); */
         
         //First compute the intersection between polygons and the streaming line (upper stream)
         std::vector<FeatureStruct> upperStreamFeatureList;
         upperStreamFeatureList.clear();
         
         //Compute the spatial filter of the upper stream
         IndexType  UpperLeftCorner;
         UpperLeftCorner[0] = (x-1)*m_StreamSize[0] + 1;
         UpperLeftCorner[1] = m_StreamSize[1]*y - 1 - 1 - m_Radius;
         
         IndexType  LowerRightCorner;
         LowerRightCorner[0] = m_StreamSize[0]*x - 1 - 1;
         LowerRightCorner[1] = m_StreamSize[1]*y - 1 - 1; //-1 to stop just before stream line
         
         OriginType  ulCorner;
         inputImage->TransformIndexToPhysicalPoint(UpperLeftCorner, ulCorner);
         OriginType  lrCorner;
         inputImage->TransformIndexToPhysicalPoint(LowerRightCorner, lrCorner);
         
         inputLayer->SetSpatialFilterRect(ulCorner[0],lrCorner[1],lrCorner[0],ulCorner[1]);
         
         inputLayer->ResetReading();
         nbFeature = inputLayer->GetFeatureCount(true);
         
         f = 0;
         while (f<nbFeature)
         {
            OGRFeature * feature = NULL;
            feature = inputLayer->GetNextFeature();
            
            FeatureStruct s;
            s.feat = feature;
            s.fusioned = false;
            upperStreamFeatureList.push_back(s);

            f++;
            
         }
         
         //Do the same thing for the lower stream
         std::vector<FeatureStruct> lowerStreamFeatureList;
         lowerStreamFeatureList.clear();
         
         //Compute the spatial filter of the lower stream
         UpperLeftCorner[0] = (x-1)*m_StreamSize[0] + 1;
         UpperLeftCorner[1] = m_StreamSize[1]*y + 1;
         
         LowerRightCorner[0] = m_StreamSize[0]*x - 1 - 1;
         LowerRightCorner[1] = m_StreamSize[1]*y + 1 + m_Radius;
         
         inputImage->TransformIndexToPhysicalPoint(UpperLeftCorner, ulCorner);
         inputImage->TransformIndexToPhysicalPoint(LowerRightCorner, lrCorner);
         
         inputLayer->SetSpatialFilterRect(ulCorner[0],lrCorner[1],lrCorner[0],ulCorner[1]);
         
         inputLayer->ResetReading();
         nbFeature = inputLayer->GetFeatureCount(true);
         
         f = 0;
         while (f<nbFeature)
         {
            OGRFeature * feature = NULL;
            feature = inputLayer->GetNextFeature();

            FeatureStruct s;
            s.feat = feature;
            s.fusioned = false;
            lowerStreamFeatureList.push_back(s);

            f++;
         }
      
         unsigned int nbUpperPolygons = upperStreamFeatureList.size();
         unsigned int nbLowerPolygons = lowerStreamFeatureList.size();
         std::vector<FusionStruct> fusionList;
         fusionList.clear();
         for(unsigned int u=0; u<nbUpperPolygons; u++)
         {
            for(unsigned int l=0; l<nbLowerPolygons; l++)
            {
               FeatureStruct upper = upperStreamFeatureList.at(u);
               FeatureStruct lower = lowerStreamFeatureList.at(l);
               if (upper.feat->GetGeometryRef()->Intersects(lower.feat->GetGeometryRef()))
               {

                  OGRGeometry * intersection;
                  intersection = upper.feat->GetGeometryRef()->Intersection(lower.feat->GetGeometryRef());
                  if (intersection != NULL)
                  {
                     FusionStruct fusion;
                     fusion.indStream1 = u;
                     fusion.indStream2 = l;
                     fusion.overlap = 0.;
                     
                     if(intersection->getGeometryType() == wkbPolygon)
                     {
                        fusion.overlap = dynamic_cast<OGRPolygon *>(intersection)->get_Area();
                     }
                     else if(intersection->getGeometryType() == wkbMultiPolygon)
                     {
                        fusion.overlap = dynamic_cast<OGRMultiPolygon *>(intersection)->get_Area();
                     }
                     else if(intersection->getGeometryType() == wkbGeometryCollection)
                     {
                        fusion.overlap = dynamic_cast<OGRGeometryCollection *>(intersection)->get_Area();
                     }
                     else if(intersection->getGeometryType() == wkbLineString)
                     {
                        fusion.overlap = dynamic_cast<OGRLineString *>(intersection)->get_Length();
                     }
                     else if (intersection->getGeometryType() == wkbMultiLineString)
                     {
                        fusion.overlap = dynamic_cast<OGRMultiLineString *>(intersection)->get_Length();
                     }
                     long upperFID = upper.feat->GetFID();
                     long lowerFID = lower.feat->GetFID();
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
               upperStreamFeatureList.at(fusionList.at(i).indStream1).fusioned = true;
               lowerStreamFeatureList.at(fusionList.at(i).indStream2).fusioned = true;
               OGRGeometry * fusionPolygon;
               fusionPolygon = upper.feat->GetGeometryRef()->Union(lower.feat->GetGeometryRef());
               upper.feat->SetGeometry(fusionPolygon);
               OGRFeature * fusionFeature;
               fusionFeature = OGRFeature::CreateFeature( inputLayer->GetLayerDefn() );
               fusionFeature->SetGeometry( fusionPolygon );
               fusionFeature->SetField(0,upper.feat->GetFieldAsInteger(0));
               inputLayer->CreateFeature(fusionFeature);
               inputLayer->DeleteFeature(lower.feat->GetFID());
               inputLayer->DeleteFeature(upper.feat->GetFID());
               OGRFeature::DestroyFeature( fusionFeature );
               if(++nbTransaction == 65536)
               {
                  inputLayer->CommitTransaction();
                  inputLayer->StartTransaction();
               }
            }
         }
         
         //free memory
         for(unsigned int u=0; u<nbUpperPolygons; u++)
         {
            OGRFeature::DestroyFeature( upperStreamFeatureList.at(u).feat );
         }
         for(unsigned int l=0; l<nbLowerPolygons; l++)
         {
            OGRFeature::DestroyFeature( lowerStreamFeatureList.at(l).feat );
         }
      
      
      /*chrono.Stop();
      std::cout<< "line fusion tile took " << chrono.GetTotal() << " sec"<<std::endl; */
      
      } //end for x
   } //end for y
   inputLayer->CommitTransaction();
   
   //Free memory
   OGRDataSource::DestroyDataSource(inputDataSource);
   

}


} // end namespace otb

#endif

