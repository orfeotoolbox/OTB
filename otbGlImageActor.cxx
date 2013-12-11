#include "otbGlImageActor.h"
#include "otbViewSettings.h"
#include "otbMath.h"
#include <GL/gl.h> 
#include <GL/glext.h>

namespace otb
{

// Shaders section
unsigned int GlImageActor::m_StandardShader = 0;
unsigned int GlImageActor::m_StandardShaderProgram = 0;
bool GlImageActor::m_ShaderInitialized = false;

GlImageActor::GlImageActor()
  : m_TileSize(256),
    m_FileName(),
    m_FileReader(),
    m_LoadedTiles(),
    m_RedIdx(1),
    m_GreenIdx(2),
    m_BlueIdx(3),
    m_MinRed(200),
    m_MaxRed(1200),
    m_MinGreen(200),
    m_MaxGreen(1200),
    m_MinBlue(200),
    m_MaxBlue(1200),
    m_CurrentResolution(1),
    m_AvailableResolutions(),
    m_Origin(),
    m_Spacing(),
    m_NumberOfComponents(0),
    m_UseShader(false)
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

void GlImageActor::Initialize(const std::string & filename)
{
  // Initialize shaders
  InitShaders();

  // First, clean up any previous data
  this->ClearLoadedTiles();

  m_FileName = filename;

  m_FileReader = ReaderType::New();
  m_FileReader->SetFileName(m_FileName);
  m_FileReader->UpdateOutputInformation();

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

  // std::cout<<"Number of resolutions in file: "<<m_AvailableResolutions.size()<<std::endl;
}

void GlImageActor::GetExtent(double & ulx, double & uly, double & lrx, double & lry) const
{
  RegionType largest = m_FileReader->GetOutput()->GetLargestPossibleRegion();

  ImageRegionToViewportExtent(largest,ulx,uly,lrx,lry);
}

void GlImageActor::ProcessViewSettings()
{
  for (TileVectorType::iterator it = m_LoadedTiles.begin();
       it!=m_LoadedTiles.end();
       ++it)
    {
    this->ImageRegionToViewportQuad(it->m_ImageRegion,it->m_UL,it->m_UR,it->m_LL,it->m_LR);
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
      newTile.m_MinRed = m_MinRed;
      newTile.m_MinGreen = m_MinGreen;
      newTile.m_MinBlue = m_MinBlue;
      newTile.m_MaxRed = m_MaxRed;
      newTile.m_MaxGreen = m_MaxGreen;
      newTile.m_MaxBlue = m_MaxBlue;
      newTile.m_Resolution = m_CurrentResolution;
      newTile.m_UseShader = m_UseShader;

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
      bool resp = (tile.m_Resolution ==  it->m_Resolution
                   && tile.m_RedIdx == it->m_RedIdx
                   && tile.m_GreenIdx == it->m_GreenIdx
                   && tile.m_BlueIdx == it->m_BlueIdx
                   && (tile.m_UseShader == m_UseShader));

      if(!m_UseShader)
        {
        return resp 
          &&tile.m_MinRed == m_MinRed
          &&tile.m_MinGreen == m_MinGreen
          &&tile.m_MinBlue == m_MinBlue
          &&tile.m_MaxRed == m_MaxRed
          &&tile.m_MaxGreen == m_MaxGreen
          &&tile.m_MaxBlue == m_MaxBlue;
        }
      else
        {
        return resp;
        }
      }
    }

  return false;
}

void GlImageActor::Render()
{
  // std::cout<<"Render: "<<m_LoadedTiles.size()<<" tiles to process"<<std::endl;

  if(m_UseShader)
    {
    // Setup shader
    glUseProgramObjectARB(m_StandardShaderProgram);
  
    int length;
    glGetProgramiv(m_StandardShaderProgram,GL_INFO_LOG_LENGTH,&length);
    char * logs = new char[length];
    glGetProgramInfoLog(m_StandardShaderProgram,1000,NULL,logs);
    // std::cout<<logs<<std::endl;
    delete [] logs;

    // Compute shifts and scales
    double shr,shg,shb,scr,scg,scb;
    shr = -m_MinRed;
    shg = -m_MinGreen;
    shb = -m_MinBlue;
    scr = 1./(m_MaxRed-m_MinRed);
    scg = 1./(m_MaxGreen-m_MinGreen);
    scb = 1./(m_MaxBlue-m_MinBlue);

    GLint shader_a= glGetUniformLocation(GlImageActor::m_StandardShaderProgram, "shader_a");
    glUniform4f(shader_a,scr,scg,scb,1.);
    GLint shader_b= glGetUniformLocation(GlImageActor::m_StandardShaderProgram, "shader_b");
    glUniform4f(shader_b,shr,shg,shb,0);
    }
  
  for(TileVectorType::iterator it = m_LoadedTiles.begin();
      it != m_LoadedTiles.end(); ++it)
    {
     glEnable(GL_TEXTURE_2D);  
     glBindTexture(GL_TEXTURE_2D,it->m_TextureId);
     
     // Reset color before rendering
     glColor3d(1.0f,1.0f,1.0f);
     
     glBegin (GL_QUADS);
     glTexCoord2f (0.0, 1.0); glVertex2f(it->m_LL[0], it->m_LL[1]);
     glTexCoord2f (1.0, 1.0); glVertex2f(it->m_LR[0], it->m_LR[1]);
     glTexCoord2f (1.0, 0.0); glVertex2f(it->m_UR[0], it->m_UR[1]);
     glTexCoord2f (0.0, 0.0); glVertex2f(it->m_UL[0], it->m_UL[1]);
     glEnd ();

    glDisable(GL_TEXTURE_2D);
    }
  
  if(m_UseShader)
    {
    glUseProgramObjectARB(0);
    }
}

void GlImageActor::LoadTile(Tile& tile)
{
  ExtractROIFilterType::Pointer extract = ExtractROIFilterType::New();
  extract->SetInput(m_FileReader->GetOutput());
  extract->SetExtractionRegion(tile.m_ImageRegion);
  extract->SetChannel(tile.m_RedIdx);
  extract->SetChannel(tile.m_GreenIdx);
  extract->SetChannel(tile.m_BlueIdx);

  if(!tile.m_UseShader)
    {
    RescaleFilterType::Pointer rescale = RescaleFilterType::New();
    rescale->SetInput(extract->GetOutput());
    rescale->AutomaticInputMinMaxComputationOff();
  
    VectorImageType::PixelType mins(3),maxs(3),omins(3),omaxs(3);
    
    mins[0] = m_MinRed;
    mins[1] = m_MinGreen;
    mins[2] = m_MinBlue;
    
    maxs[0] = m_MaxRed;
    maxs[1] = m_MaxGreen;
    maxs[2] = m_MaxBlue;
    
    omins.Fill(0);
    omaxs.Fill(255);
    
    rescale->SetInputMinimum(mins);
    rescale->SetInputMaximum(maxs);
    rescale->SetOutputMinimum(omins);
    rescale->SetOutputMaximum(omaxs);
    
    rescale->Update();
    
    itk::ImageRegionConstIterator<VectorImageType> it(rescale->GetOutput(),rescale->GetOutput()->GetLargestPossibleRegion());

    unsigned char * buffer = new unsigned char[4*rescale->GetOutput()->GetLargestPossibleRegion().GetNumberOfPixels()];
    
    unsigned int idx = 0;
    
    for(it.GoToBegin();!it.IsAtEnd();++it)
      {
      buffer[idx] = static_cast<unsigned char>(it.Get()[2]);
      ++idx;
      buffer[idx] = static_cast<unsigned char>(it.Get()[1]);
      ++idx;
      buffer[idx] = static_cast<unsigned char>(it.Get()[0]);
      ++idx;
      buffer[idx] = 255;
      ++idx;
      }
    
    // Now load the texture
    glGenTextures(1, &(tile.m_TextureId));
    glBindTexture(GL_TEXTURE_2D, tile.m_TextureId);
#if defined(GL_TEXTURE_BASE_LEVEL) && defined(GL_TEXTURE_MAX_LEVEL)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
#endif
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
        GL_TEXTURE_2D, 0, GL_RGBA8,
        rescale->GetOutput()->GetLargestPossibleRegion().GetSize()[0],
        rescale->GetOutput()->GetLargestPossibleRegion().GetSize()[1], 
        0, GL_BGRA, GL_UNSIGNED_BYTE,
        buffer);
      
      tile.m_Loaded = true;
      tile.m_UseShader = false;
  
      delete [] buffer;
    }
  // else !m_UseShader
  else
    {
    extract->Update();

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
      tile.m_UseShader = true;
  
      delete [] buffer;
    }

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
       || it->m_BlueIdx != m_BlueIdx
       || (it->m_UseShader != m_UseShader)
       || (!m_UseShader && it->m_MinRed != m_MinRed)
       || (!m_UseShader   &&it->m_MinGreen != m_MinGreen)
       || (!m_UseShader  &&it->m_MinBlue != m_MinBlue)
       || (!m_UseShader  &&it->m_MaxRed != m_MaxRed)
       || (!m_UseShader  &&it->m_MaxGreen != m_MaxGreen)
       || (!m_UseShader  &&it->m_MaxBlue != m_MaxBlue))
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
  
  // Setup RSTransform
  RSTransformType::Pointer rsTransform = RSTransformType::New();
  
  if(settings->GetUseProjection())
    {
    rsTransform->SetOutputProjectionRef(settings->GetWkt());
    rsTransform->SetOutputKeywordList(settings->GetKeywordList());
    rsTransform->SetInputProjectionRef(m_FileReader->GetOutput()->GetProjectionRef());
    rsTransform->SetInputKeywordList(m_FileReader->GetOutput()->GetImageKeywordlist());
    }
  rsTransform->InstanciateTransform();
    
  SpacingType spacing = m_FileReader->GetOutput()->GetSpacing();


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
  
  PointType pul = rsTransform->TransformPoint(iul);
  PointType pur = rsTransform->TransformPoint(iur);
  PointType pll = rsTransform->TransformPoint(ill);
  PointType plr = rsTransform->TransformPoint(ilr);

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

  
  // Setup RSTransform
  RSTransformType::Pointer rsTransform = RSTransformType::New();
  
  if(settings->GetUseProjection())
    {
    rsTransform->SetInputProjectionRef(settings->GetWkt());
    rsTransform->SetInputKeywordList(settings->GetKeywordList());
    rsTransform->SetOutputProjectionRef(m_FileReader->GetOutput()->GetProjectionRef());
    rsTransform->SetOutputKeywordList(m_FileReader->GetOutput()->GetImageKeywordlist());
    }
  rsTransform->InstanciateTransform();

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
  
  tul = rsTransform->TransformPoint(ul);
  tur = rsTransform->TransformPoint(ur);
  tll = rsTransform->TransformPoint(ll);
  tlr = rsTransform->TransformPoint(lr);

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

void GlImageActor::UpdateResolution()
{
  // Retrieve settings
  ViewSettings::ConstPointer settings = this->GetSettings();

  // Retrieve viewport spacing
  ViewSettings::SpacingType spacing = settings->GetSpacing();
  
  // Setup RSTransform
  RSTransformType::Pointer rsTransform = RSTransformType::New();
  
  // Set-up RS Transform
  if(settings->GetUseProjection())
    {
    rsTransform->SetInputProjectionRef(settings->GetWkt());
    rsTransform->SetInputKeywordList(settings->GetKeywordList());
    rsTransform->SetOutputProjectionRef(m_FileReader->GetOutput()->GetProjectionRef());
    rsTransform->SetOutputKeywordList(m_FileReader->GetOutput()->GetImageKeywordlist());
    }
  rsTransform->InstanciateTransform();

  PointType point, nullPoint;
  point[0] = spacing[0];
  point[1] = spacing[1];
  
  nullPoint.Fill(0);

  // TODO: This part needs a review
  // std::cout<<"Spacing: "<<point<<std::endl;

  // Transform the spacing vector
  point = rsTransform->TransformPoint(point);
  nullPoint = rsTransform->TransformPoint(nullPoint);

  point[0]-=nullPoint[0];
  point[1]-=nullPoint[1];

  // std::cout<<"Spacing: "<<point<<std::endl;

  // Last, divide by image spacing to get the resolution
  double resolution = std::min(vcl_abs(m_Spacing[0]/point[0]), 
                               vcl_abs(m_Spacing[1]/point[1]));

  // std::cout<<"Resolution: "<<resolution<<std::endl;
  
  // Arbitrary higher than any distance we will compute here
  double minDist       = 50000.;
  unsigned int closest = 0;

  // Compute the diff and keep the index that minimize the distance
  for (ResolutionVectorType::iterator it = m_AvailableResolutions.begin();
       it != m_AvailableResolutions.end(); ++it)
    {
    double diff = vcl_abs(1/((double)(1<<(*it))) - resolution);

    if (diff < minDist)
      {
      minDist = diff;
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


void GlImageActor::InitShaders()
{
  if(!GlImageActor::m_ShaderInitialized)
    {

    std::string source = "#version 120 \n"\
      "uniform sampler2D src;\n"\
      "uniform vec4 shader_a;\n"\
      "uniform vec4 shader_b;\n"\
      "void main (void) {\n"\
      "vec4 p = texture2D(src, gl_TexCoord[0].xy);\n"\
      "gl_FragColor = clamp((p + shader_b)*shader_a, 0.0, 1.0);\n"\
      "}";

    // std::string source = "in vec4 Color;\nvoid main()\n{\ngl_FragColor = Color;\n}\0";

    // std::cout<<"Shader source: "<<source<<std::endl;
    
    const char * cstr_source = source.c_str();
    GLint source_length = source.size();

    GlImageActor::m_StandardShaderProgram = glCreateProgram();
    GlImageActor::m_StandardShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource( GlImageActor::m_StandardShader, 1, &cstr_source,&source_length);
    glCompileShader(GlImageActor::m_StandardShader);

    GLint compiled;

    glGetShaderiv(m_StandardShader, GL_COMPILE_STATUS, &compiled);
    
    if(!compiled)
      {
      std::cerr<<"Standard shader failed to compile!"<<std::endl;

      int length;
      glGetShaderiv(m_StandardShader, GL_INFO_LOG_LENGTH , &length);

      char * logs = new char[length];
      glGetShaderInfoLog(m_StandardShader,1000,&length,logs);
      std::cerr<<logs<<std::endl;
      delete [] logs;

      }

    glAttachShader(GlImageActor::m_StandardShaderProgram, GlImageActor::m_StandardShader);
    glLinkProgram(GlImageActor::m_StandardShaderProgram);

    // // Check that shader is correctly loaded
    // glUseProgram(m_StandardShaderProgram);
    
    // int length;
    // glGetProgramiv(m_StandardShaderProgram,GL_INFO_LOG_LENGTH,&length);
    // char * logs = new char[length];
    // glGetProgramInfoLog(m_StandardShaderProgram,1000,NULL,logs);
    // std::cout<<logs<<std::endl;
    // delete [] logs;

    GlImageActor::m_ShaderInitialized = true;
    }
}

}
