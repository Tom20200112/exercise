function cone_arrow(size,direction,x,y,z)
% cone_arrow 在(x,y,z)处画一个指向direction方向、高度为size的圆锥形箭头

r=0.5*size;  %圆锥半径
length=size; %圆锥的高

a=-direction(2);b=direction(1);c=0;
costheta=direction(3)/(direction(1)^2+direction(2)^2+direction(3)^2)^0.5;
theta=acos(costheta)/pi*180; 

[X,Y,Z]=cylinder(0:0.1*size:r,100);
h1=surface(X,Y,length*Z,...
'EdgeColor','none',...
'FaceColor','r');
light;
hold on

rotate(h1,[a b c],(theta-180),[0 0 0])

set(h1,'XData',get(h1,'XData')+x)
set(h1,'YData',get(h1,'YData')+y)
set(h1,'ZData',get(h1,'ZData')+z)

end

