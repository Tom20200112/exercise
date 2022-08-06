fun2=@(x) density_g(x).*(x.^2-1).*x;
fun3=@(x) density_g(x).*(x.^3-1).*x;
fun4=@(x) density_g(x).*(x.^4-1).*x;

e2=integral(fun2,0,1);
e3=integral(fun3,0,1);
e4=integral(fun4,0,1);

a1=225*e2-420*e3+210*e4;
a2=-420*e2+2450/3*e3-420*e4;
a3=210*e2-420*e3+441/2*e4;

rho=0:0.001:1;
plot(rho,0.04*density_g(rho),'-.b');
hold on;
plot(rho,a1.*rho.^2+a2.*rho.^3+a3.*rho.^4-(a1+a2+a3),'--r');
hold on;

options = odeset('RelTol',1e-7,'AbsTol',1e-5);

[r,psi] = ode45(@poisson,[0.001 1],[0; 0],options);
plot(r, psi(:,1)-psi(size(psi,1),1), '-b');
hold on;

xlabel('\rho');
ylabel('\Phi(\rho)');

function partial=poisson(r,psi)
partial=[psi(2);-psi(2)/r-density_g(r)];
end



