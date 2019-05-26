import numpy as np
from scipy.fftpack import fft, ifft, fftfreq, fftshift
import matplotlib.pyplot as plt

if __name__ == "__main__":
    # 根據取樣定理：取樣頻率要大於被取樣的訊號中最大頻率的兩倍
    # Sampling information
    fs = 80       # Sampling rate
    Ts = 1.0 / fs # The sampling period (Sampling interval )
    t = 1.0       # seconds of sampling
    N  = fs * t   # total points in signal

    # Signal information
    f = 20 # in hertz, the desired natural frequency
    x = np.arange(0, t, Ts)  # time vector,這裡Ts也是步長
    y = np.cos(2*np.pi*f*x)

    """Aliasing

    高頻的analog信號,經過樣後被看成低頻的digital信號
    此現象稱為 aliasing
    滿足 -(1/2)*ws < w - n*ws < (1/2)*ws
    其中：w     原analog signal
         w-nws digital幻象的頻率

    example:
        Alias of f: f + n*fs, 假設 f: 20Hz, fs: 80Hz
        -> 100, 180, 260, 340...
        
    驗證:
        公式: -(1/2)*fs < f - n*fs < (1/2)*fs, w = 2*pi*f(rad/s)
          => |f - n*fs| < 1/2*fs
          => |100 - n*80| < 40
          => n = 1, f - n*fs = 20
          
          => f=20Hz 和 f=100Hz在經過fs=80Hz採樣後,spectrum完全一樣
             通常會被人視為叫低頻的信號
    """

    # Aliasing
    f = 100 # in hertz, the desired natural frequency
    x_origin = np.arange(0, t, 1.0 / 1000)
    y_origin = np.cos(2*np.pi*f*x_origin)
    x_alias = np.arange(0, t, Ts)  # time vector,這裡Ts也是步長
    y_alias = np.cos(2*np.pi*f*x_alias)
    
    # Fast Fourier Transform
    yf = fft(y)
    yf_alias = fft(y_alias)
    
    # 由於對稱性，只取一半區間 -> 絕對值 -> 歸一化
    yw = np.abs(yf[0:int(N//2)]) / N
    xw = np.linspace(0.0, 1.0/(2.0*Ts), N//2)
    yw2 = np.abs(yf_alias[0:int(N//2)]) / N
    xw2 = np.linspace(0.0, 1.0/(2.0*Ts), N//2)

    plt.subplot(211)
    plt.plot(x, y, 'k')
    #plt.xlim(0.0, t)
    #plt.title('sin(2pi(5)t)')
    plt.xlabel("Time [s]")
    plt.ylabel("Amplitude")
    plt.grid()
    plt.tight_layout()

    plt.subplot(212)
    plt.plot(xw, yw, 'r')
    #plt.xlim(0.0, fs // 2)
    #plt.ylim(0, 1)
    plt.xlabel("Frequency [Hz]")
    plt.ylabel("magnitude")
    plt.grid()
    plt.tight_layout()

    # Aliasing figure
    fig, ax = plt.subplots()
    plt.subplot(211)
    plt.plot(x_origin, y_origin, 'k')
    plt.plot(x_alias, y_alias, 'b')
    plt.plot(x, y, 'r')
    plt.xlim(0.0, 0.03)
    #plt.title('sin(2pi(25)t)')
    plt.xlabel("Time [s]")
    plt.ylabel("Amplitude")
    plt.legend(['cos(100Hz)', 'alias', 'cos(20Hz)'], loc = 0)
    plt.grid()
    plt.tight_layout()

    plt.subplot(212)
    plt.plot(xw2, yw2, 'r')
    #plt.xlim(0.0, fs // 2)
    #plt.ylim(0, 1)
    plt.xlabel("Frequency [Hz]")
    plt.ylabel("magnitude")
    plt.grid()
    plt.tight_layout()

    fig, ax = plt.subplots()
    plt.plot(xw, yw, linewidth = 3)
    ax.set_xscale('log')
    ax.set_yscale('log')
    plt.ylabel('Amplitude')
    plt.xlabel('Frequency [Hz]')
    plt.tight_layout()
    
    plt.show()