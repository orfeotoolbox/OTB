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

#include "otbTileMapFetcher.h"
#include "itkImageFileReader.h"

namespace otb
{
  /** Constructor */
  TileMapFetcher::TileMapFetcher()
  {
    // Set server name
    m_ServerName = "";
    
    // Set Extension
    m_FileSuffix = ".png";
    
    // Set Cache size
    m_CacheSize = 100;
    
    // Set cache directory
    m_CacheDirectory = "";
    
    // Set coordinates
    m_X = 0;
    m_Y = 0;
    m_Z = 1;
    
    // Generate Logo tile
    this->GenerateLogoTile();
  }
  
  /** Destructor */
  TileMapFetcher::~TileMapFetcher()
  {
    // Remove all tiles in cache directory
    this->UnregisterAllCachedTiles();
  }
  
  /** FetchTile */
  TileMapFetcher::ImagePointerType TileMapFetcher::FetchTile(unsigned int x, unsigned int y, unsigned int z)
  {
    bool isInCache = false;
    bool fetchingIsOK = true;
    
    /* Set coordinates */
    m_X = x;
    m_Y = y;
    m_Z = z;
    
    /* Create filename */
    this->GenerateFileName();
    
    /* Check if tile is in cache */
    isInCache = this->IsTileInCache();
    
    /* Fetch tile from web (check if local tile is out to date) */
    if(!isInCache)
    {
      // If fetching is ok
      if( this->FetchTileFromServer() )
      {
        fetchingIsOK = true;
        // Register tile in cache
        this->RegisterTileInCache();
      }
      else
      {
        // Else get tile from cache
        fetchingIsOK = false;
      }
        
    }
    
    /* fetch tile from cache */
    return (this->ReadTileFromCache(fetchingIsOK));
  }
  
  /** Fetch Tile from Server and set it in cache */
  bool TileMapFetcher::FetchTileFromServer()
  {
    bool result = false;
    
    // Path and filename for this tile
    std::ostringstream pathAndFileName;
    
    pathAndFileName << m_CacheDirectory;
    
    if (m_CacheDirectory.at(m_CacheDirectory.size()-1) != '/')
      pathAndFileName << "/";
    
    pathAndFileName << m_FileName;
    
    // Open file
    FILE* output_file = fopen(pathAndFileName.str().c_str(),"w");
    if (output_file == NULL)
    {
      itkExceptionMacro(<<"FetchTileFromServer : bad file name.");
      return result;
    }
    
    // Browser type
    std::ostringstream browserStream;
    browserStream   << "Mozilla/5.0 (Windows; U; Windows NT 6.0; en-GB; rv:1.8.1.11) Gecko/20071127 Firefox/2.0.0.11";
    
    // Init curl
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    
    // Generate URL
    this->GenerateOSMurl();
    
    if (m_OsmUrl == "")
    {
      itkExceptionMacro(<<"FetchTileFromServer : bad url.");
      return false;
    }
    
    char url[200];
    strcpy(url,m_OsmUrl.data());
    
    char browser[200];
    strcpy(browser,browserStream.str().data());
    
    //Download the file
    if (curl)
    {
      curl_easy_setopt(curl, CURLOPT_USERAGENT, browser);
      curl_easy_setopt(curl, CURLOPT_URL, url);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, output_file);
      res = curl_easy_perform(curl);
      
      // If res != 0 there is a problem while downloading (connection error)
      if (res != 0)
      {
        // Close and remove file in cache
        fclose(output_file);
        if (remove(pathAndFileName.str().c_str()) != 0)
        {
          itkDebugMacro(<<"FetchTileFromServer : transfert error - Error while deleting tile in cache!");
        }
        itkDebugMacro(<<"FetchTileFromServer : transfert error.");
        return false;
      }
      // Else download was proceed
      else
      {
        // Get error code
        int code;
        curl_easy_getinfo(curl,CURLINFO_HTTP_CODE, &code);
        
        // 404 Error (not found)
        if (code == 404)
        {
          // Close and remove file in cache
          fclose(output_file);
          if (remove(pathAndFileName.str().c_str()) != 0)
          {
            itkDebugMacro(<<"FetchTileFromServer : HTTP Error 404 - Error while deleting tile in cache!");
          }
          itkDebugMacro(<<"FetchTileFromServer : HTTP Error 404");
          return false;
        }
        // Tile is ok
        else
        {
          result = true;
        }
      }
    }
    // close file
    fclose(output_file);
    // Cleanup curl
    curl_easy_cleanup(curl);
    return result;
  }
  
  /** Generate Tile FileName */
  void TileMapFetcher::GenerateFileName()
  {
    std::string server = m_ServerName.substr(7,m_ServerName.size());  // Get online server name without "http://"
    
    // replace "/" by "_" in server name
    for(unsigned int i=0; i<server.size(); i++)
    {
      if (server[i] == '/')
        server[i] = '_'; 
    }
    
    std::ostringstream filename;
    
    // Generate file name : serverName_Z_X_Y.png
    filename<<server;
    filename<<m_Z;
    filename<<"_";
    filename<<m_X;
    filename<<"_";
    filename<<m_Y;
    filename<<m_FileSuffix;
    
    m_FileName = filename.str();
  }
  
  /** Generate OSM url */
  void TileMapFetcher::GenerateOSMurl()
  {
    /* Check server name */
    if  (m_ServerName[0]!='h'
         || m_ServerName[1]!='t'
         || m_ServerName[2]!='t'
         || m_ServerName[3]!='p')
    {
      itkExceptionMacro(<<"Bad server name!");
    }
    
    /* Generate urlStream */
    std::ostringstream urlStream;
    
    urlStream << m_ServerName;
    if (m_ServerName.at(m_ServerName.size()-1) != '/')
      urlStream << "/";
    urlStream << m_Z;
    urlStream << "/";
    urlStream << m_X;
    urlStream << "/";
    urlStream << m_Y;
    urlStream << m_FileSuffix;
    
    m_OsmUrl = urlStream.str();
  }
  
  /** Check if tile is in cache */
  bool TileMapFetcher::IsTileInCache()
  {
    bool result = false;
    
    // First if filename exists
    if ( m_FileName == "" )
    {
      itkDebugMacro(<<"No filename specified.");
      return result;
    }
    
    std::deque<otb::TileRecord>::iterator it;
    
    // Search tile with its filename
    for(it=m_CacheManagement.begin(); it!=m_CacheManagement.end(); it++)
    {
      if(it->GetFileName() == m_FileName)
      {
        result = true;
        break;
      }
    }
    return result;
  }
  
  /** Register Tile in cache */
  bool TileMapFetcher::RegisterTileInCache()
  {
    otb::TileRecord tr;
    
    // Configure Record
    tr.SetFileName(m_FileName);
    tr.SetDate();
    
    if(m_CacheManagement.size() > m_CacheSize)
    {
      this->UnregisterFirstCachedTile();
    }
    
    // Add record in deque
    m_CacheManagement.push_back(tr);
    
    return true;
  }
  
  /** Unregister Cached Tile */
  bool TileMapFetcher::UnregisterFirstCachedTile()
  {
    std::deque<otb::TileRecord>::iterator it;
    
    it=m_CacheManagement.begin();
    
    // Filename of the first input tile
    std::string filename = it->GetFileName();
    
    // Generate path and filename
    std::ostringstream pathAndFileName;
    
    pathAndFileName << m_CacheDirectory;
    
    if (m_CacheDirectory.at(m_CacheDirectory.size()-1) != '/')
      pathAndFileName << "/";
    
    pathAndFileName << filename;
    
    // Remove tile in deque
    m_CacheManagement.pop_front();
    
    // Remove tile file on hard drive
    if (remove(pathAndFileName.str().c_str()) != 0)
    {
      itkDebugMacro(<<"Error when deleting tile in cache!");
      return false;
    }
    return true;
  }
  
  /** Remove all tiles in cache */
  bool TileMapFetcher::UnregisterAllCachedTiles()
  {
    bool result = true;
    std::deque<otb::TileRecord>::iterator it;
    std::string filename;
    
    /* Loop on deque to get filename and delete file */
    for(it=m_CacheManagement.begin(); it!=m_CacheManagement.end(); it++)
    {
      filename = it->GetFileName();
      
      std::ostringstream pathAndFileName;
      
      pathAndFileName << m_CacheDirectory;
      
      if (m_CacheDirectory.at(m_CacheDirectory.size()-1) != '/')
        pathAndFileName << "/";
      
      pathAndFileName << filename;
      
      if (remove(pathAndFileName.str().c_str()) != 0)
      {
        itkDebugMacro(<<"Error when deleting tile in cache!");
        result = false;
      }
    }
    // Clear deque
    m_CacheManagement.clear();
    return result;
  }
  
  /** Read tile in cache directory */
  TileMapFetcher::ImagePointerType TileMapFetcher::ReadTileFromCache(bool fetchingIsOK)
  {
    // Create an output image
    ImagePointerType tempImage;
    
    // If fetch tile from server is ok
    if (fetchingIsOK)
    {
      // Create a reader
      itk::ImageFileReader<ImageType>::Pointer reader = itk::ImageFileReader<ImageType>::New();
      
      // Create path and filename
      std::ostringstream pathAndFileName;
      
      pathAndFileName << m_CacheDirectory;
      
      if (m_CacheDirectory.at(m_CacheDirectory.size()-1) != '/')
        pathAndFileName << "/";
      
      pathAndFileName << m_FileName;
      
      // Read file in cache directory
      reader->SetFileName(pathAndFileName.str());
      reader->Update();

      tempImage = reader->GetOutput();
      
    }
    // Else we return logo tile
    else
    {
      // Read logo tile
      tempImage = m_LogoTile;
    }
    
    return(tempImage);
  }
  
  /** Generate logo tile */
  void TileMapFetcher::GenerateLogoTile()
  {   
    // Create image size
    ImageType::SizeType size;
    size.Fill(256);
    
    // Create index start
    ImageType::IndexType start;
    start.Fill( 0 );
    
    // Create region size
    ImageType::RegionType region;
    region.SetIndex( start );
    region.SetSize(  size  );

    // Create image, set properties, and allocate
    m_LogoTile = ImageType::New();
    m_LogoTile->SetRegions(region);
    m_LogoTile->SetNumberOfComponentsPerPixel(3);
    m_LogoTile->Allocate();
    
    // Create pixel
    ImageType::PixelType white(3);
    
    // Fill with white
    white.Fill(255);
    
    // Fill image buffer with white pixel
    m_LogoTile->FillBuffer(white);
  }  
  
  /** Fetch Tile from Server and set it in cache - Check if tile must be update */
  /** Comment because cache is not persistant */
  /* bool TileMapFetcher::FetchTileFromServer(bool isInCache)
   {
   bool result = false;
   
   std::ostringstream pathAndFileName;
   
   pathAndFileName << m_CacheDirectory;
   
   if (m_CacheDirectory.at(m_CacheDirectory.size()-1) != '/')
   pathAndFileName << "/";
   
   pathAndFileName << m_FileName;
   
   FILE* output_file = fopen(pathAndFileName.str().c_str(),"w");
   if (output_file == NULL)
   {
   itkExceptionMacro(<<"FetchTileFromServer : bad file name.");
   return result;
   }
   
   std::ostringstream browserStream;
   browserStream   << "Mozilla/5.0 (Windows; U; Windows NT 6.0; en-GB; rv:1.8.1.11) Gecko/20071127 Firefox/2.0.0.11";
   
   CURL *curl;
   CURLcode res;
   curl = curl_easy_init();
   
   this->GenerateOSMurl();
   
   std::cout<<std::endl;
   std::cout<<"URL: "<<m_OsmUrl<<std::endl;
   std::cout<<"FILENAME: "<<pathAndFileName.str()<<std::endl;
   std::cout<<"IS IN CACHE: "<<isInCache<<std::endl;
   std::cout<<std::endl;
   
   if (m_OsmUrl == "")
   {
   itkExceptionMacro(<<"FetchTileFromServer : bad url.");
   return false;
   }
   
   char url[200];
   strcpy(url,m_OsmUrl.data());
   
   char browser[200];
   strcpy(browser,browserStream.str().data());
   
   //Download the file
   if (curl)
   {
   curl_easy_setopt(curl, CURLOPT_USERAGENT, browser);
   curl_easy_setopt(curl, CURLOPT_URL, url);
   curl_easy_setopt(curl, CURLOPT_WRITEDATA, output_file);
   
   // If tile is in cache, ask curl to check if tile is out to date
   if(isInCache)
   {
   // Search for time condition
   long time_condition;
   
   std::deque<otb::TileRecord>::iterator it;
   std::deque<otb::TileRecord>::iterator it2;
   
   for(it=m_CacheManagement.begin(); it!=m_CacheManagement.end(); it++)
   {
   if(it->GetFileName() == m_FileName)
   {
   it2 = it;
   break;
   }
   }
   
   time_condition = static_cast<long>(it2->GetDate());
   
   std::cout<<time_condition<<std::endl;
   
   curl_easy_setopt(curl, CURLOPT_TIMECONDITION, CURL_TIMECOND_IFMODSINCE);
   curl_easy_setopt(curl, CURLOPT_TIMEVALUE, time_condition);
   
   res = curl_easy_perform(curl);
   
   if (res == 0)
   {
   long response;
   curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response);
   // 304 = Not Modified
   if (response == 304)
   {         
   std::cout<<"TUILE NON MODIFIEE"<<std::endl;
   result = true;
   }
   // 200 = Ok
   else if (response == 200)
   {
   double size;
   // verify if curl sends us any data - this is a workaround on using CURLOPT_TIMECONDITION 
   // when the server has a (incorrect) time earlier than the time on the file we already have 
   curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &size);
   if (size == 0)
   {
   itkExceptionMacro(<<"FetchTileFromServer : transfert error - Size == 0.");
   return false;
   }
   else
   {
   // Update time in cache
   std::cout<<"TUILE MODIFIEE MISE A JOUR DU CACHE"<<std::endl;
   it2->SetDate();
   result = true;
   }
   }
   else
   {
   itkExceptionMacro(<<"FetchTileFromServer : transfert error.");
   return false;
   }
   }
   else
   {
   itkExceptionMacro(<<"FetchTileFromServer : transfert error.");
   return false;
   }
   }
   // Not in cache
   else
   {
   res = curl_easy_perform(curl);
   if (res != 0)
   {
   itkExceptionMacro(<<"FetchTileFromServer : transfert error.");
   return false;
   }
   else
   {
   std::cout<<"RECUPERATION DE LA TUILE"<<std::endl;
   result = true;
   }
   }
   
   fclose(output_file);
   // always cleanup 
   curl_easy_cleanup(curl);
   return result;
   }
   return false;
   } 
   */  
  
}




