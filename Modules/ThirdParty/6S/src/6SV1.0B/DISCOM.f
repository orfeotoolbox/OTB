      subroutine discom (idatmp,iaer,iaer_prof,xmus,xmuv,phi,taer55,
     &taer55p,palt,phirad,nt,mu,np,rm,gb,rp,ftray,ipol,xlm1,xlm2,
     a     roatm_fi,nfi,nfilut,filut,roluts,rolutsq,rolutsu)

c - to vary the number of quadratures
      include "paramdef.inc"
      integer nquad
      common /num_quad/ nquad
      real phasel,qhasel,uhasel
      common /sixs_phase/ phasel(20,nqmax_p),qhasel(20,nqmax_p),
     &uhasel(20,nqmax_p)
      real pha,qha,uha,alphal,betal,gammal,zetal
      common /sixs_polar/ pha(nqmax_p),qha(nqmax_p),uha(nqmax_p),
     &alphal(0:nqmax_p),betal(0:nqmax_p),gammal(0:nqmax_p),
     &zetal(0:nqmax_p)
      real nbmu 
c - to vary the number of quadratures


      integer mu,np,ipol
      integer idatmp,iaer,nt,l,k
      real rm(-mu:mu),rp(np),gb(-mu:mu)
      real ftray,xlm1(-mu:mu,np),xlm2(-mu:mu,np)
      real roatm_fi(3,20,nfi)
c THOMAS
c			real rorayl_fi(nfi),romix_fi(nfi)
			real rorayl_fi(181),romix_fi(181)
      
c THOMAS
c      real rolut(mu,41)
      real rolut(25,41)
			real roluts(20,mu,41)
c THOMAS
c      real rolutq(mu,41)
      real rolutq(25,41)
			real rolutsq(20,mu,41)
c THOMAS
c      real rolutu(mu,41)
      real rolutu(25,41)
			real rolutsu(20,mu,41)
      real filut(mu,41)
      integer nfilut(mu)
      
      real xmus,xmuv,phi
      real taer55,taer55p,palt,phirad,ext,ome,gasym,phase,roatm
      real dtdir,dtdif,utdir,utdif,sphal,wldis,trayl,traypl,s
      real wlinf,wlsup,wl,tray,trayp,taer

      real rqatm,ruatm,rurayl,ruaero,rumix,qhase,uhase
      real taerp,piza,tamoy,tamoyp,pizmoy,rorayl,rqrayl,rqmix,rqaero
      real roaero,romix,ddirtt,ddiftt,udirtt,udiftt,sphalbt,ddirtr
      real ddiftr,udirtr,udiftr,sphalbr,ddirta,ddifta,udirta,udifta
      real sphalba,coeff

      common /sixs_aer/ext(20),ome(20),gasym(20),phase(20),qhase(20),
     &uhase(20)
      common /sixs_disc/ roatm(3,20),dtdir(3,20),dtdif(3,20),
     a utdir(3,20),utdif(3,20),sphal(3,20),wldis(20),trayl(20),
     a traypl(20),rqatm(3,20),ruatm(3,20)
      common /sixs_ffu/s(1501),wlinf,wlsup


      real alt_z,taer_z,taer55_z
      common /aeroprof/ num_z,alt_z(0:nt_p_max),taer_z(0:nt_p_max),
     &taer55_z(0:nt_p_max)
      integer iaer_prof


c     computation of all scattering parameters at wavelength
c     discrete values,so we can interpolate at any wavelength
 
      do 50 l=1,20
        wl=wldis(l)
        if ((wlsup.lt.wldis(1)).and.(l.le.2)) goto 30
        if (wlinf.gt.wldis(20).and.(l.ge.19)) goto 30
        if ((l.lt.20).and.(wldis(l).lt.wlinf).and.
     a     (wldis(l+1).lt.wlinf)) goto 50
        if ((l.gt.1).and.(wldis(l).gt.wlsup).and.
     a      (wldis(l-1).gt.wlsup)) goto 50
 
c     computation of rayleigh optical depth at wl
 30     call odrayl(wl,tray)


c plane case discussed here above
      if (idatmp.eq.0.or.idatmp.eq.4) then
	  if (idatmp.eq.4) trayp=tray
	  if (idatmp.eq.0) trayp=0.
	else
          trayp=tray*ftray
        endif
        trayl(l)=tray
        traypl(l)=trayp
 
c     computation of aerosol optical properties at wl
        
        taer=taer55*ext(l)
        taerp=taer55p*ext(l)
        piza=ome(l)
	
	
c - in case of the user-defined aerosol profile
        do i=1,num_z
         taer_z(i)=taer55_z(i)*ext(l)
        enddo
c - in case of the user-defined aerosol profile

c
c     computation of atmospheric reflectances
c               rorayl is rayleigh ref
c               roaero is aerosol ref
c     call trunca to decompose aerosol phase function in 
c     Legendre polynomials.
        nbmu=nquad
          do k=1,nbmu
            pha(k)=phasel(l,k)
          enddo
	  if (ipol.ne.0)then
            do k=1,nbmu
              qha(k)=qhasel(l,k)
              uha(k)=uhasel(l,k)
            enddo
	  endif
	  
	if (iaer.ne.0) call trunca(coeff,ipol) ! the routine is modified

        tamoy=taer*(1.-piza*coeff)
        tamoyp=taerp*(1.-piza*coeff)
        pizmoy=piza*(1.-coeff)/(1.-piza*coeff)
	


        call atmref(iaer,iaer_prof,tamoy,taer,tray,pizmoy,piza,tamoyp,
     s	  taerp,trayp,palt,phi,xmus,xmuv,phirad,nt,mu,np,rm,gb,rp,rorayl,
     s    roaero,romix,rqrayl,rqaero,rqmix,rurayl,ruaero,rumix,ipol,
     s    xlm1,xlm2,rorayl_fi,romix_fi,nfi,
     a      nfilut,filut,rolut,rolutq,rolutu)
     
        roatm(1,l)=rorayl
        roatm(2,l)=romix
        roatm(3,l)=roaero
c        write(6,*) 'rorayl',rorayl
c        write(6,*)'romix',romix
c        write(6,*)'roaero',roaero

        rqatm(1,l)=rqrayl
        rqatm(2,l)=rqmix
        rqatm(3,l)=rqaero
c        write(6,*) 'rqrayl',rqrayl
c        write(6,*)'rqmix',rqmix
c        write(6,*)'rqaero',rqaero

        ruatm(1,l)=rurayl
        ruatm(2,l)=rumix
        ruatm(3,l)=ruaero
c       write(6,*) 'rurayl',rurayl
c        write(6,*)'rumix',rumix
c        write(6,*)'ruaero',ruaero

	
      do ifi=1,nfi
      roatm_fi(1,l,ifi)=rorayl_fi(ifi)
      roatm_fi(2,l,ifi)=romix_fi(ifi)
      enddo
      
      do i=1,mu
      do j=1,41
      roluts(l,i,j)=rolut(i,j)
      rolutsq(l,i,j)=rolutq(i,j)
      rolutsu(l,i,j)=rolutu(i,j)
      enddo
      enddo



 
c     computation of scattering transmitances (direct and diffuse)
c     first time for rayleigh ,next total (rayleigh+aerosols)
       call scatra(iaer_prof,tamoy,tamoyp,tray,trayp,pizmoy,
     a palt,nt,mu,rm,gb,
     a xmus,xmuv,ddirtt,ddiftt,udirtt,udiftt,sphalbt,ddirtr,ddiftr,
     a udirtr,udiftr,sphalbr,ddirta,ddifta,udirta,udifta,sphalba)
        dtdir(1,l)=ddirtr
        dtdif(1,l)=ddiftr
        dtdir(2,l)=ddirtt
        dtdif(2,l)=ddiftt
        dtdir(3,l)=ddirta
        dtdif(3,l)=ddifta
        utdir(1,l)=udirtr
        utdif(1,l)=udiftr
        utdir(2,l)=udirtt
        utdif(2,l)=udiftt
        utdir(3,l)=udirta
        utdif(3,l)=udifta
        sphal(1,l)=sphalbr
        sphal(2,l)=sphalbt
        sphal(3,l)=sphalba
	
   50 continue

      return
      end
