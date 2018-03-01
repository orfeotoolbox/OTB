
#ifndef PersistentMaskFilter_txx
#define PersistentMaskFilter_txx

#include "itkDefaultConvertPixelTraits.h"
#include "itkProgressReporter.h"

#include "otbOGRFieldWrapper.h"

#include "PersistentMaskFilter.h"

// --------- otb::PersistentMaskFilter ---------------------

namespace otb
{
template<class TInputImage>
PersistentMaskFilter<TInputImage>
::PersistentMaskFilter()
{
  this->SetNumberOfRequiredOutputs(2);
  //this->SetNthOutput(0,TInputImage::New());
  m_TileNumber = 1;
}

template<class TInputImage>
void
PersistentMaskFilter<TInputImage>
::Reset(void)
{
}

template<class TInputImage>
void
PersistentMaskFilter<TInputImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
}

template<class TInputImage>
void
PersistentMaskFilter<TInputImage>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
}
/*
template<class TImage>
void
PersistentMaskFilter<TImage>
::GenerateData()
{
  std::cout << this->GetInput()->GetRequestedRegion().GetSize() << this->GetInput()->GetRequestedRegion().GetIndex() << std::endl;
  InputImagePointer inputPtr = const_cast<TImage *>(this->GetInput());
      
}*/
  
template<class TInputImage>
typename PersistentMaskFilter<TInputImage>::OGRDataSourcePointerType
PersistentMaskFilter<TInputImage>
::ProcessTile()
{  
  ++m_TileNumber;
  // Apply an ExtractImageFilter to avoid problems with filters asking for the LargestPossibleRegion
  typedef itk::ExtractImageFilter<InputImageType, InputImageType> ExtractImageFilterType;
  typename ExtractImageFilterType::Pointer extract = ExtractImageFilterType::New();
  extract->SetInput( this->GetInput() );
  
  SizeType paddedRegionSize = this->GetInput()->GetRequestedRegion().GetSize();
  //paddedRegionSize[0] += 1;
  //paddedRegionSize[1] += 1;
  
  RegionType paddedRegion(  this->GetInput()->GetRequestedRegion().GetIndex() , paddedRegionSize );
  //paddedRegion.Crop(this->GetInput()->GetLargestPossibleRegion());
  
  extract->SetExtractionRegion( paddedRegion );
  extract->Update();



  // WARNING: itk::ExtractImageFilter does not copy the MetadataDictionary
  extract->GetOutput()->SetMetaDataDictionary(this->GetInput()->GetMetaDataDictionary());

  typename LabelImageToOGRDataSourceFilterType::Pointer labelImageToOGRDataFilter =
                                              LabelImageToOGRDataSourceFilterType::New();
  labelImageToOGRDataFilter->SetInput(extract->GetOutput());
  //labelImageToOGRDataFilter->SetInput(this->GetInput());
  
  labelImageToOGRDataFilter->UpdateLargestPossibleRegion();
 
  labelImageToOGRDataFilter->SetFieldName(m_FieldName);
  labelImageToOGRDataFilter->SetUse8Connected(m_Use8Connected);
  
  labelImageToOGRDataFilter->Update();
  OGRDataSourcePointerType tmpDS = const_cast<OGRDataSourceType *>(labelImageToOGRDataFilter->GetOutput());
  OGRLayerType tmpLayer = tmpDS->GetLayerChecked(0);
  
  OGRDataSourcePointerType outDS = OGRDataSourceType::New();
  OGRLayerType outLayer = outDS->CreateLayer("Layer");
  OGRFieldDefn field(m_FieldName.c_str(),OFTInteger);
  outLayer.CreateField(field, true);
  OGRFieldDefn fieldSize("size",OFTReal);
  outLayer.CreateField(fieldSize, true);
  OGRFieldDefn fieldPerimeter("perimeter",OFTReal);
  outLayer.CreateField(fieldPerimeter, true);
  
  //typename OGRLayerType::const_iterator featIt = tmpLayer.begin();
  typename OGRLayerType::iterator featIt = tmpLayer.begin();
  for(; featIt!=tmpLayer.end(); ++featIt)
  {
    OGRFeatureType outFeature(outLayer.GetLayerDefn());
   
    if (   std::find(m_Labels.begin(),m_Labels.end(),(*featIt)[m_FieldName].ogr::Field::GetValue<int>()) != m_Labels.end())
    {

      //simplify
      double tol = 1;
      const OGRGeometry * geom = (*featIt).GetGeometry();
      assert(geom && "geometry is NULL ! Can't simplify it.");
      (*featIt).ogr::Feature::SetGeometryDirectly(ogr::Simplify(*geom,tol));
      
      // Compute Attributes
      double area = static_cast<const OGRPolygon *>((*featIt).GetGeometry())->get_Area();
      typename InputImageType::SpacingType spacing = this->GetInput()->GetSignedSpacing();
      double pixelsArea = area / (vcl_abs(spacing[0]*spacing[1]));
    
      double perimeter = static_cast<const OGRPolygon *>((*featIt).GetGeometry())->getExteriorRing()->get_Length();
      //otbMsgDebugMacro(<<"DN = "<<field.GetValue<int>()<<", area = "<<pixelsArea);
      
      
      outFeature.SetFrom( *featIt, TRUE );
      outFeature["size"].ogr::Field::SetValue<double>(pixelsArea);
      //outFeature[m_FieldName].ogr::Field::SetValue<int>(1);
      outFeature["perimeter"].ogr::Field::SetValue<double>(perimeter);
      outLayer.CreateFeature( outFeature );

    }
    else
    {
    }
  }
  return outDS;
}



} // End namespace otb

#endif

