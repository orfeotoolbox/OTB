#ifndef _LIBSVM_H
#define _LIBSVM_H

//OTB's modifications
//namespace otb
//{
class GenericKernelFunctorBase;
//}

#include <map>
#include "otbMacro.h"

#ifdef __cplusplus
extern "C" {
#endif

struct svm_node
{
	int index;
	double value;
};

struct svm_problem
{
	int l;
	double *y;
	struct svm_node **x;
};

enum { C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR };	/* svm_type */
//OTB's modifications
enum { LINEAR, POLY, RBF, SIGMOID, PRECOMPUTED, GENERIC  }; /* kernel_type */

struct svm_parameter
{
	int svm_type;
	int kernel_type;
	int degree;	/* for poly */
	double gamma;	/* for poly/rbf/sigmoid */
	double coef0;	/* for poly/sigmoid */

//OTB's modifications : Use by the generic kernel
        /*otb::*/GenericKernelFunctorBase * kernel_generic;
        
	/* these are for training only */
	double cache_size; /* in MB */
	double eps;	/* stopping criteria */
	double C;	/* for C_SVC, EPSILON_SVR and NU_SVR */
	int nr_weight;		/* for C_SVC */
	int *weight_label;	/* for C_SVC */
	double* weight;		/* for C_SVC */
	double nu;	/* for NU_SVC, ONE_CLASS, and NU_SVR */
	double p;	/* for EPSILON_SVR */
	int shrinking;	/* use the shrinking heuristics */
	int probability; /* do probability estimates */
};

//
// svm_model
//
struct svm_model
{
	svm_parameter param;	// parameter
	int nr_class;		// number of classes, = 2 in regression/one class svm
	int l;			// total #SV
	svm_node **SV;		// SVs (SV[l])
	double **sv_coef;	// coefficients for SVs in decision functions (sv_coef[k-1][l])
	double *rho;		// constants in decision functions (rho[k*(k-1)/2])
	double *probA;          // pariwise probability information
	double *probB;

	// for classification only

	int *label;		// label of each class (label[k])
	int *nSV;		// number of SVs for each class (nSV[k])
				// nSV[0] + nSV[1] + ... + nSV[k-1] = l
	// XXX
	int free_sv;		// 1 if svm_model is created by svm_load_model
				// 0 if svm_model is created by svm_train
};

struct svm_model *svm_train(const struct svm_problem *prob, const struct svm_parameter *param);
void svm_cross_validation(const struct svm_problem *prob, const struct svm_parameter *param, int nr_fold, double *target);

int svm_save_model(const char *model_file_name, const struct svm_model *model);

int svm_get_svm_type(const struct svm_model *model);
int svm_get_nr_class(const struct svm_model *model);
void svm_get_labels(const struct svm_model *model, int *label);
double svm_get_svr_probability(const struct svm_model *model);

void svm_predict_values(const struct svm_model *model, const struct svm_node *x, double* dec_values);
double svm_predict(const struct svm_model *model, const struct svm_node *x);
double svm_predict_probability(const struct svm_model *model, const struct svm_node *x, double* prob_estimates);

void svm_destroy_model(struct svm_model *model);
void svm_destroy_param(struct svm_parameter *param);

const char *svm_check_parameter(const struct svm_problem *prob, const struct svm_parameter *param);
int svm_check_probability_model(const struct svm_model *model);

//OTB's modifications
struct svm_model *svm_load_model(const char *model_file_name, /*otb::*/GenericKernelFunctorBase * generic_kernel_functor = NULL);

#ifdef __cplusplus
}
#endif


/*
namespace otb
{
*/
//OTB's modifications
class GenericKernelFunctorBase
{
public:
  GenericKernelFunctorBase() : m_Name("FunctorName") {};
  virtual ~GenericKernelFunctorBase() {};


  template<class T> 
  T GetValue(const char *option) const
  {
        std::string Value = m_MapParameters.find(std::string(option))->second;
        T lValeur;
        ::otb::StringStream flux;
        flux << Value;
        flux >> lValeur;
        return lValeur;
  }
  template<class T> 
  void SetValue(const char *option, const T & value)
  {
        std::string lValeur;
        ::otb::StringStream flux;
        flux << value;
        flux >> lValeur;
        m_MapParameters[std::string(option)] = lValeur;
  }

/*
#define otbGetValueMacro(name,type)                                                 \
  virtual type GetValue##name (const char *option) const                            \
  {                                                                                 \
        std::string Value = m_MapParameters.find(std::string(option))->second;      \
        type lValeur;                                                               \
        ::otb::StringStream flux;                                                   \
        flux << Value;                                                              \
        flux >> lValeur;                                                            \
        return lValeur;                                                             \
  }
  
  otbGetValueMacro(String,std::string);
  otbGetValueMacro(Short,short);
  otbGetValueMacro(UShort,unsigned short);
  otbGetValueMacro(Int,int);
  otbGetValueMacro(UInt,unsigned int);
  otbGetValueMacro(Long,long);
  otbGetValueMacro(ULong,unsigned long);
  otbGetValueMacro(Float,float);
  otbGetValueMacro(Double,double);

#define otbSetValueMacro(name,type)                                                 \
  virtual void SetValue##name (const char *option, const type & value)              \
  {                                                                                 \
        std::string lValeur;                                                        \
        ::otb::StringStream flux;                                                   \
        flux << value;                                                              \
        flux >> lValeur;                                                            \
        m_MapParameters[std::string(option)] = lValeur;\
  }
  
  otbSetValueMacro(String,std::string);
  otbSetValueMacro(Short,short);
  otbSetValueMacro(UShort,unsigned short);
  otbSetValueMacro(Int,int);
  otbSetValueMacro(UInt,unsigned int);
  otbSetValueMacro(Long,long);
  otbSetValueMacro(ULong,unsigned long);
  otbSetValueMacro(Float,float);
  otbSetValueMacro(Double,double);
*/
   
  virtual double operator()(const svm_node *x, const svm_node *y, const svm_parameter& param)const // = 0
    {
      itkGenericExceptionMacro(<<"Kernel functor not definied (Null)");
      return static_cast<double>(0.);
    }
    
  virtual int load_parameters(FILE ** pfile);

  virtual int save_parameters(FILE ** pfile, const char * generic_kernel_parameters_keyword)const;

  virtual void print_parameters(void)const;
  
  virtual void Update(void){ }

  virtual double dot(const svm_node *px, const svm_node *py)const;
  
  virtual void SetName(const std::string & name ) { m_Name = name;}
  virtual std::string GetName(void) { return m_Name;}

private:

  typedef std::map<std::string,std::string>        MapType;
  typedef MapType::iterator               MapIterator;
  typedef MapType::const_iterator         MapConstIterator;

    /** Kernel functor parameters */
    MapType m_MapParameters;
    
    /** Functor label name */
    std::string m_Name;
    
};

//} // namespace otb

#endif /* _LIBSVM_H */
