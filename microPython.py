from PyQt5 import QtWidgets, QtCore, QtGui
from pyqtgraph import PlotWidget, plot
from PyQt5 import Qt
import pyqtgraph as pg
import sys 
import os
from random import randint
import serial.tools.list_ports
import random
import struct
import binascii

class MyWidget(QtWidgets.QWidget):
    def __init__(self):
        super().__init__()

        self.low=float(100)
        self.high=float(0)
        self.treshold_mid_high = float(0)
        self.treshold_low_mid = float(0)

        self.lowVibration=float(100)
        self.highVibration=float(0)
        self.treshold_mid_highVibration = float(0)
        self.treshold_low_midVibration = float(0)

        self.colors = ['g','g']

        self.xBar = [0,1]
        self.yBar = [0,0]
        self.plot=pg.plot()

        self.bargraph = pg.BarGraphItem(x=self.xBar,height=self.yBar,y0=0,width=0.7,brushes=self.colors)
        self.plot.addItem(self.bargraph)
        self.plot.setRange(xRange=[-1,2],yRange=[0,4])
        
        self.text = QtWidgets.QLabel("Ambiental si vibratii",
                                     alignment=QtCore.Qt.AlignCenter)           

        
        self.RGB = "vibratii"
        self.button = QtWidgets.QPushButton("Schimba afisajul RGB pentru " + self.RGB)

        self.layout = QtWidgets.QVBoxLayout(self)
        self.layout.addWidget(self.text)
        self.layout.addWidget(self.plot)
        self.layout.addWidget(self.button)
    

        self.button.clicked.connect(self.update_RGB)

    def updateHighAndLowVibration(self,vibrationValue):
        if vibrationValue < self.lowVibration:
            self.lowVibration = vibrationValue
        if vibrationValue > self.highVibration:
            self.highVibration = vibrationValue
    
    def updateTresholdVibration(self):
        self.treshold_mid_highVibration=self.lowVibration +(self.highVibration-self.lowVibration)*(float(2)/float(3))
        self.treshold_low_midVibration=self.lowVibration+(self.highVibration-self.lowVibration)/(float(3))

    def updateHighAndLowAmbiental(self,ambientalValue):
        if ambientalValue < self.low:
            self.low = ambientalValue
        if ambientalValue > self.high:
            self.high = ambientalValue
        
    def updateTresholdAmbiental(self):
        self.treshold_mid_high=self.low +(self.high-self.low)*(float(2)/float(3))
        self.treshold_low_mid=self.low+(self.high-self.low)/(float(3))

    def changeColor(self,ambientalValue,vibrationValue):
        self.yBar[0]=ambientalValue
        self.yBar[1]=vibrationValue

        
        if ambientalValue > self.low and ambientalValue<self.treshold_low_mid:
            self.colors[0]='g'
        elif ambientalValue > self.treshold_low_mid and ambientalValue < self.treshold_mid_high:
            self.colors[0]='y'
        elif ambientalValue > self.treshold_mid_high and ambientalValue<self.high:
            self.colors[0]='r'


        if vibrationValue > self.lowVibration and vibrationValue<self.treshold_low_midVibration:
            self.colors[1]='g'
        elif vibrationValue > self.treshold_low_midVibration and vibrationValue < self.treshold_mid_highVibration:
            self.colors[1]='y'
        elif vibrationValue > self.treshold_mid_highVibration and vibrationValue<self.highVibration:
            self.colors[1]='r'

        self.bargraph.setOpts(y=self.yBar,y0=0,x=self.xBar,brushes=self.colors)
    
    def update_RGB(self):
        if self.RGB == "vibratii":
            self.RGB = "lumina"
            swich_RGB("vibratii")
        else:
            self.RGB = "vibratii"
            swich_RGB("lumina")
            
        self.button.setText("Schimba afisajul RGB pentru " + self.RGB)


def swich_RGB(swichTo):
    global serialInstance
    
    if(swichTo == "lumina"):
        serialInstance.write('a'.encode())
    else:
        serialInstance.write('v'.encode())


def read():
    global serialInstance
    global widget
    global start

    serialInstance.write('s'.encode())

    if serialInstance.inWaiting():
        a=serialInstance.read(8)
        numbers=struct.unpack("<2f",a)
        #print(numbers)
        widget.updateHighAndLowAmbiental(ambientalValue=numbers[0])
        widget.updateTresholdAmbiental()
        widget.updateHighAndLowVibration(vibrationValue=numbers[1])
        widget.updateTresholdVibration()
        widget.changeColor(ambientalValue = numbers[0],vibrationValue=numbers[1])

serialInstance=serial.Serial()
port="COM3"
serialInstance.baudrate=115200
serialInstance.port=port
serialInstance.timeout=None
serialInstance.open()
data=30
timerSerial = QtCore.QTimer()
timerSerial.setInterval(1)

app = QtWidgets.QApplication([])


widget = MyWidget()
widget.resize(800, 600)
widget.show()

timerSerial.timeout.connect(read)
timerSerial.start()


sys.exit(app.exec())
