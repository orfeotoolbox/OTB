      program ssssss
 
c**********************************************************************c
c                                                                      c
c                                                                      c
c                                                                      c
c                                                                      c
c                                                                      c
c       ********************************************************       c
c       *           second simulation of satellite signal      *       c
c       *                 in the solar spectrum                *       c
c       *           ... (6sV) ....... (6sV) ...... (6sV) ...   *       c
c       *                        version  1.0B                 *       c
c       *                                                      *       c
c       *                        Vector Code                   *       c
c       *                                                      *       c
c       *  this code predicts the satellite signal from 0.25   *       c
c       *  to 4.0 microns assuming cloudless atmosphere.       *       c
c       *  the main atmospheric effects (gaseous absorption    *       c
c       *  by water vapor,carbon dioxyde,oxygen and ozone;     *       c
c       *  scattering by molecules and aerosols) are taken     *       c
c       *  into account. non-uniform surfaces may be           *       c
c       *  considered,as well as bidirectional reflectances    *       c
c       *            as boundary conditions                    *       c
c       *                                                      *       c
c       *   the following input parameters are needed          *       c
c       *         geometrical conditions                       *       c
c       *         atmospheric model for gaseous components     *       c
c       *         aerosol model (type and concentration)       *       c
c       *         spectral condition                           *       c
c       *         ground reflectance (type and spectral var.)  *       c
c       *   at each step, you can either select some proposed  *       c
c       *  standard conditions (for example,spectral bands of  *       c
c       *  satellite for spectral conditions) or define your   *       c
c       *  own conditions(in the example,you have to define    *       c
c       *  the assumed spectral response).                     *       c
c       *                                                      *       c
c       *   more details are given at each data input step     *       c
c       *                                                      *       c
c       ********************************************************       c
c                                                                      c
c                                                                      c
c                                                                      c
c                                                                      c
c                                                                      c
c**********************************************************************c
 
 
 
 
 
 
c**********************************************************************c
c                                                                      c
c                                                                      c
c       ********************************************************       c
c       *             The authors of this code are             *       c
c       *                                                      *       c
c       *            (1) Vermote E and Kotchenova.S.;          *       c
c       *            (2) Roger J.C..;                          *       c
c       *            (3) Tanre D.; Deuze J.L; Herman M.;       *       c
c       *            (4) Morcrette J.J..                       *       c
c       *                                                      *       c
c       *                       from                           *       c
c       *                                                      *       c
c       *     (1) Affiliation: Department of Geography         *       c
c       *         University of Maryland and                   *       c
c       *         Goddard Space Flight Center	               *       c
c       *         Code 614.5    		      	       *       c
c       *         USA                                          *       c
c       *                                                      *       c
c       *     (2) ELICO/LOCL                                   *       c
c       *         Universite du Littoral Cote d'Opale          *       c
c       *         32 Ave Foch, 62930 Wimereux                  *       c
c       *         France                                       *       c
c       *                                                      *       c
c       *     (3) laboratoire d' optique atmospherique         *       c
c       *         universite des sciences et techniques        *       c
c       *         de lille                                     *       c
c       *         u.e.r. de physique fondamentale              *       c
c       *         59655 villeneuve d' ascq cedex               *       c
c       *         france                                       *       c
c       *                                                      *       c
c       *     (4) e.c.m.w.f.                                   *       c
c       *                                                      *       c
c       *                                                      *       c
c       *                                                      *       c
c       *                                                      *       c
c       ********************************************************       c
c                                                                      c
c                                                                      c
c**********************************************************************c
 
 
c**********************************************************************c
c       ********************************************************       c
c       *                limits of validity                    *       c
c       *                                                      *       c
c       *   geometrical parameters    no limitations           *       c
c       *                                                      *       c
c       *   atmospheric model         no limitations           *       c
c       *                                                      *       c
c       *   aerosol model             the visibility must be   *       c
c       *                             better than 5.0km        *       c
c       *                             for smaller values       *       c
c       *                             calculations might be    *       c
c       *                             no more valid.           *       c
c       *                                                      *       c
c       *   spectral conditions       the gaseous transmittance*       c
c       *                             and the scattering func  *       c
c       *                             tions are valid from 0.25*       c
c       *                             to 4.0 micron. but the   *       c
c       *                             treatment of interaction *       c
c       *                             between absorption and   *       c
c       *                             scattering is correct for*       c
c       *                             not too large absorption *       c
c       *                             if you want to compute   *       c
c       *                             signal within absorption *       c
c       *                             bands,this interaction   *       c
c       *                             ought to be reconsidered *       c
c       *                                                      *       c
c       *   ground reflectance (type) you can consider a patchy*       c
c       *                             structure:that is a circu*       c
c       *                             lar target of radius rad *       c
c       *                             and of reflectance roc,  *       c
c       *                             within an environnement  *       c
c       *                             of reflectance roe.      *       c
c       *                                                      *       c
c       *   ground reflectance (type continued): for uniform   *       c
c       *                             surface conditions only, *       c
c       *                             you may consider directio*       c
c       *                             nal reflectance as bounda*       c
c       *                             ry conditions.           *       c
c       *                             some analytical model are*       c
c       *                             proposed, the user can   *       c
c       *                             specify his own values.  *       c
c       *                             the code assumes that the*       c
c       *                             brdf is spectrally inde- *       c
c       *                             pendent                  *       c
c       *                                                      *       c
c       *   ground reflectance (spectral variation) four typi  *       c
c       *                             cal reflectances are pro *       c
c       *                             posed, defined within    *       c
c       *                             given spectral range.    *       c
c       *                             this range differs accor *       c
c       *                             ding to the selected case*       c
c       *                             the reflectance is set to*       c
c       *                             0 outside this range,due *       c
c       *                             to the deficiency of data*       c
c       *                             user must verify these   *       c
c       *                             limits. that is obviously*       c
c       *                             irrelevant for brdf      *       c
c       *                                                      *       c
c       ********************************************************       c
c**********************************************************************c
 
c****************************************************************************c
c  for considering brdf< we have to compute the downward radiance in the     c
c  whole hemisphere. to perform such computions, we selected the successive  c
c  orders of scattering method. that method requires numerical integration   c
c  over angles and optical depth. the integration method is the gauss method,c
c  mu is the number of angles nmu+1, nmu is settled to 24. the accuracy of   c
c  the computations is obviously depending on the nmu value. this value      c
c  can be easily changed as a parameter as well as the nt value which        c
c  is the number of layers for performing the vertical integration. the      c
c  downward radiance is computed for nmu values of the zenith angle and np   c
c  values of the azimuth angle. the integration of the product of the        c
c  radiance by the brdf is so performed over the nmu*np values. np is settledc
c  to 13, that value can be also changed. mu2 is equal to 2 times nmu.       c
c  xlmus is the downward radiance, xf the downward irradiance, rm and gb     c
c  the angles and the weights for the gauss integration over the zenith, rp  c
c  and gp respectively for the azimuth integration.                          c
c****************************************************************************c

      include "paramdef.inc"
      dimension anglem(mu2_p),weightm(mu2_p),
     s   rm(-mu_p:mu_p),gb(-mu_p:mu_p),rp(np_p),gp(np_p)
      dimension  xlmus(-mu_p:mu_p,np_p),xlmuv(-mu_p:mu_p,np_p)
      dimension angmu(10),angphi(13),brdfints(-mu_p:mu_p,np_p)
     s    ,brdfdats(10,13),sbrdftmp(-1:1,1),sbrdf(1501),
     s     srm(-1:1),srp(1),
     s    brdfintv(-mu_p:mu_p,np_p),brdfdatv(10,13),robar(1501),
     s    robarp(1501),robard(1501),xlm1(-mu_p:mu_p,np_p),
     s    xlm2(-mu_p:mu_p,np_p)
        real romix_fi(nfi_p),rorayl_fi(nfi_p),ratm2_fi(nfi_p),
     s       refet_fi(nfi_p),roatm_fi(3,20,nfi_p),xlphim(nfi_p)
     
        real rolut(mu_p,41),roluts(20,mu_p,41),roluti(mu_p,41)
        real rolutq(mu_p,41),rolutsq(20,mu_p,41),rolutiq(mu_p,41)
        real rolutu(mu_p,41),rolutsu(20,mu_p,41),rolutiu(mu_p,41)
	real filut(mu_p,41)
	integer aerod
	real its,lutmuv,luttv,iscama,iscami,scaa,cscaa,cfi
	integer nfilut(mu_p),nbisca
	real dtr 
        real anglem,weightm,rm,gb,accu2,accu3
        real rp,gp,xlmus,xlmuv,angmu,angphi,brdfints,brdfdats
        real brdfintv,brdfdatv,robar,robarp,robard,xlm1,xlm2
        real c,wldisc,ani,anr,aini,ainr,rocl,roel,zpl,ppl,tpl,whpl
        real wopl,xacc,s,wlinf,wlsup,delta
	real nwlinf,nwlsup
	integer niinf,nisup
        real sigma,z,p,t,wh,wo,ext,ome,gasym,phase,qhase,roatm,dtdir
        real dtdif,utdir,utdif,sphal,wldis,trayl,traypl,pi,pi2,step
        real asol,phi0,avis,phiv,tu,xlon,xlat,xlonan,hna,dsol,campm
        real phi,phirad,xmus,xmuv,xmup,xmud,adif,uw,uo3,taer55
        real taer,v,xps,uwus,uo3us,xpp,taer55p,puw,puo3,puwus
        real puo3us,wl,wlmoy,tamoy,tamoyp,pizmoy,pizmoyp,trmoy
        real trmoyp,fr,rad,spalt,sha,sham,uhase
        real albbrdf,par1,par2,par3,par4,robar1,xnorm1,rob,xnor,rodir
        real rdown,rdir,robar2,xnorm2,ro,roc,roe,rapp,rocave,roeave
        real seb,sbor,swl,sb,refet,refet1,refet2,refet3,alumet
	real refeti,pinst,ksiinst,ksirad
        real rpfet,rpfet1,rpfet2,rpfet3,plumet,plumeas
        real tgasm,rog,dgasm,ugasm,sdwava,sdozon,sddica,sdoxyg
        real sdniox,sdmoca,sdmeth,suwava,suozon,sudica,suoxyg
        real suniox,sumoca,sumeth,stwava,stozon,stdica,stoxyg,stniox
        real stmoca,stmeth,sodray,sodaer,sodtot,fophsr,fophsa,sroray
        real sroaer,srotot,ssdaer,sdtotr,sdtota,sdtott,sutotr,sutota
        real sutott,sasr,sasa,sast,dtozon,dtdica,dtoxyg
        real dtniox,dtmeth,dtmoca,utozon,utdica,utoxyg,utniox
        real utmeth,utmoca,attwava,ttozon,ttdica,ttoxyg,ttniox
        real ttmeth,ttmoca,dtwava,utwava,ttwava,coef,romix,rorayl
        real roaero,phaa,phar,tsca,tray,trayp,taerp,dtott,utott
	real rqmix,rqrayl,rqaero,qhaa,qhar,foqhsr,foqhsa,foqhst
	real rumix,rurayl,ruaero,uhaa,uhar,rpmix,rpaero,rprayl
	real srpray,srpaer,srptot,rpmeas1,rpmeas2,rpmeas3
	real srqray,srqaer,srqtot,sruray,sruaer,srutot
        real astot,asray,asaer,utotr,utota,dtotr,dtota,dgtot,tgtot
        real tgp1,tgp2,rqatm,ruatm,fouhst,fouhsr,fouhsa,coefp
        real ugtot,edifr,edifa,tdird,tdiru,tdifd,tdifu,fra
        real fae,avr,romeas1,romeas2,romeas3,alumeas,sodrayp
        real sdppray,sdppaer,sdpptot,rop,sdpray,sdpaer,sdptot
	real spdpray,spdpaer,spdptot
        real ratm1,ratm2,ratm3,rsurf,rpatm1,rpatm2,rpatm3,rpsurf
        real sodaerp,sodtotp,tdir,tdif,etn,esn,es,ea0n,ea0,ee0n
        real ee0,tmdir,tmdif,xla0n,xla0,xltn,xlt,xlen,xle,pizera
        real fophst,pizerr,pizert,xrad,xa,xb,xc
        integer nt,mu,mu2,np,k,iwr,mum1,idatmp,ipol
        integer j,iread,l,igeom,month,jday,nc,nl,idatm,iaer,iaerp,n
        integer iwave,iinf,isup,ik,i,inhomo
        integer idirec,ibrdf,igroun
        integer igrou1,igrou2,isort,irapp,ilut
c variables used in the BRDF coupling correction process
	real robarstar,robarpstar,robarbarstar,tdd,tdu,tsd,tsu
	real coefa,coefb,coefc,discri,rogbrdf	
c variables related to surface polarization
        integer irop
	real ropq,ropu,pveg,wspd,azw,razw


c***********************************************************************
c                 to vary the number of quadratures
c***********************************************************************
      integer nquad
      common /num_quad/ nquad 

c***********************************************************************
c                     the aerosol profile
c***********************************************************************
      integer iaer_prof,num_z
      real alt_z,taer_z,taer55_z,total_height,height_z(0:nt_p_max)
      common/aeroprof/num_z,alt_z(0:nt_p_max),taer_z(0:nt_p_max),
     &taer55_z(0:nt_p_max)
      character aer_model(15)*50
      
c***********************************************************************
c                             return to 6s
c***********************************************************************
      dimension c(4),wldisc(20),ani(2,3),anr(2,3),aini(2,3),ainr(2,3)
      dimension rocl(1501),roel(1501)
      real rfoaml(1501),rglitl(1501),rwatl(1501)
      real rn,ri,x1,x2,x3,cij,rsunph,nrsunph,rmax,rmin,cij_out(4)
      integer icp,irsunph,i1,i2
      character etiq1(8)*60,nsat(119)*17,atmid(7)*51,reflec(8)*71
      character FILE*80,FILE2*80
      logical ier
      integer igmax

      common/sixs_ier/iwr,ier
      common /mie_in/ rmax,rmin,icp,rn(20,4),ri(20,4),x1(4),x2(4),
     s x3(4),cij(4),irsunph,rsunph(50),nrsunph(50)
      common /multorder/ igmax
c***********************************************************************
c     for considering pixel and sensor  altitude
c***********************************************************************
      real pps,palt,ftray
      common /sixs_planesim/zpl(34),ppl(34),tpl(34),whpl(34),wopl(34)
      common /sixs_test/xacc
c***********************************************************************
c     for considering aerosol and brdf
c***********************************************************************

      integer options(5)
      integer pild,pihs
      real optics(3),struct(4)
      real pxLt,pc,pRl,pTl,pRs
      real pws,phi_wind,xsal,pcl,paw,rfoam,rwat,rglit
      real rfoamave,rwatave,rglitave
      
      real uli,eei,thmi,sli,cabi,cwi,vaii,rnci,rsl1i
      real p1,p2,p3
c***********************************************************************
c                             return to 6s
c***********************************************************************
      common /sixs_ffu/s(1501),wlinf,wlsup
      common /sixs_del/ delta,sigma
      common /sixs_atm/z(34),p(34),t(34),wh(34),wo(34)
      common /sixs_aer/ext(20),ome(20),gasym(20),phase(20),qhase(20),
     suhase(20)
      common /sixs_disc/ roatm(3,20),dtdir(3,20),dtdif(3,20),
     s utdir(3,20),utdif(3,20),sphal(3,20),wldis(20),trayl(20),
     s traypl(20),rqatm(3,20),ruatm(3,20)
 
 
c****************************************************************************c
c   angmu and angphi are the angles were the brdf is measured. these values  c
c   can be changed as soon as they are well distributed over the whole space c
c   before the gauss integration, these values are interpolated to the gauss c
c   angles                                                                   c
c****************************************************************************c
      data angmu /85.0,80.0,70.0,60.0,50.0,40.0,30.0,20.0,10.0,0.00/
      data angphi/0.00,30.0,60.0,90.0,120.0,150.0,180.0,
     s          210.0,240.0,270.0,300.0,330.0,360.0/
 
c***********************************************************************
c                             return to 6s
c***********************************************************************
      data wldisc /0.350,0.400,0.412,0.443,0.470,0.488,0.515,0.550,
     s             0.590,0.633,0.670,0.694,0.760,0.860,1.240,1.536,
     s             1.650,1.950,2.250,3.750/
       
 
      data etiq1/
     s '(1h*,22x,34h user defined conditions          ,t79,1h*)',
     s '(1h*,22x,24h meteosat observation   ,t79,1h*)          ',
     s '(1h*,22x,25h goes east observation   ,t79,1h*)         ',
     s '(1h*,22x,25h goes west observation   ,t79,1h*)         ',
     s '(1h*,22x,30h avhrr (AM noaa) observation  ,t79,1h*)    ',
     s '(1h*,22x,30h avhrr (PM noaa) observation  ,t79,1h*)    ',
     s '(1h*,22x,24h h.r.v.   observation   ,t79,1h*)          ',
     s '(1h*,22x,24h t.m.     observation   ,t79,1h*)          '/
 
       data nsat/
     s ' constant        ',' user s          ',
     s ' meteosat        ',' goes east       ',' goes west       ',
     s ' avhrr 1 (noaa6) ',' avhrr 2 (noaa6) ',
     s ' avhrr 1 (noaa7) ',' avhrr 2 (noaa7) ',
     s ' avhrr 1 (noaa8) ',' avhrr 2 (noaa8) ',
     s ' avhrr 1 (noaa9) ',' avhrr 2 (noaa9) ',
     s ' avhrr 1 (noaa10)',' avhrr 2 (noaa10)',
     s ' avhrr 1 (noaa11)',' avhrr 2 (noaa11)',
     s ' hrv1 1          ',' hrv1 2          ',' hrv1 3          ',
     s ' hrv1 pan        ',
     s ' hrv2 1          ',' hrv2 2          ',' hrv2 3          ',
     s ' hrv2 pan        ',
     s '  tm  1          ','  tm  2          ','  tm  3          ',
     s '  tm  4          ','  tm  5          ','  tm  7          ',
     s '  mss 4          ','  mss 5          ',
     s '  mss 6          ','  mss 7          ',
     s '  mas 1          ','  mas 2          ','  mas 3          ',
     s '  mas 4          ','  mas 5          ','  mas 6          ',
     s '  mas 7          ','  modis 1        ','  modis 2        ',
     s '  modis 3        ','  modis 4        ','  modis 5        ',
     s '  modis 6        ','  modis 7        ','  modis 8        ',
     s ' avhrr 1 (noaa12)',' avhrr 2 (noaa12)',
     s ' avhrr 1 (noaa14)',' avhrr 2 (noaa14)',
     s ' polder 1        ',' polder 2        ',
     s ' polder 3        ',' polder 4        ',' polder 5        ',
     s ' polder 6        ',' polder 7        ',' polder 8        ',
     s ' seawifs 1       ',' seawifs 2       ',
     s ' seawifs 3       ',' seawifs 4       ',' seawifs 5       ',
     s ' seawifs 6       ',' seawifs 7       ',' seawifs 8       ',
     s ' aatsr   1       ',' aatsr   2       ',' aatsr   3       ',
     s ' aatsr   4       ',' meris   1       ',' meris   2       ',
     s ' meris   3       ',' meris   4       ',' meris   5       ',
     s ' meris   6       ',' meris   7       ',' meris   8       ',
     s ' meris   9       ',' meris   10      ',' meris   11      ',
     s ' meris   12      ',' meris   13      ',' meris   14      ',
     s ' meris   15      ',' gli     1       ',' gli     2       ',
     s ' gli     3       ',' gli     4       ',' gli     5       ',
     s ' gli     6       ',' gli     7       ',' gli     8       ',
     s ' gli     9       ',' gli     10      ',' gli     11      ',
     s ' gli     12      ',' gli     13      ',' gli     14      ',
     s ' gli     15      ',' gli     16      ',' gli     17      ',
     s ' gli     18      ',' gli     19      ',' gli     20      ',
     s ' gli     21      ',' gli     22      ',' gli     23      ',
     s ' gli     24      ',' gli     25      ',' gli     26      ',
     s ' gli     27      ',' gli     28      ',' gli     29      ',
     s ' gli     30      '/
 
       data atmid /
     s 'no absorption computed                             ',
     s 'tropical            (uh2o=4.12g/cm2,uo3=.247cm-atm)',
     s 'midlatitude summer  (uh2o=2.93g/cm2,uo3=.319cm-atm)',
     s 'midlatitude winter  (uh2o=.853g/cm2,uo3=.395cm-atm)',
     s 'subarctic  summer   (uh2o=2.10g/cm2,uo3=.480cm-atm)',
     s 'subarctic  winter   (uh2o=.419g/cm2,uo3=.480cm-atm)',
     s 'us  standard 1962   (uh2o=1.42g/cm2,uo3=.344cm-atm)'/
 
      data  reflec /
     & '(1h*,12x,39h user defined spectral reflectance     ,f6.3,t79
     & ,1h*) ',
     & '(1h*,12x,27h monochromatic reflectance ,f6.3,t79,1h*)',
     & '(1h*,12x,39h constant reflectance over the spectra ,f6.3,t79
     & ,1h*) ',
     & '(1h*,12x,39h spectral vegetation ground reflectance,f6.3,t79
     & ,1h*) ',
     & '(1h*,12x,39h spectral clear water reflectance      ,f6.3,t79
     & ,1h*) ',
     & '(1h*,12x,39h spectral dry sand ground reflectance  ,f6.3,t79
     & ,1h*) ',
     & '(1h*,12x,39h spectral lake water reflectance       ,f6.3,t79
     & ,1h*) ',
     & '(1h*,12x,39h spectral volcanic debris reflectance  ,f6.3,t79
     & ,1h*) '/

      FILE='  '
      FILE2='  '

c***********************************************************************
c   Parameters  initialization
c***********************************************************************
      nt=nt_p
      mu=mu_p
      mu2=mu2_p
      np=np_p
      nfi=nfi_p
      iwr=6
      ier=.FALSE.
      iinf=1
      isup=1501
      igmax=20
c***********************************************************************
c  preliminary computations for gauss integration
c***********************************************************************
      pi=acos(-1.)
      pi2=2*pi
      accu2=1.E-03
      accu3=1.E-07
      do k=1,13
       angphi(k)=angphi(k)*pi/180.
      enddo
      do k=1,10
       angmu(k)=cos(angmu(k)*pi/180.)
      enddo
      call gauss(-1.,1.,anglem,weightm,mu2)
      call gauss(0.,pi2,rp,gp,np)
      mum1=mu-1
      do 581 j=-mum1,-1
       k=mu+j
       rm(-j-mu)=anglem(k)
       gb(-j-mu)=weightm(k)
  581 continue
      do 582 j=1,mum1
       k=mum1+j
       rm(mu-j)=anglem(k)
       gb(mu-j)=weightm(k)
  582 continue
      gb(-mu)=0.
      gb(0)=0.
      gb(mu)=0.
 
c***********************************************************************
c                             return to 6s
c***********************************************************************
c constantes values
      sigma=0.056032
      delta=0.0279
CCC     pinst=0.02
CCC     ksiinst=0.
      xacc=1.e-06
      iread=5
      step=0.0025
      do 1111 l=1,20
       wldis(l)=wldisc(l)
 1111 continue
 
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
c                                                                      c
c   you choose your own conditions; igeom=0                            c
c         0     enter solar zenith angle   (in degrees )               c
c                     solar azimuth angle        "                     c
c                     satellite zenith angle     "                     c
c                     satellite azimuth angle    "                     c
c                     month                                            c
c                     day of the month                                 c
c                                                                      c
c   or you select one of the following satellite conditions:igeom=1to7 c
c         1       meteosat observation                                 c
c                 enter month,day,decimal hour (universal time-hh.ddd) c
c                       n. of column,n. of line.(full scale 5000*2500) c
c                                                                      c
c         2       goes east observation                                c
c                 enter month,day,decimal hour (universal time-hh.ddd) c
c                      n. of column,n. of line.(full scale 17000*12000)c
c                                                                      c
c         3       goes west observation                                c
c                 enter month,day,decimal hour (universal time-hh.ddd) c
c                      n. of column,n. of line.(full scale 17000*12000)c
c                                                                      c
c         4       avhrr ( PM noaa )                                    c
c                 enter month,day,decimal hour (universal time-hh.ddd) c
c                       n. of column(1-2048),xlonan,hna                c
c                       give long.(xlonan) and overpass hour (hna) at  c
c                       the ascendant node at equator                  c
c                                                                      c
c         5       avhrr ( AM noaa )                                    c
c                 enter month,day,decimal hour (universal time-hh.ddd) c
c                       n. of column(1-2048),xlonan,hna                c
c                       give long.(xlonan) and overpass hour (hna) at  c
c                       the ascendant node at equator                  c
c                                                                      c
c         6       hrv   ( spot )    * enter month,day,hh.ddd,long.,lat.c
c                                                                      c
c         7       tm    ( landsat ) * enter month,day,hh.ddd,long.,lat.c
c                                                                      c
c                                                                      c
c     note:       for hrv and tm experiments long. and lat. are the    c
c                 coordinates of the scene center.                     c
c                 lat. must be > 0 for north lat., < 0 for south lat.  c
c                 long. must be > 0 for east long., <0 for west long.  c
c                                                                      c
c                 solar and viewing positions are computed             c
c                                                                      c
c**********************************************************************c

      read(iread,*) igeom
 
      if (igeom.lt.0) then
          if (igeom.lt.-10) then
	     igmax=int(abs(igeom/10))
	     igeom=igeom+igmax*10
	  endif   
          ilut=0
	  igeom=0
      endif
      ilut=0	  
      goto(1001,1002,1003,1004,1005,1006,1007),igeom
c   igeom=0.....

      read(iread,*) asol,phi0,avis,phiv,month,jday

      goto 22
c
 1001 read(iread,*) month,jday,tu,nc,nl
      call posmto(month,jday,tu,nc,nl,
     1            asol,phi0,avis,phiv,xlon,xlat)
      goto 22
 1002 read(iread,*) month,jday,tu,nc,nl
      call posge(month,jday,tu,nc,nl,
     1           asol,phi0,avis,phiv,xlon,xlat)
      goto 22
 1003 read(iread,*) month,jday,tu,nc,nl
      call posgw(month,jday,tu,nc,nl,
     1           asol,phi0,avis,phiv,xlon,xlat)
      goto 22
 1004 read(iread,*) month,jday,tu,nc,xlonan,hna
      campm=1.0
      call posnoa(month,jday,tu,nc,xlonan,hna,campm,
     1            asol,phi0,avis,phiv,xlon,xlat)
      goto 22
 1005 read(iread,*) month,jday,tu,nc,xlonan,hna
      campm=-1.0
      call posnoa(month,jday,tu,nc,xlonan,hna,campm,
     1            asol,phi0,avis,phiv,xlon,xlat)
      goto 22
 1006 read(iread,*) month,jday,tu,xlon,xlat
      call posspo(month,jday,tu,xlon,xlat,
     a            asol,phi0,avis,phiv)
      goto 22
 1007 read(iread,*) month,jday,tu,xlon,xlat
      call poslan(month,jday,tu,xlon,xlat,
     s            asol,phi0,avis,phiv)
   22 continue

      if(ier) stop
      dsol=1.
      call varsol(jday,month,dsol)

c**********************************************************************c
c                                                                      c
c                                 / scattered direction                c
c                               /                                      c
c                             /                                        c
c                           / adif                                     c
c    incident   + + + + + + + + + + + + + + +                          c
c    direction                                                         c
c                                                                      c
c**********************************************************************c
      phi=abs(phiv-phi0)
      phirad=(phi0-phiv)*pi/180.
      if (phirad.lt.0.) phirad=phirad+2.*pi
      if (phirad.gt.(2.*pi)) phirad=phirad-2.*pi
      xmus=cos(asol*pi/180.)
      xmuv=cos(avis*pi/180.)
      xmup=cos(phirad)
      xmud=-xmus*xmuv-sqrt(1.-xmus*xmus)*sqrt(1.-xmuv*xmuv)*xmup
c test vermote bug
      if (xmud.gt.1.) xmud=1.
      if (xmud.lt.-1.) xmud=-1.
      adif=acos(xmud)*180./pi

 
c**********************************************************************c
c       idatm      atmospheric model                                   c
c                 --------------------                                 c
c                                                                      c
c                                                                      c
c  you select one of the following standard atmosphere: idatm=0 to 6   c
c         0    no gaseous absorption                                   c
c         1    tropical                )                               c
c         2    midlatitude summer      )                               c
c         3    midlatitude winter      )                               c
c         4    subarctic summer        )      from lowtran             c
c         5    subarctic winter        )                               c
c         6    us standard 62          )                               c
c                                                                      c
c  or you define your own atmospheric model idatm=7 or 8               c
c         7    user profile  (radiosonde data on 34 levels)            c
c              enter altitude       (  in km )                         c
c                    pressure       (  in mb )                         c
c                    temperature    (  in k  )                         c
c                    h2o density    (in  g/m3)                         c
c                    o3  density    (in  g/m3)                         c
c                                                                      c
c           for example, altitudes are  from  0 to 25km step of 1km    c
c                        from 25 to 50km step of 5km                   c
c                        and two values at 70km and 100km              c
c                        so you have 34*5 values to input.             c
c         8    enter water vapor and ozone contents                    c
c                 uw  (in  g/cm2 )                                     c
c                 uo3 (in  cm-atm)                                     c
c                 profil is taken from us62                            c
c                                                                      c
c**********************************************************************c
      uw=0.
      uo3=0.

      read(iread,*) idatm


      if(idatm.eq.0) go to 5
      if(idatm.eq.8) read(iread,*) uw,uo3
      if(idatm.ne.7) go to 6
      do 7 k=1,34
       read(iread,*) z(k),p(k),t(k),wh(k),wo(k)
    7 continue
      go to 5
    6 if(idatm.eq.1)  call tropic
      if(idatm.eq.2)  call midsum
      if(idatm.eq.3)  call midwin
      if(idatm.eq.4)  call subsum
      if(idatm.eq.5)  call subwin
      if(idatm.eq.6)  call us62
c     we have to define an atmosphere to compute rayleigh optical depth
    5 if(idatm.eq.0.or.idatm.eq.8)  call us62

c**********************************************************************c
c      THIS OPTION IS NOT AVAILABLE THE CODE RUNS WITH IPOL=1          c
c       ipol       computation of the atmospheric polarization         c
c                  -------------------------------------------         c
c                                                                      c
c**********************************************************************c

c      read(iread,*) ipol
       ipol=1
       
c**********************************************************************c
c                                                                      c
c       iaer       aerosol model(type) and profile                     c
c                  --------------                                      c
c      iaer = -1  The user-defined profile. You have to input the      c
c                 number of layers first, then the height (km),        c
c                 optical thickness (at 550 nm), and the type of       c
c                 aerosol (see below) for each layer, starting from    c 
c                 the ground. The present version of the program       c
c                 works only with the same type of aerosol for each    c
c                 layer.                                               c
c                                                                      c
c                 Example for iaer = -1:                               c
c                 4                                                    c
c                 2.0 0.200 1                                          c
c                 10.0 0.025 1                                         c
c                 8.0 0.003 1                                          c
c                 80.0 0.000 1                                         c
c                                                                      c
c   The maximum total height of all layers cannot exceed 300 km.       c
c                                                                      c
c     If you do not put iaer = -1, the program will use the default    c
c     exponential profile. In this case, you need to select one of     c 
c     the following standard aerosol models:                           c
c                                                                      c
c  iaer = 0  no aerosols                                               c
c         1  continental model  )                                      c
c         2  maritime model     )  according to sra models             c
c         3  urban model        )                                      c
c         5  shettle model for background desert aerosol               c
c         6  biomass burning                                           c
c         7  stratospheric model                                       c
c                                                                      c
c  or you define your own model using basic components: iaer=4         c
c         4 enter the volumic percentage of each component             c
c                 c(1) = volumic % of dust-like                        c
c                 c(2) = volumic % of water-soluble                    c
c                 c(3) = volumic % of oceanic                          c
c                 c(4) = volumic % of soot                             c
c                   between 0 to 1                                     c
c                                                                      c
c  or you define your own model using size distribution function:      c 
c         8  Multimodal Log Normal distribution (up to 4 modes)        c
c         9  Modified gamma  distribution                              c
c        10  Junge Power-Law distribution                              c
c                                                                      c
c  or you define a model using sun-photometer measurements:            c
c        11  Sun Photometer  distribution (50 values max)              c
c             you have to enter:  r and d V / d (logr)                 c
c                  where r is the radius (in micron) and V the volume  c
c                  and d V / d (logr) in (cm3/cm2/micron)              c
c             and then you have to enter: nr and ni for each wavelengthc
c                  where nr and ni are respectively the real and       c
c                  imaginary part of the refractive index              c
c                                                                      c
c  or you can use results computed and previously saved                c
c        12  Reading of data previously saved into FILE                c
c             you have to enter the identification name FILE in the    c
c             next line of inputs.                                     c
c                                                                      c
c                                                                      c
c  iaerp and FILE  aerosol model(type)-Printing of results             c
c                  ---------------------------------------             c
c                                                                      c
c For iaer=8,9,10,and 11:                                              c
c    results from the MIE subroutine may be saved into the file        c
c    FILE.mie (Extinction and scattering coefficients, single          c
c    scattering albedo, Asymmetry parameter, phase function at         c
c    predefined wavelengths) and then can be re-used with the          c 
c    option iaer=12 where FILE is an identification name you           c
c    have to enter.                                                    c
c                                                                      c
c    So, if you select iaer=8,9,10,or 11, next line following the      c
c    requested inputs by the options 8,9,10, or 11 you have to enter   c
c    iaerp                                                             c
c                                                                      c
c        iaerp=0    results will not be saved                          c
c        iaerp=1    results will be saved into the file FILE.mie       c
c                    next line enter FILE                              c
c                                                                      c
c                                                                      c
c   example for iaer and iaerp                                         c
c 8                      Multimodal Log-Normale distribution selected  c
c 0.0001 100.0 3         Rmin, Rmax, 3 components                      c
c 0.5000 2.99 1.66E-7    Rmean, Sigma, percentage density-1st componentc
c 1.53 1.53 1.53 1.53 1.53 1.53 1.52 1.40 1.22 1.27  nr-10 wavelengths c 
c .008 .008 .008 .008 .008 .008 .008 .008 .009 .011  ni-10 wavelengths c
c 0.0050 2.99 0.5945     Rmean, Sigma, percentage density-2nd componentc
c 1.53 1.53 1.53 1.53 1.53 1.53 1.52 1.51 1.42 1.452 nr-10 wavelengths c
c .005 .005 .005 .005 .006 .007 .012 .023 .010 .004  ni-10 wavelengths c
c 0.0118 2.00 0.4055     Rmean, Sigma, percentage density-3rd componentc
c 1.75 1.75 1.75 1.75 1.75 1.75 1.75 1.77 1.81 1.90  nr-10 wavelengths c
c .46  .45  .45  .44  .43  .43  .43  .46  .50  .57   ni-10 wavelengths c
c 1                      Results will be saved into FILE.mie           c
c URBAN-WCP112           Identification of the output file called FILE c
c                    -> results will be saved into URBAN-WCP112.mie    c
c                                                                      c
c**********************************************************************c
      rmin=0.
      rmax=0.
      icp=1
      do i=1,4
       x1(i)=0.0
       x2(i)=0.0
       x3(i)=0.0
       do l=1,20
        rn(l,i)=0.0
        ri(l,i)=0.0
       enddo
      enddo
      do i=1,50
       rsunph(i)=0.
       nrsunph(i)=0.
      enddo
      cij(1)=1.00
      
      taer=0.
      taer55=0.
      iaer_prof=0
 
      read(iread,*) iaer
      
c  the user-defined aerosol profile
      if (iaer.lt.0) then

      total_height=0.0
      iaer_prof=1
      num_z=0
      do i=0,50
      alt_z(i)=0.0
      taer55_z(i)=0.0
      taer_z(i)=0.0
      height_z(i)=0.0
      enddo

      read(5,*) num_z

      do i=0,num_z-1
       read(5,*) height_z(num_z-i),taer55_z(num_z-i),iaer
       alt_z(num_z-1-i)=total_height+height_z(num_z-i)
       total_height=total_height+height_z(num_z-i)
       taer55=taer55+taer55_z(num_z-i)
      enddo
                
      endif
c  the user-defined aerosol profile
      
      if (iaer.ge.0.and.iaer.le.7) nquad=nqdef_p
      if (iaer.ge.8.and.iaer.le.11) nquad=nquad_p

      if(iaer.eq.4) read(iread,*) (c(n),n=1,4)
      
      goto(49,40,41,42,49,49,49,49,43,44,45,46,47),iaer+1
 
   40 c(1)=0.70
      c(2)=0.29
      c(3)=0.00
      c(4)=0.01 
      go to 49
   41 c(1)=0.00
      c(2)=0.05
      c(3)=0.95
      c(4)=0.00     
      go to 49
   42 c(1)=0.17
      c(2)=0.61
      c(3)=0.00
      c(4)=0.22
      go to 49
   43 read(iread,*) rmin,rmax,icp
      do i=1,icp
       read(5,*)x1(i),x2(i),cij(i)
       read(5,*)(rn(l,i),l=1,20)
       read(5,*)(ri(l,i),l=1,20)
      enddo
        do i=1,icp
         cij_out(i)=cij(i)
        enddo
      go to 49
   44 read(iread,*) rmin,rmax
      read(iread,*) x1(1),x2(1),x3(1)
      read(5,*)(rn(l,1),l=1,20)
      read(5,*)(ri(l,1),l=1,20)
      go to 49
   45 read(iread,*) rmin,rmax
      read(iread,*) x1(1)
      read(5,*)(rn(l,1),l=1,20)
      read(5,*)(ri(l,1),l=1,20)
      go to 49
   46 read(5,*)irsunph
      do i=1,irsunph
       read(5,*)rsunph(i),nrsunph(i)
C       nrsunph(i)=nrsunph(i)/(rsunph(i)**4.)/(4*3.1415/3)
      enddo
      rmin=rsunph(1)
      rmax=rsunph(irsunph)+1e-07
      read(5,*)(rn(l,1),l=1,20)
      read(5,*)(ri(l,1),l=1,20)
      go to 49
   47 read(5,'(A80)')FILE2
      i2=index(FILE2,' ')-1
      go to 49

   49 continue

      if (iaer.ge.8.and.iaer.le.11)then
       read(5,*)iaerp
       if (iaerp.eq.1)read(5,'(A80)')FILE
       i1=index(FILE,' ')-1
       FILE2=FILE(1:I1)//'.mie'
       i2=index(FILE2,' ')-1
      endif

      call aeroso(iaer,c,xmud,wldis,FILE2,ipol)


c**********************************************************************c
c                 aerosol model (concentration)                        c
c                 ----------------------------                         c
c             (only for the default exponential profile)               c
c                                                                      c
c  v             if you have an estimate of the meteorological         c
c                parameter: the visibility v, enter directly the       c
c                value of v in km (the aerosol optical depth will      c
c                be computed from a standard aerosol profile)          c
c                                                                      c
c  v=0, taer55   if you have an estimate of aerosol optical depth ,    c
c                enter v=0 for the visibility and enter the aerosol    c
c                optical depth at 550                                  c
c                                                                      c
c  v=-1          warning:  if iaer=0, enter v=-1                       c
c                                                                      c
c**********************************************************************c

      if (iaer_prof.eq.0) then

      read(iread,*) v
      if(v) 71,10,11
   10 read(iread,*) taer55
      v=exp(-log(taer55/2.7628)/0.79902)
      goto 71
   11 call oda550(iaer,v,taer55)
  
   71 continue
      endif

c**********************************************************************c
c xps is the parameter to express the  altitude of target              c
c                                                                      c
c                                                                      c
c                  xps >=0. means the target is at the sea level       c
c                                                                      c
c                  xps <0. means you know the altitude of the target   c
c                        expressed in km and you put that value as xps c
c                                                                      c
c                                                                      c
c**********************************************************************c
 
 771   read(iread,*) xps

       if (xps.ge.0.) then
        xps=0.
        uwus=1.424
        uo3us=0.344
       else
        if (idatm.ne.8) then
         call pressure(uw,uo3,xps)
        else
         call pressure(uwus,uo3us,xps)
        endif
       endif
 
c**********************************************************************c
c                                                                      c
c  xpp is the parameter to express the sensor altitude                 c
c                                                                      c
c                                                                      c
c         xpp= -1000  means that the sensor is a board a satellite     c
c         xpp=     0  means that the sensor is at the ground level     c
c                                                                      c
c                                                                      c
c     for aircraft simulations                                         c
c    -100< xpp <0  means you know the altitude of the sensor expressed c
c                  in kilometers units      			       c
c     this altitude is relative to the target altitude                 c
c                                                                      c
c     for aircraft simulations only, you have to give                  c
c	puw,po3   (water vapor content,ozone content between the       c
c                  aircraft and the surface)                           c
c	taerp     (the aerosol optical thickness at 550nm between the  c
c                  aircraft and the surface)                           c
c    if these data are not available, enter negative values for all    c
c    of them, puw,po3 will then be interpolated from the us62 standard c
C    profile according to the values at ground level. Taerp will be    c
c    computed according to a 2km exponential profile for aerosol.      c
c**********************************************************************c

        read(iread,*) xpp

        xpp=-xpp
        if (xpp.le.0.0) then
c          ground measurement option        
           palt=0.
           pps=p(1)
	   idatmp=0
	   taer55p=0.
	   puw=0.
	   puoz=0.
           else
	   if (xpp.ge.100.) then
c	       satellite case of equivalent	   
	      palt=1000.
	      pps=0.
	      taer55p=taer55
	      ftray=1.
	      idatmp=4
	      else
c	      "real" plane case	      
              read(iread,*) puw,puo3
	      if (puw.lt.0.) then
                 call presplane(puw,puo3,xpp,ftray)
	         idatmp=2
	         if (idatm.eq.8) then
	            puwus=puw
	            puo3us=puo3
	            puw=puw*uw/uwus
	            puo3=puo3*uo3/uo3us
	            idatmp=8
	         endif
	      else
	         call presplane(puwus,puo3us,xpp,ftray)
	         idatmp=8
              endif
              if(ier) stop
              palt=zpl(34)-z(1)
	      pps=ppl(34)
              read(iread,*) taer55p
	    if ((taer55p.lt.0.).or.((taer55-taer55p).lt.accu2)) then
c a scale heigh of 2km is assumed in case no value is given for taer55p
               taer55p=taer55*(1.-exp(-palt/2.))
            else
C compute effective scale heigh
               sham=exp(-palt/4.)
               sha=1.-(taer55p/taer55)
               if (sha.ge.sham) then
                  taer55p=taer55*(1.-exp(-palt/4.))
               else
                  sha=-palt/log(sha)
                  taer55p=taer55*(1.-exp(-palt/sha))
               endif
            endif
         endif
      endif

c**********************************************************************c
c      iwave input of the spectral conditions                          c
c            --------------------------------                          c
c                                                                      c
c  you choose to define your own spectral conditions: iwave=-1,0 or 1  c
c                   (three user s conditions )                         c
c        -2  enter wlinf, wlsup, the filter function will be equal to 1c
c            over the whole band (as iwave=0) but step by step output  c
c            will be printed                                           c
c        -1  enter wl (monochr. cond,  gaseous absorption is included) c
c                                                                      c
c         0  enter wlinf, wlsup. the filter function will be equal to 1c
c            over the whole band.                                      c
c                                                                      c
c         1  enter wlinf, wlsup and user's filter function s(lambda)   c
c                          ( by step of 0.0025 micrometer).            c
c                                                                      c
c                                                                      c
c   or you select one of the following satellite spectral bands:       c
c   with indication in brackets of the band limits used in the code :  c
c                                                iwave=2 to 60         c
c         2  vis band of meteosat     ( 0.350-1.110 )                  c
c         3  vis band of goes east    ( 0.490-0.900 )                  c
c         4  vis band of goes west    ( 0.490-0.900 )                  c
c         5  1st band of avhrr(noaa6) ( 0.550-0.750 )                  c
c         6  2nd      "               ( 0.690-1.120 )                  c
c         7  1st band of avhrr(noaa7) ( 0.500-0.800 )                  c
c         8  2nd      "               ( 0.640-1.170 )                  c
c         9  1st band of avhrr(noaa8) ( 0.540-1.010 )                  c
c        10  2nd      "               ( 0.680-1.120 )                  c
c        11  1st band of avhrr(noaa9) ( 0.530-0.810 )                  c
c        12  2nd      "               ( 0.680-1.170 )                  c
c        13  1st band of avhrr(noaa10 ( 0.530-0.780 )                  c
c        14  2nd      "               ( 0.600-1.190 )                  c
c        15  1st band of avhrr(noaa11 ( 0.540-0.820 )                  c
c        16  2nd      "               ( 0.600-1.120 )                  c
c        17  1st band of hrv1(spot1)  ( 0.470-0.650 )                  c
c        18  2nd      "               ( 0.600-0.720 )                  c
c        19  3rd      "               ( 0.730-0.930 )                  c
c        20  pan      "               ( 0.470-0.790 )                  c
c        21  1st band of hrv2(spot1)  ( 0.470-0.650 )                  c
c        22  2nd      "               ( 0.590-0.730 )                  c
c        23  3rd      "               ( 0.740-0.940 )                  c
c        24  pan      "               ( 0.470-0.790 )                  c
c        25  1st band of tm(landsat5) ( 0.430-0.560 )                  c
c        26  2nd      "               ( 0.500-0.650 )                  c
c        27  3rd      "               ( 0.580-0.740 )                  c
c        28  4th      "               ( 0.730-0.950 )                  c
c        29  5th      "               ( 1.5025-1.890 )                 c
c        30  7th      "               ( 1.950-2.410 )                  c
c        31  1st band of mss(landsat5)( 0.475-0.640 )                  c
c        32  2nd      "               ( 0.580-0.750 )                  c
c        33  3rd      "               ( 0.655-0.855 )                  c
c        34  4th      "               ( 0.785-1.100 )                  c
c        35  1st band of MAS (ER2)    ( 0.5025-0.5875)                 c
c        36  2nd      "               ( 0.6075-0.7000)                 c
c        37  3rd      "               ( 0.8300-0.9125)                 c
c        38  4th      "               ( 0.9000-0.9975)                 c
c        39  5th      "               ( 1.8200-1.9575)                 c
c        40  6th      "               ( 2.0950-2.1925)                 c
c        41  7th      "               ( 3.5800-3.8700)                 c
c        42  MODIS   band 1           ( 0.6100-0.6850)                 c
c        43  MODIS   band 2           ( 0.8200-0.9025)                 c
c        44  MODIS   band 3           ( 0.4500-0.4825)                 c
c        45  MODIS   band 4           ( 0.5400-0.5700)                 c
c        46  MODIS   band 5           ( 1.2150-1.2700)                 c
c        47  MODIS   band 6           ( 1.6000-1.6650)                 c
c        48  MODIS   band 7           ( 2.0575-2.1825)                 c
c        49  MODIS   band 8           ( 0.4025-0.4225)                 c
c        50  1st band of avhrr(noaa12 ( 0.500-1.000 )                  c
c        51  2nd      "               ( 0.650-1.120 )                  c
c        52  1st band of avhrr(noaa14 ( 0.500-1.110 )                  c
c        53  2nd      "               ( 0.680-1.100 )                  c
c        54  POLDER  band 1           ( 0.4125-0.4775)                 c
c        55  POLDER  band 2 (non polar( 0.4100-0.5225)                 c
c        56  POLDER  band 3 (non polar( 0.5325-0.5950)                 c
c        57  POLDER  band 4   P1      ( 0.6300-0.7025)                 c
c        58  POLDER  band 5 (non polar( 0.7450-0.7800)                 c
c        59  POLDER  band 6 (non polar( 0.7000-0.8300)                 c
c        60  POLDER  band 7   P1      ( 0.8100-0.9200)                 c
c        61  POLDER  band 8 (non polar( 0.8650-0.9400)                 c
c        62  SEAWIFS band 1           ( 0.3825-0.70)                   c
c        63  SEAWIFS band 2           ( 0.3800-0.58)                   c
c        64  SEAWIFS band 3           ( 0.3800-1.02)                   c
c        65  SEAWIFS band 4           ( 0.3800-1.02)                   c
c        66  SEAWIFS band 5           ( 0.3825-1.15)                   c
c        67  SEAWIFS band 6           ( 0.3825-1.05)                   c
c        68  SEAWIFS band 7           ( 0.3800-1.15)                   c
c        69  SEAWIFS band 8           ( 0.3800-1.15)                   c
c        70  AATSR   band 1           ( 0.5250-0.5925)                 c
c        71  AATSR   band 2           ( 0.6275-0.6975)                 c
c        72  AATSR   band 3           ( 0.8325-0.9025)                 c
c        73  AATSR   band 4           ( 1.4475-1.7775)                 c
c        74  MERIS   band 1           ( 0.412)                         c
c        75  MERIS   band 2           ( 0.442)                         c
c        76  MERIS   band 3           ( 0.489)                         c
c        77  MERIS   band 4           ( 0.509)                         c
c        78  MERIS   band 5           ( 0.559)                         c
c        79  MERIS   band 6           ( 0.619)                         c
c        80  MERIS   band 7           ( 0.664)                         c
c        81  MERIS   band 8           ( 0.681)                         c
c        82  MERIS   band 9           ( 0.708)                         c
c        83  MERIS   band 10          ( 0.753)                         c
c        84  MERIS   band 11          ( 0.760)                         c
c        85  MERIS   band 12          ( 0.778)                         c
c        86  MERIS   band 13          ( 0.865)                         c
c        87  MERIS   band 14          ( 0.885)                         c
c        88  MERIS   band 15          ( 0.900)                         c
c        89  GLI     band 1           (0.380-1km)                      c
c        90  GLI     band 2           (0.400-1km)                      c
c        91  GLI     band 3           (0.412-1km)                      c
c        92  GLI     band 4           (0.443-1km)                      c
c        93  GLI     band 5           (0.460-1km)                      c
c        94  GLI     band 6           (0.490-1km)                      c
c        95  GLI     band 7           (0.520-1km)                      c
c        96  GLI     band 8           (0.545-1km)                      c
c        97  GLI     band 9           (0.565-1km)                      c
c        98  GLI     band 10          (0.625-1km)                      c
c        99  GLI     band 11          (0.666-1km)                      c
c       100  GLI     band 12          (0.680-1km)                      c
c       101  GLI     band 13          (0.678-1km)                      c
c       102  GLI     band 14          (0.710-1km)                      c
c       103  GLI     band 15          (0.710-1km)       (bis?)         c
c       104  GLI     band 16          (0.749-1km)                      c
c       105  GLI     band 17          (0.763-1km)                      c
c       106  GLI     band 18          (0.865-1km)                      c
c       107  GLI     band 19          (0.865-1km)       (bis?)         c
c       108  GLI     band 20          (0.460-0.25km)                   c
c       109  GLI     band 21          (0.545-0.25km)                   c
c       110  GLI     band 22          (0.660-0.25km)                   c
c       111  GLI     band 23          (0.825-0.25km)                   c
c       112  GLI     band 24          (1.050-1km)                      c
c       113  GLI     band 25          (1.135-1km)                      c
c       114  GLI     band 26          (1.240-1km)                      c
c       115  GLI     band 27          (1.338-1km)                      c
c       116  GLI     band 28          (1.640-1km)                      c
c       117  GLI     band 29          (2.210-1km)                      c
c       118  GLI     band 30          (3.715-1km)                      c
c  note: wl has to be in micrometer                                    c
c**********************************************************************c
      do 38 l=iinf,isup
       s(l)=1.
   38 continue

      read(iread,*) iwave

      if (iwave.eq.-2) goto 1600
      if (iwave) 16,17,18


   16 read(iread,*) wl


      wlinf=wl
      wlsup=wl
      go to 19
   17 read(iread,*) wlinf,wlsup
      go to 19
 1600 read(iread,*) wlinf,wlsup
      go to 19
c       110
c       111     band of meteosat        (2)
c       112     band of goes east       (3,4)
c       114     band of avhr            (5,16)
c       118     band of hrv1            (17,24)
c       121     band of tm              (25,30)
c       127     band of mss             (31,34)
c       128     band of MAS             (35,41)
c       129     MODIS   band            (42,49)
c       130     band of avhrr           (50,53)
c       131     POLDER  band            (54,61)
c       113     SEAWIFS band            (62,69)
c       150     AATSR   band            (70,73)
c       151     MERIS   band            (74,88)
c       152     GLI     band            (89,118)
   18 goto (110,
     s      111,
     s      112,112,
     s      114,114,114,114,114,114,114,114,114,114,114,114,
     s      118,118,118,118,118,118,118,118,
     s      121,121,121,121,121,121,
     s      127,127,127,127,
     s      128,128,128,128,128,128,128,
     s      129,129,129,129,129,129,129,129,
     s      130,130,130,130,
     s      131,131,131,131,131,131,131,131,
     s      113,113,113,113,113,113,113,113,
     s      150,150,150,150,
     s      151,151,151,151,151,151,151,151,
     s      151,151,151,151,151,151,151,
     s      152,152,152,152,152,152,152,152,152,152,
     s      152,152,152,152,152,152,152,152,152,152,
     s      152,152,152,152,152,152,152,152,152,152
     s     ),iwave
  110 read(iread,*) wlinf,wlsup
      iinf=(wlinf-.25)/0.0025+1.5
      isup=(wlsup-.25)/0.0025+1.5
      do 1113 ik=iinf,isup
       s(ik)=0.
 1113 continue
      read(iread,*) (s(i),i=iinf,isup)
      goto 20
  111 call meteo
      go to 19
  112 call goes(iwave-2)
      go to 19
  114 call avhrr(iwave-4)
      go to 19
  118 call hrv(iwave-16)
      go to 19
  121 call tm(iwave-24)
      go to 19
  127 call mss(iwave-30)
      goto 19
  128 call mas(iwave-34)
      goto 19
  129 call modis(iwave-41)
      goto 19
  130 call avhrr(iwave-37)
      goto 19
  131 call polder(iwave-53)
      goto 19
  113 call seawifs(iwave-61)
      goto 19
  150 call aatsr(iwave-69)
      goto 19
  151 call meris(iwave-73)
      goto 19
  152 call gli(iwave-88)
   19 iinf=(wlinf-.25)/0.0025+1.5
      isup=(wlsup-.25)/0.0025+1.5
   20 continue
 
C***********************************************************************
C LOOK UP TABLE INITIALIZATION
C***********************************************************************
C  initialization of look up table variable
C     Write(6,*) "TOTO THE HERO"
      
      do i=1,mu
      nfilut(i)=0
      do j=1,41
      rolut(i,j)=0.
      rolutq(i,j)=0.
      rolutu(i,j)=0.
      filut(i,j)=0.
      roluti(i,j)=0.
      rolutiq(i,j)=0.
      rolutiu(i,j)=0.
      enddo
      enddo
      xmus=cos(asol*pi/180.)
      its=acos(xmus)*180.0/pi
C Case standart LUT      
      if (ilut.eq.1) then
       do i=1,mu-1
         lutmuv=rm(i)
         luttv=acos(lutmuv)*180./pi
         iscama=(180-abs(luttv-its))
         iscami=(180-(luttv+its))
         nbisca=int(0.01+(iscama-iscami)/4.0)+1
         nfilut(i)=nbisca
         filut(i,1)=0.0
         filut(i,nbisca)=180.0
	 scaa=iscama
         do j=2,nfilut(i)-1
          scaa=scaa-4.0
          cscaa=cos(scaa*pi/180.)
          cfi=-(cscaa+xmus*lutmuv)/(sqrt(1-xmus*xmus)
     S	  *sqrt(1.-lutmuv*lutmuv))
          filut(i,j)=acos(cfi)*180.0/pi
         enddo
      enddo
      i=mu
         lutmuv=cos(avis*pi/180.)
         luttv=acos(lutmuv)*180./pi
         iscama=(180-abs(luttv-its))
         iscami=(180-(luttv+its))
         nbisca=int((iscama-iscami)/4)+1
         nfilut(i)=nbisca
         filut(i,1)=0.0
         filut(i,nbisca)=180.0
	 scaa=iscama
         do j=2,nfilut(i)-1
          scaa=scaa-4.0
          cscaa=cos(scaa*pi/180.)
          cfi=-(cscaa+xmus*lutmuv)/(sqrt(1-xmus*xmus)
     S	  *sqrt(1.-lutmuv*lutmuv))
          filut(i,j)=acos(cfi)*180.0/pi
         enddo
        endif
C END Case standart LUT      

C Case LUT for APS
      if (ilut.eq.3) then
       do i=1,mu-1
         nbisca=2
         nfilut(i)=nbisca
         filut(i,1)=(phi0-phiv)
         filut(i,nbisca)=(phi0-phiv)+180.0
      enddo
      i=mu
         nbisca=1
         nfilut(i)=nbisca
         filut(i,1)=(phi0-phiv)
         endif
C END 	Case LUT for APS
CCCC Check initialization  (debug)     
       do i=1,mu
         lutmuv=rm(i)
         luttv=acos(lutmuv)*180./pi
        do j=1,nfilut(i)
       cscaa=-xmus*lutmuv-cos(filut(i,j)*pi/180.)*sqrt(1.-xmus*xmus)
     S  *sqrt(1.-lutmuv*lutmuv)
       scaa=acos(cscaa)*180./pi
      write(6,*) its,luttv,filut(i,j),scaa
      enddo
      enddo
CCCC Check initialization  (debug)     
C***********************************************************************
C END LOOK UP TABLE INITIALIZATION
C***********************************************************************

 
 
 
c**********************************************************************c
c here, we first compute an equivalent wavelenght which is the input   c
c value for monochromatic conditions or the integrated value for a     c
c filter functionr (call equivwl) then, the atmospheric properties are c
c computed for that wavelength (call discom then call specinterp)      c
c molecular optical thickness is computed too (call odrayl). lastly    c
c the successive order of scattering code is called three times.       c
c first for a sun at thetas with the scattering properties of aerosols c
c and molecules, second with a pure molecular atmosphere, then with thec
c actual atmosphere for a sun at thetav. the iso code allows us to     c
c compute the scattering transmissions and the spherical albedo. all   c
c these computations are performed for checking the accuracy of the    c
c analytical expressions and in addition for computing the averaged    c
c directional reflectances                                             c
c**********************************************************************c
      if(iwave.ne.-1) then
        call equivwl(iinf,isup,step,
     s               wlmoy)
      else
        wlmoy=wl
      endif
      call discom (idatmp,iaer,iaer_prof,xmus,xmuv,phi,taer55,taer55p,
     a      palt,phirad,nt,mu,np,rm,gb,rp,ftray,ipol,xlm1,xlm2,
     a      roatm_fi,nfi,
     a      nfilut,filut,roluts,rolutsq,rolutsu)


      if(iaer.ne.0) then
        call specinterp(wlmoy,taer55,taer55p,
     s     tamoy,tamoyp,pizmoy,pizmoyp,ipol)
      endif
      call odrayl(wlmoy,
     s                   trmoy)

      trmoyp=trmoy*ftray


      if (idatmp.eq.4) then
          trmoyp=trmoy
          tamoyp=tamoy
      endif
      if (idatmp.eq.0) then
         trmoyp=0.
         tamoyp=0.
      endif

 
c*********************************************************************c
c     inhomo        ground reflectance (type)                          c
c                   ------------------                                 c
c                                                                      c
c  you consider an homogeneous surface:                                c
c     enter - inhomo=0                                                 c
c                you may consider directional surface  effects         c
c                  idirec=0 (no directional effect)                    c
c                          you have to specify the surface reflectance:c
c                          igroun  (see note1) which is uniform and    c
c                          lambertian                                  c
c                  idirec=1 ( directional effect)                      c
c                          you have to specify the brdf of the surface c
c                           for the actual solar illumination you  are c
c                           considering as well as the brdf for a sun  c
c                           which would be at an angle thetav, in      c
c                           addition you have to give the surface      c
c                           albedo (spherical albedo). you can also    c
c                           select one of the selected model from the  c
c                           ibrdf value (see note2). 3 reflectances    c
c                           are computed, robar,robarp and robard      c
c                                                                      c
c  you consider a non uniform surface, the surface is considered as a  c
c            circular target with a reflectance roc and of radius r    c
c            (expressed in km) within an environment of reflectance    c
c            roe                                                       c
c     enter - inhomo=1, then                                           c
c             igrou1,igrou2,rad                                        c
c                  - the target reflectance :igrou1  (see note1)       c
c                  - the envir. reflectance :igrou2  (see note1)       c
c                  - the target radius in km                           c
c                                                                      c
c                                                                      c
c                            ****tree****                              c
c                                                                      c
c                               inhomo                                 c
c                             /          \                             c
c                            /            \                            c
c                           /              \                           c
c                          /                \                          c
c                 ------- 0 -------       -----1 -----                 c
c                        /               /   \       \                 c
c                    idirec             /     \       \                c
c                    /  \              /       \       \               c
c                   /    \            /         \       \              c
c                  /      \       igrou1       igrou2    rad           c
c                 0        1        roc          roe     f(r)          c
c                /          \                                          c
c               /            \                                         c
c           igroun          ibrdf                                      c
c        (roc = roe)        (roc)                                      c
c                           (robar)                                    c
c                           (robarp)                                   c
c                           (robard)                                   c
c                                                                      c
c                   ground reflectance (spectral variation)            c
c                   ---------------------------------------            c
c note1: values of the reflectance selected by igroun,igrou1 or igrou2 c
c        may correspond to the following cases,                        c
c         0  constant value of ro (or roc,or roe) whatever the wavelen c
c            gth. you enter this constant value of ro (or roc or roe). c
c        -1  you have to enter the value of ro (or roc,or roe) by step c
c            of 0.0025 micron from wlinf to wlsup (if you have used thec
c            satellite bands,see implicit values for these limits).    c
c         1  mean spectral value of green vegetation                   c
c         2  mean spectral value of clear water                        c
c         3  mean spectral value of sand                               c
c         4  mean spectral value of lake water                         c
c                                                                      c
c                       ground reflectance (brdf)                      c
c                       -------------------------                      c
c note2: values of the directional reflectance is assumed spectrally   c
c        independent, so you have to specify, the brdf at the          c
c        wavelength for monochromatic condition of the mean value      c
c        over the spectral band                                        c
c         0  you have to enter the value of ro for sun at thetas by    c
c            step of 10 degrees for zenith view  angles (from 0 to 80  c
c            and the value for 85) and by step of 30 degrees for       c
c            azimuth view angles from 0 to 360 degrees, you have to do c
c            same for a sun which would be at thetav. in addition, the c
c            spherical albedo of the surface has to be specified ,as   c
C            well as the observed reflectance in the selected geometry c
c           rodir(sun zenith,view zenith, relative azimuth).	       c
c		 						       c
c        you also may select one of the following models               c
c         1  hapke model                                               c
c             the parameters are: om,af,s0,h                           c
c                    om= albedo                                        c
c                    af=assymetry parameter for the phase function     c
c                    s0=amplitude of hot spot                          c
c                    h=width of the hot spot                           c
c                                                                      c
c         2  verstraete et al. model                                   c
c             the parameters are:                                      c
c                there is three lines of parameters:                   c
c                              line 1 (choice of options)              c
c                              line 2 (structural parameters)          c
c                              line 3 (optical parameters)             c
c                line 1:  opt3 opt4 opt5                               c
c                    opt1=1 parametrized model (see verstraete et al., c
c                           JGR, 95, 11755-11765, 1990)                c
c                    opt2=1 reflectance factor (see pinty et al., JGR, c
c                           95, 11767-11775, 1990)                     c
c                    opt3=0 for given values of kappa (see struc below)c
c                         1 for goudriaan's parameterization of kappa  c
c                         2 for dickinson et al's correction to        c
c                           goudriaan's parameterization of kappa (see c
c                           dickinson et al., agricultural and forest  c
c                           meteorology, 52, 109-131, 1990)            c
c                       ---see the manual for complete references----  c
c                    opt4=0 for isotropic phase function               c
c                         1 for heyney and greensteins' phase function c
c                         2 for legendre polynomial phase function     c 
c                    opt5=0 for single scattering only                 c
c                         1 for dickinson et al. parameterization of   c
c                           multiple scattering                        c
c                line 2:  str1 str2 str3 str4                          c
c                    str1='leaf area density', in m2 m-3               c
c                    str2=radius of the sun flecks on the scatterer (m)c
c                    str3=leaf orientation parameter:                  c
c                         if opt3=0 then str3=kappa1                   c
c                         if opt3=1 or 2  then str3=chil               c
c                    str4=leaf orientation parameter (continued):      c
c                         if opt3=0 then str4=kappa2                   c
c                         if opt3=1 or 2 then str4 is not used         c
c                line 3:  optics1 optics2 optics3                      c
c                    optics1=single scattering albedo, n/d value       c
c                            between 0.0 and 1.0                       c
c                    optics2= phase function parameter:                c
c                         if opt4=0 then this input is not used        c
c                         if opt4=1 then asymmetry factor, n/d value   c
c                                   between -1.0and 1.0                c
c                         if opt4=2 then first coefficient of legendre c
c                                   polynomial                         c
c                    optics3=second coefficient of legendre polynomial c
c                            (if opt4=2)                               c
c                                                                      c
c         3  Roujean et al. model                                      c
c             the parameters are: k0,k1,k2                             c
c                 k0=albedo.                                           c
c                 k1=geometric parameter for hot spot effect           c
c                 k2=geometric parameter for hot spot effect           c
c                                                                      c
c         4  walthall et al. model                                     c
c             the parameters are: a,ap,b,c                             c    
c                 a=term in square ts*tv                               c
c                 ap=term in square ts*ts+tv*tv                        c
c                 b=term in ts*tv*cos(phi) (limacon de pascal)         c
c                 c=albedo                                             c
c                                                                      c
c         5  minnaert model                                            c
c             the parameters are: par1,par2                            c
c                                                                      c
c         6  Ocean                                                     c
c             the parameter are: pws,phi_wind,xsal,pcl                 c
c                 pws=wind speed (in m/s)                              c
c                 phi_wind=azim. of the wind (in degres)               c
c                 xsal=salinity (in ppt) xsal=34.3ppt if xsal<0        c
c                 pcl=pigment concentration (in mg/m3)                 c
c                                                                      c
c         7  Iaquinta and Pinty model                                  c
c             the parameters are:                                      c
c                there is 3 lines of parameters:                       c
c                          line 1: choice of option (pild,pihs)        c
c                          line 2: structural parameters (pxLt,pc)     c
c                          line 3: optical parameters (pRl,pTl,pRs)    c
c                Line 1: pild,pihs                                     c
c                    pild=1  planophile leaf distribution              c 
c                    pild=2  erectophile leaf distribution             c 
c                    pild=3  plagiophile leaf distribution             c 
c                    pild=4  extremophile leaf distribution            c 
c                    pild=5  uniform leaf distribution                 c 
c                                                                      c 
c                    pihs=0  no hot spot                               c 
c                    pihs=1  hot spot                                  c 
c                Line 2: pxLt,pc                                       c
c                    pxLt=Leaf area index [1.,15.]                     c 
c                    pc=Hot spot parameter: 2*r*Lambda [0.,2.]         c
c                Line 3: pRl,pTl,pRs                                   c
c                    pRl=Leaf reflectance  [0.,0.99]                   c 
c                    pTl=Leaf transmitance [0.,0.99]                   c 
c                    pRs=Soil albedo       [0.,0.99]                   c 
c                         NB: pRl+PTl <0.99                            c 
c                                                                      c
c         8  Rahman et al. model                                       c
c             the parameters are: rho0,af,xk                           c
c                 rho0=Intensity of the reflectance of the surface     c
c                      cover, N/D value greater or equal to 0          c
c                 af=Asymmetry factor, N/D value between -1.0 and 1.0  c
c                 xk=Structural parameter of the medium                c
c         9   Kuusk's multispectral CR model                           c
c             Reference:                                               c
c             Kuusk A. A multispectral canopy reflectance model.       c
c             Remote Sens. Environ., 1994, 50:75-82                    c
c                                                                      c
c                                                                      c
c             the parameters are:                                      c
c                                                                      c
c     line 1: structural parameters (ul,eps,thm,sl)                    c
c     line 2: optical parameters (cAB,cW,N,cn,s1)                      c
c                                                                      c
c             ul=LAI     [0.1...10]                                    c
c             eps,thm - LAD parameters                                 c
c             eps [0.0..0.9] thm [0.0..90.0]                           c
c             sl      - relative leaf size  [0.01..1.0]                c
c             cAB     - chlorophyll content, ug/cm^2    [30]           c
c             cW      - leaf water equivalent thickness  [0.01..0.03]  c
c             N       - the effective number of elementary layers      c
c                       inside a leaf   [1.225]                        c
c             cn      - the ratio of refractive indices of the leaf    c
c                       surface wax and internal material  [1.0]       c
c             s1      - the weight of the 1st Price function for the   c
c                       soil reflectance     [0.1..0.8]                c
c        10  MODIS operational BDRF                                     c
c             the parameters are: p1,p2,p3                             c
c                 p1 weight for lambertian kernel                      c
c                 p2 weight for Ross Thick kernel                      c
c                 p3 weight for Li Sparse  kernel                      c
c**********************************************************************c
									
      fr=0.
      rad=0.
      do 1116 ik=iinf,isup
        rocl(ik)=0.
        roel(ik)=0.
 1116 continue
 
c**********************************************************************c
c     uniform or non-uniform surface conditions                        c
c**********************************************************************c

      read(iread,*) inhomo

      if(inhomo) 30,30,31

  30  read(iread,*) idirec

      if(idirec)21,21,25
 
c**********************************************************************c
c     uniform conditions with brdf conditions                          c
c**********************************************************************c
c call os to compute downward radiation field for robar
 25   rm(-mu)=-xmuv
      rm(mu)=xmuv
      rm(0)=-xmus
      spalt=1000.
      call os(iaer_prof,tamoy,trmoy,pizmoy,tamoyp,trmoyp,spalt,
     s               phirad,nt,mu,np,rm,gb,rp,
     s                     xlmus,xlphim,nfi,rolut)
c call os to compute downward radiation field for robarp
      if (idatmp.ne.0) then
        rm(-mu)=-xmus
        rm(mu)=xmus
        rm(0)=-xmuv
        call os(iaer_prof,tamoyp,trmoyp,pizmoy,tamoyp,trmoyp,spalt,
     s               phirad,nt,mu,np,rm,gb,rp,
     s                     xlmuv,xlphim,nfi,rolut)
      endif
c
      read(iread,*) ibrdf
c*********************************************************************c
      if(ibrdf)23,23,24
c**********************************************************************c
c     brdf from in-situ measurements                                   c
c**********************************************************************c
  23  do 900 k=1,13
        read(iread,*) (brdfdats(10-j+1,k),j=1,10)
  900 continue
      do 901 k=1,13
        read(iread,*) (brdfdatv(10-j+1,k),j=1,10)
  901 continue
      read(iread,*) albbrdf
      read(iread,*) rodir
      rm(-mu)=phirad
      rm(mu)=xmuv
      rm(0)=xmus
      call brdfgrid(mu,np,rm,rp,brdfdats,angmu,angphi,
     s                 brdfints)
      rm(-mu)=2.*pi-phirad
      rm(mu)=xmus
      rm(0)=xmuv
      call brdfgrid(mu,np,rm,rp,brdfdatv,angmu,angphi,
     s                 brdfintv)
      brdfints(mu,1)=rodir
       do l=iinf,isup
          sbrdf(l)=rodir
          enddo
      go to 69
c**********************************************************************c
c     brdf from hapke's model                                          c
c**********************************************************************c
  24  if(ibrdf.eq.1) then
        read(iread,*) par1,par2,par3,par4
	
        srm(-1)=phirad
        srm(1)=xmuv
        srm(0)=xmus
        call hapkbrdf(par1,par2,par3,par4,1,1,srm,srp,
     s           sbrdftmp)
        do l=iinf,isup
           sbrdf(l)=sbrdftmp(1,1)
           enddo
	
        rm(-mu)=phirad
        rm(mu)=xmuv
        rm(0)=xmus
        call hapkbrdf(par1,par2,par3,par4,mu,np,rm,rp,
     s           brdfints)
        rm(-mu)=2.*pi-phirad
        rm(mu)=xmus
        rm(0)=xmuv
        call hapkbrdf(par1,par2,par3,par4,mu,np,rm,rp,
     s           brdfintv)
        call hapkalbe(par1,par2,par3,par4,
     s       albbrdf)
        go to 69
      endif
c**********************************************************************c
c     brdf from verstraete et al's model                               c
c**********************************************************************c
      if(ibrdf.eq.2) then
        read(iread,*) (options(i),i=3,5)
        options(1)=1
        options(2)=1
        read(iread,*) (struct(i),i=1,4)
        read(iread,*) (optics(i),i=1,3)
	
        srm(-1)=phirad
        srm(1)=xmuv
        srm(0)=xmus
        call versbrdf(options,optics,struct,1,1,srm,srp,
     s           sbrdftmp)
        do l=iinf,isup
           sbrdf(l)=sbrdftmp(1,1)
           enddo
	
        rm(-mu)=phirad
        rm(mu)=xmuv
        rm(0)=xmus
        call versbrdf(options,optics,struct,mu,np,rm,rp,
     s           brdfints)
        rm(-mu)=2.*pi-phirad
        rm(mu)=xmus
        rm(0)=xmuv
        call versbrdf(options,optics,struct,mu,np,rm,rp,
     s           brdfintv)
        call versalbe(options,optics,struct,
     s       albbrdf)
        go to 69
      endif
c**********************************************************************c
c     brdf from Roujean et al's model                                  c
c**********************************************************************c
      if(ibrdf.eq.3) then
        read(iread,*) par1,par2,par3
	
        srm(-1)=phirad
        srm(1)=xmuv
        srm(0)=xmus
        call roujbrdf(par1,par2,par3,1,1,srm,srp,
     s           sbrdftmp)
        do l=iinf,isup
           sbrdf(l)=sbrdftmp(1,1)
           enddo
	
        rm(-mu)=phirad
        rm(mu)=xmuv
        rm(0)=xmus
        call roujbrdf(par1,par2,par3,mu,np,rm,rp,
     s           brdfints)
        rm(-mu)=2.*pi-phirad
        rm(mu)=xmus
        rm(0)=xmuv
        call roujbrdf(par1,par2,par3,mu,np,rm,rp,
     s           brdfintv)
        call roujalbe(par1,par2,par3,
     s       albbrdf)
        go to 69
      endif
c**********************************************************************c
c     brdf from walthall et al's model
c**********************************************************************c
      if(ibrdf.eq.4) then
        read(iread,*) par1,par2,par3,par4
	
        srm(-1)=phirad
        srm(1)=xmuv
        srm(0)=xmus
        call waltbrdf(par1,par2,par3,par4,1,1,srm,srp,
     s           sbrdftmp)
        do l=iinf,isup
           sbrdf(l)=sbrdftmp(1,1)
           enddo
	
        rm(-mu)=phirad
        rm(mu)=xmuv
        rm(0)=xmus
        call waltbrdf(par1,par2,par3,par4,mu,np,rm,rp,
     s           brdfints)
        rm(-mu)=2.*pi-phirad
        rm(mu)=xmus
        rm(0)=xmuv
        call waltbrdf(par1,par2,par3,par4,mu,np,rm,rp,
     s           brdfintv)
        call waltalbe(par1,par2,par3,par4,
     s       albbrdf)
        go to 69
      endif
c**********************************************************************c
c     brdf from minnaert's model                                       c
c**********************************************************************c
      if(ibrdf.eq.5) then
        read(iread,*) par1,par2
	
        srm(-1)=phirad
        srm(1)=xmuv
        srm(0)=xmus
        call minnbrdf(par1,par2,1,1,srm,
     s           sbrdftmp)
        do l=iinf,isup
           sbrdf(l)=sbrdftmp(1,1)
           enddo
	
        rm(-mu)=phirad
        rm(mu)=xmuv
        rm(0)=xmus
        call minnbrdf(par1,par2,mu,np,rm,
     s           brdfints)
        rm(-mu)=2.*pi-phirad
        rm(mu)=xmus
        rm(0)=xmuv
        call minnbrdf(par1,par2,mu,np,rm,
     s           brdfintv)
        call minnalbe(par1,par2,
     s       albbrdf)
        go to 69
      endif
 
c**********************************************************************c
c     brdf from ocean condition
c**********************************************************************c
      if(ibrdf.eq.6) then
        read(iread,*) pws,phi_wind,xsal,pcl
        if (xsal.lt.0.001)xsal=34.3
        paw=phi0-phi_wind
	
        do l=iinf,isup
           srm(-1)=phirad
           srm(1)=xmuv
           srm(0)=xmus
           wl=.25+(l-1)*step 
           call oceabrdf(pws,paw,xsal,pcl,wl,rfoam,rwat,rglit,
     s         1,1,srm,srp,
     s           sbrdftmp)
     
     	   rfoaml(l)=rfoam
           rwatl(l)=rwat
	   rglitl(l)=rglit
	   sbrdf(l)=sbrdftmp(1,1)
           enddo
	
        rm(-mu)=phirad
        rm(mu)=xmuv
        rm(0)=xmus
        call oceabrdf(pws,paw,xsal,pcl,wlmoy,rfoam,rwat,rglit,
     s  	mu,np,rm,rp,
     s           brdfints)
        rm(-mu)=2.*pi-phirad
        rm(mu)=xmus
        rm(0)=xmuv
        call oceabrdf(pws,paw,xsal,pcl,wlmoy,rfoam,rwat,rglit,
     s   	mu,np,rm,rp,
     s           brdfintv)
        call oceaalbe(pws,paw,xsal,pcl,wlmoy,
     s       albbrdf)
        go to 69
      endif
c
c**********************************************************************c
c     brdf from Iaquinta and Pinty model
c**********************************************************************c
      if(ibrdf.eq.7) then
        read(iread,*) pild,pihs
        read(iread,*) pxLt,pc
        read(iread,*) pRl,pTl,pRs
	
        srm(-1)=phirad
        srm(1)=xmuv
        srm(0)=xmus
        call iapibrdf(pild,pxlt,prl,ptl,prs,pihs,pc,1,1,srm,srp,
     s           sbrdftmp)
        do l=iinf,isup
           sbrdf(l)=sbrdftmp(1,1)
           enddo
	
        rm(-mu)=phirad
        rm(mu)=xmuv
        rm(0)=xmus
        call iapibrdf(pild,pxlt,prl,ptl,prs,pihs,pc,mu,np,rm,rp,
     s           brdfints)
        rm(-mu)=2.*pi-phirad
        rm(mu)=xmus
        rm(0)=xmuv
        call iapibrdf(pild,pxlt,prl,ptl,prs,pihs,pc,mu,np,rm,rp,
     s           brdfintv)
        call iapialbe(pild,pxlt,prl,ptl,prs,pihs,pc,
     s       albbrdf)
        go to 69
      endif
c
c**********************************************************************c
c     brdf from Rahman model                
c**********************************************************************c
      if(ibrdf.eq.8) then
        read(iread,*) par1,par2,par3
	
        srm(-1)=phirad
        srm(1)=xmuv
        srm(0)=xmus
        call rahmbrdf(par1,par2,par3,1,1,srm,srp,
     s           sbrdftmp)
        do l=iinf,isup
           sbrdf(l)=sbrdftmp(1,1)
           enddo
	
        rm(-mu)=phirad
        rm(mu)=xmuv
        rm(0)=xmus
        call rahmbrdf(par1,par2,par3,mu,np,rm,rp,
     s           brdfints)
        rm(-mu)=2.*pi-phirad
        rm(mu)=xmus
        rm(0)=xmuv
        call rahmbrdf(par1,par2,par3,mu,np,rm,rp,
     s           brdfintv)
        call rahmalbe(par1,par2,par3,
     s       albbrdf)
        go to 69
      endif
c
c**********************************************************************c
c     brdf from kuusk's msrm model                                     c
c**********************************************************************c
      if(ibrdf.eq.9) then
         read(iread,*) uli,eei,thmi,sli
         read(iread,*) cabi,cwi,vaii,rnci,rsl1i
	 
        do l=iinf,isup
           srm(-1)=phirad
           srm(1)=xmuv
           srm(0)=xmus
           wl=.25+(l-1)*step 
           call akbrdf(eei,thmi,uli,sli,rsl1i,wl,rnci,cabi,cwi,vaii
     s      ,1,1,srm,srp,sbrdftmp)
           sbrdf(l)=sbrdftmp(1,1)
           enddo
	 
         rm(-mu)=phirad
         rm(mu)=xmuv
         rm(0)=xmus
         call akbrdf(eei,thmi,uli,sli,rsl1i,wlmoy,rnci,cabi,cwi,vaii
     &            ,mu,np,rm,rp,brdfints)
         rm(-mu)=2.*pi-phirad
         rm(mu)=xmus
         rm(0)=xmuv
         call akbrdf(eei,thmi,uli,sli,rsl1i,wlmoy,rnci,cabi,cwi,vaii
     &            ,mu,np,rm,rp,brdfintv)
c
         call akalbe
*    & (eei,thmi,uli,sli,rsl1i,wlmoy,rnci,cabi,cwi,vaii,albbrdf)
     & (albbrdf)
         go to 69
      endif
c
c**********************************************************************c
c     brdf from MODIS BRDF   model                                     c
c**********************************************************************c
      if(ibrdf.eq.10) then
         read(iread,*)p1,p2,p3
	 
           srm(-1)=phirad
           srm(1)=xmuv
           srm(0)=xmus
           call modisbrdf(p1,p2,p3
     s      ,1,1,srm,srp,sbrdftmp)
        do l=iinf,isup
           sbrdf(l)=sbrdftmp(1,1)
           enddo
	 
         rm(-mu)=phirad
         rm(mu)=xmuv
         rm(0)=xmus
         call modisbrdf(p1,p2,p3
     &            ,mu,np,rm,rp,brdfints)
         rm(-mu)=2.*pi-phirad
         rm(mu)=xmus
         rm(0)=xmuv
         call modisbrdf(p1,p2,p3
     &            ,mu,np,rm,rp,brdfintv)
c
         call modisalbe(p1,p2,p3
     &                 ,albbrdf)
         go to 69
      endif
c
   69 continue
c**********************************************************************c
c the downward irradiance was computed for a sun at thetas and         c
c several viewing directions (mu zenith times np azimuth). then, the   c
c code computes the product of ldown*brdf integrated over the total    c
c hemisphere and gives the averaged directional reflectance after the  c
c normalization. the resulting reflectance is named robar              c
c**********************************************************************c
      robar1=0.
      xnorm1=0.
      do 83 j=1,np
        rob=0.
        xnor=0.
        do 84 k=1,mu-1
          rdown=xlmus(-k,j)
          rdir=brdfintv(k,j)
          rob=rob+rdown*rdir*rm(k)*gb(k)
          xnor=xnor+rdown*rm(k)*gb(k)
   84   continue
        robar1=robar1+rob*gp(j)
        xnorm1=xnorm1+xnor*gp(j)
   83 continue
 
c**********************************************************************c
c the downward irradiance was computed for a sun at thetav and         c
c several viewing directions (mu zenith times np azimuth). then, the   c
c code computes the product of ldown*brdf integrated over the total    c
c hemisphere and gives the averaged directional reflectance after the  c
c normalization. the resulting reflectance is named robarp             c
c**********************************************************************c
      robar2=0.
      xnorm2=0.
      do 85 j=1,np
        rob=0.
        xnor=0.
        do 86 k=1,mu-1
          rdown=xlmuv(-k,j)
          rdir=brdfints(k,j)
          rob=rob+rdown*rdir*rm(k)*gb(k)
          xnor=xnor+rdown*rm(k)*gb(k)
   86   continue
        robar2=robar2+rob*gp(j)
        xnorm2=xnorm2+xnor*gp(j)
   85 continue
 
c  robard is assumed equal to albbrdf
c       print 301,brdfints(mu,1),robar1,xnorm1,
c    s       robar2,xnorm2,albbrdf
c       print 301,robar1/xnorm1,robar2/xnorm2
c       print 301,betal(0)/3,pizmoy
c301  format(6(f10.4,2x))
c501  format(5(i10,2x))
      do 335 l=iinf,isup
        rocl(l)=sbrdf(l)
        roel(l)=sbrdf(l)
        robar(l)=robar1/xnorm1
        if (idatmp.ne.0) then
          robarp(l)=robar2/xnorm2
        else
          robarp(l)=0.
          xnorm2=1.
          robar2=0.
        endif
        robard(l)=albbrdf
  335 continue
      go to 34
 
c**********************************************************************c
c     uniform surface with lambertian conditions                       c
c**********************************************************************c

  21  read(iread,*) igroun

      if(igroun) 29,32,33
      
  29  read(iread,*) nwlinf,nwlsup
      niinf=(nwlinf-.25)/0.0025+1.5
      nisup=(nwlsup-.25)/0.0025+1.5
      read(iread,*) (rocl(i),i=niinf,nisup)
      goto 36

  32  read(iread,*) ro

      do 35 l=iinf,isup
        rocl(l)=ro
   35 continue
      goto 36
  33  if(igroun.eq.1) call vegeta(rocl)
      if(igroun.eq.2) call clearw(rocl)
      if(igroun.eq.3) call sand  (rocl)
      if(igroun.eq.4) call lakew (rocl)
   36 do 39 l=iinf,isup
        roel(l)=rocl(l)
   39 continue
      go to 34
 
c**********************************************************************c
c     non-uniform conditions with lambertian conditions                c
c**********************************************************************c
 31   read(iread,*) igrou1,igrou2,rad
      if(igrou1) 59,60,63
  59  read(iread,*) (rocl(i),i=iinf,isup)
      goto 61
  60  read(iread,*) roc
      do 64 l=iinf,isup
        rocl(l)=roc
   64 continue
      go to 61
  63  if(igrou1.eq.1) call vegeta(rocl)
      if(igrou1.eq.2) call clearw(rocl)
      if(igrou1.eq.3) call sand  (rocl)
      if(igrou1.eq.4) call lakew (rocl)
   61 if(igrou2) 66,62,65
  66  read(iread,*) (roel(i),i=iinf,isup)
      goto 34
  62  read(iread,*) roe
      do 67 l=iinf,isup
        roel(l)=roe
   67 continue
      go to 34
  65  if(igrou2.eq.1) call vegeta(roel)
      if(igrou2.eq.2) call clearw(roel)
      if(igrou2.eq.3) call sand  (roel)
      if(igrou2.eq.4) call lakew (roel)
   34 continue
 
c**********************************************************************c
c                                                                      c
c       irapp   that input parameter allows to activate atmospheric    c
c               correction mode                                        c
c                                                                      c
c		-1: No atmospheric Correction is performed             c
c	       0,1: Atmospheric Correction with Lambertian assumption  c
c                   and with the assumption that                       c
c		    target BRDF is proportional to the input BRDF (see c
c		    case idirec=1)                                     c
c                                                                      c
c        rapp   parameter that contains the reflectance/radiance       c
c               to be corrected.                                       c
c                                                                      c
c               if rapp >0. :  the code retrieve the value of the      c
c               surface reflectance (rog) that will produce a radiance c
c               equal to rapp [w/m2/str/mic] in the atmospheric        c
c               conditions described by user before                    c
c                                                                      c
c               if -1.<rapp<0. : the code retrieve the value of the    c
c               surface reflectance (rog) value that will produce a    c
c               'reflectance' (radiance*pi/(mus*es)) equal to -rapp    c
c               where mus is the cosine of solar zenith angle,         c
c               es is the solar constant integrated upon the           c
c               filter response and taking account for earth-solar     c
c               distance, es is in [w/m2/sr/mic].                      c
c                                                                      c
c**********************************************************************c

      read(iread,*) irapp

      if (irapp.ge.0) then
         irapp=1
         read(iread,*) rapp
         endif
	 
	 
c**********************************************************************c
c                                                                      c
c      Some optional input for polarization                            c
c                                                                      c
c  you can input polarization definition through irop:                 c
c         1  enter ropq and ropu (stokes parameter for polarized       c
c            surface reflectance                                       c
c         2   enter pveg (% vegetation) for use in Nadal,Breon model   c
c         3   enter wspd for sunglint polarization  (sunglint)         c
c         anything else will result in assuming than surface does not  c
c         polarized.                                                   c
c                                                                      c
c                                                                      c
c**********************************************************************c
	 
C       ilut=0
C       read(iread,*,end=37) ilut
      
       irop=0

       read(iread,*,end=37) irop

       if (irop.eq.1) then
       read(iread,*) ropq,ropu
       endif
       
       if (irop.eq.2) then
       read(iread,*) pveg
       call polnad(asol,avis,phi,pveg,ropq,ropu)
       endif
       
       if (irop.eq.3) then
       read(iread,*) wspd,azw
       razw=phi0-azw
       call polglit(asol,avis,phi,wspd,razw,ropq,ropu)
       endif
       
 37    if ((irop.lt.1).or.(irop.gt.3)) then
       if (idirec.eq.0) then
       ropq=0.000
       ropu=0.000
       else
       if (ibrdf.eq.6) then
          irop=3
	  wspd=pws
	  azw=phi_wind
	  razw=phi0-azw
	  phi=phi0-phiv
          call polglit(asol,avis,phi,wspd,razw,ropq,ropu)
	  endif
       if (ibrdf.eq.9) then
          irop=2
          pveg=ul
	  if (pveg.gt.1.) pveg=1
	  call polnad(asol,avis,phi,pveg,ropq,ropu)
	  endif
       endif  
       endif
C      write(6,*) "Surface polarization reflectance, Q,U,rop ",
C    s            ropq,ropu,sqrt(ropq*ropq+ropu*ropu)
	 
	 

c**********************************************************************c
c**********************************************************************c
c                                                                      c
c                     example of input cards                           c
c                                                                      c
c 4                            (avhrr observation)                     c
c 7 6 10.1  600  0.0  10.0     (month,day,htu,cn,longan,han)           c
c 8                            (user's   model)                        c
c 3.0   0.35                   ( uh2o(g/cm2) ,uo3(cm-atm) )            c
c 4                            (aerosols model)                        c
c 0.25  0.25  0.25  0.25       ( % of:dust-like,water-sol,oceanic,soot)c
c 23.0                         (visibility (km) )                      c
c -0.5                         (target at 0.5km high)                  c
c -1000                        (sensor aboard a satellite)             c
c 6                            (avhrr 2 (noaa 8) band)                 c
c 1                            (ground type,i.e. non homogeneous)      c
c 2    1    0.50               (target,env.,radius(km) )               c
c -0.10                        (atmospheric correction mode for a TOA  c
c                                   reflectance equal to 0.10)         c
c                                                                      c
c**********************************************************************c
 
 
c**********************************************************************c
c                     print of initial conditions                      c
c                                                                      c
c**********************************************************************c

c ---- geometrical conditions ----
      write(iwr, 98)
      write(iwr, etiq1(igeom+1))
      if(igeom.eq.0) then
	 write(iwr, 1401)
	 write(iwr, 103)month,jday
      endif
      if(igeom.ne.0) write(iwr, 101)month,jday,tu,xlat,xlon
      write(iwr, 102)asol,phi0
      write(iwr, 1110)avis,phiv,adif,phi
 
c --- atmospheric model ----
      write(iwr, 1119)
      if(idatm-7)226,227,228
  228 write(iwr, 1281)uw,uo3
      goto 219
  227 write(iwr, 1272)
      do 229 i=1,34
        write(iwr, 1271)z(i),p(i),t(i),wh(i),wo(i)
  229 continue
      goto 219
  226 write(iwr, 1261)atmid(idatm+1)
 
c --- aerosols model (type) ----

219    write(iwr,5550)
       if(iaer.eq.0) then
        write(iwr, 5554)
        goto 1112
       endif
       
       if (iaer_prof.eq.1) then                           
       
       aer_model(1)="Continental"
       aer_model(2)=" Maritime"
       aer_model(3)="   Urban"
       aer_model(4)="user-defined"
       aer_model(5)="  Desert"
       aer_model(6)="Biomass Burning"
       aer_model(7)="Stratospheric"
       aer_model(8)="user-defined"
       aer_model(9)="user-defined"
       aer_model(10)="user-defined"
       aer_model(11)="Sun Photometer"
       aer_model(12)="user-defined"           

       num_z=num_z-1
       write(6,5551) num_z
       write(6,5552)
       do i=1,num_z
       write(6,5553)i,height_z(num_z+1-i),taer55_z(num_z+1-i),
     a aer_model(iaer)
       enddo
       
       endif
       
       if (iaer_prof.eq.0) then
       
       aer_model(1)="Continental aerosol model"
       aer_model(2)="Maritime aerosol model"
       aer_model(3)="Urban aerosol model"
       aer_model(5)="Desert aerosol model"
       aer_model(6)="Biomass Burning aerosol model"
       aer_model(7)="Stratospheric aerosol model"
       aer_model(11)="Sun Photometer aerosol model"
                
      if (iaer.ge.1.and.iaer.lt.4) write (iwr,132) aer_model(iaer)
      if (iaer.ge.5.and.iaer.le.7) write (iwr,132) aer_model(iaer)
      if (iaer.eq.11) write(iwr,132) aer_model(iaer)
      
      endif
      
       if (iaer.eq.4)write(iwr,133)(c(i),i=1,4)
       if (iaer.eq.8) then
        write(6,134) icp
        do i=1,icp
         write(iwr,135)x1(i),x2(i),cij_out(i)
        enddo
       endif
       if (iaer.eq.9) write(iwr,136)x1(1),x2(1),x3(1)
       if (iaer.eq.10) write(iwr,137)x1(1) 
       if (iaerp.eq.1)write(iwr,139)FILE2(1:i2)
       if (iaer.eq.12)write(iwr,138)FILE2(1:i2)
      

c --- aerosol model (concentration) ----
c --- for the exponential profile ----
      if (iaer_prof.eq.0) then
      if(abs(v).le.xacc) write(iwr, 140)taer55
      if(abs(v).gt.xacc) write(iwr, 141)v,taer55
      endif
1112  write(6,5555)


c --- spectral condition ----
      write(iwr, 148)
      if(iwave.eq.-2) write(iwr, 1510) nsat(1),wlinf,wlsup
      if(iwave.eq.-1) write(iwr, 149) wl
      if(iwave.ge.0) write(iwr, 1510) nsat(iwave+1), wlinf,wlsup

c ---- atmospheric polarization requested
      if (ipol.ne.0)then
	write(iwr, 142)
	if (irop.eq.1) write(iwr,146) ropq,ropq
	if (irop.eq.2) write(iwr,144) pveg*100.0
	if (irop.eq.3) write(iwr,145) wspd,azw
	write(iwr,143) ropq,ropu,sqrt(ropq*ropq+ropu*ropu),
     s	atan2(ropu,ropq)*180.0/3.1415927/2.0
      endif
 
c --- ground reflectance (type and spectral variation) ----
      if(idirec.eq.0) then
        rocave=0.
        roeave=0.
        seb=0.
 
        do 264 i=iinf,isup
          sbor=s(i)
          if(i.eq.iinf.or.i.eq.isup) sbor=sbor*0.5
          wl=.25+(i-1)*step
          call solirr(wl,
     1            swl)
          swl=swl*dsol
          rocave=rocave+rocl(i)*sbor*swl*step
          roeave=roeave+roel(i)*sbor*swl*step
          seb=seb+sbor*swl*step
  264   continue
        rocave=rocave/seb
        roeave=roeave/seb
        isort=0
        ro=rocave
 
        if(inhomo.eq.0) goto 260
        write(iwr, 169)rad
        igroun=igrou1
        ro=rocave
        write(iwr, 170)
        goto 261
 
  262   igroun=igrou2
        ro=roeave
        write(iwr, 171)
        goto 261
 
  260   write(iwr, 168)
  261   if (igroun.gt.0)write(iwr, reflec(igroun+3))ro
        if (igroun.gt.0)goto 158
        if(igroun.eq.-1) write(iwr, reflec(1))ro
        if(igroun.eq.-1) goto 158
        if(iwave.eq.-1)  write(iwr, reflec(2))ro
        if(iwave.ne.-1)  write(iwr, reflec(3))ro
 158    isort=isort+1
        if(inhomo.eq.0) goto 999
        if(isort.eq.2) goto 999
        goto 262
      else
        write(iwr, 168)
        if(idirec.eq.1) then
        rocave=0.
        rfoamave=0.
        rwatave=0.
        rglitave=0.
        seb=0.
 
        do  i=iinf,isup
          sbor=s(i)
          if(i.eq.iinf.or.i.eq.isup) sbor=sbor*0.5
          wl=.25+(i-1)*step
          call solirr(wl,
     1            swl)
          swl=swl*dsol
          rocave=rocave+rocl(i)*sbor*swl*step
          rfoamave=rfoamave+rfoaml(i)*sbor*swl*step
          rwatave=rwatave+rwatl(i)*sbor*swl*step
          rglitave=rglitave+rglitl(i)*sbor*swl*step
          seb=seb+sbor*swl*step
        enddo
        rocave=rocave/seb
	rfoamave=rfoamave/seb
	rwatave=rwatave/seb
	rglitave=rglitave/seb
	
         goto(2000,2001,2002,2003,2004,2005,2006,2007,2008,2010,2011)
     *    ,(ibrdf+1)
 2000    write(iwr, 190)
         write(iwr, 187)
     *rocave,robar1/xnorm1,robar2/xnorm2,albbrdf
         goto 2009
 2001    write(iwr, 191)par1,par2,par3,par4
         write(iwr, 187)
     *rocave,robar1/xnorm1,robar2/xnorm2,albbrdf
         goto 2009
 2002    write(iwr, 192)optics(1),struct(1),struct(2)
         if (options(5).eq.0) write(iwr, 200)
         if (options(5).eq.1) write(iwr, 201)
         if (options(3).eq.0) write(iwr, 197)struct(3),struct(4)
         if (options(3).eq.1) write(iwr, 198)struct(3)
         if (options(3).eq.2) write(iwr, 199)struct(3)
         if (options(4).eq.0) write(iwr, 202)
         if (options(4).eq.1) write(iwr, 203)optics(2)
         if (options(4).eq.2) write(iwr, 204)optics(2),optics(3)
         write(iwr, 187)
     *rocave,robar1/xnorm1,robar2/xnorm2,albbrdf
         goto 2009
 2003    write(iwr, 193)par1,par2,par3
         write(iwr, 187)
     *rocave,robar1/xnorm1,robar2/xnorm2,albbrdf
         goto 2009
 2004    write(iwr, 194)par1,par2,par3,par4
         write(iwr, 187)
     *rocave,robar1/xnorm1,robar2/xnorm2,albbrdf
         goto 2009
 2005    write(iwr, 195)par1,par2
         write(iwr, 187)
     *rocave,robar1/xnorm1,robar2/xnorm2,albbrdf
         goto 2009
 2006    write(iwr, 196)pws,phi_wind,xsal,pcl
         write(iwr,500) rfoamave,rwatave,rglitave
         write(iwr, 187)
     *rocave,robar1/xnorm1,robar2/xnorm2,albbrdf
         goto 2009
 2007    write(iwr, 205) pRl,pTl,pRs,PxLt
         if (pihs.eq.0) then
           write(iwr,207)' no hot spot       '
         else
           write(iwr,208)' hot spot parameter',pc
         endif
         if (pild.eq.1) write(iwr,209) ' planophile   leaf distribution'
         if (pild.eq.2) write(iwr,209) ' erectophile  leaf distribution'
         if (pild.eq.3) write(iwr,209) ' plagiophile  leaf distribution'
         if (pild.eq.4) write(iwr,209) ' extremophile leaf distribution'
         if (pild.eq.5) write(iwr,209) ' uniform      leaf distribution'
         write(iwr, 187)
     *rocave,robar1/xnorm1,robar2/xnorm2,albbrdf
         goto 2009
 2008    write(iwr, 206) par1,par2,par3
         goto 2009
 2010    write(iwr, 210)uli,eei,thmi,sli,cabi,cwi,vaii,rnci,rsl1i
         write(iwr, 187)
     *   rocave,robar1/xnorm1,robar2/xnorm2,albbrdf
         goto 2009
 2011    write(iwr, 211)p1,p2,p3
         write(iwr, 187)
     *   rocave,robar1/xnorm1,robar2/xnorm2,albbrdf
         goto 2009
 2009   endif
      endif
  50  continue

c --- pressure at ground level (174) and altitude (175) ----
  999 write(iwr, 173)
      write(iwr, 174)p(1)
      write(iwr, 175)xps
      if (xps.gt.0..and.idatm.ne.0) write(iwr, 176)uw,uo3
 
c --- plane simulation output if selected ----
      if (palt.lt.1000.) then
       write(iwr, 178)
       write(iwr, 179)pps
       write(iwr, 180)zpl(34)
       write(iwr, 181)
       write(iwr, 182)puo3
       write(iwr, 183)puw
       write(iwr, 184)taer55p
      endif
 
c ---- atmospheric correction  ----
      if (irapp.ge.0) then
        write(iwr, 177)
          if (irapp.eq. 0) write(iwr, 220)
          if (irapp.eq. 1) write(iwr, 221)
       if (rapp.lt.0.) then
        write(iwr, 185)-rapp
       else
        write(iwr, 186)rapp
       endif
      endif
      write(iwr, 172)
c**********************************************************************c
c                                                                      c
c                                                                      c
c                     start of computations                            c
c                                                                      c
c                                                                      c
c                                                                      c
c**********************************************************************c

c ---- initilialization
C Start Update Look up table	
	do i=1,mu
	do j=1,41
	roluti(i,j)=0.0
	rolutiq(i,j)=0.0
	rolutiu(i,j)=0.0
	enddo
	enddo
C End Update Look up table	
      sb=0.
      seb=0.
      refet=0.
      refet1=0.
      refet2=0.
      refet3=0.
      rpfet=0.
      rpfet1=0.
      rpfet2=0.
      rpfet3=0.
      alumet=0.
      plumet=0.
      tgasm=0.
      rog=0.
      dgasm=0.
      ugasm=0.
      sdwava=0.
      sdozon=0.
      sddica=0.
      sdoxyg=0.
      sdniox=0.
      sdmoca=0.
      sdmeth=0.
      suwava=0.
      suozon=0.
      sudica=0.
      suoxyg=0.
      suniox=0.
      sumoca=0.
      sumeth=0.
      stwava=0.
      stozon=0.
      stdica=0.
      stoxyg=0.
      stniox=0.
      stmoca=0.
      stmeth=0.
      sodray=0.
      sodrayp=0.
      sodaer=0.
      sodaerp=0.
      sodtot=0.
      sodtotp=0.
      fophsr=0.
      fophsa=0.
      foqhsr=0.
      foqhsa=0.
      fouhsr=0.
      fouhsa=0.
      sroray=0.
      sroaer=0.
      srotot=0.
      srpray=0.
      srpaer=0.
      srptot=0.
      srqray=0.
      srqaer=0.
      srqtot=0.
      sruray=0.
      sruaer=0.
      srutot=0.
      ssdaer=0.
      sdtotr=0.
      sdtota=0.
      sdtott=0.
      sutotr=0.
      sutota=0.
      sutott=0.
      sasr=0.
      sasa=0.
      sast=0.
      do 52 i=1,2
        do 53 j=1,3
          ani(i,j)=0.
          aini(i,j)=0.
          anr(i,j)=0.
          ainr(i,j)=0.
   53   continue
   52 continue

c ---- spectral loop ----
      if (iwave.eq.-2) write(iwr,1500)
        do 51 l=iinf,isup
        sbor=s(l)
        if(l.eq.iinf.or.l.eq.isup) sbor=sbor*0.5
        if(iwave.eq.-1) sbor=1.0/step
        roc=rocl(l)
        roe=roel(l)
        wl=.25+(l-1)*step
c
        call abstra(idatm,wl,xmus,xmuv,uw/2.,uo3,uwus,uo3us,
     a             idatmp,puw/2.,puo3,puwus,puo3us,
     a      dtwava,dtozon,dtdica,dtoxyg,dtniox,dtmeth,dtmoca,
     a      utwava,utozon,utdica,utoxyg,utniox,utmeth,utmoca,
     a      attwava,ttozon,ttdica,ttoxyg,ttniox,ttmeth,ttmoca )
        call abstra(idatm,wl,xmus,xmuv,uw,uo3,uwus,uo3us,
     a             idatmp,puw,puo3,puwus,puo3us,
     a      dtwava,dtozon,dtdica,dtoxyg,dtniox,dtmeth,dtmoca,
     a      utwava,utozon,utdica,utoxyg,utniox,utmeth,utmoca,
     a      ttwava,ttozon,ttdica,ttoxyg,ttniox,ttmeth,ttmoca )
        if (dtwava.lt.accu3) dtwava=0.
        if (dtozon.lt.accu3) dtozon=0.
        if (dtdica.lt.accu3) dtdica=0.
        if (dtniox.lt.accu3) dtniox=0.
        if (dtmeth.lt.accu3) dtmeth=0.
        if (dtmoca.lt.accu3) dtmeth=0.
        if (utwava.lt.accu3) utwava=0.
        if (utozon.lt.accu3) utozon=0.
        if (utdica.lt.accu3) utdica=0.
        if (utniox.lt.accu3) utniox=0.
        if (utmeth.lt.accu3) utmeth=0.
        if (utmoca.lt.accu3) utmeth=0.
        if (ttwava.lt.accu3) ttwava=0.
        if (ttozon.lt.accu3) ttozon=0.
        if (ttdica.lt.accu3) ttdica=0.
        if (ttniox.lt.accu3) ttniox=0.
        if (ttmeth.lt.accu3) ttmeth=0.
        if (ttmoca.lt.accu3) ttmeth=0.
c
        call solirr(wl,
     s            swl)
        swl=swl*dsol
        coef=sbor*step*swl
        coefp=sbor*step
        call interp(iaer,idatmp,wl,taer55,taer55p,xmud,romix,
     s   rorayl,roaero,phaa,phar,rqmix,rqrayl,rqaero,qhaa,qhar,
     s   rumix,rurayl,ruaero,uhaa,uhar,
     s   tsca,tray,trayp,taer,taerp,dtott,utott,astot,asray,asaer,
     s   utotr,utota,dtotr,dtota,ipol,roatm_fi,romix_fi,rorayl_fi,nfi,
     s   roluts,rolut,rolutsq,rolutq,rolutsu,rolutu,nfilut)
     
        dgtot=dtwava*dtozon*dtdica*dtoxyg*dtniox*dtmeth*dtmoca
        tgtot=ttwava*ttozon*ttdica*ttoxyg*ttniox*ttmeth*ttmoca
        ugtot=utwava*utozon*utdica*utoxyg*utniox*utmeth*utmoca
        tgp1=ttozon*ttdica*ttoxyg*ttniox*ttmeth*ttmoca
        tgp2=attwava*ttozon*ttdica*ttoxyg*ttniox*ttmeth*ttmoca


CC--- computing integrated values over the spectral band------
        sb=sb+sbor*step
        seb=seb+coef

c  ---unpolarized light
          edifr=utotr-exp(-trayp/xmuv)
          edifa=utota-exp(-taerp/xmuv)
        if (idirec.eq.1) then
          tdird=exp(-(trayp+taerp)/xmus)
          tdiru=exp(-(trayp+taerp)/xmuv)
          tdifd=dtott-tdird
          tdifu=utott-tdiru
	  rsurf=roc*tdird*tdiru+
     s          robar(l)*tdifd*tdiru+robarp(l)*tdifu*tdird+
     s          robard(l)*tdifd*tdifu+
     s    (tdifd+tdird)*(tdifu+tdiru)*astot*robard(l)*robard(l)
     s          /(1.-astot*robard(l))
        avr=robard(l)
        else
          call enviro(edifr,edifa,rad,palt,xmuv,fra,fae,fr)
          avr=roc*fr+(1.-fr)*roe
          rsurf=roc*dtott*exp(-(trayp+taerp)/xmuv)/(1.-avr*astot)
     s       +avr*dtott*(utott-exp(-(trayp+taerp)/xmuv))/(1.-avr*astot)
        endif
        ratm1=(romix-rorayl)*tgtot+rorayl*tgp1
        ratm3=romix*tgp1
        ratm2=(romix-rorayl)*tgp2+rorayl*tgp1
	do i=1,nfi
	ratm2_fi(i)=(romix_fi(i)-rorayl_fi(i))*tgp2+rorayl_fi(i)*tgp1
	enddo
        romeas1=ratm1+rsurf*tgtot
        romeas2=ratm2+rsurf*tgtot
        romeas3=ratm3+rsurf*tgtot
c    computing integrated values over the spectral band

        alumeas=xmus*swl*romeas2/pi
        alumet=alumet+alumeas*sbor*step
	rfoamave=rfoamave+rfoaml(i)*sbor*swl*step
	rwatave=rwatave+rwatl(i)*sbor*swl*step
	rglitave=rglitave+rglitl(i)*sbor*swl*step
        rog=rog+roc*coef
        refet=refet+romeas2*coef
        refet1=refet1+romeas1*coef
        refet2=refet2+romeas2*coef
        refet3=refet3+romeas3*coef
	do i=1,nfi
	refet_fi(i)=refet_fi(i)+ratm2_fi(i)*coef
	enddo
	
C Start Update Look up table	
C	do i=1,mu
C	do j=1,41
C	roluti(i,j)=roluti(i,j)+rolut(i,j)*coef
C	rolutiq(i,j)=rolutiq(i,j)+rolutq(i,j)*coef
C	rolutiu(i,j)=rolutiu(i,j)+rolutu(i,j)*coef
C	enddo
C	enddo
C End Update Look up table	
	
	
	
        if (iwave.eq.-2) then
          write(iwr,1501) wl,tgtot,dtott,utott,astot,ratm2,swl,roc,
     s            sbor,dsol,romeas2
        endif

c  ---polarized light:
c       -the spectral integration without the solar irradiance
c           because the sun does not generate polarized light
c       -we assume a Lambertian ground, then no polarized 
c           surface reflectance (rpsurf=0.0, avr=0.0, roc=0.0)
	if (ipol.ne.0)then
          rqatm2=(rqmix-rqrayl)*tgp2+rqrayl*tgp1
          ruatm2=(rumix-rurayl)*tgp2+rurayl*tgp1
	  
          tdirqu=exp(-(trayp+taerp)*(1./xmuv+1./xmus))
	  rqmeas2=rqatm2+ropq*tgtot*tdirqu
	  rumeas2=ruatm2+ropu*tgtot*tdirqu

          qlumeas=xmus*swl*rqmeas2/pi
          ulumeas=xmus*swl*rumeas2/pi
	  qlumet=qlumet+qlumeas*coefp
	  ulumet=ulumet+ulumeas*coefp
	  
          foqhsa=foqhsa+qhaa*coef
          foqhsr=foqhsr+qhar*coef
          fouhsa=fouhsa+uhaa*coef
          fouhsr=fouhsr+uhar*coef
          srqray=srqray+rqrayl*coef
          srqaer=srqaer+rqaero*coef
          srqtot=srqtot+rqmix*coef
          sruray=sruray+rurayl*coef
          sruaer=sruaer+ruaero*coef
          srutot=srutot+rumix*coef
          rqfet=rqfet+rqmeas2*coefp
          rufet=rufet+rumeas2*coefp

C Start Update Look up table	
	do i=1,mu
	do j=1,41
	roluti(i,j)=roluti(i,j)+rolut(i,j)*coef
	rolutiq(i,j)=rolutiq(i,j)+rolutq(i,j)*coef
	rolutiu(i,j)=rolutiu(i,j)+rolutu(i,j)*coef
	enddo
	enddo
C End Update Look up table	

        endif

C  ---gazes and other characteritics used in both light
        srotot=srotot+(romix)*coef
        fophsa=fophsa+phaa*coef
        fophsr=fophsr+phar*coef
        sroray=sroray+rorayl*coef
        sroaer=sroaer+roaero*coef

        sasr=sasr+asray*coef
        sasa=sasa+asaer*coef
        sast=sast+astot*coef
        sodray=sodray+tray*coef
        sodaer=sodaer+taer*coef
        sodrayp=sodrayp+trayp*coef
        sodaerp=sodaerp+taerp*coef
        ssdaer=ssdaer+tsca*coef
        sodtot=sodtot+(taer+tray)*coef
        sodtotp=sodtotp+(taerp+trayp)*coef
        tgasm=tgasm+tgtot*coef
        dgasm=dgasm+dgtot*coef
        ugasm=ugasm+ugtot*coef
        sdwava=sdwava+dtwava*coef
        sdozon=sdozon+dtozon*coef
        sddica=sddica+dtdica*coef
        sdoxyg=sdoxyg+dtoxyg*coef
        sdniox=sdniox+dtniox*coef
        sdmeth=sdmeth+dtmeth*coef
        sdmoca=sdmoca+dtmoca*coef
        suwava=suwava+utwava*coef
        suozon=suozon+utozon*coef
        sudica=sudica+utdica*coef
        suoxyg=suoxyg+utoxyg*coef
        suniox=suniox+utniox*coef
        sumeth=sumeth+utmeth*coef
        sumoca=sumoca+utmoca*coef
        stwava=stwava+ttwava*coef
        stozon=stozon+ttozon*coef
        stdica=stdica+ttdica*coef
        stoxyg=stoxyg+ttoxyg*coef
        stniox=stniox+ttniox*coef
        stmeth=stmeth+ttmeth*coef
        stmoca=stmoca+ttmoca*coef
        sdtotr=sdtotr+dtotr*coef
        sdtota=sdtota+dtota*coef
        sdtott=sdtott+dtott*coef
        sutotr=sutotr+utotr*coef
        sutota=sutota+utota*coef
        sutott=sutott+utott*coef

c  ---output at the ground level.
        tdir=exp(-(tray+taer)/xmus)
        tdif=dtott-tdir
        etn=dtott*dgtot/(1.-avr*astot)
        esn=tdir*dgtot
        es=tdir*dgtot*xmus*swl
        ea0n=tdif*dgtot
        ea0=tdif*dgtot*xmus*swl
        ee0n=dgtot*avr*astot*dtott/(1.-avr*astot)
        ee0=xmus*swl*dgtot*avr*astot*dtott/(1.-avr*astot)
        if (etn.gt.accu3) then
           ani(1,1)=esn/etn
           ani(1,2)=ea0n/etn
           ani(1,3)=ee0n/etn
        else
           ani(1,1)=0.
           ani(1,2)=0.
           ani(1,3)=0.
        endif
        ani(2,1)=es
        ani(2,2)=ea0
        ani(2,3)=ee0
        do 955 j=1,3
          aini(1,j)=aini(1,j)+ani(1,j)*coef
          aini(2,j)=aini(2,j)+ani(2,j)*sbor*step
  955   continue
 
c  ---output at satellite level
C old version is commented (new changes are immediately below 
C Jan-15-2004
C        tmdir=exp(-(tray+taerp)/xmuv)
        tmdir=exp(-(trayp+taerp)/xmuv)
        tmdif=utott-tmdir
        xla0n=ratm2
        xla0=xla0n*xmus*swl/pi
        xltn=roc*dtott*tmdir*tgtot/(1.-avr*astot)
        xlt=xltn*xmus*swl/pi
        xlen=avr*dtott*tmdif*tgtot/(1.-avr*astot)
        xle=xlen*xmus*swl/pi
        anr(1,1)=xla0n
        anr(1,2)=xlen
        anr(1,3)=xltn
        anr(2,1)=xla0
        anr(2,2)=xle
        anr(2,3)=xlt
        do 56 j=1,3
          ainr(1,j)=ainr(1,j)+anr(1,j)*coef
          ainr(2,j)=ainr(2,j)+anr(2,j)*sbor*step
   56   continue
   51   continue
 
cc---- integrated values of apparent reflectance, radiance          ---- 
cc---- and gaseous transmittances (total,downward,separately gases) ----



      tgasm=tgasm/seb
      dgasm=dgasm/seb
      ugasm=ugasm/seb
      sasa=sasa/seb
      sasr=sasr/seb
      sast=sast/seb
      sdniox=sdniox/seb
      sdmoca=sdmoca/seb
      sdmeth=sdmeth/seb
      sdwava=sdwava/seb
      sdozon=sdozon/seb
      sddica=sddica/seb
      suniox=suniox/seb
      sumoca=sumoca/seb
      sumeth=sumeth/seb
      suwava=suwava/seb
      suozon=suozon/seb
      sudica=sudica/seb
      suoxyg=suoxyg/seb
      sdoxyg=sdoxyg/seb
      stniox=stniox/seb
      stmoca=stmoca/seb
      stmeth=stmeth/seb
      stwava=stwava/seb
      stozon=stozon/seb
      stdica=stdica/seb
      stoxyg=stoxyg/seb
      sdtotr=sdtotr/seb
      sdtota=sdtota/seb
      sdtott=sdtott/seb
      sutotr=sutotr/seb
      sutota=sutota/seb
      sutott=sutott/seb
      sodray=sodray/seb
      sodaer=sodaer/seb
      sodtot=sodtot/seb
      sodrayp=sodrayp/seb
      sodaerp=sodaerp/seb
      sodtotp=sodtotp/seb
      pizera=0.0
      pizerr=1.
      if(iaer.ne.0) pizera=ssdaer/sodaer/seb
      pizert=(pizerr*sodray+pizera*sodaer)/(sodray+sodaer)
      
      
      rfoamave=rfoamave/seb
      rwatave=rwatave/seb
      rglitave=rglitave/seb


      sroray=sroray/seb
      sroaer=sroaer/seb
      srotot=srotot/seb
      fophsa=fophsa/seb
      fophsr=fophsr/seb
      fophst=(sodray*fophsr+sodaer*fophsa)/(sodray+sodaer)
 
c  ---unpolarized light
        refet=refet/seb
        refet1=refet1/seb
        refet2=refet2/seb
        refet3=refet3/seb
        rog=rog/seb
        alumet=alumet/sb

c  ---polarized light
      if (ipol.ne.0)then
	rqfet=rqfet/sb
	rufet=rufet/sb
	
 	srqray=srqray/seb
 	srqaer=srqaer/seb
 	srqtot=srqtot/seb
 	sruray=sruray/seb
 	sruaer=sruaer/seb
 	srutot=srutot/seb
	plumet=plumet/sb
 	foqhsa=foqhsa/seb
 	foqhsr=foqhsr/seb
        foqhst=(sodray*foqhsr+sodaer*foqhsa)/(sodray+sodaer)
 	fouhsa=fouhsa/seb
 	fouhsr=fouhsr/seb
        fouhst=(sodray*fouhsr+sodaer*fouhsa)/(sodray+sodaer)
c      we define the polarized reflectances
	srpray=sqrt(srqray**2.+sruray**2.)
 	srpaer=sqrt(srqaer**2.+sruaer**2.)
	srptot=sqrt(srqtot**2.+srutot**2.)
c      we define the primary degrees of polarization
	spdpray=foqhsr/fophsr
	if (iaer.ne.0) then
	 spdpaer=foqhsa/fophsa
	else
	 spdpaer=0.0
	endif
	spdptot=foqhst/fophst
c      we define the degrees of polarization
	sdpray=100.*srpray/sroray
	if (sroaer.ne.0) then
	 sdpaer=100.*srpaer/sroaer
	else sdpaer=0.0
	endif 
	sdptot=100.*srptot/srotot
c      and we compute the direction of the plane of polarization
	call dirpopol(srqray*xmus,sruray*xmus,sdppray)
	call dirpopol(srqaer*xmus,sruaer*xmus,sdppaer)
	call dirpopol(srqtot*xmus,srutot*xmus,sdpptot)
CC	ksirad=sdpptot*3.1415927/180.
CC	refeti=refet+pinst*rpfet*cos(2*(ksiinst*3.1415925/180.+ksirad))
      endif

      do 57 j=1,3
c  ---output at the ground level.
        aini(1,j)=aini(1,j)/seb
        aini(2,j)=aini(2,j)/sb
c  ---output at satellite level
        ainr(1,j)=ainr(1,j)/seb
        ainr(2,j)=ainr(2,j)/sb
   57 continue

c**********************************************************************c
c                                                                      c
c                       print of final results                         c
c                                                                      c
c**********************************************************************c
C begining case for a lut output
C SIMPLE LUT in azimuth
      if (ilut.eq.2) then
          do ifi=1,nfi
	  xtphi=(ifi-1)*180.0/(nfi-1)
	  write(6,*) "lutfi ",xtphi,ratm2_fi(ifi)
	  enddo
      endif	  

C LUT FOR Look up table data        
      if (ilut.eq.1) then
      its=acos(xmus)*180.0/pi
      open(10,file='rotoa_bs',ACCESS='APPEND')
      write(10,2222) "AERO-LUT Lambda min,max ",wlinf,wlsup
 2222 Format(A28,3(F10.7,1X))      
      write(10,2222) "Tau-Lambda,Tau550 asol  ",sodaer,taer55,asol
      aerod=0
      if (iaer.eq.12) then
      write(10,2223) "aerosol model ",FILE2(1:i2)
      aerod=1
      endif
      if (iaer.eq.1) then
      write(10,2223) "aerosol model ","CONTINENTAL"
      aerod=1
      endif
      if (iaer.eq.2) then
      write(10,2223) "aerosol model ","MARITIME"
      aerod=1
      endif
      if (iaer.eq.3) then
      write(10,2223) "aerosol model ","URBAN"
      aerod=1
      endif
      if (iaer.eq.5) then
      write(10,2223) "aerosol model ","DESERTIC"
      aerod=1
      endif
      if (iaer.eq.6) then
      write(10,2223) "aerosol model ","SMOKE"
      aerod=1
      endif
      if (iaer.eq.7) then
      write(10,2223) "aerosol model ","STRATOSPHERIC"
      aerod=1
      endif
      if (aerod.eq.0) then
      write(10,2223) "aerosol model ","UNDEFINED"
      endif
 2223 format(A24,1X,A80)      
      lutmuv=cos(avis*pi/180.)
      cscaa=-xmus*lutmuv-cos(filut(mu,1)*pi/180.)*sqrt(1.-xmus*xmus)
     S  *sqrt(1.-lutmuv*lutmuv)
      iscama=acos(cscaa)*180./pi
      cscaa=-xmus*lutmuv-cos(filut(mu,nfilut(mu))*pi/180.)
     S  *sqrt(1.-xmus*xmus)*sqrt(1.-lutmuv*lutmuv)
      iscami=acos(cscaa)*180./pi
      write(10,333) its,avis,nfilut(mu),iscama,iscami
      write(10,'(41(F8.5,1X))')(roluti(mu,j)/seb,j=1,nfilut(mu))
C      write(10,'(41(F8.5,1X))')(rolutiq(mu,j)/seb,j=1,nfilut(mu))
C      write(10,'(41(F8.5,1X))')(rolutiu(mu,j)/seb,j=1,nfilut(mu))
      do i=1,mu-1
      lutmuv=rm(i)
      luttv=acos(lutmuv)*180./pi
      cscaa=-xmus*lutmuv-cos(filut(i,1)*pi/180.)*sqrt(1.-xmus*xmus)
     S  *sqrt(1.-lutmuv*lutmuv)
      iscama=acos(cscaa)*180./pi
      cscaa=-xmus*lutmuv-cos(filut(i,nfilut(i))*pi/180.)
     S  *sqrt(1.-xmus*xmus)*sqrt(1.-lutmuv*lutmuv)
      iscami=acos(cscaa)*180./pi
      write(10,333) its,luttv,nfilut(i),iscama,iscami
 333  Format(F10.5,1X,F10.5,1X,I3,F10.5,F10.5)    
      write(10,'(41(F8.5,1X))')(roluti(i,j)/seb,j=1,nfilut(i))
C      write(10,'(41(F8.5,1X))')(rolutiq(i,j)/seb,j=1,nfilut(i))
C      write(10,'(41(F8.5,1X))')(rolutiu(i,j)/seb,j=1,nfilut(i))
      enddo
      close(10)
      endif
C Case a LUT output is desired

C Case for an aps LUT
      if (ilut.eq.3) then
      its=acos(xmus)*180.0/pi
      open(10,file='rotoa_aps_bs',ACCESS='APPEND')
      write(10,2222) "AERO-LUT Lambda min,max ",wlinf,wlsup
      write(10,2222) "Tau-Lambda,Tau550 asol  ",sodaer,taer55,asol
      aerod=0
      if (iaer.eq.12) then
      write(10,2223) "aerosol model ",FILE2(1:i2)
      aerod=1
      endif
      if (iaer.eq.1) then
      write(10,2223) "aerosol model ","CONTINENTAL"
      aerod=1
      endif
      if (iaer.eq.2) then
      write(10,2223) "aerosol model ","MARITIME"
      aerod=1
      endif
      if (iaer.eq.3) then
      write(10,2223) "aerosol model ","URBAN"
      aerod=1
      endif
      if (iaer.eq.5) then
      write(10,2223) "aerosol model ","DESERTIC"
      aerod=1
      endif
      if (iaer.eq.6) then
      write(10,2223) "aerosol model ","SMOKE"
      aerod=1
      endif
      if (iaer.eq.7) then
      write(10,2223) "aerosol model ","STRATOSPHERIC"
      aerod=1
      endif
      if (aerod.eq.0) then
      write(10,2223) "aerosol model ","UNDEFINED"
      endif

C 
      dtr=atan(1.)*4./180.
      write(10,'(A5,1X,41(F8.4,1X))') 'phi',(filut(i,1),i=16,1,-1),
     S                      filut(mu,1),(filut(i,2),i=1,16)
     
      write(10,'(A5,1X,41(F8.5,1X))') 'tv',(acos(rm(i))/dtr,i=16,1,-1)
     S  ,acos(rm(0))/dtr,(acos(rm(k))/dtr,k=1,16)
     
      write(10,'(41(F8.5,1X))')(roluti(i,1)/seb,i=16,1,-1)
     S     ,roluti(mu,1)/seb ,(roluti(i,2)/seb,i=1,16)
      write(10,'(41(F8.5,1X))')(rolutiq(i,1)/seb,i=16,1,-1)
     S     ,rolutiq(mu,1)/seb,(rolutiq(i,2)/seb,i=1,16)
      write(10,'(41(F8.5,1X))')(rolutiu(i,1)/seb,i=16,1,-1)
     S     ,rolutiu(mu,1)/seb,(rolutiu(i,2)/seb,i=1,16)
      close(10)
      endif
C Case a LUT output is desired      

 160  continue
 
        write(iwr, 430 )refet,alumet,tgasm
        write(iwr, 431 )refet1,refet2,refet3

      if (ipol.eq.1)then
        rpfet=sqrt(rqfet*rqfet+rufet*rufet)
	plumet=sqrt(qlumet*qlumet+ulumet*ulumet)
	xpol=atan2(rufet,rqfet)*180.0/3.14159/2.
        write(iwr, 429 )rpfet,plumet,xpol,rpfet/refet
C       write(iwr, 428 )rpfet1,rpfet2,rpfet3
      endif
 
        if(inhomo.ne.0) then
          write(iwr, 432)(aini(1,j),j=1,3),'environment','target',
     s          (ainr(1,j),j=1,3)
          write(iwr, 434)(aini(2,j),j=1,3),'environment','target',
     s         (ainr(2,j),j=1,3)

        endif
        if(inhomo.eq.0) then
          write(iwr, 432)(aini(1,j),j=1,3),'background ','pixel ',
     s		(ainr(1,j),j=1,3)
          write(iwr, 434)(aini(2,j),j=1,3),'background ','pixel ',
     s         (ainr(2,j),j=1,3)
        endif
      
      if (iwave.eq.-1)then
        write(iwr, 436)seb
      else
        write(iwr, 437)sb,seb
      endif

c**********************************************************************c
c                                                                      c
c                    print of complementary results                    c
c                                                                      c
c**********************************************************************c
      write(iwr, 929)
      write(iwr, 930)
      write(iwr, 931)'global gas. trans. :',dgasm,ugasm,tgasm
      write(iwr, 931)'water   "     "    :',sdwava,suwava,stwava
      write(iwr, 931)'ozone   "     "    :',sdozon,suozon,stozon
      write(iwr, 931)'co2     "     "    :',sddica,sudica,stdica
      write(iwr, 931)'oxyg    "     "    :',sdoxyg,suoxyg,stoxyg
      write(iwr, 931)'no2     "     "    :',sdniox,suniox,stniox
      write(iwr, 931)'ch4     "     "    :',sdmeth,sumeth,stmeth
      write(iwr, 931)'co      "     "    :',sdmoca,sumoca,stmoca
      write(iwr, 1401)
      write(iwr, 1401)
 
      write(iwr, 931)'rayl.  sca. trans. :',sdtotr,sutotr,sutotr*sdtotr
      write(iwr, 931)'aeros. sca.   "    :',sdtota,sutota,sutota*sdtota
      write(iwr, 931)'total  sca.   "    :',sdtott,sutott,sutott*sdtott
      write(iwr, 1401)
      write(iwr, 1401)
 
      write(iwr, 939)
      write(iwr, 931)'spherical albedo   :',sasr,sasa,sast
      write(iwr, 931)'optical depth total:',sodray,sodaer,sodtot
      write(iwr, 931)'optical depth plane:',sodrayp,sodaerp,sodtotp
      if (ipol.eq.0) then
        write(iwr, 931)'reflectance        :',sroray,sroaer,srotot
        write(iwr, 931)'phase function     :',fophsr,fophsa,fophst
      else 
        write(iwr, 931)'reflectance I      :',sroray,sroaer,srotot
        write(iwr, 931)'reflectance Q      :',srqray,srqaer,srqtot
        write(iwr, 931)'reflectance U      :',sruray,sruaer,srutot
        write(iwr, 931)'polarized reflect. :',srpray,srpaer,srptot
        write(iwr, 932)'degree of polar.   :',sdpray,sdpaer,sdptot
        write(iwr, 932)'dir. plane polar.  :',sdppray,sdppaer,sdpptot
CCC	write(iwr, 931)'instrument app ref.:',zero,zero,refeti
        write(iwr, 931)'phase function I   :',fophsr,fophsa,fophst
        write(iwr, 931)'phase function Q   :',foqhsr,foqhsa,foqhst
        write(iwr, 931)'phase function U   :',fouhsr,fouhsa,fouhst
        write(iwr, 931)'primary deg. of pol:',spdpray,spdpaer,spdptot
      endif
      write(iwr, 931)'sing. scat. albedo :',pizerr,pizera,pizert
      write(iwr, 1401)
      write(iwr, 1402)
 
c**********************************************************************c
c                                                                      c
c                    atmospheric correction                            c
c                                                                      c
c**********************************************************************c
       if (irapp.ge.0) then
	 if (rapp.ge.0.) then
	    xrad=rapp
	    rapp=pi*xrad*sb/xmus/seb
	 else
	    rapp=-rapp
	    xrad=xmus*seb*(rapp)/pi/sb
	 endif
         rog=rapp/tgasm
         rog=(rog-ainr(1,1)/tgasm)/sutott/sdtott
         rog=rog/(1.+rog*sast)
	 xa=pi*sb/xmus/seb/tgasm/sutott/sdtott
	 xap=1./tgasm/sutott/sdtott
	 xb=ainr(1,1)/sutott/sdtott/tgasm
	 xb=ainr(1,1)/sutott/sdtott/tgasm
	 xc=sast
c        BRDF coupling correction 
         if (idirec.eq.1) then 
c compute the coefficient of the 2nd degree equation
C  a*(ros^2)+b*ros+c	
C   compute ratios and transmissions
         robarstar=(robar1/xnorm1)/brdfints(mu,1)
	 robarpstar=(robar2/xnorm2)/brdfints(mu,1)
	 robarbarstar=albbrdf/brdfints(mu,1)
	 tdd=exp(-sodtot/xmus)
	 tdu=exp(-sodtot/xmuv)
	 tsd=sdtott-tdd
	 tsu=sutott-tdu
	 
c compute coefficients
	 
	 coefc=-(rapp/tgasm-ainr(1,1)/tgasm)
	  
	 coefb=tdd*tdu+tdu*tsd*robarstar+tsu*tdd*robarpstar
	 coefb=coefb+tsu*tsd*robarbarstar
	 
	 coefa=sdtott*sutott*sast*robarbarstar*robarbarstar
	 coefa=coefa/(1-sast*(rog/brdfints(mu,1)*albbrdf))
	 
c solve equations, compute solutions
         discri=sqrt(coefb*coefb-4*coefa*coefc)
         rogbrdf=(-coefb+discri)/(2*coefa)

c second pass use update value for rog
         coefa=sdtott*sutott*sast*robarbarstar*robarbarstar  
         coefa=coefa/(1-sast*(rogbrdf/brdfints(mu,1)*albbrdf))
         discri=sqrt(coefb*coefb-4*coefa*coefc)
         rogbrdf=(-coefb+discri)/(2*coefa)
	 else
	 rogbrdf=rog
	 endif
	 
         write(iwr, 940)
         write(iwr, 941)rapp
         write(iwr, 942)xrad
	 if (irapp.eq.0) then  
         write(iwr, 943)rog
         write(iwr, 944)xa,xb,xc
	 else
	 write(iwr,222)rog,rogbrdf
	 endif
	 
         write(iwr, 944)xa,xb,xc
C         write(iwr, *) "david roy ", xap,xb,xc
         y=xa*xrad-xb
c        write(6,'(A5,F9.5)') 'rog=', rog
c        write(6,'(A5,F9.5,A8,F9.5)') 'y=',y, '  acr=',y/(1.+xc*y)
c        write(6,*) 'rogbrdf=',rogbrdf,' rodir=',brdfints(mu,1),
c    s            ' diff=',rogbrdf-brdfints(mu,1)
      endif
      stop
 
c**********************************************************************c
c                                                                      c
c                   output editing formats                             c
c                                                                      c
c                                                                      c
c**********************************************************************c
   98 format(/////,1h*,30(1h*),18h 6sV version 1.0B ,30(1h*),t79               
     s       ,1h*,/,1h*,t79,1h*,/,
     s       1h*,22x,34h geometrical conditions identity  ,t79,1h*,/,
     s       1h*,22x,34h -------------------------------  ,t79,1h*)
  101 format(1h*,15x,7h month:,i3,7h day : ,i3,
     s                 16h universal time:,f6.2,
     s                 10h (hh.dd)  ,t79,1h*,/,
     s   1h*, 15x,10hlatitude: ,f7.2,5h deg ,6x,
     s                 12h longitude: ,f7.2,5h deg ,t79,1h*)
  102 format(1h*,2x,22h solar zenith angle:  ,f6.2,5h deg ,
     s     29h solar azimuthal angle:      ,f6.2,5h deg ,t79,1h*)
  103 format(1h*,2x,7h month:,i3,7h day : ,i3,t79,1h*)
 1110 format(1h*,2x,22h view zenith angle:   ,f6.2,5h deg ,
     s       29h view azimuthal angle:       ,f6.2,5h deg ,
     s      t79,1h*,/,
     s       1h*,2x,22h scattering angle:    ,f6.2,5h deg ,
     s           29h azimuthal angle difference: ,f6.2,5h deg ,
     s      t79,1h*)
 1119 format(1h*,t79,1h*,/,
     s       1h*,22x,31h atmospheric model description ,t79,1h*,/,
     s       1h*,22x,31h ----------------------------- ,t79,1h*)
 1261 format(1h*,10x,30h atmospheric model identity : ,t79,1h*,/,
     s       1h*,15x,a51,t79,1h*)
 1272 format(1h*,30h atmospheric model identity : ,t79,1h*,/,
     s       1h*,12x,33h user defined atmospheric model  ,t79,1h*,/,
     s       1h*,12x,11h*altitude  ,11h*pressure  ,
     s           11h*temp.     ,11h*h2o dens. ,11h*o3 dens.  ,t79,1h*)
 1271 format(1h*,12x,5e11.4,t79,1h*)
 1281 format(1h*,10x,31h atmospheric model identity :  ,t79,1h*,
     s     /,1h*,12x,35h user defined water content : uh2o=,f6.3,
     s                  7h g/cm2 ,t79,1h*,
     s     /,1h*,12x,35h user defined ozone content : uo3 =,f6.3,
     s                  7h cm-atm,t79,1h*)


 5550 format(1h*,10x,25h aerosols type identity :,t79,1h*)
 5551 format(1h*,11x,31h  user-defined aerosol profile:, I2,   
     s 7h layers,t79,1h*)
 5552 format(1h*,13x,46h Layer   Height(km)   Opt. thick.(at 0.55 mkm), 
     s 3x,7h  Model,t79,1h*)    
 5553 format(1h*,15x,I2,1x,f10.1,13x,f5.3,15x,A15,t79,1h*)
 5554 format(1h*,15x,20hno aerosols computed,t79,1h*) 
 5555 format(1h*,t79,1h*) 
 132  format(1h*,15x,a30,t79,1h*)         
 133  format(1h*,13x,28huser-defined aerosol model: ,t79,1h*,/,
     s  1h*,26x,f6.3,15h % of dust-like,t79,1h*,/,
     s  1h*,26x,f6.3,19h % of water-soluble,t79,1h*,/,
     s  1h*,26x,f6.3,13h % of oceanic,t79,1h*,/,
     s  1h*,26x,f6.3,10h % of soot,t79,1h*)
 134  format(1h*,13x,28huser-defined aerosol model: ,I2, 
     s 32h Log-Normal size distribution(s),t79,1h*,/,
     s 1h*,15x,43hMean radius   Stand. Dev.  Percent. density,
     s  t79,1h*)   
 135  format(1h*,t19,f6.4,T33,f5.3,T47,e8.3,T79,1h*)
 136  format(1h*,13x,27huser-defined aerosol model:, 
     s 33h modified Gamma size distribution,t79,1h*,/,
     s  1h*,19x,7hAlpha: ,f6.3,6h   b: ,f6.3,10h   Gamma: ,f6.3,t79,1h*)    
 137  format(1h*,13x,27huser-defined aerosol model:, 
     s 34h Junge Power-Law size distribution,t79,1h*,/,
     s  1h*,19x,7hAlpha: ,f6.3,t79,1h*)
 138  format(1h*,13x,42huser-defined aerosol model using data from,
     s  10h the file:,t79,1h*,/,1h*,20x,A30,T79,1h*)
 139  format(1h*,15x,29h results saved into the file:,t79,1h*,/,
     s  1h*,20x,A30,T79,1h*)


  140 format(1h*,10x,29h optical condition identity :,t79,1h*,/,
     s       1h*,15x,34h user def. opt. thick. at 550 nm :,f7.4,
     s       t79,1h*,/,1h*,t79,1h*)
  141 format(1h*,10x,29h optical condition identity :,t79,1h*,/,
     s       1h*,14x,13h visibility :,f6.2,4h km ,
     s                 22h opt. thick. 550 nm : ,f7.4,t79,1h*)
  142 format(1h*,t79,1h*,/,1h*,22x,
     s36h Surface polarization parameters    ,t79,1h*,/,1h*,
     s22x,36h ---------------------------------- ,t79,1h*,/,
     s1h*,t79,1h*)
  143 format(1h*,t79,1h*,/,1h*,
     s36h Surface Polarization Q,U,Rop,Chi   ,3(F8.5,1X),
     s F8.2,1X,t79,1h*,/,1h*,t79,1h*)
     
  144 format(1h*,t79,1h*,/,1h*,
     s36h Nadal and Breon with %  vegetation  ,1(F8.2,1X),
     s t79,1h*,/,1h*)
     
  145 format(1h*,t79,1h*,/,1h*,
     s36h  Sunglint Model  windspeed,azimuth ,2(F8.3,1X),
     s t79,1h*,/,1h*)
     
  146 format(1h*,t79,1h*,/,1h*,
     s36h  User's input roQ and roU          ,2(F8.3,1X),
     s t79,1h*,/,1h*)
  
  148 format(1h*,22x,21h spectral condition  ,t79,1h*,/,1h*,
     s             22x,21h ------------------  ,t79,1h*)   

  149 format(1h*,11x,32h monochromatic calculation at wl :,
     s                              f6.3,8h micron ,t79,1h*)
 1510 format(1h*,10x,a17,t79,1h*,/,
     s 1h*,15x,26hvalue of filter function :,t79,1h*,/,1h*,
     s 15x,8h wl inf=,f6.3,4h mic,2x,8h wl sup=,f6.3,4h mic,t79,1h*)
  168 format(1h*,t79,1h*,/,1h*,22x,14h target type  ,t79,1h*,/,1h*,
     s                         22x,14h -----------  ,t79,1h*,/,1h*,
     s                         10x,20h homogeneous ground ,t79,1h*)
  169 format(1h*,t79,1h*,/,1h*,22x,14h target type  ,t79,1h*,/,1h*,
     s                         22x,14h -----------  ,t79,1h*,/,1h*,
     s    10x,41h inhomogeneous ground , radius of target ,f6.3,
     s         5h km  ,t79,1h*)
  170 format(1h*,15x,22h target reflectance : ,t79,1h*)
  171 format(1h*,15x,29h environmental reflectance : ,t79,1h*)
  172 format(1h*,t79,1h*,/,79(1h*),///)
  173 format(1h*,t79,1h*,/,
     s       1h*,22x,30h target elevation description ,t79,1h*,/,
     s       1h*,22x,30h ---------------------------- ,t79,1h*)
  174 format(1h*,10x,22h ground pressure  [mb]    ,1x,f7.2,1x,t79,1h*)
  175 format(1h*,10x,22h ground altitude  [km]    ,f6.3,1x,t79,1h*)
  176 format(1h*,15x,34h gaseous content at target level: ,t79,1h*,
     s     /,1h*,15x,6h uh2o=,f6.3,7h g/cm2 ,
     s           5x,6h  uo3=,f6.3,7h cm-atm,t79,1h*)
  177 format(1h*,t79,1h*,/,
     s       1h*,23x,34h atmospheric correction activated ,t79,1h*,/,
     s       1h*,23x,34h -------------------------------- ,t79,1h*)

  220 format(1h*,23x,34h Lambertian assumption  selected  ,t79,1h*)
  221 format(1h*,23x,34h BRDF coupling correction         ,t79,1h*)


  185 format(1h*,10x,30h input apparent reflectance : , f6.3,t79,1h*)
  186 format(1h*,10x,39h input measured radiance [w/m2/sr/mic] ,
     s       f7.3,t79,1h*)

  187 format(1h*,t79,1h*,/,
     s       1h*,15x,34h brdf selected                    ,t79,1h*,/,
     s 1h*,15x,40h     rodir    robar    ropbar    albedo ,t79,1h*,/,
     s       1h*,15x,4(f9.4,1x),t79,1h*)
  190 format(1h*,15x,31h brdf from in-situ measurements,t79,1h*)
  191 format(1h*,15x,23h Hapke's model selected,t79,1h*
     s       /,1h*,16x,3hom:,f5.3,1x,3haf:,f5.3,1x,3hs0:,f5.3,1x,
     s       2hh:,f5.3,t79,1h*)
  192 format(1h*,15x,38h Pinty and Verstraete's model selected,t79,1h*
     s       /,1h*,16x,3hom:,f5.3,1x,5hrad :,f5.3,1x,6hlad  :,f5.3,1x,
     s        t79,1h*)
  193 format(1h*,15x,32h Roujean et al.'s model selected,t79,1h*
     s       /,1h*,16x,3hk0:,f5.3,1x,3hk1:,f5.3,1x,3hk2:,f5.3,
     s       t79,1h*)
  194 format(1h*,15x,33h Walthall et al.'s model selected,t79,1h*
     s       /,1h*,16x,2ha:,f5.3,1x,3hap:,f5.3,1x,2hb:,f5.3,1x,
     s       3hom:,f5.3,t79,1h*)
  195 format(1h*,15x,26h Minnaert's model selected,t79,1h*
     s       /,1h*,16x,5hpar1:,f5.3,1x,5hpar2:,f5.3,t79,1h*)
  196 format(1h*,15x,21h ocean model selected,t79,1h*
     s       /,1h*,16x,18hwind speed [m/s] :,f5.1,
     s             2x,27hazimuth of the wind [deg] :,f8.2,t79,1h*
     s       /,1h*,16x,16hsalinity [ppt] :,f5.1,
     s             4x,23hpigment conc. [mg/m3] :,f6.2,t79,1h*)
  197 format(1h*,15x,41h given kappa1 and kappa2:                ,t79,
     s    1h*,/,1h*,20x,5hkpa1:,f5.3,1x,5hkpa2:,f5.3,t79,1h*)
  198 format(1h*,15x,41h Goudrian's parametrization of kappa :   ,t79,
     s   1h*,/,1h*,20x,6h ksil:,f5.3,1x,t79,1h*)
  199 format(1h*,15x,41h modified Goudrian's parametrization :   ,t79,
     s   1h*,/,1h*,20x,6h ksil:,f5.3,1x,t79,1h*)
  200 format(1h*,15x,40h single scattering only              :  ,t79,
     s   1h*)
  201 format(1h*,15x,40h multiple scattering (Dickinson et al)  ,t79,
     s   1h*)
  202 format(1h*,15x,40h isotropic phase function            :  ,t79,
     s   1h*)
  203 format(1h*,15x,40h Heyney-Greenstein's phase function  :  ,t79,
     s   1h*,/,1h*,20x,6hassym:,f5.3,1x,t79,1h*)
  204 format(1h*,15x,40h Legendre polynomial phase function  :  ,t79,
     s   1h*,/,1h*,20x,6hbeta1:,f5.3,1x,6hbeta2:,f5.3,t79,1h*)
  205 format(1h*,15x,40h Iaquinta and Pinty BRDF model selected ,t79,
     s       1h*,/,1h*,16x,3hRl:,f5.3,1x,3hTl:,f5.3,1x,3hRs:,f5.3,1x
     s       ,1x,4hLAl:,f5.3,t79,1h*)
  206 format(1h*,15x,30h Rahman et al. model selected ,t79,
     s       1h*,/,1h*,16x,4hRho0:,f6.3,1x,2haf:,f6.3,1x,3hxk:,f6.3,1x
     s       ,t79,1h*)
  207 format(1h*,15x,A19,t79,1h*)
  208 format(1h*,15x,A19,1x,f5.2,t79,1h*)
  209 format(1h*,15x,A31,t79,1h*)
  210 format(1h*,2x,40h Kuusk BRDF model,                      ,t79,1h*,
     s       /,1h*,12x,4hLAI:,f5.3,2x,4heps:,f6.4,2x,4hthm:,f4.1
     s       ,1x,3hsl:,f4.2,t79,1h*,
     s       /,1h*,12x,4hcAB:,f6.2,1x,3hcW:,f5.3,1x,2hN:,f5.3,1x,3hcn:
     s       ,f4.2,1x,5hrsl1:,f5.3,t79,1h*)
  211 format(1h*,15x,30h MODIS BRDF    model selected ,t79,
     s       1h*,/,1h*,16x,4h  p1:,f6.3,1x,3hp2:,f6.3,1x,3hp3:,f6.3,1x
     s       ,t79,1h*)

c pressure at ground level (174) and altitude (175)
  178 format(1h*,t79,1h*,/,
     s       1h*,22x,30h plane simulation description ,t79,1h*,/,
     s       1h*,22x,30h ---------------------------- ,t79,1h*)
  179 format(1h*,10x,31h plane  pressure          [mb] ,f7.2,1x,t79,1h*)
  180 format(1h*,10x,31h plane  altitude absolute [km] ,f6.3,1x,t79,1h*)
  181 format(1h*,15x,37h atmosphere under plane description: ,t79,1h*)
  182 format(1h*,15x,26h ozone content            ,f6.3,1x,t79,1h*)
  183 format(1h*,15x,26h h2o   content            ,f6.3,1x,t79,1h*)
  184 format(1h*,15x,26haerosol opt. thick. 550nm ,f6.3,1x,t79,1h*)
 
  426 format(1h*,t79,1h*,/,
     s       1h*,24x,27h coupling aerosol -wv  :   ,t79,1h*,/,
     s       1h*,24x,27h --------------------      ,t79,1h*,/,
     s       1h*,10x,20h wv above aerosol : ,f5.3,4x,
     s               25h wv mixed with aerosol : ,f5.3,1x,t79,1h*,/,
     s       1h*,22x,20h wv under aerosol : ,f5.3,t79,1h*,/,1h*,t79,
     s 1h*,/,1h*,24x,34h coupling polarized aerosol -wv  :,t79,1h*,/,
     s       1h*,24x,34h ------------------------------   ,t79,1h*,/,
     s       1h*,10x,20h wv above aerosol : ,f5.3,4x,
     s               25h wv mixed with aerosol : ,f5.3,1x,t79,1h*,/,
     s       1h*,22x,20h wv under aerosol : ,f5.3,t79,1h*)
  427 format(79(1h*),/,1h*,t79,1h*,/,
     s       1h*,24x,27h integrated values of  :   ,t79,1h*,/,
     s       1h*,24x,27h --------------------      ,t79,1h*,/,
     s       1h*,t79,1h*,/,
     s       1h*,6x,22h apparent reflectance ,f9.2,1x,
     s                 26h appar. rad.(w/m2/sr/mic) ,f10.3,1x,t79,1h*,/,
     s       1h*,6x,22h app. polarized refl. ,f7.4,3x,
     s                 26h app. pol. rad. ( "  "  ) ,f10.3,1x,t79,1h*,/,
     s       1h*,12x,39h direction of the plane of polarization,
     s       f6.2,t79,1h*,/,
     s       1h*,18x,30h total gaseous transmittance  ,f5.3,t79,1h*,/,
     s       1h*,t79,1h*,/,79(1h*))
  428 format(1h*,t79,1h*,/,
     s 1h*,24x,34h coupling polarized aerosol -wv  :,t79,1h*,/,
     s       1h*,24x,34h ------------------------------   ,t79,1h*,/,
     s       1h*,10x,20h wv above aerosol : ,f5.3,4x,
     s               25h wv mixed with aerosol : ,f5.3,1x,t79,1h*,/,
     s       1h*,22x,20h wv under aerosol : ,f5.3,t79,1h*)
  429 format(79(1h*),/,1h*,t79,1h*,/,
     s       1h*,24x,27h integrated values of  :   ,t79,1h*,/,
     s       1h*,24x,27h --------------------      ,t79,1h*,/,
     s       1h*,t79,1h*,/,
     s       1h*,6x,22h app. polarized refl. ,f7.4,3x,
     s       30h app. pol. rad. (w/m2/sr/mic) ,f8.3,
     s       1x,t79,1h*,/,
     s       1h*,12x,39h direction of the plane of polarization,
     s       f6.2,t79,1h*,/,
     s       1h*,18x,30h total polarization ratio     ,f5.3,t79,1h*,/,
     s       1h*,t79,1h*,/,79(1h*))
  430 format(79(1h*),/,1h*,t79,1h*,/,
     s       1h*,24x,27h integrated values of  :   ,t79,1h*,/,
     s       1h*,24x,27h --------------------      ,t79,1h*,/,
     s       1h*,t79,1h*,/,
     s       1h*,6x,22h apparent reflectance ,f10.7,1x,
     s                 26h appar. rad.(w/m2/sr/mic) ,f8.3,1x,t79,1h*,/,
     s       1h*,18x,30h total gaseous transmittance  ,f5.3,
     s  t79,1h*,/,1h*,t79,1h*,/,79(1h*))
  500 format(1h*,6x,40h water reflectance components:           ,
     s       t79,1h*,/,
     s       1h*,6x,10h Foam:    ,1x, f10.5,1x
     s              ,10h Water:   ,1x, f10.5,1x
     s              ,10h Glint:   ,1x, f10.5,1x,t79,1h*)
  431 format(1h*,t79,1h*,/,
     s       1h*,24x,27h coupling aerosol -wv  :   ,t79,1h*,/,
     s       1h*,24x,27h --------------------      ,t79,1h*,/,
     s       1h*,10x,20h wv above aerosol : ,f7.3,4x,
     s               25h wv mixed with aerosol : ,f7.3,1x,t79,1h*,/,
     s       1h*,22x,20h wv under aerosol : ,f7.3,t79,1h*)
  432 format(1h*,t79,1h*,/,1h*,
     s        24x,32h int. normalized  values  of  : ,t79,1h*,/,1h*,
     s        24x,32h ---------------------------    ,t79,1h*,/,1h*,
     s             22x,31h% of irradiance at ground level,
     s  t79,1h*,/,1h*,5x,17h% of direct  irr.,
     s                    4x,17h% of diffuse irr.,
     s                    4x,17h% of enviro. irr ,t79,1h*,/,
     s             1h*,3(10x,f10.3),t79,1h*,/,
     s 1h*,22x,31h reflectance at satellite level  ,t79,1h*,/,
     s                1h*,5x,17hatm. intrin. ref.,
     s                    3x,a11,5h ref.,
     s                    2x,a6,12h reflectance,t79,1h*,/,
     s             1h*,3(10x,f10.3),t79,1h*,/,1h*,t79,1h*)
  436 format(1h*,t79,1h*,/,1h*,22x,24hsol. spect (in w/m2/mic),t79,1h*,
     s/,1h*,30x,f10.3,t79,1h*,/,1h*,t79,1h*,/,79(1h*))
  437 format(1h*,t79,1h*,/,1h*,10x,29hint. funct filter (in mic)
     s               ,10x,26h int. sol. spect (in w/m2),t79,1h*,/,
     s1h*,10x,f12.7,30x,f10.3,t79,1h*,/,1h*,t79,1h*,/,79(1h*))
  434 format(1h*,24x,24h int. absolute values of,t79,
     s 1h*,/,1h*,24x,24h -----------------------               ,
     s  t79,1h*,/,1h*,22x,33hirr. at ground level (w/m2/mic)  ,
     s  t79,1h*,/,1h*, 5x,17hdirect solar irr.,
     s             4x,17hatm. diffuse irr.,
     s             4x,17henvironment  irr ,t79,1h*,/,
     s             1h*,3(10x,f10.3),t79,1h*,/,
     s        1h*,22x,33hrad at satel. level (w/m2/sr/mic),t79,1h*,/,
     s                1h*,5x,17hatm. intrin. rad.,
     s                    4x,a11,5h rad.,
     s                    4x,a6,9h radiance,t79,1h*,/,
     s             1h*,3(10x,f10.3),t79,1h*,/,1h*,t79,1h*)
  929 format(1h ,////)
  930 format(79(1h*),/,1h*,t79,1h*,/,
     s       1h*,t27,27h integrated values of  :   ,t79,1h*,/,
     s       1h*,t27,27h --------------------      ,t79,1h*,/,
     s       1h*,t79,1h*,/,
     s       1h*,t30,10h downward ,t45,10h  upward  ,
     s            t60,10h   total  ,t79,1h*)
  931 format(1h*,6x,a20,t32,f8.5,t47,f8.5,t62,f8.5,t79,1h*)
  932 format(1h*,6x,a20,t32,f8.2,t47,f8.2,t62,f8.2,t79,1h*)
  939 format(1h*,t79,1h*,/,1h*,
     s             t30,10h rayleigh ,t45,10h aerosols ,
     s            t60,10h   total  ,t79,1h*,/,1h*,t79,1h*)
  940 format(79(1h*),/,/,/,/,79(1h*),/
     s       1h*,23x,31h atmospheric correction result ,t79,1h*,/,
     s       1h*,23x,31h ----------------------------- ,t79,1h*)
  941 format(1h*,6x,40h input apparent reflectance            :, 
     s           1x, f8.3, t79,1h*)
  942 format(1h*,6x,40h measured radiance [w/m2/sr/mic]       :, 
     s           1x, f8.3, t79,1h*)
  943 format(1h*,6x,40h atmospherically corrected reflectance :,
     s           1x, f8.3, t79,1h*)
  222 format(1h*,6x,40h atmospherically corrected reflectance  ,
     s       t79,1h*,/,
     s       1h*,6x,20h Lambertian case :  ,1x, f10.5, t79,1h*,/,
     s       1h*,6x,20h BRDF       case :  ,1x, f10.5, t79,1h*)
  944 format(1h*,6x,40h coefficients xa xb xc                 :, 
     s           1x, 3(f8.5,1x),t79,1h*,/,1h*,6x,
     s           ' y=xa*(measured radiance)-xb;  acr=y/(1.+xc*y)',
     s               t79,1h*,/,79(1h*))
 1401 format(1h*,t79,1h*)
 1402 format(1h*,t79,1h*,/,79(1h*))
 1500 format(1h*,1x,42hwave   total  total  total  total  atm.   ,
     s           33hswl    step   sbor   dsol   toar ,t79,1h*,/,
     s  1h*,1x,42h       gas    scat   scat   spheri intr   ,t79,1h*,/,
     s  1h*,1x,42h       trans  down   up     albedo refl   ,t79,1h*)
 1501 format(1h*,6(F6.4,1X),F6.1,1X,4(F6.4,1X),t79,1h*)
 1502 format(1h*,6(F5.3,1X),F6.1,1X,1(F6.4,1X),t79,1h*)
 1503 format(1h*,6x,5(F5.3,1X),F6.1,1X,1(F6.4,1X),t79,1h*)

      end
