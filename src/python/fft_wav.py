import numpy as np
import matplotlib.pyplot as plt
from scipy.fftpack import fft
from scipy.io import wavfile as wav

if __name__ == "__main__":
    fs, data = wav.read("../../audio/test1.wav")
    Ts = 1.0 / fs
    N = len(data)
    t = N / fs
    left_data = data[:, 0]
    right_data = data[:, 1]
    x = np.arange(0, t, Ts)

    fft_left = np.fft.fft(left_data)
    yw = np.abs(fft_left[: N // 2]) / N
    xw = np.linspace(0.0, 1.0 / (2.0 * Ts), N // 2)

    fs2, data2 = wav.read("../../audio/test2.wav")
    Ts2 = 1.0 / fs2
    N2 = len(data2)
    t2 = N2 / fs2
    left_data2 = data2[:, 0]
    right_data2 = data2[:, 1]
    x2 = np.arange(0, t2, Ts2)
    
    fft_left2 = fft(left_data2)
    yw2 = np.abs(fft_left2[: N2 // 2]) / N2
    xw2 = np.linspace(0.0, 1.0 / (2.0 * Ts2), N2 // 2)

    # fs = 80, fs2 = 400
    # Fn = n * (fs / N), Fres = fs / N
    # Fres_1 = 80 / 400  -> 間隔: 0.2Hz
    # Fres_2 = 400 / 400 -> 間隔: 1Hz   
    # -> n = 100時
    #   w1 = 100 * 0.2 = 20Hz
    #   w2 = 100 * 1   = 100Hz 

    plt.figure()
    plt.subplot(221)
    plt.plot(x, left_data, 'k')
    plt.xlabel("Time [s]")
    plt.ylabel("Amplitude")
    plt.grid()

    plt.subplot(222)
    plt.plot(x2, left_data2, 'k')
    plt.xlabel("Time [s]")
    plt.ylabel("Amplitude")
    plt.grid()

    plt.subplot(223)
    plt.plot(xw, yw, 'r')
    plt.xlabel("Frequency [Hz]")
    plt.ylabel("magnitude")
    plt.grid()

    plt.subplot(224)
    plt.plot(xw2, yw2, 'r')
    plt.xlabel("Frequency [Hz]")
    plt.ylabel("magnitude")
    plt.grid()
        
    plt.tight_layout()
    plt.show()
