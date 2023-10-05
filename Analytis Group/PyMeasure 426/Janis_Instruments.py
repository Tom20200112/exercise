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
from lakeshore331_test import LakeShore331
from lakeshore_625_magnet_driver import Model_625

rm = pyvisa.ResourceManager()



class LakeShore335:
    T_field=['Setpoint (K)', 'Ramp rate (K/mn)']
    ch_name=['vap','probe']
    htr_range=['off', 'low', 'medium', 'high']
    ctrl_label=['Setpoint (K)', 'Ramp rate (K/mn)', 'Heater Range']
    #measure_every=100
    measure_every=0
    def __init__(self, name, channels):
        self.name=name
        self.ch=ast.literal_eval(channels)
        self.instr=LakeShore331('GPIB0::2::INSTR')
        self.msmtcount=0
        self.ta=self.instr.temperature_A
        self.tb=self.instr.temperature_B
        self.measure()
        print('Connected to Lakeshore 335 ('+name+')!')
    
    def measure(self):
        if self.msmtcount==self.measure_every:
            msmt=[]
            if self.ch[0]:
                self.ta=self.instr.temperature_A
                msmt.append(self.ta)
            if self.ch[1]:
                self.tb=self.instr.temperature_B
                msmt.append(self.tb)
            self.msmtcount=0
        else:
            self.msmtcount+=1
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
        label={}
        j=0
        for i, f in enumerate(self.ctrl_label):
            label[f] = QLabel(f)
            j+=1
        self.stop_T = QLineEdit(frame)
        self.ramp_T = QLineEdit(frame)
        self.heatrange= QComboBox()
        self.heatrange.addItems(self.htr_range)
        currentsetpoint=self.instr.setpoint_2
        self.stop_T.setText(str(currentsetpoint))
        currentrate=self.instr.ramp_rate_2[1]
        self.ramp_T.setText(str(currentrate))
        self.start_Tramp = QPushButton('Ramp')
        self.start_Tramp.clicked.connect(lambda: self.set_temp(self.stop_T,self.ramp_T, self.heatrange))
        htrrange=self.instr.heater_range_2
        idx=self.heatrange.findText(htrrange)
        if idx>=0:
            self.heatrange.setCurrentIndex(idx)

        layout = QGridLayout()
        for i, f in enumerate(self.ctrl_label):
            layout.addWidget(label[f], i, 0, 1, 1) 
        layout.addWidget(self.stop_T, 0, 1, 1, 1)
        layout.addWidget(self.ramp_T, 1, 1, 1, 1)
        layout.addWidget(self.heatrange, 2, 1, 1, 1)
        layout.addWidget(self.start_Tramp, 3, 0, 1, 2)
        layout.setRowStretch(j+1, 2)
        frame.setLayout(layout) 
        
#    def get_temp(self):
#        return self.instr.query('READ:DEV:DB6.T1:TEMP:SIG:TEMP').split(':')[6].replace('K','')
#    
#    def get_ramp(self):
#        return self.instr.query('READ:DEV:DB6.T1:TEMP:LOOP:RSET').split(':')[6].replace('K/m','')
#    
    def set_temp(self, Tsetp, Tramp, htrrange):
        ramp = float(Tramp.text())
        setpoint = float(Tsetp.text())
        rng=htrrange.currentText()
        self.instr.heater_range_1= rng 
        self.instr.heater_range_2=rng
        self.instr.ramp_rate_1=ramp
        self.instr.ramp_rate_2=ramp
        self.instr.setpoint_1=setpoint
        self.instr.setpoint_2=setpoint
    
    def off(self):
        self.instr.query('SET:DEV:DB6.T1:TEMP:LOOP:RENA:OFF')
        self.instr.query('SET:DEV:DB6.T1:TEMP:LOOP:ENAB:ON')
        self.instr.query('SET:DEV:DB6.T1:TEMP:LOOP:HSET:0')
        self.instr.query('SET:DEV:DB6.T1:TEMP:LOOP:TSET:1.5')

class LakeShore625:
    BLoop_field = ['Setpoint (T)', 'Ramp rate list (A/s)']
    def __init__(self, name):
        self.name=name
        self.ch=[True]
        self.instr=Model_625('B','GPIB0::12::INSTR')
        self.measure()
        
        print('Connected to Lakeshore 625 ('+name+')!')
    
    def measure(self):
        b=self.instr.field_value()
        msmt=[b]
        self.last=msmt
        return msmt
    
    def last_msmt(self):
        return self.last

    def header(self):
        hdr=[self.name]
        return hdr

    def control_GUI(self, frame):
        label={}
        j=0
        for i, f in enumerate(self.BLoop_field):
            label[f] = QLabel(f)
            j+=1
        self.stop_B = QLineEdit(frame)
        self.ramp_B = QLineEdit(frame)
        
        self.start_Bramp = QPushButton('Ramp to setpoint')
        self.start_Bramp.clicked.connect(lambda: self.setB(self.stop_B,self.ramp_B))
        current_setpoint=self.getBTarget()
        current_rate=self.getBRate()
        self.stop_B.setText(str(current_setpoint))
        self.ramp_B.setText(str(current_rate))
        self.RTOZ_button = QPushButton('Ramp to 0 T')
        self.RTOZ_button.clicked.connect(lambda: self.Btozero())
        
        self.switch_status=QLineEdit()
        self.checkHtrStatus(self.switch_status)
        self.check_switch=QPushButton('Check Switch')
        self.check_switch.clicked.connect(lambda: self.checkHtrStatus(self.switch_status))
#        self.switch_on=QPushButton('Switch On')
#        self.switch_on.clicked.connect(lambda: B_cmd.htrOn())
#        self.switch_off=QPushButton('Switch Off')
#        self.switch_off.clicked.connect(lambda: B_cmd.htrOff())
        
        layout = QGridLayout()
        for i, f in enumerate(self.BLoop_field):
            layout.addWidget(label[f], i, 0, 1, 1)
        layout.addWidget(self.stop_B, 0, 1, 1, 1)
        layout.addWidget(self.ramp_B, 1, 1, 1, 1)
        layout.addWidget(self.start_Bramp, 2, 0, 1, 2)
        layout.addWidget(self.RTOZ_button, 3, 0, 1, 2)
        layout.addWidget(self.switch_status,4,0,1,1)
        layout.addWidget(self.check_switch,4,1,1,1)
#        layout.addWidget(self.switch_on,4,1,1,1)
#        layout.addWidget(self.switch_off,5,1,1,1)
        layout.setRowStretch(j+2, 2)
        frame.setLayout(layout) 
    def setB(self, Bsetp, Bramp):
        ramp = float(Bramp.text())
        setpoint = float(Bsetp.text())
        if ramp>.035:
            ramp=.035
        if setpoint>9:
            setpoint=9
        elif setpoint<-9:
            setpoint=-9
        
        htr_stat = self.instr.pers_htr_state()
        if htr_stat == 1:
            self.instr.current_ramp_rate(ramp)
            self.instr.field_ramp_target(setpoint)
        elif htr_stat == 0:
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
    def htrOn(self):
        self.instr.htr_on()
    def htrOff(self):
        self.instr.htr_off()
    def getBTarget(self):
        return self.instr.current_ramp_target()
    def getBRate(self):
        return self.instr.current_ramp_rate()
    
    def checkHtrStatus(self, box):
        htr_stat = self.instr.pers_htr_state()
        if htr_stat == 1:
            box.setText('On')    
            box.setStyleSheet("background-color: #008B45;")
        elif htr_stat == 0:
            box.setText('Off')
            box.setStyleSheet("background-color: #CD4F39;")
        else:
            pass
    
    def Btozero(self):
        self.instr.field_ramp_target(0)
#    def get_temp(self):
#        return self.instr.query('READ:DEV:DB6.T1:TEMP:SIG:TEMP').split(':')[6].replace('K','')
#    
#    def get_ramp(self):
#        return self.instr.query('READ:DEV:DB6.T1:TEMP:LOOP:RSET').split(':')[6].replace('K/m','')
#
'''
class LM510:
    measure_every=1
    def __init__(self, name, channels):
        self.name=name
        self.ch=[True,True]
        self.instr=rm.open_resource('ASRL3::INSTR')
        self.msmtcount=0
        self.ta=self.instr.
        print('Connected to Lakeshore 335 ('+name+')!')
    
    def measure(self):
        if self.msmtcount==self.measure_every:
            self.ta=self.instr.temperature_A
            self.tb=self.instr.temperature_B
            self.msmtcount=0
        else:
            self.msmtcount+=1
        msmt=[str(self.tb),str(self.ta)]
        return msmt

    def header(self):
        hdr=[]
        for ind in range(len(self.ch)):
            if self.ch[ind]:
                hdr.append(self.name+'_'+self.ch_name[ind])
        return hdr
    def control_GUI(self, frame):
        label={}
        j=0
        for i, f in enumerate(self.ctrl_label):
            label[f] = QLabel(f)
            j+=1
        self.stop_T = QLineEdit(frame)
        self.ramp_T = QLineEdit(frame)
        self.heatrange= QComboBox()
        self.heatrange.addItems(self.htr_range)
        currentsetpoint=self.instr.setpoint_2
        self.stop_T.setText(str(currentsetpoint))
        currentrate=self.instr.ramp_rate_2[1]
        self.ramp_T.setText(str(currentrate))
        self.start_Tramp = QPushButton('Ramp')
        self.start_Tramp.clicked.connect(lambda: self.set_temp(self.stop_T,self.ramp_T, self.heatrange))
        htrrange=self.instr.heater_range_2
        idx=self.heatrange.findText(htrrange)
        if idx>=0:
            self.heatrange.setCurrentIndex(idx)

        layout = QGridLayout()
        for i, f in enumerate(self.ctrl_label):
            layout.addWidget(label[f], i, 0, 1, 1) 
        layout.addWidget(self.stop_T, 0, 1, 1, 1)
        layout.addWidget(self.ramp_T, 1, 1, 1, 1)
        layout.addWidget(self.heatrange, 2, 1, 1, 1)
        layout.addWidget(self.start_Tramp, 3, 0, 1, 2)
        layout.setRowStretch(j+1, 2)
        frame.setLayout(layout) 
        
#    def get_temp(self):
#        return self.instr.query('READ:DEV:DB6.T1:TEMP:SIG:TEMP').split(':')[6].replace('K','')
#    
#    def get_ramp(self):
#        return self.instr.query('READ:DEV:DB6.T1:TEMP:LOOP:RSET').split(':')[6].replace('K/m','')
#    
    def set_temp(self, Tsetp, Tramp, htrrange):
        ramp = float(Tramp.text())
        setpoint = float(Tsetp.text())
        rng=htrrange.currentText()
        self.instr.heater_range_1= rng 
        self.instr.heater_range_2=rng
        self.instr.ramp_rate_1=ramp
        self.instr.ramp_rate_2=ramp
        self.instr.setpoint_1=setpoint
        self.instr.setpoint_2=setpoint
    
    def off(self):
        self.instr.query('SET:DEV:DB6.T1:TEMP:LOOP:RENA:OFF')
        self.instr.query('SET:DEV:DB6.T1:TEMP:LOOP:ENAB:ON')
        self.instr.query('SET:DEV:DB6.T1:TEMP:LOOP:HSET:0')
        self.instr.query('SET:DEV:DB6.T1:TEMP:LOOP:TSET:1.5')
        
'''