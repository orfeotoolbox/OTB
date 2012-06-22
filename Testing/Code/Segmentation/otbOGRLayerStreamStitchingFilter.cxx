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

#include "otbOGRLayerStreamStitchingFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "itksys/SystemTools.hxx"
#include "otbOGRDataSourceWrapper.h"

int otbOGRLayerStreamStitchingFilter(int argc, char * argv[])
{
  if (argc != 5)
    {
      std::cerr << "Usage: " << argv[0];
      std::cerr << " inputImage inputOGR outputOGR streamingSize" << std::endl;
      return EXIT_FAILURE;
    }
  
  const char * infname      = argv[1];
  const char * inOGRfname   = argv[2];
  const char * tmpOGRfname  = argv[3];
  unsigned int size         = atoi(argv[4]);
  
  /** Typedefs */
  const unsigned int Dimension = 2;
  typedef float PixelType;
  typedef otb::Image<PixelType, Dimension> ImageType;

  typedef otb::OGRLayerStreamStitchingFilter<ImageType>   FilterType;
  typedef otb::ImageFileReader<ImageType>       ReaderType;
  
  ReaderType::Pointer reader = ReaderType::New();
  FilterType::Pointer filter = FilterType::New();
  
  //first copy the input OGR file as it will be updated with the fusionned polygons
  itksys::SystemTools::CopyAFile(inOGRfname,tmpOGRfname,true);
  
  //Get the base name of the .shp file
  std::string inPathName = itksys::SystemTools::GetFilenamePath(inOGRfname);
  std::string inBaseName = itksys::SystemTools::GetFilenameWithoutExtension(inOGRfname);
  std::string outPathName = itksys::SystemTools::GetFilenamePath(tmpOGRfname);
  std::string outBaseName = itksys::SystemTools::GetFilenameWithoutExtension(tmpOGRfname);


  //copy the .shx, .dbf, .prj files
  std::string in = inPathName+"/"+inBaseName+".shx";
  std::string out = outPathName+"/"+outBaseName+".shx";
  itksys::SystemTools::CopyAFile(in.c_str(),out.c_str(),true);
  in = inPathName+"/"+inBaseName+".dbf";
  out = outPathName+"/"+outBaseName+".dbf";
  itksys::SystemTools::CopyAFile(in.c_str(),out.c_str(),true);
  in = inPathName+"/"+inBaseName+".prj";
  out = outPathName+"/"+outBaseName+".prj";
  itksys::SystemTools::CopyAFile(in.c_str(),out.c_str(),true);

  const std::string layerName = outBaseName;
  
  reader->SetFileName(infname);
  reader->UpdateOutputInformation();
  
  ImageType::SizeType streamSize;
  streamSize.Fill(size);
  
  otb::ogr::DataSource::Pointer ogrDS = otb::ogr::DataSource::New(tmpOGRfname, otb::ogr::DataSource::Modes::Update_LayerUpdate);
  
  filter->SetInput(reader->GetOutput());
  filter->SetOGRLayer(ogrDS->GetLayer(layerName));
  filter->SetStreamSize(streamSize);
  filter->GenerateData();

  return EXIT_SUCCESS;
}
