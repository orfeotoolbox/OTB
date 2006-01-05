// Software Guide : BeginLatex
//
// Cet exemple permet d'afficher sur la sortie standard un petit message de 'Bonjour'
//
// Software Guide : EndLatex 
// Software Guide : BeginCodeSnippet
#include "otbBonjour.h"
#include <iostream>
#include <iostream>
#include <string>

int main()
{
	otb::Bonjour lBonjour;
	std::string lString = lBonjour.getMessage();
	std::cout << "Message : "<<lString<<std::endl;
	return 0;
}
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  Ce code permet d'afficher sur la sortie standard, le message enregistre dans la classe \code{otb::Bonjour}.
//
//  Software Guide : EndLatex 


