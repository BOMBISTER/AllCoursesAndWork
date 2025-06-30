import math
import numpy as np
import matplotlib.pyplot as plt
import scipy.signal as scs
import random


# Функция применения низкочастотного фильтра (ФНЧ) к сигналу
def lpf(x_arr, y_arr_f0, cutoff_freq):
    i = complex(0, 1)
    freq_arr = np.linspace(1, 1 / (x_arr[1] - x_arr[0]), len(x_arr))
    y_arr_fft = np.fft.fft(y_arr_f0)
    wc = 2 * math.pi * cutoff_freq
    y_arr_lpf_fft = []
    for q in range(len(x_arr)):
        w = 2 * math.pi * freq_arr[q]
        h = -wc ** 2 / (-w ** 2 + i * 2 ** 0.5 * wc * w + 1)
        y_arr_lpf_fft.append(y_arr_fft[q] * abs(h))
    return np.fft.ifft(y_arr_lpf_fft)


# Функция применения высокочастотного фильтра (ФВЧ) к сигналу
def hpf(x_arr, y_arr_f0, cutoff_freq):
    i = complex(0, 1)
    freq_arr = np.linspace(1, 1 / (x_arr[1] - x_arr[0]), len(x_arr))
    y_arr_fft = np.fft.fft(y_arr_f0)
    wc = 2 * math.pi * cutoff_freq
    y_arr_hpf_fft = []
    for q in range(len(x_arr)):
        w = 2 * math.pi * freq_arr[q]
        h = w ** 2 / (-w ** 2 + i * 2 ** 0.5 * wc * w + wc ** 2)
        y_arr_hpf_fft.append(y_arr_fft[q] * abs(h))
    return np.fft.ifft(y_arr_hpf_fft)


# Функция применения полосового фильтра (ПФ) с двумя частотами среза
def bf(x_arr, y_arr_f0, cutoff_freq_a, cutoff_freq_b):
    i = complex(0, 1)
    freq_arr = np.linspace(1, 1 / (x_arr[1] - x_arr[0]), len(x_arr))
    y_arr_fft = np.fft.fft(y_arr_f0)
    wc_a = 2 * math.pi * cutoff_freq_a
    wc_b = 2 * math.pi * cutoff_freq_b
    y_arr_bf_fft = []
    for q in range(len(x_arr)):
        w = 2 * math.pi * freq_arr[q]
        h_lpf = w ** 2 / (-w ** 2 + i * 2 ** 0.5 * wc_b * w + wc_b ** 2)
        h_hpf = wc_a ** 2 / (-wc_a ** 2 + i * 2 ** 0.5 * wc_a * w + 1)
        y_arr_bf_fft.append(y_arr_fft[q] * h_lpf * h_hpf * (-1))
    return np.fft.ifft(y_arr_bf_fft)


# Функция применения заградительного фильтра (ЗФ) с двумя частотами среза
def sf(x_arr, y_arr_f0, cutoff_freq_a, cutoff_freq_b):
    i = complex(0, 1)
    freq_arr = np.linspace(1, 1 / (x_arr[1] - x_arr[0]), len(x_arr))
    y_arr_fft = np.fft.fft(y_arr_f0)
    wc_a = 2 * math.pi * cutoff_freq_a
    wc_b = 2 * math.pi * cutoff_freq_b
    y_arr_sf_fft = []
    for q in range(len(x_arr)):
        w = 2 * math.pi * freq_arr[q]
        h_lpf = w ** 2 / (-w ** 2 + i * 2 ** 0.5 * wc_b * w + wc_b ** 2)
        h_hpf = wc_a ** 2 / (-wc_a ** 2 + i * 2 ** 0.5 * wc_a * w + 1)
        y_arr_sf_fft.append(y_arr_fft[q] * (1 + h_lpf * h_hpf))
    return np.fft.ifft(y_arr_sf_fft)


# Функция применения фильтра n-го порядка к сигналу
def f_nth(x_arr, y_arr_f0, cutoff_freq, n):
    i = complex(0, 1)
    freq_arr = np.linspace(1, 1 / (x_arr[1] - x_arr[0]), len(x_arr))
    y_arr_fft = np.fft.fft(y_arr_f0)
    wc = 2 * math.pi * cutoff_freq
    y_arr_fnth_fft = []
    for q in range(len(x_arr)):
        w = 2 * math.pi * freq_arr[q]
        h = 1 / (1 + (-1) ** n * (i * w / wc) ** (2 * n))
        y_arr_fnth_fft.append(y_arr_fft[q] * h)
    return np.fft.ifft(y_arr_fnth_fft)


# Функция создания массива значений x с заданным шагом
def x_arr_constructor(a, b, d=0.01):
    a = int(a // d)
    b = int(b // d)
    return [i * d for i in range(a, b)]


# Функция вычисления значений функции для массива x
def function_iterator(function, x_arr):
    return [function(x) for x in x_arr]


# Задача 1: Построение исходного сигнала и его спектра
def task_1():
    a, b = -0.1, 0.1
    w1, w2, w3 = [2 * math.pi * f for f in [50, 150, 450]]
    cos_lab_3 = lambda x: math.cos(w1 * x) + math.cos(w2 * x) + math.cos(w3 * x)
    x_arr = x_arr_constructor(a, b, 0.0001)
    y_arr_f0 = function_iterator(cos_lab_3, x_arr)
    freq_arr = np.linspace(1, 1 / (x_arr[1] - x_arr[0]), len(x_arr))
    y_arr_fft = np.fft.fft(y_arr_f0)

    plt.figure(figsize=(10, 6))
    plt.subplot(2, 1, 1)
    plt.plot(x_arr, y_arr_f0, 'b-', linewidth=1.5)
    plt.xlabel("x", fontsize=12)
    plt.ylabel("y", fontsize=12)
    plt.title("Исходная функция", fontsize=14)
    plt.grid(True, linestyle='--', alpha=0.7)

    plt.subplot(2, 1, 2)
    plt.plot(freq_arr[:len(x_arr) // 20], np.abs(y_arr_fft[:len(x_arr) // 20]), 'r--', linewidth=1.5)
    plt.xlabel("Частота", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.title("Спектр (FFT)", fontsize=14)
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.tight_layout()
    plt.show()


# Задача 2: Применение ФНЧ и сравнение спектров
def task_2():
    a, b, cutoff_freq = -0.1, 0.1, 120
    w1, w2, w3 = [2 * math.pi * f for f in [50, 150, 450]]
    cos_lab_3 = lambda x: math.cos(w1 * x) + math.cos(w2 * x) + math.cos(w3 * x)
    special_cos = lambda x: math.cos(2 * math.pi * 50 * x)
    x_arr = x_arr_constructor(a, b, 0.0001)
    y_arr_f0 = function_iterator(cos_lab_3, x_arr)
    y_arr_f01 = function_iterator(special_cos, x_arr)
    freq_arr = np.linspace(1, 1 / (x_arr[1] - x_arr[0]), len(x_arr))
    y_arr_fft_0 = np.fft.fft(y_arr_f0)
    y_arr_f1 = lpf(x_arr, y_arr_f0, cutoff_freq)
    y_arr_fft_1 = np.fft.fft(y_arr_f1)

    plt.figure(figsize=(12, 8))
    plt.subplot(2, 2, 1)
    plt.plot(x_arr, y_arr_f0, 'g-', linewidth=1.5)
    plt.xlabel("x", fontsize=12)
    plt.ylabel("y", fontsize=12)
    plt.title("Исходный сигнал", fontsize=14)
    plt.grid(True)

    plt.subplot(2, 2, 2)
    plt.plot(freq_arr[:len(x_arr) // 20], np.abs(y_arr_fft_0[:len(x_arr) // 20]), 'm--', linewidth=1.5)
    plt.xlabel("Частота", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.title("Спектр (FFT)", fontsize=14)
    plt.grid(True)

    plt.subplot(2, 2, 3)
    plt.plot(x_arr, y_arr_f1.real, 'b-', linewidth=1.5, label='ФНЧ')
    plt.plot(x_arr, y_arr_f01, 'r--', linewidth=1.5, label='Эталон')
    plt.xlabel("x", fontsize=12)
    plt.ylabel("y", fontsize=12)
    plt.title("Сигнал после ФНЧ", fontsize=14)
    plt.legend()
    plt.grid(True)

    plt.subplot(2, 2, 4)
    plt.plot(freq_arr[:len(x_arr) // 20], np.abs(y_arr_fft_1[:len(x_arr) // 20]), 'c-.', linewidth=1.5)
    plt.xlabel("Частота", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.title("Спектр после ФНЧ", fontsize=14)
    plt.grid(True)
    plt.tight_layout()
    plt.show()


# Задача 3: Сравнение сигналов с разными частотами после ФНЧ
def task_3():
    a, b, cutoff_freq = -0.1, 0.1, 120
    w1, w2, w3 = [2 * math.pi * f for f in [50, 150, 450]]
    cos_lab_3 = lambda x: math.cos(w1 * x) + math.cos(w2 * x) + math.cos(w3 * x)
    special_cos = lambda x: math.cos(2 * math.pi * 50 * x)
    cos_lab_3_1 = lambda x: math.cos(w1 * x) + math.cos(w3 * x)
    x_arr = x_arr_constructor(a, b, 0.0001)
    y_arr_f0 = function_iterator(cos_lab_3, x_arr)
    y_arr_f01 = function_iterator(special_cos, x_arr)
    y_arr_f0_1 = function_iterator(cos_lab_3_1, x_arr)
    freq_arr = np.linspace(1, 1 / (x_arr[1] - x_arr[0]), len(x_arr))
    y_arr_f1 = lpf(x_arr, y_arr_f0, cutoff_freq)
    y_arr_fft_1 = np.fft.fft(y_arr_f1)
    y_arr_f1_1 = lpf(x_arr, y_arr_f0_1, cutoff_freq)
    y_arr_fft_1_1 = np.fft.fft(y_arr_f1_1)

    plt.figure(figsize=(12, 8))
    plt.subplot(2, 2, 1)
    plt.plot(x_arr, y_arr_f1.real, 'b-', linewidth=1.5, label='ФНЧ')
    plt.plot(x_arr, y_arr_f01, 'r--', linewidth=1.5, label='Эталон')
    plt.xlabel("x", fontsize=12)
    plt.ylabel("y", fontsize=12)
    plt.title("Сигнал (50, 150, 450 Гц) после ФНЧ", fontsize=14)
    plt.legend()
    plt.grid(True)

    plt.subplot(2, 2, 2)
    plt.plot(freq_arr[:len(x_arr) // 20], np.abs(y_arr_fft_1[:len(x_arr) // 20]), 'm-.', linewidth=1.5)
    plt.xlabel("Частота", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.title("Спектр (50, 150, 450 Гц) после ФНЧ", fontsize=14)
    plt.grid(True)

    plt.subplot(2, 2, 3)
    plt.plot(x_arr, y_arr_f1_1.real, 'g-', linewidth=1.5, label='ФНЧ')
    plt.plot(x_arr, y_arr_f01, 'r--', linewidth=1.5, label='Эталон')
    plt.xlabel("x", fontsize=12)
    plt.ylabel("y", fontsize=12)
    plt.title("Сигнал (50, 450 Гц) после ФНЧ", fontsize=14)
    plt.legend()
    plt.grid(True)

    plt.subplot(2, 2, 4)
    plt.plot(freq_arr[:len(x_arr) // 20], np.abs(y_arr_fft_1_1[:len(x_arr) // 20]), 'c--', linewidth=1.5)
    plt.xlabel("Частота", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.title("Спектр (50, 450 Гц) после ФНЧ", fontsize=14)
    plt.grid(True)
    plt.tight_layout()
    plt.show()


# Задача 4: Применение ФВЧ и сравнение спектров
def task_4():
    a, b, cutoff_freq = -0.1, 0.1, 300
    w1, w2, w3 = [2 * math.pi * f for f in [50, 150, 450]]
    cos_lab_3 = lambda x: math.cos(w1 * x) + math.cos(w2 * x) + math.cos(w3 * x)
    special_cos = lambda x: math.cos(2 * math.pi * 450 * x)
    x_arr = x_arr_constructor(a, b, 0.0001)
    y_arr_f0 = function_iterator(cos_lab_3, x_arr)
    y_arr_f01 = function_iterator(special_cos, x_arr)
    freq_arr = np.linspace(1, 1 / (x_arr[1] - x_arr[0]), len(x_arr))
    y_arr_fft_0 = np.fft.fft(y_arr_f0)
    y_arr_f1 = hpf(x_arr, y_arr_f0, cutoff_freq)
    y_arr_fft_1 = np.fft.fft(y_arr_f1)

    plt.figure(figsize=(12, 8))
    plt.subplot(2, 2, 1)
    plt.plot(x_arr, y_arr_f0, 'g-', linewidth=1.5)
    plt.xlabel("x", fontsize=12)
    plt.ylabel("y", fontsize=12)
    plt.title("Исходный сигнал", fontsize=14)
    plt.grid(True)

    plt.subplot(2, 2, 2)
    plt.plot(freq_arr[:len(x_arr) // 20], np.abs(y_arr_fft_0[:len(x_arr) // 20]), 'm--', linewidth=1.5)
    plt.xlabel("Частота", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.title("Спектр (FFT)", fontsize=14)
    plt.grid(True)

    plt.subplot(2, 2, 3)
    plt.plot(x_arr, y_arr_f1.real, 'b-', linewidth=1.5, label='ФВЧ')
    plt.plot(x_arr, y_arr_f01, 'r--', linewidth=1.5, label='Эталон')
    plt.xlabel("x", fontsize=12)
    plt.ylabel("y", fontsize=12)
    plt.title("Сигнал после ФВЧ", fontsize=14)
    plt.legend()
    plt.grid(True)

    plt.subplot(2, 2, 4)
    plt.plot(freq_arr[:len(x_arr) // 20], np.abs(y_arr_fft_1[:len(x_arr) // 20]), 'c-.', linewidth=1.5)
    plt.xlabel("Частота", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.title("Спектр после ФВЧ", fontsize=14)
    plt.grid(True)
    plt.tight_layout()
    plt.show()


# Задача 5: Применение ПФ и ЗФ
def task_5():
    a, b, cutoff_freq_a, cutoff_freq_b = -0.1, 0.1, 100, 200
    w1, w2, w3 = [2 * math.pi * f for f in [50, 150, 450]]
    cos_lab_3 = lambda x: math.cos(w1 * x) + math.cos(w2 * x) + math.cos(w3 * x)
    x_arr = x_arr_constructor(a, b, 0.0001)
    y_arr_f0 = function_iterator(cos_lab_3, x_arr)
    freq_arr = np.linspace(1, 1 / (x_arr[1] - x_arr[0]), len(x_arr))
    y_arr_bf = bf(x_arr, y_arr_f0, cutoff_freq_a, cutoff_freq_b)
    y_arr_bf_fft = np.fft.fft(y_arr_bf)
    y_arr_sf = sf(x_arr, y_arr_f0, 100, 250)
    y_arr_sf_fft = np.fft.fft(y_arr_sf)

    plt.figure(figsize=(12, 8))
    plt.subplot(2, 2, 1)
    plt.plot(x_arr, y_arr_bf.real, 'b-', linewidth=1.5)
    plt.xlabel("x", fontsize=12)
    plt.ylabel("y", fontsize=12)
    plt.title("Сигнал после ПФ", fontsize=14)
    plt.grid(True)

    plt.subplot(2, 2, 2)
    plt.plot(freq_arr[:len(x_arr) // 20], np.abs(y_arr_bf_fft[:len(x_arr) // 20]), 'r--', linewidth=1.5)
    plt.xlabel("Частота", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.title("Спектр после ПФ", fontsize=14)
    plt.grid(True)

    plt.subplot(2, 2, 3)
    plt.plot(x_arr, y_arr_sf.real, 'g-', linewidth=1.5)
    plt.xlabel("x", fontsize=12)
    plt.ylabel("y", fontsize=12)
    plt.title("Сигнал после ЗФ", fontsize=14)
    plt.grid(True)

    plt.subplot(2, 2, 4)
    plt.plot(freq_arr[:len(x_arr) // 20], np.abs(y_arr_sf_fft[:len(x_arr) // 20]), 'm-.', linewidth=1.5)
    plt.xlabel("Частота", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.title("Спектр после ЗФ", fontsize=14)
    plt.grid(True)
    plt.tight_layout()
    plt.show()


# Задача 6: Сравнение ФНЧ и фильтра n-го порядка
def task_6():
    a, b, cutoff_freq = -0.1, 0.1, 120
    w1, w2, w3 = [2 * math.pi * f for f in [50, 150, 450]]
    cos_lab_3 = lambda x: math.cos(w1 * x) + math.cos(w2 * x) + math.cos(w3 * x)
    x_arr = x_arr_constructor(a, b, 0.0001)
    y_arr_f0 = function_iterator(cos_lab_3, x_arr)
    freq_arr = np.linspace(1, 1 / (x_arr[1] - x_arr[0]), len(x_arr))
    y_arr_fft_0 = np.fft.fft(y_arr_f0)
    y_arr_fnth = f_nth(x_arr, y_arr_f0, cutoff_freq, 5)
    y_arr_fnth_2 = f_nth(x_arr, y_arr_f0, cutoff_freq, 2)
    y_arr_fnth_fft = np.fft.fft(y_arr_fnth)
    y_arr_fnth_fft_2 = np.fft.fft(y_arr_fnth_2)
    y_arr_lpf = lpf(x_arr, y_arr_f0, cutoff_freq)
    y_arr_lpf_fft = np.fft.fft(y_arr_lpf)

    plt.figure(figsize=(12, 10))
    plt.subplot(3, 2, 1)
    plt.plot(x_arr, y_arr_f0, 'g-', linewidth=1.5)
    plt.xlabel("x", fontsize=12)
    plt.ylabel("y", fontsize=12)
    plt.title("Исходный сигнал", fontsize=14)
    plt.grid(True)

    plt.subplot(3, 2, 2)
    plt.plot(freq_arr[:len(x_arr) // 20], np.abs(y_arr_fft_0[:len(x_arr) // 20]), 'r--', linewidth=1.5)
    plt.xlabel("Частота", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.title("Спектр (FFT)", fontsize=14)
    plt.grid(True)

    plt.subplot(3, 2, 3)
    plt.plot(x_arr, y_arr_lpf.real, 'b-', linewidth=1.5)
    plt.xlabel("x", fontsize=12)
    plt.ylabel("y", fontsize=12)
    plt.title("Сигнал после ФНЧ", fontsize=14)
    plt.grid(True)

    plt.subplot(3, 2, 4)
    plt.plot(freq_arr[:len(x_arr) // 20], np.abs(y_arr_lpf_fft[:len(x_arr) // 20]), 'c-.', linewidth=1.5)
    plt.xlabel("Частота", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.title("Спектр после ФНЧ", fontsize=14)
    plt.grid(True)

    plt.subplot(3, 2, 5)
    plt.plot(x_arr, y_arr_fnth.real, 'm-', linewidth=1.5)
    plt.xlabel("x", fontsize=12)
    plt.ylabel("y", fontsize=12)
    plt.title("Сигнал после фильтра 5-го порядка", fontsize=14)
    plt.grid(True)

    plt.subplot(3, 2, 6)
    plt.plot(freq_arr[:len(x_arr) // 20], np.abs(y_arr_fnth_fft[:len(x_arr) // 20]), 'k--', linewidth=1.5, label='n=5')
    plt.plot(freq_arr[:len(x_arr) // 20], np.abs(y_arr_fnth_fft_2[:len(x_arr) // 20]), 'y-.', linewidth=1.5,
             label='n=2')
    plt.xlabel("Частота", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.title("Спектр после фильтра n-го порядка", fontsize=14)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()


# Задача 8: Сравнение фильтра Баттерворта и n-го порядка
def task_8():
    a, b, cutoff_freq, n = -0.1, 0.1, 120, 5
    w1, w2, w3 = [2 * math.pi * f for f in [50, 150, 450]]
    cos_lab_3 = lambda x: math.cos(w1 * x) + math.cos(w2 * x) + math.cos(w3 * x)
    x_arr = x_arr_constructor(a, b, 0.0001)
    y_arr_f0 = function_iterator(cos_lab_3, x_arr)
    freq_arr = np.linspace(1, 1 / (x_arr[1] - x_arr[0]), len(x_arr))
    y_arr_fft_0 = np.fft.fft(y_arr_f0)
    y_arr_fnth = f_nth(x_arr, y_arr_f0, cutoff_freq, n)
    y_arr_fnth_fft = np.fft.fft(y_arr_fnth)
    nq, wcq = scs.butter(n, cutoff_freq * 2 / 10000)
    y_arr_filtfilt = scs.filtfilt(nq, wcq, y_arr_f0)
    y_arr_filtfilt_fft = np.fft.fft(y_arr_filtfilt)

    plt.figure(figsize=(12, 10))
    plt.subplot(3, 2, 1)
    plt.plot(x_arr, y_arr_f0, 'g-', linewidth=1.5)
    plt.xlabel("x", fontsize=12)
    plt.ylabel("y", fontsize=12)
    plt.title("Исходный сигнал", fontsize=14)
    plt.grid(True)

    plt.subplot(3, 2, 2)
    plt.plot(freq_arr[:len(x_arr) // 20], np.abs(y_arr_fft_0[:len(x_arr) // 20]), 'r--', linewidth=1.5)
    plt.xlabel("Частота", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.title("Спектр (FFT)", fontsize=14)
    plt.grid(True)

    plt.subplot(3, 2, 3)
    plt.plot(x_arr, y_arr_filtfilt, 'b-', linewidth=1.5)
    plt.xlabel("x", fontsize=12)
    plt.ylabel("y", fontsize=12)
    plt.title("Сигнал после filtfilt", fontsize=14)
    plt.grid(True)

    plt.subplot(3, 2, 4)
    plt.plot(freq_arr[:len(x_arr) // 20], np.abs(y_arr_filtfilt_fft[:len(x_arr) // 20]), 'c-.', linewidth=1.5)
    plt.xlabel("Частота", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.title("Спектр после filtfilt", fontsize=14)
    plt.grid(True)

    plt.subplot(3, 2, 5)
    plt.plot(x_arr, y_arr_fnth.real, 'm-', linewidth=1.5)
    plt.xlabel("x", fontsize=12)
    plt.ylabel("y", fontsize=12)
    plt.title("Сигнал после фильтра 5-го порядка", fontsize=14)
    plt.grid(True)

    plt.subplot(3, 2, 6)
    plt.plot(freq_arr[:len(x_arr) // 20], np.abs(y_arr_fnth_fft[:len(x_arr) // 20]), 'k--', linewidth=1.5)
    plt.xlabel("Частота", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.title("Спектр после фильтра 5-го порядка", fontsize=14)
    plt.grid(True)
    plt.tight_layout()
    plt.show()


# Задача 9: Фильтрация сигнала с шумом
def task_9():
    a, b, cutoff_freq, n = -0.1, 0.1, 120, 5
    w1, w2, w3 = [2 * math.pi * f for f in [50, 150, 450]]
    cos_lab_3_noise = lambda x: math.cos(w1 * x) + math.cos(w2 * x) + math.cos(w3 * x) + 2 * random.random()
    x_arr = x_arr_constructor(a, b, 0.0001)
    y_arr_f0 = function_iterator(cos_lab_3_noise, x_arr)
    freq_arr = np.linspace(1, 1 / (x_arr[1] - x_arr[0]), len(x_arr))
    y_arr_fft_0 = np.fft.fft(y_arr_f0)
    y_arr_fnth = f_nth(x_arr, y_arr_f0, cutoff_freq, n)
    y_arr_fnth_fft = np.fft.fft(y_arr_fnth)
    nq, wcq = scs.butter(n, cutoff_freq * 2 / 10000)
    y_arr_filtfilt = scs.filtfilt(nq, wcq, y_arr_f0)
    y_arr_filtfilt_fft = np.fft.fft(y_arr_filtfilt)

    plt.figure(figsize=(12, 10))
    plt.subplot(3, 2, 1)
    plt.plot(x_arr, y_arr_f0, 'g-', linewidth=1.5)
    plt.xlabel("x", fontsize=12)
    plt.ylabel("y", fontsize=12)
    plt.title("Сигнал с шумом", fontsize=14)
    plt.grid(True)

    plt.subplot(3, 2, 2)
    plt.plot(freq_arr[:len(x_arr) // 20], np.abs(y_arr_fft_0[:len(x_arr) // 20]), 'r--', linewidth=1.5)
    plt.xlabel("Частота", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.title("Спектр сигнала с шумом", fontsize=14)
    plt.grid(True)

    plt.subplot(3, 2, 3)
    plt.plot(x_arr, y_arr_filtfilt, 'b-', linewidth=1.5)
    plt.xlabel("x", fontsize=12)
    plt.ylabel("y", fontsize=12)
    plt.title("Сигнал после filtfilt", fontsize=14)
    plt.grid(True)

    plt.subplot(3, 2, 4)
    plt.plot(freq_arr[:len(x_arr) // 20], np.abs(y_arr_filtfilt_fft[:len(x_arr) // 20]), 'c-.', linewidth=1.5)
    plt.xlabel("Частота", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.title("Спектр после filtfilt", fontsize=14)
    plt.grid(True)

    plt.subplot(3, 2, 5)
    plt.plot(x_arr, y_arr_fnth.real, 'm-', linewidth=1.5)
    plt.xlabel("x", fontsize=12)
    plt.ylabel("y", fontsize=12)
    plt.title("Сигнал после фильтра 5-го порядка", fontsize=14)
    plt.grid(True)

    plt.subplot(3, 2, 6)
    plt.plot(freq_arr[:len(x_arr) // 20], np.abs(y_arr_fnth_fft[:len(x_arr) // 20]), 'k--', linewidth=1.5)
    plt.xlabel("Частота", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.title("Спектр после фильтра 5-го порядка", fontsize=14)
    plt.grid(True)
    plt.tight_layout()
    plt.show()



# Задача 10: Фильтр Чебышева и Баттерворта с сравнением
def task_10():

    def chebyshev_polynomial(n, x):
        if n == 0:
            return 1
        elif n == 1:
            return x
        else:
            return 2 * x * chebyshev_polynomial(n - 1, x) - chebyshev_polynomial(n - 2, x)


    def chebyshev_filter_response(n, epsilon, wc, w):
        Tn = chebyshev_polynomial(n, w / wc)
        H_squared = 1 / (1 + epsilon ** 2 * Tn ** 2)
        return H_squared


    a, b, cutoff_freq, n = -0.1, 0.1, 120, 5
    w1, w2, w3 = [2 * math.pi * f for f in [50, 150, 450]]
    cos_lab_3 = lambda x: math.cos(w1 * x) + math.cos(w2 * x) + math.cos(w3 * x)
    pure_signal = lambda x: math.cos(2 * math.pi * 120 * x)  # Чистый сигнал 120 Гц
    x_arr = np.arange(a, b, 0.0001)
    y_arr_f0 = np.array([cos_lab_3(x) for x in x_arr])
    y_arr_pure = np.array([pure_signal(x) for x in x_arr])
    freq_arr = np.linspace(1, 1 / (x_arr[1] - x_arr[0]), len(x_arr))


    y_arr_fft_0 = np.fft.fft(y_arr_f0)


    epsilon = 0.4
    wc = 2 * math.pi * cutoff_freq
    H_squared_cheb = chebyshev_filter_response(n, epsilon, wc, 2 * math.pi * freq_arr)
    y_arr_cheb_fft = y_arr_fft_0 * np.sqrt(H_squared_cheb)
    y_arr_cheb = np.fft.ifft(y_arr_cheb_fft).real


    nq, wcq = scs.butter(n, cutoff_freq * 2 / 10000)  # Частота нормируется относительно Fs/2
    y_arr_butter = scs.filtfilt(nq, wcq, y_arr_f0)
    y_arr_butter_fft = np.fft.fft(y_arr_butter)


    plt.figure(figsize=(15, 12))


    plt.subplot(3, 2, 1)
    plt.plot(x_arr, y_arr_butter, 'b-', linewidth=1.5)
    plt.xlabel("x", fontsize=12)
    plt.ylabel("y", fontsize=12)
    plt.title("Сигнал после фильтра Баттерворта (n=5)", fontsize=14)
    plt.grid(True)

    plt.subplot(3, 2, 2)
    plt.plot(x_arr, y_arr_cheb, 'g-', linewidth=1.5)
    plt.xlabel("x", fontsize=12)
    plt.ylabel("y", fontsize=12)
    plt.title("Сигнал после фильтра Чебышева (n=5)", fontsize=14)
    plt.grid(True)

    plt.subplot(3, 2, 3)
    plt.plot(x_arr, y_arr_pure, 'r-', linewidth=1.5)
    plt.xlabel("x", fontsize=12)
    plt.ylabel("y", fontsize=12)
    plt.title("Чистый сигнал 120 Гц", fontsize=14)
    plt.grid(True)


    plt.subplot(3, 2, 4)
    plt.plot(x_arr, y_arr_butter, 'b-', linewidth=1.5, label='Баттерворт')
    plt.plot(x_arr, y_arr_cheb, 'g-', linewidth=1.5, label='Чебышев')
    plt.plot(x_arr, y_arr_pure, 'r--', linewidth=1.5, label='120 Гц')
    plt.xlabel("x", fontsize=12)
    plt.ylabel("y", fontsize=12)
    plt.title("Все сигналы вместе", fontsize=14)
    plt.legend()
    plt.grid(True)


    plt.subplot(3, 2, 5)
    plt.plot(freq_arr[:len(x_arr) // 20], np.abs(y_arr_butter_fft[:len(x_arr) // 20]), 'b-', linewidth=1.5, label='Баттерворт')
    plt.plot(freq_arr[:len(x_arr) // 20], np.abs(y_arr_cheb_fft[:len(x_arr) // 20]), 'g-', linewidth=1.5, label='Чебышев')
    plt.plot(freq_arr[:len(x_arr) // 20], np.abs(np.fft.fft(y_arr_pure)[:len(x_arr) // 20]), 'r--', linewidth=1.5, label='120 Гц')
    plt.xlabel("Частота", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.title("Все спектры вместе", fontsize=14)
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.show()



# Выполнение всех задач
#task_1()
#task_2()
#task_3()
#task_4()
#task_5()
#task_6()
#task_8()
#task_9()
task_10()
print("Готово!")