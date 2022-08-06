clc();
clf();
clear(); import('preprocess.*'); % parameters
dir = '~/Downloads/'; % file path
sstname = [dir,'sst.mean.nc']; % file name
moi = 4:7; % months of interest
latoi = [50,75]; % latitude of interest(N)
lonoi = [-25,10]; % longitude of interest(E) geodetic2cartesian(0,0,latoi,lonoi,1); landthreshold = 1/16; % min ratio of NaNs in land
sst = ncread(sstname); % read dataset
sst.time = datetime(1800,1,1)+sst.time; % day, from 1800.1.1
sst = filtbymonth(sst,moi);
sst = meanbyyear(sst);
sst = trimlongitude(sst); % use negative present W. lat instead of 180+
sst = filtbylatlon(sst,latoi,lonoi);
sst.sst = permute(sst.sst,[2,1,3]); % permute to make imshow() convenience 
[sst,landmask] = landclear(sst,round(landthreshold*numel(sst.time))); % filter land NaNs 
sst.sst = reshape(sst.sst,[],60);
sst.sst = sst.sst(:,any(sst.sst));
Y = sst.sst;
A=[
crossvalidation(Y,[1:50, []],51:60) crossvalidation(Y,[1:40,51:60],41:50) crossvalidation(Y,[1:30,41:60],31:40) crossvalidation(Y,[1:20,31:60],21:30) crossvalidation(Y,[1:10,21:60],11:20) crossvalidation(Y,[ [],11:60], 1:10) ];
A=sort(sqrt(A));
% plot(A(:,[1,3]))
b = boxplot(A,'Labels',{'(p=1, d=1)','(p=1, d=2)','(p=2, d=1)','(p=2, d=2)'}...
    );
grid('on');