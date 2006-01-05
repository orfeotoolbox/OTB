%
% InsightSoftwareGuide.cls for the Insight documentation
%

\NeedsTeXFormat{LaTeX2e}[1995/12/01]
\ProvidesClass{InsightSoftwareGuide}
             [1998/03/03 Document class (Insight SoftwareGuide)]

%
%  Change this to say a4paper instead of letterpaper if you want A4.  These
%  are the latex defaults.
%
\newcommand{\itk@paper}{letterpaper}
\newcommand{\itk@ptsize}{10pt}


\newcommand{\itkcaption}[2][]{\caption[{#1}]{\small \textsf{{#2}} \normalsize}}
\newcommand{\itkpiccaption}[2][]{\piccaption[{#1}]{\small \textsf{{#2}} \normalsize}}


%  These set up the fonts for the documents.
%
%  The "times" package makes the default font the PostScript Times
%  font, which makes for smaller PostScript and a font that more people 
%  like.
%
%  The "avant" package causes the AvantGarde font to be used for
%  sans-serif text, instead of the uglier Helvetica set up by the "times"
%  package.
%
%\RequirePackage{avant}\typeout{Using AvantGarde instead of Computer Modern.}
\RequirePackage{times}\typeout{Using Times instead of Computer Modern.}


% Change the options here to get a different set of basic options, but only
% if you have to.  
%
\LoadClass[\itk@paper,\itk@ptsize,twoside,titlepage,openright]{book}

%\paperheight 9.50in
%\paperwidth  7.25in

\paperheight 9.375in
\paperwidth  7.25in

\topmargin 0.625in
\setlength{\voffset}{-0.625in}


% Increase printable page size (copied from fullpage.sty)
\advance \topmargin by -\headheight
\advance \topmargin by -\headsep

\oddsidemargin   0.625in
\evensidemargin -0.625in


\setcounter{secnumdepth}{2}

% Print out the values just for verification
\typeout{Dimension textheight  = \the\textheight}
\typeout{Dimension paperheight = \the\paperheight}
\typeout{Dimension topmargin   = \the\topmargin}


% Optional packages:
%
% If processing of these documents fails at your TeX installation,
% these may be commented out (independently) to make things work.
% These are both supplied with the current version of the teTeX
% distribution.
%
% The "fancyhdr" package makes nicer page footers reasonable to
% implement, and is used to put the chapter and section information in 
% the footers.
%
\RequirePackage{fancyhdr}\typeout{Using fancier footers than usual.}


% Required packages:
%
% The "fncychap" package is used to get the nice chapter headers.  The
% .sty file is distributed with Insight, so you should not need to disable
% it.  You'd also end up with a mixed page style; uglier than stock LaTeX!
%
\RequirePackage[Bjarne]{fncychap}\typeout{Using fancy chapter headings.}
% Do horizontal rules it this way to match:
\newcommand{\itk@doHorizontalRule}{\mghrulefill{\RW}}
%
%


% This gives us all the Insight-specific markup that we really want.
% This should come last.  Do not change this.
%
\RequirePackage{Insight}

% support for module synopsis sections:
\newcommand{\itk@ModSynopsisFilename}{\jobname\thechapter.syn}
\let\itk@OldChapter=\chapter
\renewcommand{\chapter}{
  \itk@ProcessModSynopsis
  \itk@closeModSynopsisFile
  \itk@OldChapter
}


% Change the title page to look a bit better, and fit in with the
% fncychap ``Bjarne'' style a bit better.
%
\renewcommand{\maketitle}{%
  \begin{titlepage}%
    \let\footnotesize\small
    \let\footnoterule\relax
    \itk@doHorizontalRule%
    \@ifundefined{pdfinfo}{}{{
      % This \def is required to deal with multi-line authors; it
      % changes \\ to ', ' (comma-space), making it pass muster for
      % generating document info in the PDF file.
      \def\\{, }
      \pdfinfo{
        /Author (\@author)
        /Title (\@title)
      }
    }}
    \begin{flushright}%
      {\rm\Huge\itk@HeaderFamily \@title \par}%
      {\em\LARGE\itk@HeaderFamily \itk@release \par}
      \vfill
      {\LARGE\itk@HeaderFamily \@author \par}
      \vfill\vfill
      {\large
       \@date \par
       \vfill
       \itk@authoraddress \par
      }%
    \end{flushright}%\par
    \@thanks
  \end{titlepage}%
  \setcounter{footnote}{0}%
  \let\thanks\relax\let\maketitle\relax
  \gdef\@thanks{}\gdef\@author{}\gdef\@title{}
}



% This wraps the \tableofcontents macro with all the magic to get the
% spacing right and have the right number of pages if the 'openright'
% option has been used.  This eliminates a fair amount of crud in the
% individual document files.
%
%\let\itk@OldTableofcontents=\tableofcontents
%\renewcommand{\tableofcontents}{%
%  \setcounter{page}{1}%
%  \pagebreak%
%  {%
%    \parskip = 0mm%
%    \itk@OldTableofcontents%
%    \if@openright%
%      \ifodd\value{page}%
%        \typeout{Adding blank page after the table of contents.}%
%        \pagebreak\hspace{0pt}%
%      \fi%
%    \fi%
%    \cleardoublepage%
%  }%
%  \itk@doing@page@targetstrue%
%}
% This is needed to get the width of the section # area wide enough in the
% library reference.  Doing it here keeps it the same for all the manuals.
%
\renewcommand*\l@section{\@dottedtocline{1}{1.5em}{2.6em}}
\renewcommand*\l@subsection{\@dottedtocline{2}{4.1em}{3.5em}}
\setcounter{tocdepth}{1}


% Fix the theindex environment to add an entry to the Table of
% Contents; this is much nicer than just having to jump to the end of
% the book and flip around, especially with multiple indexes.
%
\let\itk@OldTheindex=\theindex
\renewcommand{\theindex}{
  \cleardoublepage
  \itk@OldTheindex
  \addcontentsline{toc}{chapter}{\indexname}
}
