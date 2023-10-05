from Measurement_Instruments import SR830_inst, SR860_inst, K2400_inst, SIM_inst, yoko_inst
#from Oxford_3He_instr import MercuryiTC
from Teslatron_instr import MercuryiTC_teslatron, MercuryiPS_teslatron
from Oxford_3He_instr import MercuryiTC, AMI430
from Janis_Instruments import LakeShore335, LakeShore625
import time
import threading

class Instruments:
    
    def __init__(self):
        
        self.dic={}
        self.list_names=[];self.list_instr=[];self.list_gpib=[];self.list_ch=[]
        self.instr=[]
        self.lockins=[]
        self.not_lockins=[]
        self.read_file()
        
        # self.T_controller=LakeShore331('GPIB0::2::INSTR')
        # self.B_controller = Model_625('B','GPIB0::12::INSTR')
        
        for ind in range(len(self.list_names)):
            gpib='GPIB0::'+self.list_gpib[ind]+'::INSTR'
            if self.list_instr[ind]=='SR830':
                li=SR830_inst(self.list_names[ind],gpib,self.list_ch[ind])
                self.instr.append(li)
                self.lockins.append(li)
            elif self.list_instr[ind]=='SR860':
                li860=SR860_inst(self.list_names[ind],gpib,self.list_ch[ind])
                self.instr.append(li860)
#                self.lockins.append(li860)    
            elif self.list_instr[ind]=='Keithley 2400':
                k=K2400_inst(self.list_names[ind],gpib,self.list_ch[ind])
                self.instr.append(k)
                self.not_lockins.append(k)
            elif self.list_instr[ind]=='Yoko':
                y=yoko_inst(self.list_names[ind],gpib,self.list_ch[ind])
                self.instr.append(y)
                self.not_lockins.append(y)
            elif self.list_instr[ind]=='iTC 3He':
                itc=MercuryiTC(self.list_names[ind],self.list_ch[ind])
                self.instr.append(itc)
                self.not_lockins.append(itc)
            elif self.list_instr[ind]=='AMI 430 3He':
                ami=AMI430(self.list_names[ind])
                self.instr.append(ami)
                self.not_lockins.append(ami)
            elif self.list_instr[ind]=='iPS Teslatron':
                ips_tt=MercuryiPS_teslatron(self.list_names[ind],self.list_ch[ind])
                self.instr.append(ips_tt)
                self.not_lockins.append(ips_tt)
            elif self.list_instr[ind]=='iTC Teslatron':
                itc_tt=MercuryiTC_teslatron(self.list_names[ind],self.list_ch[ind])
                self.instr.append(itc_tt)
                self.not_lockins.append(itc_tt)
            elif self.list_instr[ind]=='Janis Temperature':
                temp=LakeShore335(self.list_names[ind],self.list_ch[ind])
                self.instr.append(temp)
                self.not_lockins.append(temp)
            elif self.list_instr[ind]=='Janis Magnet':
                temp=LakeShore625(self.list_names[ind])
                self.instr.append(temp)
                self.not_lockins.append(temp)
            elif self.list_instr[ind]=='SIM':
                sim=SIM_inst(self.list_names[ind],gpib,self.list_ch[ind])
                self.instr.append(sim)
                self.not_lockins.append(sim)
            elif self.list_instr[ind]=='tester':
                self.instr.append(TestInstr(self.list_names[ind],self.list_gpib[ind],self.list_ch[ind]))
                
            else:
                pass
        self.hdr=self.header()
        
    def header(self):
        hdr=[]
        for inst in self.instr:
            hdr.extend(inst.header())
        return hdr
    def measure(self):
        st=time.time()
        msmt=[]
        threads=[]
        fl=time.time()
        for inst in self.instr:
            threads.append(threading.Thread(target=inst.measure))
        sl=time.time()
        for t in threads:
            t.start()
        tl=time.time()
        for t in threads:
            t.join()
        el=time.time()
        for inst in self.instr:
            msmt.extend(inst.last_msmt())
        fin=time.time()
        
        # print(str(fl-st)+'\t'+str(sl-fl)+'\t'+str(tl-sl)+'\t'+str(el-sl)+'\t'+str(fin-el))
        # for inst in self.instr:
        #     msmt.extend(inst.measure())
        #print(time.time()-st)
        return msmt
    def read_file(self):
        with open('Instruments.txt', 'rt') as f:
            for line in f:
                if line.startswith('Name')==True:
                    self.list_names.append(line.split('=')[1].rstrip())
                elif line.startswith('Instrument')==True:
                    self.list_instr.append(line.split('=')[1].rstrip())
                elif line.startswith('GPIB')==True:
                    self.list_gpib.append(line.split('=')[1].rstrip())
                elif line.startswith('channels')==True:
                    self.list_ch.append(line.split('=')[1].rstrip())
                else:
                    pass
    def get_instr(self, name):
        return self.instr[self.list_names.index(name)]
        