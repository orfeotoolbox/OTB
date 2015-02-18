      program call6Sv1_0b

C_otb MOD V2 : New outputs : 
C_otb	otb_tdif_up = Upward diffuse transmittance
C_otb 	otb_tdir_up = Upward direct transmittance
C_otb 	otb_tdif_up_ray = Upward diffuse transmittance for rayleigh
C_otb  	otb_tdif_up_aer = Upward diffuse transmittance for aerosols
	
	
      IMPLICIT NONE
      
      real wlinf,wlsup,s(1501)
      real asol,phi0,avis,phiv
      integer month,jday
      real uw,uo3,taer55
        
      real tgasm,sdtott,sutott,sast
      real sutotr,sutota
 
      integer iwave,iinf,isup,inhomo,idirec,iaer
      integer iread,i,ik,l
	
C_otb	Atmospheric parameters
      real otb_ratm	!Atmospheric reflectance
      real pressure     !Atmospheric pressure
      real otb_tdif_up  !Upward diffuse transmittance
      real otb_tdir_up !Upward direct transmittance
      real otb_tdif_up_ray !Upward diffuse transmittance for rayleigh
      real otb_tdif_up_aer !Upward diffuse transmittance for aerosols


c***********************************************************************
c   Parameters  initialization
c***********************************************************************  
      iread=5
      iinf=1
      isup=1501

      write(6,*) "------> Execution de l'interface d'appel de 6S "

 
c**********************************************************************c
c                                                                      c
c                                                *     sun             c
c                                              \ * /                   c
c                                            * * * * *                 c
c                                   z          / * \                   c
c                                   +           /*                     c
c            satellite    /         +          /                       c
c                       o/          +         /                        c
c                      /.\          +        /.                        c
c                     / . \  _avis-_+_-asol_/ .                        c
c                       .  \-      -+      /  .    north               c
c                       .   \       +     /   .  +                     c
c                       .    \      +    /    .+                       c
c                       .     \     +   /    +.                        c
c                       .      \    +  /   +  .                        c
c                       .       \   + /  +    .                        c
c                       .        \  +/ +      .                        c
c    west + + + + + + + . + + + + +\+ + + + + . + + + + + + + + east   c
c                       .          +..        .                        c
c                       .        + .   .      .                        c
c                       .      +  .      .    .                        c
c                       .    +   .       .'.  .                        c
c                       .  +    .. . , '     ..                        c
c                       .+     .       \       .                       c
c                      +.     .         \        .                     c
c                    +  .    .           \         .                   c
c             south     .   .       (phiv-phi0)                        c
c                                                                      c
c                                                                      c
c                                                                      c
c**********************************************************************c
 
c**********************************************************************c
c       igeom               geometrical conditions                     c
c               --------------------------------------                 c
c                                                                      c
c   you choose your own conditions; igeom=0                            c
c         0     enter solar zenith angle   (in degrees )               c
c                     solar azimuth angle        "                     c
c                     satellite zenith angle     "                     c
c                     satellite azimuth angle    "                     c
c                     month                                            c
c                     day of the month                                 c
c                                                                      c
c**********************************************************************c
      read(iread,*) asol,phi0,avis,phiv,month,jday
      write(6,*) "asol,phi0,avis,phiv,month,jday : "
      write(6,*)  asol,phi0,avis,phiv,month,jday
      write(6,*)
      
c**********************************************************************c
c       idatm      atmospheric model                                   c
c                 --------------------                                 c
c                 pressure (  in mb )                                  c
c                 uw  (in  g/cm2 )                                     c
c                 uo3 (in  cm-atm)                                     c
c                 profil is taken from us62                            c
c                                                                      c
c**********************************************************************c     
      read(iread,*) pressure, uw, uo3   
      write(6,*) "pressure, uw, uo3"
      write(6,*)  pressure, uw, uo3 
      write(6,*)  
  
c**********************************************************************c
c                                                                      c
c       iaer       aerosol model(type)                                 c
c                  --------------                                      c
c                                                                      c
c                                                                      c
c  you select one of the following standard aerosol models:            c
c         0  no aerosols                                               c
c         1  continental model  )                                      c
c         2  maritime model     )  according to sra models             c
c         3  urban model        )                                      c
c         5  shettle model for background desert aerosol               c
c                                                                      c
c**********************************************************************c
      read(iread,*) iaer
      write(6,*) "iaer = ", iaer
      write(6,*)
      
      if ((iaer.ne.0).and.(iaer.ne.1).and.(iaer.ne.2).and.
     c    (iaer.ne.3).and.(iaer.ne.5)) then 
          write(6,*) "Choix iaer non compatible : ", iaer
	  write(6,*) "Choisir 0, 1, 2, 3 ou 5 uniquement"
          goto 999
      endif
 

      read(iread,*) taer55
      write(6,*)  "taer55 = ", taer55
      write(6,*)
      
c**********************************************************************c
c      iwave input of the spectral conditions                          c
c            --------------------------------                          c
c                                                                      c
c         1  enter wlinf, wlsup and user's filter function s(lambda)   c
c                          ( by step of 0.0025 micrometer).            c
c                                                                      c
c**********************************************************************c
      do 38 l=iinf,isup
       s(l)=0.
   38 continue
      
  110 read(iread,*) wlinf,wlsup
      write(6,*) " wlinf,wlsup : ", wlinf,wlsup
      write(6,*)
      
      iinf=(wlinf-.25)/0.0025+1.5
      isup=(wlsup-.25)/0.0025+1.5
      do 1113 ik=iinf,isup
       s(ik)=0.
 1113 continue
      read(iread,*) (s(i),i=iinf,isup)
      
 
 

c**********************************************************************c
c                                                                      c
c                                                                      c
c                     start of computations                            c
c                                                                      c
c                                                                      c
c                                                                      c
c**********************************************************************c
c      goto 800
      
      call otb_6S(asol,phi0,avis,phiv,month,jday,
     I            pressure, uw, uo3,
     I            iaer, taer55,
     I            wlinf, wlsup, s,
     O            otb_ratm,sast,tgasm,sdtott,
     O            sutott, otb_tdif_up, otb_tdir_up,
     O		  otb_tdif_up_ray, otb_tdif_up_aer)


      write(6,*)
800   write(6,100) otb_ratm
      write(6,200) sast
      write(6,300) tgasm
      write(6,400) sdtott
      write(6,500) sutott
      write(6,600) otb_tdif_up
      write(6,700) otb_tdir_up
      write(6,710) otb_tdif_up_ray 
      write(6,720) otb_tdif_up_aer

100   format(10x,40h ----->  atmospheric reflectance :      , f6.5) 
200   format(10x,40h ----->  atmospheric spherical albedo : , f6.5)   
300   format(10x,40h ----->  total gaseous transmission :   , f6.5)   
400   format(10x,40h ----->  downward transmittance :       , f6.5)   
500   format(10x,40h ----->  upward transmittance :         , f6.5) 
600   format(10x,40h ----->  upward diffuse transmittance : , f6.5) 
700   format(10x,40h ----->  upward direct transmittance :  , f6.5) 
710   format(10x,40h ----->  upward diffuse transm. ray:    , f6.5) 
720   format(10x,40h ----->  upward diffuse transm. aer:    , f6.5)  
      
999   continue
      end
