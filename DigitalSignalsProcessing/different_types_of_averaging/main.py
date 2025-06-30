import numpy as np
import matplotlib.pyplot as plt
from scipy.fft import fft, fftfreq
from scipy.signal import medfilt

def task_1():
    N = 1000
    t = np.linspace(0, 1, N)
    signal_freq = 5
    clean_signal = np.sin(2 * np.pi * signal_freq * t)
    noise_level = 0.5
    noisy_signal = clean_signal + np.random.normal(0, noise_level, N)

    def moving_average(signal, k):
        averaged = np.zeros_like(signal)
        for i in range(len(signal)):
            start = max(0, i - k)
            end = min(len(signal), i + k + 1)
            averaged[i] = np.mean(signal[start:end])
        return averaged

    k = 5
    averaged_signal = moving_average(noisy_signal, k)

    def compute_spectrum(signal, N):
        yf = fft(signal)
        xf = fftfreq(N, 1/N) #генерирует массив частот, соответствующих результату FFT     Результат включает как положительные, так и отрицательные частоты
        return xf[:N//2], 2/N * np.abs(yf[0:N//2]) # только положительные частоты (симметричная часть спектра отбрасывается),  2/N это нормировка чтоб не зависило от длины N

    freq_noisy, spectrum_noisy = compute_spectrum(noisy_signal, N)
    freq_avg, spectrum_avg = compute_spectrum(averaged_signal, N)

    plt.figure(figsize=(12, 8))
    plt.subplot(2, 2, 1)
    plt.plot(t, noisy_signal, label='Зашумленный сигнал', alpha=0.7)
    plt.plot(t, averaged_signal, label='Усредненный сигнал', color='red')
    plt.title('Сигналы во временной области')
    plt.xlabel('Время')
    plt.ylabel('Амплитуда')
    plt.legend()
    plt.subplot(2, 2, 2)
    plt.plot(freq_noisy, spectrum_noisy, label='Зашумленный сигнал', alpha=0.7)
    plt.plot(freq_avg, spectrum_avg, label='Усредненный сигнал', color='red')
    plt.title('Спектры сигналов')
    plt.xlabel('Частота (Гц)')
    plt.ylabel('Амплитуда')
    plt.xlim(0, 50)
    plt.legend()
    plt.subplot(2, 2, 3)
    plt.plot(t[100:200], noisy_signal[100:200], label='Зашумленный сигнал', alpha=0.7)
    plt.plot(t[100:200], averaged_signal[100:200], label='Усредненный сигнал', color='red')
    plt.title('Увеличенный участок сигналов')
    plt.xlabel('Время')
    plt.ylabel('Амплитуда')
    plt.legend()
    plt.tight_layout()
    plt.show()

task_1()

def task_2():
    N = 1000
    t = np.linspace(0, 1, N)
    signal_freq1 = 5
    signal_freq2 = 20
    clean_signal = 0.5 * np.sin(2 * np.pi * signal_freq1 * t) + 0.2 * np.sin(2 * np.pi * signal_freq2 * t)
    noise_level = 0.3
    noisy_signal = clean_signal + np.random.normal(0, noise_level, N)

    def moving_average(signal, k):
        averaged = np.zeros_like(signal)
        for i in range(len(signal)):
            start = max(0, i - k)
            end = min(len(signal), i + k + 1)
            averaged[i] = np.mean(signal[start:end])
        return averaged

    def gaussian_smoothing(signal, k, omega=5):
        indices = np.arange(-k, k + 1)   #  создаёт массив с равномерно распределёнными значениями в заданном диапазоне, шаг 1 (по умолчанию)   2k + 1-длина окна
        gaussian_kernel = np.exp(-4 * np.log(2) * indices ** 2 / omega ** 2)
        gaussian_kernel /= np.sum(gaussian_kernel)  # гарантирует, что сумма коэффициентов ядра равна 1 (чтобы не изменялась общая энергия сигнала).

        smoothed = np.zeros_like(signal)  # Создаем пустой массив для сглаженного сигнала
        for i in range(len(signal)):
            start = max(0, i - k) # start: начало окна с учетом границ (не может быть < 0)
            end = min(len(signal), i + k + 1)
            kernel_start = max(0, k - i)  #  смещение начала ядра при приближении к левой границе
            kernel_end = min(2 * k + 1, k + (len(signal) - i))
            smoothed[i] = np.sum(signal[start:end] * gaussian_kernel[kernel_start:kernel_end]) # Вырезается окно сигнала signal[start:end] Вырезается соответствующая часть ядра gaussian_kernel[kernel_start:kernel_end]Вычисляется их поэлементное произведение и сумма
        return smoothed

    k = 15
    omega = 10
    ma_signal = moving_average(noisy_signal, k)
    gs_signal = gaussian_smoothing(noisy_signal, k, omega)

    def compute_spectrum(signal, N):
        yf = fft(signal)
        xf = fftfreq(N, 1 / N)
        return xf[:N // 2], 2 / N * np.abs(yf[0:N // 2])

    freq_noisy, spectrum_noisy = compute_spectrum(noisy_signal, N)
    freq_ma, spectrum_ma = compute_spectrum(ma_signal, N)
    freq_gs, spectrum_gs = compute_spectrum(gs_signal, N)

    plt.figure(figsize=(15, 10))
    plt.subplot(2, 2, 1)
    plt.plot(t, noisy_signal, label='Зашумленный сигнал', alpha=0.5)
    plt.plot(t, ma_signal, label='Скользящее среднее', linewidth=2)
    plt.plot(t, gs_signal, label='Гауссово усреднение', linewidth=2)
    plt.title('Сравнение методов усреднения')
    plt.xlabel('Время')
    plt.ylabel('Амплитуда')
    plt.legend()
    plt.subplot(2, 2, 2)
    plt.plot(freq_noisy, spectrum_noisy, label='Зашумленный', alpha=0.5)
    plt.plot(freq_ma, spectrum_ma, label='Скользящее среднее')
    plt.plot(freq_gs, spectrum_gs, label='Гауссово усреднение')
    plt.title('Спектры сигналов')
    plt.xlabel('Частота (Гц)')
    plt.ylabel('Амплитуда')
    plt.xlim(0, 50)
    plt.legend()
    plt.subplot(2, 2, 3)
    zoom_start, zoom_end = 100, 300
    plt.plot(t[zoom_start:zoom_end], noisy_signal[zoom_start:zoom_end], label='Зашумленный', alpha=0.5)
    plt.plot(t[zoom_start:zoom_end], ma_signal[zoom_start:zoom_end], label='Скользящее среднее')
    plt.plot(t[zoom_start:zoom_end], gs_signal[zoom_start:zoom_end], label='Гауссово усреднение')
    plt.title('Увеличенный участок')
    plt.xlabel('Время')
    plt.ylabel('Амплитуда')
    plt.legend()
    plt.show()

task_2()

def task_3():
    N = 1000
    t = np.linspace(0, 1, N)
    np.random.seed(42)
    peaks_positions = np.random.randint(50, 950, 10)
    signal = np.zeros(N)
    signal[peaks_positions] = 1
    noise_level = 0.1
    noisy_signal = signal + np.random.normal(0, noise_level, N)

    def gaussian_smoothing(signal, k, omega=5):
        indices = np.arange(-k, k + 1)
        gaussian_kernel = np.exp(-4 * np.log(2) * indices ** 2 / omega ** 2)
        gaussian_kernel /= np.sum(gaussian_kernel)
        smoothed = np.zeros_like(signal)
        for i in range(len(signal)):
            start = max(0, i - k)
            end = min(len(signal), i + k + 1)
            kernel_start = max(0, k - i)
            kernel_end = min(2 * k + 1, k + (len(signal) - i))
            smoothed[i] = np.sum(signal[start:end] * gaussian_kernel[kernel_start:kernel_end])
        return smoothed

    k = 20
    omega_narrow = 5
    omega_wide = 15

    gs_narrow = gaussian_smoothing(noisy_signal, k, omega_narrow)
    gs_wide = gaussian_smoothing(noisy_signal, k, omega_wide)

    def compute_spectrum(signal, N):
        yf = fft(signal)
        xf = fftfreq(N, 1 / N)
        return xf[:N // 2], 2 / N * np.abs(yf[0:N // 2])

    freq_noisy, spectrum_noisy = compute_spectrum(noisy_signal, N)
    freq_narrow, spectrum_narrow = compute_spectrum(gs_narrow, N)
    freq_wide, spectrum_wide = compute_spectrum(gs_wide, N)

    plt.figure(figsize=(15, 10))
    plt.subplot(2, 2, 1)
    plt.plot(t, noisy_signal, label='Зашумленный сигнал с пиками', alpha=0.5)
    plt.plot(t, gs_narrow, label=f'Гауссово (ω={omega_narrow})', linewidth=2)
    plt.plot(t, gs_wide, label=f'Гауссово (ω={omega_wide})', linewidth=2)
    plt.title('Обработка сигнала с пиками')
    plt.xlabel('Время')
    plt.ylabel('Амплитуда (V)')
    plt.legend()
    plt.subplot(2, 2, 2)
    plt.plot(freq_noisy, spectrum_noisy, label='Зашумленный', alpha=0.5)
    plt.plot(freq_narrow, spectrum_narrow, label=f'Гауссово (ω={omega_narrow})')
    plt.plot(freq_wide, spectrum_wide, label=f'Гауссово (ω={omega_wide})')
    plt.title('Спектры сигналов')
    plt.xlabel('Частота (Гц)')
    plt.ylabel('Амплитуда')
    plt.xlim(0, 100)
    plt.legend()
    zoom_start, zoom_end = 300, 700
    plt.subplot(2, 2, 3)
    plt.plot(t[zoom_start:zoom_end], noisy_signal[zoom_start:zoom_end], label='Зашумленный', alpha=0.5)
    plt.plot(t[zoom_start:zoom_end], gs_narrow[zoom_start:zoom_end], label=f'Гауссово (ω={omega_narrow})')
    plt.plot(t[zoom_start:zoom_end], gs_wide[zoom_start:zoom_end], label=f'Гауссово (ω={omega_wide})')
    plt.title('Увеличенный участок с пиками')
    plt.xlabel('Время')
    plt.ylabel('Амплитуда (V)')
    plt.legend()
    plt.subplot(2, 2, 4)
    indices = np.arange(-k, k + 1)
    kernel_narrow = np.exp(-4 * np.log(2) * indices ** 2 / omega_narrow ** 2)
    kernel_narrow /= np.sum(kernel_narrow)
    kernel_wide = np.exp(-4 * np.log(2) * indices ** 2 / omega_wide ** 2)
    kernel_wide /= np.sum(kernel_wide)
    plt.plot(indices, kernel_narrow, label=f'Узкое ядро (ω={omega_narrow})')
    plt.plot(indices, kernel_wide, label=f'Широкое ядро (ω={omega_wide})')
    plt.title('Гауссовы ядра (нормированные)')
    plt.xlabel('Отсчеты')
    plt.ylabel('Вес')
    plt.legend()
    plt.tight_layout()
    plt.show()

task_3()

def task_4():
    def threshold_filter(signal, threshold):
        """Пороговая фильтрация - заменяет значения выше порога на медиану соседних"""
        filtered = signal.copy()
        for i in range(len(signal)):
            if abs(signal[i]) > threshold:
                left = signal[i - 1] if i > 0 else signal[i + 1]
                right = signal[i + 1] if i < len(signal) - 1 else signal[i - 1]
                filtered[i] = (left + right) / 2
        return filtered

    def median_filter(signal, window_size=5):
        """Медианный фильтр с заданным размером окна"""
        filtered = np.zeros_like(signal)
        half_window = window_size // 2

        for i in range(len(signal)):
            left = max(0, i - half_window)
            right = min(len(signal), i + half_window + 1)
            window = signal[left:right] #вырезаем нужный кусок
            filtered[i] = np.median(window) #медиану записываем в нужную позицию

        return filtered

    def compute_spectrum(signal, N):
        yf = fft(signal)
        xf = fftfreq(N, 1 / N)[:N // 2]
        return xf, 2 / N * np.abs(yf[0:N // 2])

    N = 1000
    t = np.linspace(0, 1, N)
    signal_freq1 = 2
    signal_freq2 = 15
    clean_signal = 0.7 * np.sin(2 * np.pi * signal_freq1 * t) + 0.3 * np.sin(2 * np.pi * signal_freq2 * t)
    np.random.seed(42)
    noisy_signal = clean_signal.copy()
    num_spikes = 25
    spike_indices = np.random.choice(N, size=num_spikes, replace=False)
    spike_amplitudes = np.random.uniform(1.5, 10, size=num_spikes) * np.random.choice([-1, 1], size=num_spikes)
    noisy_signal[spike_indices] += spike_amplitudes

    threshold = np.mean(np.abs(noisy_signal)) + 3 * np.std(noisy_signal) #Любое значение, выходящее за этот порог, считается выбросом.
    thresholded = threshold_filter(noisy_signal, threshold)
    filtered_signal = median_filter(thresholded)

    freq_clean, spectrum_clean = compute_spectrum(clean_signal, N)
    freq_noisy, spectrum_noisy = compute_spectrum(noisy_signal, N)
    freq_filtered, spectrum_filtered = compute_spectrum(filtered_signal, N)

    plt.figure(figsize=(15, 10))
    plt.subplot(2, 2, 1)
    plt.plot(t, clean_signal, 'b', label='Чистый сигнал', alpha=0.7)
    plt.plot(t, noisy_signal, 'r', label='Зашумленный сигнал', alpha=0.5)
    plt.title('Сигналы во временной области')
    plt.xlabel('Время')
    plt.ylabel('Амплитуда')
    plt.legend()
    plt.grid(True)
    plt.subplot(2, 2, 2)
    plt.plot(t, clean_signal, 'b', label='Чистый сигнал', alpha=0.7)
    plt.plot(t, filtered_signal, 'g', label='Фильтрованный сигнал', alpha=0.8)
    plt.title('Результат фильтрации')
    plt.xlabel('Время')
    plt.ylabel('Амплитуда')
    plt.legend()
    plt.grid(True)
    plt.subplot(2, 2, 3)
    plt.plot(freq_clean, spectrum_clean, 'b', label='Чистый сигнал')
    plt.plot(freq_noisy, spectrum_noisy, 'r', label='Зашумленный сигнал', alpha=0.5)
    plt.title('Спектры сигналов')
    plt.xlabel('Частота (Гц)')
    plt.ylabel('Амплитуда')
    plt.xlim(0, 50)
    plt.legend()
    plt.grid(True)
    plt.subplot(2, 2, 4)
    plt.plot(freq_clean, spectrum_clean, 'b', label='Чистый сигнал')
    plt.plot(freq_filtered, spectrum_filtered, 'g', label='Фильтрованный сигнал')
    plt.title('Спектр после фильтрации')
    plt.xlabel('Частота (Гц)')
    plt.ylabel('Амплитуда')
    plt.xlim(0, 50)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()


task_4()

def task_5():
    from scipy.linalg import lstsq

    np.random.seed(42)
    n_points = 1000
    t = np.linspace(0, 10, n_points)

    signal_freq1 = 1.5
    signal_freq2 = 5.0
    clean_signal = 0.5 * np.sin(2 * np.pi * signal_freq1 * t) + 0.3 * np.sin(2 * np.pi * signal_freq2 * t)

    # Добавление нелинейного тренда и шума
    trend = 0.1 * t ** 2 - 0.5 * t + 0.05 * t ** 3
    noise = np.random.normal(0, 0.2, n_points)
    signal_with_trend = clean_signal + trend + noise

    def calculate_bic(y_true, y_pred, k):
        n = len(y_true)
        residuals = y_true - y_pred
        sigma_sq = np.sum(residuals ** 2) / n
        bic = n * np.log(sigma_sq) + k * np.log(n)
        return bic

    # Функция для создания матрицы плана полинома
    def create_poly_matrix(t, degree):
        return np.column_stack([t ** i for i in range(degree + 1)])

    # Подбор оптимальной степени полинома
    max_poly_degree = 6
    bic_values = []

    for degree in range(1, max_poly_degree + 1):
        # Создаем матрицу плана
        X = create_poly_matrix(t, degree)

        # Решаем методом наименьших квадратов
        coefficients, _, _, _ = lstsq(X, signal_with_trend) #решает систему линейных уравнений методом наименьших квадратов, вектор найденных коэффициентов полинома.
        y_pred = X @ coefficients #предсказанные значения сигнала, полученные подстановкой коэффициентов в модель.

        # Вычисляем BIC
        bic = calculate_bic(signal_with_trend, y_pred, degree + 1)
        bic_values.append(bic)

        # Визуализация для каждой степени
        plt.figure(figsize=(10, 4))
        plt.plot(t, signal_with_trend, label='Исходный сигнал')
        plt.plot(t, y_pred, label=f'Полином {degree} степени', color='red')
        plt.title(f'Аппроксимация тренда (степень {degree}), BIC={bic:.2f}')
        plt.legend()
        plt.show()

    # Выбор оптимальной степени по минимальному BIC
    optimal_degree = np.argmin(bic_values) + 1
    print(f"Оптимальная степень полинома: {optimal_degree} (минимальный BIC)")

    # Удаление тренда оптимальным полиномом
    X_optimal = create_poly_matrix(t, optimal_degree)
    coefficients_optimal, _, _, _ = lstsq(X_optimal, signal_with_trend) #коэффициенты полинома нужны, остальные значение нет
    trend_estimate = X_optimal @ coefficients_optimal #Умножение матрицы плана X_optimal на вектор коэффициентов даёт предсказанные значения тренда
    detrended_signal = signal_with_trend - trend_estimate #trend_estimate — это аппроксимация тренда в сигнале  мы его удаляем

    # Вычисление спектров
    def compute_spectrum(signal, n_points):
        yf = fft(signal)
        xf = fftfreq(n_points, t[1] - t[0])
        return xf[:n_points // 2], 2 / n_points * np.abs(yf[0:n_points // 2])

    freq_original, spectrum_original = compute_spectrum(signal_with_trend, n_points)
    freq_detrended, spectrum_detrended = compute_spectrum(detrended_signal, n_points)
    freq_clean, spectrum_clean = compute_spectrum(clean_signal, n_points)

    # Визуализация результатов
    plt.figure(figsize=(15, 10))

    # Сигналы во временной области
    plt.subplot(2, 2, 1)
    plt.plot(t, signal_with_trend, label='Исходный сигнал с трендом')
    plt.plot(t, trend_estimate, label=f'Оценка тренда ({optimal_degree} степени)', color='red')
    plt.plot(t, detrended_signal, label='Сигнал без тренда', color='green')
    plt.title('Удаление нелинейного тренда')
    plt.xlabel('Время')
    plt.ylabel('Амплитуда')
    plt.legend()

    # Спектры
    plt.subplot(2, 2, 2)
    plt.plot(freq_original, spectrum_original, label='Исходный сигнал')
    plt.plot(freq_detrended, spectrum_detrended, label='Без тренда', color='green')
    plt.plot(freq_clean, spectrum_clean, label='Чистый сигнал', linestyle='--')
    plt.title('Сравнение спектров')
    plt.xlabel('Частота (Гц)')
    plt.ylabel('Амплитуда')
    plt.xlim(0, 10)
    plt.legend()

    # BIC критерий для разных степеней
    plt.subplot(2, 2, 3)
    plt.plot(range(1, max_poly_degree + 1), bic_values, marker='o')
    plt.title('Критерий BIC для разных степеней полинома')
    plt.xlabel('Степень полинома')
    plt.ylabel('Значение BIC')
    plt.xticks(range(1, max_poly_degree + 1))

    # Увеличенный участок сигналов
    zoom_start, zoom_end = 400, 600
    plt.subplot(2, 2, 4)
    plt.plot(t[zoom_start:zoom_end], signal_with_trend[zoom_start:zoom_end], label='С трендом')
    plt.plot(t[zoom_start:zoom_end], detrended_signal[zoom_start:zoom_end], label='Без тренда', color='green')
    plt.plot(t[zoom_start:zoom_end], clean_signal[zoom_start:zoom_end], label='Чистый', linestyle='--')
    plt.title('Увеличенный участок сигналов')
    plt.xlabel('Время')
    plt.ylabel('Амплитуда')
    plt.legend()

    plt.tight_layout()
    plt.show()

task_5()