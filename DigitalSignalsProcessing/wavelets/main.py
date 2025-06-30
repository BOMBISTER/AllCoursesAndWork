import librosa
import numpy as np
import matplotlib.pyplot as plt
from scipy.fft import fftfreq, ifft, fft, fftshift
from scipy.signal import convolve
from scipy import fftpack


def task_1():
    alpha = 1.0
    omega0_list = [2, 5, 10]
    t = np.linspace(-5, 5, 1000)

    test_signal = np.sin(5 * t)

    plt.figure(figsize=(18, 12))

    for i, omega0 in enumerate(omega0_list):
        psi_t = np.exp(-t ** 2 / alpha ** 2) * np.exp(1j * omega0 * t)  # Вейвлет Морле

        plt.subplot(3, 3, i + 1)
        plt.plot(t, np.real(psi_t), label='Real part')
        #plt.plot(t, np.imag(psi_t), label='Imag part')
        plt.title(f'Вейвлет Морле, ω0={omega0}')
        plt.xlabel('Время (t)')
        plt.ylabel('Амплитуда')
        plt.legend()
        plt.grid()

    N = len(t)
    dt = t[1] - t[0]
    freq = fftfreq(N, dt)
    freq = fftshift(freq)

    for i, omega0 in enumerate(omega0_list):
        psi_omega = alpha * np.sqrt(np.pi) * np.exp(
            -alpha ** 2 * (freq - omega0) ** 2 / 4)  # Вейвлет в частотном пространстве

        plt.subplot(3, 3, i + 4)
        plt.plot(freq, np.abs(psi_omega))
        plt.title(f'Спектр вейвлета, ω0={omega0}')
        plt.xlabel('Частота (ω)')
        plt.ylabel('Амплитуда')
        plt.xlim([-15, 15])
        plt.grid()

    omega0 = 5
    morlet = np.exp(-t ** 2 / alpha ** 2) * np.exp(1j * omega0 * t)

    signal_fft = fft(test_signal)
    morlet_fft = fft(morlet)
    conv_result = np.fft.ifft(signal_fft * morlet_fft)

    test_signal_spectrum = np.abs(fftshift(fft(test_signal)))
    conv_result_spectrum = np.abs(fftshift(fft(conv_result)))

    plt.subplot(3, 3, 7)
    plt.plot(t, test_signal)
    plt.title('Исходный сигнал (5 Гц)')
    plt.xlabel('Время')
    plt.ylabel('Амплитуда')
    plt.grid()

    plt.subplot(3, 3, 8)
    plt.plot(t, np.real(conv_result), label='Real part')
    #plt.plot(t, np.imag(conv_result), label='Imag part')
    plt.title('Результат свертки с вейвлетом')
    plt.xlabel('Время')
    plt.ylabel('Амплитуда')
    plt.legend()
    plt.grid()

    plt.subplot(3, 3, 9)
    plt.plot(freq, test_signal_spectrum, label='Спектр сигнала')
    plt.plot(freq, conv_result_spectrum / 2000, label='Спектр свертки', linestyle='--')
    plt.title('Сравнение спектров')
    plt.xlabel('Частота (Гц)')
    plt.ylabel('Амплитуда')
    plt.xlim([-15, 15])
    plt.legend()
    plt.grid()

    plt.tight_layout()
    plt.show()


def task_2():
    sigma = 0.5
    t0 = 0
    omega0_list = [1, 2, 10]
    t = np.linspace(-5, 5, 1000)

    test_signal = np.sin(5 * t)

    plt.figure(figsize=(18, 12))

    for i, omega0 in enumerate(omega0_list):
        psi_t = (1 - (t - t0) ** 2 / sigma ** 2) * np.exp(-(t - t0) ** 2 / (2 * sigma ** 2)) * np.exp(1j * omega0 * t0)

        plt.subplot(3, 3, i + 1)
        plt.plot(t, np.real(psi_t), label='Real part')
        #plt.plot(t, np.imag(psi_t), label='Imag part')
        plt.title(f'Mexican Hat, ω0={omega0}, σ={sigma}')
        plt.xlabel('Время (t)')
        plt.ylabel('Амплитуда')
        plt.legend()
        plt.grid()

    N = len(t)
    dt = t[1] - t[0]
    freq = fftfreq(N, dt)
    freq = fftshift(freq)

    for i, omega0 in enumerate(omega0_list):
        psi_omega = (freq - omega0) ** 2 * np.exp(-(freq - omega0) ** 2 / sigma ** 2)

        plt.subplot(3, 3, i + 4)
        plt.plot(freq, np.abs(psi_omega))
        plt.title(f'Спектр Mexican Hat, ω0={omega0}')
        plt.xlabel('Частота (ω)')
        plt.ylabel('Амплитуда')
        plt.xlim([-15, 15])
        plt.grid()

    omega0 = 5
    mexhat = (1 - (t - t0) ** 2 / sigma ** 2) * np.exp(-(t - t0) ** 2 / (2 * sigma ** 2)) * np.exp(1j * omega0 * t0)

    signal_fft = fft(test_signal)
    mexhat_fft = fft(mexhat)
    conv_result = np.fft.ifft(signal_fft * mexhat_fft)

    test_signal_spectrum = np.abs(fftshift(fft(test_signal)))
    conv_result_spectrum = np.abs(fftshift(fft(conv_result)))

    plt.subplot(3, 3, 7)
    plt.plot(t, test_signal)
    plt.title('Исходный сигнал (5 Гц)')
    plt.xlabel('Время')
    plt.ylabel('Амплитуда')
    plt.grid()

    plt.subplot(3, 3, 8)
    plt.plot(t, np.real(conv_result), label='Real part')
    #plt.plot(t, np.imag(conv_result), label='Imag part')
    plt.title('Результат свертки с Mexican Hat')
    plt.xlabel('Время')
    plt.ylabel('Амплитуда')
    plt.legend()
    plt.grid()

    plt.subplot(3, 3, 9)
    plt.plot(freq, test_signal_spectrum, label='Спектр сигнала')
    plt.plot(freq, conv_result_spectrum, label='Спектр свертки', linestyle='--')
    plt.title('Сравнение спектров')
    plt.xlabel('Частота (Гц)')
    plt.ylabel('Амплитуда')
    plt.xlim([-15, 15])
    plt.legend()
    plt.grid()

    plt.tight_layout()
    plt.show()


def task_3():
    t = np.linspace(-1, 2, 2000)
    a_values = [0.5, 1.0, 2.0]

    test_signal = np.sin(5 * t)

    def haar_wavelet(t):
        return np.where((0 <= t) & (t < 0.5), 1, np.where((0.5 <= t) & (t < 1), -1, 0))

    def scaled_haar(t, a):
        return (1 / np.sqrt(a)) * haar_wavelet(t / a)

    # def haar_freq(omega, a):
    #     return 2j * np.sin(a * omega / 2) * np.exp(-1j * a * omega / 2) * np.sqrt(a)

    plt.figure(figsize=(18, 12))

    for i, a in enumerate(a_values):
        psi_t = scaled_haar(t, a)

        plt.subplot(3, 3, i + 1)
        plt.plot(t, psi_t)
        plt.title(f'Вейвлет Хаара (a={a})')
        plt.xlabel('Время')
        plt.ylabel('Амплитуда')
        plt.grid(True)

        N = len(t)
        dt = t[1] - t[0]
        psi_omega = fft(psi_t)
        freq = fftfreq(N, dt)
        omega = 2 * np.pi * freq

        plt.subplot(3, 3, i + 4)
        plt.plot(fftshift(freq), np.abs(fftshift(psi_omega)))
        plt.title(f'Спектр Хаара (a={a})')
        plt.xlabel('Частота (Гц)')
        plt.ylabel('Амплитуда')
        plt.xlim(-10, 10)
        plt.grid(True)

    a = 0.7
    haar = scaled_haar(t, a)

    signal_fft = fft(test_signal)
    haar_fft = fft(haar)
    conv_result = np.fft.ifft(signal_fft * haar_fft)

    test_signal_spectrum = np.abs(fftshift(fft(test_signal)))
    conv_result_spectrum = np.abs(fftshift(fft(conv_result)))
    freq = fftshift(fftfreq(len(t), t[1] - t[0]))

    plt.subplot(3, 3, 7)
    plt.plot(t, test_signal)
    plt.title('Исходный сигнал (5 Гц)')
    plt.xlabel('Время')
    plt.ylabel('Амплитуда')
    plt.grid()

    plt.subplot(3, 3, 8)
    plt.plot(t, np.real(conv_result))
    plt.title('Результат свертки с Хааром')
    plt.xlabel('Время')
    plt.ylabel('Амплитуда')
    plt.grid()

    plt.subplot(3, 3, 9)
    plt.plot(freq, test_signal_spectrum, label='Спектр сигнала')
    plt.plot(freq, conv_result_spectrum / 1000, label='Спектр свертки', linestyle='--')
    plt.title('Сравнение спектров')
    plt.xlabel('Частота (Гц)')
    plt.ylabel('Амплитуда')
    plt.xlim(-15, 15)
    plt.legend()
    plt.grid()

    plt.tight_layout()
    plt.show()


def task_4():
    np.random.seed(42)
    t = np.linspace(-5, 5, 1000)
    signal = np.sin(5 * t) + 0.3 * np.random.randn(len(t))
    signal_5 = np.sin(5 * t)

    def normalize_l2(wavelet):
        """L2-нормировка вейвлета (по энергии)"""
        return wavelet / np.sqrt(np.sum(np.abs(wavelet) ** 2))

    def morlet_wavelet(t, omega0=5, alpha=1):
        wavelet = np.exp(-t ** 2 / alpha ** 2) * np.exp(1j * omega0 * t)
        return normalize_l2(wavelet)

    def mexican_hat(t, sigma=0.5, t0=0):
        wavelet = (1 - ((t - t0) / sigma) ** 2) * np.exp(-((t - t0) / sigma) ** 2 / 2)
        return normalize_l2(wavelet)

    def haar_wavelet(t):
        a = 0.3
        wavelet = np.where((0 <= t) & (t < a), 1, np.where((a <= t) & (t < 1), -1, 0))
        return normalize_l2(wavelet)

    t_wavelet = np.linspace(-5, 5, 1000)

    # Создаем и нормируем вейвлеты
    morlet = morlet_wavelet(t_wavelet)
    mexhat = mexican_hat(t_wavelet)
    haar = haar_wavelet(t_wavelet)

    # Вычисляем свертки
    conv_morlet = convolve(signal, np.real(morlet), mode='same')
    conv_mexhat = convolve(signal, mexhat, mode='same')
    conv_haar = convolve(signal, haar, mode='same')

    def compute_positive_spectrum(signal):
        n = len(signal)
        freq = fftfreq(n, d=t[1] - t[0])
        spectrum = fft(signal)
        # Берем только положительные частоты
        positive_mask = freq >= 0
        return freq[positive_mask], np.abs(spectrum)[positive_mask]

    # Вычисляем спектры только для положительных частот
    freq_signal, spectrum_signal = compute_positive_spectrum(signal)
    freq_signal_5, spectrum_signal_5 = compute_positive_spectrum(signal_5)

    signal_fft = fft(signal)
    freq_pos = fftfreq(len(t), t[1] - t[0])
    pos_mask = freq_pos >= 0

    plt.figure(figsize=(15, 12))

    # Исходный сигнал
    plt.subplot(4, 2, 1)
    plt.plot(t, signal)
    plt.title('Исходный сигнал с шумом')
    plt.xlabel('Время (с)')
    plt.ylabel('Амплитуда')
    plt.grid(True)

    # Спектр сигнала (только положительные частоты)
    plt.subplot(4, 2, 2)
    plt.plot(freq_signal, spectrum_signal, label='С шумом')
    plt.plot(freq_signal_5, spectrum_signal_5, label='Чистый')
    plt.title('Спектры сигналов (положительные частоты)')
    plt.xlabel('Частота (Гц)')
    plt.ylabel('Амплитуда')
    plt.xlim(0, 10)  # Только положительная область
    plt.legend()
    plt.grid(True)

    # Морле
    morlet_fft = fft(morlet)
    conv_result = np.fft.ifft(signal_fft * morlet_fft)
    conv_freq, conv_spectrum = compute_positive_spectrum(conv_result)

    plt.subplot(4, 2, 3)
    plt.plot(t, conv_morlet, label='Свертка')
    plt.plot(t, signal, "red", alpha=0.5, label='Сигнал')
    plt.title('Свертка с вейвлетом Морле (L2)')
    plt.xlabel('Время (с)')
    plt.ylabel('Амплитуда')
    plt.legend()
    plt.grid(True)

    plt.subplot(4, 2, 4)
    plt.plot(conv_freq, conv_spectrum, label='Спектр свертки', linestyle='--')
    plt.plot(freq_signal, spectrum_signal, "red", label='Спектр сигнала')
    plt.title('Спектр вейвлета Морле (положительные частоты)')
    plt.xlabel('Частота (Гц)')
    plt.ylabel('Амплитуда')
    plt.xlim(0, 10)
    plt.legend()
    plt.grid(True)

    # Мексиканская шляпа
    mexhat_fft = fft(mexhat)
    conv_result = np.fft.ifft(signal_fft * mexhat_fft)
    conv_freq, conv_spectrum = compute_positive_spectrum(conv_result)

    plt.subplot(4, 2, 5)
    plt.plot(t, conv_mexhat, color='orange', label='Свертка')
    plt.plot(t, signal, "red", alpha=0.5, label='Сигнал')
    plt.title('Свертка с мексиканской шляпой (L2)')
    plt.xlabel('Время (с)')
    plt.ylabel('Амплитуда')
    plt.legend()
    plt.grid(True)

    plt.subplot(4, 2, 6)
    plt.plot(conv_freq, conv_spectrum, color='blue', label='Спектр свертки')
    plt.plot(freq_signal, spectrum_signal, "red", label='Спектр сигнала')
    plt.title('Спектр мексиканской шляпы (положительные частоты)')
    plt.xlabel('Частота (Гц)')
    plt.ylabel('Амплитуда')
    plt.xlim(0, 10)
    plt.legend()
    plt.grid(True)

    # Хаар
    haar_fft = fft(haar)
    conv_result = np.fft.ifft(signal_fft * haar_fft)
    conv_freq, conv_spectrum = compute_positive_spectrum(conv_result)

    plt.subplot(4, 2, 7)
    plt.plot(t, conv_haar, color='green', label='Свертка')
    plt.plot(t, signal, "red", alpha=0.5, label='Сигнал')
    plt.title('Свертка с вейвлетом Хаара (L2)')
    plt.xlabel('Время (с)')
    plt.ylabel('Амплитуда')
    plt.legend()
    plt.grid(True)

    plt.subplot(4, 2, 8)
    plt.plot(conv_freq, conv_spectrum, color='blue', label='Спектр свертки')
    plt.plot(freq_signal, spectrum_signal, "red", label='Спектр сигнала')
    plt.title('Спектр вейвлета Хаара (положительные частоты)')
    plt.xlabel('Частота (Гц)')
    plt.ylabel('Амплитуда')
    plt.xlim(0, 10)
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.show()


def task_5():
    audio_data, sr = librosa.load("D_296Hz.mp3", sr=None, mono=True)
    n_fft = 2048  # Размер окна для FFT
    t_wavelet = np.linspace(-5, 5, 1000)

    def normalize_l2(wavelet):
        """L2-нормировка вейвлета (по энергии)"""
        return wavelet / np.sqrt(np.sum(np.abs(wavelet) ** 2))

    def morlet_wavelet(t, omega0=5, alpha=1):
        wavelet = np.exp(-t ** 2 / alpha ** 2) * np.exp(1j * omega0 * t)
        return normalize_l2(wavelet)

    def mexican_hat(t, sigma=0.5, t0=0):
        wavelet = (1 - ((t - t0) / sigma) ** 2) * np.exp(-((t - t0) / sigma) ** 2 / 2)
        return normalize_l2(wavelet)

    def haar_wavelet(t):
        a = 0.3
        wavelet = np.where((0 <= t) & (t < a), 1, np.where((a <= t) & (t < 1), -1, 0))
        return normalize_l2(wavelet)

    # Создаем и нормируем вейвлеты
    morlet = morlet_wavelet(np.array([(1 / sr) * k for k in range(len(audio_data))]))
    mexhat = mexican_hat(t_wavelet)
    haar = haar_wavelet(t_wavelet)

    # Вычисляем свертки
    conv_morlet = convolve(audio_data, np.real(morlet), mode='same')
    conv_mexhat = convolve(audio_data, mexhat, mode='same')
    conv_haar = convolve(audio_data, haar, mode='same')

    plt.subplot(2, 2, 1)
    plt.specgram(audio_data, n_fft, pad_to=1000, mode='magnitude', Fs=sr)
    plt.title("Исходный файл")
    plt.grid(True)

    plt.subplot(2, 2, 2)
    plt.specgram(conv_morlet, n_fft, pad_to=1000, mode='magnitude', Fs=sr)
    plt.grid(True)

    plt.subplot(2, 2, 3)
    plt.specgram(conv_mexhat, n_fft, pad_to=1000, mode='magnitude', Fs=sr)
    plt.grid(True)

    plt.subplot(2, 2, 4)
    plt.specgram(conv_haar, n_fft, pad_to=1000, mode='magnitude', Fs=sr)
    plt.grid(True)

    plt.show()


task_1()
task_2()
task_3()
task_4()
task_5()

print('End!')