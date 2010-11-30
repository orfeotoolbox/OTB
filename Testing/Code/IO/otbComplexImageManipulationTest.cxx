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
#include <complex>

#include "otbImageFileReader.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbMacro.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageRegionIterator.h"

/***********
 * 1.
 * Read Image<double> as Image<complex>
 * out : real = in, imag = 0
 ***********/
int otbImageDoubleToImageComplex(int argc, char * argv[])
{
  typedef double                              RealType;
  typedef std::complex<RealType>              PixelType;
  typedef otb::Image<PixelType, 2>            CplxImageType;
  typedef otb::ImageFileReader<CplxImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  CplxImageType::IndexType id;
  CplxImageType::SizeType size;
  CplxImageType::RegionType region;

  id.Fill(0);
  size[0] = 10;
  size[1] = 1;

  region.SetSize( size );
  region.SetIndex( id ); 

  reader->GetOutput()->SetRequestedRegion(region);
  reader->Update();

  itk::ImageRegionIteratorWithIndex<CplxImageType> it( reader->GetOutput(), region );

  unsigned int count = 0;

  it.GoToBegin();
  while( it.IsAtEnd()==false )
    {
      count = it.GetIndex()[1]*reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1]+it.GetIndex()[0];

      if( (it.Get().real() != static_cast<RealType>(count)) || (it.Get().imag() != static_cast<RealType>(0)) ) 
	{
	  std::cout<<"Image double read as Image complex error : "<<it.Get()<<", waited for ("<<count<<", 0)"<<std::endl;

	  return EXIT_FAILURE;
	}

      ++it;
    }

  return EXIT_SUCCESS;
}

/***********
 * 2.
 * Read Image<complex> as Image<double>
 * out : norm(in)
 ***********/
int otbImageComplexToImageDouble(int argc, char * argv[])
{
  typedef double                                RealType;
  typedef otb::Image<RealType, 2>               ScalImageType;

  typedef otb::ImageFileReader<ScalImageType>       ReaderType;


  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  ScalImageType::IndexType id;
  ScalImageType::SizeType size;
  ScalImageType::RegionType region;

  id.Fill(0);
  size[0] = 10;
  size[1] = 2;

  region.SetSize( size );
  region.SetIndex( id ); 

  reader->GetOutput()->SetRequestedRegion(region);
  reader->Update();

  itk::ImageRegionIteratorWithIndex<ScalImageType> it( reader->GetOutput(), region );

  unsigned int count = 0;
  it.GoToBegin();
  while( it.IsAtEnd()==false )
    {
      count = 2*(it.GetIndex()[1]*reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1]+it.GetIndex()[0]);
   
      double norm = vcl_sqrt(static_cast<double>( (count*count) + (count+1)*(count+1)));

      if(it.Get() != static_cast<RealType>(norm))
	{
	  std::cout<<"Image complex read as Image double error : value (should be norm): "<<it.Get()<<", waited for "<<norm<<"."<<std::endl;
	  
	  return EXIT_FAILURE;
	}
      ++it;
    }
  
  
  return EXIT_SUCCESS;
}


/***********
 * 3.
 * Read Image<complex> as Image<complex>
 * out : in
 ***********/
int otbImageComplexToImageComplex(int argc, char * argv[])
{
  typedef double                                RealType;
  typedef std::complex<RealType>                PixelType;
  typedef otb::Image<PixelType, 2>              CplxImageType;
 
  typedef otb::ImageFileReader<CplxImageType>       ReaderType;


  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  CplxImageType::IndexType id;
  CplxImageType::SizeType size;
  CplxImageType::RegionType region;

  id.Fill(0);
  size[0] = 10;
  size[1] = 1;

  region.SetSize( size );
  region.SetIndex( id ); 

  reader->GetOutput()->SetRequestedRegion(region);
  reader->Update();

  itk::ImageRegionIteratorWithIndex<CplxImageType> it( reader->GetOutput(), region );

  unsigned int count = 0;
  it.GoToBegin();
  while( it.IsAtEnd()==false )
    {
     count = 2*(it.GetIndex()[1]*reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1]+it.GetIndex()[0]);

      if( (it.Get().real() != static_cast<RealType>(count)) || ( it.Get().imag() != static_cast<RealType>(count+1) ) ) 
	{
	  std::cout<<"Image complex read as Image complex error: "<<it.Get()<<", waited for ("<<count<<", "<<count+1<<")"<<std::endl;
	  
	  return EXIT_FAILURE;
	}
      

      ++it;
    }


  return EXIT_SUCCESS;

}

/***********
 * 4.
 * Read Image<complex> as VectorImage<double>
 * out : [0]=in.real(), [1]=in.imag()
 ***********/
int otbImageComplexToVectorImageDouble(int argc, char * argv[])
{
  typedef double                                RealType;
  typedef otb::VectorImage<RealType, 2>               ScalVectorImageType;
  typedef otb::ImageFileReader<ScalVectorImageType>       ReaderType;


  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  ScalVectorImageType::IndexType id;
  ScalVectorImageType::SizeType size;
  ScalVectorImageType::RegionType region;

  id.Fill(0);
  size[0] = 10;
  size[1] = 1;

  region.SetSize( size );
  region.SetIndex( id ); 

  reader->GetOutput()->SetRequestedRegion(region);
  reader->Update();

  itk::ImageRegionIteratorWithIndex<ScalVectorImageType> it( reader->GetOutput(), region );

  unsigned int count = 0;
  it.GoToBegin();
  while( it.IsAtEnd()==false )
    {
     count = 2*(it.GetIndex()[1]*reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1]+it.GetIndex()[0]);

     if( (it.Get()[0] != static_cast<RealType>(count)) || (it.Get()[1] != static_cast<RealType>(count+1)) )
	{
	  std::cout<<"Image complex read as Vector Image double error: "<<it.Get()<<", waited for ("<<count<<", "<<count+1<<")"<<std::endl;
	  
	  return EXIT_FAILURE;
	}

      //count += 2;
      ++it;
    }
  
  
  return EXIT_SUCCESS;
}

/***********
 * 5.
 * Read Image<complex> as VectorImage<complex>
 * out : [0]=in
 ***********/
int otbImageComplexToVectorImageComplex(int argc, char * argv[])
{
  typedef double                                RealType;
  typedef std::complex<RealType>                PixelType;
  typedef otb::VectorImage<PixelType, 2>               CmplxVectorImageType;
  typedef otb::ImageFileReader<CmplxVectorImageType>       ReaderType;


  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  CmplxVectorImageType::IndexType id;
  CmplxVectorImageType::SizeType size;
  CmplxVectorImageType::RegionType region;

  id.Fill(0);
  size[0] = 10;
  size[1] = 1;

  region.SetSize( size );
  region.SetIndex( id ); 

  reader->GetOutput()->SetRequestedRegion(region);
  reader->Update();

  itk::ImageRegionIteratorWithIndex<CmplxVectorImageType> it( reader->GetOutput(), region );

  unsigned int count = 0;
  it.GoToBegin();
  while( it.IsAtEnd()==false )
    {
      count = 2*(it.GetIndex()[1]*reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1]+it.GetIndex()[0]);

      if( (it.Get()[0].real() != static_cast<RealType>(count)) || ( it.Get()[0].imag() != static_cast<RealType>(count+1) ) ) 
	{
	  std::cout<<"Image complex read as Vector Image complex error: "<<it.Get()[0]<<", waited for ("<<count<<", "<<count+1<<")"<<std::endl;
	  
	  return EXIT_FAILURE;
	}
   
       ++it;
    }
  
  
  return EXIT_SUCCESS;
}

/***********
 * 6.
 * Read VectorImage<double> as Image<complex>
 * out : out.real=in[0], out.imag=in[1]
 ***********/
int otbVectorImageDoubleToImageComplex(int argc, char * argv[])
{
  typedef double                                RealType;
  typedef std::complex<RealType>                PixelType;
  typedef otb::Image<PixelType, 2>               CmplxImageType;
  typedef otb::ImageFileReader<CmplxImageType>       ReaderType;


  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  CmplxImageType::IndexType id;
  CmplxImageType::SizeType size;
  CmplxImageType::RegionType region;

  id.Fill(0);
  size[0] = 10;
  size[1] = 1;

  region.SetSize( size );
  region.SetIndex( id ); 

  reader->GetOutput()->SetRequestedRegion(region);
  reader->Update();

  itk::ImageRegionIteratorWithIndex<CmplxImageType> it( reader->GetOutput(), region );

  unsigned int count = 0;
  it.GoToBegin();
  while( it.IsAtEnd()==false )
    {
      count = 2*(it.GetIndex()[1]*reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1]+it.GetIndex()[0]);
  
      if(it.Get().real() != static_cast<RealType>(count) || (it.Get().imag() != static_cast<RealType>(count+1)) ) 
	{
	  std::cout<<"Vector Image double read as Image double error: "<<it.Get()<<", waited for ("<<count<<", "<<count+1<<")"<<std::endl;
	  
	  return EXIT_FAILURE;
	}

      ++it;
    }
  
  
  return EXIT_SUCCESS;
}

/***********
 * 7.
 * Read VectorImage<double> as VectorImage<complex>
 * out : (out[0].real=in, out[0].imag=1), (out[1].real=in, out[1].imag=0), ...
 ***********/
int otbVectorImageDoubleToVectorImageComplex(int argc, char * argv[])
{
  typedef double                                RealType;
  typedef std::complex<RealType>                PixelType;
  typedef otb::VectorImage<PixelType, 2>               CmplxVectorImageType;
  typedef otb::ImageFileReader<CmplxVectorImageType>       ReaderType;


  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  CmplxVectorImageType::IndexType id;
  CmplxVectorImageType::SizeType size;
  CmplxVectorImageType::RegionType region;

  id.Fill(0);
  size[0] = 10;
  size[1] = 1;

  region.SetSize( size );
  region.SetIndex( id ); 

  reader->GetOutput()->SetRequestedRegion(region);
  reader->Update();

  itk::ImageRegionIteratorWithIndex<CmplxVectorImageType> it( reader->GetOutput(), region );

  unsigned int count = 0;
  it.GoToBegin();
  while( it.IsAtEnd()==false )
    {
      count = 2*(it.GetIndex()[1]*reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1]+it.GetIndex()[0]);;

      if(it.Get()[0].real() != static_cast<RealType>(count) || (it.Get()[0].imag() != static_cast<RealType>(0)) ) 
	{
	  std::cout<<"Vector Image double read as Vector Image complex error: "<<it.Get()[0]<<", waited for ("<<count<<", 0)."<<std::endl;
	  
	  return EXIT_FAILURE;
	}

      ++it;
    }
  
  
  return EXIT_SUCCESS;
}

/***********
 * 8.
 * Read VectorImage<complex> as VectorImage<complex>
 * out : out[0]=in[0], out[1]=in[1], ...
 ***********/
int otbVectorImageComplexToVectorImageDouble(int argc, char * argv[])
{
  typedef double                                RealType;
  typedef otb::VectorImage<RealType, 2>               ScalVectorImageType;
  typedef otb::ImageFileReader<ScalVectorImageType>       ReaderType;


  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  ScalVectorImageType::IndexType id;
  ScalVectorImageType::SizeType size;
  ScalVectorImageType::RegionType region;

  id.Fill(0);
  size[0] = 10;
  size[1] = 1;

  region.SetSize( size );
  region.SetIndex( id ); 

  reader->GetOutput()->SetRequestedRegion(region);
  reader->Update();
  unsigned int l_Size = reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0]* reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1];

  itk::ImageRegionIteratorWithIndex<ScalVectorImageType> it( reader->GetOutput(), region );
  unsigned int count = 0;
  it.GoToBegin();
  while( it.IsAtEnd()==false )
    {
      count = 2*(it.GetIndex()[1]*reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1]+it.GetIndex()[0]);
      double norm1 = vcl_sqrt(static_cast<double>(count*count + (count+1)*(count+1)));
      double norm2 = vcl_sqrt(static_cast<double>((2*l_Size+count)*(2*l_Size+count) + (2*l_Size+count+1)*(2*l_Size+count+1)));
 
      if( (it.Get()[0] != norm1) || (it.Get()[1] != norm2) ) 
	{
	  std::cout<<"Vector Image complex read as Vector Image double error: "<<it.Get()<<", waited for ("<<norm1<<", "<<norm2<<")."<<std::endl;
	  
	  return EXIT_FAILURE;
	}

      ++it;
    }
  
  
  return EXIT_SUCCESS;
}


/***********
 * 9.
 * Read VectorImage<complex> as VectorImage<complex>
 * out : out[0]=norm(in[0]), out[0]=norm(in[1]), ...
 ***********/
int otbVectorImageComplexToVectorImageComplex(int argc, char * argv[])
{
  typedef double                                RealType;
  typedef std::complex<RealType>                PixelType;
  typedef otb::VectorImage<PixelType, 2>               CmplxVectorImageType;
  typedef otb::ImageFileReader<CmplxVectorImageType>       ReaderType;


  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  CmplxVectorImageType::IndexType id;
  CmplxVectorImageType::SizeType size;
  CmplxVectorImageType::RegionType region;

  id.Fill(0);
  size[0] = 10;
  size[1] = 1;

  region.SetSize( size );
  region.SetIndex( id ); 

  reader->GetOutput()->SetRequestedRegion(region);
  reader->Update();

  itk::ImageRegionIteratorWithIndex<CmplxVectorImageType> it( reader->GetOutput(), region );
  unsigned int l_Size = reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0]* reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1];

  unsigned int count = 0;
  it.GoToBegin();
  while( it.IsAtEnd()==false )
    {
     count = 2*(it.GetIndex()[1]*reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1]+it.GetIndex()[0]);

      PixelType cmplx1(count, count+1);
      PixelType cmplx2(2*l_Size+count, 2*l_Size+count+1);

      if( (it.Get()[0] != cmplx1) || (it.Get()[1] != cmplx2) ) 
	{
	  std::cout<<"Image double read as Vector Image complex error: "<<std::endl;
	  std::cout<<it.Get()[0]<<", waited for "<<cmplx1<<"."<<std::endl;
	  std::cout<<it.Get()[1]<<", waited for "<<cmplx2<<"."<<std::endl;

	  return EXIT_FAILURE;
	}

      ++it;
    }
  
  
  return EXIT_SUCCESS;
}


/***********
 * 10.
 * Read Image<double> as VectorImage<complex>
 * out : out[0].real()=in, out[0].imag=0
 ***********/
int otbImageDoubleToVectorImageComplex(int argc, char * argv[])
{
  typedef double                                RealType;
  typedef std::complex<RealType>                PixelType;
  typedef otb::VectorImage<PixelType, 2>               CmplxVectorImageType;
  typedef otb::ImageFileReader<CmplxVectorImageType>       ReaderType;


  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  CmplxVectorImageType::IndexType id;
  CmplxVectorImageType::SizeType size;
  CmplxVectorImageType::RegionType region;

  id.Fill(0);
  size[0] = 10;
  size[1] = 1;

  region.SetSize( size );
  region.SetIndex( id ); 

  reader->GetOutput()->SetRequestedRegion(region);
  reader->Update();

  if(reader->GetOutput()->GetNumberOfComponentsPerPixel() != 1)
    {
      std::cout<<"Invalid image size, should be 1, no "<<reader->GetOutput()->GetNumberOfComponentsPerPixel()<<"."<<std::endl;

      return EXIT_FAILURE;
    }

  itk::ImageRegionIteratorWithIndex<CmplxVectorImageType> it( reader->GetOutput(), region );
  unsigned int l_Size = reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0]* reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1];

  unsigned int count = 0;
  it.GoToBegin();
  while( it.IsAtEnd()==false )
    {
      count = it.GetIndex()[1]*reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1]+it.GetIndex()[0];

      if( (it.Get()[0].real() != static_cast<RealType>(count)) || (it.Get()[0].imag() != 0) ) 
	{
	  std::cout<<"Image double read as Vector Image complex error: "<<it.Get()[0]<<", waited for ("<<count<<", 0)."<<std::endl;
	  
	  return EXIT_FAILURE;
	}

      ++it;
    }
  
  
  return EXIT_SUCCESS;
}

