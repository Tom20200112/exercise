function res=bicycle(t,y,yp)
res=((4*10^7*1*exp(-1*y)+60*t^2/y^3)*(-yp)+60*t/y^2)/(78400000-4*10^7*exp(-1*y)-0.5*60*t^2/y^2)+1*y/t;
end