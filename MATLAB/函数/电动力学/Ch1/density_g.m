function g = density_g(rho)
% 柱对称电荷分布样例，来自J.D.Jackson 经典电动力学
%   Detailed explanation goes here
g = -5.*(1-rho)+10^4.*rho.^5.*(1-rho).^5;

end

