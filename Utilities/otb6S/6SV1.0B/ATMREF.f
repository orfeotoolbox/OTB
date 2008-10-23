      subroutine atmref (iaer,iaer_prof,tamoy,taer,trmoy,pizmoy,piza,
     s tamoyp,taerp,trmoyp,palt,phi,xmus,xmuv,phirad,nt,mu,np,rm,gb,rp,
     s rorayl,roaero,romix,rqrayl,rqaero,rqmix,rurayl,ruaero,rumix,
     a      ipol,xlm1,xlm2,rorayl_fi,romix_fi,nfi,
     a      nfilut,filut,rolut,rolutq,rolutu)
     
      real    rolut(mu,41),rolutq(mu,41),rolutu(mu,41)
c THOMAS
c			real    rolutd(mu,41)
			real    rolutd(25,41)
			
      real    filut(mu,41)
      integer nfilut(mu)

      integer mu,np,nfi
      real rm(-mu:mu),rp(np),gb(-mu:mu)
      real tamoy,trmoy,pizmoy,tamoyp,trmoyp,palt
      real xlm2(-mu:mu,np)
c THOMAS
C      real xlphim(nfi)
      real xlphim(181)
      real rorayl_fi(nfi),romix_fi(nfi)
      real phi,xmus,xmuv,phirad
      real delta,sigma,tamol,tamolp
      real rorayl,roaero,romix,rqrayl,rqaero,rqmix
      real rurayl,ruaero,rumix
      real xlm1(-mu:mu,np)
c THOMAS
c			real xqm1(-mu:mu,np),xum1(-mu:mu,np)
			real xqm1(-25:25,49),xum1(-25:25,49)
      
       integer iaer,nt,ipol,iaer_prof
 
      common /sixs_del/ delta,sigma

c
c     atmospheric reflectances
      rorayl=0.
      roaero=0.
      romix=0.
      rqrayl=0.
      rqaero=0.
      rqmix=0.
      rurayl=999.
      ruaero=999.
      rumix=999.
c
c 3 possible cases (satellite,plane,ground)
      if(palt.gt.0.0)then
        rm(-mu)=-xmuv
        rm(mu)=xmuv
        rm(0)=-xmus

c  -----rayleigh reflectance = rorayl,rprayl
        tamol=0.
        tamolp=0.
 	call ospol(iaer_prof,tamol,trmoy,piza,tamolp,trmoyp,palt,
     s             phirad,nt,mu,np,rm,gb,rp,xlm1,xqm1,xum1,xlphim,nfi,
     s                     nfilut,filut,rolutd,rolutd,rolutd)
	if (ipol.ne.1)then
          rorayl=xlm1(-mu,1)/xmus
          romix=rorayl
	do ifi=1,nfi
           rorayl_fi(ifi)=(xlphim(ifi)/xmus)
           romix_fi(ifi)=(xlphim(ifi)/xmus)
        enddo
	endif
	if (ipol.ne.0)then
c              -> here we define 2 reflectances from Stockes' parameters
c                 but they don't have any physical interpretations, this is
c                 just to be coherent with the reflectance rorayl
c              -> parameters rorayl2,roaero2,romix2 have been introduced
c                 to compute the degrees of polarization. 
	  rorayl=xlm1(-mu,1)/xmus
	  rqrayl=xqm1(-mu,1)/xmus
C         WRITE(6,*) "TAURAYL=",trmoy," RORAYL = ",rorayl
          rqmix=rqrayl
	  rurayl=xum1(-mu,1)/xmus
          rumix=rurayl
	do ifi=1,nfi
           rorayl_fi(ifi)=(xlphim(ifi)/xmus)
        enddo
	endif
c       write(6,*)'ATMREF- rayleigh',rorayl,rqrayl,rurayl

        if (iaer.eq.0) then
	 romix=rorayl
	 rqmix=rqrayl
	 rumix=rurayl
	 roaero=0.0
	 rqaero=0.0
	 ruaero=0.0
        return
	endif

c  -----aerosol reflectance = roaero,rpaero 
        tamol=0.
        tamolp=0.
	if (ipol.ne.1)then
          call os(iaer_prof,tamoy,tamol,pizmoy,tamoyp,tamolp,palt,
     s      phirad,nt,mu,np,rm,gb,rp,xlm1,xlphim,nfi,rolutd)
          roaero=(xlm1(-mu,1)/xmus)
	endif
	if (ipol.ne.0)then
 	  call ospol(iaer_prof,taer,tamol,piza,taerp,tamolp,palt,
     s             phirad,nt,mu,np,rm,gb,rp,xlm1,xqm1,xum1,xlphim,nfi,
     s                     nfilut,filut,rolutd,rolutd,rolutd)
	  rqaero=xqm1(-mu,1)/xmus
	  ruaero=xum1(-mu,1)/xmus
	  if (ipol.eq.1)roaero=xlm1(-mu,1)/xmus
	endif
c       write(6,*)'ATMREF - aero', roaero,rqaero,ruaero

c  -----rayleigh+aerosol reflectance = romix,rpmix
        if (ipol.ne.1)then
 	  call os(iaer_prof,tamoy,trmoy,pizmoy,tamoyp,trmoyp,palt,
     s      phirad,nt,mu,np,rm,gb,rp,xlm1,xlphim,nfi,rolutd)
          romix=(xlm1(-mu,1)/xmus)
	do ifi=1,nfi
           romix_fi(ifi)=(xlphim(ifi)/xmus)
        enddo
	endif
	if (ipol.ne.0)then
	  call ospol(iaer_prof,taer,trmoy,piza,taerp,trmoyp,palt,
     s             phirad,nt,mu,np,rm,gb,rp,xlm1,xqm1,xum1,xlphim,nfi,
     s                     nfilut,filut,rolut,rolutq,rolutu)
	  rqmix=xqm1(-mu,1)/xmus
	  rumix=xum1(-mu,1)/xmus
	  if (ipol.eq.1)then 
	  romix=xlm1(-mu,1)/xmus
          do ifi=1,nfi
          romix_fi(ifi)=(xlphim(ifi)/xmus)
          enddo
	  endif
	endif
	
	do i=1,mu
	do j=1,41
	rolut(i,j)=rolut(i,j)/xmus
	rolutq(i,j)=rolutq(i,j)/xmus
	rolutu(i,j)=rolutu(i,j)/xmus
	enddo
	enddo
	
c       write(6,*)'ATMREF - mix',romix,rqmix,rumix
      endif
c
      return
      end
