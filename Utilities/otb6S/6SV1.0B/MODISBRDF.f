      subroutine modisbrdf(p1,p2,p3,mu,np,rm,rp,
     s           brdfint)
      real p1,p2,p3,xmu,view
      real dts,dtv,dfs,dfv,dfi
      real rts,rtv,rfs,rfv,rfi,rpha
      real cts,ctv,cfi,cpha
      real sts,stv,sfi
      real tanti,tantv
      real cost,sint,tvar
      real rossthick,rosselt,lispars
      real angdist,angtemp,angover
      integer mu,np,k,j
      real rm(-mu:mu),rp(np),brdfint(-mu:mu,np)
      rts=acos(rm(0))
      pi=atan(1.)*4.
      do 1 k=1,np
      do 2 j=1,mu
      rtv=acos(rm(j))
      if (j.eq.mu) then
         rfi=rm(-mu)
         else
         rfi=rp(k)+rm(-mu)
         endif
      rfi=abs(rfi)
	cts=cos(rts)
	ctv=cos(rtv)
	sts=sin(rts)
	stv=sin(rtv)
	cfi=cos(rfi)
	sfi=sin(rfi)
	cpha=cts*ctv+sts*stv*cfi
	rpha=acos(cpha)
	
	rosselt=(pi/2-rpha)*cpha+sin(rpha)
	rossthick=(rosselt/(cts+ctv))-pi/4.
	
	tanti=tan(rts)
	tantv=tan(rtv)
	
	angdist=tanti*tanti+tantv*tantv-2.*tanti*tantv*cfi
	angdist=sqrt(angdist)
	
	angtemp=1./cts+1./ctv
	cost=2.*sqrt(angdist*angdist+tanti*tanti*tantv
     &	*tantv*sfi*sfi)
	cost=cost/angtemp
	if (cost.ge.1.) cost=1.
	if (cost.le.-1.) cost=-1.
	tvar=acos(cost)
	sint=sqrt(1.-cost*cost)
	angover=(tvar-sint*cost)*angtemp/pi
	lispars=angover-angtemp+0.5*(1.+cpha)/cts/ctv
      
      brdfint(j,k)=p1+p2*rossthick+p3*lispars
  2   continue
  1   continue
      return
      end
