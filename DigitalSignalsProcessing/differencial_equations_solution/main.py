import numpy as np
import matplotlib.pyplot as plt
from scipy.sparse import diags
from scipy.sparse.linalg import spsolve

T0 = 273.15  # 0°C = 273.15K
k = 0.025  # теплопроводность, Вт/(м·К)
rho = 1.2  # плотность, кг/м^3
c = 1005  # теплоемкость, Дж/(кг·К)
alpha = k / (rho * c)
q = 1000  # источник тепла, Вт/м^3

# Сетка
L = 1.0  # длина стержня, м
Nx = 100  # количество узлов
dx = L / Nx
x = np.linspace(0, L, Nx + 1)

# Временные параметры
total_time = 2.0
dt = 0.1 * dx ** 2 / (2 * alpha)  # устойчивость явной схемы
Nt = int(total_time / dt) + 1


# Граничные условия в Кельвинах
def T_left(t):
    return max(283.15, 293.15 - 5 * t)  # min 10°C = 283.15K


def T_right(t):
    return max(263.15, 273.15 - 5 * t)  # min -10°C = 263.15K


# Точные решения в Кельвинах
def exact_solution_no_source(x):
    return 293.15 * (1 - x) + 273.15 * x  # линейное распределение


def exact_solution_with_source(x):
    return (293.15 * (1 - x) + 273.15 * x) + (q * L ** 2 / (2 * k)) * (x - x ** 2)


# Task 1a: Стационарное решение без источника
def task_1a():  # Метод: Итерационный процесс (метод Якоби)
    T = np.linspace(T_left(0), T_right(0), Nx + 1)
    max_iter = 10000
    tolerance = 1e-6
    for _ in range(max_iter):
        T_old = T.copy()
        for i in range(1, Nx):
            T[i] = 0.5 * (T[i - 1] + T[i + 1])
        if np.max(np.abs(T - T_old)) < tolerance:
            break
    return x, T


# Явная схема (a, b, c)
def explicit_scheme(with_source=False, time_dependent_bc=True):  # Добавлен параметр time_dependent_bc
    T = np.linspace(T_left(0), T_right(0), Nx + 1)  # линейная интерполяция
    times = np.linspace(0, total_time, Nt)  # 2D-массив
    temps = np.zeros((Nt, Nx + 1))
    temps[0] = T.copy()  # сохраняет начальное распределение температуры в первый слой
    r = alpha * dt / dx ** 2  # r: Число Куранта (alpha * dt / dx²), должно быть ≤ 0.5 для устойчивости.
    q_term = q * dt / (rho * c) if with_source else 0

    for n in range(1, Nt):  # (проходим по всем временным слоям)
        t = times[n]  # текущее время
        T_new = temps[n - 1].copy()

        # Изменено: выбор граничных условий в зависимости от time_dependent_bc
        T_new[0] = T_left(t) if time_dependent_bc else T_left(0)  # Граничные условия
        T_new[-1] = T_right(t) if time_dependent_bc else T_right(0)

        for i in range(1, Nx):
            T_new[i] = temps[n - 1, i] + r * (temps[n - 1, i - 1] - 2 * temps[n - 1, i] + temps[n - 1, i + 1]) + q_term

        temps[n] = T_new

    return x, times, temps


# Неявная схема (a, b, c)
def implicit_scheme(with_source=False, time_dependent_bc=True):  # Добавлен параметр time_dependent_bc
    T = np.linspace(T_left(0), T_right(0), Nx + 1)
    times = np.linspace(0, total_time, Nt)
    temps = np.zeros((Nt, Nx + 1))
    temps[0] = T.copy()
    r = alpha * dt / dx ** 2  # Безусловно устойчива (можно использовать большие dt).
    q_term = q * dt / (rho * c) if with_source else 0

    # Матрица системы
    diagonals = [[-r] * (Nx - 2), [1 + 2 * r] * (Nx - 1), [-r] * (Nx - 2)]  # Создаёт трёхдиагональную матрицу A
    A = diags(diagonals, [-1, 0, 1], format='csc')  # эффективное хранение разреженных матриц

    for n in range(1, Nt):
        t = times[n]
        b = temps[n - 1, 1:-1].copy()  # копирует температуру внутренних точек
        if with_source:
            b += q_term  # Добавляет вклад от источника тепла

        # Изменено: учет граничных условий в зависимости от time_dependent_bc
        if time_dependent_bc:
            b[0] += r * T_left(t)
            b[-1] += r * T_right(t)
        else:
            b[0] += r * T_left(0)
            b[-1] += r * T_right(0)

        temps[n, 1:-1] = spsolve(A, b)  # Решает систему
        temps[n, 0] = T_left(t) if time_dependent_bc else T_left(0)  # Граничные условия
        temps[n, -1] = T_right(t) if time_dependent_bc else T_right(0)

    return x, times, temps


# Решение всех задач с правильными параметрами
x, T_stat = task_1a()
# Стационарные случаи (a) - time_dependent_bc=False
x, times_exp_a, temps_exp_a = explicit_scheme(with_source=False, time_dependent_bc=False)
x, times_imp_a, temps_imp_a = implicit_scheme(with_source=False, time_dependent_bc=False)
# Нестационарные случаи (b) - time_dependent_bc=True
x, times_exp_b, temps_exp_b = explicit_scheme(with_source=False, time_dependent_bc=True)
x, times_imp_b, temps_imp_b = implicit_scheme(with_source=False, time_dependent_bc=True)
# Случаи с источником (c) - time_dependent_bc=True
x, times_exp_c, temps_exp_c = explicit_scheme(with_source=True, time_dependent_bc=True)
x, times_imp_c, temps_imp_c = implicit_scheme(with_source=True, time_dependent_bc=True)


# Функция для построения графиков (без изменений)
def plot_solutions(x, times, temps_list, titles, main_title):
    plt.figure(figsize=(15, 5))
    for i, (temps, title) in enumerate(zip(temps_list, titles), 1):
        plt.subplot(1, 3, i)
        time_indices = np.linspace(0, len(times) - 1, 6, dtype=int)
        for idx in time_indices:
            plt.plot(x, temps[idx] - T0, label=f't={times[idx]:.2f} с')
        plt.title(title)
        plt.xlabel('x, м')
        plt.ylabel('T, °C')
        plt.grid()
        plt.legend()
    plt.suptitle(main_title)
    plt.tight_layout()
    plt.show()


# Построение графиков
plot_solutions(x, times_exp_a,
               [temps_exp_a, temps_exp_b, temps_exp_c],
               ['Явная: стационар (a)', 'Явная: нестационар (b)', 'Явная: с источником (c)'],
               'Решения с использованием явной схемы')

plot_solutions(x, times_imp_a,
               [temps_imp_a, temps_imp_b, temps_imp_c],
               ['Неявная: стационар (a)', 'Неявная: нестационар (b)', 'Неявная: с источником (c)'],
               'Решения с использованием неявной схемы')



# Дополнительное моделирование для большого времени (без изменений)
total_time_long = 100
dt_long = 0.1 * dx ** 2 / (2 * alpha)
Nt_long = int(total_time_long / dt_long) + 1
times_long = np.linspace(0, total_time_long, Nt_long)


def explicit_scheme_long(with_source=False, time_dependent_bc=True):
    T = np.linspace(T_left(0), T_right(0), Nx + 1)
    temps = np.zeros((Nt_long, Nx + 1))
    temps[0] = T.copy()
    r = alpha * dt_long / dx ** 2
    q_term = q * dt_long / (rho * c) if with_source else 0

    for n in range(1, Nt_long):
        t = times_long[n]
        T_new = temps[n - 1].copy()
        T_new[0] = T_left(t) if time_dependent_bc else T_left(0)
        T_new[-1] = T_right(t) if time_dependent_bc else T_right(0)

        for i in range(1, Nx):
            T_new[i] = temps[n - 1, i] + r * (
                    temps[n - 1, i - 1] - 2 * temps[n - 1, i] + temps[n - 1, i + 1]
            ) + q_term

        temps[n] = T_new

    return temps


# Моделирование с правильными параметрами
temps_long_no_source = explicit_scheme_long(with_source=False, time_dependent_bc=True)
temps_long_with_source = explicit_scheme_long(with_source=True, time_dependent_bc=True)

plot_solutions(x, times_long,
               [temps_long_no_source, temps_long_with_source],
               ['Без источника (q=0)', 'С источником (q=1000 Вт/м³)'],
               'Эволюция температуры при длительном времени')