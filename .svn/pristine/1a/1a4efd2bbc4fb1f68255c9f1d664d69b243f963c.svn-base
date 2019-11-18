import tkinter
import threading
import numpy as np
from matplotlib import pyplot as plt

from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg, NavigationToolbar2Tk)
from matplotlib.backend_bases import key_press_handler
from matplotlib.figure import Figure

import ctypes
iicore = ctypes.cdll.LoadLibrary("..\\x64\\Release\\InvestableIndexCore.dll")

iicore.InvestableIndexInit.restype = ctypes.c_int
iicore.IISetIndexBasePool.restype = ctypes.c_int
iicore.IISetIndexBasePool.argtypes = [ctypes.c_int]
iicore.IISimulate.restype = ctypes.c_int
iicore.IIGetDayCount.restype = ctypes.c_int
iicore.IIGetCumReturn.argtypes = [ctypes.POINTER(ctypes.c_double)]

print("creating index for data...")


iicore.InvestableIndexInit()
iicore.IISimulate()
l = iicore.IIGetDayCount()

r = np.arange(l, dtype = np.double)
p = r.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
iicore.IIGetCumReturn(p)

RedrawEvent = threading.Event()
RedrawEvent.clear()

plotupdated = True

root = tkinter.Tk()
root.wm_title("windows test")

fig = Figure(figsize = (5,4), dpi = 100)
figplot = fig.add_subplot(111)
figplot.plot(r)

canvas = FigureCanvasTkAgg(fig, master = root)
canvas.draw()
canvas.get_tk_widget().pack(side = tkinter.BOTTOM, fill = tkinter.BOTH, expand = 1)

toolbar = NavigationToolbar2Tk(canvas, root)
toolbar.update()
canvas.get_tk_widget().pack(side = tkinter.BOTTOM, fill = tkinter.BOTH, expand = 1)

def on_key_press(event):
    print("you pressed {}".format(event.key))
    key_press_handler(event, canvas, toolbar)

canvas.mpl_connect("key_press_event", on_key_press)

ba = tkinter.Frame(master = root)
ba.pack(side=tkinter.TOP)

clearflag = tkinter.IntVar()

def _quit():
    root.quit()
    root.destroy()

button = tkinter.Button(master = ba, text="Quit", command=_quit)
button.pack(side=tkinter.RIGHT)


def _to300():
    iicore.IISetIndexBasePool(3)
    RedrawEvent.set();


button = tkinter.Button(master = ba, text="300", command=_to300)
button.pack(side=tkinter.RIGHT)

def _to500():
    iicore.IISetIndexBasePool(5)
    RedrawEvent.set();

button = tkinter.Button(master = ba, text="500", command=_to500)
button.pack(side=tkinter.RIGHT)


ckbox = tkinter.Checkbutton(master = ba, text="clear", variable = clearflag, onvalue = 1, offvalue = 0)
ckbox.pack(side=tkinter.RIGHT)

def RedrawPlot():
    global plotupdated
    while True:
        if RedrawEvent.is_set():
            print("entry")
            RedrawEvent.clear()
            print("clear event")
            iicore.IISimulate()
            l = iicore.IIGetDayCount()
            r = np.arange(l, dtype = np.double)
            p = r.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
            iicore.IIGetCumReturn(p)
            if clearflag.get() == 1:
                figplot.cla()
            figplot.plot(r)
            plotupdated = True
            print("wait")
            RedrawEvent.wait()

def refresh_plot():
    global plotupdated
    if plotupdated:
        plotupdated = False
        canvas.draw()
    root.after(50, refresh_plot)

th1 = threading.Thread(target = RedrawPlot, args=())
th1.setDaemon(True)
th1.start()

refresh_plot()
tkinter.mainloop()

