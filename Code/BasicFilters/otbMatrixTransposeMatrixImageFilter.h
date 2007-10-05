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
#ifndef __otbMatrixTransposeMatrixImageFilter_h
#define __otbMatrixTransposeMatrixImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkNumericTraits.h"
#include "itkArray.h"
#include "itkSimpleDataObjectDecorator.h"
#include "otbStreamingTraits.h"
#include "itkImageRegionSplitter.h"
#include "itkVariableSizeMatrix.h"
#include "itkVariableLengthVector.h"


namespace otb {
  
  /** \class MatrixTransposeMatrixImageFilter
   * \brief Compute \f[X^T.Y \f]. Allow a padding of ones.
   * 
   * \f[X\f] and \f[Y\f] are the input images.
   * The padding has the effect of adding a component filled with ones to the image
   *
   * \sa StreamingTraits
   * \sa StatisticsImageFilter
   * \ingroup MathematicalStatisticsImageFilters
   */
  template<class TInputImage, class TInputImage2>
    class ITK_EXPORT MatrixTransposeMatrixImageFilter :
    public itk::ImageToImageFilter<TInputImage, TInputImage>
    {
    public:
      /** Standard Self typedef */
      typedef MatrixTransposeMatrixImageFilter              Self;
      typedef itk::ImageToImageFilter<TInputImage,TInputImage>  Superclass;
      typedef itk::SmartPointer<Self>                           Pointer;
      typedef itk::SmartPointer<const Self>                     ConstPointer;
      
      /** Method for creation through the object factory. */
      itkNewMacro(Self);
      
      /** Runtime information support. */
      itkTypeMacro(MatrixTransposeMatrixImageFilter, ImageToImageFilter);
      
      /** Image related typedefs. */
      // First Input
      typedef TInputImage                                                            ImageType;
      typedef typename TInputImage::Pointer                                          InputImagePointer;
      typedef typename TInputImage::RegionType                                       RegionType;
      typedef typename TInputImage::SizeType                                         SizeType;
      typedef typename TInputImage::IndexType                                        IndexType;
      typedef typename TInputImage::PixelType                                        PixelType;
      typedef typename TInputImage::InternalPixelType                                InternalPixelType;

      typedef typename TInputImage2::IndexType                                        IndexType2;
      typedef typename TInputImage2::PixelType                                        PixelType2;
      typedef typename TInputImage2::InternalPixelType                                InternalPixelType2;

      typedef StreamingTraits<TInputImage>                                           StreamingTraitsType;
      typedef StreamingMode                                                           StreamingModeType;
      typedef itk::ImageRegionSplitter<itkGetStaticConstMacro(InputImageDimension)>   SplitterType;
      typedef typename SplitterType::Pointer                                          SplitterPointer;
      
      itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
      
      
      /** Streaming-related accessors */
      itkSetMacro(BufferMemorySize, unsigned long);
      itkGetMacro(BufferMemorySize, unsigned long);
      itkSetMacro(BufferNumberOfLinesDivisions, unsigned long);
      itkGetMacro(BufferNumberOfLinesDivisions, unsigned long);
      itkSetMacro(NumberOfStreamDivisions,unsigned long);
      itkGetMacro(NumberOfStreamDivisions,unsigned long);
      itkSetMacro(StreamingMode,StreamingModeType);
      itkGetMacro(StreamingMode,StreamingModeType);
      
      
      itkSetMacro(UsePadFirstInput,bool);
      itkGetMacro(UsePadFirstInput,bool);
      itkSetMacro(UsePadSecondInput,bool);
      itkGetMacro(UsePadSecondInput,bool);


      itkSetObjectMacro(RegionSplitter, SplitterType);
      itkGetObjectMacro(RegionSplitter, SplitterType);
      
      /** Image related typedefs. */
      itkStaticConstMacro(ImageDimension, unsigned int, TInputImage::ImageDimension );
      
      
      
      /** Type to use for computations. */
      // First Input
      typedef typename itk::NumericTraits<InternalPixelType>::RealType RealType;
      typedef itk::VariableLengthVector<RealType>                      RealPixelType;
      
      
      /** Smart Pointer type to a DataObject. */
      typedef typename itk::DataObject::Pointer DataObjectPointer;
      
      /** Type of DataObjects used for scalar outputs */
      typedef typename itk::Array<long>                     ArrayLongPixelType;
      typedef typename itk::VariableSizeMatrix<RealType>    MatrixType;
      typedef typename std::vector<MatrixType>              ArrayMatrixType;
      typedef typename std::vector<RealPixelType>           ArrayRealPixelType;
      typedef typename std::vector<PixelType>               ArrayPixelType;
      typedef itk::SimpleDataObjectDecorator<RealPixelType> RealPixelObjectType;
      typedef itk::SimpleDataObjectDecorator<PixelType>     PixelObjectType;
      typedef itk::SimpleDataObjectDecorator<MatrixType>    MatrixObjectType;
      
      
      /** Return the computed transpose(Image1)*Image2. */
      MatrixType GetResult() const { return this->GetResultOutput()->Get(); };
      MatrixObjectType* GetResultOutput();
      const MatrixObjectType* GetResultOutput() const;
      
      
      /** Make a DataObject of the correct type to be used as the specified
       * output. 
       */
      virtual DataObjectPointer MakeOutput(unsigned int idx);
      
      /** Pass the input through unmodified. Do this by Grafting in the
       *  AllocateOutputs method.
       */
      void AllocateOutputs();
      void GenerateOutputInformation();


      /** Input wrapper */
      void SetFirstInput(const TInputImage  *input1){ this->SetInput(0, input1 ); };
      void SetSecondInput(const TInputImage2 *input2){ this->SetInput(1, input2 ); };

      const TInputImage* GetFirstInput()
	{
	  if( this->GetNumberOfInputs() < 1 )
	    {
	      return 0;
	    }
	  else
	    return( static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(0)) );
	}
      
      const TInputImage2* GetSecondInput()
	{
	  if( this->GetNumberOfInputs() < 2 )
	    {
	      return 0;
	    }
	  else
	    return( static_cast<const TInputImage2 *>(this->itk::ProcessObject::GetInput(1)) );
	}
      
    protected:
      MatrixTransposeMatrixImageFilter();
      ~MatrixTransposeMatrixImageFilter(){};
      void PrintSelf(std::ostream& os, itk::Indent indent) const;
      /** Initialize some accumulators before the threads run. */
      virtual void BeforeThreadedGenerateData ();
      /** Do final mean and variance computation from data accumulated in threads. */
      virtual void AfterThreadedGenerateData ();
      /** Multi-thread version GenerateData. */
      virtual void  ThreadedGenerateData (const RegionType& outputRegionForThread,int threadId);
      // Override since the filter needs all the data for the algorithm
      virtual void GenerateInputRequestedRegion();
      
    private:
      MatrixTransposeMatrixImageFilter(const Self&); //purposely not implemented
      void operator=(const Self&); //purposely not implemented
      
      ArrayMatrixType    m_ThreadSum;
      bool m_UsePadFirstInput;
      bool m_UsePadSecondInput;

      /** Nulber Of Component per Pixel. Change for padding */
      unsigned int m_NumberOfComponents1;
      unsigned int m_NumberOfComponents2;

      /** Use to define the method used to calculate number of divisions */ 
      unsigned long m_BufferMemorySize;
      unsigned long m_BufferNumberOfLinesDivisions;
      unsigned long m_NumberOfStreamDivisions;
      
      SplitterPointer m_RegionSplitter;
      
      /** Use to determine method of calculation number of divisions */
      StreamingModeType  m_StreamingMode;
    }; // end of class
  
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMatrixTransposeMatrixImageFilter.txx"
#endif

#endif
