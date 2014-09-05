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
#include "otbGlImageActor.h"
#include "otbViewSettings.h"
#include "otbMath.h"
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glew.h>

#include "otbStandardShader.h"

#include "itkListSample.h"
#include "otbListSampleToHistogramListGenerator.h"

namespace otb
{

GlImageActor::GlImageActor()
  : m_TileSize(256),
    m_FileName(),
    m_FileReader(),
    m_LoadedTiles(),
    m_RedIdx(1),
    m_GreenIdx(2),
    m_BlueIdx(3),
    m_CurrentResolution(1),
    m_AvailableResolutions(),
    m_Origin(),
    m_Spacing(),
    m_NumberOfComponents(0),
    m_Shader(StandardShader::New()),
    m_ViewportToImageTransform(RSTransformType::New()),
    m_ImageToViewportTransform(RSTransformType::New()),
    m_ViewportForwardRotationTransform(RigidTransformType::New()),
    m_ViewportBackwardRotationTransform(RigidTransformType::New())
{}

GlImageActor::~GlImageActor()
{}

const GlImageActor::PointType & GlImageActor::GetOrigin() const
{
  return m_Origin;
}

const GlImageActor::SpacingType & GlImageActor::GetSpacing() const
{
  return m_Spacing;
}

std::string GlImageActor::GetWkt() const
{
  return m_FileReader->GetOutput()->GetProjectionRef();
}

GlImageActor::ImageKeywordlistType GlImageActor::GetKwl() const
{
  return m_FileReader->GetOutput()->GetImageKeywordlist();
}

GlImageActor::MetaDataDictionaryType & GlImageActor::GetMetaDataDictionary() const
{
  return m_FileReader->GetOutput()->GetMetaDataDictionary();
}


void GlImageActor::Initialize(const std::string & filename)
{
  // First, clean up any previous data
  this->ClearLoadedTiles();

  m_FileName = filename;

  m_FileReader = ReaderType::New();
  m_FileReader->SetFileName(m_FileName);
  m_FileReader->UpdateOutputInformation();

  m_LargestRegion = m_FileReader->GetOutput()->GetLargestPossibleRegion();

  if(m_FileReader->GetOutput()->GetNumberOfComponentsPerPixel() < 3)
    {
    m_RedIdx = 1;
    m_GreenIdx = 1;
    m_BlueIdx = 1;
    }

  m_Origin = m_FileReader->GetOutput()->GetOrigin();
  m_Spacing = m_FileReader->GetOutput()->GetSpacing();
  m_NumberOfComponents = m_FileReader->GetOutput()->GetNumberOfComponentsPerPixel();

  m_AvailableResolutions = m_FileReader->GetAvailableResolutions();

  m_CurrentResolution = m_AvailableResolutions.front();

  // Update transforms once data is read
  UpdateTransforms();

  // std::cout<<"Number of resolutions in file: "<<m_AvailableResolutions.size()<<std::endl;
}

void GlImageActor::GetExtent(double & ulx, double & uly, double & lrx, double & lry) const
{
  RegionType largest = m_FileReader->GetOutput()->GetLargestPossibleRegion();

  ImageRegionToViewportExtent(largest,ulx,uly,lrx,lry);
}

void GlImageActor::ProcessViewSettings()
{
  // Is there anything to do ?
  ViewSettings::ConstPointer settings = this->GetSettings();

  
  if(settings->GetUseProjection() && settings->GetGeometryChanged())
    {
    UpdateTransforms();
    
    for (TileVectorType::iterator it = m_LoadedTiles.begin();
         it!=m_LoadedTiles.end();
         ++it)
      {
      this->ImageRegionToViewportQuad(it->m_ImageRegion,it->m_UL,it->m_UR,it->m_LL,it->m_LR);
      }
    } 
}

void GlImageActor::UpdateData()
{
  // Update resolution needed
  UpdateResolution();

  // First, clean existing tiles
  CleanLoadedTiles();

  // Retrieve settings
  ViewSettings::ConstPointer settings = this->GetSettings();

  RegionType largest = m_FileReader->GetOutput()->GetLargestPossibleRegion();
 
  double ulx, uly, lrx, lry;

  settings->GetViewportExtent(ulx,uly,lrx,lry);

  // std::cout<<"Viewport extent: "<<ulx<<", "<<uly<<" - "<<lrx<<", "<<lry<<std::endl;

  RegionType requested;

  this->ViewportExtentToImageRegion(ulx,uly,lrx,lry,requested);

  if(!requested.Crop(m_FileReader->GetOutput()->GetLargestPossibleRegion()))
    {
    return;
    }

  // std::cout<<"Corresponding image region: "<<requested<<std::endl;
 
  // Now we have the requested part of image, we need to find the
  // corresponding tiles

  // First compute needed tiles
  unsigned int nbTilesX = vcl_ceil(static_cast<double>(requested.GetIndex()[0] + requested.GetSize()[0])/m_TileSize) -vcl_floor(static_cast<double>(requested.GetIndex()[0])/m_TileSize);
  unsigned int nbTilesY = vcl_ceil(static_cast<double>(requested.GetIndex()[1] + requested.GetSize()[1])/m_TileSize) -vcl_floor(static_cast<double>(requested.GetIndex()[1])/m_TileSize);
  //unsigned int nbTilesY = vcl_ceil(static_cast<double>(requested.GetSize()[1])/m_TileSize);
  unsigned int tileStartX = m_TileSize*(requested.GetIndex()[0]/m_TileSize);
  unsigned int tileStartY = m_TileSize*(requested.GetIndex()[1]/m_TileSize);

  // std::cout<<"Required tiles: "<<nbTilesX<<" x "<<nbTilesY<<std::endl;

  SizeType tileSize;
  tileSize.Fill(m_TileSize);

   for(unsigned int i = 0; i < nbTilesX; ++i)
    {
    for(unsigned int j = 0; j<nbTilesY; ++j)
      {
      Tile newTile;
      newTile.m_TextureId = 0;

      IndexType tileIndex;
      tileIndex[0] = static_cast<unsigned int>(tileStartX+i*m_TileSize);
      tileIndex[1] = static_cast<unsigned int>(tileStartY+j*m_TileSize);
      
      newTile.m_ImageRegion.SetSize(tileSize);
      newTile.m_ImageRegion.SetIndex(tileIndex);
      
      newTile.m_ImageRegion.Crop(m_FileReader->GetOutput()->GetLargestPossibleRegion());

      ImageRegionToViewportQuad(newTile.m_ImageRegion,newTile.m_UL,newTile.m_UR,newTile.m_LL,newTile.m_LR);

       // std::cout<<"Loading tile "<<newTile.m_ImageRegion<<std::endl;
       // std::cout<<"Mapped to "<<newTile.m_UL<<", "<<newTile.m_UR<<", "<<newTile.m_LL<<", "<<newTile.m_LR<<std::endl;

      newTile.m_RedIdx = m_RedIdx;
      newTile.m_GreenIdx = m_GreenIdx;
      newTile.m_BlueIdx = m_BlueIdx;
      newTile.m_Resolution = m_CurrentResolution;

      if(!TileAlreadyLoaded(newTile))
        {
        LoadTile(newTile);
        }
      }
    }
}

bool GlImageActor::TileAlreadyLoaded(const Tile& tile)
{
    for(TileVectorType::iterator it = m_LoadedTiles.begin();
      it!=m_LoadedTiles.end();
      ++it)
    {
    if(it->m_ImageRegion == tile.m_ImageRegion)
      {
      return (tile.m_Resolution ==  it->m_Resolution
              && tile.m_RedIdx == it->m_RedIdx
              && tile.m_GreenIdx == it->m_GreenIdx
              && tile.m_BlueIdx == it->m_BlueIdx);
      }
    }

  return false;
}

void GlImageActor::Render()
{
  m_Shader->LoadShader();
  m_Shader->SetupShader();

  for(TileVectorType::iterator it = m_LoadedTiles.begin();
      it != m_LoadedTiles.end(); ++it)
    {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

     glEnable(GL_TEXTURE_2D);  
     glBindTexture(GL_TEXTURE_2D,it->m_TextureId);

     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);     
     if(m_CurrentResolution == 0)
       {
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       }
     else
       {
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
       }

     // Reset color before rendering
     glColor3d(1.0f,1.0f,1.0f);
     
     glBegin (GL_QUADS);
     glTexCoord2f (0.0, 1.0); glVertex2f(it->m_LL[0], it->m_LL[1]);
     glTexCoord2f (1.0, 1.0); glVertex2f(it->m_LR[0], it->m_LR[1]);
     glTexCoord2f (1.0, 0.0); glVertex2f(it->m_UR[0], it->m_UR[1]);
     glTexCoord2f (0.0, 0.0); glVertex2f(it->m_UL[0], it->m_UL[1]);
     glEnd ();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    }
  
  m_Shader->UnloadShader();
}

void GlImageActor::LoadTile(Tile& tile)
{
  ExtractROIFilterType::Pointer extract = ExtractROIFilterType::New();
  extract->SetInput(m_FileReader->GetOutput());
  extract->SetExtractionRegion(tile.m_ImageRegion);
  extract->SetChannel(tile.m_RedIdx);
  extract->SetChannel(tile.m_GreenIdx);
  extract->SetChannel(tile.m_BlueIdx);
  extract->Update();

  tile.m_Image = extract->GetOutput();
  
  itk::ImageRegionConstIterator<VectorImageType> it(extract->GetOutput(),extract->GetOutput()->GetLargestPossibleRegion());
  
  float * buffer = new float[4*extract->GetOutput()->GetLargestPossibleRegion().GetNumberOfPixels()];
  
  unsigned int idx = 0;
  
  for(it.GoToBegin();!it.IsAtEnd();++it)
    {
    buffer[idx] = static_cast<float>(it.Get()[2]);
    ++idx;
    buffer[idx] = static_cast<float>(it.Get()[1]);
    ++idx;
    buffer[idx] = static_cast<float>(it.Get()[0]);
    ++idx;
    buffer[idx] = 255.;
    ++idx;
    }
  
  // Now load the texture
  glGenTextures(1, &(tile.m_TextureId));
  glBindTexture(GL_TEXTURE_2D, tile.m_TextureId);
#if defined(GL_TEXTURE_BASE_LEVEL) && defined(GL_TEXTURE_MAX_LEVEL)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
#endif
  //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
// #if defined(GL_CLAMP_TO_BORDER)      
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
// #elif defined (GL_CLAMP_TO_BORDER_EXT)
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER_EXT);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER_EXT);
// #elif defined (GL_MIRRORED_REPEAT)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
// #endif
  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_RGB32F,
    extract->GetOutput()->GetLargestPossibleRegion().GetSize()[0],
    extract->GetOutput()->GetLargestPossibleRegion().GetSize()[1], 
    0, GL_BGRA, GL_FLOAT,
    buffer);
  
  tile.m_Loaded = true;
  
  delete [] buffer;
  
  // And push to loaded texture
  m_LoadedTiles.push_back(tile);
}

void GlImageActor::UnloadTile(Tile& tile)
{
  if(tile.m_Loaded)
    {
    glDeleteTextures(1,&tile.m_TextureId);
    tile.m_Loaded = false;
    }
}

void GlImageActor::CleanLoadedTiles()
{
  TileVectorType newLoadedTiles;

  // Retrieve settings
  ViewSettings::ConstPointer settings = this->GetSettings();
    
  double ulx, uly, lrx, lry;

  settings->GetViewportExtent(ulx,uly,lrx,lry);
    
  RegionType requested;
  
  this->ViewportExtentToImageRegion(ulx,uly,lrx,lry,requested);

  for(TileVectorType::iterator it = m_LoadedTiles.begin();
  it!=m_LoadedTiles.end();++it)
    {
    RegionType tileRegion = it->m_ImageRegion;

    // Test if tileRegion intersects requested region
    if(!tileRegion.Crop(requested)
       || it->m_Resolution != m_CurrentResolution
       || it->m_RedIdx != m_RedIdx
       || it->m_GreenIdx != m_GreenIdx
       || it->m_BlueIdx != m_BlueIdx)
      {     
      // Tile will not be used anymore, unload it from GPU
      UnloadTile(*it);
      }
    else
      {
      newLoadedTiles.push_back(*it);
      }
    }

  // std::cout<<"GPU memory cleanup: removing "<<m_LoadedTiles.size() - newLoadedTiles.size() << " over "<<m_LoadedTiles.size() <<" tiles"<<std::endl;

  m_LoadedTiles.swap(newLoadedTiles);


}

void GlImageActor::ClearLoadedTiles()
{
  for(TileVectorType::iterator it = m_LoadedTiles.begin();
      it!=m_LoadedTiles.end();++it)
    {
    UnloadTile(*it);
    }
  m_LoadedTiles.clear();
}

void GlImageActor::ImageRegionToViewportExtent(const RegionType& region, double & ulx, double & uly, double & lrx, double& lry) const
{
  PointType tul,tur,tll,tlr;
  
  ImageRegionToViewportQuad(region,tul,tur,tll,tlr);

  // TODO: Take into account origin/scaling/rotation here ?
  // TODO: Screen to image / image to screen transform calls here ?

  ulx = std::min(std::min(tul[0],tur[0]),std::min(tll[0],tlr[0]));
  uly = std::min(std::min(tul[1],tur[1]),std::min(tll[1],tlr[1]));
  lrx = std::max(std::max(tul[0],tur[0]),std::max(tll[0],tlr[0]));
  lry = std::max(std::max(tul[1],tur[1]),std::max(tll[1],tlr[1]));
}

void GlImageActor::ImageRegionToViewportQuad(const RegionType & region, PointType & ul, PointType & ur, PointType & ll, PointType & lr) const
{
  // Retrieve settings
  ViewSettings::ConstPointer settings = this->GetSettings();
    
  itk::ContinuousIndex<double,2> cul,cur,cll,clr;
  
  cul[0] = region.GetIndex()[0];
  cul[1] = region.GetIndex()[1];
  cur = cul;
  cur[0]+=region.GetSize()[0];
  cll = cul;
  cll[1]+=region.GetSize()[1];
  clr = cur;
  clr[1]+=region.GetSize()[1];

  PointType iul, iur,ill,ilr;

  m_FileReader->GetOutput()->TransformContinuousIndexToPhysicalPoint(cul,iul);
  m_FileReader->GetOutput()->TransformContinuousIndexToPhysicalPoint(cur,iur);
  m_FileReader->GetOutput()->TransformContinuousIndexToPhysicalPoint(cll,ill);
  m_FileReader->GetOutput()->TransformContinuousIndexToPhysicalPoint(clr,ilr);
  
  PointType pul = m_ViewportBackwardRotationTransform->TransformPoint(m_ImageToViewportTransform->TransformPoint(iul));
  PointType pur = m_ViewportBackwardRotationTransform->TransformPoint(m_ImageToViewportTransform->TransformPoint(iur));
  PointType pll = m_ViewportBackwardRotationTransform->TransformPoint(m_ImageToViewportTransform->TransformPoint(ill));
  PointType plr = m_ViewportBackwardRotationTransform->TransformPoint(m_ImageToViewportTransform->TransformPoint(ilr));

  ul[0] = pul[0];
  ul[1] = pul[1];
  ur[0] = pur[0];
  ur[1] = pur[1];
  ll[0] = pll[0];
  ll[1] = pll[1];
  lr[0] = plr[0];
  lr[1] = plr[1];
}

void GlImageActor::ViewportExtentToImageRegion(const double& ulx, const double & uly, const double & lrx, const double & lry, RegionType & region) const
{
  // Retrieve settings
  ViewSettings::ConstPointer settings = this->GetSettings();

  RegionType largest = m_FileReader->GetOutput()->GetLargestPossibleRegion();

  PointType ul,ur,ll,lr,tul,tur,tll,tlr;
  
  ul[0]=ulx;
  ul[1]=uly;
  ur[0]=lrx;
  ur[1]=uly;
  ll[0]=ulx;
  ll[1]=lry;
  lr[0]=lrx;
  lr[1]=lry;
  
  tul = m_ViewportToImageTransform->TransformPoint(m_ViewportForwardRotationTransform->TransformPoint(ul));
  tur = m_ViewportToImageTransform->TransformPoint(m_ViewportForwardRotationTransform->TransformPoint(ur));
  tll = m_ViewportToImageTransform->TransformPoint(m_ViewportForwardRotationTransform->TransformPoint(ll));
  tlr = m_ViewportToImageTransform->TransformPoint(m_ViewportForwardRotationTransform->TransformPoint(lr));

  itk::ContinuousIndex<double,2> cul,cur,cll,clr;

  m_FileReader->GetOutput()->TransformPhysicalPointToContinuousIndex(tul,cul);
  m_FileReader->GetOutput()->TransformPhysicalPointToContinuousIndex(tur,cur);
  m_FileReader->GetOutput()->TransformPhysicalPointToContinuousIndex(tll,cll);
  m_FileReader->GetOutput()->TransformPhysicalPointToContinuousIndex(tlr,clr);
  
  // TODO: Take into account origin/scaling/rotation here ?
  // TODO: Screen to image / image to screen transform calls here ?

  double iulx, iuly, ilrx, ilry;

  iulx = std::max(std::min(std::min(cul[0],cur[0]),std::min(cll[0],clr[0])),0.);
  iuly = std::max(std::min(std::min(cul[1],cur[1]),std::min(cll[1],clr[1])),0.);
  ilrx = std::min(std::max(std::max(cul[0],cur[0]),std::max(cll[0],clr[0])),static_cast<double>(largest.GetSize()[0]));
  ilry = std::min(std::max(std::max(cul[1],cur[1]),std::max(cll[1],clr[1])),static_cast<double>(largest.GetSize()[1]));
 
  // Now we have the requested part of image, we need to find the
  // corresponding tiles

  IndexType index;
  index[0] = static_cast<unsigned int>(iulx);
  index[1] = static_cast<unsigned int>(iuly);
 
  SizeType size;
  size[0] = static_cast<int>(ilrx-iulx);
  size[1] = static_cast<int>(ilry-iuly);
     
  region.SetSize(size);
  region.SetIndex(index);
  
  region.Crop(m_FileReader->GetOutput()->GetLargestPossibleRegion());
}

GlImageActor::PointType GlImageActor::ViewportToImageTransform(const PointType & in, bool physical) const
{
  PointType inRotated = m_ViewportForwardRotationTransform->TransformPoint(in);

  PointType imgPoint = m_ViewportToImageTransform->TransformPoint(inRotated);

  if(!physical)
    {
    imgPoint[0]=(imgPoint[0]-m_Origin[0])/m_Spacing[0];
    imgPoint[1]=(imgPoint[1]-m_Origin[1])/m_Spacing[1];
    }

  return imgPoint;
}

GlImageActor::PointType GlImageActor::ImageToViewportTransform(const PointType & in, bool physical) const
{
  PointType imgPoint = in;

  if(!physical)
    {
    imgPoint[0]=imgPoint[0]*m_Spacing[0]+m_Origin[0];
    imgPoint[1]=imgPoint[1]*m_Spacing[1]+m_Origin[1];
    }

  PointType out =  m_ImageToViewportTransform->TransformPoint(imgPoint);
  
  return m_ViewportBackwardRotationTransform->TransformPoint(out);

}

bool GlImageActor::GetPixelFromViewport(const PointType & in, PixelType& pixel) const
{
  PointType imgPoint = ViewportToImageTransform(in);



  // Transform to index at current resolution
  IndexType idx;
  idx[0] = static_cast<unsigned int>((imgPoint[0]-m_FileReader->GetOutput()->GetOrigin()[0])/m_FileReader->GetOutput()->GetSpacing()[0]);
  idx[1] = static_cast<unsigned int>((imgPoint[1]-m_FileReader->GetOutput()->GetOrigin()[1])/m_FileReader->GetOutput()->GetSpacing()[1]);

  // Look-up tiles
  for (TileVectorType::const_iterator it = m_LoadedTiles.begin();
       it!=m_LoadedTiles.end();
       ++it)
    {
    if(it->m_ImageRegion.IsInside(idx))
      {
      idx[0]-=it->m_ImageRegion.GetIndex()[0];
      idx[1]-=it->m_ImageRegion.GetIndex()[1];
      pixel = it->m_Image->GetPixel(idx);
      
      return true;
      }
    }

  return false;
}



void GlImageActor::UpdateResolution()
{
  // Retrieve settings
  ViewSettings::ConstPointer settings = this->GetSettings();

  // Retrieve viewport spacing
  ViewSettings::SpacingType spacing = settings->GetSpacing();
  
  PointType pointA, pointB;

  pointA  = settings->GetOrigin();
  pointB = pointA;
  
  pointB[0]+=100*spacing[0];
  pointB[1]+=100*spacing[1];

  // TODO: This part needs a review

  // Transform the spacing vector
  pointA = m_ViewportForwardRotationTransform->TransformPoint(pointA);
  pointA = m_ViewportToImageTransform->TransformPoint(pointA);
  pointB = m_ViewportForwardRotationTransform->TransformPoint(pointB);
  pointB = m_ViewportToImageTransform->TransformPoint(pointB);

  SpacingType outSpacing;
  outSpacing[0] = (pointB[0]-pointA[0])/100;
  outSpacing[1] = (pointB[1]-pointA[1])/100;

  // std::cout<<"Estimated spacing: "<<outSpacing<<std::endl;

  // Last, divide by image spacing to get the resolution
  double resolution = std::min(vcl_abs(m_Spacing[0]/outSpacing[0]), 
                               vcl_abs(m_Spacing[1]/outSpacing[1]));

  // std::cout<<"Resolution: "<<resolution<<std::endl;
  
  // Arbitrary higher than any distance we will compute here
  double minDist       = 50000.;
  unsigned int closest = 0;

  // Compute the diff and keep the index that minimize the distance
  for (ResolutionVectorType::iterator it = m_AvailableResolutions.begin();
       it != m_AvailableResolutions.end(); ++it)
    {
    // std::cout<<(*it)<<" "<<(1/((double)(1<<(*it))))<<std::endl;
    double diff = 1/((double)(1<<(*it))) - resolution;

    if (diff > 0 && vcl_abs(diff) < minDist)
      {
      minDist = vcl_abs(diff);
      closest = std::distance(m_AvailableResolutions.begin(),it);
      }
    }
  
  // std::cout<<"Nearest resolution level: "<<closest<<std::endl;

  m_CurrentResolution = closest;

  std::ostringstream extFilename;
  extFilename<<m_FileName<<"?&resol="<<m_CurrentResolution;

  // std::cout<<"Extfname = "<<extFilename.str()<<std::endl;

  m_FileReader = ReaderType::New();
  m_FileReader->SetFileName(extFilename.str());
  m_FileReader->UpdateOutputInformation();
}

void GlImageActor::UpdateTransforms()
{
  if(m_FileName == "")
    {
    return;
    }

  // Retrieve settings
  ViewSettings::ConstPointer settings = this->GetSettings();

  m_ViewportToImageTransform = RSTransformType::New();
  m_ImageToViewportTransform = RSTransformType::New();

  if(settings->GetUseProjection())
    {
    m_ViewportToImageTransform->SetInputProjectionRef(settings->GetWkt());
    m_ViewportToImageTransform->SetInputKeywordList(settings->GetKeywordList());
    m_ViewportToImageTransform->SetOutputProjectionRef(m_FileReader->GetOutput()->GetProjectionRef());
    m_ViewportToImageTransform->SetOutputKeywordList(m_FileReader->GetOutput()->GetImageKeywordlist());

    m_ImageToViewportTransform->SetOutputProjectionRef(settings->GetWkt());
    m_ImageToViewportTransform->SetOutputKeywordList(settings->GetKeywordList());
    m_ImageToViewportTransform->SetInputProjectionRef(m_FileReader->GetOutput()->GetProjectionRef());
    m_ImageToViewportTransform->SetInputKeywordList(m_FileReader->GetOutput()->GetImageKeywordlist());
    }
  m_ViewportToImageTransform->InstanciateTransform();
  m_ImageToViewportTransform->InstanciateTransform();

  m_ViewportForwardRotationTransform = RigidTransformType::New();
  m_ViewportBackwardRotationTransform = RigidTransformType::New();

  RigidTransformType::ParametersType rigidParameters(5);
  rigidParameters.Fill(0);
  rigidParameters[0]=settings->GetRotationAngle();
  rigidParameters[1]=settings->GetRotationCenter()[0];
  rigidParameters[2]=settings->GetRotationCenter()[1];

  m_ViewportForwardRotationTransform->SetParameters(rigidParameters);

  m_ViewportForwardRotationTransform->GetInverse(m_ViewportBackwardRotationTransform);
}

void GlImageActor::AutoColorAdjustment(double & minRed, double & maxRed, double & minGreen, double & maxGreen, double & minBlue, double & maxBlue, bool full, double lcp, double hcp)
{
  typedef itk::Statistics::ListSample<VectorImageType::PixelType> ListSampleType;
  typedef itk::Statistics::DenseFrequencyContainer2 DFContainerType;
  typedef ListSampleToHistogramListGenerator<ListSampleType, VectorImageType::InternalPixelType, DFContainerType> HistogramsGeneratorType;

  ListSampleType::Pointer listSample = ListSampleType::New();
  listSample->SetMeasurementVectorSize(3);
  
  if(full)
    {
    // Retrieve the lowest resolution
    ReaderType::Pointer reader = ReaderType::New();
    std::ostringstream extFilename;

    unsigned int nb_pixel_ref = 500 * 500;

    // Compute the diff and keep the index that minimize the distance
    unsigned int resol = 0;
    bool found = false;

    for (ResolutionVectorType::iterator it = m_AvailableResolutions.begin();
         it != m_AvailableResolutions.end() && !found; ++it,++resol)
      {
      unsigned int factor = 1<<(*it);
      unsigned int sizex = m_LargestRegion.GetSize()[0]/factor;
      unsigned int sizey = m_LargestRegion.GetSize()[1]/factor;

      unsigned int nb_pixel = sizex * sizey;
      if(nb_pixel<nb_pixel_ref)
        {
        found=true;
        break;
        }
      }

    // If no resolution can give less than nb_pixel_ref pixels, take
    // the lowest
    if(!found)
      {
      resol = m_AvailableResolutions.size()-1;
      }

    extFilename<<m_FileName<<"?&resol="<<m_AvailableResolutions[resol];
    reader->SetFileName(extFilename.str());

    ExtractROIFilterType::Pointer extract = ExtractROIFilterType::New();
    extract->SetInput(reader->GetOutput());
    extract->SetChannel(m_RedIdx);
    extract->SetChannel(m_GreenIdx);
    extract->SetChannel(m_BlueIdx);
    extract->Update();
    
    itk::ImageRegionConstIterator<VectorImageType> it(extract->GetOutput(),extract->GetOutput()->GetLargestPossibleRegion());
    for(it.GoToBegin();!it.IsAtEnd();++it)
      {
      bool nonan = true;

      for(unsigned int i = 0; i < it.Get().Size();++i)
        {
        nonan = nonan && !vnl_math_isnan(it.Get()[i]);
        }

      if(nonan)
        {
        listSample->PushBack(it.Get());
        }
      }
    }
  else
    {
    // Retrieve all tiles
    for(TileVectorType::iterator it = m_LoadedTiles.begin();it!=m_LoadedTiles.end();++it)
    {
    itk::ImageRegionConstIterator<VectorImageType> imIt(it->m_Image,it->m_Image->GetLargestPossibleRegion());
    for(imIt.GoToBegin();!imIt.IsAtEnd();++imIt)
      {
      bool nonan = true;

      for(unsigned int i = 0; i < imIt.Get().Size();++i)
        {
        nonan = nonan && !vnl_math_isnan(imIt.Get()[i]);
        }

      if(nonan)
        {
        listSample->PushBack(imIt.Get());
        }
      }
    }
    }

    // Compute the histogram
    HistogramsGeneratorType::Pointer histogramsGenerator = HistogramsGeneratorType::New();
    histogramsGenerator->SetListSample(listSample);
    histogramsGenerator->SetNumberOfBins(255);
    histogramsGenerator->NoDataFlagOn();
    histogramsGenerator->Update();

    minRed   = histogramsGenerator->GetOutput()->GetNthElement(0)->Quantile(0,lcp);
    maxRed   = histogramsGenerator->GetOutput()->GetNthElement(0)->Quantile(0,1-hcp);
    minGreen = histogramsGenerator->GetOutput()->GetNthElement(1)->Quantile(0,lcp);
    maxGreen = histogramsGenerator->GetOutput()->GetNthElement(1)->Quantile(0,1-hcp);
    minBlue =  histogramsGenerator->GetOutput()->GetNthElement(2)->Quantile(0,lcp);
    maxBlue =  histogramsGenerator->GetOutput()->GetNthElement(2)->Quantile(0,1-hcp);   
}
}
