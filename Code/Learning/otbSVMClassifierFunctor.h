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
#ifndef __otbSVMClassifierFunctor_h
#define __otbSVMClassifierFunctor_h

#include "otbSVMModel.h"
#include "otbMacro.h"

namespace otb
{
namespace Functor
  {
    /** \class SVMClassifierFunctor
     *
     * \brief This Functor performs svm classification according to a model.
     *
     *  Before using this functor, one must first set the model and call the AllocateProblem() method.
     *  This functor is used in the following classes:
     *  \sa SVMImageClassificationFilter
     *  \sa SVMClassifier
     *
     *  Please note that this functor is NOT thread-safe, so you should use one functor per thread.
     *
     */
    template<class TMeasurement, class TLabel>
      class SVMClassifierFunctor
      {
      public:
	//// Constructor
	SVMClassifierFunctor()
	  {
	    m_NumberOfComponentsPerSample = 0;
	    m_SVMType = 0;
	    m_NumberOfClasses = 0;
	    
	    m_PredictProbability = true;
	    m_ProblemAllocated = false;
	    
	    // Initialize pointers
	    m_X =NULL;
	    m_Model=NULL;
	    m_Labels = NULL;
	    m_ProbabilityEstimates = NULL;
	  }

	/// Destructor
	~SVMClassifierFunctor()
	  {
	    FreeProblem();
	  }

	/// Operator performing the classification
	TLabel operator()(const TMeasurement & A)
	  {
	    // Check if problem has been allocated
	    if(!m_ProblemAllocated)
	      {
		itkGenericExceptionMacro(<<"Classification task impossible: Problem is not allocated.");
	      }

	    unsigned int i=0; 
	    double v = 0;

	    // Fill the samples
	    for(i=0; i<m_NumberOfComponentsPerSample; i++)
	      {
		m_X[i].index = i+1 ;
		m_X[i].value = A[i];
	      }
	    m_X[i].index = -1;
	    
	    // Classify A
	    if(m_PredictProbability && (m_SVMType == C_SVC || m_SVMType ==  NU_SVC))
	      {
		v = svm_predict_probability(m_Model,m_X,m_ProbabilityEstimates);
	      }
	    else
	      {
		v = svm_predict(m_Model,m_X);
	      }

	    // return the label
	    TLabel label = static_cast<TLabel>(v);
	    return label;
	  }

	/// Allocate the data for the classification problem
	void AllocateProblem()
	  {
	    if(m_Model == NULL)
	      {
		itkGenericExceptionMacro(<<"No input model, can not allocate problem.");
	      }

	    if(m_NumberOfComponentsPerSample == 0)
	      {
		itkGenericExceptionMacro(<<"Number of components per sample is zero, can not allocate problem.");
	      }

	    // Clear previous memory
	    FreeProblem();

	    // Allocate Xspace
	    m_X = new svm_node[m_NumberOfComponentsPerSample+1];

	    // Check probability estimates
	    if(svm_check_probability_model(m_Model)==0)
	      {
		throw itk::ExceptionObject(__FILE__, __LINE__,
					   "Model does not support probabiliy estimates",ITK_LOCATION);
	      }
	    // Get svm type
	    m_SVMType = svm_get_svm_type(m_Model);
	    
	    // Get number of classes
	    m_NumberOfClasses = svm_get_nr_class(m_Model);
	    
	    // Allocates lables
	    m_Labels = new int[m_NumberOfClasses];
	    
	    // Check if model supports probabilty prediction
	    if(m_PredictProbability)
	      {
		if (m_SVMType==NU_SVR || m_SVMType==EPSILON_SVR)
		  {  
		    otbGenericMsgDebugMacro(<<"Prob. model for test data: target value = predicted value + z,\nz: Laplace distribution e^(-|z|/sigma)/(2sigma),sigma="<<svm_get_svr_probability(m_Model));
		  }
		else
		  {
		    svm_get_labels(m_Model,m_Labels);
		    m_ProbabilityEstimates = new double[m_NumberOfClasses];
		  }
	      }
	    m_ProblemAllocated = true;

	  };

	// Free the data for the classification problem
	void FreeProblem()
	  {
	    if(m_X != NULL)
	      delete [] m_X;
	    if(m_Labels != NULL)
	      delete [] m_Labels;
	    if(m_ProbabilityEstimates != NULL)
	      delete [] m_ProbabilityEstimates;
	    m_ProblemAllocated = false;
	  };
	
	// Set the svm model
	void SetModel(const struct svm_model * model)
	  {
	    FreeProblem();
	    m_Model = const_cast<struct svm_model *>(model);
	  }

	// Set the number of Components per sample
	void SetNumberOfComponentsPerSample(unsigned int nb)
	  {
	    FreeProblem();
	    m_NumberOfComponentsPerSample = nb;
	  }

      protected:
	struct svm_node * m_X;
        struct svm_model * m_Model;
	int * m_Labels;
	double * m_ProbabilityEstimates;

	bool m_PredictProbability;
	bool m_ProblemAllocated;

	unsigned int m_NumberOfComponentsPerSample;
	int m_SVMType;
	int m_NumberOfClasses;
      };
  } // end namespace Functor
} // end namespace otb

#endif //__otbSVMClassifierFunctor_h
