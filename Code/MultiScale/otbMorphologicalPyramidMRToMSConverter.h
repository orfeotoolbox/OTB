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
#ifndef _otbMorphologicalPyramidMRToMSConverter_h
#define _otbMorphologicalPyramidMRToMSConverter_h

#include "otbImageListToImageListFilter.h"

namespace otb
{
  namespace morphologicalPyramid
    {
      /**
       * \class MRToMSConverter
       * \brief This class convert a multi resolution decomposition from the morphological pyramid to a multi-scale one.
       * 
       * This class convert a multi resolution decomposition from the morphological pyramid to a multi-scale one.
       * 
       * The operation consists in iterativly upsampling and adding decimation details to a given detail image.
       * 
       * \sa MorphologicalPyramidAnalyseFilter
       * \sa MorphologicalPyramidSynthesisFilter
       */
      template <class TInputImage, class TOutputImage>
	class  MRToMSConverter
	: public ImageListToImageListFilter<TInputImage,TOutputImage>
	{
	  public :
	    /** Standard typedefs */ 
	    typedef MRToMSConverter                                    Self;
	  typedef ImageListToImageListFilter<TInputImage,TOutputImage> Superclass;
	  typedef itk::SmartPointer<Self>                              Pointer;
	  typedef itk::SmartPointer<const Self>                        ConstPointer;
	  /** Creation through object factory macro */
	  itkNewMacro(Self);
	  /** Type macro */
	  itkTypeMacro(MRToMSConverter,ImageListToImageListFilter);
	  /** Input parameters typedefs */
	  typedef TInputImage                                     InputImageType;
	  typedef typename Superclass::InputImagePointer          InputImagePointer;
	  typedef typename Superclass::InputImageListPointerType  InputImageListPointerType;
	  /** Output parameters typedefs */
	  typedef TOutputImage                                    OutputImageType;
	  typedef typename Superclass::OutputImagePointer         OutputImagePointerType;
	  typedef typename Superclass::OutputImageListType        OutputImageListType;
	  typedef typename Superclass::OutputImageListPointerType OutputImageListPointerType;
	  /** Input image lists setters */
	  itkSetMacro(SupFiltre,InputImageListPointerType);
	  itkSetMacro(InfFiltre,InputImageListPointerType);
	  itkSetMacro(SupDeci,InputImageListPointerType);
	  itkSetMacro(InfDeci,InputImageListPointerType);

	  /** Number of iterations accessors */
	  itkSetMacro(NumberOfIterations, unsigned int);
	  itkGetMacro(NumberOfIterations, unsigned int);

	  /** Full resolution detail image lists getters */
	  itkGetMacro(SupFiltreFullResolution,OutputImageListPointerType);
	  itkGetMacro(InfFiltreFullResolution,OutputImageListPointerType);
	protected:
	  /** Constructor */
	  MRToMSConverter();
	  /** Destructor */
	  ~MRToMSConverter() {};

	  /** Main computation method */
	  void GenerateData();
	  /** PrintSelf method */
	  void PrintSelf(std::ostream& os, itk::Indent indent) const;

	  private :
	    MRToMSConverter(const Self&); // purposely not implemented
	  void operator=(const Self&); // purposely not implemented

	  InputImageListPointerType  m_SupFiltre;
	  InputImageListPointerType  m_InfFiltre;
	  InputImageListPointerType  m_SupDeci;
	  InputImageListPointerType  m_InfDeci;
	  OutputImageListPointerType m_SupFiltreFullResolution;
	  OutputImageListPointerType m_InfFiltreFullResolution;
	  unsigned int m_NumberOfIterations;




	};
    } // End namespace morphologicalPyramid
}// End namespace otb

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbMorphologicalPyramidMRToMSConverter.txx"
#endif

#endif
