# -*- coding: utf-8 -*-
"""
Created on Tue Mar  9 12:12:43 2021

@author: CDNEL
"""
import pyvisa
import ast

from PyQt5.QtWidgets import (QComboBox, 
        QGridLayout, QLabel, QLineEdit,
        QPushButton, QCheckBox,QFileDialog, QMessageBox)
from PyQt5.QtGui import QDoubleValidator
from PyQt5.QtTest import QTest

rm = pyvisa.ResourceManager()



class MercuryiTC_teslatron:
    T_field=['Setpoint (K)', 'Ramp rate (K/mn)']
    ch_name=['Probe','VTI', 'VTI_P', 'NV']
    htr_control=['Probe','VTI']
    oncooldown=False
    vti_pressure=1.8
    def __init__(self, name, channels):
        self.name=name
        self.ch=ast.literal_eval(channels)
        self.instr=rm.open_resource('ASRL7::INSTR')
        self.instr.read_termination='\n'
        self.instr.write_termination='\n'
        
        print('Connected to Mercury iTC ('+name+')!')
    def last_msmt(self):
        return self.last
    def measure(self):
        msmt=[]
        if self.ch[0]:
            msmt.append(self.instr.query('READ:DEV:DB8.T1:TEMP:SIG:TEMP').split(':')[6].replace('K',''))
        if self.ch[1]:
            msmt.append(self.instr.query('READ:DEV:MB1.T1:TEMP:SIG:TEMP').split(':')[6].replace('K','')) #DB7.T1
        if self.ch[2]:
            msmt.append(self.instr.query('READ:DEV:DB5.P1:PRES:SIG:PRES').split(':')[6].replace('mB',''))
        if self.ch[3]:
            msmt.append(self.instr.query('READ:DEV:DB5.P1:TEMP:LOOP:FSET').split(':')[6])
        if self.oncooldown:
            if float(msmt[0])<10:
                self.setNV()
        self.last=msmt
        return msmt

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
        self.stop_T = QLineEdit(frame)
        self.ramp_T = QLineEdit(frame)
        
        self.start_Tramp = QPushButton('Ramp')
        self.start_Tramp.clicked.connect(lambda: self.setT(self.stop_T,self.ramp_T))
        
        self.start_cooldown = QPushButton('Cooldown')
        self.start_cooldown.clicked.connect(lambda: self.cooldown())
        
        layout = QGridLayout()
        for i, f in enumerate(self.T_field):
            layout.addWidget(label[f], i, 0, 1, 1) 
        layout.addWidget(self.stop_T, 0, 1, 1, 1)
        layout.addWidget(self.ramp_T, 1, 1, 1, 1)
        layout.addWidget(self.start_Tramp, 2, 0, 1, 2)
        layout.addWidget(self.start_cooldown, 3, 0, 1, 2)
        layout.setRowStretch(j+1, 2)
        frame.setLayout(layout)
    
    def setT(self, Tsetp, Tramp):
        self.instr.query('SET:DEV:DB8.T1:TEMP:LOOP:RENA:ON')
        self.instr.query('SET:DEV:MB1.T1:TEMP:LOOP:RENA:ON')
        
        ramp = float(Tramp.text())
        setpoint = float(Tsetp.text())
        
        # print(ramp, setpoint)
        
        self.instr.query(f'SET:DEV:DB8.T1:TEMP:LOOP:RSET:{ramp}')
        self.instr.query(f'SET:DEV:MB1.T1:TEMP:LOOP:RSET:{ramp}')
        
        
        if setpoint > 0.5:
            self.instr.query(f'SET:DEV:MB1.T1:TEMP:LOOP:TSET:{setpoint-0.5}')
        else:
            self.instr.query('SET:DEV:MB1.T1:TEMP:LOOP:TSET:0')
        self.instr.query(f'SET:DEV:DB8.T1:TEMP:LOOP:TSET:{setpoint}')
        
        self.instr.query('SET:DEV:MB1.T1:TEMP:LOOP:ENAB:ON')
        self.instr.query('SET:DEV:DB8.T1:TEMP:LOOP:ENAB:ON')
        
        #self.instr.write('SET:DEV:DB8.T1:TEMP:LOOP:RENA:OFF')
        #self.instr.write('SET:DEV:MB1.T1:TEMP:LOOP:RENA:OFF')
        # ramp = None
        # setpoint = None        
    
    def cooldown(self):
        self.oncooldown=True
        self.instr.query('SET:DEV:DB5.P1:PRES:LOOP:TSET:30')
        
        self.instr.query('SET:DEV:MB1.T1:TEMP:LOOP:RENA:OFF')
        self.instr.query('SET:DEV:MB1.T1:TEMP:LOOP:ENAB:ON')
        self.instr.query('SET:DEV:MB1.T1:TEMP:LOOP:HSET:0')
        self.instr.query('SET:DEV:MB1.T1:TEMP:LOOP:TSET:0')
        
        self.instr.query('SET:DEV:DB8.T1:TEMP:LOOP:RENA:OFF')
        self.instr.query('SET:DEV:DB8.T1:TEMP:LOOP:ENAB:ON')
        self.instr.query('SET:DEV:DB8.T1:TEMP:LOOP:HSET:0')
        self.instr.query('SET:DEV:DB8.T1:TEMP:LOOP:TSET:0')
        #self.off()
    
    def setNV(self):
        self.instr.query('SET:DEV:DB5.P1:PRES:LOOP:TSET:1.8')
    
    def get_temp(self):
        return self.instr.query('READ:DEV:DB6.T1:TEMP:SIG:TEMP').split(':')[6].replace('K','')
    
    def get_ramp(self):
        return self.instr.query('READ:DEV:DB6.T1:TEMP:LOOP:RSET').split(':')[6].replace('K/m','')
    
    def set_temp(self, tset, ramprate):
        tset=tset.text()
        ramprate=ramprate.text()
        self.instr.query('SET:DEV:DB6.T1:TEMP:LOOP:RENA:ON')
        self.instr.query('SET:DEV:DB6.T1:TEMP:LOOP:RSET:'+ramprate)
        self.instr.query('SET:DEV:DB6.T1:TEMP:LOOP:TSET:'+tset)
        self.instr.query('SET:DEV:DB6.T1:TEMP:LOOP:ENAB:ON')
        
    def cmdtest(self):
        return self.instr.query('READ:DEV:DB5.P1:PRES:LOOP:FSET')
        #return self.instr.query('READ:DEV:DB5.P1:PRES:SIG:PRES').split(':')[6].replace('mB','')


class MercuryiPS_teslatron:
    B_field=['Setpoint (T)', 'Ramp rate (T/mn)']
    ch_name=['Field','Temperature', 'PT1', 'PT2']
    def __init__(self, name, channels):
        self.name=name
        self.ch=ast.literal_eval(channels)
        self.instr=rm.open_resource('ASRL8::INSTR')
        self.instr.read_termination='\n'
        self.instr.write_termination='\n'
        
        print('Connected to Mercury iTC ('+name+')!')
    
    def measure(self):
        msmt=[]
        if self.ch[0]:
            msmt.append(self.instr.query('READ:DEV:GRPZ:PSU:SIG:FLD').split(':')[6].replace('T',''))
        if self.ch[1]:
            msmt.append(self.instr.query('READ:DEV:MB1.T1:TEMP:SIG:TEMP').split(':')[6].replace('K',''))
        if self.ch[2]:
            msmt.append(self.instr.query('READ:DEV:DB8.T1:TEMP:SIG:TEMP').split(':')[6].replace('K',''))
        if self.ch[3]:
            msmt.append(self.instr.query('READ:DEV:DB7.T1:TEMP:SIG:TEMP').split(':')[6].replace('K',''))
        self.last=msmt
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
        for i, f in enumerate(self.B_field):
            label[f] = QLabel(f)
            j+=1
        self.stop_B = QLineEdit(frame)
        self.ramp_B = QLineEdit(frame)
        
        self.start_Bramp = QPushButton('Ramp to setpoint')
        self.start_Bramp.clicked.connect(lambda: self.setB(self.stop_B,self.ramp_B))
        
        self.RTOZ_button = QPushButton('Ramp to 0 T')
        self.RTOZ_button.clicked.connect(lambda: self.Btozero())
        
        layout = QGridLayout()
        for i, f in enumerate(self.B_field):
            layout.addWidget(label[f], i, 0, 1, 1)
        layout.addWidget(self.stop_B, 0, 1, 1, 1)
        layout.addWidget(self.ramp_B, 1, 1, 1, 1)
        layout.addWidget(self.start_Bramp, 2, 0, 1, 2)
        layout.addWidget(self.RTOZ_button, 3, 0, 1, 2)
        layout.setRowStretch(j+2, 2)
        frame.setLayout(layout) 
        
    def setB(self, Bsetp, Bramp):
        ramp = float(Bramp.text())
        setpoint = float(Bsetp.text())
        
        htr_stat = self.instr.query('READ:DEV:GRPZ:PSU:SIG:SWHT').split(':')[6]
        if htr_stat == 'ON':
            self.instr.query(f'SET:DEV:GRPZ:PSU:SIG:RFST:{ramp}')
            self.instr.query(f'SET:DEV:GRPZ:PSU:SIG:FSET:{setpoint}')
            self.instr.query('SET:DEV:GRPZ:PSU:ACTN:RTOS')
        elif htr_stat == 'OFF':
            warning = QMessageBox()
            warning.setIcon(QMessageBox.Information)
            warning.setWindowTitle("Warning")
            warning.setText('Heater if OFF.\nCannot sweep to target field.')
            warning.setStandardButtons(QMessageBox.Ok)
            warning.exec()
        else:
            pass
        #htr_stat = self.instr.query('READ:DEV:GRPZ:PSU:SIG:SWHT').split(':')[6]
        ramp = None
        setpoint = None
        
    def Btozero(self):
        self.instr.write('SET:DEV:GRPZ:PSU:ACTN:RTOZ')