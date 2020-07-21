
#include "otbDEMHandler.h"
#include "otbGDALDriverManagerWrapper.h"
#include "boost/filesystem.hpp"
#include "gdal_utils.h"


namespace otb {
// Adapted from boost filesystem documentation : https://www.boost.org/doc/libs/1_53_0/libs/filesystem/doc/reference.html
std::vector<std::string> GetFilesInDirectory(const std::string & directoryPath)
{
  std::vector<std::string> fileList;
  
  if ( !boost::filesystem::exists( directoryPath) )
  {
    return fileList;
  }
  else if (!boost::filesystem::is_directory(directoryPath))
  {
    fileList.push_back(directoryPath);
    return fileList;
  }
  
  boost::filesystem::directory_iterator end_itr; // default construction yields past-the-end
  for ( boost::filesystem::directory_iterator itr( directoryPath ); itr != end_itr; ++itr )
  {
    if ( boost::filesystem::is_directory(itr->status()) )
    {
      auto subDirList = GetFilesInDirectory(itr->path().string());
      fileList.insert(fileList.end(), subDirList.begin(), subDirList.end());
    }
    else
    {
      fileList.push_back(itr->path().string());
    }
  }
  return fileList;
}

/** Initialize the singleton */
DEMHandler::Pointer DEMHandler::m_Singleton = nullptr;

DEMHandler::Pointer DEMHandler::Instance()
{
  if (m_Singleton.GetPointer() == nullptr)
  {
    m_Singleton = itk::ObjectFactory<Self>::Create();

    if (m_Singleton.GetPointer() == nullptr)
    {
      m_Singleton = new DEMHandler;
    }
    m_Singleton->UnRegister();
  }

  return m_Singleton;
}


DEMHandler::DEMHandler() : m_Dataset(nullptr),
                           m_shiftedDS(nullptr),
                           m_GeoidDS(nullptr),
                           m_ApplyVerticalDatum(false), 
                           m_DefaultHeightAboveEllipsoid(0.0)
{
  GDALAllRegister();
  INIT_RASTERIO_EXTRA_ARG(m_RasterIOArgs);

  //TODO : Setter for the resampling algorithm
  // In OSSIM bilinear resampling is used
  m_RasterIOArgs.eResampleAlg = GDALRIOResampleAlg::GRIORA_Bilinear;
};

void DEMHandler::OpenDEMFile(const std::string & path)
{
  m_DatasetList.push_back(otb::GDALDriverManagerWrapper::GetInstance().Open(path));
  m_Dataset = m_DatasetList.back()->GetDataSet();
  m_DEMDirectories.push_back(path);
  
  if (m_GeoidDS)
  {
    CreateShiftedDataset();
  }
}

void DEMHandler::OpenDEMDirectory(const std::string& DEMDirectory)
{
  auto demFiles = GetFilesInDirectory(DEMDirectory);
  
  for (const auto & file : demFiles)
  {
    auto ds = otb::GDALDriverManagerWrapper::GetInstance().Open(file);
    if (ds)
    {  
      m_DatasetList.push_back(ds );
    }
  }
  
  int vrtSize = m_DatasetList.size();
 
  // Don't build a vrt if there is less than two dataset
  if (m_DatasetList.empty())
  {
    m_Dataset = nullptr;
  }
  else
  {
    if (vrtSize == 1)
    {
      m_Dataset = m_DatasetList[0]->GetDataSet();
      m_DEMDirectories.push_back(DEMDirectory);
    }
    else
    {
      auto vrtDatasetList = new GDALDatasetH[vrtSize];
      for (int i = 0; i < vrtSize; i++)
      {
        vrtDatasetList[i] = m_DatasetList[i]->GetDataSet();
      }

      int error = 0;

      m_Dataset = (GDALDataset *) GDALBuildVRT(nullptr, vrtSize, vrtDatasetList, 
                                                nullptr, nullptr, &error);
      m_DEMDirectories.push_back(DEMDirectory);
      delete[] vrtDatasetList;
    }
    
    if (m_GeoidDS)
    {
      CreateShiftedDataset();
    }
  }
}

void DEMHandler::OpenDEMDirectory(const char* DEMDirectory)
{ 
  OpenDEMDirectory(DEMDirectory);
}

boost::optional<double> DEMHandler::GetDEMValue(double lon, double lat, GDALDataset* ds)
{std::cout << "hey " << m_Dataset << " " << m_GeoidDS << " " << m_shiftedDS << " " << std::endl;
  double value= 0.;
  GDALRasterIOExtraArg extraArgs = m_RasterIOArgs;
  
  extraArgs.nVersion = 1;
  
  double geoTransform[6];
  
  ds->GetGeoTransform(geoTransform);

  auto x = (lon - geoTransform[0]) / geoTransform[1] - 0.5;
  auto y = (lat - geoTransform[3]) / geoTransform[5] - 0.5;
  std::cout <<"x="<< x << " y=" << y << std::endl;
  
  if (x < 0 || y < 0 || x > ds->GetRasterXSize() || y > ds->GetRasterYSize())
  {
    std::cout << "invalid coordinates" << std::endl;
    return boost::none;
  }

  extraArgs.bFloatingPointWindowValidity = TRUE;
  extraArgs.dfXOff = x;
  extraArgs.dfYOff = y;
  extraArgs.dfXSize = 1;
  extraArgs.dfYSize = 1;

  auto err = ds->GetRasterBand(1)->RasterIO( GF_Read, static_cast<int>(x), static_cast<int>(y), 1, 1,
              &value, 1, 1, GDT_Float64,
              0, 0, &extraArgs);
  
  std::cout << "value :" << value << std::endl;
  return value;
}

bool DEMHandler::OpenGeoidFile(const char* geoidFile)
{
  int pbError;

  m_GeoidDS = static_cast<GDALDataset*>(GDALOpenVerticalShiftGrid(geoidFile, &pbError));
  m_GeoidFilename = geoidFile;

  if (m_Dataset)
  {
    CreateShiftedDataset();
  }
}

bool DEMHandler::OpenGeoidFile(const std::string& geoidFile)
{
  OpenGeoidFile(geoidFile.c_str());
}

void DEMHandler::CreateShiftedDataset()
{
  char **papszOptions = nullptr;
  // Geoid resampling is bilinear in OSSIM
  papszOptions = CSLSetNameValue( papszOptions, "RESAMPLING", "BILINEAR" ); // RESAMPLING=NEAREST/BILINEAR/CUBIC
  papszOptions = CSLSetNameValue( papszOptions, "MAX_ERROR", "0" ); //MAX_ERROR=val
  papszOptions = CSLSetNameValue( papszOptions, "DATATYPE", "Float64" ); //DATATYPE=Byte/UInt16/Int16/Float32/Float64
  // ERROR_ON_MISSING_VERT_SHIFT=YES/NO
  // SRC_SRS=srs_def 

  m_shiftedDS = static_cast<GDALDataset*>( GDALApplyVerticalShiftGrid( m_Dataset,
                                            m_GeoidDS,
                                           false,
                                           1.0,
                                           1.0,
                                           papszOptions ));

  m_ApplyVerticalDatum = true;
}

double DEMHandler::GetHeightAboveEllipsoid(double lon, double lat)
{
  auto ds = m_Dataset && m_GeoidDS ? m_shiftedDS
           : m_Dataset ? m_Dataset
           : m_GeoidDS ? m_GeoidDS
           : nullptr;

  if (ds)
  {
    auto result = GetDEMValue(lon, lat, ds);
    if (result)
    {
      return *result;
    }
  }

  return m_DefaultHeightAboveEllipsoid;
}

double DEMHandler::GetHeightAboveEllipsoid(const PointType& geoPoint)
{
  return GetHeightAboveEllipsoid(geoPoint[0], geoPoint[1]);
}

double DEMHandler::GetHeightAboveMSL(double lon, double lat)
{
  if (m_Dataset)
  { 
    auto result = GetDEMValue(lon, lat, m_Dataset);
    
    if (result)
    {
      return *result;
    }
  }

  return 0;
}

double DEMHandler::GetHeightAboveMSL(const PointType& geoPoint)
{
  return GetHeightAboveMSL(geoPoint[0], geoPoint[1]);
}

unsigned int DEMHandler::GetDEMCount() const
{
  return m_DatasetList.size();
}

bool DEMHandler::IsValidDEMDirectory(const char* DEMDirectory)
{
  for (const auto & filename : GetFilesInDirectory(DEMDirectory))
  {
    // test if a driver can identify this dataset
    auto identifyDriverH = static_cast<GDALDriver *>(GDALIdentifyDriver(filename.c_str(), nullptr));
    if (identifyDriverH)
    {
      return true;
    }
  }

  return false;
}

std::string DEMHandler::GetDEMDirectory(unsigned int idx) const
{
  if (idx >= m_DEMDirectories.size())
    return "";
  else
    return m_DEMDirectories[idx];
}

std::string DEMHandler::GetGeoidFile() const
{
  return m_GeoidFilename;
}

void DEMHandler::ClearDEMs()
{
  m_DEMDirectories.clear();

  GDALClose(m_shiftedDS);
  // This will call GDALClose on all datasets
  m_DatasetList.clear();
}

}
