function q=millikan(Rho,g,Eta,l,tf,b,p,d,U)
q= 18*pi/sqrt(2*Rho*g)*(Eta*l/(tf*(1+b/(p*sqrt(9*Eta*l/(2*Rho*g*tf))))))^(1.5)*d/U;
end