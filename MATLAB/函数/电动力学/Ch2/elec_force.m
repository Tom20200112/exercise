function [F]=elec_force(qa,qb,xa,xb,d)
% elec_force: Classical Electrodynamics by J.D.Jackson, problem 2.6
% calculate the mutual force between the two charged spheres

F=0;
for i=1:size(qa)
    for j=1:size(qb)
        F=F+qa(i)*qb(j)/(d-xa(i)-xb(j))^2;
    end    
end

end


