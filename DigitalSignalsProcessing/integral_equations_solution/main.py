import numpy as np
from scipy.linalg import solve
import matplotlib.pyplot as plt
from scipy.special import factorial
from scipy import integrate
import math

def task_1():
    a, b = 0, 1
    h = 1 / 30
    lam = 1 / 2

    def K(x, s):
        return x * s

    def f(x):
        return (5 / 6) * x

    def y_exact(x):
        return x

    def solve_integral_equation(K, f, a, b, h, lam):
#         Этот цикл заполняет матрицу системы:
#         Для каждой точки xi сетки:
#         Первый и последний элементы строки рассчитываются с весом wt = 0.5 (крайние точки)
#         Средние элементы рассчитываются с весом wj = 1
#         На диагонали добавляется 1 (единичная матрица)
#         Фактически, это дискретизация интегрального оператора с помощью квадратурной формулы.

        x = np.arange(a, b + h, h)
        n = len(x)
        x_col = x.reshape(-1, 1) #это та же сетка, но преобразованная в столбец
        wt, wj = 0.5, 1
        A = np.zeros((n, n))
        for i in range(n):
            xi = x_col[i, 0]
            A[i, 0] = -h * wt * K(xi, x_col[0, 0]) * lam
            for j in range(1, n - 1):
                A[i, j] = -h * wj * K(xi, x_col[j, 0]) * lam
            A[i, n - 1] = -h * wt * K(xi, x_col[n - 1, 0]) * lam
            A[i, i] += 1

        B = f(x_col).flatten() #Создаётся вектор правой части, равный значениям функции f в узлах сетки
        y_approx = np.linalg.solve(A, B) #Решается система линейных уравнений A·y = B
        return x, y_approx

    x, y_approx = solve_integral_equation(K, f, a, b, h, lam)

    plt.figure(figsize=(10, 6))
    plt.plot(x, y_exact(x), 'r-', label='Точное решение')
    plt.plot(x, y_approx, 'b--', label='Приближенное решение')
    plt.xlabel('x')
    plt.ylabel('y(x)')
    plt.legend()
    plt.grid(True)
    plt.show()

    print(f"Максимальная ошибка: {np.max(np.abs(y_approx - y_exact(x))):.3e}")

task_1()

def task_2():
    left_integrate_border = 0 # Левая граница интегрирования
    right_integrate_boarder = 1
    lam = -1
    step = 1 / 20

    x = np.arange(left_integrate_border, right_integrate_boarder, step)
    x = x.reshape(len(x), 1) # Преобразуем в столбец (20x1)
    n = len(x)

    alpha = lambda x: [x ** 2, x ** 3, x ** 4]  # Базисные функции для представления решения
    beta = lambda s: [s, s ** 2 / 2, s ** 3 / 6]   # Базисные функции для вырожденного ядра
    f = lambda t: np.exp(t) - t   # Свободный член уравнения

    y_exact = np.ones(20) # Предполагаемое точное решение (константа 1)

    def bfun(t, m, f): # Вычисляет m-ю компоненту вектора правой части
        return beta(t)[m] * f(t)

    def Aijfun(t, m, k): # Вычисляет элемент матрицы системы
        return beta(t)[m] * alpha(t)[k]

    def Solve(f, t, Lambda, a, b):
        m = len(alpha(0))  # Размерность базиса (3)
        M = np.zeros((m, m))  # Матрица системы (3x3)
        r = np.zeros((m, 1))  # Вектор правой части (3x1)
        for i in range(m):
            r[i] = integrate.quad(bfun, a, b, args=(i, f))[0] # Численное интегрирование для правой части
            for j in range(m):
                M[i][j] = -Lambda * integrate.quad(Aijfun, a, b, args=(i, j))[0] # Численное интегрирование для элементов матрицы
        for i in range(m):
            M[i][i] = M[i][i] + 1 # Добавляем единичную матрицу
        c = np.linalg.solve(M, r)# Решение системы линейных уравнений
        return Lambda*(c[0]*alpha(t)[0] + c[1]*alpha(t)[1] + c[2]*alpha(t)[2]) + f(t)

    y_approx = Solve(f, x, lam, left_integrate_border, right_integrate_boarder)
    plt.plot(x, y_exact, '-g', linewidth=2, label='y_exact')
    plt.plot(x, y_approx, 'or', label='y_approx')
    plt.xlabel("x")
    plt.ylabel("y")
    plt.legend('1', fontsize=12)
    plt.legend(bbox_to_anchor=(1, 1), loc='best')
    plt.ylim([0, max(y_exact) + 0.2])
    plt.show()

task_2()


def task_3():
    a, b = -1, 1
    x = np.linspace(a, b, 100).reshape(-1, 1)

    y_exact = lambda x: 1 + 6 * x ** 2 # Предполагаемое точное решение

    phi = [lambda t: t, lambda t: t ** 2] # Базисные функции для решения
    psi = [lambda s: 1, lambda s: s] # Тестовые функции (базис для метода Галёркина)

    K = lambda x, s: x ** 2 + x * s # Ядро интегрального уравнения
    f = lambda x: 1 # Свободный член уравнения

    def compute_system():
        A = np.zeros((2, 2))  # Матрица системы 2x2
        b_vec = np.zeros(2) # Вектор правой части

        for i in range(2):  # Вычисление i-го элемента вектора b
            b_vec[i] = integrate.dblquad(
                lambda x, s: psi[i](x) * K(x, s) * f(s),
                a, b, a, b
            )[0]

            for j in range(2):
                term1 = integrate.quad(lambda x: psi[i](x) * phi[j](x), a, b)[0]
                term2 = integrate.dblquad(
                    lambda x, s: psi[i](x) * K(x, s) * phi[j](s),
                    a, b,   # Пределы по x
                    a, b    # Пределы по s
                )[0]
                A[i, j] = term1 - term2

        return A, b_vec

    A, b_vec = compute_system()
    c = np.linalg.solve(A, b_vec) # Решение системы линейных уравнений

    y_approx = lambda x: 1 + c[0] * phi[0](x) + c[1] * phi[1](x)

    plt.figure(figsize=(10, 6))
    plt.plot(x, y_exact(x), 'g-', lw=2, label='Точное решение')
    plt.plot(x, y_approx(x), 'r--', lw=2, label=f'Приближенное: $1 + {c[0]:.2f}x + {c[1]:.2f}x^2$')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.legend()
    plt.grid(True)
    plt.title('Решение уравнения')
    plt.show()


    print("Матрица A:\n", A)
    print("\nВектор b:\n", b_vec)
    print("\nКоэффициенты c₁, c₂:", c)
task_3()