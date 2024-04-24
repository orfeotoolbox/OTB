/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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


#include "otbFormosatImageMetadataInterface.h"

#include "otbMacro.h"
#include "otbMath.h"
#include "itkMetaDataObject.h"
#include "otbStringUtils.h"

#include "otbDimapMetadataHelper.h"

#include "otbXMLMetadataSupplier.h"

namespace otb
{

unsigned int FormosatImageMetadataInterface::BandIndexToWavelengthPosition(unsigned int i) const
{
  otbMsgDevMacro(<< "Formosat2 detected: band 0 and 2 inverted");
  if (i == 0)
    return 2;
  if (i == 2)
    return 0;
  return i;
}

void FormosatImageMetadataInterface::FetchSatAngles(
                    const std::vector<double> & incidenceAngles,
                    const std::vector<double> & alongTrackViewingAngles,
                    const std::vector<double> & acrossTrackViewingAngles,
                    const std::vector<double> & sceneOrientation,
                    const std::string & softwareVersion,
                    double satAzimuth,
                    ImageMetadata& imd)
{
  if(incidenceAngles.size() != 1)
  {
    otbGenericExceptionMacro(MissingMetadataException,<<"Missing satellite angles in Dimap")
  }

  imd.Add(MDNum::SatElevation, 90. - incidenceAngles[0]);

  // In some software version, a bug exists.
  // We have to check the version to correct the satellite azimuth angle contained in the metadata
  if ((softwareVersion == "R2P_02.03.P1") 
      || (softwareVersion == "R2P_02.02") 
      || (softwareVersion == "R2P_02.03"))
  {
    if(sceneOrientation.size() != 1 
       || alongTrackViewingAngles.size() != 1 
       || acrossTrackViewingAngles.size() != 1)
    {
      otbGenericExceptionMacro(MissingMetadataException,<<"Missing satellite angles in Dimap")
    }

    double alpha = std::atan(std::tan(acrossTrackViewingAngles[0] * CONST_PI_180) 
                      / std::tan(alongTrackViewingAngles[0] * CONST_PI_180)) * CONST_180_PI;

    if (alongTrackViewingAngles[0] < 0)
    {
      if (alpha > 0)
      {
        alpha = alpha - 180;
      }
      else
      {
        alpha = alpha + 180;
      }
    }

    alpha -= sceneOrientation[0];
    if (alpha > 0)
    {
      satAzimuth += 180;
    }
    else
    {
      satAzimuth = 180 - satAzimuth;
    }
  }
  
  imd.Add(MDNum::SatAzimuth, satAzimuth);
}

void FormosatImageMetadataInterface::FetchSpectralSensitivity(ImageMetadata& imd)
{
  std::unordered_map<std::string, std::vector<double>> BandNameToSpectralSensitivityTable = {
    {"P", {
      0.0f,         0.0f,         0.0f,         0.0f,        0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
      0.0f,         0.0f,         0.0f,         0.0f,        0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
      0.0f,         0.0f,         0.0f,         0.0f,        0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
      0.0f,         0.0f,         0.0f,         0.0f,        0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
      0.0f,         0.0f,         0.0f,         0.0f,        0.0f,         0.0f,         0.000219376f, 0.000219425f, 0.000231534f, 0.000284519f, 0.000550079f,
      0.001f,       0.003f,       0.006f,       0.012f,      0.0235f,      0.035f,       0.043f,       0.0503893f,   0.0525f,      0.055f,       0.0565f,
      0.058f,       0.059f,       0.059f,       0.0608594f,  0.061f,       0.0615f,      0.062f,       0.063f,       0.063f,       0.064f,       0.063f,
      0.064f,       0.065f,       0.066f,       0.066f,      0.066f,       0.066f,       0.0675f,      0.069f,       0.0708104f,   0.0709615f,   0.0709001f,
      0.071f,       0.0715f,      0.073f,       0.074f,      0.074f,       0.073f,       0.073f,       0.0735f,      0.075f,       0.0765f,      0.078f,
      0.078f,       0.078f,       0.0785f,      0.0800145f,  0.0815f,      0.083f,       0.083f,       0.083f,       0.083f,       0.083f,       0.0835f,
      0.085f,       0.0865f,      0.087f,       0.088f,      0.087f,       0.087f,       0.088f,       0.0885f,      0.0907023f,   0.0915f,      0.092f,
      0.092f,       0.092f,       0.092f,       0.093f,      0.094f,       0.095f,       0.096f,       0.096f,       0.096f,       0.095f,       0.095f,
      0.095f,       0.095f,       0.096f,       0.0975f,     0.098f,       0.099f,       0.099f,       0.098f,       0.098f,       0.0965f,      0.096f,
      0.0965f,      0.098f,       0.0985f,      0.099f,      0.0995f,      0.099f,       0.0975f,      0.097f,       0.0955f,      0.095f,       0.095f,
      0.095f,       0.0965f,      0.097f,       0.098f,      0.098f,       0.0975f,      0.097f,       0.096f,       0.095f,       0.095f,       0.094f,
      0.094f,       0.095f,       0.0955f,      0.096f,      0.097f,       0.097f,       0.097f,       0.097f,       0.097f,       0.097f,       0.096f,
      0.097f,       0.097f,       0.097f,       0.098f,      0.098f,       0.096f,       0.095f,       0.094f,       0.093f,       0.092f,       0.091f,
      0.0905658f,   0.089f,       0.089f,       0.089f,      0.088f,       0.088f,       0.0875f,      0.087f,       0.086f,       0.085f,       0.084f,
      0.083f,       0.0825f,      0.082f,       0.081f,      0.081f,       0.0804051f,   0.079f,       0.078f,       0.077f,       0.077f,       0.076f,
      0.075f,       0.074f,       0.0735f,      0.073f,      0.0725f,      0.072f,       0.071f,       0.071f,       0.0704136f,   0.069f,       0.0685f,
      0.068f,       0.067f,       0.066f,       0.0645f,     0.063f,       0.062f,       0.0607892f,   0.059f,       0.058f,       0.057f,       0.056f,
      0.0545f,      0.052f,       0.0455f,      0.038f,      0.0286994f,   0.0202138f,   0.0125f,      0.007f,       0.004f,       0.002f,       0.001f,
      0.000909856f, 0.000512159f, 0.000357051f, 0.00029112f, 0.000215752f, 0.000187213f, 0.000171918f, 0.000169724f, 0.000166392f, 0.000163058f, 0.000159726f,
      0.000156393f, 0.000153061f, 0.000149728f, 0.0f,        0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
      0.0f,         0.0f,         0.0f,         0.0f,        0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
      0.0f,         0.0f,         0.0f,         0.0f,        0.0f,         0.0f}},
    {"B1", {
      0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,        0.0f,
      0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,        0.0f,
      0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,        0.0f,
      0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.000354959f, 0.000354603f, 0.000376745f, 0.000478212f, 0.0009718f,  0.002f,
      0.005f, 0.0124297f, 0.024f, 0.0415f, 0.057f, 0.0685f, 0.074f, 0.074f,  0.076f,       0.0795695f,   0.082f,       0.084f,       0.083f,      0.0825f,
      0.086f, 0.0910953f, 0.094f, 0.096f,  0.096f, 0.094f,  0.094f, 0.096f,  0.099f,       0.0995f,      0.099f,       0.098f,       0.095f,      0.092f,
      0.095f, 0.0985f,    0.092f, 0.0695f, 0.037f, 0.025f,  0.009f, 0.0025f, 0.001f,       0.000847053f, 0.000588401f, 0.00051966f,  0.00050602f, 0.0f,
      0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,        0.0f,
      0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,        0.0f,
      0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,        0.0f,
      0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,        0.0f,
      0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,        0.0f,
      0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,        0.0f,
      0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,        0.0f,
      0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,        0.0f,
      0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,        0.0f,
      0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,        0.0f,
      0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,        0.0f,
      0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,        0.0f,
      0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,        0.0f,
      0}},
    {"B2", {
      0.0f,         0.0f,    0.0f,   0.0f,       0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
      0.0f,         0.0f,    0.0f,   0.0f,       0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
      0.0f,         0.0f,    0.0f,   0.0f,       0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
      0.0f,         0.0f,    0.0f,   0.0f,       0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
      0.0f,         0.0f,    0.0f,   0.0f,       0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
      0.0f,         0.0f,    0.0f,   0.0f,       0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
      0.0f,         0.0f,    0.0f,   0.0f,       0.0f,   0.0f,    0.0f,         0.0f,         0.000324677f, 0.000326475f, 0.000331943f, 0.000386303f,
      0.000447683f, 0.001f,  0.003f, 0.0045f,    0.012f, 0.0205f, 0.036f,       0.0615f,      0.075f,       0.081f,       0.082f,       0.0845f,
      0.087f,       0.0885f, 0.088f, 0.087f,     0.086f, 0.0875f, 0.0908484f,   0.0925f,      0.094f,       0.094f,       0.093f,       0.0925f,
      0.093f,       0.0955f, 0.097f, 0.098f,     0.099f, 0.099f,  0.099f,       0.099f,       0.099f,       0.099f,       0.095f,       0.0815f,
      0.057f,       0.032f,  0.018f, 0.0112534f, 0.005f, 0.0015f, 0.000758484f, 0.000604297f, 0.000512471f, 0.000475316f, 0.000453283f, 0.00044559f,
      0.0f,         0.0f,    0.0f,   0.0f,       0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
      0.0f,         0.0f,    0.0f,   0.0f,       0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
      0.0f,         0.0f,    0.0f,   0.0f,       0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
      0.0f,         0.0f,    0.0f,   0.0f,       0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
      0.0f,         0.0f,    0.0f,   0.0f,       0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
      0.0f,         0.0f,    0.0f,   0.0f,       0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
      0.0f,         0.0f,    0.0f,   0.0f,       0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
      0.0f,         0.0f,    0.0f,   0.0f,       0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
      0.0f,         0.0f,    0.0f,   0.0f,       0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
      0.0f,         0.0f,    0.0f,   0.0f,       0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
      0.0f,         0.0f,    0.0f,   0.0f,       0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
      0.0f,         0.0f,    0.0f,   0.0f,       0.0f,   0.0f,    0.0f,         0.0f,         0.0f,         0.0f,         0.0f,         0.0f,
      0.0f,         0.0f,    0.0f,   0.0f,       0}},
    {"B3", {
      0.0f,   0.0f,    0.0f,   0.0f,       0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,
      0.0f,   0.0f,    0.0f,   0.0f,       0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,
      0.0f,   0.0f,    0.0f,   0.0f,       0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,
      0.0f,   0.0f,    0.0f,   0.0f,       0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,
      0.0f,   0.0f,    0.0f,   0.0f,       0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,
      0.0f,   0.0f,    0.0f,   0.0f,       0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,
      0.0f,   0.0f,    0.0f,   0.0f,       0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,
      0.0f,   0.0f,    0.0f,   0.0f,       0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,
      0.0f,   0.0f,    0.0f,   0.0f,       0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.000461752f,
      0.001f, 0.001f,  0.003f, 0.006f,     0.013f, 0.0335f,    0.063f, 0.089f,  0.098f, 0.099f,  0.099f, 0.099f,  0.099f,       0.0985f,
      0.097f, 0.0945f, 0.092f, 0.0906796f, 0.089f, 0.0907659f, 0.093f, 0.0965f, 0.1f,   0.097f,  0.091f, 0.0865f, 0.086f,       0.0910438f,
      0.094f, 0.092f,  0.093f, 0.088f,     0.064f, 0.034f,     0.015f, 0.0075f, 0.006f, 0.0045f, 0.003f, 0.001f,  0.000607601f, 0.000202927f,
      0.0f,   0.0f,    0.0f,   0.0f,       0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,
      0.0f,   0.0f,    0.0f,   0.0f,       0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,
      0.0f,   0.0f,    0.0f,   0.0f,       0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,
      0.0f,   0.0f,    0.0f,   0.0f,       0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,
      0.0f,   0.0f,    0.0f,   0.0f,       0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,
      0.0f,   0.0f,    0.0f,   0.0f,       0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,
      0.0f,   0.0f,    0.0f,   0.0f,       0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,
      0.0f,   0.0f,    0.0f,   0.0f,       0.0f,   0.0f,       0.0f,   0.0f,    0.0f,   0.0f,    0.0f,   0.0f,    0.0f,         0.0f,
      0}},
    {"B4", {
      0.0f,         0.0f,         0.0f,          0.0f,         0.0f,         0.0f,         0.0f,         0.0f, 
      0.0f,         0.0f,         0.0f,          0.0f,         0.0f,         0.0f,         0.0f,         0.0f, 
      0.0f,         0.0f,         0.0f,          0.0f,         0.0f,         0.0f,         0.0f,         0.0f, 
      0.0f,         0.0f,         0.0f,          0.0f,         0.0f,         0.0f,         0.0f,         0.0f, 
      0.0f,         0.0f,         0.0f,          0.0f,         0.0f,         0.0f,         0.0f,         0.0f, 
      0.0f,         0.0f,         0.0f,          0.0f,         0.0f,         0.0f,         0.0f,         0.0f, 
      0.0f,         0.0f,         0.0f,          0.0f,         0.0f,         0.0f,         0.0f,         0.0f, 
      0.0f,         0.0f,         0.0f,          0.0f,         0.0f,         0.0f,         0.0f,         0.0f, 
      0.0f,         0.0f,         0.0f,          0.0f,         0.0f,         0.0f,         0.0f,         0.0f, 
      0.0f,         0.0f,         0.0f,          0.0f,         0.0f,         0.0f,         0.0f,         0.0f, 
      0.0f,         0.0f,         0.0f,          0.0f,         0.0f,         0.0f,         0.0f,         0.0f, 
      0.0f,         0.0f,         0.0f,          0.0f,         0.0f,         0.0f,         0.0f,         0.0f, 
      0.0f,         0.0f,         0.0f,          0.0f,         0.0f,         0.0f,         0.0f,         0.0f, 
      0.0f,         0.0f,         0.0f,          0.0f,         0.0f,         0.0f,         0.0f,         0.0f, 
      0.0f,         0.0f,         0.0f,          0.0f,         0.0f,         0.0f,         0.0f,         0.0f, 
      0.0f,         0.0f,         0.0f,          0.0f,         0.0f,         0.0f,         0.0f,         0.0f, 
      0.0f,         0.0f,         0.0f,          0.0f,         0.0f,         0.0f,         0.0f,         0.0f, 
      0.0f,         0.0f,         0.0f,          0.0f,         0.0f,         0.0f,         0.0f,         0.0f, 
      0.0f,         0.0f,         0.0f,          0.0f,         0.0f,         0.0f,         0.0f,         0.0f, 
      0.0f,         0.0f,         0.0f,          0.0f,         0.0f,         0.0f,         0.0f,         0.0f, 
      0.0f,         0.0f,         0.0f,          0.000481456f, 0.00071923f,  0.00098303f,  0.001f,       0.000947915f, 
      0.00090899f,  0.000887367f, 0.000576847f,  0.000470858f, 0.00052246f,  0.000713811f, 0.001f,       0.0015f, 
      0.003f,       0.005f,       0.008f,        0.0145f,      0.022f,       0.035f,       0.049f,       0.0595f, 
      0.075f,       0.0905f,      0.098f,        0.0995f,      0.097f,       0.093f,       0.091f,       0.0925f, 
      0.094f,       0.096f,       0.096f,        0.0955f,      0.094f,       0.092f,       0.0907811f,   0.089f, 
      0.088f,       0.088f,       0.088f,        0.088f,       0.088f,       0.0875f,      0.087f,       0.086f, 
      0.085f,       0.084f,       0.083f,        0.083f,       0.082f,       0.081f,       0.0806396f,   0.079f, 
      0.078f,       0.077f,       0.076f,        0.075f,       0.074f,       0.073f,       0.072f,       0.071f, 
      0.0700369f,   0.0685f,      0.067f,        0.0655f,      0.064f,       0.063f,       0.063f,       0.062f, 
      0.059f,       0.054f,       0.043f,        0.034f,       0.025f,       0.016f,       0.009f,       0.0055f, 
      0.003f,       0.0015f,      0.001f,        0.000691333f, 0.000432126f, 0.000356974f, 0.000265441f, 0.000219773f, 
      0.000195346f, 0.000192716f, 0.000188932f,  0.000185148f, 0.000181364f, 0.00017758f,  0.000173796f, 0.000170011f, 
      0.000166227f, 0.0f,         0.0f,          0.0f,         0.0f,         0.0f,         0.0f,         0.0f, 
      0.0f,         0.0f,         0.0f,          0.0f,         0.0f,         0.0f,         0.0f,         0.0f, 
      0.0f,         0.0f,         0.0f,          0.0f,         0.0f,         0.0f,         0.0f,         0.0f, 
      0}}};

  otb::MetaData::LUT1D spectralSensitivity;

  spectralSensitivity.Axis[0].Origin = 0.3;
  spectralSensitivity.Axis[0].Spacing = 0.0025;
  spectralSensitivity.Axis[0].Size = 281;


  for (auto & band: imd.Bands)
  {
    auto SpectralSensitivityIt = BandNameToSpectralSensitivityTable.find(band[MDStr::BandName] );
    if (SpectralSensitivityIt != BandNameToSpectralSensitivityTable.end())
    {
      spectralSensitivity.Array = SpectralSensitivityIt->second;

      band.Add(MDL1D::SpectralSensitivity, spectralSensitivity);
    }
  }
  
}

void FormosatImageMetadataInterface::Parse(ImageMetadata &imd)
{
  DimapMetadataHelper helper;

  // GDAL DIMAP metadata case
  if (m_MetadataSupplierInterface->GetAs<std::string>("", "IMAGERY/SATELLITEID") == "FORMOSAT 2")
  {
    //Find the METADATA.DIM file (Dimap V1 metadata file)
    auto resourceFiles = m_MetadataSupplierInterface->GetResourceFiles();
    
    std::string metadataFile;
    for (const auto & file: resourceFiles)
    {
      if (file.find("METADATA.DIM")!=std::string::npos)
      {
        metadataFile = file;
        break;
      }
    }

    if (metadataFile.empty())
    {
      otbGenericExceptionMacro(MissingMetadataException,<<"Cannot find the METADATA.DIM file")
    }

    XMLMetadataSupplier xmlMds(metadataFile);
    helper.ParseDimapV1(xmlMds, "Dimap_Document.");
    
  }
  // geom metadata case
  else if (m_MetadataSupplierInterface->GetAs<std::string>("", "support_data.sensorID" ) == "Formosat 2")
  {
    helper.ParseGeom(*m_MetadataSupplierInterface);
  }

  else
  {
    otbGenericExceptionMacro(MissingMetadataException,<<"Not a Formosat product")
  }

  imd.Add(MDStr::Mission, "Formasat 2");
  imd.Add(MDStr::SensorID, "FORMOSAT 2");


  const auto & dimapData = helper.GetDimapData();
  auto nbBands = imd.Bands.size();

  // Band names are not in the metadata. Two cases should be considered 
  // here: panchromatic and multispectral
  if (nbBands == 1)
  {
    imd.Bands[0].Add(MDStr::BandName, "P");
  }
  else if (nbBands == 4)
  {
    imd.Bands[0].Add(MDStr::BandName, "B1");
    imd.Bands[1].Add(MDStr::BandName, "B2");
    imd.Bands[2].Add(MDStr::BandName, "B3");
    imd.Bands[3].Add(MDStr::BandName, "B4");
  }
  else
  {
    otbGenericExceptionMacro(MissingMetadataException,
            "Invalid number of band for a FORMOSAT product")
  }

  imd.Add(MDTime::ProductionDate,
          MetaData::ReadFormattedDate(dimapData.ProductionDate));

  imd.Add(MDTime::AcquisitionDate,
          MetaData::ReadFormattedDate(dimapData.AcquisitionDate));

  imd.Add(MDStr::Instrument, dimapData.Instrument);
  imd.Add(MDStr::InstrumentIndex, dimapData.InstrumentIndex);

  imd.Add(MDNum::SunAzimuth, dimapData.SunAzimuth[0]);
  imd.Add(MDNum::SunElevation, dimapData.SunElevation[0]);

  FetchSatAngles(dimapData.IncidenceAngle, dimapData.AlongTrackViewingAngle,
                 dimapData.AcrossTrackViewingAngle, dimapData.SceneOrientation,
                 dimapData.softwareVersion, dimapData.SatAzimuth,
                 imd);

  std::vector<double> solarIrradianceVec;
  if (dimapData.SolarIrradiance.empty())
  {
    solarIrradianceVec = std::vector<double>(nbBands, 0.);
  }
  else
  {
    solarIrradianceVec = dimapData.SolarIrradiance;
  }

  auto setDimapBandMetadata = [&imd](const MDNum & key, const std::vector<double> & input)
  {
    if (input.size() == 1)
    {
      // this is a PAN image
      imd.Bands[0].Add(key, input[0]);
    }
    else if (input.size() == 4)
    {
      // MS image, note the band ordering
      imd.Bands[0].Add(key, input[2]);
      imd.Bands[1].Add(key, input[1]);
      imd.Bands[2].Add(key, input[0]);
      imd.Bands[3].Add(key, input[3]);
    }
    else
    {
      otbGenericExceptionMacro(MissingMetadataException,
      << "The number of bands in image metadatas is incoherent with the DIMAP product")
    }
  };

  setDimapBandMetadata(MDNum::PhysicalBias, dimapData.PhysicalBias);
  setDimapBandMetadata(MDNum::PhysicalGain, dimapData.PhysicalGain);
  setDimapBandMetadata(MDNum::SolarIrradiance, solarIrradianceVec);

  FetchSpectralSensitivity(imd);

  // Default display
  imd.Add(MDNum::RedDisplayChannel, 0);
  imd.Add(MDNum::GreenDisplayChannel, 1);
  imd.Add(MDNum::BlueDisplayChannel, 2);
}


} // end namespace otb
