#https://pythonbaba.com/online-python-code-editor-and-ide-for-data-science/
# -*- coding: utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt

tau = 100e-6

def impulse(t, tau):
    if abs(t) >= tau / 2:
        return 0.0
    return (1 + np.cos(2 * np.pi * t / tau)) / 2
def spectrum(f, tau):
    if np.isclose(f, 0):
        return tau / (2 * (1 - f * f * tau * tau))
    return np.sin(np.pi * f * tau) / (2 * np.pi * f * (1 - f * f * tau * tau))

t_band = np.linspace(-tau, tau, num = 2**14)
f_band = np.linspace(-10 / tau, 10 / tau, num = 2**14)
x = np.array([impulse(t, tau) for t in t_band])
Xf = np.array([spectrum(f, tau) for f in f_band])

plt.figure()
plt.subplot(2, 1, 1)
plt.plot(t_band * 1e6, x)
plt.title("Импульс $x_5(t)$")
plt.xlabel("$t$, мкc")
plt.ylabel("$x_5(t)$, В")
plt.tight_layout()
plt.grid()
plt.subplot(2, 1, 2)
plt.title("АЧХ импульса $x_5(t)$")
plt.plot(f_band * 1e-3, abs(Xf)*1e6)
plt.ylabel("$|X_5(f)|$, мкВ/Гц")
plt.xlabel("$f$, кГц")
plt.tight_layout()
plt.grid()
plt.show()
