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

#ifndef otb_ImageSettings_h
#define otb_ImageSettings_h


#include "itkObject.h"
#include "itkObjectFactory.h"


namespace otb
{


class ImageSettings : public itk::Object
{
public:
  typedef ImageSettings Self;
  typedef itk::Object Superclass;
  typedef itk::SmartPointer< Self > Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  
  itkSetMacro( MinRed, double );
  itkSetMacro( MinGreen, double );
  itkSetMacro( MinBlue, double );

  itkGetMacro( MinRed, double );
  itkGetMacro( MinGreen, double );
  itkGetMacro( MinBlue, double );

  itkSetMacro( MaxRed, double );
  itkSetMacro( MaxGreen, double );
  itkSetMacro( MaxBlue, double);

  itkGetMacro( MaxRed, double);
  itkGetMacro( MaxGreen, double);
  itkGetMacro( MaxBlue, double);  

  itkSetMacro( UseNoData, bool );
  itkGetMacro( UseNoData, bool );

  itkSetMacro( NoData, double );
  itkGetMacro( NoData, double );

  itkSetMacro( CurrentRed, double );
  itkSetMacro( CurrentGreen, double );
  itkSetMacro( CurrentBlue, double );

  itkGetMacro( CurrentRed, double );
  itkGetMacro( CurrentGreen, double );
  itkGetMacro( CurrentBlue, double );

  itkSetMacro( Gamma, double );
  itkGetMacro( Gamma, double );

  itkSetMacro( Alpha, double );
  itkGetMacro( Alpha, double );

  itkNewMacro( Self );


protected:
  ImageSettings();

  ~ImageSettings() override;


private:
  // prevent implementation
  ImageSettings( const Self & );
  void operator = ( const Self & );

  double m_MinRed;
  double m_MaxRed;

  double m_MinGreen;
  double m_MaxGreen;

  double m_MinBlue;
  double m_MaxBlue;

  bool   m_UseNoData;
  double m_NoData;

  double m_Gamma;

  double m_Alpha;

  double m_CurrentRed;
  double m_CurrentGreen;
  double m_CurrentBlue;
}; // End class ImageSettings

} // End namespace otb

#endif
