#ifndef otbSLICScheduler_txx
#define otbSLICScheduler_txx

#include "otbSLICScheduler.h"

namespace otb{

  template <class TInputImage, class TOutputLabelImage>
  SLICScheduler<TInputImage, TOutputLabelImage>
  ::SLICScheduler() :
    m_SpatialWidth(100),
    m_SpatialDistanceWeight(1),
    m_MaxIterationNumber(10),
    m_Threshold(0.1),
    m_NbTilesX(1),
    m_NbTilesY(1),
    m_Margin(0)
  {}

  template <class TInputImage, class TOutputLabelImage>
  SLICScheduler<TInputImage, TOutputLabelImage>
  ::~SLICScheduler()
  {}
  
  template <class TInputImage, class TOutputLabelImage>
  void
  SLICScheduler<TInputImage, TOutputLabelImage>
  ::SetInputImage(typename TInputImage::Pointer InputImagePointer)
  {
    m_InputImage = InputImagePointer;
  }
  
  template <class TInputImage, class TOutputLabelImage>
  void
  SLICScheduler<TInputImage, TOutputLabelImage>
  ::Run()
  {
    typename otb::MPIConfig::Pointer mpiConfig = otb::MPIConfig::Instance();
    unsigned int myRank = mpiConfig->GetMyRank();
    unsigned int nbProcs = mpiConfig->GetNbProcs();
    
    const unsigned int imageSizeX = m_InputImage->GetLargestPossibleRegion().GetSize()[0];
    const unsigned int imageSizeY = m_InputImage->GetLargestPossibleRegion().GetSize()[1];
    
    const unsigned int tileWidth = imageSizeX/m_NbTilesX;
    const unsigned int tileHeight = imageSizeY/m_NbTilesY;
    const unsigned int pixelMargin = m_Margin*m_SpatialWidth;
  
    typedef std::vector<otb::ProcessingTile> TileListType;

    //Tiles for processing with no margins
    TileListType tilesNoMargin = SplitOTBImage(m_InputImage, tileWidth, tileHeight, 0, m_NbTilesX, m_NbTilesY, m_Prefix);

    //Tiles for processing with margins
    TileListType tilesMargin = SplitOTBImage(m_InputImage, tileWidth, tileHeight, pixelMargin, m_NbTilesX, m_NbTilesY, m_Prefix);

    TileListType whiteTiles;
    TileListType blackTiles;
    TileListType tilesToWrite;

    //Generate list of black and white tiles to process
    //Margin mode
    if (m_Margin > 0) {    
      for(unsigned int y = 0 ; y < m_NbTilesY; ++y){
	for(unsigned int x = 0 ; x < m_NbTilesX; ++x){
	  if(!((x+y)%2)){ //if x + y even
	    blackTiles.push_back(tilesNoMargin[x+m_NbTilesX*y]);
	  }
	  else{
	    whiteTiles.push_back(tilesMargin[x+m_NbTilesX*y]);
	  }
	}
      }  
      if(myRank < blackTiles.size())
	{
	  //Dispatch according to rank of MPI process
	  unsigned int splitIdx = myRank;
	  while(splitIdx < blackTiles.size())
	    {
	      tilesToWrite.push_back(blackTiles[splitIdx]);
	      splitIdx+=nbProcs;
	    }
	}
    }
    //Simple streaming mode
    else{
      if(myRank < tilesNoMargin.size())
	{
	  unsigned int splitIdx = myRank;
	  while(splitIdx < tilesNoMargin.size())
	    {
	      tilesToWrite.push_back(tilesNoMargin[splitIdx]);
	      splitIdx+=nbProcs;
	    }
	}
    }
    //First process the black tiles (no margins)
    for (otb::ProcessingTile const& t : tilesToWrite)
      {	
	typename ExtractFilterType::Pointer extractFilter = ExtractFilterType::New();
	extractFilter->SetReleaseDataFlag(true);
	extractFilter->SetInput(m_InputImage);
	extractFilter->SetExtractionRegion(t.region);

	typename SLICFilter::Pointer filter = SLICFilter::New();
	filter->SetReleaseDataFlag(true);
	filter->SetSpatialWidth(m_SpatialWidth);
	filter->SetSpatialDistanceWeight(m_SpatialDistanceWeight);
	filter->SetMaxIterationNumber(m_MaxIterationNumber);
	filter->SetThreshold(m_Threshold);
	filter->SetNbSPx(vcl_ceil((float) m_InputImage->GetLargestPossibleRegion().GetSize()[0]/m_SpatialWidth));
	filter->SetTile(t);
	filter->SetMargin(m_Margin);
    	filter->SetInput(extractFilter->GetOutput());
	
	typename WriterType::Pointer writer = WriterType::New();
    	std::stringstream name;
    	name << m_Prefix << "_" << t.region.GetIndex()[0] << "_" << t.region.GetIndex()[1] << "_" << t.region.GetSize()[0] << "_" << t.region.GetSize()[1] << ".tif";
    	writer->SetInput(filter->GetOutput());
    	writer->SetFileName(name.str());
	writer->SetNumberOfDivisionsStrippedStreaming(1);	
    	writer->Update();	
    	std::cout << myRank << " writing b region :" << name.str() << std::endl;
      }
    //Wait for all black regions to be processed
    mpiConfig->barrier();

    //Dispatch white tiles to different processes 
    if(m_Margin > 0){
      tilesToWrite.clear();
      for(unsigned int line = 0 ; line < m_NbTilesY ; line++){
	unsigned int nbWhite = vcl_floor(m_NbTilesX/2);
	if (m_NbTilesX%2){ 
	  nbWhite += line%2;
	}
	if(myRank < nbWhite)
	  {
	    unsigned int splitIdx = myRank;
	    while(splitIdx < nbWhite)
	      {
		unsigned int ind;
		if(m_NbTilesX%2)
		  ind = m_NbTilesX*vcl_floor(line/2) + splitIdx + vcl_floor(m_NbTilesX/2)*(line%2);
		else
		  ind = splitIdx + nbWhite*line;
		tilesToWrite.push_back(whiteTiles[ind]);
		splitIdx+=nbProcs;
	      }
	  }

	//Process tiles
	for (otb::ProcessingTile t : tilesToWrite)
	  {
	    typename SLICFilter::Pointer filter = SLICFilter::New();
	    filter->SetSpatialWidth(m_SpatialWidth);
	    filter->SetSpatialDistanceWeight(m_SpatialDistanceWeight);
	    filter->SetMaxIterationNumber(m_MaxIterationNumber);
	    filter->SetThreshold(m_Threshold);
	    filter->SetNbSPx(vcl_ceil((float) m_InputImage->GetLargestPossibleRegion().GetSize()[0]/m_SpatialWidth));
	    filter->SetMargin(m_Margin);
	    typename ExtractFilterType::Pointer extractFilter = ExtractFilterType::New();
	    extractFilter->SetReleaseDataFlag(true);
	    extractFilter->SetInput(m_InputImage);//m_InputImage.GetLargestPossibleRegion() ?
	    //Extract main region
	    extractFilter->SetExtractionRegion(t.region);
	    filter->SetTile(t);
	    filter->ClearMargins();
	    filter->SetInput(extractFilter->GetOutput());
	    filter->SetReleaseDataFlag(true);
	    //Read neighboring black tiles margin areas and load into filter
	    for (unsigned int i = 0; i < 8 ; i++){
	      if(pixelMargin > 0 && t.tileNeighbors[i]>=0){
		typename TOutputLabelImage::RegionType marginRegion;
		typename TOutputLabelImage::RegionType r;
		bool read = false;
		switch (i){
		case otb::NBH_TOP:{
		  //Normal case
		  r =  tilesNoMargin[t.tileNeighbors[i]].region;
		  // typename TOutputLabelImage::RegionType::IndexType index = r.GetIndex();
		  typename TOutputLabelImage::RegionType::IndexType index({0,0});
		  typename TOutputLabelImage::RegionType::SizeType size({r.GetSize()[0],pixelMargin});	
		  index[1]+=r.GetSize()[1]-pixelMargin;
		  //Accomodate for corners
		  if(t.tileNeighbors[otb::NBH_LEFT]>=0){
		    //if left neighbor
		    index[0]+=pixelMargin;
		    size[0]-=pixelMargin;

		  }
		  if(t.tileNeighbors[otb::NBH_RIGHT]>=0){
		    //if right neighbor
		    size[0]-=pixelMargin;
		  }
		  marginRegion.SetIndex(index);
		  marginRegion.SetSize(size);
		  read = true;
		  break;
		}
		case otb::NBH_BOTTOM:{
		  r =  tilesNoMargin[t.tileNeighbors[i]].region;
		  // typename TOutputLabelImage::RegionType::IndexType index = r.GetIndex();
		  typename TOutputLabelImage::RegionType::IndexType index({0,0});
		  const typename TOutputLabelImage::RegionType::SizeType size({r.GetSize()[0],pixelMargin});	
		  marginRegion.SetIndex(index);
		  marginRegion.SetSize(size);
		  read = true;
		  break;
		}
		case otb::NBH_LEFT:{
		  r =  tilesNoMargin[t.tileNeighbors[i]].region;
		  // typename TOutputLabelImage::RegionType::IndexType index = r.GetIndex();
		  typename TOutputLabelImage::RegionType::IndexType index({0,0});
		  index[0]+=r.GetSize()[0]-pixelMargin;
		  typename TOutputLabelImage::RegionType::SizeType size({pixelMargin,r.GetSize()[1]});
		  if(t.tileNeighbors[otb::NBH_TOP]>=0){
		    //if right neighbor
		    index[1]+=pixelMargin;
		    size[1]-=pixelMargin;
		  }
		  marginRegion.SetIndex(index);
		  marginRegion.SetSize(size);
		  read = true;
		  break;
		}
		case otb::NBH_RIGHT:{
		  r =  tilesNoMargin[t.tileNeighbors[i]].region;
		  // typename TOutputLabelImage::RegionType::IndexType index = r.GetIndex();
		  typename TOutputLabelImage::RegionType::IndexType index({0,0});
		  typename TOutputLabelImage::RegionType::SizeType size({pixelMargin,r.GetSize()[1]});
		  if(t.tileNeighbors[otb::NBH_TOP]>=0){
		    //if right neighbor
		    index[1]+=pixelMargin;
		    size[1]-=pixelMargin;
		  }
		  marginRegion.SetIndex(index);
		  marginRegion.SetSize(size);
		  read = true;
		  break;
		}
		case otb::NBH_TOP_LEFT: {
		  r = tilesMargin[t.tileNeighbors[i]].region;
		  // typename TOutputLabelImage::RegionType::IndexType index = r.GetIndex();
		  typename TOutputLabelImage::RegionType::IndexType index({0,0});
		  index[0]+=r.GetSize()[0] - 2*pixelMargin;
		  index[1]+=r.GetSize()[1] - 2*pixelMargin;
		  const typename TOutputLabelImage::RegionType::SizeType size({2*pixelMargin,2*pixelMargin});	
		  marginRegion.SetIndex(index);
		  marginRegion.SetSize(size);
		  read = true;
		  break;
		}
		case otb::NBH_TOP_RIGHT: {
		  r = tilesMargin[t.tileNeighbors[i]].region;
		  // typename TOutputLabelImage::RegionType::IndexType index = r.GetIndex();
		  typename TOutputLabelImage::RegionType::IndexType index({0,0});
		  index[1]+=r.GetSize()[1] - 2*pixelMargin;
		  const typename TOutputLabelImage::RegionType::SizeType size({2*pixelMargin,2*pixelMargin});	
		  marginRegion.SetIndex(index);
		  marginRegion.SetSize(size);
		  read = true;
		  break;
		}
		}
		if(read){
		  typename LabelExtractFilterType::Pointer marginExtract = LabelExtractFilterType::New();
		  std::stringstream readNeighborName;
		  readNeighborName << m_Prefix <<"_"<< r.GetIndex()[0] << "_" << r.GetIndex()[1] << "_"<< r.GetSize()[0] << "_"<< r.GetSize()[1] << ".tif";
		  typename LabelReaderType::Pointer labelReader = LabelReaderType::New();
		  labelReader->SetReleaseDataFlag(true);
		  labelReader->SetFileName(readNeighborName.str());
		  //Set the index of the output (reader gives 0 0)
		  marginExtract->SetInput(labelReader->GetOutput());
		  marginExtract->SetExtractionRegion(marginRegion);	    
		  marginExtract->Update();
		  marginRegion.SetIndex({marginRegion.GetIndex()[0]+r.GetIndex()[0],marginRegion.GetIndex()[1]+r.GetIndex()[1]});
		  marginExtract->GetOutput()->SetRegions(marginRegion);
		  filter->AddInputMargin(marginExtract->GetOutput());
		}
	      }
	    }
	    filter->Update();
	    typename WriterType::Pointer writer = WriterType::New();
	    writer->SetInput(filter->GetOutput());
	    std::stringstream name;
	    name << m_Prefix << "_" << t.region.GetIndex()[0] << "_" << t.region.GetIndex()[1] << "_" << t.region.GetSize()[0] << "_" << t.region.GetSize()[1] << ".tif";
	    writer->SetFileName(name.str());
	    writer->SetNumberOfDivisionsStrippedStreaming(1);
	    writer->Update();
	    std::cout << myRank << " writing w region :" << name.str() << std::endl;
	  }
	tilesToWrite.clear();
	mpiConfig->barrier();
      }
    }
    try
      {
	if (myRank == 0)
	  {
	    // VRT Filename
	    std::stringstream vrtfOut;
	    vrtfOut<< m_Prefix << ".vrt";

	    // Data type
	    GDALDataType dataType;
	    dataType = GDALGetDataTypeByName("UInt32");

	    // Get VRT driver
	    GDALAllRegister();
	    GDALDriver *driver = GetGDALDriverManager()->GetDriverByName("VRT");
	    if (driver == NULL) {
	      mpiConfig->logError("Error opening VRT driver.");
	      mpiConfig->abort(EXIT_FAILURE);
	    }

	    // Create output raster
	    GDALDataset *VRTOutput = driver->Create(vrtfOut.str().c_str(),
						    imageSizeX,
						    imageSizeY,
						    0,
						    dataType,
						    NULL); // No options
	    if (VRTOutput == NULL) {
	      mpiConfig->logError("driver->Create call failed.\n");
	      mpiConfig->abort(EXIT_FAILURE);
	    }

	    // Set GeoTransform
	    double gt[6];
	    gt[0] = m_InputImage->GetOrigin()[0] - 0.5*m_InputImage->GetSignedSpacing()[0];
	    gt[1] = m_InputImage->GetSignedSpacing()[0];
	    gt[2] = 0.0;
	    gt[3] = m_InputImage->GetOrigin()[1] - 0.5*m_InputImage->GetSignedSpacing()[1];
	    gt[4] = 0.0;
	    gt[5] = m_InputImage->GetSignedSpacing()[1];
	    VRTOutput->SetGeoTransform(gt);

	    // Set projection
	    OGRSpatialReference out_sr;
	    char *wkt = NULL;
	    out_sr.SetFromUserInput(m_InputImage->GetProjectionRef().c_str());
	    out_sr.exportToWkt(&wkt);
	    VRTOutput->SetProjection(wkt);
	    VRTOutput->AddBand(dataType, NULL);
	    if(m_Margin == 0){
	      writeVRTRegions(tilesNoMargin, VRTOutput, m_Prefix, 1);
	    }
	    else{
	      writeVRTRegions(blackTiles, VRTOutput, m_Prefix, 1);
	      writeVRTRegions(whiteTiles, VRTOutput, m_Prefix, 1);
	    }
	    // Close
	    OGRFree(wkt);
	    GDALClose(VRTOutput);
	  }
      }
    catch (itk::ExceptionObject& err)
      {
	std::stringstream message;
	message << "ExceptionObject caught: " << err << std::endl;
	mpiConfig->logError(message.str());
	mpiConfig->abort(EXIT_FAILURE);
      }
  }
  template<class TInputImage, class TOutputLabelImage>
  void
  SLICScheduler<TInputImage,TOutputLabelImage>
  ::writeVRTRegions(TileListType const& inputRegions, GDALDataset *VRTOutput, std::string prefix, unsigned int band){
    for(otb::ProcessingTile t : inputRegions)
      {
	int tileSizeX = t.region.GetSize()[0];
	int tileSizeY = t.region.GetSize()[1];
	int tileIndexX = t.region.GetIndex()[0];
	int tileIndexY = t.region.GetIndex()[1];
	std::stringstream tileFileName;
	tileFileName <<prefix<<"_"<<tileIndexX<<"_"<<tileIndexY<<"_"<<tileSizeX<<"_"<<tileSizeY<<".tif";

	GDALDataset *dataset = (GDALDataset*) GDALOpen(tileFileName.str().c_str(), GA_ReadOnly);

	VRTSourcedRasterBand *VRTBand = dynamic_cast<VRTSourcedRasterBand*> (VRTOutput->GetRasterBand(band));
	VRTBand->AddComplexSource(dataset->GetRasterBand(band),
				  0,//Xoffset
				  0,//Yoffset
				  tileSizeX,//Dimensions
				  tileSizeY,//Dimensions
				  tileIndexX,//Index
				  tileIndexY,//Index
				  tileSizeX,//Dimensions
				  tileSizeY,//Dimensions
				  0.0,//Scale offset
				  1,//Scale Ratio
				  0);//NodataValue
      }
  }
  template<class TInputImage, class TOutputLabelImage>
  std::vector<ProcessingTile>
  SLICScheduler<TInputImage,TOutputLabelImage>
  ::SplitOTBImage(const TInputImage * imagePtr, // input image
		  const unsigned int tileWidth, // width of the tile
		  const unsigned int tileHeight, // height of the tile
		  const unsigned int margin, // stability margin
		  const unsigned int nbTilesX,
		  const unsigned int nbTilesY,
		  const std::string temporaryFilesPrefix)
  {
    std::vector<ProcessingTile> tiles;
    // Image size
    const unsigned int imageWidth = imagePtr->GetLargestPossibleRegion().GetSize()[0];
    const unsigned int imageHeight = imagePtr->GetLargestPossibleRegion().GetSize()[1];

    /* Loop over the tiles*/
    tiles.assign(nbTilesX * nbTilesY, ProcessingTile());
    unsigned int i = 0;
    for(unsigned int row = 0; row < nbTilesY; ++row)
      {
	for(unsigned int col = 0; col < nbTilesX; ++col)
	  {
	    /* Local variables for the next loop */
	    // Compute current tile start and size
	    const unsigned int startX = col * tileWidth;
	    const unsigned int startY = row * tileHeight; // Upper left coordinates of the tile.
	    unsigned int sizeX = tileWidth;
	    unsigned int sizeY = tileHeight; // Size of the tiles.

	    // Current tile size might be different for right and bottom borders
	    if (col == nbTilesX -1)
	      {
		sizeX += imageWidth % tileWidth;
	      }
	    if (row == nbTilesY -1)
	      {
		sizeY += imageHeight % tileHeight;
	      }

	    /* Margin at the top ? */
	    if( row > 0 )
	      {
		tiles[i].margin[POS_TOP] = margin;
		tiles[i].rows[0] = row * tileHeight;
		tiles[i].tileNeighbors[NBH_TOP] = i - nbTilesX;
	      }
	    else
	      {
		// Tile is on the top row --> no top margin
		tiles[i].margin[POS_TOP] = 0;
		tiles[i].rows[0] = 0;
		tiles[i].tileNeighbors[NBH_TOP] = -1;
	      }

	    /* Margin at the right */
	    if( col < nbTilesX - 1 )
	      {
		tiles[i].margin[POS_RIGHT] = margin;
		tiles[i].columns[1] = col * tileWidth + sizeX - 1; //sizeX
		tiles[i].tileNeighbors[NBH_RIGHT] = i+1;
	      }
	    else
	      {
		// Tile is on the right column --> no right margin
		tiles[i].margin[POS_RIGHT] = 0;
		tiles[i].columns[1] = imageWidth - 1;
		tiles[i].tileNeighbors[NBH_RIGHT] = -1;
	      }

	    /* Margin at the bottom */
	    if( row < nbTilesY - 1)
	      {
		tiles[i].margin[POS_BOTTOM] = margin;
		tiles[i].rows[1] = row * tileHeight + sizeY - 1; // sizeY
		tiles[i].tileNeighbors[NBH_BOTTOM] = i + nbTilesX;
	      }
	    else
	      {
		// Tile is on the bottom --> no bottom margin
		tiles[i].margin[POS_BOTTOM] = 0;
		tiles[i].rows[1] = imageHeight - 1;
		tiles[i].tileNeighbors[NBH_BOTTOM] = -1;
	      }

	    /* Margin at the left */
	    if( col > 0 )
	      {
		tiles[i].margin[POS_LEFT] = margin;
		tiles[i].columns[0] = col * tileWidth;
		tiles[i].tileNeighbors[NBH_LEFT] = i-1;
	      }
	    else
	      {
		// Tile is on the left --> no left margin
		tiles[i].margin[POS_LEFT] = 0;
		tiles[i].columns[0] = 0;
		tiles[i].tileNeighbors[NBH_LEFT] = -1;
	      }

	    /* Store the tile region */
	    typename TInputImage::IndexType index;
	    index[0] = startX - tiles[i].margin[POS_LEFT];
	    index[1] = startY - tiles[i].margin[POS_TOP];
	    typename TInputImage::SizeType size;
	    size[0] = sizeX + tiles[i].margin[POS_LEFT] + tiles[i].margin[POS_RIGHT];
	    size[1] = sizeY + tiles[i].margin[POS_TOP] + tiles[i].margin[POS_BOTTOM];
	    typename TInputImage::RegionType region(index, size);
	    tiles[i].region = region;

	    // std::cout << "Tile " << i << ": start at " << index << " with size " << size << std::endl;

	    /* Is there a neighbor at the rop right */
	    if(row > 0 && col < nbTilesX - 1)
	      tiles[i].tileNeighbors[NBH_TOP_RIGHT] = i - nbTilesX + 1;
	    else
	      tiles[i].tileNeighbors[NBH_TOP_RIGHT] = -1;

	    /* Is there a neighbor at the bottom right */
	    if(col < nbTilesX - 1 && row < nbTilesY - 1)
	      tiles[i].tileNeighbors[NBH_BOTTOM_RIGHT] = i + nbTilesX + 1;
	    else
	      tiles[i].tileNeighbors[NBH_BOTTOM_RIGHT] = -1;

	    /* Is there a neighbor at the bottom left */
	    if(row < nbTilesY - 1 && col > 0)
	      tiles[i].tileNeighbors[NBH_BOTTOM_LEFT] = i + nbTilesX - 1;
	    else
	      tiles[i].tileNeighbors[NBH_BOTTOM_LEFT] = -1;

	    /* Is there a neighbor at the top left */
	    if(col > 0 && row > 0)
	      tiles[i].tileNeighbors[NBH_TOP_LEFT] = i - nbTilesX - 1;
	    else
	      tiles[i].tileNeighbors[NBH_TOP_LEFT] = -1;

	    const std::string suffix = std::to_string(row) + "_" + std::to_string(col) + ".tif";
	    tiles[i].nodeFileName = temporaryFilesPrefix + "_node_" + suffix;
	    tiles[i].edgeFileName = temporaryFilesPrefix + "_edge_" + suffix;
	    tiles[i].nodeMarginFileName = temporaryFilesPrefix + "_nodeMargin_" + suffix;
	    tiles[i].edgeMarginFileName = temporaryFilesPrefix + "_edgeMargin_" + suffix;

	    i++;
	  } // end for(unsigned int col = 0; col < nbTilesX; ++col)

      } // for(unsigned int row = 0; row < nbTilesY; ++row)

    return tiles;
  }
} //end namespace otb
#endif

