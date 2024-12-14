from scipy.io import wavfile
import heartpy as hp

# Read data from 'LAB5_500HzFHR.wav'
samplerate, data = wavfile.read('./LAB5_500HzFHR.wav')
# Signal normalization
normalized = data / max(abs(data))

working_data, measures = hp.process(data, samplerate)

print(f"Measured BPM: {measures['bpm']}")