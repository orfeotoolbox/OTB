#include "otbVectorImage.h"
#include "vnl/vnl_matrix.h"
#include "vnl/vnl_cross.h"
#include "vnl/vnl_vector.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

typedef double 											PixelType;
typedef otb::VectorImage<PixelType, 2> 					VectorImageType;
typedef vnl_matrix<double>								VnlMatrixType;
typedef vnl_vector<double>								VectorType;
typedef itk::VariableLengthVector<double>				ItkVectorType;

typedef otb::ImageFileReader<VectorImageType>			ReaderType;
typedef otb::ImageFileWriter<VectorImageType>			WriterType;



#define active_debug 0
#define active_debug_loop 0
#define divisor 1
#define divisorParam 10
#define maxIter 100


void AddOneRowOfOnes(const vnl_matrix<double> & m,
		vnl_matrix<double> & M)
{
	M.set_size(m.rows()+1, m.cols());

	for (int i=0; i<M.rows()-1; ++i)
	{
		M.set_row(i, m.get_row(i));
	}
	M.set_row(M.rows()-1, 1.0);
}



double Criterion(const VnlMatrixType & X,
		const VnlMatrixType & A,
		const VnlMatrixType & S,
		const double &delt,
		const double &lambdS,
		const double &lambdD)
{
	// This function computes
	// f =	||Xsu-Asu.S||_2^2 -
	//		lambdS * ||1/J*ones - S||_2^2 +
	//		lambdD * (trace(transpose(A)*A)-1/L*trace(transpose(A).ones.A));
	//
	//	 =	||E1||_2^2 -
	//		lambdS * ||E2||_2^2 +
	//		lambdD * (trace(A'*A)-1/L*trace(E3);
	//
	// where	|| ||_2 is the L2 frobenius_norm,
	//			J is the number of endmembers and
	//			L is the number of spectral bands

	unsigned int nbEndmembers = A.cols();
	unsigned nbBands = A.rows();
	VnlMatrixType Xsu, Asu, ones, E1, E2;	//, E3;
	double evalf, sumColsOfA, trE3;

	Xsu.set_size(X.rows()+1, X.cols());
	Asu.set_size(A.rows()+1, A.cols());
	AddOneRowOfOnes(A, Asu);
	AddOneRowOfOnes(X, Xsu);

	//-------   Computing the function blocs E1, E2 and E3   -------//
	// Bloc 1
	E1 = Xsu - Asu*S;

	// Bloc 2
	E2 = S - 1./ ((double) nbEndmembers);// * ones - S;

	// Computing trace(transpose(A)*A)
	double trAtA = 0;
	for (int i=0; i<A.columns(); ++i)
	{
		trAtA += A.get_column(i).two_norm() * A.get_column(i).two_norm();
	}

	// Bloc 3: computing fast trE3 = trace(transpose(A)*ones*A)
	trE3 = 0;
	for (int j=0; j<nbEndmembers; ++j)
	{
		sumColsOfA = A.get_column(j).sum();
		trE3 += sumColsOfA * sumColsOfA;
	}


/*	for (int j=0; j<nbEndmembers; ++j)
	{
		sumColsOfA(j) = A.get_column(j).sum();
	}

	E3.set_size(nbEndmembers, nbEndmembers);
	for (int j1=0; j1<nbEndmembers; ++j1)
	{
		for (int j2=0; j2<nbEndmembers; ++j2)
		{
			E3(j1,j2) = sumColsOfA(j1)*sumColsOfA(j2);
		}
	}
*/
	//--------------------   Computing f   --------------------------//
	evalf = E1.frobenius_norm() * E1.frobenius_norm()
			- lambdS * E2.frobenius_norm() * E2.frobenius_norm()
			+ lambdD * (trAtA - (1./ ((double) nbBands) * trE3));
	return evalf;
}



class F
{
public:
	virtual double Call(const VnlMatrixType & variMat,
			const VnlMatrixType & fixedMat,
			const VnlMatrixType & X,
			const double & delt,
			const double & lambdS,
			const double & lambdD) = 0;

};



class FA: public F
{
public:
	double Call(const VnlMatrixType & variMat,
			const VnlMatrixType & fixedMat,
			const VnlMatrixType & X,
			const double & delt,
			const double & lambdS,
			const double & lambdD)
	{
		double evalf;
		evalf = Criterion(X, variMat, fixedMat, delt, lambdS, lambdD);
		return evalf;
	}

};



class FS: public F
{
public:
	double Call(const VnlMatrixType & variMat,
			const VnlMatrixType & fixedMat,
			const VnlMatrixType & X,
			const double & delt,
			const double & lambdS,
			const double & lambdD)
	{
		double evalf;
		evalf = Criterion(X, fixedMat, variMat, delt, lambdS, lambdD);
		return evalf;
	}
};



void evalGradS(const VnlMatrixType &X,
		const VnlMatrixType &A,
		const VnlMatrixType &S,
		const double &delt,
		const double &lambdS,
		VnlMatrixType & gradS)
{
	// Calculus of: gradS = 2 * Asu' * (Asu*S-Xsu) - lambd * 2 * (S - 1/J*ones(J,I));

	VnlMatrixType Xsu, Asu, ones;
	Xsu.set_size(X.rows()+1, X.cols());
	Asu.set_size(A.rows()+1, A.cols());
	ones.set_size(S.rows(), S.cols());
	ones.fill(1.);
	AddOneRowOfOnes(A, Asu);
	AddOneRowOfOnes(X, Xsu);

	gradS = 2. * Asu.transpose() * (Asu*S-Xsu) - lambdS * 2. * (S - (1./((double) S.rows())));	//))*ones);
}


void evalGradA(const VnlMatrixType &X,
		const VnlMatrixType &A,
		const VnlMatrixType &S,
		const double &delt,
		const double &lambdD,
		VnlMatrixType &gradA)
{
	// Compute gradA
	//	= (A*S-X) * (transpose(S)) + lambdD*(A-1/nbBands*ones(L,L)*A)
	//	= (A*S-X) * (transpose(S)) + lambdD*A- lambdD*/nbBands*ones(L,L)*A)

	VnlMatrixType onesA;
	VectorType sumColulmnsOfA;
	sumColulmnsOfA.set_size(A.cols());
	unsigned int nbBands = A.rows();

	// Computing vector onesA
	for (int j=0; j<onesA.size(); ++j)
	{
		sumColulmnsOfA(j) = A.get_column(j).sum();
	}

	// Initialize gradA
	gradA = (A*S-X) * S.transpose();

	// 1st update of gradA
	gradA += A*lambdD;

	// 2nd and last update id gradA, row by row (for performance reasons)
	for (int i=0; i<nbBands; ++i)
	{
		gradA.set_row(i, gradA.get_row(i) - ( sumColulmnsOfA*lambdD/((double)nbBands)));
	}
}


void SetNegativeCoefficientsToZero(VnlMatrixType & M)
{
	for (int i=0; i<M.rows(); ++i)
	{
		for (int j=0; j<M.cols(); ++j)
		{
			if (M(i,j)<0)
				M(i,j) = 0;
		}
	}
}


VnlMatrixType TermByTermMatrixProduct(const VnlMatrixType & M1, const VnlMatrixType & M2)
{
	VnlMatrixType M;
	M.set_size(M1.rows(), M1.cols());
	for (int i=0; i<M.rows(); ++i)
	{
		for (int j=0; j<M.cols(); ++j)
		{
			M(i,j) = M1(i,j) * M2(i,j);
		}
	}
	return M;
}

double SumMatrixElements(const VnlMatrixType & M)
{
	double sum = 0;
	for (int i = 0; i<M.cols(); ++i)
	{
		sum += M.get_column(i).sum();
	}
	return sum;
}



bool ArmijoTest(const double & sig,
		const VnlMatrixType variMat,
		const VnlMatrixType & newVariMat,
		const double & evalf,
		const double & newEvalf,
		const VnlMatrixType & gradVariMat,
		const double & alph)
{
	bool bit;

	//const unsigned int I = variMat.rows();
	//const unsigned int J = variMat.cols();

	const VnlMatrixType prod = TermByTermMatrixProduct(gradVariMat, newVariMat-variMat);
	double sumProd = SumMatrixElements(prod);

	if (newEvalf-evalf <= sig*alph*sumProd)
		bit = true;
	else
		bit = false;

	return bit;
}


void huckProjGradOneStep(
		const VnlMatrixType & X,
		const VnlMatrixType & fixedMat,
		const VnlMatrixType & gradVariMat,
		F & f,
		const double & sig,
		const double & betinit,
		const double & delt,
		const double & lambdS,
		const double & lambdD,

		VnlMatrixType & variMat,
		double & alph)

{


	double evalf, newEvalf, bet;
	evalf = f.Call(variMat, fixedMat, X, delt, lambdS, lambdD);	// compute evalf

	VnlMatrixType newVariMat = variMat - alph*gradVariMat;
	SetNegativeCoefficientsToZero(newVariMat);
	newEvalf = f.Call(newVariMat, fixedMat, X, delt, lambdS, lambdD);	// compute newEvalf
	bool bit = ArmijoTest(sig, variMat, newVariMat, evalf, newEvalf, gradVariMat, alph);

	int count = 1;
	if (bit == true)
	{
		while (bit == true)
		{
			bet = pow(betinit, count);
			alph = alph/bet;
			newVariMat = variMat - alph*gradVariMat;
			SetNegativeCoefficientsToZero(newVariMat);
			newEvalf = f.Call(newVariMat, fixedMat, X, delt, lambdS, lambdD);
			bit = ArmijoTest(sig, variMat, newVariMat, evalf, newEvalf, gradVariMat, alph);
			++count;
		}
		alph = alph*bet;
		newVariMat = variMat - alph*gradVariMat;
		SetNegativeCoefficientsToZero(newVariMat);
	}
	else
	{
		while (bit == false)
		{
			bet = pow(betinit, count);
			alph = alph*bet;
			newVariMat = variMat - alph*gradVariMat;
			SetNegativeCoefficientsToZero(newVariMat);
			newEvalf = f.Call(newVariMat, fixedMat, X, delt, lambdS, lambdD);
			bit = ArmijoTest(sig, variMat, newVariMat, evalf, newEvalf, gradVariMat, alph);
			++count;
		}
	}
	variMat = newVariMat;
}


//-------------------------------------------------------------------//
//---------------------------   MAIN   ------------------------------//
//-------------------------------------------------------------------//

int main(int argc, char * argv[])
{
	typedef itk::ImageRegionConstIterator<VectorImageType> 	ConstIteratorType;
	typedef itk::ImageRegionIterator<VectorImageType>		IteratorType;


	//---------------------   Input parameters   --------------------//

	// Reading input arguments
	const char * inputFilename = 	argv[1];
	const char * outputFilename =	argv[2];
	const int nbEndmembers = 		atoi(argv[3]);

	if (active_debug)
		std::cout << "Loading params: OK" << std::endl;

	// Stop value for the criterion
	const double critStopValue = 	0.005;

	// Tunning the optimized function parameters
	const double delt = 			1.;
	const double lambdD = 			0.01;
	const double lambdS = 			0.01 * (double) nbEndmembers;

	// Tunning the projected gradient parameters
	double sig = 0.05;
	double bet = 0.99;
	double alphA = 1.;
	double alphS = 1.;

	// Other declarations
	double critA, critS, crit = 1;
	int i;		// a counter

	if (active_debug)
		std::cout << "Loading all parameters: OK" << std::endl;

	// Definition of the region to unmix
	VectorImageType::RegionType::IndexType index;
	index[0] = 0;
	index[1] = 0;
	VectorImageType::RegionType::SizeType size;
	size[0] = 60;
	size[1] = 75;

	VectorImageType::RegionType region;
	region.SetIndex(index);
	region.SetSize(size);


	if (active_debug)
		std::cout << "Building region: OK" << std::endl;

	//-----   Setting the hsi spectral pixel into matrix columns ----//

	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(inputFilename);
	reader->GetOutput()->SetRegions(region);
	reader->GetOutput();
	reader->Update();

	ConstIteratorType inputIt(reader->GetOutput(), region);

	VnlMatrixType X;
	X.set_size(reader->GetOutput()->GetNumberOfComponentsPerPixel(), region.GetNumberOfPixels());

	i = 0;
	for (inputIt.GoToBegin(); !inputIt.IsAtEnd(); ++inputIt)
	{
		for (int j = 0; j<reader->GetOutput()->GetNumberOfComponentsPerPixel(); ++j)
		{
			X(j,i) = inputIt.Get().GetElement(j); //inputIt.Get()[j];
		}
		++i;
	}


	//-------   A and S declaration and initialization   --------//
	//---> These values fit with the ones chosen in the matlab "nmf_validationOtb.m" function to validate the OTB code.
	VnlMatrixType A;
	A.set_size(reader->GetOutput()->GetNumberOfComponentsPerPixel(), nbEndmembers);
	A.fill(100.);
	A.set_column(1,200.);
	A.set_column(2,300.);
	A.set_column(3,400.);
	A.set_column(4,500.);

	VnlMatrixType S;
	S.set_size(nbEndmembers, region.GetNumberOfPixels());
	S.fill(1.);



	//-----------   Declaration of useful variables   -----------//

	VnlMatrixType Sold, Sdiff;
	Sold.set_size(nbEndmembers, region.GetNumberOfPixels());
	Sdiff.set_size(nbEndmembers, region.GetNumberOfPixels());

	VnlMatrixType Aold, Adiff;
	Aold.set_size(reader->GetOutput()->GetNumberOfComponentsPerPixel(), nbEndmembers);
	Adiff.set_size(reader->GetOutput()->GetNumberOfComponentsPerPixel(), nbEndmembers);

	VnlMatrixType gradS;
	gradS.set_size(nbEndmembers, region.GetNumberOfPixels());

	VnlMatrixType gradA;
	gradA.set_size(reader->GetOutput()->GetNumberOfComponentsPerPixel(), nbEndmembers);



	//-----------------   Optimization loop   -------------------//

	FA fA;
	FS fS;

	std::cout << "normX = " << X.fro_norm() << std::endl;


	unsigned int counter = 0;

	while ((crit > critStopValue) && (counter < maxIter))
	{

		//----------------   Update S   -----------------//
		Sold = S;

	    evalGradS(X, A, S, delt, lambdS, gradS);	// Compute gradS

    	if (counter%divisorParam == 0)
	    {

    		std::cout << "Iteration = " << counter << std::endl;
    		std::cout << "Criterion = " << Criterion(X, A, S, delt, lambdS, lambdD) << std::endl;
    		std::cout << "statGradS = " << gradS.fro_norm() << std::endl;
    		std::cout << "gradS(0,0) = " << gradS(0,0) << std::endl;
    		std::cout << "alphS = " << alphS << std::endl;
    		std::cout << "normS = " << S.fro_norm() << std::endl;
    		std::cout << "S(0,0) = " << S(0,0) << std::endl;
	    }

		huckProjGradOneStep(X, A, gradS, fS, sig, bet, delt,lambdS, lambdD, S, alphS);

		if (counter%divisorParam == 0)
	    {
    		std::cout << "alphS = " << alphS << std::endl;
    		std::cout << "normS = " << S.fro_norm() << std::endl;
    		std::cout << "S(0,0) = " << S(0,0) << std::endl;
	    }


		//----------------   Update A   -----------------//
		Aold = A;

		evalGradA(X, A, S, delt, lambdD, gradA);	// Compute gradS

		if (counter%divisorParam == 0)
	    {
		    std::cout << "gradA(0,0) = " << gradA(0,0) << std::endl;
	    }


    	if (counter%divisorParam == 0)
	    {
    		std::cout << "alphA = " << alphA << std::endl;
    		std::cout << "normA = " << A.fro_norm() << std::endl;
    		std::cout << "A(0,0) = " << A(0,0) << std::endl;
	    }
		huckProjGradOneStep(X, S, gradA, fA, sig, bet, delt, lambdS, lambdD, A, alphA);

		if (counter%divisorParam == 0)
	    {
    		std::cout << "alphA = " << alphA << std::endl;
    		std::cout << "normA = " << A.fro_norm() << std::endl;
    		std::cout << "A(0,0) = " << A(0,0) << std::endl;
	    }



		//------------   crit evaluation   --------------//
	    Adiff = Aold-A;
	    critA = Adiff.absolute_value_max();
	    Sdiff = Sold - S;
	    critS = Sdiff.absolute_value_max();
	    crit = max(critA,critS);

    	if (counter%divisorParam == 0)
	    {
    		std::cout << "critA value: " <<  critA << std::endl;
    		std::cout << "critS value: " <<  critS << std::endl;
    		std::cout << "crit value: " <<  crit << std::endl;
    		std::cout << "criterion value: " <<  Criterion(X, A, S, delt, lambdS, lambdD) << std::endl;
    		std::cout << std::endl;
	    }

	    ++counter;
	}


	//---   Putting the rows of in the bands of the output vector image   ---//
	//---> Could be impoved choosing an imageList for the abundance maps and a vector list for the endmember spectra (columns of A).

	VectorImageType::RegionType::IndexType outputindex;
	outputindex[0] = 0;
	outputindex[1] = 0;
	VectorImageType::RegionType outputRegion;
	outputRegion.SetSize(size);
	outputRegion.SetIndex(outputindex);

	VectorImageType::Pointer outputImage = VectorImageType::New();
	outputImage->SetNumberOfComponentsPerPixel(nbEndmembers);
	outputImage->SetRegions(outputRegion);
	outputImage->Allocate();

	IteratorType outputIt(outputImage, outputRegion);

	VectorImageType::PixelType vectorPixel;
	vectorPixel.SetSize(outputImage->GetNumberOfComponentsPerPixel());


	i = 0;
	for (outputIt.GoToBegin();	!outputIt.IsAtEnd(); ++outputIt)
	{
		for (int j=0; j<nbEndmembers; ++j)
		{
			vectorPixel.SetElement(j, S(j,i));
		}
		outputIt.Set(vectorPixel);
		++i;
	}


	//------   Write the abundance map vector output image   -------//

	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName(outputFilename);
	writer->SetInput(outputImage);
	writer->Update();

	std::cout << "Number of iterations: " << counter << std::endl;


	std::cout << A << std::endl;


	return EXIT_SUCCESS;
}



