/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otb_GlImageActor_h
#define otb_GlImageActor_h


#include <vcl_algorithm.h>

#include "itkCenteredRigid2DTransform.h"

#include "otbFragmentShader.h"
#include "otbGenericRSTransform.h"
#include "otbGeoInterface.h"
#include "otbGlActor.h"
#include "otbImageFileReader.h"
#include "otbImageSettings.h"
#include "otbMultiChannelExtractROI.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "otbVectorImage.h"


namespace otb
{

class OTBIce_EXPORT GlImageActor 
  : public GlActor, public GeoInterface
{
public:
  typedef GlImageActor                                    Self;
  typedef GlActor                                         Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  itkNewMacro(Self);

  typedef VectorImage<float>                              VectorImageType;
  typedef VectorImage<unsigned char>                      UCharVectorImageType;
  typedef VectorImageType::PixelType                      PixelType;
  typedef VectorImageType::ImageKeywordlistType           ImageKeywordlistType;
  typedef itk::MetaDataDictionary                        MetaDataDictionaryType;
  typedef VectorImageType::SizeType                       SizeType;
  typedef VectorImageType::IndexType                      IndexType;
  typedef VectorImageType::RegionType                     RegionType;
  typedef VectorImageType::SpacingType                    SpacingType;
  typedef VectorImageType::PointType                      PointType;
  typedef VectorRescaleIntensityImageFilter<VectorImageType,UCharVectorImageType> RescaleFilterType;
  
  struct ResolutionAlgorithm
  {
    enum type
    {
      Invalid,
      Nearest, ///<Use the nearest resolution (lower or upper)
      Nearest_Lower, ///<Use the nearest lower resolution (better for
                     ///performances, poorer quality
      Nearest_Upper,///<Use the nearest upper resolution (lower
                    ///performances, better quality)
      MAX__};
    };
  
  // Initialize with a new image
  void Initialize(const std::string & filename);

  // Retrieve the full extent of the actor
  void GetExtent(double & ulx, double & uly, double & lrx, double & lry) const override;

  // Update internal actor state with respect to ViewSettings
  void ProcessViewSettings() override;

  // Heavy load/unload operations of data
  void UpdateData() override;

  // Gl rendering of current state
  void Render() override;

  // Automatic color adjustment
  void AutoColorAdjustment( double & minRed, double & maxRed,
			    double & minGreen, double & maxGreen,
			    double & minBlue, double & maxBlue,
			    bool full = true,
			    unsigned int refSize = 500,
			    double lcp = 0.02, double hcp = 0.02 );

  const PointType & GetOrigin() const;

  const GeoInterface::Spacing2 & GetSpacing() const override;

  std::string GetWkt() const override;

  ImageKeywordlistType GetKwl() const;

  bool HasKwl() const override;

  bool GetKwl( ImageKeywordlist & ) const override;

  MetaDataDictionaryType & GetMetaDataDictionary() const;

  itkGetMacro(RedIdx,unsigned int);
  itkGetMacro(GreenIdx,unsigned int);
  itkGetMacro(BlueIdx,unsigned int);

  itkGetMacro(NumberOfComponents,unsigned int);
  itkGetMacro(CurrentResolution,unsigned int);
  itkGetMacro(LargestRegion,RegionType);

  itkSetMacro(TileSize,unsigned int);
  itkGetMacro(TileSize,unsigned int);

  itkBooleanMacro(SoftwareRendering );
  itkSetMacro(SoftwareRendering, bool );
  itkGetMacro(SoftwareRendering, bool );

  void CreateShader();

  void SetResolutionAlgorithm(ResolutionAlgorithm::type alg)
  {
    m_ResolutionAlgorithm = alg;
  }

  ResolutionAlgorithm::type GetResolutionAlgorithm() const
  {
    return m_ResolutionAlgorithm;
  }

  virtual void SetRedIdx(const unsigned int idx)
  {
  if ( this->m_RedIdx != idx )
   { 
   this->m_RedIdx = std::min(this->GetNumberOfComponents(),idx);
   this->Modified();
   }
  }

  virtual void SetGreenIdx(const unsigned int idx)
  {
  if ( this->m_GreenIdx != idx )
    { 
    this->m_GreenIdx = std::min(this->GetNumberOfComponents(),idx);
    this->Modified();
    }
  }  

  virtual void SetBlueIdx(const unsigned int idx)
  {
  if ( this->m_BlueIdx != idx )
    { 
    this->m_BlueIdx = std::min(this->GetNumberOfComponents(),idx);
    this->Modified(); 
    }
  }

  PointType ViewportToImageTransform(const PointType & in, bool physical = true) const;

  PointType ImageToViewportTransform(const PointType & in, bool physical = true) const;
  
  bool GetPixelFromViewport( const PointType & in, PixelType & pixel ) const;

  bool GetPixelFromViewport( const PointType & view,
			     PixelType & pixel,
			     PointType & physical,
			     IndexType & index ) const;

  bool GetPixel( const PointType & physical, PixelType & pixel, IndexType & index ) const;

  itkGetObjectMacro(Shader,FragmentShader);
  itkSetObjectMacro(Shader,FragmentShader);

  itkGetObjectMacro( ImageSettings, ImageSettings );

  //
  // otb::GlActor overloads.
  //

  bool TransformFromViewport( Point2d & out,
                                      const Point2d & in,
                                      bool isPhysical = true ) const override;

  bool TransformToViewport( Point2d & out,
                                    const Point2d & in,
                                    bool isPhysical = true ) const override;


  void UpdateTransforms();

protected:
  GlImageActor();
  
  ~GlImageActor() override;

  typedef ImageFileReader<VectorImageType>                                        ReaderType;
  typedef MultiChannelExtractROI<float,float>                                     ExtractROIFilterType;
  typedef otb::GenericRSTransform<>                                               RSTransformType;
  typedef itk::CenteredRigid2DTransform<>                                         RigidTransformType;
  typedef std::vector<unsigned int>                                               ResolutionVectorType;

  // Internal class to hold tiles
  class Tile
  {
  public:
    Tile()
      : m_Loaded(false),
        m_TextureId(0),
        m_ImageRegion(),
        m_TileSize(0),
        m_Image(),
        m_UL(),
        m_UR(),
        m_LL(),
        m_LR(),
        m_Resolution(1),
        m_RedIdx(1),
        m_GreenIdx(2),
        m_BlueIdx(3),
        m_RescaleFilter(nullptr)
    {
      m_UL.Fill(0);
      m_UR.Fill(0);
      m_LL.Fill(0);
      m_LR.Fill(0);
    }

    bool m_Loaded;
    unsigned int m_TextureId;
    RegionType m_ImageRegion;
    unsigned int m_TileSize;
    VectorImageType::Pointer m_Image;
    PointType m_UL;
    PointType m_UR;
    PointType m_LL;
    PointType m_LR;
    unsigned int m_Resolution;
    unsigned int m_RedIdx;
    unsigned int m_GreenIdx;
    unsigned int m_BlueIdx;
    RescaleFilterType::Pointer m_RescaleFilter;
  };

  typedef std::vector<Tile>                                                       TileVectorType;    
  
private:
  // prevent implementation
  GlImageActor(const Self&);
  void operator=(const Self&);

  // Load tile to GPU
  void LoadTile(Tile& tile);
  
  // Unload tile from GPU
  void UnloadTile(Tile& tile);

  // Clean the loaded tiles, getting rid of unnecessary ones
  void CleanLoadedTiles();

  // Clear all loaded tiles
  void ClearLoadedTiles();

  // Is tile loaded ?
  bool TileAlreadyLoaded(const Tile& tile);

  void ImageRegionToViewportExtent(const RegionType& region, double & ulx, double & uly, double & lrx, double& lry) const;

  void ImageRegionToViewportQuad(const RegionType & region, PointType & ul, PointType & ur, PointType & ll, PointType & lr, bool rotate = true) const;

  void ViewportExtentToImageRegion(const double& ulx, const double & uly, const double & lrx, const double & lry, RegionType & region) const;

  void UpdateResolution();
 
  unsigned int m_TileSize;

  std::string m_FileName;
  
  ReaderType::Pointer m_FileReader;

  TileVectorType m_LoadedTiles;

  unsigned int m_RedIdx;

  unsigned int m_GreenIdx;

  unsigned int m_BlueIdx;

  unsigned int m_CurrentResolution;

  ResolutionVectorType m_AvailableResolutions;

  PointType    m_Origin;
  SpacingType  m_Spacing;
  RegionType   m_LargestRegion;
  unsigned int m_NumberOfComponents;

  ImageSettings::Pointer m_ImageSettings;
  FragmentShader::Pointer m_Shader;

  RSTransformType::Pointer m_ViewportToImageTransform;
  RSTransformType::Pointer m_ImageToViewportTransform;

  RigidTransformType::Pointer m_ViewportForwardRotationTransform;
  RigidTransformType::Pointer m_ViewportBackwardRotationTransform;

  ResolutionAlgorithm::type m_ResolutionAlgorithm;

  bool m_SoftwareRendering;

}; // End class GlImageActor

} // End namespace otb

#endif
