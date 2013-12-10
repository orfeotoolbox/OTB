#include "otbGPUTileLoader.h"

#include "itkImageRegionConstIterator.h"

#include <GL/gl.h> 

namespace otb
{

GPUTileLoader::GPUTileLoader()
  : m_Running(false),
    m_FileName(),
    m_ImageFileReader(ReaderType::New()),
    m_LoadingQueue(),
    m_LoadedTiles(),
    m_Resolution(1),
    m_RedIdx(1),
    m_GreenIdx(2),
    m_BlueIdx(3),
    m_ULX(0),
    m_ULY(0),
    m_LRX(0),
    m_LRY(0),
    m_TileSize(256),
    m_MinRed(200),
    m_MaxRed(1024),
    m_MinGreen(200),
    m_MaxGreen(1024),
    m_MinBlue(200),
    m_MaxBlue(1024),
    m_Threader(ThreaderType::New()),
    m_ThreadId(0)
{}

GPUTileLoader::~GPUTileLoader()
{
  // Ensure GPU memory is released
  this->ClearLoadedTiles();
}

void GPUTileLoader::Render()
{
  //std::cout<<"Render triggered, tile cache size: "<<m_LoadedTiles.size()<<std::endl;

  for(TileFIFOType::iterator it = m_LoadedTiles.begin();
      it != m_LoadedTiles.end(); ++it)
    {
    // std::cout<<"Loading texture: "<<it->m_TextureId<<" "<<it->m_ULX<<" "<<it->m_ULY<<" "<<it->m_LRX<<" "<<it->m_LRY<<std::endl;

     glEnable(GL_TEXTURE_2D);  
     glBindTexture(GL_TEXTURE_2D,it->m_TextureId);
    
    // Reset color before rendering
    glColor3d(1.0f,1.0f,1.0f);

    glBegin (GL_QUADS);
    glTexCoord2f (0.0, 1.0); glVertex2f(it->m_ULX,it->m_LRY);
    glTexCoord2f (1.0, 1.0); glVertex2f(it->m_LRX, it->m_LRY);
    glTexCoord2f (1.0, 0.0); glVertex2f(it->m_LRX, it->m_ULY);
    glTexCoord2f (0.0, 0.0); glVertex2f(it->m_ULX, it->m_ULY);
    glEnd ();

    glDisable(GL_TEXTURE_2D);
    }

}

void GPUTileLoader::Initialize(std::string filename)
{
  // Ensure loader is stopped
  this->Stop();

  m_FileName = filename;

  m_ImageFileReader = ReaderType::New();
  m_ImageFileReader->SetFileName(m_FileName);
  m_ImageFileReader->UpdateOutputInformation();

  m_ImageSize = m_ImageFileReader->GetOutput()->GetLargestPossibleRegion().GetSize();
}

void GPUTileLoader::UpdateResolution(unsigned int resolution)
{}

void GPUTileLoader::UpdateViewport(double ulx, double uly, double lrx, double lry)
{
  // Crop to ensure image fit
  ulx = ulx < 0 ? 0 : ulx;
  uly = uly < 0 ? 0 : uly;
  lrx = lrx > m_ImageSize[0] ? m_ImageSize[0] : lrx;
  lry = lry > m_ImageSize[1] ? m_ImageSize[1] : lry;

  m_ULX = ulx;
  m_ULY = uly;
  m_LRX = lrx;
  m_LRY = lry;

  // First compute needed tiles
  // TODO: Update this with geometry changes
  unsigned int nbTilesX = vcl_ceil((lrx-ulx)/m_TileSize);
  unsigned int nbTilesY = vcl_ceil((lrx-ulx)/m_TileSize);
  unsigned int tileStartX = m_TileSize*(static_cast<unsigned int>(vcl_ceil(ulx))/m_TileSize);
  unsigned int tileStartY = m_TileSize*(static_cast<unsigned int>(vcl_ceil(uly))/m_TileSize);

  for(unsigned int i = 0; i < nbTilesX; ++i)
    {
    for(unsigned int j = 0; j<nbTilesY; ++j)
      {
      Tile newTile;
      
      newTile.m_Loaded = false;
      // Is this really usefull ?
      newTile.m_TileIdx = 0;
      newTile.m_TextureId = 0;
      newTile.m_ULX = static_cast<double>(tileStartX+i*m_TileSize);
      newTile.m_ULY = static_cast<double>(tileStartY+j*m_TileSize);
      newTile.m_LRX = static_cast<double>(tileStartX+(i+1)*m_TileSize);
      newTile.m_LRY = static_cast<double>(tileStartY+(j+1)*m_TileSize);
      newTile.m_RedIdx = m_RedIdx;
      newTile.m_GreenIdx = m_GreenIdx;
      newTile.m_BlueIdx = m_BlueIdx;
      newTile.m_Resolution = 1;
      
        // Crop to ensure image fit
      newTile.m_ULX = newTile.m_ULX < 0 ? 0 : newTile.m_ULX;
      newTile.m_ULY = newTile.m_ULY < 0 ? 0 : newTile.m_ULY;
      newTile.m_LRX = newTile.m_LRX > m_ImageSize[0] ? m_ImageSize[0] : newTile.m_LRX;
      newTile.m_LRY = newTile.m_LRY > m_ImageSize[1] ? m_ImageSize[1] : newTile.m_LRY;

      m_LoadingQueue.push_back(newTile);
      }
    }
}

void GPUTileLoader::UpdateGeometry()
{}

void GPUTileLoader::UpdateColor(unsigned int red, unsigned int green, unsigned int blue)
{}

void GPUTileLoader::Update()
{
  this->ProcessingLoop();
}

void GPUTileLoader::Start()
{
  m_Running = true;
  m_ThreadId = m_Threader->SpawnThread(StaticThreadedRun,this);
}

void GPUTileLoader::Stop()
{
  m_Running = false;
}

void GPUTileLoader::ProcessingLoop()
{
  while(!m_LoadingQueue.empty())
    {
    Tile nextTile = m_LoadingQueue.front();
    m_LoadingQueue.pop_front();
    // std::cout<<"Received tile "<<nextTile.m_ULX<<" "<<nextTile.m_ULY<<" "<<nextTile.m_LRX<<" "<<nextTile.m_LRY<<std::endl;
    
    // If tile already exists, discard
    if(!TileAlreadyLoaded(nextTile))
      {
      this->LoadTile(nextTile);
      }
    // else
    //   {
    //   std::cout<<"Tile "<<nextTile.m_ULX<<" "<<nextTile.m_ULY<<" "<<nextTile.m_LRX<<" "<<nextTile.m_LRY<<" already loaded"<<std::endl;
      
    //   }

    // TODO: Missing notify here (for tilewise refresh)

    // Remove any useless tile
    CleanLoadedTiles();
    

    }

}

bool GPUTileLoader::TileAlreadyLoaded(const Tile& tile)
{
  for(TileFIFOType::iterator it = m_LoadedTiles.begin();
      it!=m_LoadedTiles.end();
      ++it)
    {
    if(it->m_ULX == tile.m_ULX
       && it->m_LRX == tile.m_LRX
       && it->m_LRY == tile.m_LRY
       && it->m_ULY == tile.m_ULY)
      {
      return (tile.m_Resolution ==  it->m_Resolution
              && tile.m_RedIdx == it->m_RedIdx
              && tile.m_GreenIdx == it->m_GreenIdx
              && tile.m_BlueIdx == it->m_BlueIdx);
      
      }
    }

  return false;
}

void GPUTileLoader::LoadTile(Tile& tile)
{
  ExtractROIFilterType::Pointer extract = ExtractROIFilterType::New();
  extract->SetInput(m_ImageFileReader->GetOutput());
  extract->SetStartX(static_cast<unsigned int>(tile.m_ULX));
  extract->SetStartY(static_cast<unsigned int>(tile.m_ULY));
  extract->SetSizeX(static_cast<unsigned int>(tile.m_LRX - tile.m_ULX));
  extract->SetSizeY(static_cast<unsigned int>(tile.m_LRY - tile.m_ULY));
  extract->SetChannel(tile.m_RedIdx);
  extract->SetChannel(tile.m_GreenIdx);
  extract->SetChannel(tile.m_BlueIdx);

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
  
#if defined(GL_CLAMP_TO_BORDER)      
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
#elif defined (GL_CLAMP_TO_BORDER_EXT)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER_EXT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER_EXT);
#elif defined (GL_MIRRORED_REPEAT)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
#endif
  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_RGBA8,
    rescale->GetOutput()->GetLargestPossibleRegion().GetSize()[0],
    rescale->GetOutput()->GetLargestPossibleRegion().GetSize()[1], 
    0, GL_BGRA, GL_UNSIGNED_BYTE,
    buffer);

  tile.m_Loaded = true;
  
  delete [] buffer;

  // And push to loaded texture
  m_LoadedTiles.push_back(tile);
}

void GPUTileLoader::UnloadTile(Tile & tile)
{
  if(tile.m_Loaded)
    {
    glDeleteTextures(1,&tile.m_TextureId);
    tile.m_Loaded = false;
    }
}

void GPUTileLoader::ClearLoadedTiles()
{
  for(TileFIFOType::iterator it = m_LoadedTiles.begin();
  it!=m_LoadedTiles.end();++it)
    {
    UnloadTile(*it);
    }
  m_LoadedTiles.clear();
}

void GPUTileLoader::CleanLoadedTiles()
{
  TileFIFOType newLoadedTiles;

  for(TileFIFOType::iterator it = m_LoadedTiles.begin();
  it!=m_LoadedTiles.end();++it)
    {
    if(it->m_LRX < m_ULX
       || it->m_ULX > m_LRX
       || it->m_LRY < m_ULY
       || it->m_ULY > m_LRY
       || it->m_Resolution!= m_Resolution
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
  m_LoadedTiles.swap(newLoadedTiles);
}



ITK_THREAD_RETURN_TYPE GPUTileLoader::StaticThreadedRun(void * t)
{
  struct itk::MultiThreader::ThreadInfoStruct * pInfo = (itk::MultiThreader::ThreadInfoStruct *) (t);
  GPUTileLoader *  lThis = (GPUTileLoader*) (pInfo->UserData);
  lThis->ThreadedRun();
  return 0;
}

int GPUTileLoader::Sleep(unsigned int microsec)
{
  return Threads::Sleep(microsec);
}

void GPUTileLoader::ThreadedRun()
{
  while(m_Running)
    {
    this->ProcessingLoop();
    this->Sleep();
    }
  std::cout<<"TileLoader stopping"<<std::endl;
}

}
