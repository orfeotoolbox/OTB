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

  virtual ~ImageSettings();


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
