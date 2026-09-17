/*
 * Copyright (C) 2005-2026 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbApplicationWithNoInputImage_h
#define otbApplicationWithNoInputImage_h

#include "OTBApplicationEngineExport.h"
#include "otbWrapperMacros.h"
#include "otbWrapperApplication.h"
#include "otbRepack.h"

// MapProjection handler
#include "otbWrapperMapProjectionParametersHandler.h"
#include <type_traits>

namespace otb
{

enum
{
  Mode_UserDefined,
  Mode_OutputROI,
  Mode_OrthoFit
};

namespace Wrapper
{

/**
 * Helper parent class for applications that don't transform an input image, but work on a user
 * specified region.
 *
 * This decorator class provides the following services:
 * - `DoInit_NoInputImage()` meant to be called from `DoInit()` specialisation.
 * - `DoUpdateParameters_NoInputImage()` meant to be called from `DoExecute()` specialisation,
 * - `InstantiateImageGenerator()` meant to be called from `DoExecute()` to instantiate the actual
 *    image generator.
 *
 * It defines parameters that mimic `OrthoRectification` interface: it defines the same parameters
 * for specifying the output footprint.
 *
 * \tparam TApplication         Exact application type in case layers of decorator need to be
 *                              stacked (expected to inherit from `otb::Application`)
 *
 * \author Luc Hermitte
 * \copyright CS Group
 */
template <typename TApplication = Application>
class OTBApplicationEngine_EXPORT_TEMPLATE ApplicationWithoutInputImage : public TApplication
{
  static_assert(
      std::is_base_of<otb::Wrapper::Application, TApplication>::value,
      "Expected to inherit from otb::Wrapper::Application"
  );

  /**\name Application parameter keys */
  //@{
  static constexpr auto o_lrx      = "outputs.lrx";
  static constexpr auto o_lry      = "outputs.lry";
  static constexpr auto o_ulx      = "outputs.ulx";
  static constexpr auto o_uly      = "outputs.uly";
  static constexpr auto o_spacingx = "outputs.spacingx";
  static constexpr auto o_spacingy = "outputs.spacingy";
  static constexpr auto o_sizex    = "outputs.sizex";
  static constexpr auto o_sizey    = "outputs.sizey";
  static constexpr auto o_ortho    = "outputs.ortho";
  static constexpr auto o_mode     = "outputs.mode";
  //@}

protected:

  /**
   * Defines application parameters.
   * The parameter names follow the parameters from OrthoRectification.
   * \note Meant to be appled from `DoInit()` specialisation.
   */
  void DoInit_NoInputImage()
  {
    // Build the Output Map Projection
    MapProjectionParametersHandler::AddMapProjectionParameters(this, "map");

    // Add the output parameters in a group
    this->AddParameter(ParameterType_Group, "outputs", "Output Image Grid");
    this->SetParameterDescription(
        "outputs",
        "This group of parameters allows one to define the grid on which the input image will be "
        "resampled."
    );

    // UserDefined values
    this->AddParameter(ParameterType_Choice, o_mode, "Parameters estimation modes");
    this->AddChoice("outputs.mode.auto", "User Defined");
    this->SetParameterDescription(
        "outputs.mode.auto", "This mode allows you to fully modify default values."
    );
    this->AddChoice("outputs.mode.outputroi", "Automatic Size from Spacing and output corners");
    this->SetParameterDescription(
        "outputs.mode.outputroi",
        "This mode allows you to automatically compute the optimal image size from spacing (pixel "
        "size) and output corners"
    );
    this->AddChoice("outputs.mode.orthofit", "Fit to ortho");
    this->SetParameterDescription(
        "outputs.mode.orthofit",
        "Fit the size, origin and spacing to an existing ortho image (uses the value of "
        "outputs.ortho)"
    );

    // Upper left point coordinates
    this->AddParameter(ParameterType_Double, o_ulx, "Upper Left X");
    this->SetParameterDescription(
        o_ulx,
        "Cartographic X coordinate of upper-left corner (meters for cartographic projections, "
        "degrees for geographic ones)"
    );

    this->AddParameter(ParameterType_Double, o_uly, "Upper Left Y");
    this->SetParameterDescription(
        o_uly,
        "Cartographic Y coordinate of the upper-left corner (meters for cartographic projections, "
        "degrees for geographic ones)"
    );

    // Size of the output image
    this->AddParameter(ParameterType_Int, o_sizex, "Size X");
    this->SetParameterDescription(o_sizex, "Size of projected image along X (in pixels)");

    this->AddParameter(ParameterType_Int, o_sizey, "Size Y");
    this->SetParameterDescription(o_sizey, "Size of projected image along Y (in pixels)");

    // Spacing of the output image
    this->AddParameter(ParameterType_Double, o_spacingx, "Pixel Size X");
    this->SetParameterDescription(
        o_spacingx,
        "Size of each pixel along X axis (meters for cartographic projections, degrees for "
        "geographic ones)"
    );


    this->AddParameter(ParameterType_Double, o_spacingy, "Pixel Size Y");
    this->SetParameterDescription(
        o_spacingy,
        "Size of each pixel along Y axis (meters for cartographic projections, degrees for "
        "geographic ones)"
    );

    // Lower right point coordinates
    this->AddParameter(ParameterType_Double, o_lrx, "Lower right X");
    this->SetParameterDescription(
        o_lrx,
        "Cartographic X coordinate of the lower-right corner (meters for cartographic projections, "
        "degrees for geographic ones)"
    );

    this->AddParameter(ParameterType_Double, o_lry, "Lower right Y");
    this->SetParameterDescription(
        o_lry,
        "Cartographic Y coordinate of the lower-right corner (meters for cartographic projections, "
        "degrees for geographic ones)"
    );

    // Existing ortho image that can be used to compute size, origin and spacing of the output
    this->AddParameter(ParameterType_InputImage, o_ortho, "Model ortho-image");
    this->SetParameterDescription(
        o_ortho,
        "A model ortho-image that can be used to compute size, origin and spacing of the output"
    );

    // Setup parameters for initial UserDefined mode
    this->DisableParameter(o_lrx);
    this->DisableParameter(o_lry);
    this->DisableParameter(o_ortho);
    this->MandatoryOff(o_lrx);
    this->MandatoryOff(o_lry);
    this->MandatoryOff(o_ortho);
  }

  /**
   * Analyses the parameters passed, and update the other ones.
   * \warning Meant to be appled from `DoExecute()` specialisation.
   *          Indeed, we cannot use `GetParameterDouble("*")` to obtain the parameters passed in
   *          order to compute missing parameters: in `DoUpdateParameters()`, we can only fetch the
   *          default values.
   *          As such, we shall do nothing in `DoUpdateParameters()` specialisation, and we set the
   *          defaults in `DoUpdateParameters_NoInputImage()` called from `DoExecute().
   */
  void DoUpdateParameters_NoInputImage()
  {
    // Get the output projection Ref from map.* parameters
    m_OutputProjectionRef = MapProjectionParametersHandler::GetProjectionRefFromChoice(this, "map");

    auto enable_parameter = [&self = *this](std::string const& name) {
      self.EnableParameter(name);
      self.MandatoryOn(name);
    };

    auto disable_parameter = [&self = *this](std::string const& name) {
      self.DisableParameter(name);
      self.MandatoryOff(name);
    };

    // Handle the spacing and size field following the mode chosen by the user

    switch (this->GetParameterInt(o_mode))
    {
      case Mode_UserDefined:
        {
          otbAppLogINFO("mode is Mode_UserDefined");
          // Enable all the parameters except lower right coordinates
          // Make all the parameters in this mode mandatory except lower right coordinates
          enable_parameter(o_ulx);
          enable_parameter(o_uly);
          enable_parameter(o_spacingx);
          enable_parameter(o_spacingy);
          enable_parameter(o_sizex);
          enable_parameter(o_sizey);
          disable_parameter(o_lrx);
          disable_parameter(o_lry);
          disable_parameter(o_ortho);

          // Update lower right
          this->SetParameterDouble(
              o_lrx,
              GetParameterULX() + GetParameterSpacingX() * GetParameterSizeX());
          this->SetParameterDouble(
              o_lry,
              GetParameterULY() + GetParameterSpacingY() * GetParameterSizeY());
        }
        break;
     case Mode_OutputROI:
        {
          otbAppLogINFO("mode is Mode_OutputROI");
          // Set the size according to the output ROI
          this->EnableParameter(o_ulx);
          this->EnableParameter(o_uly);
          this->DisableParameter(o_sizex);
          this->DisableParameter(o_sizey);
          this->EnableParameter(o_spacingx);
          this->EnableParameter(o_spacingy);
          this->EnableParameter(o_lrx);
          this->EnableParameter(o_lry);
          this->DisableParameter(o_ortho);

          // Adapt the status of the param to this mode
          this->MandatoryOn(o_ulx);
          this->MandatoryOn(o_uly);
          this->MandatoryOn(o_spacingx);
          this->MandatoryOn(o_spacingy);
          this->MandatoryOff(o_sizex);
          this->MandatoryOff(o_sizey);
          this->MandatoryOn(o_lrx);
          this->MandatoryOn(o_lry);
          this->MandatoryOff(o_ortho);

          auto const spacingx = GetParameterSpacingX();
          auto const spacingy = GetParameterSpacingY();

          // Set the  processed size relative to this forced spacing
          if (std::abs(spacingx) > 0.0)
            this->SetParameterInt(o_sizex, static_cast<int>(std::ceil((GetParameterLRX() - GetParameterULX()) / spacingx)));
          if (std::abs(spacingy) > 0.0)
            this->SetParameterInt(o_sizey, static_cast<int>(std::ceil((GetParameterLRY() - GetParameterULY()) / spacingy)));
        }
        break;
      case Mode_OrthoFit:
        {
          otbAppLogINFO("mode is Mode_OrthoFit");
          // Make all the parameters in this mode mandatory
          this->MandatoryOff(o_ulx);
          this->MandatoryOff(o_uly);
          this->MandatoryOff(o_spacingx);
          this->MandatoryOff(o_spacingy);
          this->MandatoryOff(o_sizex);
          this->MandatoryOff(o_sizey);
          this->MandatoryOff(o_lrx);
          this->MandatoryOff(o_lry);
          this->MandatoryOn(o_ortho);

          // Disable the parameters
          this->DisableParameter(o_ulx);
          this->DisableParameter(o_uly);
          this->DisableParameter(o_spacingx);
          this->DisableParameter(o_spacingy);
          this->DisableParameter(o_sizex);
          this->DisableParameter(o_sizey);
          this->DisableParameter(o_lrx);
          this->DisableParameter(o_lry);
          this->EnableParameter(o_ortho);

          if (this->HasValue(o_ortho))
          {
            // input image
            FloatVectorImageType::Pointer inOrtho = this->GetParameterImage(o_ortho);

            auto const orig    = inOrtho->GetOrigin();
            auto const spacing = inOrtho->GetSignedSpacing();
            auto const size    = inOrtho->GetLargestPossibleRegion().GetSize();

            this->SetParameterInt(o_sizex, size[0]);
            this->SetParameterInt(o_sizey, size[1]);
            this->SetParameterDouble(o_spacingx, spacing[0]);
            this->SetParameterDouble(o_spacingy, spacing[1]);
            this->SetParameterDouble(o_ulx, orig[0] - 0.5 * spacing[0]);
            this->SetParameterDouble(o_uly, orig[1] - 0.5 * spacing[1]);
            // Update lower right
            this->SetParameterDouble(
                o_lrx,
                GetParameterULX() + GetParameterSpacingX() * GetParameterSizeX());
            this->SetParameterDouble(
                o_lry,
                GetParameterULY() + GetParameterSpacingY() * GetParameterSizeY());
          }
        }
        break;
    } // switch (GetParameterInt(o_mode) )
  }

  /**
   * Instantiates the image generator.
   * \tparam TImageGeneratorType  Type of the image generator. Expected to inherit from
   *                              `itk::ImageSource`, and to provide: `SetOutputSize()`,
   *                              `SetOutputSpacing()` and `SetOutputOrigin()` and
   *                              `SetOutputProjectionRef()`
   * \tparam BuildArgs            Auto detected types of parameters to forward
   * \param[in] args  Construction parameters forwarded to `TImageGeneratorType` constructor.
   *
   * \return A new instance of `TImageGeneratorType` fully configured regarding the image output
   * region, spatial reference...
   *
   * \sa `otb::ImageSourceGeometryDecorator`, `otb::ImageSourceProjectionReferenceDecorator`
   */
  template <typename TImageGeneratorType, typename... BuildArgs>
  typename TImageGeneratorType::Pointer InstanciateImageGenerator(BuildArgs&&... args)
  {
    // Check size
    itk::SizeValueType const out_sizex = GetParameterSizeX();
    itk::SizeValueType const out_sizey = GetParameterSizeY();
    if (out_sizex <= 0 || out_sizey <= 0)
    {
      otbAppLogCRITICAL("Wrong value: negative size: (" << out_sizex << " , " << out_sizey << ")");
    }

    // Check spacing sign
    if (GetParameterSpacingY() > 0.)
    {
      otbAppLogWARNING(<< "Wrong value for outputs.spacingy: Pixel size along Y axis should be negative, (outputs.spacingy="
                       << GetParameterSpacingY() << ")")
    }

    typename TImageGeneratorType::Pointer generator
      = TImageGeneratorType::New(std::forward<BuildArgs>(args)...);

    // Set Output information
    typename TImageGeneratorType::SizeType const size{out_sizex, out_sizey};
    generator->SetOutputSize(size);
    otbAppLogINFO("Generating output with size = " << size);

    auto const spacing = otb::repack<typename TImageGeneratorType::SpacingType>(
        GetParameterSpacingX(),
        GetParameterSpacingY()
    );
    generator->SetOutputSpacing(spacing);
    otbAppLogINFO("Generating output with pixel spacing = " << spacing);
    double const out_spacing_x = spacing[0];
    double const out_spacing_y = spacing[1];

    auto const origin = otb::repack<typename TImageGeneratorType::PointType>(
        GetParameterULX() + 0.5 * out_spacing_x,
        GetParameterULY() + 0.5 * out_spacing_y
    );
    generator->SetOutputOrigin(origin);
    otbAppLogINFO("Generating output with origin = " << origin);

    otbAppLogDEBUG("Generating output projection_ref = " << m_OutputProjectionRef);
    generator->SetOutputProjectionRef(m_OutputProjectionRef);

    return generator;
  }

  /** Projection Reference getter. */
  std::string const& GetOutputProjectionRef() const noexcept { return m_OutputProjectionRef; }

  double GetParameterULX() const { return this->GetParameterDouble(o_ulx); }
  double GetParameterULY() const { return this->GetParameterDouble(o_uly); }
  double GetParameterLRX() const { return this->GetParameterDouble(o_lrx); }
  double GetParameterLRY() const { return this->GetParameterDouble(o_lry); }

  double GetParameterSpacingX() const { return this->GetParameterDouble(o_spacingx); }
  double GetParameterSpacingY() const { return this->GetParameterDouble(o_spacingy); }

  int GetParameterSizeX() const { return this->GetParameterInt(o_sizex); }
  int GetParameterSizeY() const { return this->GetParameterInt(o_sizey); }

private:
  std::string m_OutputProjectionRef;
};

} // otb Wrapper
} // otb namespace

#endif // otbApplicationWithNoInputImage_h
