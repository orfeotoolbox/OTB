

#include "otbVectorImage.h"
#include "otbMultiChannelExtractROI.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "otbImageFileReader.h"
#include "otbThreads.h"

#include <deque>
namespace otb
{

class Tile
{
public:
  unsigned int m_Loaded;
  unsigned int m_TileIdx;
  unsigned int m_TextureId;
  double m_ULX;
  double m_ULY;
  double m_LRX;
  double m_LRY;
  unsigned int m_Resolution;
  unsigned int m_RedIdx;
  unsigned int m_GreenIdx;
  unsigned int m_BlueIdx;
};

class GPUTileLoader 
  : public itk::LightObject
{
public:
  typedef GPUTileLoader   Self;
  typedef itk::LightObject Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);

  typedef otb::VectorImage<float>               VectorImageType;
  typedef otb::ImageFileReader<VectorImageType> ReaderType;
  typedef otb::MultiChannelExtractROI<float,float> ExtractROIFilterType;
  typedef otb::VectorRescaleIntensityImageFilter<VectorImageType,VectorImageType> RescaleFilterType;
  typedef std::deque<Tile>                      TileFIFOType;

  typedef itk::MultiThreader                    ThreaderType;

  void Initialize(std::string filename);
  
  void UpdateResolution(unsigned int resolution = 1);

  void UpdateViewport(double ulx, double uly, double lrx, double lry);

  void UpdateGeometry();

  void UpdateColor(unsigned int red = 1, unsigned int green = 2, unsigned int blue = 3);

  void Render();

  void Update();

  void Start();

  void Stop();

protected:
  GPUTileLoader();

  ~GPUTileLoader();

private:
  void ProcessingLoop();

  void LoadTile(Tile& tile);

  void UnloadTile(Tile& tile);

  bool TileAlreadyLoaded(const Tile& tile);

  void ClearLoadedTiles();

  void CleanLoadedTiles();

  static ITK_THREAD_RETURN_TYPE StaticThreadedRun(void * t);
  
  void ThreadedRun();

  static int Sleep(unsigned int microsec = 500);
  
  bool m_Running;

  std::string m_FileName;

  ReaderType::Pointer m_ImageFileReader;

  VectorImageType::SizeType m_ImageSize;

  // Tiles to process
  TileFIFOType m_LoadingQueue;

  // Tiles cache
  TileFIFOType m_LoadedTiles;

  VectorImageType::SizeType m_Size;

  unsigned int m_Resolution;

  unsigned int m_RedIdx;

  unsigned int m_GreenIdx;

  unsigned int m_BlueIdx;

  // The viewport
  double m_ULX;
  double m_ULY;
  double m_LRX;
  double m_LRY;

  unsigned int m_TileSize;

  double m_MinRed;
  double m_MaxRed;
  double m_MinBlue;
  double m_MaxBlue;
  double m_MinGreen;
  double m_MaxGreen;

  ThreaderType::Pointer m_Threader;

  int m_ThreadId;

};
}
