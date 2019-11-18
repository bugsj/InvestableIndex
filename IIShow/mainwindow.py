# -*- coding: utf-8 -*-

from PyQt5 import QtGui, QtCore
import numpy as np
import pyqtgraph as pg
import ctypes
import threading
import re

class InvestableIndex:
    def __init__(self):
        self.iicore = ctypes.cdll.LoadLibrary("..\\x64\\Release\\InvestableIndexCore.dll")
        self.iicore.InvestableIndexInit.restype = ctypes.c_int
        self.iicore.IISetIndexBasePool.restype = ctypes.c_int
        self.iicore.IISetIndexBasePool.argtypes = [ctypes.c_int, ctypes.c_int]
        self.iicore.IISimulate.restype = ctypes.c_int
        self.iicore.IISimulate.argtypes = [ctypes.c_int]
        self.iicore.IIGetDayCount.restype = ctypes.c_int
        self.iicore.IIGetDayCount.argtypes = [ctypes.c_int]
        self.iicore.IIGetDates.restype = ctypes.c_int
        self.iicore.IIGetDates.argtypes = [ctypes.c_int, ctypes.POINTER(ctypes.c_int)]
        self.iicore.IIGetCumReturn.restype = ctypes.c_int
        self.iicore.IIGetCumReturn.argtypes = [ctypes.c_int, ctypes.POINTER(ctypes.c_double)]
        self.iicore.IIAppendSimplePool.restype = ctypes.c_int
        self.iicore.IIAppendSimplePool.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.c_int)]
        self.iicore.IIRemovePools.restype = ctypes.c_int
        self.iicore.IIRemovePools.argtypes = [ctypes.c_int]
        self.iicore.IISetTopStkWeight.restype = ctypes.c_int
        self.iicore.IISetTopStkWeight.argtypes = [ctypes.c_int]
        self.iicore.IISetTopStkCount.restype = ctypes.c_int
        self.iicore.IISetTopStkCount.argtypes = [ctypes.c_int]

        self.indexobj = self.iicore.InvestableIndexInit()

    def setIndexBasePool(self, base):
        self.iicore.IISetIndexBasePool(self.indexobj, base)

    def simulate(self):
        self.iicore.IISimulate(self.indexobj)

    def getDayCount(self):
        return self.iicore.IIGetDayCount(self.indexobj)

    def getDates(self, buf):
        cnt = self.iicore.IIGetDayCount(self.indexobj)
        if buf.size != cnt:
            buf.resize(cnt)
        self.iicore.IIGetDates(self.indexobj, buf.ctypes.data_as(ctypes.POINTER(ctypes.c_int)))

    def getCumReturn(self, buf):
        cnt = self.iicore.IIGetDayCount(self.indexobj)
        if buf.size != cnt:
            buf.resize(cnt)
        self.iicore.IIGetCumReturn(self.indexobj, buf.ctypes.data_as(ctypes.POINTER(ctypes.c_double)))

    def appendSimplePool(self, stks):
        if stks.size > 0:
            self.iicore.IIAppendSimplePool(self.indexobj, stks.size, stks.ctypes.data_as(ctypes.POINTER(ctypes.c_int32)))
            
    def removePools(self):
        self.iicore.IIRemovePools(self.indexobj)

    def setTopStkCount(self, top):
        self.iicore.IISetTopStkCount(self.indexobj, top)

    def setTopStkWeight(self, top):
        self.iicore.IISetTopStkWeight(self.indexobj, top)

class MainWindow(QtGui.QWidget):
    def __init__(self, index, parent=None):
        QtGui.QWidget.__init__(self)

        self.window = parent

        vbox = QtGui.QVBoxLayout()
        self.setLayout(vbox)
        
        hbox = QtGui.QHBoxLayout()
        vbox.addLayout(hbox)

        label1 = QtGui.QLabel("基础股票池", self)
        label1.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        hbox.addWidget(label1)
        self.baseindexcombo = QtGui.QComboBox()
        self.baseindexcombo.addItems(["沪深300","中证500","中证全指","全体A股"])
        self.baseindexcombo.currentIndexChanged.connect(self.baseindexclicked)
        hbox.addWidget(self.baseindexcombo)

        label2 = QtGui.QLabel("最大选股数量", self)
        label2.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        hbox.addWidget(label2)

        self.intvalidator = QtGui.QIntValidator()
        self.maxcnt = QtGui.QLineEdit(self)
        self.maxcnt.setValidator(self.intvalidator)
        self.maxcnt.editingFinished.connect(self.topcntfinished)
        hbox.addWidget(self.maxcnt)

        label3 = QtGui.QLabel("最大个股权重", self)
        label3.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        hbox.addWidget(label3)

        self.maxweight = QtGui.QLineEdit(self)
        self.maxweight.setValidator(self.intvalidator)
        self.maxweight.editingFinished.connect(self.topweightfinished)
        hbox.addWidget(self.maxweight)

        label4 = QtGui.QLabel("%", self)
        label4.setAlignment(QtCore.Qt.AlignLeft|QtCore.Qt.AlignVCenter)
        hbox.addWidget(label4)

        vbox.addWidget(QtGui.QLabel("自定义选股", self))
               
        self.stkpool = QtGui.QPlainTextEdit(self)
        vbox.addWidget(self.stkpool)

        bapplypool = QtGui.QPushButton("自定义选股生效", self)
        bapplypool.clicked.connect(self.applyclicked)
        vbox.addWidget(bapplypool)

        self.chart = pg.PlotWidget()
        vbox.addWidget(self.chart)
        self.statusbar = QtGui.QStatusBar()
        self.window.setStatusBar(self.statusbar)
        self.updatestatusbar = False
        self.newstatusbarmsg = None

        self.index = index
        self.index.simulate()
        l = index.getDayCount()
        self.r = np.empty(l, dtype = np.float64)
        index.getCumReturn(self.r)
        self.dates = np.empty(l, dtype = np.int32)
        index.getDates(self.dates)

        self.curve = self.chart.plot(self.r)
        ticks = [list(zip(range(l), [str(x) for x in self.dates.tolist()]))[::250]]
        self.chart.getAxis("bottom").setTicks(ticks)
        
        self.redraw_event = threading.Event()
        self.redraw_event.set()
        self.datathread = threading.Thread(target = self.redrawPlot, args=())
        self.datathread.setDaemon(True)
        self.datathread.start()


        self.timer = QtCore.QTimer()
        self.timer.setInterval(50)
        self.timer.timeout.connect(self.updateData)
        self.timer.start(50)

    def textmsg(self, msg):
        print(msg)
        #self.logwindow.appendPlainText(msg)

    def redrawPlot(self):
        while True:
            if self.redraw_event.is_set():
                self.textmsg("event entry")
                self.redraw_event.clear()
                self.textmsg("clear event")
                self.newstatusbarmsg = "正在计算指数..."
                self.updatestatusbar = True
                self.index.simulate()
                self.index.getCumReturn(self.r)
                self.newstatusbarmsg = None
                self.updatestatusbar = True
                self.textmsg("wait")
                self.redraw_event.wait()

    def baseindexclicked(self, i):
        sel = self.baseindexcombo.currentText()
        if sel == '沪深300':
            self.index.setIndexBasePool(3)
        elif sel == '中证500':
            self.index.setIndexBasePool(5)
        elif sel == '中证全指':
            self.index.setIndexBasePool(9)
        elif sel == '全体A股':
            self.index.setIndexBasePool(0)
        else:
            self.index.setIndexBasePool(3)

        self.redraw_event.set()
            

    def topcntfinished(self):
        cnt = 0
        if self.maxcnt.text().isdigit():
            cnt = int(self.maxcnt.text())
        self.index.setTopStkCount(cnt)
        self.redraw_event.set()

    def topweightfinished(self):
        weight = 0
        if self.maxweight.text().isdigit():
            weight = int(self.maxweight.text())
        self.index.setTopStkWeight(weight)
        self.redraw_event.set()

    def b985clicked(self):
        self.index.setIndexBasePool(9)
        self.redraw_event.set()

    def applyclicked(self):
        stk = []
        for li in self.stkpool.toPlainText().split('\n'):
            dn = re.search("\d*", li).group(0)
            if dn != '':
                stk.append(int(dn))
        pool = np.array(stk, dtype = np.int32)
        self.index.removePools()
        self.index.appendSimplePool(pool)
        self.redraw_event.set()

    def updateData(self):
        self.curve.setData(self.r)

        if self.updatestatusbar:
            if self.newstatusbarmsg == None:
                self.statusbar.clearMessage()
            else:
                self.statusbar.showMessage(self.newstatusbarmsg)
            self.updatestatusbar = False

if __name__ == '__main__':
    import sys
    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        app = QtGui.QApplication([])
        wm = QtGui.QMainWindow()
        wm.setWindowTitle("指数模拟")
        wm.resize(800,600)
        print("creating index...")
        cw = MainWindow(InvestableIndex(), wm)
        wm.setCentralWidget(cw)
        wm.show()
        QtGui.QApplication.instance().exec_()
