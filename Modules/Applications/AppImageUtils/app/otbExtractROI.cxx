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

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbMultiChannelExtractROI.h"
#include "otbStandardFilterWatcher.h"
#include "otbWrapperNumericalParameter.h"
#include "otbWrapperListViewParameter.h"
#include "otbWrapperTypes.h"
#include "otbOGRDataSourceToLabelImageFilter.h"
#include <algorithm> 

#include "otbWrapperElevationParametersHandler.h"

#include "otbGenericRSTransform.h"

namespace otb
{
namespace Wrapper
{

class ExtractROI : public Application
{
public:
  /** Standard class typedefs. */
  typedef ExtractROI                          Self;
  typedef Application                         Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  typedef otb::GenericRSTransform<>           RSTransformType;
  typedef RSTransformType::InputPointType     Point3DType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ExtractROI, otb::Application);

  /** Filters typedef */
  typedef otb::MultiChannelExtractROI<FloatVectorImageType::InternalPixelType,
                                      FloatVectorImageType::InternalPixelType> ExtractROIFilterType;

private:
  void DoInit() ITK_OVERRIDE
  {
    SetName("ExtractROI");
    SetDescription("Extract a ROI defined by the user.");

    // Documentation
    SetDocName("Extract ROI");
    SetDocLongDescription("This application extracts a Region Of Interest with user defined size, or reference.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Manip);

    // Set parameter input
    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription("in", "Input image.");
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "Output image.");

    AddRAMParameter();

    AddParameter(ParameterType_Choice,"mode","Extraction mode");
    AddChoice("mode.standard","Standard");
    SetParameterDescription("mode.standard","In standard mode, extract is done according to the coordinates entered by the user");

    // Fit mode : the ROI is computed through a ref vector dataset or a ref image 
    AddChoice("mode.fit","Fit");
    SetParameterDescription("mode.fit","In fit mode, extract is made from a reference : image or vector dataset.");
    AddParameter( ParameterType_InputImage , "mode.fit.im" , "Reference image" );
    SetParameterDescription( "mode.fit.im" , "Reference image to define the ROI" );
    AddParameter( ParameterType_InputFilename , "mode.fit.vect" , "Vector dataset" );
    SetParameterDescription( "mode.fit.vect" , "Reference vector dataset to define the ROI" );

    // Extent mode : ROI is computed with two point (upper left and lower left corners)
    AddChoice( "mode.extent" , "Extent" );
    SetParameterDescription( "mode.extent" , "In extent mode, the ROI is defined by two points, the upper left corner and the lower right corner." );
    AddParameter( ParameterType_Float , "mode.extent.ulx" , "Up left X/lon" );
    SetParameterDescription( "mode.extent.ulx" , "X/lon coordinate of upper left corner point." );
    AddParameter( ParameterType_Float , "mode.extent.uly" , "Up left Y/lat" );
    SetParameterDescription( "mode.extent.uly" , "Y/lat coordinate of upper left corner point." );
    AddParameter( ParameterType_Float , "mode.extent.lrx" , "Low right X/lon" );
    SetParameterDescription( "mode.extent.lrx" , "X/lon coordinate of lower right corner point." );
    AddParameter( ParameterType_Float , "mode.extent.lry" , "Low right Y/lat" );
    SetParameterDescription( "mode.extent.lry" , "Y/lat coordinate of lower right corner point." );
    // Unit of extent mode
    AddParameter( ParameterType_Choice , "mode.extent.unit" , "Unit" );
    AddChoice( "mode.extent.unit.pxl" , "Pixel" );
    AddChoice( "mode.extent.unit.phy" , "Physical" );
    AddChoice( "mode.extent.unit.lonlat" , "Lon/Lat" );

    // Radius mode : ROI is computed through a point and a radius
    AddChoice( "mode.radius" , "Radius" );
    SetParameterDescription( "mode.radius" , "In radius mode, the ROI is defined by a point and a radius." );

    AddParameter( ParameterType_Float , "mode.radius.r" , "Radius" );
    AddParameter( ParameterType_Choice , "mode.radius.unitr" , "Radius unit" );
    AddChoice( "mode.radius.unitr.pxl" , "Pixel" );
    AddChoice( "mode.radius.unitr.phy" , "Physical" );

    AddParameter( ParameterType_Float , "mode.radius.cx" , "X/lon coordinate of the center" );
    AddParameter( ParameterType_Float , "mode.radius.cy" , "Y/lat coordinate of the center" );
    AddParameter( ParameterType_Choice , "mode.radius.unitc" , "Center unit" );
    AddChoice( "mode.radius.unitc.pxl" , "Pixel" );
    AddChoice( "mode.radius.unitc.phy" , "Physical" );
    AddChoice( "mode.radius.unitc.lonlat" , "Lon/Lat" );

    // Elevation
    ElevationParametersHandler::AddElevationParameters(this,"elev");

    // Standard parameter
    AddParameter(ParameterType_Int,  "startx", "Start X");
    SetParameterDescription("startx", "ROI start x position.");
    AddParameter(ParameterType_Int,  "starty", "Start Y");
    SetParameterDescription("starty", "ROI start y position.");
    AddParameter(ParameterType_Int,  "sizex",  "Size X");
    SetParameterDescription("sizex","size along x in pixels.");
    AddParameter(ParameterType_Int,  "sizey",  "Size Y");
    SetParameterDescription("sizey","size along y in pixels.");

    // Default values
    SetDefaultParameterInt("startx", 0);
    SetDefaultParameterInt("starty", 0);
    SetDefaultParameterInt("sizex",  0);
    SetDefaultParameterInt("sizey",  0);

    SetDefaultParameterInt("mode.extent.ulx", 0);
    SetDefaultParameterInt("mode.extent.uly", 0);
    SetDefaultParameterInt("mode.extent.lrx",  0);
    SetDefaultParameterInt("mode.extent.lry",  0);

    SetDefaultParameterInt("mode.radius.r", 0);
    SetDefaultParameterInt("mode.radius.cx",  0);
    SetDefaultParameterInt("mode.radius.cy",  0);

    // Channelist Parameters
    AddParameter(ParameterType_ListView,  "cl", "Output Image channels");
    SetParameterDescription("cl","Channels to write in the output image.");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "VegetationIndex.hd");
    SetDocExampleParameterValue("startx", "40");
    SetDocExampleParameterValue("starty", "250");
    SetDocExampleParameterValue("sizex", "150");
    SetDocExampleParameterValue("sizey", "150");
    SetDocExampleParameterValue("out", "ExtractROI.tif");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
    // Update the sizes only if the user has not defined a size
    if ( HasValue("in") )
      {
      ExtractROIFilterType::InputImageType* inImage = GetParameterImage("in");
      ExtractROIFilterType::InputImageType::RegionType  largestRegion = inImage->GetLargestPossibleRegion();

      if (!HasUserValue("sizex")  && !HasUserValue("sizey") )
        {
        SetParameterInt("sizex",largestRegion.GetSize()[0], false);
        SetParameterInt("sizey",largestRegion.GetSize()[1], false);

        // Compute extent parameter with default sizex and sizey
        if ( GetParameterString("mode") == "extent" )
          {
          if ( GetParameterString( "mode.extent.unit" ) == "pxl" )
            {
            if  ( !HasUserValue("mode.extent.ulx") && !HasUserValue("mode.extent.uly") )
              {
              SetParameterFloat("mode.extent.ulx", 0 , false);
              SetParameterFloat("mode.extent.uly", 0 , false);
              }

            if  ( !HasUserValue("mode.extent.lrx") && !HasUserValue("mode.extent.lry") )
              {
              SetParameterFloat("mode.extent.lrx",largestRegion.GetSize()[0], false);
              SetParameterFloat("mode.extent.lry",largestRegion.GetSize()[1], false);
              }
            }
          else if ( GetParameterString( "mode.extent.unit" ) == "phy" )
            {
            itk::Point<float, 2> ulp,  lrp;
            FloatVectorImageType::IndexType uli , lri;
            if  ( !HasUserValue("mode.extent.ulx") && !HasUserValue("mode.extent.uly") )
              {
              uli.Fill(0);
              inImage->TransformIndexToPhysicalPoint(uli,ulp);
              SetParameterFloat("mode.extent.ulx",ulp[0], false);
              SetParameterFloat("mode.extent.uly",ulp[1], false);
              }
            if  ( !HasUserValue("mode.extent.lrx") && !HasUserValue("mode.extent.lry") )
              {
              lri[ 0 ] = largestRegion.GetSize()[0];
              lri[ 1 ] = largestRegion.GetSize()[1];
              inImage->TransformIndexToPhysicalPoint(lri,lrp);
              SetParameterFloat("mode.extent.lrx",lrp[0], false);
              SetParameterFloat("mode.extent.lry",lrp[1], false);  
              }
            }
          else if ( GetParameterString( "mode.extent.unit" ) == "lonlat" )
            {
            RSTransformType::Pointer rsTransform = RSTransformType::New();
            rsTransform->SetInputKeywordList( inImage->GetImageKeywordlist() );
            rsTransform->SetInputProjectionRef( inImage->GetProjectionRef() );
            rsTransform->InstantiateTransform();
            FloatVectorImageType::IndexType uli , lri;
            itk::Point<float, 2> ulp_in,  lrp_in , ulp_out , lrp_out;
            if  ( !HasUserValue("mode.extent.ulx") && !HasUserValue("mode.extent.uly") )
              {
              uli.Fill(0);
              inImage->TransformIndexToPhysicalPoint(uli,ulp_in);
              ulp_out = rsTransform->TransformPoint( ulp_in );
              SetParameterFloat( "mode.extent.ulx" , ulp_out[ 0 ] , false );
              SetParameterFloat( "mode.extent.uly" , ulp_out[ 1 ] , false );
              }
            if  ( !HasUserValue("mode.extent.lrx") && !HasUserValue("mode.extent.lry") )
              {
              lri[ 0 ] = largestRegion.GetSize()[ 0 ];
              lri[ 1 ] = largestRegion.GetSize()[ 1 ];
              inImage->TransformIndexToPhysicalPoint( lri , lrp_in );
              lrp_out = rsTransform->TransformPoint( lrp_in );
              SetParameterFloat( "mode.extent.lrx" , lrp_out[ 0 ] , false );
              SetParameterFloat( "mode.extent.lry" , lrp_out[ 1 ] , false );
              }
            }
          }

        // Compute radius parameter with default sizex and sizey
        if ( GetParameterString("mode") == "radius" )
          {
          if (!HasUserValue("mode.radius.r")  && !HasUserValue("mode.radius.cx") \
              && !HasUserValue("mode.radius.cy") )
            {
            FloatVectorImageType::IndexType centeri , helpRi;
            centeri[ 0 ] = largestRegion.GetSize()[0] / 2;
            centeri[ 1 ] = largestRegion.GetSize()[1] / 2;
            helpRi[ 0 ] = 0;
            helpRi[ 1 ] = largestRegion.GetSize()[1] / 2;

            if ( GetParameterString("mode.radius.unitr") == "pxl" )
              {
              SetParameterFloat( "mode.radius.r" , helpRi[1] , false );
              }
            if ( GetParameterString("mode.radius.unitr") == "phy" )
              {
              itk::Point<float, 2> centerp , helpRp;
              inImage->TransformIndexToPhysicalPoint(centeri,centerp);
              inImage->TransformIndexToPhysicalPoint(helpRi,helpRp);
              SetParameterFloat( "mode.radius.r" , centerp[0]-helpRp[0] , false );
              }
            if ( GetParameterString("mode.radius.unitc") == "pxl" )
              {
              SetParameterFloat( "mode.radius.cx" , centeri[0] , false );
              SetParameterFloat( "mode.radius.cy" , centeri[1] , false) ;
              }
            if ( GetParameterString("mode.radius.unitc") == "phy" )
              {
              itk::Point<float, 2> centerp , helpRp;
              inImage->TransformIndexToPhysicalPoint(centeri,centerp);
              SetParameterFloat( "mode.radius.cx" , centerp[0] , false );
              SetParameterFloat( "mode.radius.cy" , centerp[1] , false) ;
              }
            if ( GetParameterString("mode.radius.unitc") == "lonlat" )
              {
              RSTransformType::Pointer rsTransform = RSTransformType::New();
              rsTransform->SetInputKeywordList( inImage->GetImageKeywordlist() );
              rsTransform->SetInputProjectionRef( inImage->GetProjectionRef() );
              rsTransform->InstantiateTransform();
              itk::Point<float, 2> centerp_in,  centerp_out;
              inImage->TransformIndexToPhysicalPoint(centeri,centerp_in);
              centerp_out = rsTransform->TransformPoint( centerp_in );
              SetParameterFloat( "mode.radius.cx" , centerp_out[ 0 ] , false );
              SetParameterFloat( "mode.radius.cy" , centerp_out[ 1 ] , false );
              }
            }
          }
        }


      unsigned int nbComponents = inImage->GetNumberOfComponentsPerPixel();
      ListViewParameter *clParam = dynamic_cast<ListViewParameter*>(GetParameterByKey("cl"));
      // Update the values of the channels to be selected if nbComponents is changed
      if (clParam != ITK_NULLPTR && clParam->GetNbChoices() != nbComponents)
        {

        ClearChoices("cl");
        for (unsigned int idx = 0; idx < nbComponents; ++idx)
          {
          std::ostringstream key, item;
          key<<"cl.channel"<<idx+1;
          item<<"Channel"<<idx+1;
          AddChoice(key.str(), item.str());
          }
        }

      // Put the limit of the index and the size relative the image
      SetMinimumParameterIntValue("sizex", 0);
      SetMaximumParameterIntValue("sizex", largestRegion.GetSize(0));

      SetMinimumParameterIntValue("sizey", 0);
      SetMaximumParameterIntValue("sizey", largestRegion.GetSize(1));

      SetMinimumParameterIntValue("startx", 0);
      SetMaximumParameterIntValue("startx", largestRegion.GetSize(0));

      SetMinimumParameterIntValue("starty", 0);
      SetMaximumParameterIntValue("starty", largestRegion.GetSize(1));

      // Update the start and size parameter depending on the mode
      if ( GetParameterString("mode") == "extent" )
          computeExtent();
      if (GetParameterString("mode") == "radius")
          computeRadius();

      // Crop the roi region to be included in the largest possible
      // region

      if(!this->CropRegionOfInterest())
        {
        // Put the index of the ROI to origin and try to crop again
        SetParameterInt("startx",0, false);
        SetParameterInt("starty",0, false);
        this->CropRegionOfInterest();
        }
      
      

    if(GetParameterString("mode")=="fit")
      {
      this->SetParameterRole("startx",Role_Output);
      this->SetParameterRole("starty",Role_Output);
      this->SetParameterRole("sizex",Role_Output);
      this->SetParameterRole("sizey",Role_Output);

      this->DisableParameter("startx");
      this->DisableParameter("starty");
      this->DisableParameter("sizex");
      this->DisableParameter("sizey");

      }
    else if(GetParameterString("mode")=="standard" || GetParameterString("mode")=="extent" \
            || GetParameterString("mode")== "radius" )
      {
      this->SetParameterRole("startx",Role_Input);
      this->SetParameterRole("starty",Role_Input);
      this->SetParameterRole("sizex",Role_Input);
      this->SetParameterRole("sizey",Role_Input);

      this->EnableParameter("startx");
      this->EnableParameter("starty");
      this->EnableParameter("sizex");
      this->EnableParameter("sizey");
      }
    }

    // If not standard mode start and size parameter will be computed by the application
    if ( GetParameterString( "mode" ) != "standard" )
      {
      MandatoryOff("startx");
      MandatoryOff("starty");
      MandatoryOff("sizex");
      MandatoryOff("sizey");
      }
    else if ( GetParameterString( "mode" ) == "standard" )
      {
      MandatoryOn("startx");
      MandatoryOn("starty");
      MandatoryOn("sizex");
      MandatoryOn("sizey");
      }

    if ( GetParameterString( "mode" ) == "fit" && HasValue( "mode.fit.im" ) )
      {
      MandatoryOff( "mode.fit.vect" );
      MandatoryOn( "mode.fit.im" );
      }
    else if ( GetParameterString( "mode" ) == "fit" && HasValue( "mode.fit.vect" ) )
      {
      MandatoryOff( "mode.fit.im" );
      MandatoryOn( "mode.fit.vect" );
      }
  } 

  bool CropRegionOfInterest()
  {
    FloatVectorImageType::RegionType region;
    region.SetSize(0,  GetParameterInt("sizex"));
    region.SetSize(1,  GetParameterInt("sizey"));
    region.SetIndex(0, GetParameterInt("startx"));
    region.SetIndex(1, GetParameterInt("starty"));

    if ( HasValue("in") )
      {
      if (region.Crop(GetParameterImage("in")->GetLargestPossibleRegion()))
        {
        SetParameterInt("sizex",region.GetSize(0), HasUserValue("sizex"));
        SetParameterInt("sizey",region.GetSize(1), HasUserValue("sizey"));
        SetParameterInt("startx",region.GetIndex(0), HasUserValue("startx"));
        SetParameterInt("starty",region.GetIndex(1), HasUserValue("starty"));
        return true;
        }
      }
    return false;
  }

  void
  computeExtent()
  {
    assert( GetParameterString( "mode" ) == "extent" );
    int pixelValue;
    // Compute standard parameter depending on the unit choosen by the user
    if (GetParameterString( "mode.extent.unit" ) == "pxl" )
      {
      pixelValue = floor( GetParameterFloat( "mode.extent.ulx" ) );
      SetParameterInt( "startx", pixelValue , false );
      pixelValue = floor( GetParameterFloat( "mode.extent.lrx" ) - pixelValue ) + 1;
      SetParameterInt( "sizex", pixelValue , false );
      pixelValue = floor( GetParameterFloat( "mode.extent.uly" ) );
      SetParameterInt( "starty", pixelValue , false );
      pixelValue = floor( GetParameterFloat( "mode.extent.lry" ) - pixelValue ) + 1;
      SetParameterInt( "sizey", pixelValue , false );
      }
    else if( GetParameterString( "mode.extent.unit" ) == "phy" )
      {
      itk::Point<float, 2> ulp,  lrp;
      ulp[ 0 ] = GetParameterFloat( "mode.extent.ulx" );
      ulp[ 1 ] = GetParameterFloat( "mode.extent.uly" );
      lrp[ 0 ] = GetParameterFloat( "mode.extent.lrx" );
      lrp[ 1 ] = GetParameterFloat( "mode.extent.lry" );

      ExtractROIFilterType::InputImageType * inImage = GetParameterImage("in");
      FloatVectorImageType::IndexType uli , lri;
      bool startin , sizein ;
      startin = inImage->TransformPhysicalPointToIndex(ulp,uli);
      sizein = inImage->TransformPhysicalPointToIndex(lrp,lri);
      if ( startin )
        {
        SetParameterInt( "startx", uli[0] , false );
        SetParameterInt( "starty", uli[1] , false );
        }
        
      if( startin && sizein )
        {
        SetParameterInt( "sizex", lri[0] - uli[0] , false );
        SetParameterInt( "sizey", lri[1] - uli[1] , false );
        }
      }
    else if( GetParameterString( "mode.extent.unit" ) == "lonlat" )
      {
      RSTransformType::Pointer rsTransform = RSTransformType::New();
      ExtractROIFilterType::InputImageType* inImage = GetParameterImage("in");
      rsTransform->SetOutputKeywordList( inImage->GetImageKeywordlist() );
      rsTransform->SetOutputProjectionRef( inImage->GetProjectionRef() );
      rsTransform->InstantiateTransform();
      itk::Point<float, 2> ulp_in,  lrp_in , ulp_out , lrp_out;
      ulp_in[ 0 ] = GetParameterFloat( "mode.extent.ulx" );
      ulp_in[ 1 ] = GetParameterFloat( "mode.extent.uly" );
      lrp_in[ 0 ] = GetParameterFloat( "mode.extent.lrx" );
      lrp_in[ 1 ] = GetParameterFloat( "mode.extent.lry" );
      ulp_out = rsTransform->TransformPoint(ulp_in);
      lrp_out = rsTransform->TransformPoint(lrp_in);

      FloatVectorImageType::IndexType uli_out , lri_out;
      bool startin , sizein ;
      startin = inImage->TransformPhysicalPointToIndex(ulp_out,uli_out);
      sizein = inImage->TransformPhysicalPointToIndex(lrp_out,lri_out);

      if ( startin )
        {
        SetParameterInt( "startx", uli_out[0] , false );
        SetParameterInt( "starty", uli_out[1] , false );
        }
            
      if( startin && sizein )
        {
        SetParameterInt( "sizex", lri_out[0] - uli_out[0] , false );
        SetParameterInt( "sizey", lri_out[1] - uli_out[1] , false );
        }
      }  
  }

  void
  computeRadius()
  {
    int pixelValue;
    assert( GetParameterString( "mode" ) == "radius" );
    // First compute sizex sizey thanks to the radius
    if ( GetParameterString( "mode.radius.unitr" ) == "pxl" )
      {
      pixelValue = floor( 2 * GetParameterFloat( "mode.radius.r" ) + 1);
      SetParameterInt( "sizey", pixelValue , false );
      SetParameterInt( "sizex", pixelValue , false );
      }
    if ( GetParameterString( "mode.radius.unitr" ) == "phy" )
      {
      ExtractROIFilterType::InputImageType * inImage = GetParameterImage("in");
      itk::Point<float, 2> sizexp , sizeyp , ulcp ;
      FloatVectorImageType::IndexType sizexi , sizeyi , ulci;
      ulci.Fill(0);
      inImage->TransformIndexToPhysicalPoint( ulci , ulcp );
      sizexp = ulcp; 
      sizeyp = ulcp; 
      sizexp[0] += GetParameterFloat( "mode.radius.r" );
      sizeyp[1] += GetParameterFloat( "mode.radius.r" );
      bool lgtx , lgty;
      lgtx = inImage->TransformPhysicalPointToIndex(sizexp,sizexi);
      lgty = inImage->TransformPhysicalPointToIndex(sizeyp,sizeyi);
      int maxR = ( std::min( inImage->GetLargestPossibleRegion().GetSize()[0] , inImage->GetLargestPossibleRegion().GetSize()[1] ) )/2;
      if ( lgtx && lgty)
        {
        pixelValue = std::max( sizexi[0] , sizeyi[1] );
        if ( maxR>pixelValue )
          {
          pixelValue = std::min( static_cast<int>(std::min( sizexi[0] , sizeyi[1] ) ) , maxR );
          } 
        }
      else if ( lgtx )
        {
        pixelValue = sizexi[0];
        }
      else if ( lgty )
        {
        pixelValue = sizeyi[1];
        }
      if ( maxR>pixelValue )
        {
          pixelValue = std::min( pixelValue , maxR );
        }
      SetParameterInt( "sizey", 2 * pixelValue + 1 , false );
      SetParameterInt( "sizex", 2 * pixelValue + 1 , false );
      }

    // Then compute startx and starty
    bool size = ( HasValue("sizex")  && HasValue("sizey") );
    if ( GetParameterString( "mode.radius.unitc" ) == "pxl" && size )
      {
      pixelValue = floor(GetParameterFloat( "mode.radius.cx" ));
      SetParameterInt( "startx", pixelValue - GetParameterInt("sizex") , false );
      pixelValue = floor(GetParameterFloat( "mode.radius.cy" ));
      SetParameterInt( "starty", pixelValue - GetParameterInt("sizey") , false );
      }
    if ( GetParameterString( "mode.radius.unitc" ) == "phy" && size ) 
      {
        itk::Point<float, 2> centerp;
        centerp[ 0 ] = GetParameterFloat( "mode.radius.cx" );
        centerp[ 1 ] = GetParameterFloat( "mode.radius.cy" );
        ExtractROIFilterType::InputImageType * inImage = GetParameterImage("in");
        FloatVectorImageType::IndexType centeri ;
        bool isIn ;
        isIn = inImage->TransformPhysicalPointToIndex(centerp,centeri);
        if ( isIn )
        {
        SetParameterInt( "startx", centeri[0] - GetParameterInt("sizex") , false );
        SetParameterInt( "starty", centeri[1] - GetParameterInt("sizey") , false );
        }
      }
    if ( GetParameterString( "mode.radius.unitc" ) == "lonlat" && size )
      {
      RSTransformType::Pointer rsTransform = RSTransformType::New();
      ExtractROIFilterType::InputImageType* inImage = GetParameterImage("in");
      rsTransform->SetOutputKeywordList( inImage->GetImageKeywordlist() );
      rsTransform->SetOutputProjectionRef( inImage->GetProjectionRef() );
      rsTransform->InstantiateTransform();
      itk::Point<float, 2> centerp_in , centerp_out;
      centerp_in[ 0 ] = GetParameterFloat( "mode.extent.cx" );
      centerp_in[ 1 ] = GetParameterFloat( "mode.extent.cy" );
      centerp_out = rsTransform->TransformPoint(centerp_in);
      FloatVectorImageType::IndexType centeri_out;
      bool isIn;
      isIn = inImage->TransformPhysicalPointToIndex( centerp_out , centeri_out );
      if ( isIn )
        {
        SetParameterInt( "startx", centeri_out[0] - GetParameterInt("sizex") , false );
        SetParameterInt( "starty", centeri_out[1] - GetParameterInt("sizey") , false );
        }
      }
  }

  void DoExecute() ITK_OVERRIDE
  {
    ExtractROIFilterType::InputImageType* inImage = GetParameterImage("in");
    inImage->UpdateOutputInformation();

    if ( HasValue( "mode.fit.vect" ) && GetParameterString("mode") == "fit" )
      {
      otb::ogr::DataSource::Pointer ogrDS;
      ogrDS = otb::ogr::DataSource::New(GetParameterString("in"), otb::ogr::DataSource::Modes::Read);
      double ulx, uly, lrx, lry;
      bool extentAvailable = true;
      std::string inputProjectionRef = "";
      // First try to get the extent in the metadata
      try
        {
        inputProjectionRef = ogrDS->GetGlobalExtent(ulx,uly,lrx,lry);
        }
      catch(const itk::ExceptionObject&)
        {
        extentAvailable = false;
        }
      // If no extent available force the computation of the extent
      if (!extentAvailable)
        {
        try
          {
          inputProjectionRef = ogrDS->GetGlobalExtent(ulx,uly,lrx,lry,true);
          extentAvailable = true;
          }
        catch(itk::ExceptionObject & err)
          {
          extentAvailable = false;

          otbAppLogFATAL(<<"Failed to retrieve the spatial extent of the dataset in force mode. The spatial extent is mandatory when orx, ory, spx and spy parameters are not set, consider setting them. Error from library: "<<err.GetDescription());
          }
        }

      if (extentAvailable)
        {
        RSTransformType::Pointer rsTransform = RSTransformType::New();
        rsTransform->SetInputProjectionRef(inputProjectionRef);
        rsTransform->SetOutputKeywordList( inImage->GetImageKeywordlist() );
        rsTransform->SetOutputProjectionRef( inImage->GetProjectionRef() );
        rsTransform->InstantiateTransform();

        itk::Point<float, 2> ulp_in,  lrp_in , ulp_out , lrp_out;
        ulp_in[ 0 ] = ulx ;
        ulp_in[ 1 ] = uly ;
        lrp_in[ 0 ] = lrx ;
        lrp_in[ 1 ] = lry ;
        ulp_out = rsTransform->TransformPoint(ulp_in);
        lrp_out = rsTransform->TransformPoint(lrp_in);

        FloatVectorImageType::IndexType uli_out , lri_out;
        bool startin , sizein ;
        startin = inImage->TransformPhysicalPointToIndex(ulp_out,uli_out);
        sizein = inImage->TransformPhysicalPointToIndex(lrp_out,lri_out);

        if ( startin )
            {
            SetParameterInt( "startx", uli_out[0] , false );
            SetParameterInt( "starty", uli_out[1] , false );
            }
                
        if( startin && sizein )
            {
            SetParameterInt( "sizey", lri_out[1] - uli_out[1] , false );
            SetParameterInt( "sizex", lri_out[0] - uli_out[0] , false );
            }
        }
        
      }

    if( HasValue( "mode.fit.im" ) && GetParameterString( "mode" ) == "fit" )
      {
      // Setup the DEM Handler
      otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"elev");

      FloatVectorImageType::Pointer referencePtr = this->GetParameterImage("mode.fit.im");
      referencePtr->UpdateOutputInformation();

      RSTransformType::Pointer rsTransform = RSTransformType::New();
      rsTransform->SetInputKeywordList(referencePtr->GetImageKeywordlist());
      rsTransform->SetInputProjectionRef(referencePtr->GetProjectionRef());
      rsTransform->SetOutputKeywordList(inImage->GetImageKeywordlist());
      rsTransform->SetOutputProjectionRef(inImage->GetProjectionRef());
      rsTransform->InstantiateTransform();

      FloatVectorImageType::IndexType uli_ref,uri_ref,lli_ref,lri_ref;

      uli_ref = referencePtr->GetLargestPossibleRegion().GetIndex();
      uri_ref = uli_ref;
      uri_ref[0]+=referencePtr->GetLargestPossibleRegion().GetSize()[0];
      lli_ref = uli_ref;
      lli_ref[1]+=referencePtr->GetLargestPossibleRegion().GetSize()[1];
      lri_ref = lli_ref;
      lri_ref[0]+=referencePtr->GetLargestPossibleRegion().GetSize()[0];

      FloatVectorImageType::PointType ulp_ref,urp_ref,llp_ref,lrp_ref;

      referencePtr->TransformIndexToPhysicalPoint(uli_ref,ulp_ref);
      referencePtr->TransformIndexToPhysicalPoint(uri_ref,urp_ref);
      referencePtr->TransformIndexToPhysicalPoint(lli_ref,llp_ref);
      referencePtr->TransformIndexToPhysicalPoint(lri_ref,lrp_ref);

      FloatVectorImageType::PointType ulp_out, urp_out, llp_out,lrp_out;

      ulp_out = rsTransform->TransformPoint(ulp_ref);
      urp_out = rsTransform->TransformPoint(urp_ref);
      llp_out = rsTransform->TransformPoint(llp_ref);
      lrp_out = rsTransform->TransformPoint(lrp_ref);

      FloatVectorImageType::IndexType uli_out, uri_out, lli_out, lri_out;

      inImage->TransformPhysicalPointToIndex(ulp_out,uli_out);
      inImage->TransformPhysicalPointToIndex(urp_out,uri_out);
      inImage->TransformPhysicalPointToIndex(llp_out,lli_out);
      inImage->TransformPhysicalPointToIndex(lrp_out,lri_out);

      FloatVectorImageType::IndexType uli, lri;

      uli[0] = std::min(std::min(uli_out[0],uri_out[0]),std::min(lli_out[0],lri_out[0]));
      uli[1] = std::min(std::min(uli_out[1],uri_out[1]),std::min(lli_out[1],lri_out[1]));

      lri[0] = std::max(std::max(uli_out[0],uri_out[0]),std::max(lli_out[0],lri_out[0]));
      lri[1] = std::max(std::max(uli_out[1],uri_out[1]),std::max(lli_out[1],lri_out[1]));

      SetParameterInt("startx",uli[0]);
      SetParameterInt("starty",uli[1]);
      SetParameterInt("sizex",lri[0]-uli[0]);
      SetParameterInt("sizey",lri[1]-uli[1]);

      this->CropRegionOfInterest();
      }


    m_ExtractROIFilter = ExtractROIFilterType::New();
    m_ExtractROIFilter->SetInput(inImage);
    m_ExtractROIFilter->SetStartX(GetParameterInt("startx"));
    m_ExtractROIFilter->SetStartY(GetParameterInt("starty"));
    m_ExtractROIFilter->SetSizeX(GetParameterInt("sizex"));
    m_ExtractROIFilter->SetSizeY(GetParameterInt("sizey"));

    for (unsigned int idx = 0; idx < GetSelectedItems("cl").size(); ++idx)
      {
      m_ExtractROIFilter->SetChannel(GetSelectedItems("cl")[idx] + 1 );
      }

    SetParameterOutputImage("out", m_ExtractROIFilter->GetOutput());
  }

  ExtractROIFilterType::Pointer   m_ExtractROIFilter;

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ExtractROI)
