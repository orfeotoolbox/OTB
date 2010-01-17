dnl Note: this is the cppunit.m4 file copied from cppunit v1.12.1 
dnl available here: http://cppunit.sourceforge.net/.
dnl 
dnl 		  GNU LESSER GENERAL PUBLIC LICENSE
dnl 		       Version 2.1, February 1999
dnl 
dnl  Copyright (C) 1991, 1999 Free Software Foundation, Inc.
dnl      59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
dnl  Everyone is permitted to copy and distribute verbatim copies
dnl  of this license document, but changing it is not allowed.
dnl 
dnl [This is the first released version of the Lesser GPL.  It also counts
dnl  as the successor of the GNU Library Public License, version 2, hence
dnl  the version number 2.1.]
dnl 
dnl 			    Preamble
dnl 
dnl   The licenses for most software are designed to take away your
dnl freedom to share and change it.  By contrast, the GNU General Public
dnl Licenses are intended to guarantee your freedom to share and change
dnl free software--to make sure the software is free for all its users.
dnl 
dnl   This license, the Lesser General Public License, applies to some
dnl specially designated software packages--typically libraries--of the
dnl Free Software Foundation and other authors who decide to use it.  You
dnl can use it too, but we suggest you first think carefully about whether
dnl this license or the ordinary General Public License is the better
dnl strategy to use in any particular case, based on the explanations below.
dnl 
dnl   When we speak of free software, we are referring to freedom of use,
dnl not price.  Our General Public Licenses are designed to make sure that
dnl you have the freedom to distribute copies of free software (and charge
dnl for this service if you wish); that you receive source code or can get
dnl it if you want it; that you can change the software and use pieces of
dnl it in new free programs; and that you are informed that you can do
dnl these things.
dnl 
dnl   To protect your rights, we need to make restrictions that forbid
dnl distributors to deny you these rights or to ask you to surrender these
dnl rights.  These restrictions translate to certain responsibilities for
dnl you if you distribute copies of the library or if you modify it.
dnl 
dnl   For example, if you distribute copies of the library, whether gratis
dnl or for a fee, you must give the recipients all the rights that we gave
dnl you.  You must make sure that they, too, receive or can get the source
dnl code.  If you link other code with the library, you must provide
dnl complete object files to the recipients, so that they can relink them
dnl with the library after making changes to the library and recompiling
dnl it.  And you must show them these terms so they know their rights.
dnl 
dnl   We protect your rights with a two-step method: (1) we copyright the
dnl library, and (2) we offer you this license, which gives you legal
dnl permission to copy, distribute and/or modify the library.
dnl 
dnl   To protect each distributor, we want to make it very clear that
dnl there is no warranty for the free library.  Also, if the library is
dnl modified by someone else and passed on, the recipients should know
dnl that what they have is not the original version, so that the original
dnl author's reputation will not be affected by problems that might be
dnl introduced by others.
dnl 
dnl   Finally, software patents pose a constant threat to the existence of
dnl any free program.  We wish to make sure that a company cannot
dnl effectively restrict the users of a free program by obtaining a
dnl restrictive license from a patent holder.  Therefore, we insist that
dnl any patent license obtained for a version of the library must be
dnl consistent with the full freedom of use specified in this license.
dnl 
dnl   Most GNU software, including some libraries, is covered by the
dnl ordinary GNU General Public License.  This license, the GNU Lesser
dnl General Public License, applies to certain designated libraries, and
dnl is quite different from the ordinary General Public License.  We use
dnl this license for certain libraries in order to permit linking those
dnl libraries into non-free programs.
dnl 
dnl   When a program is linked with a library, whether statically or using
dnl a shared library, the combination of the two is legally speaking a
dnl combined work, a derivative of the original library.  The ordinary
dnl General Public License therefore permits such linking only if the
dnl entire combination fits its criteria of freedom.  The Lesser General
dnl Public License permits more lax criteria for linking other code with
dnl the library.
dnl 
dnl   We call this license the "Lesser" General Public License because it
dnl does Less to protect the user's freedom than the ordinary General
dnl Public License.  It also provides other free software developers Less
dnl of an advantage over competing non-free programs.  These disadvantages
dnl are the reason we use the ordinary General Public License for many
dnl libraries.  However, the Lesser license provides advantages in certain
dnl special circumstances.
dnl 
dnl   For example, on rare occasions, there may be a special need to
dnl encourage the widest possible use of a certain library, so that it becomes
dnl a de-facto standard.  To achieve this, non-free programs must be
dnl allowed to use the library.  A more frequent case is that a free
dnl library does the same job as widely used non-free libraries.  In this
dnl case, there is little to gain by limiting the free library to free
dnl software only, so we use the Lesser General Public License.
dnl 
dnl   In other cases, permission to use a particular library in non-free
dnl programs enables a greater number of people to use a large body of
dnl free software.  For example, permission to use the GNU C Library in
dnl non-free programs enables many more people to use the whole GNU
dnl operating system, as well as its variant, the GNU/Linux operating
dnl system.
dnl 
dnl   Although the Lesser General Public License is Less protective of the
dnl users' freedom, it does ensure that the user of a program that is
dnl linked with the Library has the freedom and the wherewithal to run
dnl that program using a modified version of the Library.
dnl 
dnl   The precise terms and conditions for copying, distribution and
dnl modification follow.  Pay close attention to the difference between a
dnl "work based on the library" and a "work that uses the library".  The
dnl former contains code derived from the library, whereas the latter must
dnl be combined with the library in order to run.
dnl 
dnl 		  GNU LESSER GENERAL PUBLIC LICENSE
dnl    TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
dnl 
dnl   0. This License Agreement applies to any software library or other
dnl program which contains a notice placed by the copyright holder or
dnl other authorized party saying it may be distributed under the terms of
dnl this Lesser General Public License (also called "this License").
dnl Each licensee is addressed as "you".
dnl 
dnl   A "library" means a collection of software functions and/or data
dnl prepared so as to be conveniently linked with application programs
dnl (which use some of those functions and data) to form executables.
dnl 
dnl   The "Library", below, refers to any such software library or work
dnl which has been distributed under these terms.  A "work based on the
dnl Library" means either the Library or any derivative work under
dnl copyright law: that is to say, a work containing the Library or a
dnl portion of it, either verbatim or with modifications and/or translated
dnl straightforwardly into another language.  (Hereinafter, translation is
dnl included without limitation in the term "modification".)
dnl 
dnl   "Source code" for a work means the preferred form of the work for
dnl making modifications to it.  For a library, complete source code means
dnl all the source code for all modules it contains, plus any associated
dnl interface definition files, plus the scripts used to control compilation
dnl and installation of the library.
dnl 
dnl   Activities other than copying, distribution and modification are not
dnl covered by this License; they are outside its scope.  The act of
dnl running a program using the Library is not restricted, and output from
dnl such a program is covered only if its contents constitute a work based
dnl on the Library (independent of the use of the Library in a tool for
dnl writing it).  Whether that is true depends on what the Library does
dnl and what the program that uses the Library does.
dnl   
dnl   1. You may copy and distribute verbatim copies of the Library's
dnl complete source code as you receive it, in any medium, provided that
dnl you conspicuously and appropriately publish on each copy an
dnl appropriate copyright notice and disclaimer of warranty; keep intact
dnl all the notices that refer to this License and to the absence of any
dnl warranty; and distribute a copy of this License along with the
dnl Library.
dnl 
dnl   You may charge a fee for the physical act of transferring a copy,
dnl and you may at your option offer warranty protection in exchange for a
dnl fee.
dnl 
dnl   2. You may modify your copy or copies of the Library or any portion
dnl of it, thus forming a work based on the Library, and copy and
dnl distribute such modifications or work under the terms of Section 1
dnl above, provided that you also meet all of these conditions:
dnl 
dnl     a) The modified work must itself be a software library.
dnl 
dnl     b) You must cause the files modified to carry prominent notices
dnl     stating that you changed the files and the date of any change.
dnl 
dnl     c) You must cause the whole of the work to be licensed at no
dnl     charge to all third parties under the terms of this License.
dnl 
dnl     d) If a facility in the modified Library refers to a function or a
dnl     table of data to be supplied by an application program that uses
dnl     the facility, other than as an argument passed when the facility
dnl     is invoked, then you must make a good faith effort to ensure that,
dnl     in the event an application does not supply such function or
dnl     table, the facility still operates, and performs whatever part of
dnl     its purpose remains meaningful.
dnl 
dnl     (For example, a function in a library to compute square roots has
dnl     a purpose that is entirely well-defined independent of the
dnl     application.  Therefore, Subsection 2d requires that any
dnl     application-supplied function or table used by this function must
dnl     be optional: if the application does not supply it, the square
dnl     root function must still compute square roots.)
dnl 
dnl These requirements apply to the modified work as a whole.  If
dnl identifiable sections of that work are not derived from the Library,
dnl and can be reasonably considered independent and separate works in
dnl themselves, then this License, and its terms, do not apply to those
dnl sections when you distribute them as separate works.  But when you
dnl distribute the same sections as part of a whole which is a work based
dnl on the Library, the distribution of the whole must be on the terms of
dnl this License, whose permissions for other licensees extend to the
dnl entire whole, and thus to each and every part regardless of who wrote
dnl it.
dnl 
dnl Thus, it is not the intent of this section to claim rights or contest
dnl your rights to work written entirely by you; rather, the intent is to
dnl exercise the right to control the distribution of derivative or
dnl collective works based on the Library.
dnl 
dnl In addition, mere aggregation of another work not based on the Library
dnl with the Library (or with a work based on the Library) on a volume of
dnl a storage or distribution medium does not bring the other work under
dnl the scope of this License.
dnl 
dnl   3. You may opt to apply the terms of the ordinary GNU General Public
dnl License instead of this License to a given copy of the Library.  To do
dnl this, you must alter all the notices that refer to this License, so
dnl that they refer to the ordinary GNU General Public License, version 2,
dnl instead of to this License.  (If a newer version than version 2 of the
dnl ordinary GNU General Public License has appeared, then you can specify
dnl that version instead if you wish.)  Do not make any other change in
dnl these notices.
dnl 
dnl   Once this change is made in a given copy, it is irreversible for
dnl that copy, so the ordinary GNU General Public License applies to all
dnl subsequent copies and derivative works made from that copy.
dnl 
dnl   This option is useful when you wish to copy part of the code of
dnl the Library into a program that is not a library.
dnl 
dnl   4. You may copy and distribute the Library (or a portion or
dnl derivative of it, under Section 2) in object code or executable form
dnl under the terms of Sections 1 and 2 above provided that you accompany
dnl it with the complete corresponding machine-readable source code, which
dnl must be distributed under the terms of Sections 1 and 2 above on a
dnl medium customarily used for software interchange.
dnl 
dnl   If distribution of object code is made by offering access to copy
dnl from a designated place, then offering equivalent access to copy the
dnl source code from the same place satisfies the requirement to
dnl distribute the source code, even though third parties are not
dnl compelled to copy the source along with the object code.
dnl 
dnl   5. A program that contains no derivative of any portion of the
dnl Library, but is designed to work with the Library by being compiled or
dnl linked with it, is called a "work that uses the Library".  Such a
dnl work, in isolation, is not a derivative work of the Library, and
dnl therefore falls outside the scope of this License.
dnl 
dnl   However, linking a "work that uses the Library" with the Library
dnl creates an executable that is a derivative of the Library (because it
dnl contains portions of the Library), rather than a "work that uses the
dnl library".  The executable is therefore covered by this License.
dnl Section 6 states terms for distribution of such executables.
dnl 
dnl   When a "work that uses the Library" uses material from a header file
dnl that is part of the Library, the object code for the work may be a
dnl derivative work of the Library even though the source code is not.
dnl Whether this is true is especially significant if the work can be
dnl linked without the Library, or if the work is itself a library.  The
dnl threshold for this to be true is not precisely defined by law.
dnl 
dnl   If such an object file uses only numerical parameters, data
dnl structure layouts and accessors, and small macros and small inline
dnl functions (ten lines or less in length), then the use of the object
dnl file is unrestricted, regardless of whether it is legally a derivative
dnl work.  (Executables containing this object code plus portions of the
dnl Library will still fall under Section 6.)
dnl 
dnl   Otherwise, if the work is a derivative of the Library, you may
dnl distribute the object code for the work under the terms of Section 6.
dnl Any executables containing that work also fall under Section 6,
dnl whether or not they are linked directly with the Library itself.
dnl 
dnl   6. As an exception to the Sections above, you may also combine or
dnl link a "work that uses the Library" with the Library to produce a
dnl work containing portions of the Library, and distribute that work
dnl under terms of your choice, provided that the terms permit
dnl modification of the work for the customer's own use and reverse
dnl engineering for debugging such modifications.
dnl 
dnl   You must give prominent notice with each copy of the work that the
dnl Library is used in it and that the Library and its use are covered by
dnl this License.  You must supply a copy of this License.  If the work
dnl during execution displays copyright notices, you must include the
dnl copyright notice for the Library among them, as well as a reference
dnl directing the user to the copy of this License.  Also, you must do one
dnl of these things:
dnl 
dnl     a) Accompany the work with the complete corresponding
dnl     machine-readable source code for the Library including whatever
dnl     changes were used in the work (which must be distributed under
dnl     Sections 1 and 2 above); and, if the work is an executable linked
dnl     with the Library, with the complete machine-readable "work that
dnl     uses the Library", as object code and/or source code, so that the
dnl     user can modify the Library and then relink to produce a modified
dnl     executable containing the modified Library.  (It is understood
dnl     that the user who changes the contents of definitions files in the
dnl     Library will not necessarily be able to recompile the application
dnl     to use the modified definitions.)
dnl 
dnl     b) Use a suitable shared library mechanism for linking with the
dnl     Library.  A suitable mechanism is one that (1) uses at run time a
dnl     copy of the library already present on the user's computer system,
dnl     rather than copying library functions into the executable, and (2)
dnl     will operate properly with a modified version of the library, if
dnl     the user installs one, as long as the modified version is
dnl     interface-compatible with the version that the work was made with.
dnl 
dnl     c) Accompany the work with a written offer, valid for at
dnl     least three years, to give the same user the materials
dnl     specified in Subsection 6a, above, for a charge no more
dnl     than the cost of performing this distribution.
dnl 
dnl     d) If distribution of the work is made by offering access to copy
dnl     from a designated place, offer equivalent access to copy the above
dnl     specified materials from the same place.
dnl 
dnl     e) Verify that the user has already received a copy of these
dnl     materials or that you have already sent this user a copy.
dnl 
dnl   For an executable, the required form of the "work that uses the
dnl Library" must include any data and utility programs needed for
dnl reproducing the executable from it.  However, as a special exception,
dnl the materials to be distributed need not include anything that is
dnl normally distributed (in either source or binary form) with the major
dnl components (compiler, kernel, and so on) of the operating system on
dnl which the executable runs, unless that component itself accompanies
dnl the executable.
dnl 
dnl   It may happen that this requirement contradicts the license
dnl restrictions of other proprietary libraries that do not normally
dnl accompany the operating system.  Such a contradiction means you cannot
dnl use both them and the Library together in an executable that you
dnl distribute.
dnl 
dnl   7. You may place library facilities that are a work based on the
dnl Library side-by-side in a single library together with other library
dnl facilities not covered by this License, and distribute such a combined
dnl library, provided that the separate distribution of the work based on
dnl the Library and of the other library facilities is otherwise
dnl permitted, and provided that you do these two things:
dnl 
dnl     a) Accompany the combined library with a copy of the same work
dnl     based on the Library, uncombined with any other library
dnl     facilities.  This must be distributed under the terms of the
dnl     Sections above.
dnl 
dnl     b) Give prominent notice with the combined library of the fact
dnl     that part of it is a work based on the Library, and explaining
dnl     where to find the accompanying uncombined form of the same work.
dnl 
dnl   8. You may not copy, modify, sublicense, link with, or distribute
dnl the Library except as expressly provided under this License.  Any
dnl attempt otherwise to copy, modify, sublicense, link with, or
dnl distribute the Library is void, and will automatically terminate your
dnl rights under this License.  However, parties who have received copies,
dnl or rights, from you under this License will not have their licenses
dnl terminated so long as such parties remain in full compliance.
dnl 
dnl   9. You are not required to accept this License, since you have not
dnl signed it.  However, nothing else grants you permission to modify or
dnl distribute the Library or its derivative works.  These actions are
dnl prohibited by law if you do not accept this License.  Therefore, by
dnl modifying or distributing the Library (or any work based on the
dnl Library), you indicate your acceptance of this License to do so, and
dnl all its terms and conditions for copying, distributing or modifying
dnl the Library or works based on it.
dnl 
dnl   10. Each time you redistribute the Library (or any work based on the
dnl Library), the recipient automatically receives a license from the
dnl original licensor to copy, distribute, link with or modify the Library
dnl subject to these terms and conditions.  You may not impose any further
dnl restrictions on the recipients' exercise of the rights granted herein.
dnl You are not responsible for enforcing compliance by third parties with
dnl this License.
dnl 
dnl   11. If, as a consequence of a court judgment or allegation of patent
dnl infringement or for any other reason (not limited to patent issues),
dnl conditions are imposed on you (whether by court order, agreement or
dnl otherwise) that contradict the conditions of this License, they do not
dnl excuse you from the conditions of this License.  If you cannot
dnl distribute so as to satisfy simultaneously your obligations under this
dnl License and any other pertinent obligations, then as a consequence you
dnl may not distribute the Library at all.  For example, if a patent
dnl license would not permit royalty-free redistribution of the Library by
dnl all those who receive copies directly or indirectly through you, then
dnl the only way you could satisfy both it and this License would be to
dnl refrain entirely from distribution of the Library.
dnl 
dnl If any portion of this section is held invalid or unenforceable under any
dnl particular circumstance, the balance of the section is intended to apply,
dnl and the section as a whole is intended to apply in other circumstances.
dnl 
dnl It is not the purpose of this section to induce you to infringe any
dnl patents or other property right claims or to contest validity of any
dnl such claims; this section has the sole purpose of protecting the
dnl integrity of the free software distribution system which is
dnl implemented by public license practices.  Many people have made
dnl generous contributions to the wide range of software distributed
dnl through that system in reliance on consistent application of that
dnl system; it is up to the author/donor to decide if he or she is willing
dnl to distribute software through any other system and a licensee cannot
dnl impose that choice.
dnl 
dnl This section is intended to make thoroughly clear what is believed to
dnl be a consequence of the rest of this License.
dnl 
dnl   12. If the distribution and/or use of the Library is restricted in
dnl certain countries either by patents or by copyrighted interfaces, the
dnl original copyright holder who places the Library under this License may add
dnl an explicit geographical distribution limitation excluding those countries,
dnl so that distribution is permitted only in or among countries not thus
dnl excluded.  In such case, this License incorporates the limitation as if
dnl written in the body of this License.
dnl 
dnl   13. The Free Software Foundation may publish revised and/or new
dnl versions of the Lesser General Public License from time to time.
dnl Such new versions will be similar in spirit to the present version,
dnl but may differ in detail to address new problems or concerns.
dnl 
dnl Each version is given a distinguishing version number.  If the Library
dnl specifies a version number of this License which applies to it and
dnl "any later version", you have the option of following the terms and
dnl conditions either of that version or of any later version published by
dnl the Free Software Foundation.  If the Library does not specify a
dnl license version number, you may choose any version ever published by
dnl the Free Software Foundation.
dnl 
dnl   14. If you wish to incorporate parts of the Library into other free
dnl programs whose distribution conditions are incompatible with these,
dnl write to the author to ask for permission.  For software which is
dnl copyrighted by the Free Software Foundation, write to the Free
dnl Software Foundation; we sometimes make exceptions for this.  Our
dnl decision will be guided by the two goals of preserving the free status
dnl of all derivatives of our free software and of promoting the sharing
dnl and reuse of software generally.
dnl 
dnl 			    NO WARRANTY
dnl 
dnl   15. BECAUSE THE LIBRARY IS LICENSED FREE OF CHARGE, THERE IS NO
dnl WARRANTY FOR THE LIBRARY, TO THE EXTENT PERMITTED BY APPLICABLE LAW.
dnl EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR
dnl OTHER PARTIES PROVIDE THE LIBRARY "AS IS" WITHOUT WARRANTY OF ANY
dnl KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE
dnl IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
dnl PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE
dnl LIBRARY IS WITH YOU.  SHOULD THE LIBRARY PROVE DEFECTIVE, YOU ASSUME
dnl THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.
dnl 
dnl   16. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN
dnl WRITING WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY
dnl AND/OR REDISTRIBUTE THE LIBRARY AS PERMITTED ABOVE, BE LIABLE TO YOU
dnl FOR DAMAGES, INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR
dnl CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OR INABILITY TO USE THE
dnl LIBRARY (INCLUDING BUT NOT LIMITED TO LOSS OF DATA OR DATA BEING
dnl RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD PARTIES OR A
dnl FAILURE OF THE LIBRARY TO OPERATE WITH ANY OTHER SOFTWARE), EVEN IF
dnl SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
dnl DAMAGES.
dnl 
dnl 		     END OF TERMS AND CONDITIONS
dnl 
dnl            How to Apply These Terms to Your New Libraries
dnl 
dnl   If you develop a new library, and you want it to be of the greatest
dnl possible use to the public, we recommend making it free software that
dnl everyone can redistribute and change.  You can do so by permitting
dnl redistribution under these terms (or, alternatively, under the terms of the
dnl ordinary General Public License).
dnl 
dnl   To apply these terms, attach the following notices to the library.  It is
dnl safest to attach them to the start of each source file to most effectively
dnl convey the exclusion of warranty; and each file should have at least the
dnl "copyright" line and a pointer to where the full notice is found.
dnl 
dnl     <one line to give the library's name and a brief idea of what it does.>
dnl     Copyright (C) <year>  <name of author>
dnl 
dnl     This library is free software; you can redistribute it and/or
dnl     modify it under the terms of the GNU Lesser General Public
dnl     License as published by the Free Software Foundation; either
dnl     version 2.1 of the License, or (at your option) any later version.
dnl 
dnl     This library is distributed in the hope that it will be useful,
dnl     but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl     Lesser General Public License for more details.
dnl 
dnl     You should have received a copy of the GNU Lesser General Public
dnl     License along with this library; if not, write to the Free Software
dnl     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 
dnl     USA
dnl 
dnl Also add information on how to contact you by electronic and paper mail.
dnl 
dnl You should also get your employer (if you work as a programmer) or your
dnl school, if any, to sign a "copyright disclaimer" for the library, if
dnl necessary.  Here is a sample; alter the names:
dnl 
dnl   Yoyodyne, Inc., hereby disclaims all copyright interest in the
dnl   library `Frob' (a library for tweaking knobs) written by James Random
dnl   Hacker.
dnl 
dnl   <signature of Ty Coon>, 1 April 1990
dnl   Ty Coon, President of Vice
dnl 
dnl That's all there is to it!

dnl
dnl AM_PATH_CPPUNIT(MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
dnl
AC_DEFUN([AM_PATH_CPPUNIT],
[

AC_ARG_WITH(cppunit-prefix,[  --with-cppunit-prefix=PFX   Prefix where CppUnit is installed (optional)],
            cppunit_config_prefix="$withval", cppunit_config_prefix="")
AC_ARG_WITH(cppunit-exec-prefix,[  --with-cppunit-exec-prefix=PFX  Exec prefix where CppUnit is installed (optional)],
            cppunit_config_exec_prefix="$withval", cppunit_config_exec_prefix="")

  if test x$cppunit_config_exec_prefix != x ; then
     cppunit_config_args="$cppunit_config_args --exec-prefix=$cppunit_config_exec_prefix"
     if test x${CPPUNIT_CONFIG+set} != xset ; then
        CPPUNIT_CONFIG=$cppunit_config_exec_prefix/bin/cppunit-config
     fi
  fi
  if test x$cppunit_config_prefix != x ; then
     cppunit_config_args="$cppunit_config_args --prefix=$cppunit_config_prefix"
     if test x${CPPUNIT_CONFIG+set} != xset ; then
        CPPUNIT_CONFIG=$cppunit_config_prefix/bin/cppunit-config
     fi
  fi

  AC_PATH_PROG(CPPUNIT_CONFIG, cppunit-config, no)
  cppunit_version_min=$1

  AC_MSG_CHECKING(for Cppunit - version >= $cppunit_version_min)
  no_cppunit=""
  if test "$CPPUNIT_CONFIG" = "no" ; then
    AC_MSG_RESULT(no)
    no_cppunit=yes
  else
    CPPUNIT_CFLAGS=`$CPPUNIT_CONFIG --cflags`
    CPPUNIT_LIBS=`$CPPUNIT_CONFIG --libs`
    cppunit_version=`$CPPUNIT_CONFIG --version`

    cppunit_major_version=`echo $cppunit_version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    cppunit_minor_version=`echo $cppunit_version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    cppunit_micro_version=`echo $cppunit_version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`

    cppunit_major_min=`echo $cppunit_version_min | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    if test "x${cppunit_major_min}" = "x" ; then
       cppunit_major_min=0
    fi
    
    cppunit_minor_min=`echo $cppunit_version_min | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    if test "x${cppunit_minor_min}" = "x" ; then
       cppunit_minor_min=0
    fi
    
    cppunit_micro_min=`echo $cppunit_version_min | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    if test "x${cppunit_micro_min}" = "x" ; then
       cppunit_micro_min=0
    fi

    cppunit_version_proper=`expr \
        $cppunit_major_version \> $cppunit_major_min \| \
        $cppunit_major_version \= $cppunit_major_min \& \
        $cppunit_minor_version \> $cppunit_minor_min \| \
        $cppunit_major_version \= $cppunit_major_min \& \
        $cppunit_minor_version \= $cppunit_minor_min \& \
        $cppunit_micro_version \>= $cppunit_micro_min `

    if test "$cppunit_version_proper" = "1" ; then
      AC_MSG_RESULT([$cppunit_major_version.$cppunit_minor_version.$cppunit_micro_version])
    else
      AC_MSG_RESULT(no)
      no_cppunit=yes
    fi
  fi

  if test "x$no_cppunit" = x ; then
     ifelse([$2], , :, [$2])     
  else
     CPPUNIT_CFLAGS=""
     CPPUNIT_LIBS=""
     ifelse([$3], , :, [$3])
  fi

  AC_SUBST(CPPUNIT_CFLAGS)
  AC_SUBST(CPPUNIT_LIBS)
])



