/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otb_NonOptGlImageActor_h
#define otb_NonOptGlImageActor_h

#include "otbVectorImage.h"
#include "otbMultiChannelExtractROI.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "otbImageFileReader.h"
#include "otbGenericRSTransform.h"

#include "otbGlActor.h"
#include <string>

namespace otb
{

class OTBIce_EXPORT NonOptGlImageActor
  : public GlActor
{
public:
  typedef NonOptGlImageActor                                    Self;
  typedef GlActor                                         Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  itkNewMacro(Self);

  typedef VectorImage<float>                              VectorImageType;
  typedef VectorImageType::ImageKeywordlistType           ImageKeywordlistType;
  typedef VectorImageType::SizeType                       SizeType;
  typedef VectorImageType::IndexType                      IndexType;
  typedef VectorImageType::RegionType                     RegionType;
  typedef VectorImageType::SpacingType                    SpacingType;
  typedef VectorImageType::PointType                      PointType;

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

  const PointType & GetOrigin() const;

  const SpacingType & GetSpacing() const;

  std::string GetWkt() const;

  ImageKeywordlistType GetKwl() const;

  itkSetMacro(UseShader,bool);
  itkGetConstReferenceMacro(UseShader,bool);
  itkBooleanMacro(UseShader);

  itkSetMacro(MinRed,double);
  itkSetMacro(MinGreen,double);
  itkSetMacro(MinBlue,double);
  itkGetMacro(MinRed,double);
  itkGetMacro(MinGreen,double);
  itkGetMacro(MinBlue,double);

  itkSetMacro(MaxRed,double);
  itkSetMacro(MaxGreen,double);
  itkSetMacro(MaxBlue,double);
  itkGetMacro(MaxRed,double);
  itkGetMacro(MaxGreen,double);
  itkGetMacro(MaxBlue,double);

  itkSetMacro(RedIdx,unsigned int);
  itkGetMacro(RedIdx,unsigned int);
  itkSetMacro(GreenIdx,unsigned int);
  itkGetMacro(GreenIdx,unsigned int);
  itkSetMacro(BlueIdx,unsigned int);
  itkGetMacro(BlueIdx,unsigned int);

  itkGetMacro(NumberOfComponents,unsigned int);

protected:
  NonOptGlImageActor();

  virtual ~NonOptGlImageActor();

  typedef ImageFileReader<VectorImageType>                                        ReaderType;
  typedef MultiChannelExtractROI<float,float>                                     ExtractROIFilterType;
  typedef VectorRescaleIntensityImageFilter<VectorImageType,VectorImageType>      RescaleFilterType;
  typedef otb::GenericRSTransform<>                                               RSTransformType;
  typedef std::vector<unsigned int>                                               ResolutionVectorType;

  // Internal class to hold tiles
  class Tile
  {
  public:
    Tile()
      : m_Loaded(false),
        m_TextureId(0),
        m_ImageRegion(),
        m_UL(),
        m_UR(),
        m_LL(),
        m_LR(),
        m_Resolution(1),
        m_RedIdx(1),
        m_GreenIdx(2),
        m_BlueIdx(3),
        m_UseShader(false),
        m_MinRed(0),
        m_MaxRed(0),
        m_MinGreen(0),
        m_MaxGreen(0),
        m_MinBlue(0),
        m_MaxBlue(0)
    {
      m_UL.Fill(0);
      m_UR.Fill(0);
      m_LL.Fill(0);
      m_LR.Fill(0);
    }

    bool m_Loaded;
    unsigned int m_TextureId;
    RegionType m_ImageRegion;
    PointType m_UL;
    PointType m_UR;
    PointType m_LL;
    PointType m_LR;
    unsigned int m_Resolution;
    unsigned int m_RedIdx;
    unsigned int m_GreenIdx;
    unsigned int m_BlueIdx;
    bool m_UseShader;
    double m_MinRed;
    double m_MaxRed;
    double m_MinGreen;
    double m_MaxGreen;
    double m_MinBlue;
    double m_MaxBlue;
  };

  typedef std::vector<Tile>                                                       TileVectorType;

private:
  // prevent implementation
  NonOptGlImageActor(const Self&);
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

  void ImageRegionToViewportQuad(const RegionType & region, PointType & ul, PointType & ur, PointType & ll, PointType & lr) const;

  void ViewportExtentToImageRegion(const double& ulx, const double & uly, const double & lrx, const double & lry, RegionType & region) const;

  void UpdateResolution();

  void UpdateTransforms();

  static void InitShaders();

   unsigned int m_TileSize;

  std::string m_FileName;

  ReaderType::Pointer m_FileReader;

  TileVectorType m_LoadedTiles;

  unsigned int m_RedIdx;

  unsigned int m_GreenIdx;

  unsigned int m_BlueIdx;

  double m_MinRed;
  double m_MaxRed;
  double m_MinGreen;
  double m_MaxGreen;
  double m_MinBlue;
  double m_MaxBlue;

  unsigned int m_CurrentResolution;

  ResolutionVectorType m_AvailableResolutions;

  PointType    m_Origin;
  SpacingType  m_Spacing;
  RegionType   m_LargestRegion;
  unsigned int m_NumberOfComponents;

  bool m_UseShader;

  static unsigned int m_StandardShader;
  static unsigned int m_StandardShaderProgram;
  static bool m_ShaderInitialized;

  RSTransformType::Pointer m_ViewportToImageTransform;
  RSTransformType::Pointer m_ImageToViewportTransform;

}; // End class NonOptGlImageActor

} // End namespace otb

#endif
