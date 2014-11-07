from pyqtgraph.Qt import QtGui, QtCore
from time import sleep
import pyqtgraph as pg
import numpy as np
import serial
import sys

class SerialWorker(QtCore.QThread):
    def __init__(self, parent = None):
        QtCore.QThread.__init__(self, parent)
        self.parent = parent
        self.f = serial.Serial('/dev/tty.usbmodem1411', 57600, timeout=1)

    def run(self):
        print "thread started"
        data_red = self.parent.data_red
        data_ir = self.parent.data_ir
        dataptr_red = 0
        dataptr_ir = 0
        data_pointer = dataptr_red
        data_logging = data_red
        while True:
            line = self.f.readline().strip()
            print line
            try:
                [time,object,reading] = line.split(" ")
            except ValueError:
                # We've caught slightly less than a full line of serial output, so skip it
                continue
            if (object == "RED"):
                data_logging = data_red
                data_pointer = dataptr_red
            elif (object == "IR"):
                data_logging = data_ir
                data_pointer = dataptr_ir
            elif (object == "PHOTODIODE"):
                value = int(reading)
                print value
                print data_pointer
                # data_logging[data_pointer] = value
                data_pointer = (data_pointer + 1) % len(data_logging)
            else:
                continue
            print data_logging


class MainWindow(QtGui.QWidget):
    def __init__(self, parent=None):
        print "__init__ called"
        QtGui.QWidget.__init__(self, parent)

        ## create four areas to add plots
        # self.w1 = view.addPlot()

        BUFFERSIZE = 256;

        self.data_red = []
        self.data_ir = []
        for i in range(BUFFERSIZE):
            self.data_red.append(0)
            self.data_ir.append(0)

        serial_thread = SerialWorker(self)
        serial_thread.start()

        self.setGeometry(0, 0, 600, 600)
        self.setWindowTitle('Pulse Oximeter Data')

    def __del__(self):
        print "MainWindow destructor called"
        self.serial_thread.stop()

    def stop(self):
        print "stop called"

    def close(self):
        print "close called"


app = QtGui.QApplication(sys.argv)
mw = MainWindow()
mw.show()
app.exec_()
