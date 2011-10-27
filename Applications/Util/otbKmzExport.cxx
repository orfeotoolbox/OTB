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
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbKmzProductWriter.h"


namespace otb
{
namespace Wrapper
{

namespace Functor
{
template< class TScalar >
class ITK_EXPORT LogFunctor
{
public:
  LogFunctor(){};
  ~LogFunctor(){};
  TScalar operator() (const TScalar& v) const
  {
    return vcl_log(v);
  }
};
} // end namespace Functor


class KmzExport : public Application
{
public:
  /** Standard class typedefs. */
  typedef KmzExport                       Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(KmzExport, otb::Application);

private:
   KmzExport()
  {
    SetName("KmzExport");
    SetDescription("Export the input image in a KMZ product.");
    // Documentation
    SetDocName("Image to KMZ Export Application");
    SetDocLongDescription("This application exports the input image in a kmz product that can be display in the Google Earth software. The user can set the size of the product size, a logo and a legend to the product. Furthemore, to obtain a product that fits the relief, a DEM can be used.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("Convertion");
    SetDocCLExample("otbApplicationLauncherCommandLine KmzExport ${OTB-BIN}/bin --in ${OTB-Data}/Input/qb_RoadExtract.img --out otbKmzExport --logo ${OTB-Data}/Input/cnes.png ${OTB-Data}/Input/otb_big.png");
    AddDocTag("KMZ");
    AddDocTag("Export");
  }

  virtual ~KmzExport()
  {
  }
  
  void DoCreateParameters()
  {
    AddParameter(ParameterType_InputImage,  "in",   "Input image");
    SetParameterDescription("in", "Input image");
    
    AddParameter(ParameterType_Filename, "out",  "Output .kmz product");
    SetParameterDescription("out", "Output Kmz product directory (with .kmz extension)");
    
    AddParameter(ParameterType_Int, "tilesize",  "Tile Size");
    SetParameterDescription("tilesize", "Size of the tiles in the kmz product, in number of pixels.");   
    MandatoryOff("tilesize");
    
    AddParameter(ParameterType_InputImage, "logo",  "Image logo");
    SetParameterDescription("logo", "Path to the image logo to add to the KMZ product."); 
    MandatoryOff("logo");

    AddParameter(ParameterType_InputImage, "legend",  "Image legend");
    SetParameterDescription("legend", "Path to the image legend to add to the KMZ product."); 
    MandatoryOff("legend");

    AddParameter(ParameterType_Directory, "dem",  "DEM directory");
    SetParameterDescription("dem", "Path to the directory that contains elevation information."); 
    MandatoryOff("dem");
  }


  void DoUpdateParameters()
  {
    // Nothing to do here for the parameters : all are independent
  }

 void DoExecute()
  {
    typedef otb::KmzProductWriter<FloatVectorImageType> KmzProductWriterType;
    
    // Second part : Image To Kmz
    KmzProductWriterType::Pointer kmzWriter  = KmzProductWriterType::New();
    kmzWriter->SetInput( this->GetParameterImage("in") );
    kmzWriter->SetPath( this->GetParameterString("out") );

    // Use DEM if any
    if( this->HasValue("dem") )
      {
      kmzWriter->SetDEMDirectory(this->GetParameterString("dem"));
      }


    // If the tile size is set
    if( this->HasValue("tilesize") )
      {
      kmzWriter->SetTileSize( this->GetParameterInt("tilesize") );
      }
    
    // Add the logo if any
    if( this->HasValue("logo") )
      {
      kmzWriter->SetLogo( this->GetParameterImage("logo") );
      }

    // Add the legend if any
    if( this->HasValue("legend") )
      {
      kmzWriter->AddLegend( this->GetParameterImage("legend") );
      }
  
    // trigger the writing
    kmzWriter->Update();
  }
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::KmzExport)



/*
namespace otb
{

int KmzExport::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("KmzExport");
  descriptor->SetDescription("Export the input image as Kmz");
  descriptor->AddInputImage();
  descriptor->AddOption("OutputProductName", "Output Kmz product", "kmz", 1, true,
                        ApplicationDescriptor::InputImage);
  descriptor->AddOption("TileSize", "Set the size of the tiles", "s", 1, false,
                        ApplicationDescriptor::Integer);
  descriptor->AddOption("LogoImage", "Add a logo to the final Kmz", "lo", 1, false,
                        ApplicationDescriptor::FileName);
  descriptor->AddOption("LegendImage", "Add a legend to the image exported", "le", 1,
                        false, ApplicationDescriptor::FileName);
 
  return EXIT_SUCCESS;
}

int KmzExport::Execute(otb::ApplicationOptionsResult* parseResult)
{
  typedef otb::VectorImage<float, 2>                      ImageType;
  typedef otb::KmzProductWriter<ImageType>                KmzProductWriterType;
  typedef otb::ImageFileReader<ImageType>                 ReaderType;

  // Instanciate reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(parseResult->GetInputImage());
  reader->UpdateOutputInformation();
  
  // Second part : Image To Kmz
  KmzProductWriterType::Pointer    kmzWriter  = KmzProductWriterType::New();
  kmzWriter->SetInput(reader->GetOutput());
  kmzWriter->SetPath(parseResult->GetParameterString("OutputProductName"));

  // If the tilesize is set
  if(parseResult->IsOptionPresent("TileSize"))
    {
    kmzWriter->SetTileSize(parseResult->GetParameterUInt("TileSize"));
    }
  // Read the logo if any
  if(parseResult->IsOptionPresent("LogoImage"))
    {
    ReaderType::Pointer logoReader  = ReaderType::New();
    logoReader->SetFileName(parseResult->GetParameterString("LogoImage"));
    logoReader->Update();
    kmzWriter->SetLogo(logoReader->GetOutput());
    }
  // Read the legend if any
  if(parseResult->IsOptionPresent("LegendImage"))
    {
    ReaderType::Pointer legendReader  = ReaderType::New();
    legendReader->SetFileName(parseResult->GetParameterString("LegendImage"));
    legendReader->Update();
    kmzWriter->AddLegend("Input Legend", legendReader->GetOutput());
    kmzWriter->AddLegend(legendReader->GetOutput());
    }
  
  // trigger the writing
  kmzWriter->Update();

  return EXIT_SUCCESS;
}
}
*/
