# -*- coding: utf-8 -*-
"""
Created on Mon Feb  1 13:59:25 2021

@author: Jordan
"""
import ast
import numpy as np
from qcodes.instrument_drivers.stanford_research.SR830 import SR830
from qcodes.instrument_drivers.stanford_research.SR860 import SR860
from qcodes.instrument_drivers.yokogawa.GS200 import GS200
from qcodes.instrument_drivers.tektronix.Keithley_2400 import Keithley_2400
from qcodes.instrument_drivers.tektronix.Keithley_2450 import Keithley2450
import visa

rm = visa.ResourceManager()
# from lakeshore331_test import LakeShore331
# from lakeshore_625_magnet_driver import Model_625

from PyQt5.QtWidgets import (QComboBox, 
        QGridLayout, QLabel, QLineEdit,
        QPushButton, QCheckBox,QFileDialog)
from PyQt5.QtGui import QDoubleValidator
from PyQt5.QtTest import QTest

class SR830_inst:
    ch_name=['X','Y','R','P','AUX_IN_1','AUX_IN_2','AUX_IN_3','AUX_IN_4']
    fields = ['Internal Reference','ch1 display','ch2 display','Frequency','Amplitude','Time constant','Increase sensitivity','Decrease sensitivity']
    ch1Display = ['X', 'R']
    ch2Display = ['Y', 'Phase']
    lockin_tc_val = ['10e-6','30e-6','100e-6','300e-6','1e-3','3e-3','10e-3','30e-3','100e-3','300e-3','1','3','10','30','100','300','1e3','3e3','10e3','30e3']
    def __init__(self, name, address, channels):
        self.name=name
        self.gpib=address
        self.ch=ast.literal_eval(channels)
        self.instr=SR830(name, address)
        self.measure()
        
        print('Connected to SR830 ('+name+')!')
        
    def measure(self):        
        val=[]
        if self.ch[0]:
            val.append('x')
        if self.ch[1]:
            val.append('y')
        if self.ch[2]:
            val.append('r')
        if self.ch[3]:
            val.append('p')
        if self.ch[4]:
            val.append('aux1')
        if self.ch[5]:
            val.append('aux2')
        if self.ch[6]:
            val.append('aux3')
        if self.ch[7]:
            val.append('aux4')
        msmt=list(self.instr.snap(*val))
        #msmt=[f"{msmt:.5g}" for msmt in msmt]
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
    def control_GUI(self, frame): #Create the apearence for lockins (830 and 860)
        j=0
        label={}
        for i, f in enumerate(self.fields):
            if i in range(6):
                label[f] = QLabel(f)
            else:
                label[f] = None
            j+=1
        reference_entry = QCheckBox()
        ch1display_entry = QComboBox()
        ch1display_entry.addItems(self.ch1Display)
        ch2display_entry = QComboBox()
        ch2display_entry.addItems(self.ch2Display)
        freq_entry = QLineEdit(frame)
        freq_entry.setDisabled(True)
        amp_entry = QLineEdit(frame)
        amp_entry.setDisabled(True)
        reference_entry.toggled.connect(freq_entry.setEnabled)
        reference_entry.toggled.connect(amp_entry.setEnabled)
        tc_entry = QComboBox()
        tc_entry.addItems(self.lockin_tc_val)
        upsens_button = QPushButton('Increase sensitivity')
        upsens_button.clicked.connect(lambda: self.inc_sens_lockin())
        downsens_button = QPushButton('Decrease sensitivity')
        downsens_button.clicked.connect(lambda: self.dec_sens_lockin())
        set_button = QPushButton('Set lockin parameters')
        set_button.clicked.connect(lambda: self.set_lockin_param(reference_entry,ch1display_entry,ch2display_entry,freq_entry,amp_entry,tc_entry))
        layout = QGridLayout()
        for i, f in enumerate(self.fields):
            if i in range(6):
                layout.addWidget(label[f], i, 0, 1, 1)
        layout.addWidget(reference_entry, 0, 1, 1, 1)
        layout.addWidget(ch1display_entry, 1, 1, 1, 1)
        layout.addWidget(ch2display_entry, 2, 1, 1, 1)
        layout.addWidget(freq_entry, 3, 1, 1, 1)
        layout.addWidget(amp_entry, 4, 1, 1, 1)
        layout.addWidget(tc_entry, 5, 1, 1, 1)
        layout.addWidget(upsens_button, 6, 0, 1, 1)
        layout.addWidget(downsens_button, 6, 1, 1, 1)
        layout.addWidget(set_button, 7, 0, 1, 2)
        layout.setRowStretch(j+2, 2)
        frame.setLayout(layout)
    def set_lockin_param(self,ref,display1,display2,freq,amp,tc):
        if self.instr.amplitude() < 0.004:
            amp_start = 0.004
        elif self.instr.amplitude() > 5:
            amp_start = 5
        else:
            amp_start = self.instr.amplitude()
        if ref.isChecked()==True:
            self.instr.reference_source('internal')
        else:
            self.instr.reference_source('external')
        self.instr.ch1_display(display1.currentText())
        self.instr.ch2_display(display2.currentText())
        if len(freq.text()) == 0:
            pass
        else:
            self.instr.frequency(float(freq.text()))
        if len(amp.text()) == 0:
            pass
        else:
            if float(amp.text())>5:
                amplitude = 5
            elif float(amp.text())<0.004:
                amplitude = 0.004
            else:
                amplitude = float(amp.text())
            L = np.linspace(amp_start,amplitude, 21)
            for amp in L:
                self.instr.amplitude(amp)
                QTest.qWait(0.1*1000)
        print('Setting time constant:' + tc.currentText())
        self.instr.time_constant(float(tc.currentText()))
                
    def inc_sens_lockin(self):
        print('Sensitivity set to' + str(self.instr.sensitivity()))
        self.instr.increment_sensitivity()
        
    def dec_sens_lockin(self):
        print('Sensitivity set to' + str(self.instr.sensitivity()))
        self.instr.decrement_sensitivity()
    
    def set_freq(self, fr):
        self.instr.frequency(float(fr))

class SR860_inst:
    ch_name=['X','Y','R','P']
    def __init__(self, name, address, channels):
        self.name=name
        self.gpib=address
        self.ch=ast.literal_eval(channels)
        self.instr=SR860(name, address)
        self.measure()             
        
        print('Connected to SR860 ('+name+')!')
        
    def measure(self):
        msmt=[]
#        pt = self.instr.get_values('X','Y','R','P')
        if self.ch==[True,True,False,False]:
            val=self.instr.get_values('X','Y')
            msmt=[sval[0],val[1]]
        elif self.ch==[False,False,True,True]:
            val=self.instr.get_values('R','P')
            msmt=[val[0],val[1]]
        elif self.ch==[True,False,False,True]:
            val=self.instr.get_values('X','P')
            msmt=[val[0],val[1]]
        else:
            if self.ch[0]:
                msmt.append(self.instr.X())
            if self.ch[1]:
                msmt.append(self.instr.Y())
            if self.ch[2]:
                msmt.append(self.instr.R())
            if self.ch[3]:
                msmt.append(self.instr.P())
        #msmt=[f"{msmt:.5g}" for msmt in msmt]
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

class K2400_inst:
    ch_name=['V','I']
    KGUI_field = ['Disable','Output', 'Mode', 'Compliance', 'Range', 'Sweep To']
    keithley_range = ['--VOLTAGE MODE--','200e-3','2','20','200','--CURRENT MODE--','1e-6','10e-6','100e-6','1e-3','10e-3','100e-3','1']
    keithley_comp = ['--VOLTAGE MODE--','1.05e-6','10.5e-6', '105e-6','1.05e-3', '10.5e-3', '105e-3', '1.05','--CURRENT MODE--','210e-3','2.1','21','210']
    K_field = ['Disable','Output', 'Mode', 'Compliance', 'Range', 'Sweep To']
    
    def __init__(self, name, address, channels):
        self.name=name
        self.gpib=address
        self.ch=ast.literal_eval(channels)
        self.instr=Keithley_2400(name, address)        
        self.param_K1 = [self.instr.output(),str(self.instr.mode())]
        print('Connected to K2400 ('+name+')!')
        self.val=self.now_at()
        
    def measure(self):
        msmt=[]
        if self.ch[0]:
            # msmt.append(str(self.instr.volt()))
            msmt.append(self.val)
        if self.ch[1]:
            msmt.append(self.instr.curr())
        #msmt=[f"{msmt:.5g}" for msmt in msmt]
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
    
    def set_val(self, val, mode=0): #Make compatible with current
        if mode==0:
            if self.instr.mode()=='CURR':
                self.instr.curr(val)
            else:
                self.instr.volt(val)
                
        elif mode==1:
            self.instr.volt(val)
        elif mode==2:
            self.instr.curr(val)
        self.val=val
    def now_at(self, mode=0):
        if mode==0:
            if self.instr.mode()=='CURR':
                return self.instr.curr()
            else:
                return self.instr.volt()
        elif mode==1:
            return self.instr.volt()
        elif mode==2:
            return self.instr.curr()
    
    # def sweep_val(self,val, step, rate):
    #     if self.instr.mode()=='CURR':
    #         m=2
    #     else:
    #         m=1
    #     v0=self.now_at(mode=m)
        
    #     num=int(abs((val-v0)/step)+1)
    #     sweep=np.linspace(v0,val,num)
    #     for v in sweep:
    #         self.set_val(v, mode=m)
    #     self.set_val(val, mode=m)
    def sweep_val(self,val, pts, wait):
        if self.instr.mode()=='CURR':
            m=2
        else:
            m=1
        v0=self.now_at(mode=m)
        
        sweep=np.linspace(v0,val,pts)
        for v in sweep:
            self.set_val(v, mode=m)
            QTest.qWait(int(wait*1000))
        self.set_val(val, mode=m)
    
    def control_GUI(self, frame):#Apearence for Keithley control panel
        j=0
        label={}
        for i, f in enumerate(self.KGUI_field):
            label[f] = QLabel(f)
            j+=1
        protec_check = QCheckBox()    
        protec_check.setChecked(True)
        protec_button = None
        
        out_entry = None
        out_button = QPushButton('ON' if self.param_K1[0] == True else 'OFF')
        out_button.setEnabled(False)
        protec_check.toggled.connect(out_button.setDisabled)
        out_button.setStyleSheet("background-color: #008B45;" if self.param_K1[0] == True else "background-color: #CD4F39;")
        out_button.clicked.connect(lambda: self.set_output(out_button))
            
        mode_entry = None
        mode_button = QPushButton('VOLT' if self.param_K1[1] in ['VOLT','voltage'] else 'CURR')
        mode_button.setEnabled(False)
        protec_check.toggled.connect(mode_button.setDisabled)
        mode_button.clicked.connect(lambda: self.set_mode(mode_button,out_button))

        comp_entry = QComboBox()
        comp_entry.addItems(self.keithley_comp)
        # if instr.get_idn()['model'] == '2400':
        #     comp_entry.addItems(self.keithley_comp)
        # else:
        #     comp_entry.addItems(self.keithley2450_comp)
        comp_button = QPushButton('Set')
        comp_button.clicked.connect(lambda: self.set_compliance(comp_entry))
        
        range_entry = QComboBox()
        range_entry.addItems(self.keithley_range)
        range_button = QPushButton('Set')
        range_button.clicked.connect(lambda: self.set_range(range_entry))
        
        sweep_entry = QLineEdit(frame)
        sweepTime_entry = QLineEdit(frame)
        sweep_button = QPushButton('Set')
        sweep_button.clicked.connect(lambda: self.sweep_to(sweepTime_entry,sweep_entry, sweep_button))
            
        layout = QGridLayout()
        for i, f in enumerate(self.K_field):
            layout.addWidget(label[f], i, 0, 1, 1)
        layout.addWidget(protec_check, 0, 1, 1, 2)
        layout.addWidget(protec_button, 0, 3, 1, 1)
        layout.addWidget(out_entry, 1, 1, 1, 2)
        layout.addWidget(out_button, 1, 3, 1, 1)
        layout.addWidget(mode_entry, 2, 1, 1, 2)
        layout.addWidget(mode_button, 2, 3, 1, 1)
        layout.addWidget(comp_entry, 3, 1, 1, 2)
        layout.addWidget(comp_button, 3, 3, 1, 1)
        layout.addWidget(range_entry, 4, 1, 1, 2)
        layout.addWidget(range_button, 4, 3, 1, 1)
        layout.addWidget(QLabel('value'), 5, 1, 1, 1)
        layout.addWidget(QLabel('number of points'), 5, 2, 1, 1)
        layout.addWidget(sweep_entry, 6, 1, 1, 1)
        layout.addWidget(sweepTime_entry, 6, 2, 1, 1)
        layout.addWidget(sweep_button, 6, 3, 1, 1)

        layout.setRowStretch(j+2, 3)
        frame.setLayout(layout)
    def set_output(self,button):
        if self.instr.output()==True:
            self.instr.output(False)
            button.setText('OFF')
            button.setStyleSheet("background-color: #CD4F39;")
        else:
            self.instr.output(True)
            button.setText('ON')
            button.setStyleSheet("background-color: #008B45;")
    
    def set_mode(self,val,out):
        if self.instr.mode()=='CURR':
            val.setText('VOLT')
            self.instr.mode('VOLT')
            out.setText('OFF')
            out.setStyleSheet("background-color: #CD4F39;")
            
        else:
            val.setText('CURR')
            self.instr.mode('CURR')
            out.setText('OFF')
            out.setStyleSheet("background-color: #CD4F39;")
    # def set_read():
    #     return
    
    def set_compliance(self, val):
        value = float(val.currentText())
        if self.instr.mode() == 'CURR':
            self.instr.compliancev(value)
        else:
            self.instr.compliancei(value)
    
    def set_range(self, val):
        value = float(val.currentText())
        if self.instr.mode() == 'CURR':
            self.instr.rangei(value)
        else:
            self.instr.rangev(value)
    
    def sweep_to(self,ste,val,but):
        value = float(val.text())
        step = int(ste.text())
        but.setStyleSheet("background-color: #008B45;")
        self.sweep_val(value, step, .1)
        but.setStyleSheet("background-color: #464646;")

class K2450_inst:
    ch_name=['V','I']
    KGUI_field = ['Disable','Output', 'Mode', 'Compliance', 'Range', 'Sweep To']
    keithley2450_range = ['--VOLTAGE MODE--','20e-3','200e-3','2','20','200','--CURRENT MODE--','10e-9','100e-9','1e-6','10e-6','100e-6','1e-3','10e-3','100e-3','1']
    keithley2450_comp = ['--READ CURRENT MODE--','10e-9','100e-9','1e-6','10e-6', '100e-6','1e-3', '10e-3', '100e-3', '1','--READ VOLTAGE MODE--','210e-3','2.1','21','210']

    def __init__(self, name, address, channels):
        self.name=name
        self.gpib=address
        self.ch=ast.literal_eval(channels)
        self.instr=Keithley2450(name, address)        
        self.param_K1 = [self.instr.output_enabled(),self.instr.source.function(),self.instr.sense.function()]
        print('Connected to K2450 ('+name+')!')
        
    def measure(self):
        msmt=[]
        if self.ch[0]:
            msmt.append(self.instr.volt())
        if self.ch[1]:
            msmt.append(self.instr.curr())
        return msmt

    def header(self):
        hdr=[]
        for ind in range(len(self.ch)):
            if self.ch[ind]:
                hdr.append(self.name+'_'+self.ch_name[ind])
        return hdr
    
    def set_val(self, val, mode=0): #Make compatible with current
        if mode==0:
            if self.instr.source.function()=='current':
                self.instr.currrent(val)
            else:
                self.instr.voltage(val)
                
        elif mode==1:
            self.instr.voltage(val)
        elif mode==2:
            self.instr.current(val)
    def now_at(self, mode=0):
        if mode==0:
            if self.instr.source.function()=='current':
                return self.instr.currrent()
            else:
                return self.instr.voltage()
        elif mode==1:
            return self.instr.voltage()
        elif mode==2:
            return self.instr.current()
    
    def sweep_val(self,val, step, rate):
        if self.instr.source.function()=='current':
            m=2
        else:
            m=1
        v0=self.now_at(mode=m)
        
        num=int(abs((val-v0)/step)+1)
        sweep=np.linspace(v0,val,num)
        for v in sweep:
            self.set_val(v, mode=m)
        self.set_val(val, mode=m)
    
    def control_GUI(self, frame):#Apearence for Keithley control panel
    
        j=0
        label={}
        for i, f in enumerate(self.KGUI_field):
            label[f] = QLabel(f)
            j+=1
        protec_check = QCheckBox()    
        protec_check.setChecked(True)
        protec_button = None
        
        out_entry = None
        out_button = QPushButton('ON' if self.param_K1[0] == True else 'OFF')
        out_button.setEnabled(False)
        protec_check.toggled.connect(out_button.setDisabled)
        out_button.setStyleSheet("background-color: #008B45;" if self.param_K1[0] == True else "background-color: #CD4F39;")
        out_button.clicked.connect(lambda: self.set_output(out_button))
            
        mode_entry = None
        mode_button = QPushButton('VOLT' if self.param_K1[1] in ['VOLT','voltage'] else 'CURR')
        mode_button.setEnabled(False)
        protec_check.toggled.connect(mode_button.setDisabled)
        mode_button.clicked.connect(lambda: self.set_mode(mode_button,out_button))
        

        read_entry = None
        read_button = QPushButton('VOLT' if self.param_K1[2] in ['VOLT','voltage'] else 'CURR')
        read_button.setEnabled(False)
        protec_check.toggled.connect(read_button.setDisabled)
        read_button.clicked.connect(lambda: self.set_read(read_button))

        comp_entry = QComboBox()
        comp_entry.addItems(self.keithley_comp)
        # if instr.get_idn()['model'] == '2400':
        #     comp_entry.addItems(self.keithley_comp)
        # else:
        #     comp_entry.addItems(self.keithley2450_comp)
        comp_button = QPushButton('Set')
        comp_button.clicked.connect(lambda: self.set_compliance(comp_entry))
        
        range_entry = QComboBox()
        range_entry.addItems(self.keithley_range)
        range_button = QPushButton('Set')
        range_button.clicked.connect(lambda: self.set_range(range_entry))
        
        sweep_entry = QLineEdit(frame)
        sweepTime_entry = QLineEdit(frame)
        sweep_button = QPushButton('Set')
        sweep_button.clicked.connect(lambda: self.sweep_to(sweepTime_entry,sweep_entry))
            
        layout = QGridLayout()
        for i, f in enumerate(self.K_field):
            layout.addWidget(label[f], i, 0, 1, 1)
        layout.addWidget(protec_check, 0, 1, 1, 2)
        layout.addWidget(protec_button, 0, 3, 1, 1)
        layout.addWidget(out_entry, 1, 1, 1, 2)
        layout.addWidget(out_button, 1, 3, 1, 1)
        layout.addWidget(mode_entry, 2, 1, 1, 2)
        layout.addWidget(mode_button, 2, 3, 1, 1)
        layout.addWidget(read_entry, 3, 1, 1, 2)
        layout.addWidget(read_button, 3, 3, 1, 1)
        layout.addWidget(comp_entry, 4, 1, 1, 2)
        layout.addWidget(comp_button, 4, 3, 1, 1)
        layout.addWidget(range_entry, 5, 1, 1, 2)
        layout.addWidget(range_button, 5, 3, 1, 1)
        layout.addWidget(QLabel('value'), 6, 1, 1, 1)
        layout.addWidget(QLabel('number of points'), 6, 2, 1, 1)
        layout.addWidget(sweep_entry, 7, 1, 1, 1)
        layout.addWidget(sweepTime_entry, 7, 2, 1, 1)
        layout.addWidget(sweep_button, 7, 3, 1, 1)

        layout.setRowStretch(j+2, 3)
        frame.setLayout(layout)
    def set_output(self,button):
        if self.instr.output_enabled()==True:
            self.instr.output_enabled(False)
            button.setText('OFF')
            button.setStyleSheet("background-color: #CD4F39;")
        else:
            self.instr.output_enabled(True)
            button.setText('ON')
            button.setStyleSheet("background-color: #008B45;")
    
    def set_mode(self,val,out):
        if self.instr.source.function() == 'current':
            val.setText('VOLT')
            self.instr.source.function('voltage')
            out.setText('OFF')
            out.setStyleSheet("background-color: #CD4F39;")
        else:
            val.setText('CURR')
            self.instr.source.function('current')
            out.setText('OFF')
            out.setStyleSheet("background-color: #CD4F39;")
    
    def set_compliance(self, val):
        value = float(val.currentText())
        if self.instr.mode() == 'CURR':
            self.instr.compliancev(value)
        else:
            self.instr.compliancei(value)
    
    def set_range(self, val):
        value = float(val.currentText())
        if self.instr.mode() == 'CURR':
            self.instr.rangei(value)
        else:
            self.instr.rangev(value)
    
    def sweep_to(self,ste,val):
        value = float(val.text())
        step = float(ste.text())
        self.sweep_val(value, step, .1)
    
    def set_read(self,val):
        if self.instr.sense.function() == 'current':
            self.instr.sense.function('voltage')
            val.setText('VOLT')
        elif self.instr.sense.function() == 'voltage':
            self.instr.sense.function('current')
            val.setText('CURR')

class yoko_inst:
    ch_name=['V','I']
    Y_field = ['Disable','Output', 'Mode', 'Range', 'Sweep To']
    yoko_range=['--VOLTAGE MODE--','10e-3','100e-3','1', '10','30','--CURRENT MODE','1e-3','10e-3','100e-3','200e-3']
    def __init__(self, name, address, channels):
        self.name=name
        self.gpib=address
        self.ch=ast.literal_eval(channels)
        self.instr=GS200(name, address)      
        self.param = [str(self.instr.output()),str(self.instr.source_mode())]
        print(self.instr.voltage_range)
        # if self.param[1]=='VOLT':
        #     self.instr.voltage_range(self.instr.voltage_range)
        # elif self.param[1]=='CURR':
        #     self.instr.current_range(self.instr.current_range)
        print('Connected to Yoko ('+name+')!')
        
    def measure(self):
        msmt=[]
        if self.ch[0]:
            msmt.append(self.instr.voltage())
        if self.ch[1]:
            msmt.append(self.instr.curreent())
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
    
    def set_output(self,val):
        if self.instr.output()=='on':
            self.instr.off()
            val.setText('OFF')
            val.setStyleSheet("background-color: #CD4F39;")
        else:
            self.instr.on()
            val.setText('ON')
            val.setStyleSheet("background-color: #008B45;")
    
    def set_val(self, val, mode=0): #Make compatible with current
        if mode==0:
            if self.instr.source_mode()=='CURR':
                self.instr.currrent(val)
            else:
                self.instr.voltage(val)
                
        elif mode==1:
            self.instr.voltage(val)
        elif mode==2:
            self.instr.current(val)
    def now_at(self, mode=0):
        if mode==0:
            if self.instr.source_mode()=='CURR':
                return self.instr.currrent()
            else:
                return self.instr.voltage()
        elif mode==1:
            return self.instr.voltage()
        elif mode==2:
            return self.instr.current()
    
    def sweep_val(self,val, pts, wait):
        
        if self.instr.source_mode()=='CURR':
            m=2
        else:
            m=1
        v0=self.now_at(mode=m)
        
        sweep=np.linspace(v0,val,pts)
        for v in sweep:
            self.set_val(v, mode=m)
            QTest.qWait(int(wait*1000))
        self.set_val(val, mode=m)

    
    def control_GUI(self, frame):#Apearence for Keithley control panel
        label={}
        j=0
        for i, f in enumerate(self.Y_field):
            label[f] = QLabel(f)
            j+=1
        protec_check = QCheckBox()    
        protec_check.setChecked(True)
        protec_button = None    
        
        out_entry = None
        out_button = QPushButton('ON' if self.param[0] == 'on' else 'OFF')
        out_button.setEnabled(False)
        protec_check.toggled.connect(out_button.setDisabled)
        out_button.setStyleSheet("background-color: #008B45;" if self.param[0] == 'on' else "background-color: #CD4F39;")
        out_button.clicked.connect(lambda: self.set_output(out_button))

        mode_entry = None
        mode_button = QPushButton('VOLT' if self.param[1]== 'VOLT' else 'CURR')
        mode_button.setEnabled(False)
        protec_check.toggled.connect(mode_button.setDisabled)
        mode_button.clicked.connect(lambda: self.set_mode(mode_button,out_button))

        range_entry = QComboBox()
        range_entry.addItems(self.yoko_range)
        range_button = QPushButton('Set')
        range_button.clicked.connect(lambda: self.set_range(range_entry))

        sweep_entry = QLineEdit(frame)
        sweepTime_entry = QLineEdit(frame)
        sweep_button = QPushButton('Set')
        sweep_button.clicked.connect(lambda: self.sweep_to(sweepTime_entry,sweep_entry,sweep_button))
        layout = QGridLayout()
        for i, f in enumerate(self.Y_field):
            layout.addWidget(label[f], i, 0, 1, 1)
        layout.addWidget(protec_check, 0, 1, 1, 2)
        layout.addWidget(protec_button, 0, 3, 1, 1)
        layout.addWidget(out_entry, 1, 1, 1, 2)
        layout.addWidget(out_button, 1, 3, 1, 1)
        layout.addWidget(mode_entry, 2, 1, 1, 2)
        layout.addWidget(mode_button, 2, 3, 1, 1)
        layout.addWidget(range_entry, 3, 1, 1, 2)
        layout.addWidget(range_button, 3, 3, 1, 1)
        layout.addWidget(QLabel('value'), 4, 1, 1, 1)
        layout.addWidget(QLabel('number of points'), 4, 2, 1, 1)
        layout.addWidget(sweep_entry, 5, 1, 1, 1)
        layout.addWidget(sweepTime_entry, 5, 2, 1, 1)
        layout.addWidget(sweep_button, 5, 3, 1, 1)
        
        layout.setRowStretch(j+2, 3)
        frame.setLayout(layout)
    
    def set_mode(self,val,out):
        if self.instr.source_mode()=='CURR':
            self.instr.source_mode('VOLT')
            val.setText('VOLT')
        else:
            self.instr.source_mode('CURR')
            val.setText('CURR')
    
    def set_compliance(self, val):
        value = float(val.currentText())
        if self.instr.mode() == 'CURR':
            self.instr.compliancev(value)
        else:
            self.instr.compliancei(value)
    
    def set_range(self, val):
        value = float(val.currentText())
        if self.instr.source_mode()=='CURR':
            self.instr.current_range(value)
        else:
            self.instr.voltage_range(value)
    
    def sweep_to(self,ste,val,but):
        value = float(val.text())
        step = float(ste.text())
        but.setStyleSheet("background-color: #008B45;")
        self.sweep_val(value, int(step), .1)
        but.setStyleSheet("background-color: #464646;")
        
        
    
    def set_read(self,val):
        if self.instr.sense.function() == 'current':
            self.instr.sense.function('voltage')
            val.setText('VOLT')
        elif self.instr.sense.function() == 'voltage':
            self.instr.sense.function('current')
            val.setText('CURR')

class SIM_inst:
    ch_name=['1','2','3','4','5','6','7','8']
    KGUI_field = ['Disable','Output', 'Mode', 'Compliance', 'Range', 'Sweep To']
    keithley_range = ['--VOLTAGE MODE--','200e-3','2','20','200','--CURRENT MODE--','1e-6','10e-6','100e-6','1e-3','10e-3','100e-3','1']
    keithley_comp = ['--VOLTAGE MODE--','1.05e-6','10.5e-6', '105e-6','1.05e-3', '10.5e-3', '105e-3', '1.05','--CURRENT MODE--','210e-3','2.1','21','210']
    K_field = ['Disable','Output', 'Mode', 'Compliance', 'Range', 'Sweep To']
    
    def __init__(self, name, address, channels):
        self.name=name
        self.gpib=address
        self.ch=ast.literal_eval(channels)
        self.instr = rm.open_resource(self.gpib)
        self.instr.read_termination='\r\n'
        self.instr.write_termination='\r\n'
        print(self.instr.query('*IDN?'))
        print('Connected to SIM ('+name+')!')
        self.val=self.now_at()
        
    def measure(self):
        msmt=[]
        if self.ch[0]:
            # msmt.append(str(self.instr.volt()))
            msmt.append(str(self.val))
        if self.ch[1]:
            msmt.append(str(self.instr.curr()))
        return msmt

    def header(self):
        hdr=[]
        for ind in range(len(self.ch)):
            if self.ch[ind]:
                hdr.append(self.name+'_'+self.ch_name[ind])
        return hdr
    
    def set_val(self, val, por=2): #Make compatible with current
        
        self.instr.write('main_esc')
        self.instr.write('CONN'+str(por)+', "main_esc"')
        self.instr.write('VOLT '+str(val))
        self.instr.write('main_esc')
        self.val=val

    def now_at(self, port=2):
        self.instr.write('main_esc')
        self.instr.write('CONN'+str(port)+', "main_esc"')
        v=self.instr.query('VOLT?')
        self.instr.write('main_esc')
        return v
    
    def sim_voltsweep(self, st, va, po, wa =.1):
        value=float(va.text())
        step=float(st.text())
        port=int(po.text())
        
        self.sim_voltset(step, value, port, wa)
    def sweep_val(self,val, pts, wait, port=2):
        v0=self.now_at(port=port)
        
        sweep=np.linspace(v0,val,pts)
        for v in sweep:
            self.set_val(v, por=port)
            QTest.qWait(int(wait*1000))
        self.set_val(val,por=port)
    
    def control_GUI(self, frame): #Apearence for SIM
#        simcmd=Commands(instr)
        port={}
        vset={}
        batt={}
        sweep={}
        output={}
        port_list=[]
        for i,f in enumerate(self.ch):
            if f:
                port_list.append(int(self.ch_name[i]))
#        portdefault=[2,3,4]
        layout = QGridLayout()
        for i in range(len(port_list)):
            port=QLineEdit(frame)
            port.setText(str(port_list[i]))
            vset=QLineEdit(frame)
            vset.setText(self.now_at(port_list[i]))
            step=QLineEdit(frame)
            step.setText('0.1')
            batt=QPushButton('Switch Battery')
            sweep=QPushButton('Sweep')
            output=QLineEdit('Output')
            output.setDisabled(True)
            self.check_output(output,port_list[i])
            button=QPushButton('Toggle Output')
            button.clicked.connect(lambda state, outp=output, po=port: self.toggle_output(outp,po))
            sweep.clicked.connect(lambda state, st=step,va=vset,po=port: self.sweep_to(st,va,po))
            
            layout.addWidget(QLabel('Port '+str(i)),0,3*i,1,1)
            layout.addWidget(QLabel('Setpoint (V) '),1,3*i,1,1)
            layout.addWidget(batt,3,3*i,1,1)
            layout.addWidget(sweep,3,3*i+1,1,1)
            layout.addWidget(vset,1,3*i+1,1,1)
            layout.addWidget(port,0,3*i+1,1,1)
            layout.addWidget(output,4,3*i,1,1)
            layout.addWidget(button,4,3*i+1,1,1)
            layout.addWidget(QLabel('Step Size (V)'),2,3*i,1,1)
            layout.addWidget(step,2,3*i+1,1,1)
        frame.setLayout(layout)
    def check_output(self, button, port):
        self.instr.write('main_esc')
        self.instr.write('CONN'+str(port)+',"main_esc"')
        print(port)
        on=int(float(self.instr.query('EXON?')))
        self.instr.write('main_esc')
        if on == 1:
            button.setText('On')    
            button.setStyleSheet("background-color: #008B45;")
        elif on == 0:
            button.setText('Off')
            button.setStyleSheet("background-color: #CD4F39;")
        else:
            pass
    def toggle_output(self, button, portl):
        port=portl.text()
        self.instr.write('main_esc')
        self.instr.write('CONN'+port+', "main_esc"')
        on=int(float(self.instr.query('EXON?')))
        if on == 1:
            self.instr.write('OPOF')
            button.setText('Off')    
            button.setStyleSheet("background-color: #CD4F39;")
        elif on == 0:
            self.instr.write('OPON')
            button.setText('On')
            button.setStyleSheet("background-color: #008B45;")
        else:
            pass
        
        self.instr.write('main_esc')

    def set_output(self,button):
        if self.instr.output()==True:
            self.instr.output(False)
            button.setText('OFF')
            button.setStyleSheet("background-color: #CD4F39;")
        else:
            self.instr.output(True)
            button.setText('ON')
            button.setStyleSheet("background-color: #008B45;")
    
    def sweep_to(self,ste,val, por):
        value = float(val.text())
        step = int(ste.text())
        p = int(por.text())
        self.sweep_val(value, step, .1, port=p)