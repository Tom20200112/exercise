function [equator_a,equator_b] = equa_potential(qa,qb,xa,xb,ra,rb,d)

% evaluate the electrostatic potential on the equators of the two spheres a
% and b

theta=0:2*pi/600:2*pi;
equator_a=zeros(size(theta,2),1);
equator_b=zeros(size(theta,2),1);



for i=1:size(theta,2)
    for j=1:size(qa,1)
        equator_a(i)=equator_a(i)+qa(j)/sqrt(ra^2+xa(j)^2-2*ra*xa(j)*cos(theta(i)))+qb(j)/sqrt(ra^2+(d-xb(j))^2-2*ra*(d-xb(j))*cos(theta(i)));
        equator_b(i)=equator_b(i)+qb(j)/sqrt(rb^2+xb(j)^2-2*rb*xb(j)*cos(theta(i)))+qa(j)/sqrt(rb^2+(d-xa(j))^2-2*rb*(d-xa(j))*cos(theta(i)));
    end
end
    
end
