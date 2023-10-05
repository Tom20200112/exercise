# -*- coding: utf-8 -*-
"""
Created on Sun Jan  2 11:45:35 2022

@author: thecd
"""

import numpy as np
import matplotlib.pyplot as plt
from Measurement_Instruments import SR830_inst#, SR860_inst, K2400_inst, SIM_inst, yoko_inst, K6221_inst
import time
import os.path as osp
from Save1 import Savetxt

adr=6
adr='GPIB0::6::INSTR'
ins=SR830_inst('lock-in',adr,'[True,True,False,False,False,False,False,False]')

path_name='C:/Users/Cory Dean/Desktop/USER DATA/Jordan'
filename='filter_test'
header=['f','V_x','V_y']
userdoc=osp.join(osp.expanduser("~"),path_name)
header="\t".join(header)
notes='100mV excitation, round trip 1->2'
               
txt_mean = Savetxt(header,filename,userdoc,notes) #Creates file to save

stop=1
start=5 #A
steps=101
# delta=2e-5
wait=.3

#pt=npts*steps
data=np.zeros((3,steps))

exponents=np.linspace(start,stop,steps, endpoint=True)
ten=np.zeros_like(exponents)+10
freq=np.power(ten,exponents)
j=0

for val in freq:
    ins.set_freq(val)
    time.sleep(1)
    d=ins.measure()
    data[0,j]=val
    data[1,j]=d[0]
    data[2,j]=d[1]
    pt=[str(val),str(d[0]),str(d[1])]
    txt_mean.save_line(pt)
    j+=1
#%%
fig,ax=plt.subplots()
ax.plot(data[0],data[1])
ax.set_xscale('log')