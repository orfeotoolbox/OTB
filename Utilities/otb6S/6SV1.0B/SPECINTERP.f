      subroutine specinterp(wl,taer55,taer55p,
     s     tamoy,tamoyp,pizmoy,pizmoyp,ipol)

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
      integer nbmu
c - to vary the number of quadratures

      real wl,taer55,taer55p,tamoy,tamoyp,pizmoy,pizmoyp,roatm
      real dtdir,dtdif,utdir,utdif,sphal,wldis,trayl,traypl
      real ext,ome,gasym,phase,coef,rpatm,dpatm
      real qhase,uhase
      real wlinf,alphaa,betaa,tsca,coeff
      integer linf,ll,lsup,k,ipol,ipol0

      common /sixs_disc/ roatm(3,20),dtdir(3,20),dtdif(3,20),
     s utdir(3,20),utdif(3,20),sphal(3,20),wldis(20),trayl(20),
     s traypl(20),rpatm(3,20),dpatm(3,20)
      common /sixs_aer/ext(20),ome(20),gasym(20),phase(20),qhase(20),
     &uhase(20)


      real test1,test2,test3

      nbmu=nquad
      linf=1
      do 80 ll=1,19
        if(wl.ge.wldis(ll).and.wl.le.wldis(ll+1)) linf=ll
   80 continue
      if(wl.gt.wldis(20)) linf=19
      lsup=linf+1
      coef=alog(wldis(lsup)/wldis(linf))
      coefl=(wl-wldis(linf))/(wldis(lsup)-wldis(linf))
      wlinf=wldis(linf)
      alphaa=alog(ext(lsup)*ome(lsup)/(ext(linf)*ome(linf)))/coef
      betaa=ext(linf)*ome(linf)/(wlinf**(alphaa))
      tsca=taer55*betaa*(wl**alphaa)/ext(4)
      alphaa=alog(ext(lsup)/(ext(linf)))/coef
      betaa=ext(linf)/(wlinf**(alphaa))
      tamoy=taer55*betaa*(wl**alphaa)/ext(4)
      tamoyp=taer55p*betaa*(wl**alphaa)/ext(4)
      pizmoy=tsca/tamoy
      pizmoyp=pizmoy
      do 81 k=1,nbmu
        alphaa=alog(phasel(lsup,k)/phasel(linf,k))/coef
        betaa=phasel(linf,k)/(wlinf**(alphaa))
        pha(k)=betaa*(wl**alphaa)
	if (ipol.ne.0)then
	
	  test1=qhasel(linf,k)
	  test2=qhasel(lsup,k)
	  test3=qhasel(lsup,k)*qhasel(linf,k)
        if((test1.lt.0.001).or.(test2.lt.0.001)
     s            .or.(test3.lt.0.0)) then
	  qha(k)=qhasel(linf,k)+(qhasel(lsup,k)-qhasel(linf,k))*coefl
	  else
          alphaa=alog(qhasel(lsup,k)/qhasel(linf,k))/coef
          betaa=qhasel(linf,k)/(wlinf**(alphaa))
          qha(k)=betaa*(wl**alphaa)
	  endif
	  
	  test1=uhasel(linf,k)
	  test2=uhasel(lsup,k)
	  test3=uhasel(lsup,k)*qhasel(linf,k)
        if((test1.lt.0.001).or.(test2.lt.0.001)
     s            .or.(test3.lt.0.0)) then
	  uha(k)=uhasel(linf,k)+(uhasel(lsup,k)-uhasel(linf,k))*coefl
	  else
          alphaa=alog(uhasel(lsup,k)/uhasel(linf,k))/coef
          betaa=uhasel(linf,k)/(wlinf**(alphaa))
          uha(k)=betaa*(wl**alphaa)
	  endif
	endif  
	
 81     continue
c here we don't need coefficients for computation of the polarization
      ipol0=0
      call trunca(coeff,ipol0)
      tamoy=tamoy*(1.-pizmoy*coeff)
      tamoyp=tamoyp*(1.-pizmoyp*coeff)
      pizmoy=pizmoy*(1.-coeff)/(1.-pizmoy*coeff)
      return
      end
