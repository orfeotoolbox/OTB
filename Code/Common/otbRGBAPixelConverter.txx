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

#ifndef __otbRGBAPixelConverter_txx
#define __otbRGBAPixelConverter_txx


namespace otb
{

//Base
template <class TInternalInputPixelType, class TOutputPixelType>
typename RGBAPixelConverter<TInternalInputPixelType,
                            TOutputPixelType>
::OutputPixelType
RGBAPixelConverter<TInternalInputPixelType,
                            TOutputPixelType>
::Convert(InputPixelType input)
{
  double result;

  result = (
    ( 2125.0 * static_cast<double>(input[0]) +
      7154.0 * static_cast<double>(input[1]) +
      0721.0 * static_cast<double>(input[2])) / 10000.0); //alpha not supported yet

  return (static_cast<OutputPixelType>(result));
}

//RGBA to RGBA
template <class TInternalInputPixelType, class TInternalOutputPixelType>
typename RGBAPixelConverter<TInternalInputPixelType,
                            typename itk::RGBAPixel<TInternalOutputPixelType> >
::OutputPixelType
RGBAPixelConverter<TInternalInputPixelType,
                   typename itk::RGBAPixel<TInternalOutputPixelType> >
::Convert(InputPixelType input)
{
  OutputPixelType result;

  result[0] = static_cast<TInternalOutputPixelType>(input[0]);
  result[1] = static_cast<TInternalOutputPixelType>(input[1]);
  result[2] = static_cast<TInternalOutputPixelType>(input[2]);
  result[3] = static_cast<TInternalOutputPixelType>(input[3]);

  return result;
}

//RGBA to RGB
template <class TInternalInputPixelType, class TInternalOutputPixelType>
typename RGBAPixelConverter<TInternalInputPixelType,
                           typename itk::RGBPixel<TInternalOutputPixelType> >
::OutputPixelType
RGBAPixelConverter<TInternalInputPixelType,
                   typename itk::RGBPixel<TInternalOutputPixelType> >
::Convert(InputPixelType input)
{
  OutputPixelType result;

  result[0] = static_cast<TInternalOutputPixelType>(input[0]);
  result[1] = static_cast<TInternalOutputPixelType>(input[1]);
  result[2] = static_cast<TInternalOutputPixelType>(input[2]);

  return result;
}


} // end namespace otb


#endif
