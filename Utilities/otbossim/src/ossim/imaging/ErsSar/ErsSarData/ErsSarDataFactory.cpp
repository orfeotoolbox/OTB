#include <ossim/imaging/ErsSar/ErsSarData/ErsSarDataFactory.h>

#include <ossim/imaging/ErsSar/ErsSarData/ErsSarImageOptionsFileDescriptor.h>


ErsSarDataFactory::ErsSarDataFactory()
{
	RegisterRecord(0, new ErsSarImageOptionsFileDescriptor());
	RegisterRecord(1, new ErsSarImageOptionsFileDescriptor());
}

ErsSarDataFactory::~ErsSarDataFactory()
{

}