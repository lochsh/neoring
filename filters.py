import numpy as np
import scipy.signal
import matplotlib.pyplot as plt

h = scipy.signal.firwin(numtaps=31, width=0.01, cutoff=0.0625)
h_fp = np.round(h*127)
print("const uint8_t lpf_h[31] = {")
print(", ".join([str(int(x)) for x in h_fp]))
print("};")
print()
print("sum is:", np.sum(h_fp))

w, h = scipy.signal.freqz(h_fp/127.0)
plt.plot(w, 10*np.log10(np.abs(h)))
plt.grid()
plt.ylabel("Attenuation (dB)")
plt.xlabel("Digital Frequency")
plt.axvline(0.0625 * np.pi, color='r')
plt.savefig("filter.png")
