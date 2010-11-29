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
      if( (it.Get().real() != static_cast<RealType>(count)) || (it.Get().imag() != static_cast<RealType>(0)) ) 
	{
	  std::cout<<"Image double read as Image complex error : "<<it.Get()<<", waited for ("<<count<<", 0)"<<std::endl;

	  return EXIT_FAILURE;
	}
      count++;
      ++it;
    }

  return EXIT_SUCCESS;
}


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
  size[1] = 1;

  region.SetSize( size );
  region.SetIndex( id ); 

  reader->GetOutput()->SetRequestedRegion(region);
  reader->Update();

  itk::ImageRegionIterator<ScalImageType> it( reader->GetOutput(), region );

  unsigned int count = 0;
  it.GoToBegin();
  while( it.IsAtEnd()==false )
    {
      double norm = vcl_sqrt(static_cast<double>(count*count + (count+1)*(count+1)));
      if(it.Get() != static_cast<RealType>(norm))
	{
	  std::cout<<"Image complex read as Image double error : waited for norm "<<it.Get()<<", waited for "<<norm<<"."<<std::endl;
	  
	  return EXIT_FAILURE;
	}
      count += 2;
      ++it;
    }
  
  
  return EXIT_SUCCESS;
}



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

  itk::ImageRegionIterator<CplxImageType> it( reader->GetOutput(), region );

  unsigned int count = 0;
  it.GoToBegin();
  while( it.IsAtEnd()==false )
    {
      if( (it.Get().real() != static_cast<RealType>(count)) || ( it.Get().imag() != static_cast<RealType>(count+1) ) ) 
	{
	  std::cout<<"Image complex read as Image complex error: "<<it.Get()<<", waited for ("<<count<<", "<<count+1<<")"<<std::endl;
	  
	  return EXIT_FAILURE;
	}
      
      count += 2;
      ++it;
    }


  return EXIT_SUCCESS;

}


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

  itk::ImageRegionIterator<ScalVectorImageType> it( reader->GetOutput(), region );

  unsigned int count = 0;
  it.GoToBegin();
  while( it.IsAtEnd()==false )
    {
      if( (it.Get()[0] != static_cast<RealType>(count)) || (it.Get()[1] != static_cast<RealType>(count+1)) )
	{
	  std::cout<<"Image complex read as Vector Image double error: "<<it.Get()<<", waited for ("<<count<<", "<<count+1<<")"<<std::endl;
	  
	  return EXIT_FAILURE;
	}

      count += 2;
      ++it;
    }
  
  
  return EXIT_SUCCESS;
}


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

  itk::ImageRegionIterator<CmplxVectorImageType> it( reader->GetOutput(), region );

  unsigned int count = 0;
  it.GoToBegin();
  while( it.IsAtEnd()==false )
    {
      if( (it.Get()[0].real() != static_cast<RealType>(count)) || ( it.Get()[0].imag() != static_cast<RealType>(count+1) ) ) 
	{
	  std::cout<<"Image complex read as Vector Image complex error: "<<it.Get()[0]<<", waited for ("<<count<<", "<<count+1<<")"<<std::endl;
	  
	  return EXIT_FAILURE;
	}
   
      count += 2;
      ++it;
    }
  
  
  return EXIT_SUCCESS;
}


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

  itk::ImageRegionIterator<CmplxImageType> it( reader->GetOutput(), region );

  unsigned int count = 0;
  it.GoToBegin();
  while( it.IsAtEnd()==false )
    {
      if(it.Get().real() != static_cast<RealType>(count) || (it.Get().imag() != static_cast<RealType>(count+1)) ) 
	{
	  std::cout<<"Vector Image double read as Image double error: "<<it.Get()<<", waited for ("<<count<<", "<<count+1<<")"<<std::endl;
	  
	  return EXIT_FAILURE;
	}

      count += 2;
      ++it;
    }
  
  
  return EXIT_SUCCESS;
}


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

  itk::ImageRegionIterator<CmplxVectorImageType> it( reader->GetOutput(), region );

  unsigned int count = 0;
  it.GoToBegin();
  while( it.IsAtEnd()==false )
    {
      if(it.Get()[0].real() != static_cast<RealType>(count) || (it.Get()[0].imag() != static_cast<RealType>(0)) ) 
	{
	  std::cout<<"Vector Image double read as Vector Image complex error: "<<it.Get()[0]<<", waited for ("<<count<<", 0)."<<std::endl;
	  
	  return EXIT_FAILURE;
	}

      count += 2;
      ++it;
    }
  
  
  return EXIT_SUCCESS;
}

int otbVectorImageComplexToVectorImageDouble(int argc, char * argv[])
{
  typedef double                                RealType;
  typedef otb::VectorImage<RealType, 2>               ScalVectorImageType;
  typedef otb::ImageFileReader<ScalVectorImageType>       ReaderType;


  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  ScalVectorImageType::IndexType id;
  SccalVectorImageType::SizeType size;
  ScalVectorImageType::RegionType region;

  id.Fill(0);
  size[0] = 10;
  size[1] = 1;

  region.SetSize( size );
  region.SetIndex( id ); 

  reader->GetOutput()->SetRequestedRegion(region);
  reader->Update();

  itk::ImageRegionIterator<ScalVectorImageType> it( reader->GetOutput(), region );
  unsigned int l_Size = reader->GetOutpuGetOutput()->GetLargetPossibleRegion().GetSize()[0]* reader->GetOutpuGetOutput()->GetLargetPossibleRegion().GetSize()[1];
  unsigned int count = 0;
  it.GoToBegin();
  while( it.IsAtEnd()==false )
    {
      RealType norm1 = static_cast<double>( vcl_sqrt( static_cast<double>( (count*count)+((count+1)*(count+1)) ) ) );
      RealType norm2 = static_cast<double>( vcl_sqrt( static_cast<double>( ((l_size+count)*(l_Size+count))+((l_Size+count+1)*(l_Size+count+1)) ) ) );
      if( (it.Get()[0] != norm1) || (it.Get()[1] != norm2) ) 
	{
	  std::cout<<"Vector Image complex read as Vector Image double error: "<<it.Get()<<", waited for ("<<norm1<<", "<<norm2<<")."<<std::endl;
	  
	  return EXIT_FAILURE;
	}

      count += 2;
      ++it;
    }
  
  
  return EXIT_SUCCESS;
}


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

  itk::ImageRegionIterator<CmplxVectorImageType> it( reader->GetOutput(), region );
  unsigned int l_Size = reader->GetOutpuGetOutput()->GetLargetPossibleRegion().GetSize()[0]* reader->GetOutpuGetOutput()->GetLargetPossibleRegion().GetSize()[1];

  unsigned int count = 0;
  it.GoToBegin();
  while( it.IsAtEnd()==false )
    {
      if( (it.Get()[0].real() != static_cast<RealType>(count)) || (it.Get()[0].imag() != 0) ) 
	{
	  std::cout<<"Image double read as Vector Image complex error: "<<it.Get()[0]<<", waited for ("<<count<<", 0)."<<std::endl;
	  
	  return EXIT_FAILURE;
	}

      count++;
      ++it;
    }
  
  
  return EXIT_SUCCESS;
}

