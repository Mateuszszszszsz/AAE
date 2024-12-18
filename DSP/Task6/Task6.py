import heartpy as hp
import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile
from scipy.signal import spectrogram, filtfilt, butter, find_peaks
import matplotlib
matplotlib.use('Agg')  # Use the Agg backend for rendering (non-interactive)

# Load the WAV file
samplerate, data = wavfile.read('LAB5_500HzFHR.wav')
data = data / np.max(np.abs(data), axis=0) #normalization
f_n = samplerate/2 #nyquist frequency

#ploting raw data after normalization
plt.figure(1)
plt.plot(data)
plt.savefig('data.png')


#bandpass filtering
filtered = hp.filter_signal(data, cutoff = [0.5, 5], sample_rate = samplerate , order = 4, filtertype='bandpass')
# b, a = butter(4,[1/f_n,5/f_n], btype = 'band')    #butterworth coefs
# filtered  = filtfilt(b,a,data)                    #butterworth filter

plt.figure(2)
plt.plot(filtered)
plt.savefig('filtered_data.png')



# Compute the Spectrogram
frequencies, times, Sxx = spectrogram(filtered, fs=samplerate, nperseg=1024, noverlap=512)

# Plot the Spectrogram 

# plt.specgram(filtered, Fs=samplerate, NFFT=1024, noverlap=512, cmap='viridis')
# plt.title('Spectrogram of Filtered Signal')
# plt.xlabel('Time [s]')
# plt.ylabel('Frequency [Hz]')
# plt.colorbar(label='Power/Frequency [dB/Hz]')
# plt.ylim(0, 20)



plt.pcolormesh(times, frequencies, -10, shading='gouraud', cmap='viridis')
plt.title('Time-Frequency Spectrogram')
plt.ylabel('Frequency (Hz)')
plt.xlabel('Time (s)')
plt.colorbar(label='Power (dB)')
plt.ylim([0, 10])  # Limit to Nyquist frequency
plt.tight_layout()

# Save the plot to a file
plt.savefig('spectrogram.png')
print("Spectrogram saved as 'spectrogram.png'")

peaks, _ = find_peaks(filtered, distance = f_n*1.2)

plt.figure()
plt.plot(filtered)
plt.plot(peaks, filtered[peaks], 'ro')  # Peaks from find_peaks
plt.savefig('manual_peaks.png')

intervals = np.diff(peaks) / samplerate
heart_rate = 60 / np.mean(intervals)

time = np.arange(len(data)) / samplerate

print(f"BPM measured ourselves: {heart_rate}")

working_data, measures = hp.process(filtered, f_n)
print(f"BPM measured from heartpy: {measures['bpm']}")