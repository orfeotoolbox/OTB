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
#ifndef otb_GlImageActor_h
#define otb_GlImageActor_h

#include "otbGlActor.h"

#include "otbVectorImage.h"
#include "otbMultiChannelExtractROI.h"
#include "otbImageFileReader.h"
#include "otbGenericRSTransform.h"
#include "otbFragmentShader.h"

namespace otb
{

class GlImageActor 
  : public GlActor
{
public:
  typedef GlImageActor                                    Self;
  typedef GlActor                                         Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  itkNewMacro(Self);

  typedef VectorImage<float>                              VectorImageType;
  typedef VectorImageType::PixelType                      PixelType;
  typedef VectorImageType::ImageKeywordlistType           ImageKeywordlistType;
  typedef itk::MetaDataDictionary                        MetaDataDictionaryType;
  typedef VectorImageType::SizeType                       SizeType;
  typedef VectorImageType::IndexType                      IndexType;
  typedef VectorImageType::RegionType                     RegionType;
  typedef VectorImageType::SpacingType                    SpacingType;
  typedef VectorImageType::PointType                      PointType;

  // Initialize with a new image
  void Initialize(const std::string & filename);

  // Retrieve the full extent of the actor
  virtual void GetExtent(double & ulx, double & uly, double & lrx, double & lry) const;

  // Update internal actor state with respect to ViewSettings
  virtual void ProcessViewSettings();

  // Heavy load/unload operations of data
  virtual void UpdateData();

  // Gl rendering of current state
  virtual void Render();

  // Automatic color adjustment
  void AutoColorAdjustment(double & minRed, double & maxRed, double & minGreen, double & maxGreen, double & minBlue, double & maxBlue, bool full = true, double lcp = 0.002, double hcp = 0.002);

  const PointType & GetOrigin() const;

  const SpacingType & GetSpacing() const;

  std::string GetWkt() const;
  
  ImageKeywordlistType GetKwl() const;

  MetaDataDictionaryType & GetMetaDataDictionary() const;

  itkSetMacro(RedIdx,unsigned int);
  itkGetMacro(RedIdx,unsigned int);
  itkSetMacro(GreenIdx,unsigned int);
  itkGetMacro(GreenIdx,unsigned int);
  itkSetMacro(BlueIdx,unsigned int);
  itkGetMacro(BlueIdx,unsigned int);

  itkGetMacro(NumberOfComponents,unsigned int);
  itkGetMacro(CurrentResolution,unsigned int);

  PointType ViewportToImageTransform(const PointType & in, bool physical = true) const;
  
  bool GetPixelFromViewport(const PointType & in, PixelType & pixel) const;

  itkGetObjectMacro(Shader,FragmentShader);
  itkSetObjectMacro(Shader,FragmentShader);

protected:
  GlImageActor();
  
  virtual ~GlImageActor();

  typedef ImageFileReader<VectorImageType>                                        ReaderType;
  typedef MultiChannelExtractROI<float,float>                                     ExtractROIFilterType;
  typedef otb::GenericRSTransform<>                                               RSTransformType;
  typedef std::vector<unsigned int>                                               ResolutionVectorType;

  // Internal class to hold tiles
  class Tile
  {
  public:
    Tile()
      : m_Loaded(false),
        m_ImageRegion(),
        m_Image(),
        m_UL(),
        m_UR(),
        m_LL(),
        m_LR(),
        m_Resolution(1),
        m_RedIdx(1),
        m_GreenIdx(2),
        m_BlueIdx(3)
    {
      m_UL.Fill(0);
      m_UR.Fill(0);
      m_LL.Fill(0);
      m_LR.Fill(0);
    }

    bool m_Loaded;
    unsigned int m_TextureId;
    RegionType m_ImageRegion;
    VectorImageType::Pointer m_Image;
    PointType m_UL;
    PointType m_UR;
    PointType m_LL;
    PointType m_LR;
    unsigned int m_Resolution;
    unsigned int m_RedIdx;
    unsigned int m_GreenIdx;
    unsigned int m_BlueIdx;
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

  // Clean the loaded tiles, getting rid of unecessary ones
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

  FragmentShader::Pointer m_Shader;

  RSTransformType::Pointer m_ViewportToImageTransform;
  RSTransformType::Pointer m_ImageToViewportTransform;

}; // End class GlImageActor

} // End namespace otb

#endif
