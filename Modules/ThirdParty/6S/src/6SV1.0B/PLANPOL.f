      Subroutine dirpopol(xq,xu,dirpol)
c
c This subroutine computes the plane of polarization

      real xu,xq,dirpol,pi

      pi=acos(-1.)

      if (abs(xq).lt.0.00001)then
	if (xu.gt.0.)then
	  dirpol=45.0
	else
	  dirpol=-45.0
	endif
	return
      endif

      if (abs(xu).lt.0.00001)then
        if (xq.gt.0.)then
	  dirpol=0.0
	else
	  dirpol=90.0
	endif
	return
      endif

      if (xq.gt.0.)then
	dirpol=90./pi*atan(xu/xq)
	return
      endif

      if (xu.gt.0.)then
	dirpol=90.+90./pi*atan(xu/xq)
      else
	dirpol=-90.+90./pi*atan(xu/xq)
      endif

      return
      end
