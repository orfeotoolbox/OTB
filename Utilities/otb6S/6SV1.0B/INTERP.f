      subroutine interp (iaer,idatmp,wl,taer55,taer55p,xmud,
     a  romix,rorayl,roaero,phaa,phar,rqmix,rqrayl,rqaero,qhaa,qhar,
     a  rumix,rurayl,ruaero,uhaa,uhar,
     a  tsca,tray,trayp,taer,taerp,dtott,utott,astot,asray,asaer,
     a  utotr,utota,dtotr,dtota,ipol,
     a       roatm_fi,romix_fi,rorayl_fi,nfi,
     s   roluts,rolut,rolutsq,rolutq,rolutsu,rolutu,nfilut)
 
      include "paramdef.inc"
      real test1,test2,test3
      Real wl,taer55,taer55p
      Real xmud,romix,rorayl,roaero,phaa,phar,tsca,tray
      Real rqmix,rqrayl,rqaero,qhaa,qhar,rqatm,qhase
      Real rumix,rurayl,ruaero,uhaa,uhar,ruatm,uhase
      Real trayp,taer,taerp,dtott,utott,astot,asray,asaer,utotr
      Real utota,dtotr,dtota,ext,ome,gasym,phase,roatm,dtdir
      Real dtdif,utdir,utdif,sphal,wldis,trayl,traypl,delta,sigma
      Real alphaa,betaa,alphar,betar,alphac,betac,coef,coefl,wlinf
      Real drinf,drsup,dtinf,dtsup,dtotc,dainf,dasup,urinf,ursup
      Real utinf,utsup,utotc,uainf,uasup,arinf,arsup,atinf,atsup
      Real aainf,aasup,depolar1,depolar2
      real romix_fi(nfi),rorayl_fi(nfi),roatm_fi(3,20,nfi)
      real rolut(mu_p,41),roluts(20,mu_p,41)
      real rolutq(mu_p,41),rolutsq(20,mu_p,41)
      real rolutu(mu_p,41),rolutsu(20,mu_p,41)
      integer nfilut(mu_p),mu
      Integer iaer,idatmp,linf,ll,lsup,ipol

      common /sixs_aer/ext(20),ome(20),gasym(20),phase(20),qhase(20),
     &uhase(20)
      common /sixs_disc/ roatm(3,20),dtdir(3,20),dtdif(3,20),
     a utdir(3,20),utdif(3,20),sphal(3,20),wldis(20),trayl(20),
     a traypl(20),rqatm(3,20),ruatm(3,20)
      common /sixs_del/ delta,sigma

 
      mu=mu_p
c     that for the atmosphere (if polarization requested) :
c     the reflectances
c                     rayleigh                   = rorayl ,(rqrayl,rurayl)
c                     aerosols                   = roaero ,(rqaero,ruaero)
c                     mixing                     = romix ,(rqmix,rumix)
c     the downward transmittances
c                     rayleigh                   = dtotr
c                     aerosols                   = dtota
c                     total                      = dtott
c     the upward transmittances
c                     rayleigh                   = utotr
c                     aerosols                   = utota
c                     total                      = utott
c     the spherical albedos
c                     rayleigh                   = asray
c                     aerosols                   = asaer
c                     total                      = astot
c     the optical thickness of total atmosphere
c                     rayleigh                   = tray
c                     aerosols                   = taer
c     the optical thickness of the atmosphere above the plane
c                     rayleigh                   = trayp
c                     aerosols                   = taerp
c     the tsca of the aerosols 
c                     total atmosphere           = tsca
      
      linf=1
      do 81 ll=1,19
        if(wl.gt.wldis(ll).and.wl.le.wldis(ll+1)) linf=ll
   81 continue
      if(wl.gt.wldis(20)) linf=19
      lsup=linf+1
 
c     interpolation in function of wavelength for scattering
c     atmospheric functions from discrete values at wldis
 
      alphaa=0.
      betaa=0.
      alphar=0.
      betar=0.
      alphac=0.
      betac=0.
      phaa=0.
      qhaa=0.
      uhaa=0.
      phar=0.
      qhar=0.
      uhar=0.
      roaero=0.
      rqaero=0.
      ruaero=0.
      rorayl=0.
      rqrayl=0.
      rurayl=0.
      romix=0.
      rqmix=0.
      rumix=0.
      dtota=1.
      utota=1.
      asaer=0.
      taer=0.
      taerp=0.
      coef=alog(wldis(lsup)/wldis(linf))
      wlinf=wldis(linf)
c Here we spectrally interpolate phase functions and Stokes' 
c parameters I, Q and U (all divided by cos(teta_sun)), both
c for Rayleigh, Aerosol, and a mixture Rayleigh+aerosol
c For the Rayleigh Phase functions we need to take into
c account the depolarization factor delta.
      depolar1=2.*(1.-delta)/(2.+delta)
      depolar2=3.*delta/(2.+delta)
c
c we first start with the parameter I (with truncation)
c    (divided by cos(teta_sun))
c if requested, we continue with the parameter I (without truncation)...
c    (divided by cos(teta_sun))
          if(iaer.eq.0) goto 2240
          alphaa=alog(phase(lsup)/phase(linf))/coef
          betaa=phase(linf)/(wlinf**(alphaa))
          phaa=betaa*(wl**alphaa)
 2240     continue
          phar=depolar1*.75*(1.+xmud*xmud)+depolar2
          if (idatmp.eq.0) goto 2234

C          if((roatm(1,linf).le..001).or.(roatm(1,lsup).le..001)) then
C            rorayl=roatm(1,linf)+(roatm(1,lsup)-roatm(1,linf))
C     s       *(wl-wldis(linf))/(wldis(lsup)-wldis(linf))
C          else
             alphar=alog(roatm(1,lsup)/roatm(1,linf))/ coef
             betar=roatm(1,linf)/(wlinf**(alphar))
 	    rorayl=betar*(wl**alphar)
	    do ifi=1,nfi
	    alphar=alog(roatm_fi(1,lsup,ifi)/roatm_fi(1,linf,ifi))/ coef
            betar=roatm_fi(1,linf,ifi)/(wlinf**(alphar))
	    rorayl_fi(ifi)=betar*(wl**alphar)
            enddo
C          endif
c         write(6,*)'Ib ',phar,rorayl,betar,alphar,roatm(1,lsup),
c    s            roatm(1,linf)
C          if((roatm(2,linf).le..00000001).or.(roatm(2,lsup).le..00000001)) then
C            romix=roatm(2,linf)+(roatm(2,lsup)-roatm(2,linf))
C     s       *(wl-wldis(linf))/(wldis(lsup)-wldis(linf))
C            xtmp=(wl-wldis(linf))/(wldis(lsup)-wldis(linf))
C            do ifi=1,nfi
C	    romix_fi(ifi)=roatm_fi(2,linf,ifi)+
C     s      (roatm_fi(2,lsup,ifi)-roatm_fi(2,linf,ifi))*xtmp
C            enddo
C          else
            alphac=alog(roatm(2,lsup)/roatm(2,linf))/coef
            betac=roatm(2,linf)/(wlinf**(alphac))
	    romix=betac*(wl**alphac)
     	    do ifi=1,nfi
            alphac=alog(roatm_fi(2,lsup,ifi)/roatm_fi(2,linf,ifi))/coef
            betac=roatm_fi(2,linf,ifi)/(wlinf**(alphac))
	    romix_fi(ifi)=betac*(wl**alphac)
	    enddo
C          endif
          if(iaer.eq.0) goto 2234
C          if((roatm(3,linf).lt..001).or.(roatm(3,lsup).le..001))then
C            roaero=roatm(3,linf)+(roatm(3,lsup)-roatm(3,linf))
C     s       *(wl-wldis(linf))/(wldis(lsup)-wldis(linf))
C          else
            alphaa=alog(roatm(3,lsup)/roatm(3,linf))/coef
            betaa=roatm(3,linf)/(wlinf**(alphaa))
            roaero=betaa*(wl**alphaa)
C          endif
	  
C Look up table update
        coefl=(wl-wldis(linf))/(wldis(lsup)-wldis(linf))
	do i=1,mu
	do j=1,nfilut(i)
C	if ((roluts(lsup,i,j).gt.0.001).and.(roluts(linf,i,j).gt.0.001)) then
           alphac=alog(roluts(lsup,i,j)/roluts(linf,i,j))/coef
           betac=roluts(linf,i,j)/(wlinf**(alphac))
	   rolut(i,j)=betac*(wl**alphac)
C	   else
C	   rolut(i,j)=roluts(linf,i,j)
C     &      +(roluts(lsup,i,j)-roluts(linf,i,j))*coefl
C	endif
	
	if ((rolutsq(lsup,i,j).gt.0.001).and.(rolutsq(linf,i,j).gt.0.001)) then
           alphac=alog(rolutsq(lsup,i,j)/rolutsq(linf,i,j))/coef
           betac=rolutsq(linf,i,j)/(wlinf**(alphac))
	   rolutq(i,j)=betac*(wl**alphac)
	   else
	   rolutq(i,j)=rolutsq(linf,i,j)
     &      +(rolutsq(lsup,i,j)-rolutsq(linf,i,j))*coefl
	endif


	if ((rolutsu(lsup,i,j).gt.0.001).and.(rolutsu(linf,i,j).gt.0.001)) then
           alphac=alog(rolutsu(lsup,i,j)/rolutsu(linf,i,j))/coef
           betac=rolutsu(linf,i,j)/(wlinf**(alphac))
	   rolutu(i,j)=betac*(wl**alphac)
	   else
	   rolutu(i,j)=rolutsu(linf,i,j)
     &      +(rolutsu(lsup,i,j)-rolutsu(linf,i,j))*coefl
	endif

	enddo
	enddo
C End Look up table update	
 2234     continue

c we continue with the parameter Q ....
        if(iaer.eq.0) goto 3240
	if ((qhase(lsup).gt.0.001).and.(qhase(linf).gt.0.001)) then
        alphaa=alog(qhase(lsup)/qhase(linf))/coef
        betaa=qhase(linf)/(wlinf**(alphaa))
        qhaa=betaa*(wl**alphaa)
	else
	qhaa=qhase(linf)+(qhase(lsup)-qhase(linf))*coefl
	endif
 3240   continue
        qhar=depolar1*.75*(xmud*xmud-1.)
        if (idatmp.eq.0) goto 3234

	test1=abs(rqatm(1,linf))
	test2=abs(rqatm(1,lsup))
	test3=rqatm(1,lsup)*rqatm(1,linf)
        if((test1.lt.0.001).or.(test2.lt.0.001)
     s            .or.(test3.lt.0.0)) then
          rqrayl=rqatm(1,linf)+(rqatm(1,lsup)-rqatm(1,linf))
     s     *(wl-wldis(linf))/(wldis(lsup)-wldis(linf))
        else
          alphar=alog(rqatm(1,lsup)/rqatm(1,linf))/ coef
          betar=rqatm(1,linf)/(wlinf**(alphar))
	  rqrayl=betar*(wl**alphar)
	endif
c       write(6,*)'Q ',qhar,rqrayl,betar,alphar,rqatm(1,lsup),
c    s          rqatm(1,linf)

	test1=abs(rqatm(2,linf))
	test2=abs(rqatm(2,lsup))
	test3=rqatm(2,lsup)*rqatm(2,linf)
        if((test1.lt.0.001).or.(test2.lt.0.001)
     s            .or.(test3.lt.0.0)) then
          rqmix=rqatm(2,linf)+(rqatm(2,lsup)-rqatm(2,linf))
     s     *(wl-wldis(linf))/(wldis(lsup)-wldis(linf))
	else
          alphac=alog(rqatm(2,lsup)/rqatm(2,linf))/coef
          betac=rqatm(2,linf)/(wlinf**(alphac))
          rqmix=betac*(wl**alphac)
        endif
        if(iaer.eq.0) goto 3234
C        write(6,*) "rqaero ",rqatm(3,linf),rqatm(3,lsup)
	
	test1=abs(rqatm(3,linf))
	test2=abs(rqatm(3,lsup))
	test3=rqatm(3,lsup)*rqatm(3,linf)
        if((test1.lt.0.001).or.(test2.lt.0.001)
     s            .or.(test3.lt.0.0)) then
          rqaero=rqatm(3,linf)+(rqatm(3,lsup)-rqatm(3,linf))
     s     *(wl-wldis(linf))/(wldis(lsup)-wldis(linf))
	else
	  
          alphaa=alog(rqatm(3,lsup)/rqatm(3,linf))/coef
          betaa=rqatm(3,linf)/(wlinf**(alphaa))
          rqaero=betaa*(wl**alphaa)
	endif
C        write(6,*) "rqaero ",rqaero
 3234  	continue

c .... and we finish with the parameter U
        if(iaer.eq.0) goto 4242
	if ((uhase(lsup).gt.0.001).and.(uhase(linf).gt.0.001)) then
        alphaa=alog(uhase(lsup)/uhase(linf))/coef
        betaa=uhase(linf)/(wlinf**(alphaa))
        uhaa=betaa*(wl**alphaa)
	else
	uhaa=uhase(linf)+(uhase(lsup)-uhase(linf))*coefl
	endif
 4242   continue
        uhar=depolar1*3./2.*xmud
        if (idatmp.eq.0) goto 4234

	test1=abs(ruatm(1,linf))
	test2=abs(ruatm(1,lsup))
	test3=ruatm(1,lsup)*ruatm(1,linf)
	
        if((test1.lt.0.001).or.(test2.lt.0.001)
     s            .or.(test3.lt.0.0)) then
          rurayl=ruatm(1,linf)+(ruatm(1,lsup)-ruatm(1,linf))
     s     *(wl-wldis(linf))/(wldis(lsup)-wldis(linf))
        else
          alphar=alog(ruatm(1,lsup)/ruatm(1,linf))/ coef
          betar=ruatm(1,linf)/(wlinf**(alphar))
	  rurayl=betar*(wl**alphar)
	endif
c       write(6,*)'U ',uhar,rurayl,betar,alphar,ruatm(1,lsup),
c    s          ruatm(1,linf)
	test1=abs(ruatm(2,linf))
	test2=abs(ruatm(2,lsup))
	test3=ruatm(2,lsup)*ruatm(2,linf)
        if((test1.lt.0.001).or.(test2.lt.0.001)
     s            .or.(test3.lt.0.0)) then
          rumix=ruatm(2,linf)+(ruatm(2,lsup)-ruatm(2,linf))
     s     *(wl-wldis(linf))/(wldis(lsup)-wldis(linf))
	else
          alphac=alog(ruatm(2,lsup)/ruatm(2,linf))/coef
          betac=ruatm(2,linf)/(wlinf**(alphac))
          rumix=betac*(wl**alphac)
        endif
        if(iaer.eq.0) goto 4234
	test1=abs(ruatm(3,linf))
	test2=abs(ruatm(3,lsup))
	test3=ruatm(3,lsup)*ruatm(3,linf)
        if((test1.lt.0.001).or.(test2.lt.0.001)
     s            .or.(test3.lt.0.0)) then
          ruaer0=ruatm(3,linf)+(ruatm(3,lsup)-ruatm(3,linf))
     s     *(wl-wldis(linf))/(wldis(lsup)-wldis(linf))
	else
          alphaa=alog(ruatm(3,lsup)/ruatm(3,linf))/coef
          betaa=ruatm(3,linf)/(wlinf**(alphaa))
          ruaero=betaa*(wl**alphaa)
        endif
 4234  	continue
c
c
      alphar=alog(trayl(lsup)/trayl(linf))/coef
      betar=trayl(linf)/(wlinf**(alphar))
      tray=betar*(wl**alphar)
      if (idatmp.ne.0.) then
        alphar=alog(traypl(lsup)/traypl(linf))/coef
        betar=traypl(linf)/(wlinf**(alphar))
        trayp=betar*(wl**alphar)
      else
        trayp=0.
      endif
c
      if(iaer.eq.0) goto 1235
      alphaa=alog(ext(lsup)*ome(lsup)/(ext(linf)*ome(linf)))/coef
      betaa=ext(linf)*ome(linf)/(wlinf**(alphaa))
      tsca=taer55*betaa*(wl**alphaa)/ext(8)
      alphaa=alog(ext(lsup)/ext(linf))/coef
      betaa=ext(linf)/(wlinf**(alphaa))
      taerp=taer55p*betaa*(wl**alphaa)/ext(8)
      taer=taer55*betaa*(wl**alphaa)/ext(8)
c
 1235 drinf=dtdif(1,linf)+dtdir(1,linf)
      drsup=dtdif(1,lsup)+dtdir(1,lsup)
      alphar=alog(drsup/drinf)/coef
      betar=drinf/(wlinf**(alphar))
      dtotr=betar*(wl**alphar)
      dtinf=dtdif(2,linf)+dtdir(2,linf)
      dtsup=dtdif(2,lsup)+dtdir(2,lsup)
      alphac=alog((dtsup*drinf)/(dtinf*drsup))/coef
      betac=(dtinf/drinf)/(wlinf**(alphac))
      dtotc=betac*(wl**alphac)
      dainf=dtdif(3,linf)+dtdir(3,linf)
      dasup=dtdif(3,lsup)+dtdir(3,lsup)
      if(iaer.eq.0) goto 1236
      alphaa=alog(dasup/dainf)/coef
      betaa=dainf/(wlinf**(alphaa))
      dtota=betaa*(wl**alphaa)
 1236 dtott=dtotc*dtotr
      urinf=utdif(1,linf)+utdir(1,linf)
      ursup=utdif(1,lsup)+utdir(1,lsup)
      alphar=alog(ursup/urinf)/ coef
      betar=urinf/(wlinf**(alphar))
      utotr=betar*(wl**alphar)
      utinf=utdif(2,linf)+utdir(2,linf)
      utsup=utdif(2,lsup)+utdir(2,lsup)
      alphac=alog((utsup*urinf)/(utinf*ursup))/ coef
      betac=(utinf/urinf)/(wlinf**(alphac))
      utotc=betac*(wl**alphac)
      uainf=utdif(3,linf)+utdir(3,linf)
      uasup=utdif(3,lsup)+utdir(3,lsup)
      if(iaer.eq.0) goto 1237
      alphaa=alog(uasup/uainf)/ coef
      betaa=uainf/(wlinf**(alphaa))
      utota=betaa*(wl**alphaa)
 1237 utott=utotc*utotr
      arinf=sphal(1,linf)
      arsup=sphal(1,lsup)
      alphar=alog(arsup/arinf)/ coef
      betar=arinf/(wlinf**(alphar))
      asray=betar*(wl**alphar)
      atinf=sphal(2,linf)
      atsup=sphal(2,lsup)
      alphac=alog(atsup/atinf)/coef
      betac=atinf/(wlinf**(alphac))
      astot=betac*(wl**alphac)
      aainf=sphal(3,linf)
      aasup=sphal(3,lsup)
      if(iaer.eq.0) goto 1239
      alphaa=alog(aasup/aainf)/coef
      betaa=aainf/(wlinf**(alphaa))
      asaer=betaa*(wl**alphaa)
 1239 return
      end
