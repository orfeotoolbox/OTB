       Subroutine POLNAD(xts,xtv,phi,pveg,ropq,ropu)
       real xts,xtv,phi,pveg,ropq,ropu
       
C      compute the polarized components of the surface 
C      according to the Nadal and Breon model

C   xts is the sun zenith angle in degrees
C   xtv is the view zenith angle in degrees
C   phi is the relative azimuth between sun and view in degrees
C   pveg is the "percentage of vegetation" for use in ponderating
C   the polarization of bare soil and vegetation
C   ropq and ropu and the stokes parameter (Q,U) of the polarized
C   surface reflectance
        real pi,dtr,csca,sca,alpha,alphap,N,mui,mut
C   csca is the cosine of the scattering angle (sca)
C   alpha is the incidence angle used for specular reflection
C   alphap is the refraction angle after specular reflection
C   N is the refractive index of vegetation (assumed 1.5)

C   intermediate variable to compute direction of polarization 
C   according to fresnel law
        real mus,muv,sinv,cksi,ksi,xf1,xf2
        N=1.5	
        pi=acos(0.0)*2.0
        dtr=pi/180.0
	csca=-cos(xts*dtr)*cos(xtv*dtr)-sin(xts*dtr)
     s      *sin(xtv*dtr)*cos(phi*dtr)
        sca=acos(csca)
	alpha=(pi-sca)/2.0
	alphap=asin(sin(alpha)/N)
	mui=cos(alpha)
	mut=cos(alphap)
	
	xf1=(N*mut-mui)/(N*mut+mui)
	xf2=(N*mui-mut)/(N*mui+mut)
	fpalpha=0.5*(xf1*xf1-xf2*xf2)
        rpveg=fpalpha/4./(cos(xts*dtr)+cos(xtv*dtr))
	rpsoil=fpalpha/4./cos(xts*dtr)/cos(xtv*dtr)
	rpsur=rpveg*pveg+rpsoil*(1.-pveg)
	
C compute rotation angle for Q and U
        muv=cos(xtv*dtr)
	mus=cos(xts*dtr)
	sinv=sin(xtv*dtr)
	if (xtv.gt.0.5) then
        if (sin(phi*dtr).lt.0) then
        cksi=(muv*csca+mus)/sqrt(1.-csca*csca)/sinv
        else
        cksi=-(muv*csca+mus)/sqrt(1.-csca*csca)/sinv
        endif
        else
        cksi=0.0
        endif
        
	if (cksi.gt.1.) cksi=1.
	if (cksi.lt.-1.) cksi=-1.
        ksi=acos(cksi)/dtr
C	write(6,*) "KSI=",ksi	
	ropq=rpsur*(2.*cksi*cksi-1.)
	ropu=-rpsur*2.*cksi*sqrt(1.-cksi*cksi)
	return
	end
       
