#Contact: Pierre Lassalle <lassallepierre34@gmail.com> 
otb_fetch_module(otbGRM
  "This module provides the GRM OTB application to perform multiscale
region-merging segmentation on satellite images. Three local homogeneity
criteria are available: the Baatz & Schäpe criterion, the Full Lambda Schedule
criterion and the simple Euclidean Distance criterion.  This application uses
the GRM (Generic Region Merging) library which allows quickly adding a new local
homogeneity criterion. Look at the template header file: GRMSegmenterTemplate.h
to see which format you must respect to add a new criterion.

A more detailed description can be found on the project website:
http://tully.ups-tlse.fr/lassallep/grm
"
  GIT_REPOSITORY https://github.com/orfeotoolbox/GRM
  GIT_TAG 209161cc0b3fadad7999e80b83838868f430289e
)
