from matplotlib import pyplot as plt
from scipy.io import wavfile
from scipy.signal import find_peaks, filtfilt, butter
import numpy as np
import heartpy as hp

# Read data from 'LAB5_500HzFHR.wav'
samplerate, data = wavfile.read('./LAB5_500HzFHR.wav')

time = np.arange(len(data)) / samplerate

normalized = data / max(abs(data))
plt.figure(figsize=(12, 6))
plt.plot(time, normalized, label='Normalized signal')
# plt.plot(peaks / samplerate, filtered[peaks], 'r.', label='Detected Peaks')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.title('Normalized signal')
plt.legend()
plt.show()


# Signal normalization
plt.specgram(normalized, Fs=samplerate, NFFT=1024, noverlap=512, cmap='viridis')
plt.title('Spectrogram of Normalized Signal')
plt.xlabel('Time [s]')
plt.ylabel('Frequency [Hz]')
plt.colorbar(label='Power/Frequency [dB/Hz]')
plt.ylim(0, 20)
plt.show()

# Filtration - bandpass 1-5Hz
# Nyquist frequency
f_n = samplerate / 2
# Butterworth, 4-order, bandpass 1 to 5 Hz
b, a = butter(4, [1 / f_n, 5 / f_n], btype='band')
filtered = filtfilt(b, a, normalized)

plt.specgram(filtered, Fs=samplerate, NFFT=1024, noverlap=512, cmap='viridis')
plt.title('Spectrogram of Filtered Signal')
plt.xlabel('Time [s]')
plt.ylabel('Frequency [Hz]')
plt.colorbar(label='Power/Frequency [dB/Hz]')
plt.ylim(0, 20)
plt.show()
plt.show()

peaks, _ = find_peaks(filtered, distance = f_n)

intervals = np.diff(peaks) / samplerate
heart_rate = 60 / np.mean(intervals)

time = np.arange(len(data)) / samplerate

print(f"BPM measured manually: {heart_rate}")

working_data, measures = hp.process(filtered, f_n)
print(f"BPM measured with heartpy: {measures['bpm']}")

plt.figure(figsize=(12, 6))
plt.plot(time, filtered, label='Filtered Signal')
plt.plot(peaks / samplerate, filtered[peaks], 'r.', label='Detected Peaks')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.title('Filtered Signal and Detected Peaks')
plt.legend()
plt.show()
