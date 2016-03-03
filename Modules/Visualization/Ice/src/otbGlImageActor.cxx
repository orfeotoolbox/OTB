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
    m_ImageSettings( ImageSettings::New() ),
    m_Shader(),
    m_ViewportToImageTransform(),
    m_ImageToViewportTransform(),
    m_ViewportForwardRotationTransform(RigidTransformType::New()),
    m_ViewportBackwardRotationTransform(RigidTransformType::New()),
    m_ResolutionAlgorithm(ResolutionAlgorithm::Nearest),
    m_SoftwareRendering(false)
{}

GlImageActor
::~GlImageActor()
{
  // Release OpenGL texture names.
  for( TileVectorType::iterator it( m_LoadedTiles.begin() );
       it!=m_LoadedTiles.end();
       ++it )
    UnloadTile( *it );

  m_LoadedTiles.clear();
}

void
GlImageActor
::CreateShader()
{
  StandardShader::Pointer shader( StandardShader::New() );

  shader->SetImageSettings( m_ImageSettings );

  m_Shader = shader;
}

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


bool
GlImageActor
::HasKwl() const
{
  return true;
}


bool
GlImageActor
::GetKwl( ImageKeywordlist & kwl ) const
{
  assert( !m_FileReader.IsNull() );
  assert( m_FileReader->GetOutput()!=NULL );

  kwl = m_FileReader->GetOutput()->GetImageKeywordlist();

  return true;
}


GlImageActor::MetaDataDictionaryType & GlImageActor::GetMetaDataDictionary() const
{
  return m_FileReader->GetOutput()->GetMetaDataDictionary();
}


void GlImageActor::Initialize(const std::string & filename)
{
  // std::cout
  //   << std::hex << this << "::Initialize( '" << filename << "' )" << std::endl;

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

  unsigned int ovrCount = m_FileReader->GetOverviewsCount();

  m_AvailableResolutions.clear();

  m_AvailableResolutions.push_back(0);

  for(unsigned int i =0; i < ovrCount;++i)
    {
    m_AvailableResolutions.push_back(i+1);
    }

  m_CurrentResolution = m_AvailableResolutions.front();

  // Update transforms once data is read
  UpdateTransforms();

  // std::cout<<"Number of resolutions in file: "<<m_AvailableResolutions.size()<<std::endl;
}


void GlImageActor::GetExtent(double & ulx, double & uly, double & lrx, double & lry) const
{
  RegionType largest = m_FileReader->GetOutput()->GetLargestPossibleRegion();

  const_cast< GlImageActor * >( this )->UpdateTransforms();

  ImageRegionToViewportExtent(largest,ulx,uly,lrx,lry);
}

void GlImageActor::ProcessViewSettings()
{
  // Is there anything to do ?
  ViewSettings::ConstPointer settings = this->GetSettings();

  RigidTransformType::ParametersType rigidParameters(5);
  rigidParameters.Fill(0);
  rigidParameters[0]=settings->GetRotationAngle();
  rigidParameters[1]=settings->GetRotationCenter()[0];
  rigidParameters[2]=settings->GetRotationCenter()[1];

  m_ViewportForwardRotationTransform->SetParameters(rigidParameters);

  rigidParameters[0]=-settings->GetRotationAngle();
  
  m_ViewportBackwardRotationTransform->SetParameters(rigidParameters);
  
  UpdateTransforms();
  
  for (TileVectorType::iterator it = m_LoadedTiles.begin();
       it!=m_LoadedTiles.end();
       ++it)
    {
    // Do not rotate here, handled by opengl
    this->ImageRegionToViewportQuad(it->m_ImageRegion,it->m_UL,it->m_UR,it->m_LL,it->m_LR,false);
    }
}

void GlImageActor::UpdateData()
{
  // Update resolution needed
  UpdateResolution();

  // First, clean existing tiles
  CleanLoadedTiles();

  // Retrieve settings
  ViewSettings::ConstPointer settings = GetSettings();

  RegionType largest( m_FileReader->GetOutput()->GetLargestPossibleRegion() );
 
  double ulx, uly, lrx, lry;

  settings->GetViewportExtent(ulx,uly,lrx,lry);

  // std::cout<<"Viewport extent: "<<ulx<<", "<<uly<<" - "<<lrx<<", "<<lry<<std::endl;

  RegionType requested;

  ViewportExtentToImageRegion(ulx,uly,lrx,lry,requested);

  if( !requested.Crop( largest ) )
    return;

  // std::cout << "Requested: " << requested;
 
  // Now we have the requested part of image, we need to find the
  // corresponding tiles

  // First compute needed tiles
  unsigned int nbTilesX = vcl_ceil(static_cast<double>(requested.GetIndex()[0] + requested.GetSize()[0])/m_TileSize) -vcl_floor(static_cast<double>(requested.GetIndex()[0])/m_TileSize);
  unsigned int nbTilesY = vcl_ceil(static_cast<double>(requested.GetIndex()[1] + requested.GetSize()[1])/m_TileSize) -vcl_floor(static_cast<double>(requested.GetIndex()[1])/m_TileSize);
  //unsigned int nbTilesY = vcl_ceil(static_cast<double>(requested.GetSize()[1])/m_TileSize);
  unsigned int tileStartX = m_TileSize*(requested.GetIndex()[0]/m_TileSize);
  unsigned int tileStartY = m_TileSize*(requested.GetIndex()[1]/m_TileSize);

  // std::cout << std::endl;
  // std::cout << "Required tiles: " << nbTilesX << " x " << nbTilesY << std::endl;

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

      // std::cout << "Largest: " << largest;

      newTile.m_ImageRegion.Crop( largest );

      ImageRegionToViewportQuad(newTile.m_ImageRegion,newTile.m_UL,newTile.m_UR,newTile.m_LL,newTile.m_LR,false);

      // std::cout << "Tile: " << newTile.m_ImageRegion; // <<std::endl;
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
  // std::cout
  //   << "otb::GlImageActor@" << std::hex << this << std::dec << std::endl
  //   << "\tresolution: " << m_ResolutionAlgorithm << std::endl
  //   << "\tpixel: " << m_SoftwareRendering << std::endl
  //   << "\ttile: " << m_TileSize << std::endl;

  if( !m_SoftwareRendering && !m_Shader.IsNull() )
    {
    // std::cout << "\tGLSL" << std::endl;

    m_Shader->LoadShader();
    m_Shader->SetupShader();
    }
  else
    {
    // std::cout << "\tOTB" << std::endl;

    for(TileVectorType::iterator it = m_LoadedTiles.begin();
        it != m_LoadedTiles.end(); ++it)
      {
      if(!it->m_RescaleFilter)
        {
        it->m_RescaleFilter = RescaleFilterType::New();
        it->m_RescaleFilter->AutomaticInputMinMaxComputationOff();
        it->m_RescaleFilter->SetInput(it->m_Image);
        }
      
      VectorImageType::PixelType mins(3),maxs(3),omins(3),omaxs(3);
      mins.Fill(0);
      maxs.Fill(255);
        
      double gamma(1.0);

      bool useNoData(false);
      double noData(0.);

      assert( !m_ImageSettings.IsNull() );

      mins[ 0 ] = m_ImageSettings->GetMinRed();
      mins[ 1 ] = m_ImageSettings->GetMinGreen();
      mins[ 2 ] = m_ImageSettings->GetMinBlue();
    
      maxs[ 0 ] = m_ImageSettings->GetMaxRed();
      maxs[ 1 ] = m_ImageSettings->GetMaxGreen();
      maxs[ 2  ] = m_ImageSettings->GetMaxBlue();

      gamma = m_ImageSettings->GetGamma();

      if( m_ImageSettings->GetUseNoData() )
	noData = m_ImageSettings->GetNoData();
      
      omins.Fill( 0 );
      omaxs.Fill( 255 );
    
      it->m_RescaleFilter->SetInputMinimum(mins);
      it->m_RescaleFilter->SetInputMaximum(maxs);
      it->m_RescaleFilter->SetOutputMinimum(omins);
      it->m_RescaleFilter->SetOutputMaximum(omaxs);
      it->m_RescaleFilter->SetGamma(gamma);
      
      it->m_RescaleFilter->Update();


      itk::ImageRegionConstIterator<UCharVectorImageType> imIt(it->m_RescaleFilter->GetOutput(),it->m_RescaleFilter->GetOutput()->GetLargestPossibleRegion());
      itk::ImageRegionConstIterator<VectorImageType> inIt(it->m_Image,it->m_Image->GetLargestPossibleRegion());

      
      
      unsigned char * buffer = new unsigned char[4*it->m_RescaleFilter->GetOutput()->GetLargestPossibleRegion().GetNumberOfPixels()];
      
      unsigned int idx = 0;
      
      for(imIt.GoToBegin(),inIt.GoToBegin();!imIt.IsAtEnd()&&!inIt.IsAtEnd();++imIt,++inIt)
        {
        buffer[idx] = static_cast<unsigned char>(imIt.Get()[2]);
        ++idx;
        buffer[idx] = static_cast<unsigned char>(imIt.Get()[1]);
        ++idx;
        buffer[idx] = static_cast<unsigned char>(imIt.Get()[0]);
        ++idx;
        buffer[idx] = 255;

        if(useNoData && (inIt.Get()[0] == noData ||inIt.Get()[1] == noData ||inIt.Get()[2] == noData))
          {
          buffer[idx] = 0;
          }
        
        ++idx;
        }

      if(!it->m_TextureId)
        {
        glGenTextures(1, &(it->m_TextureId));
        }
      glBindTexture(GL_TEXTURE_2D, it->m_TextureId);
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
        it->m_RescaleFilter->GetOutput()->GetLargestPossibleRegion().GetSize()[0],
        it->m_RescaleFilter->GetOutput()->GetLargestPossibleRegion().GetSize()[1], 
        0, GL_BGRA, GL_UNSIGNED_BYTE,
        buffer);

      it->m_Loaded = true;
      
      delete [] buffer;
      }
    }

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

if(!m_SoftwareRendering)
  {
    m_Shader->UnloadShader();
  }

}

void GlImageActor::LoadTile(Tile& tile)
{
  // std::cout
  //   << std::hex << this
  //   << "::LoadTile(" << &tile << ")"
  //   << std::dec << std::endl;

  // std::cout
  //   << "[ " << tile.m_ImageRegion.GetIndex()[ 0 ]
  //   << ", " << tile.m_ImageRegion.GetIndex()[ 1 ]
  //   << " ]-[ " << tile.m_ImageRegion.GetSize()[ 0 ]
  //   << ", " << tile.m_ImageRegion.GetSize()[ 1 ]
  //   << "]"
  //   << std::endl;

  ExtractROIFilterType::Pointer extract = ExtractROIFilterType::New();

  extract->SetInput(m_FileReader->GetOutput());
  extract->SetExtractionRegion(tile.m_ImageRegion);
  extract->SetChannel(tile.m_RedIdx);
  extract->SetChannel(tile.m_GreenIdx);
  extract->SetChannel(tile.m_BlueIdx);

  // std::cout << "ExtractROIFilter::Update()...";
  extract->Update();
  // std::cout << "\tDONE\n";

  tile.m_Image = extract->GetOutput();

  if(!m_SoftwareRendering)
    {
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
    assert( tile.m_TextureId==0 );

    glGenTextures( 1, &tile.m_TextureId );
    assert( glGetError()==GL_NO_ERROR );

    // std::cout << "Generated texture #" << tile.m_TextureId << std::endl;

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
    }
    
    // And push to loaded texture
    m_LoadedTiles.push_back(tile);
}
  
void GlImageActor::UnloadTile(Tile& tile)
{
  // std::cout << std::hex << this << std::dec << "::UnloadTile()" << std::endl;

  if( tile.m_Loaded )
    {
    assert( tile.m_TextureId>0 );

    glDeleteTextures( 1, &tile.m_TextureId );

    // std::cout << "Deleted texture #" << tile.m_TextureId << std::endl;

    tile.m_TextureId = 0;

    tile.m_Image = VectorImageType::Pointer();

    tile.m_RescaleFilter = RescaleFilterType::Pointer();

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
       || it->m_ImageRegion.GetSize()[0]!=m_TileSize
       || it->m_ImageRegion.GetSize()[1]!=m_TileSize)
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

void GlImageActor::ImageRegionToViewportQuad(const RegionType & region, PointType & ul, PointType & ur, PointType & ll, PointType & lr, bool rotate) const
{
  // Retrieve settings
  ViewSettings::ConstPointer settings = this->GetSettings();
    
  itk::ContinuousIndex<double,2> cul,cur,cll,clr;
  
  cul[0] = region.GetIndex()[0];
  cul[1] = region.GetIndex()[1];
  cur = cul;
  cur[0]+=region.GetSize()[0]-1;
  cll = cul;
  cll[1]+=region.GetSize()[1]-1;
  clr = cur;
  clr[1]+=region.GetSize()[1]-1;

  PointType iul, iur,ill,ilr;

  m_FileReader->GetOutput()->TransformContinuousIndexToPhysicalPoint(cul,iul);
  m_FileReader->GetOutput()->TransformContinuousIndexToPhysicalPoint(cur,iur);
  m_FileReader->GetOutput()->TransformContinuousIndexToPhysicalPoint(cll,ill);
  m_FileReader->GetOutput()->TransformContinuousIndexToPhysicalPoint(clr,ilr);

  // Take into account that Origin refers to center of first pixel
  SpacingType spacing = m_FileReader->GetOutput()->GetSpacing();
  iul[0]-=0.5*spacing[0];
  iul[1]-=0.5*spacing[1];
  iur[0]+=0.5*spacing[0];
  iur[1]-=0.5*spacing[1];
  ill[0]-=0.5*spacing[0];
  ill[1]+=0.5*spacing[1];
  ilr[0]+=0.5*spacing[0];
  ilr[1]+=0.5*spacing[1];
  
  PointType pul = m_ImageToViewportTransform->TransformPoint(iul);
  PointType pur = m_ImageToViewportTransform->TransformPoint(iur);
  PointType pll = m_ImageToViewportTransform->TransformPoint(ill);
  PointType plr = m_ImageToViewportTransform->TransformPoint(ilr);

  if(rotate)
    {
    pul = m_ViewportBackwardRotationTransform->TransformPoint(pul);
    pur = m_ViewportBackwardRotationTransform->TransformPoint(pur);
    pll = m_ViewportBackwardRotationTransform->TransformPoint(pll);
    plr = m_ViewportBackwardRotationTransform->TransformPoint(plr);
    }
  
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


bool
GlImageActor
::GetPixelFromViewport( const PointType & in,
			PixelType & pixel ) const
{
  PointType p;
  IndexType i;

  return GetPixelFromViewport( in, pixel, p, i );
}


bool
GlImageActor
::GetPixelFromViewport( const PointType & ptView,
			PixelType & pixel,
			PointType & ptPhysical,
			IndexType & index ) const
{
  ptPhysical = ViewportToImageTransform( ptView );

  return GetPixel( ptPhysical, pixel, index );
}


bool
GlImageActor
::GetPixel( const PointType & physical,
	    PixelType & pixel,
	    IndexType & index ) const
{

  // std::cout << std::endl;

  // std::cout << "O: (" << m_Origin[ 0 ] << ", " << m_Origin[ 1 ] << ")";
  // std::cout << "\tS: (" << m_Spacing[ 1 ] << ", " << m_Spacing[ 1 ] << ")";
  // std::cout << std::endl;

  // std::cout << "P: (" << physical[ 0 ] << ", " << physical[ 1 ] << ")";
  // std::cout << std::endl;

  // First, we need to return index in full img (not in overviews)
#if 0
  index[ 0 ] = static_cast<unsigned int>((physical[0]-m_Origin[0])/m_Spacing[0]);
  index[ 1 ] = static_cast<unsigned int>((physical[1]-m_Origin[1])/m_Spacing[1]);

#else
  index[ 0 ] =
    static_cast< IndexType::IndexValueType >(
      ( physical[ 0 ] - m_Origin[ 0 ] ) / 
      m_Spacing[ 0 ]
    );

  index[ 1 ] =
    static_cast< IndexType::IndexValueType >(
      ( physical[ 1 ]-  m_Origin[ 1 ] ) /
      m_Spacing[ 1 ]
    );

#endif

  // std::cout << "I: (" << index[ 0 ] << ", " << index[ 1 ] << ")";
  // std::cout
  //   << "\tI: ("
  //   << ( ( physical[ 0 ] - m_Origin[ 0 ] ) / m_Spacing[ 0 ] )
  //   << ", "
  //   << ( ( physical[ 1 ] - m_Origin[ 1 ] ) / m_Spacing[ 1 ] )
  //   << ")";
  // std::cout << std::endl;

  // Then, we need to find the index in the currently loaded overview
  IndexType ovrIndex;
  m_FileReader->GetOutput()->TransformPhysicalPointToIndex( physical, ovrIndex );

  // std::cout << "O: (" << ovrIndex[ 0 ] << ", " << ovrIndex[ 1 ] << ")";
  // std::cout << std::endl;

  // And look it up in loaded tiles
  for (TileVectorType::const_iterator it = m_LoadedTiles.begin();
       it!=m_LoadedTiles.end();
       ++it)
    {
    // std::cout
    //   << "R: ("
    //   << it->m_ImageRegion.GetIndex()[ 0 ]
    //   << ", "
    //   << it->m_ImageRegion.GetIndex()[ 1 ]
    //   << ")-("
    //   << it->m_ImageRegion.GetSize()[ 0 ]
    //   << ", "
    //   << it->m_ImageRegion.GetSize()[ 1 ]
    //   << ")";

    if(it->m_ImageRegion.IsInside(ovrIndex))
      {
      IndexType idx;

      idx[ 0 ] = ovrIndex[ 0 ] - it->m_ImageRegion.GetIndex()[ 0 ];
      idx[ 1 ] = ovrIndex[ 1 ] - it->m_ImageRegion.GetIndex()[ 1 ];

      // std::cout << "\tIr: (" << idx[ 0 ] << ", " << idx[ 1 ] << ")";

      pixel = it->m_Image->GetPixel( idx );

      return true;
      }

    // std::cout << std::endl;
    }

  return false;
}



void GlImageActor::UpdateResolution()
{
  // Retrieve settings
  ViewSettings::ConstPointer settings = this->GetSettings();

  // Retrieve viewport spacing
  ViewSettings::SpacingType spacing = settings->GetSpacing();
  
  // 100 screen pixels
  PointType pointA, pointB, pointC;

  pointA  = settings->GetOrigin();
  pointB = pointA;
  pointC = pointA;
  
  pointB[0]+=100*spacing[0];
  pointC[1]+=100*spacing[1];

  // Transform the spacing vector
  pointA = m_ViewportForwardRotationTransform->TransformPoint(pointA);
  pointA = m_ViewportToImageTransform->TransformPoint(pointA);
  pointB = m_ViewportForwardRotationTransform->TransformPoint(pointB);
  pointB = m_ViewportToImageTransform->TransformPoint(pointB);
  pointC = m_ViewportForwardRotationTransform->TransformPoint(pointC);
  pointC = m_ViewportToImageTransform->TransformPoint(pointC);

  // Equivalence in image pixels
  pointA[0] = (pointA[0] - m_Origin[0])/m_Spacing[0];
  pointA[1] = (pointA[1] - m_Origin[1])/m_Spacing[1];
  pointB[0] = (pointB[0] - m_Origin[0])/m_Spacing[0];
  pointB[1] = (pointB[1] - m_Origin[1])/m_Spacing[1];
  pointC[0] = (pointC[0] - m_Origin[0])/m_Spacing[0];
  pointC[1] = (pointC[1] - m_Origin[1])/m_Spacing[1];

  double distAB = vcl_sqrt((pointA[0]-pointB[0])*(pointA[0]-pointB[0])+(pointA[1]-pointB[1])*(pointA[1]-pointB[1]));
  double distAC = vcl_sqrt((pointA[0]-pointC[0])*(pointA[0]-pointC[0])+(pointA[1]-pointC[1])*(pointA[1]-pointC[1]));
  
  double resolution = std::min(100/distAB,100/distAC);
  
  // Arbitrary higher than any distance we will compute here
  double minDist = 50000.;
  m_CurrentResolution = 0;

  bool isFound = false;

  // OTB always include full resolution level in available resolutions.
  assert( !m_AvailableResolutions.empty() );

  // Compute the diff and keep the index that minimize the distance
  for (ResolutionVectorType::iterator it = m_AvailableResolutions.begin();
       it != m_AvailableResolutions.end(); ++it)
    {

    double diff = 1/((double)(1<<(*it))) - resolution;

    if( ( ( m_ResolutionAlgorithm == ResolutionAlgorithm::Nearest_Lower &&
	    diff < 0 )
	  ||
	  ( m_ResolutionAlgorithm == ResolutionAlgorithm::Nearest_Upper &&
	    diff > 0 )
	  ||
	  ( m_ResolutionAlgorithm == ResolutionAlgorithm::Nearest ) )
	&&
	vcl_abs(diff) < minDist )
      {
      isFound = true;

      minDist = vcl_abs(diff);
      m_CurrentResolution = std::distance(m_AvailableResolutions.begin(),it);
      }
    }

  // MANTIS-1147: Cap current-resolution.
  if( !isFound )
    m_CurrentResolution = m_AvailableResolutions.size() - 1;

  std::ostringstream extFilename;
  extFilename<<m_FileName<<"?&resol="<<m_CurrentResolution;

  // ReaderType::New() is forced because of warning message
  // 'Duplicated option detected: <option>. Using value <value>.'
  // output by otb::ExtendedFilenameHelper.
  m_FileReader = ReaderType::New();
  m_FileReader->SetFileName(extFilename.str());
  m_FileReader->UpdateOutputInformation();

  // std::cout << "Switched to resolution: " << m_CurrentResolution << std::endl;
}

void GlImageActor::UpdateTransforms()
{
  // std::cout << std::hex << this << "::UpdateTransforms()" << std::endl;
  // std::cout << "{" << std::endl;

  // std::cout << "filename: " << m_FileName << std::endl;

  if(m_FileName == "")
    {
    // std::cout << "}" << std::endl;
    return;
    }

  // Retrieve settings
  ViewSettings::ConstPointer settings = this->GetSettings();

  bool hasChanged = false;

  bool geometryChanged = settings->GetGeometryChanged();

  if( ( !settings->GetUseProjection() && geometryChanged ) ||
      m_ViewportToImageTransform.IsNull() ||
      m_ImageToViewportTransform.IsNull())
    {
    // std::cout
    //   << "otb::GlImageActor@" << std::hex << this
    //   << " Proj: OFF" << std::endl;

    m_ViewportToImageTransform = RSTransformType::New();
    m_ImageToViewportTransform = RSTransformType::New();

    hasChanged = true;
    }

  // std::cout
  //   << std::hex << this << std::dec
  //   << " geometry-changed: " << geometryChanged << std::endl;

  // bool isEqualOrNot =
  //   m_ViewportToImageTransform->GetInputProjectionRef() != settings->GetWkt();

  // std::cout
  //   << std::hex << this << std::dec
  //   << " WKT-changed: " << isEqualOrNot << std::endl;

    geometryChanged = geometryChanged
  || (m_ViewportToImageTransform.IsNotNull() && m_ViewportToImageTransform->GetInputProjectionRef() != settings->GetWkt())
  || (m_ImageToViewportTransform.IsNotNull() && m_ImageToViewportTransform->GetOutputProjectionRef() != settings->GetWkt())
    || (m_ViewportToImageTransform.IsNotNull() && !(m_ViewportToImageTransform->GetInputKeywordList() == settings->GetKeywordList()))
        || (m_ImageToViewportTransform.IsNotNull() && !(m_ImageToViewportTransform->GetOutputKeywordList() == settings->GetKeywordList()));

  if(settings->GetUseProjection() && geometryChanged)
    {
    // std::cout
    //   << "otb::GlImageActor@" << std::hex << this << std::dec
    //   << " Proj: ON" << std::endl;

    m_ViewportToImageTransform = RSTransformType::New();
    m_ImageToViewportTransform = RSTransformType::New();

    m_ViewportToImageTransform->SetInputProjectionRef(settings->GetWkt());
    m_ViewportToImageTransform->SetInputKeywordList(settings->GetKeywordList());
    m_ViewportToImageTransform->SetOutputProjectionRef(m_FileReader->GetOutput()->GetProjectionRef());
    m_ViewportToImageTransform->SetOutputKeywordList(m_FileReader->GetOutput()->GetImageKeywordlist());

    m_ImageToViewportTransform->SetOutputProjectionRef(settings->GetWkt());
    m_ImageToViewportTransform->SetOutputKeywordList(settings->GetKeywordList());
    m_ImageToViewportTransform->SetInputProjectionRef(m_FileReader->GetOutput()->GetProjectionRef());
    m_ImageToViewportTransform->SetInputKeywordList(m_FileReader->GetOutput()->GetImageKeywordlist());

    hasChanged = true;
    }

  if( hasChanged )
    {
    // std::cout << std::hex << this << " -> InstanciateTransform()" << std::endl;

    m_ViewportToImageTransform->InstanciateTransform();
    m_ImageToViewportTransform->InstanciateTransform();
    }

  // std::cout << "}" << std::endl;
}

void GlImageActor::AutoColorAdjustment( double & minRed, double & maxRed,
					double & minGreen, double & maxGreen,
					double & minBlue, double & maxBlue,
					bool full,
					unsigned int refSize,
					double lcp, double hcp )
{
  typedef itk::Statistics::ListSample<VectorImageType::PixelType> ListSampleType;
  typedef itk::Statistics::DenseFrequencyContainer2 DFContainerType;
  typedef ListSampleToHistogramListGenerator<ListSampleType, VectorImageType::InternalPixelType, DFContainerType> HistogramsGeneratorType;

  assert( refSize>0 );

  ListSampleType::Pointer listSample = ListSampleType::New();
  listSample->SetMeasurementVectorSize(3);
  
  if(full)
    {
    // Retrieve the lowest resolution
    ReaderType::Pointer reader = ReaderType::New();
    std::ostringstream extFilename;

    unsigned int nb_pixel_ref = refSize * refSize;

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


bool
GlImageActor
::TransformFromViewport( Point2d & out,
                         const Point2d & in,
                         bool isPhysical ) const
{
  out = ViewportToImageTransform( in, isPhysical );

  return true;
}


bool
GlImageActor
::TransformToViewport( Point2d & out,
                       const Point2d & in,
                       bool isPhysical ) const
{
  out = ViewportToImageTransform( in, isPhysical );

  return true;
}


} // End of namespace 'otb'.
