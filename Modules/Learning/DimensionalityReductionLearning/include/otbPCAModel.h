/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
#ifndef otbPCAModel_h
#define otbPCAModel_h

#include "otbMachineLearningModelTraits.h"
#include "otbMachineLearningModel.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#endif
#include "otb_shark.h"
#include <shark/Algorithms/Trainers/PCA.h>
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

namespace otb
{
template <class TInputValue>
class ITK_EXPORT PCAModel: public  MachineLearningModel<itk::VariableLengthVector< TInputValue> , itk::VariableLengthVector< TInputValue>>    
{

public:
	
	typedef PCAModel Self;
	typedef MachineLearningModel<itk::VariableLengthVector< TInputValue> , itk::VariableLengthVector< TInputValue>> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	typedef typename Superclass::InputValueType 			InputValueType;
	typedef typename Superclass::InputSampleType 			InputSampleType;
	typedef typename Superclass::InputListSampleType 		InputListSampleType;
	typedef typename InputListSampleType::Pointer 			ListSamplePointerType;
	typedef typename Superclass::TargetValueType 			TargetValueType;
	typedef typename Superclass::TargetSampleType 			TargetSampleType;
	typedef typename Superclass::TargetListSampleType 		TargetListSampleType;

	/// Confidence map related typedefs
	
	typedef typename Superclass::ConfidenceValueType  				ConfidenceValueType;
	typedef typename Superclass::ConfidenceSampleType 				ConfidenceSampleType;
	typedef typename Superclass::ConfidenceListSampleType      		ConfidenceListSampleType;


	itkNewMacro(Self);
	itkTypeMacro(PCAModel, DimensionalityReductionModel);
/*
	unsigned int GetDimension() {return m_Dimension;}; 
	itkSetMacro(Dimension,unsigned int);
	*/
	itkSetMacro(Do_resize_flag,bool);
	
	itkSetMacro(WriteEigenvectors, bool);
	itkGetMacro(WriteEigenvectors, bool);
	
	bool CanReadFile(const std::string & filename);
	bool CanWriteFile(const std::string & filename);

	void Save(const std::string & filename, const std::string & name="")  ITK_OVERRIDE;
	void Load(const std::string & filename, const std::string & name="")  ITK_OVERRIDE;

	void Train() ITK_OVERRIDE;
	//void Dimensionality_reduction()  {}; // Dimensionality reduction is done by DoPredict
	 

protected:
	PCAModel();	
	~PCAModel() ITK_OVERRIDE;
 
	virtual TargetSampleType DoPredict(const InputSampleType& input, ConfidenceValueType * quality = ITK_NULLPTR) const;

	virtual void DoPredictBatch(const InputListSampleType *, const unsigned int & startIndex, const unsigned int & size, TargetListSampleType *, ConfidenceListSampleType * quality = ITK_NULLPTR) const ITK_OVERRIDE;
  
private:
	shark::LinearModel<> m_encoder;
	shark::LinearModel<> m_decoder;
	shark::PCA m_pca;
	//unsigned int m_Dimension;
	bool m_Do_resize_flag;
	bool m_WriteEigenvectors;
};
} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPCAModel.txx"
#endif


#endif

