function [xa,xb,qa,qb] = generate(Qa,Qb,N,ra,rb,d)
% generate: Classical Electrodynamics by J.D.Jackson, problem 2.6
% generate arrays of image charges and their positions in both spheres,
% based on given total charges Qa&Qb, radii ra&rb, and number of image
% charges in one sphere (N)

qa=zeros(N,1);
qb=zeros(N,1);
xa=zeros(N,1);
xb=zeros(N,1);
qa(1)=1;
qb(1)=1;

qoa=0;
qea=0;
qob=0;
qeb=0;



for j=2:N
qa(j)=-ra*qb(j-1)/(d-xb(j-1));
qb(j)=-rb*qa(j-1)/(d-xa(j-1));
xa(j)=ra^2/(d-xb(j-1));
xb(j)=rb^2/(d-xa(j-1));
end


for i=1:2:N
    qoa=qoa+qa(i);
    qob=qob+qb(i);
end

for i=2:2:N
    qea=qea+qa(i);
    qeb=qeb+qb(i);
end    

x=(qea-qob*Qa/Qb)/(qeb*Qa/Qb-qoa);

for i=1:2:N
    qa(i)=qa(i)*x;
end

for i=2:2:N
    qb(i)=qb(i)*x;
end    

qa=qa*(Qa/sum(qa));
qb=qb*Qb/sum(qb);

end

