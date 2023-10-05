# -*- coding: utf-8 -*-
"""
Created on Tue Mar  9 12:12:43 2021

@author: CDNEL
"""
import sys
sys.path.append('/Library/Frameworks/Python.framework/Versions/3.11/lib/python3.11/site-packages')

import pyvisa
import ast


from PyQt5.QtWidgets import (QComboBox, 
        QGridLayout, QLabel, QLineEdit,
        QPushButton, QCheckBox,QFileDialog,QTabWidget, QWidget)
from PyQt5.QtGui import QDoubleValidator
from PyQt5.QtTest import QTest

rm = pyvisa.ResourceManager()



class MercuryiTC:
    T_field=['Thermometer','Setpoint (K)', 'Ramp rate (K/mn)','Heater (%)']
    ch_name=['3He_high','3He_low','Sorb','1Kpot']
    htr_control=['3He_high','Sorb']
    def __init__(self, name, channels):
        self.name=name
        self.ch=ast.literal_eval(channels)
        self.instr=rm.open_resource('TCPIP0::192.168.1.3::7020::SOCKET')
        self.instr.read_termination='\n'
        self.instr.write_termination='\n'
        self.npts=0
        self.counter=self.npts
        self.measure()
        print('Connected to Mercury iTC ('+name+')!')
    
    def measure(self):
        if self.counter==self.npts:
            msmt=[]
            if self.ch[0]:
                msmt.append(float(self.instr.query('READ:DEV:DB6.T1:TEMP:SIG:TEMP').split(':')[6].replace('K','')))
            if self.ch[1]:
                msmt.append(float(self.instr.query('READ:DEV:DB7.T1:TEMP:SIG:TEMP').split(':')[6].replace('K',''))) #DB7.T1
            if self.ch[2]:
                msmt.append(float(self.instr.query('READ:DEV:MB1.T1:TEMP:SIG:TEMP').split(':')[6].replace('K','')))
            if self.ch[3]:
                msmt.append(float(self.instr.query('READ:DEV:DB8.T1:TEMP:SIG:TEMP').split(':')[6].replace('K','')))
            self.last=msmt
            self.counter=0
        else:
            msmt=self.last
            self.counter+=1
        return msmt
    
    def last_msmt(self):
        return self.last
    
    def header(self):
        hdr=[]
        for ind in range(len(self.ch)):
            if self.ch[ind]:
                hdr.append(self.name+'_'+self.ch_name[ind])
        return hdr
    
    def control_GUI(self, frame):
        j=0
        label={}
        for i, f in enumerate(self.T_field):
            label[f] = QLabel(f)
            j+=1
        self.thermometer=QComboBox()
        self.thermometer.addItems(self.htr_control)
        self.stop_T = QLineEdit(frame)
        self.ramp_T = QLineEdit(frame)
        # self.heatrange= QComboBox()
        # self.heatrange.addItems(self.htr_range)
        currentsetpoint=self.get_temp()
        self.stop_T.setText(str(currentsetpoint))
        currentrate=self.get_ramp()
        self.ramp_T.setText(str(currentrate))
        
        self.start_Tramp = QPushButton('Ramp')
        self.start_Tramp.clicked.connect(lambda: self.set_temp(self.stop_T,self.ramp_T,self.thermometer))
        
        self.htr_off = QPushButton('Heater Off')
        self.htr_off.clicked.connect(lambda: self.off(self.thermometer))
        
        self.defnpts = QLineEdit(frame)
        self.defnpts.setText(str(self.npts))
        self.setnpts = QPushButton('Set')
        self.setnpts.clicked.connect(lambda: self.set_pts(int(self.defnpts.text())))
        
        layout = QGridLayout()
        for i, f in enumerate(self.T_field):
            layout.addWidget(label[f], i, 0, 1, 1)
        
        layout.addWidget(QLabel('Measure Every n Points'), 4, 0, 1, 1)
        layout.addWidget(self.thermometer, 0, 1, 1, 1)
        layout.addWidget(self.stop_T, 1, 1, 1, 1)
        layout.addWidget(self.ramp_T, 2, 1, 1, 1)
        # layout.addWidget(self.heatrange, 2, 1, 1, 1)
        layout.addWidget(self.start_Tramp, 3, 1, 1, 1)
        layout.addWidget(self.htr_off, 3, 0, 1, 1)
        layout.addWidget(self.defnpts, 4, 1, 1, 1)
        layout.addWidget(self.setnpts, 5, 1, 1, 1)
        layout.setRowStretch(j+1, 2)
        frame.setLayout(layout) 
        
    def get_temp(self):
        return self.instr.query('READ:DEV:DB6.T1:TEMP:SIG:TEMP').split(':')[6].replace('K','')
    
    def get_ramp(self):
        return self.instr.query('READ:DEV:DB6.T1:TEMP:LOOP:RSET').split(':')[6].replace('K/m','')
    
    def set_temp(self, tset, ramprate, therm):
        tset=tset.text()
        ramprate=ramprate.text()
        if therm.currentText()=='3He_high':
            self.instr.query('SET:DEV:DB6.T1:TEMP:LOOP:RENA:ON')
            self.instr.query('SET:DEV:DB6.T1:TEMP:LOOP:RSET:'+ramprate)
            self.instr.query('SET:DEV:DB6.T1:TEMP:LOOP:TSET:'+tset)
            self.instr.query('SET:DEV:DB6.T1:TEMP:LOOP:ENAB:ON')
        elif therm.currentText()=='Sorb':
            self.instr.query('SET:DEV:MB1.T1:TEMP:LOOP:RENA:ON')
            self.instr.query('SET:DEV:MB1.T1:TEMP:LOOP:RSET:'+ramprate)
            self.instr.query('SET:DEV:MB1.T1:TEMP:LOOP:TSET:'+tset)
            self.instr.query('SET:DEV:MB1.T1:TEMP:LOOP:ENAB:ON')
    
    def off(self,therm):
        if therm.currentText()=='3He_high':
            self.instr.query('SET:DEV:DB6.T1:TEMP:LOOP:RENA:OFF')
            self.instr.query('SET:DEV:DB6.T1:TEMP:LOOP:ENAB:ON')
            self.instr.query('SET:DEV:DB6.T1:TEMP:LOOP:HSET:0')
            self.instr.query('SET:DEV:DB6.T1:TEMP:LOOP:TSET:1.5')
        if therm.currentText()=='Sorb':
            self.instr.query('SET:DEV:MB1.T1:TEMP:LOOP:RENA:OFF')
            self.instr.query('SET:DEV:MB1.T1:TEMP:LOOP:ENAB:ON')
            self.instr.query('SET:DEV:MB1.T1:TEMP:LOOP:HSET:0')
    def set_pts(self, num):
        self.npts=num
        self.counter=num
            
class AMI430:
    ctrl_label=['Field Setpoint']
    def __init__(self, name):
        self.name=name
        self.npts=0
        self.counter=self.npts
        # self.ch=ast.literal_eval(channels)
        self.instr=rm.open_resource("TCPIP0::192.168.1.5::7180::SOCKET")
        self.instr.read_termination='\r\n'
        self.instr.write_termination='\r\n'
        print('Connected to: ' + str(self.instr.query('*IDN?')))
        self.instr.read()
        self.instr.read()
    
    def measure(self):
        if self.counter==self.npts:
            b=self.instr.query("FIELD:MAGnet?")
            msmt=[float(b)]
            self.last=msmt
            self.counter=0
        else:
            msmt=self.last
            self.counter+=1
        return msmt
    
    def last_msmt(self):
        return self.last
    
    def header(self):
        hdr=['B']
        return hdr
    
    def control_GUI(self, frame):
        tab_B = QTabWidget()
        tab_set = QWidget()
        tab_ramp = QWidget()
        tab_msmt = QWidget()
        tab_B.addTab(tab_set,'Setpoint')
        tab_B.addTab(tab_ramp,'Ramp Rate')
        tab_B.addTab(tab_msmt,'Measurement Settings')
        
        
        label={}
        j=0
        for i, f in enumerate(self.ctrl_label):
            label[f] = QLabel(f)
            j+=1
        self.field_set = QLineEdit(frame)
        currentsetpoint=self.current_setpoint()
        self.field_set.setText(str(currentsetpoint))
        self.start_ramp = QPushButton('Ramp')
        self.pause = QPushButton('Pause')
        self.start_ramp.clicked.connect(lambda: self.set_B(self.field_set))
        self.pause.clicked.connect(self.pause_ramp)

        layout = QGridLayout()
        for i, f in enumerate(self.ctrl_label):
            layout.addWidget(label[f], i, 0, 1, 1) 
        layout.addWidget(self.field_set, 0, 1, 1, 1)
        layout.addWidget(self.pause, 1, 1, 1, 1)
        layout.addWidget(self.start_ramp, 2, 0, 1, 2)
        # layout.setRowStretch(j+1, 2)
        
        tab_set.setLayout(layout)
        
        seg=int(self.instr.query('RAMP:RATE:SEG?'))
        rlabel={}
        self.rset={}
        ratelayout=QGridLayout()
        for i in range(seg):
            s=i+1
            top=self.instr.query('RAMP:RATE:FIELD:'+str(s)+'?').split(',')[1]
            rlabel[i]=QLabel('Segment '+str(s)+'(to '+top+' T)')
            self.rset[i]=QLineEdit()
            self.rset[i].setText(self.instr.query('RAMP:RATE:FIELD:'+str(s)+'?').split(',')[0])
            ratelayout.addWidget(rlabel[i],i,0,1,1)
            ratelayout.addWidget(self.rset[i],i,1,1,1)
        setrate=QPushButton('Set Rates')
        ratelayout.addWidget(setrate,seg,1,1,1)
        setrate.clicked.connect(lambda: self.set_rate(self.rset))
        
        tab_ramp.setLayout(ratelayout)
        
        msmtlayout=QGridLayout()
        
        self.defnpts = QLineEdit(frame)
        self.defnpts.setText(str(self.npts))
        self.setnpts = QPushButton('Set')
        self.setnpts.clicked.connect(lambda: self.set_pts(int(self.defnpts.text())))        
        
        msmtlayout.addWidget(QLabel('Measure Every n Points'), 0, 0, 1, 1)
        msmtlayout.addWidget(self.defnpts, 0, 1, 1, 1)
        msmtlayout.addWidget(self.setnpts, 1, 1, 1, 1)
        
        tab_msmt.setLayout(msmtlayout)
        
        lo=QGridLayout()
        lo.addWidget(tab_B,0,0,1,2)
        frame.setLayout(lo) 
      
    def current_setpoint(self):
        return self.instr.query('FIELD:TARG?')
    
    def set_B(self, value):
        setpoint=str(value.text())
        num=float(setpoint)
        if num>14.9:
            setpoint='14.9'
        elif num<-14.9:
            setpoint='-14.9'

        self.instr.write('CONF:FIELD:TARG '+setpoint)
        self.instr.write('RAMP')
        
    def pause_ramp(self):
        self.instr.write('PAUSE')

    def set_rate(self, seglist):
        i=1
        print(seglist[0].text)
        for j in seglist:
            rate=seglist[j].text()
            to=self.instr.query('RAMP:RATE:FIELD:'+str(i)+'?').split(',')[1]
            self.instr.write('CONF:RAMP:RATE:FIELD '+str(i)+','+str(rate)+','+to)
            i+=1
    
    def set_pts(self, num):
        self.npts=num
        self.counter=num