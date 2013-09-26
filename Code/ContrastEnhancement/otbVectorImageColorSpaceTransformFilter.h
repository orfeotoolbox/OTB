/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbVectorImageColorSpaceTransformFilter_h
#define __otbVectorImageColorSpaceTransformFilter_h


#include "itkUnaryFunctorImageFilter.h"
#include "itkIndex.h"



namespace otb
{

namespace Functor {

template<class TInput, class TOutput>
class RGBToYUVFunctor {

public:

  typedef itk::Index<3> IndexType;

  RGBToYUVFunctor()
  :m_rChannel(0),
  m_gChannel(1),
   m_bChannel(2)
  {

  }
  ~RGBToYUVFunctor() { }

  void SetRGBIndex(IndexType index)
  {
    m_rChannel = index[0];
    m_gChannel = index[1];
    m_bChannel = index[2];

  }
inline void SetDelta(unsigned int delta)
{
m_Delta = delta;
}



  inline TOutput operator ()(const TInput& input) const
  {

     TOutput output;
        output.SetSize(3);

     typedef typename TInput::ValueType InputType;
     typedef typename TOutput::ValueType OutputType;
//     int m_rChannel = 0;
//     int m_gChannel = 1;
//     int m_bChannel = 2;

     InputType R = input[m_rChannel] ;
     InputType G = input[m_gChannel] ;
     InputType B = input[m_bChannel] ;

    double Y  =  0.299 * R + 0.587 * G + 0.114  * B;
    double Cb = -0.169 * R - 0.331 * G + 0.499  * B + m_Delta;
    double Cr =  0.499 * R - 0.418 * G - 0.0813 * B + m_Delta;

    output[0] = static_cast<OutputType>(Y); // Y
      output[1] = static_cast<OutputType>(Cb); // Cb
    output[2] = static_cast<OutputType>(Cr); // Cr

    return output;

  }


private:
  int m_rChannel;
  int m_gChannel;
  int m_bChannel;
  unsigned int m_Delta;

};


template<class TInput, class TOutput>
class YUVToRGBFunctor {

public:

  typedef itk::Index<3> IndexType;
     typedef typename TInput::ValueType InputType;
     typedef typename TOutput::ValueType OutputType;

  YUVToRGBFunctor()
  :m_rChannel(0),
  m_gChannel(1),
  m_bChannel(2)
  {

  }


  ~YUVToRGBFunctor() { }


  void SetRGBIndex(IndexType index)
  {
    m_rChannel = index[0];
    m_gChannel = index[1];
    m_bChannel = index[2];

  }


inline void SetDelta(unsigned int delta)
{

m_Delta = delta;
}

inline void SetMaxPixelValue(OutputType maxValue)
{

m_MaxPixelValue = maxValue;
}

  inline TOutput operator ()(const TInput& input)
  {

     TOutput output;
        output.SetSize(3);

     double Y =  input[0] ;
     double Cb = input[1] - m_Delta;
     double Cr = input[2] - m_Delta;

     double R = Y + 1.402 * Cr;
     double G = Y - 0.344 * Cb - 0.714 *Cr;
     double B = Y + 1.772 * Cb;


     output[0] = ClipToMinMax(R);
     output[1] = ClipToMinMax(G);
     output[2] = ClipToMinMax(B);


    return output;
  }

  OutputType ClipToMinMax(double value)
  {

    if(value > m_MaxPixelValue)
      return m_MaxPixelValue;
    else if(value < 0)
      return 0;
    else
      return (OutputType)value;
  
  }




private:
  int m_rChannel;
  int m_gChannel;
  int m_bChannel;
  unsigned int m_Delta;
  OutputType m_MaxPixelValue;

};


}

template<class TInput, class TOutput, class TFunctor >
class ITK_EXPORT VectorImageColorSpaceTransformFilter :
    public itk::UnaryFunctorImageFilter <TInput, TOutput,TFunctor >
{
public:

  typedef VectorImageColorSpaceTransformFilter          Self;
  typedef itk::UnaryFunctorImageFilter<TInput, TOutput, TFunctor>   Superclass;
  typedef itk::SmartPointer<Self>                    Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;

  typedef typename TInput::InternalPixelType   InternalPixelType;
  typedef itk::Index<3> IndexType;

  itkNewMacro(Self);


  itkTypeMacro(Self, Superclass);

  typedef typename TInput::RegionType          RegionType;
  typedef typename TInput::Pointer            InputImagePointer;
  typedef typename TInput::PixelType           PixelType;
  



  void SetRGBIndex(const IndexType index)
  {

    this->GetFunctor().SetRGBIndex(index);
    this->Modified();


  }

  void SetMaxPixelValue(unsigned int value)
  {

    this->GetFunctor().SetMaxPixelValue(value);
    this->Modified();

  }

  void SetDelta(unsigned int delta)
  {
    this->GetFunctor().SetDelta(delta);
    this->Modified();

  }

  virtual void GenerateOutputInformation();
  virtual void BeforeThreadedGenerateData();



protected:

  VectorImageColorSpaceTransformFilter()
    {
    this->SetNumberOfRequiredInputs( 1 );
    }
  virtual ~VectorImageColorSpaceTransformFilter()
  {

  }

  void PrintSelf(std::ostream& os, itk::Indent indent) const { }


private:

//  IndexType m_RGBIndex;

  VectorImageColorSpaceTransformFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorImageColorSpaceTransformFilter.txx"
#endif

#endif
