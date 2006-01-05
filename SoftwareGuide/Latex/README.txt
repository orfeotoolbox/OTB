Style Guidelines for the ITK Software Guide
-------------------------------------------

1. Make sure code is formatted according to ITK style guidelines; especially
   note the use of {} and two-space indentation:

   for ()
     {
     yabba++;
     dabba++;
     do++;
     }

2. \doxygen{} - The first occurence of a class in a section should use this
   (or for those in sub-name spaces like itk::Statistics::) use \subdoxygen{}.

   Subsequent references to the class name should just use plain text (do not
   use \code{} or \doxygen{}). This is for readibility (too much font changing
   drives the reader nuts).

3. \code{} - Method names (which should have a trailing (), e.g., SetSigma());
   reserved C++ words (for, while, switch, etc.); simple types (char,
   unsigned char, float, etc.); code fragments

4. Section Naming - Use upper case for most words with the exception of
   articles and prepositions (e.g., The Holy Iterator and ITK)

5. Begin Latex sections in code with:
  // Software Guide : BeginLatex
  //
  include the extra blank line with //, it makes formatting easier. End with
  //
  // Software Guide : EndLatex

6. Include code snippets in code with:
  // Software Guide : BeginCodeSnippet
  // Software Guide : EndCodeSnippet
  Anything inside these gets put into the document; avoid blank lines or too
  much whitespace. Make sure any comments in included code are correct in
  terms of grammar, spelling, and are complete sentences.
  Note that the code should not exced column 80 or it will go out of
  margins in the PDF document.

7. Figure/table captions should be proper sentences and end in a period
   ".".

8. Specific proper nouns like Figure 1 or Table 2 should be capitalized. A
   general, non-specific noun ("referring to the figure above") should not
   be capitalized. Try to use the ~ symbol to connect Figure~\ref{} to
   avoid bad page breaks.

9. Don't forget to index. You may need hyphenation directives in long
   class/method names, etc.
   Avoid the use of the {textbf} at the end of the index. It looks nice on 
   the printed version but breaks the hyperlinks in the PDF online version.

10. When possible, a short paragraph on motivation/theory prior to an example
    is good. It's not a theory book, so balance the amount of material
    carefully.
