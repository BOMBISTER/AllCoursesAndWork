import numpy as np
import matplotlib.pyplot as plt
import scipy.signal as signal



def task_1():
    A = 1.0
    pulse_width = 0.2
    period = 1.0
    t = np.linspace(0, 5, 1000)

    signal = A * (np.mod(t, period) < pulse_width).astype(float)

    x_kernel = np.linspace(-2, 2, 100)
    gaussian_kernel_values = A * np.exp(-x_kernel ** 2)
    linear_kernel_values = x_kernel

    convolved_gaussian = np.convolve(signal, gaussian_kernel_values, mode='same')
    convolved_linear_np = np.convolve(signal, linear_kernel_values, mode='same')
    convolved_linear_custom = custom_convolution(signal, linear_kernel_values)

    # Урезаем результат самописной свертки до длины сигнала для сравнения
    start_idx = (len(convolved_linear_custom) - len(signal)) // 2
    convolved_linear_custom = convolved_linear_custom[start_idx:start_idx + len(signal)]

    plt.figure(figsize=(14, 10))

    plt.subplot(3, 1, 1)
    plt.plot(t, signal, label="Прямоугольный сигнал", color='dodgerblue', linewidth=2)
    plt.xlabel("Время", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.title("Исходный сигнал", fontsize=14)
    plt.legend(fontsize=10)

    plt.subplot(3, 1, 2)
    plt.plot(t, convolved_gaussian, label="Свертка с гауссовым ядром", color='darkorange', linewidth=2)
    plt.xlabel("Время", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.title("Свертка с гауссовым ядром", fontsize=14)
    plt.legend(fontsize=10)

    plt.subplot(3, 1, 3)
    plt.plot(t, convolved_linear_np, label="Свертка с линейным ядром (np.convolve)", color='forestgreen', linewidth=2)
    plt.plot(t, convolved_linear_custom, label="Свертка с линейным ядром (custom)", color='crimson', linestyle='--', linewidth=2)
    plt.xlabel("Время", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.title("Сравнение свертки с линейным ядром", fontsize=14)
    plt.legend(fontsize=10)

    plt.tight_layout()
    plt.show()

def task_2():
    signal = np.array([0, 0, 1, 1, 1, 0, 0])
    kernel = np.array([0.5, 1, 0.5])
    convolved_signal = custom_convolution(signal, kernel)

    plt.figure(figsize=(12, 8))

    plt.subplot(3, 1, 1)
    plt.plot(signal, label="Исходный сигнал", color='dodgerblue', linewidth=2, marker='o')
    plt.title("Исходный сигнал", fontsize=14)
    plt.xlabel("Время", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.legend(fontsize=10)

    plt.subplot(3, 1, 2)
    plt.plot(kernel, label="Ядро свертки", color='forestgreen', linewidth=2, marker='o')
    plt.title("Ядро свертки", fontsize=14)
    plt.xlabel("Время", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.legend(fontsize=10)

    plt.subplot(3, 1, 3)
    plt.plot(convolved_signal, label="Результат свертки", color='crimson', linewidth=2, marker='o')
    plt.title("Результат свертки", fontsize=14)
    plt.xlabel("Время", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.legend(fontsize=10)

    plt.tight_layout()
    plt.show()



def custom_convolution(signal, kernel):
    signal_length = len(signal)
    kernel_length = len(kernel)
    result_length = signal_length + kernel_length - 1
    result = np.zeros(result_length)

    for n in range(result_length):
        for k in range(kernel_length):
            if n - k >= 0 and n - k < signal_length:
                result[n] += signal[n - k] * kernel[k]

    return result



def check_convolution_theorem(signal, kernel):
    conv_time = custom_convolution(signal, kernel)
    fft_signal = np.fft.fft(signal, n=len(conv_time))
    fft_kernel = np.fft.fft(kernel, n=len(conv_time))
    fft_result = fft_signal * fft_kernel
    conv_freq = np.fft.ifft(fft_result).real

    # Частотная ось для спектров
    freq = np.fft.fftfreq(len(conv_time))

    plt.figure(figsize=(12, 10))

    plt.subplot(4, 1, 1)
    plt.plot(conv_time, label="Свертка во временной области", color='dodgerblue', linewidth=2)
    plt.title("Свертка во временной области", fontsize=14)
    plt.xlabel("Время", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.legend(fontsize=10)

    plt.subplot(4, 1, 2)
    plt.plot(freq[:len(freq) // 2], np.abs(np.fft.fft(conv_time)[:len(freq) // 2]),
             label="Спектр свертки (временная область)", color='darkorange', linewidth=2)
    plt.title("Спектр свертки (временная область)", fontsize=14)
    plt.xlabel("Частота", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.legend(fontsize=10)

    plt.subplot(4, 1, 3)
    plt.plot(conv_freq, label="Свертка через частотную область", color='crimson', linestyle='--', linewidth=2)
    plt.title("Свертка через частотную область", fontsize=14)
    plt.xlabel("Время", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.legend(fontsize=10)

    plt.subplot(4, 1, 4)
    plt.plot(freq[:len(freq) // 2], np.abs(np.fft.fft(conv_freq)[:len(freq) // 2]),
             label="Спектр свертки (частотная область)", color='forestgreen', linewidth=2)
    plt.title("Спектр свертки (частотная область)", fontsize=14)
    plt.xlabel("Частота", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.legend(fontsize=10)

    plt.tight_layout()
    plt.show()

    if np.allclose(conv_time, conv_freq, atol=1e-10):
        print("Результаты совпадают: теорема о свертке подтверждена!")
    else:
        print("Результаты не совпадают: что-то пошло не так.")



def task_3():
    signal = np.array([0, 0, 1, 1, 1, 0, 0])
    kernel = np.array([0.5, 1, 0.5])
    check_convolution_theorem(signal, kernel)



def task_5():
    fs = 1000
    t = np.linspace(0, 1, fs, endpoint=False)
    frequencies = np.fft.fftfreq(len(t), d=1 / fs)

    signal = (np.sin(2 * np.pi * 50 * t) +
              np.sin(2 * np.pi * 120 * t) +
              np.sin(2 * np.pi * 200 * t))
    signal_120hz = np.sin(2 * np.pi * 120 * t)  # Чистый сигнал 120 Гц

    def gaussian_kernel(frequencies, peak_freq, fwhm):
        sigma = fwhm / (2 * np.sqrt(2 * np.log(2)))
        gaussian = np.exp(-((frequencies - peak_freq) ** 2) / (2 * sigma ** 2))
        return gaussian

    def apply_gaussian_filter(signal, peak_freq, fwhm):
        fft_signal = np.fft.fft(signal)
        gaussian = gaussian_kernel(frequencies, peak_freq, fwhm)
        fft_filtered = fft_signal * gaussian
        filtered_signal = np.fft.ifft(fft_filtered).real
        return filtered_signal, gaussian

    filtered_signal_narrowband, gaussian_narrowband = apply_gaussian_filter(signal, peak_freq=120, fwhm=10)
    filtered_signal_lowpass, gaussian_lowpass = apply_gaussian_filter(signal, peak_freq=0, fwhm=100)

    plt.figure(figsize=(14, 10))

    plt.subplot(3, 2, 1)
    plt.plot(t, signal, color='dodgerblue', linewidth=2)
    plt.title("Исходный сигнал", fontsize=14)
    plt.xlabel("Время (с)", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)

    plt.subplot(3, 2, 2)
    plt.plot(frequencies[:len(frequencies) // 2], np.abs(np.fft.fft(signal)[:len(frequencies) // 2]),
             color='dodgerblue', linewidth=2)
    plt.title("Спектр исходного сигнала", fontsize=14)
    plt.xlabel("Частота (Гц)", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)

    plt.subplot(3, 2, 3)
    plt.plot(frequencies[:len(frequencies) // 2], gaussian_narrowband[:len(frequencies) // 2], color='darkorange',
             linewidth=2)
    plt.title("Гауссово ядро (узкополосный фильтр)", fontsize=14)
    plt.xlabel("Частота (Гц)", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)

    plt.subplot(3, 2, 4)
    plt.plot(t, filtered_signal_narrowband, color='darkorange', linewidth=2, label="Отфильтрованный сигнал")
    plt.plot(t, signal_120hz, color='gray', linestyle='--', linewidth=1.5, label="Сигнал 120 Гц")
    plt.title("Отфильтрованный сигнал (узкополосный)", fontsize=14)
    plt.xlabel("Время (с)", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.legend(fontsize=10)

    plt.subplot(3, 2, 5)
    plt.plot(frequencies[:len(frequencies) // 2], gaussian_lowpass[:len(frequencies) // 2], color='forestgreen',
             linewidth=2)
    plt.title("Гауссово ядро (низкочастотный фильтр)", fontsize=14)
    plt.xlabel("Частота (Гц)", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)

    plt.subplot(3, 2, 6)
    plt.plot(t, filtered_signal_lowpass, color='forestgreen', linewidth=2, label="Отфильтрованный сигнал")
    plt.plot(t, signal_120hz, color='gray', linestyle='--', linewidth=1.5, label="Сигнал 120 Гц")
    plt.title("Отфильтрованный сигнал (низкочастотный)", fontsize=14)
    plt.xlabel("Время (с)", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.legend(fontsize=10)

    plt.tight_layout()
    plt.show()



def planck_taper(N, epsilon=0.1):

    n = np.arange(N)
    window = 1 / (1 + np.exp((2 * epsilon * N / (n - (N - 1) / 2)) - (2 * epsilon * N / (N - 1))))
    return window



def bandpass_filter(signal, fs, lowcut, highcut, epsilon=0.1):
    fft_signal = np.fft.fft(signal)
    frequencies = np.fft.fftfreq(len(signal), d=1 / fs)
    N = len(signal)
    window = np.zeros(N)
    # Определяем индексы для частотного диапазона
    freq_mask = (np.abs(frequencies) >= lowcut) & (np.abs(frequencies) <= highcut)
    window[freq_mask] = 1
    # Применяем окно Планка
    planck_window = planck_taper(N, epsilon)
    window = window * planck_window
    fft_filtered = fft_signal * window
    filtered_signal = np.fft.ifft(fft_filtered).real
    return filtered_signal, window



def task_6():
    fs = 1000
    t = np.linspace(0, 1, fs, endpoint=False)
    signal = (np.sin(2 * np.pi * 50 * t) +
              np.sin(2 * np.pi * 120 * t) +
              np.sin(2 * np.pi * 200 * t))
    signal_120hz = np.sin(2 * np.pi * 120 * t)  # Чистый сигнал 120 Гц
    lowcut = 115
    highcut = 125
    filtered_signal, window = bandpass_filter(signal, fs, lowcut, highcut, epsilon=0.1)

    frequencies = np.fft.fftfreq(len(t), d=1 / fs)

    plt.figure(figsize=(14, 10))

    plt.subplot(3, 1, 1)
    plt.plot(t, signal, color='dodgerblue', linewidth=2)
    plt.title("Исходный сигнал", fontsize=14)
    plt.xlabel("Время (с)", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)

    plt.subplot(3, 1, 2)
    plt.plot(frequencies[:len(frequencies) // 2], window[:len(frequencies) // 2], color='darkorange', linewidth=2)
    plt.title("Частотная характеристика фильтра (окно Планка)", fontsize=14)
    plt.xlabel("Частота (Гц)", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)

    plt.subplot(3, 1, 3)
    plt.plot(t, filtered_signal, color='forestgreen', linewidth=2, label="Отфильтрованный сигнал")
    plt.plot(t, signal_120hz, color='gray', linestyle='--', linewidth=1.5, label="Сигнал 120 Гц")
    plt.title("Отфильтрованный сигнал", fontsize=14)
    plt.xlabel("Время (с)", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.legend(fontsize=10)

    plt.tight_layout()
    plt.show()
def task_7():
    fs = 1000
    t = np.linspace(0, 1, fs, endpoint=False)
    signal = (np.sin(2 * np.pi * 50 * t) +
              np.sin(2 * np.pi * 120 * t) +
              np.sin(2 * np.pi * 200 * t))
    signal_120hz = np.sin(2 * np.pi * 120 * t)  # Чистый сигнал 120 Гц
    noise = 0.5 * np.random.normal(size=len(t))
    noisy_signal = signal + noise

    def gaussian_kernel(frequencies, peak_freq, fwhm):
        s = (fwhm * (2 * np.pi - 1)) / (4 * np.pi)
        gaussian = np.exp(-5 * ((frequencies - peak_freq) / s) ** 2)
        return gaussian

    def apply_gaussian_filter(signal, peak_freq, fwhm):
        fft_signal = np.fft.fft(signal)
        frequencies = np.fft.fftfreq(len(signal), d=1 / fs)
        gaussian = gaussian_kernel(frequencies, peak_freq, fwhm)
        fft_filtered = fft_signal * gaussian
        filtered_signal = np.fft.ifft(fft_filtered).real
        return filtered_signal, gaussian

    filtered_signal_gaussian, gaussian_narrowband = apply_gaussian_filter(noisy_signal, peak_freq=120, fwhm=10)
    lowcut = 115
    highcut = 125
    filtered_signal_planck, window = bandpass_filter(noisy_signal, fs, lowcut, highcut, epsilon=0.1)

    frequencies = np.fft.fftfreq(len(t), d=1 / fs)
    fft_noisy = np.fft.fft(noisy_signal)
    fft_gaussian = np.fft.fft(filtered_signal_gaussian)
    fft_planck = np.fft.fft(filtered_signal_planck)

    plt.figure(figsize=(14, 12))

    plt.subplot(4, 2, 1)
    plt.plot(t, noisy_signal, color='dodgerblue', linewidth=1)
    plt.title("Зашумленный сигнал", fontsize=14)
    plt.xlabel("Время (с)", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)

    plt.subplot(4, 2, 2)
    plt.plot(frequencies[:len(frequencies) // 2], np.abs(fft_noisy[:len(frequencies) // 2]),
             color='dodgerblue', linewidth=2)
    plt.title("Спектр зашумленного сигнала", fontsize=14)
    plt.xlabel("Частота (Гц)", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)

    plt.subplot(4, 2, 3)
    plt.plot(t, filtered_signal_gaussian, color='darkorange', linewidth=2, label="Отфильтрованный сигнал")
    plt.plot(t, signal_120hz, color='gray', linestyle='--', linewidth=1.5, label="Сигнал 120 Гц")
    plt.title("Отфильтрованный сигнал (гауссов фильтр)", fontsize=14)
    plt.xlabel("Время (с)", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.legend(fontsize=10)

    plt.subplot(4, 2, 4)
    plt.plot(frequencies[:len(frequencies) // 2], np.abs(fft_gaussian[:len(frequencies) // 2]),
             color='darkorange', linewidth=2)
    plt.title("Спектр (гауссов фильтр)", fontsize=14)
    plt.xlabel("Частота (Гц)", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)

    plt.subplot(4, 2, 5)
    plt.plot(t, filtered_signal_planck, color='forestgreen', linewidth=2, label="Отфильтрованный сигнал")
    plt.plot(t, signal_120hz, color='gray', linestyle='--', linewidth=1.5, label="Сигнал 120 Гц")
    plt.title("Отфильтрованный сигнал (окно Планка)", fontsize=14)
    plt.xlabel("Время (с)", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)
    plt.legend(fontsize=10)

    plt.subplot(4, 2, 6)
    plt.plot(frequencies[:len(frequencies) // 2], np.abs(fft_planck[:len(frequencies) // 2]),
             color='forestgreen', linewidth=2)
    plt.title("Спектр (окно Планка)", fontsize=14)
    plt.xlabel("Частота (Гц)", fontsize=12)
    plt.ylabel("Амплитуда", fontsize=12)

    plt.tight_layout()
    plt.show()


# Выполнение задач
task_1()
#task_2()
#task_3()
task_5()
task_6()
task_7()