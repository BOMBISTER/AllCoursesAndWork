import matplotlib.pyplot as plt
import fourier_block
import user_function_bank
import numpy
import math

def x_arr_constructor(a, b, d=0.01):
    a = int(a // d)
    b = int(b // d)
    x_arr = []
    for i in range(a, b):
        x_arr.append(i*d)
    return x_arr

def function_iterator(function, x_arr):
    y_arr = []
    for x in x_arr:
        y_arr.append(function(x))
    return y_arr


def task_1(a, b):
    fourier_object = fourier_block.Fourier(user_function_bank.flashing_function, 2, 0, 30)
    fourier_object.ab_constructor()

    x_arr = x_arr_constructor(a, b)
    y_arr_f0 = function_iterator(fourier_object.function, x_arr)
    y_arr_f1 = function_iterator(fourier_object.fourier_series, x_arr)
    y_arr_f2 = function_iterator(fourier_object.approximation_error, x_arr)

    plt.subplot(2, 1, 1)
    plt.plot(x_arr, y_arr_f0)
    plt.plot(x_arr, y_arr_f1)
    plt.xlabel("x")
    plt.ylabel("y")
    plt.subplot(2, 1, 2)
    plt.plot(x_arr, y_arr_f2)
    plt.xlabel("x")
    plt.ylabel("y")
    plt.show()

def task_2(a, b):
    fourier_object = fourier_block.Fourier(user_function_bank.special_cos, 0.01, 0, 5, 0.001)
    fourier_object.ab_constructor()

    x_arr = x_arr_constructor(a, b, 0.001)
    y_arr_f0 = function_iterator(fourier_object.function, x_arr)
    y_arr_f1 = function_iterator(fourier_object.fourier_series, x_arr)
    y_arr_f2 = numpy.fft.fft(y_arr_f1)
    freq_arr = numpy.linspace(1, 1 / (x_arr[1] - x_arr[0]), len(x_arr))

    a10_arr = []
    for freq in freq_arr:
        fourier_object.period = 1/freq
        fourier_object.ab_constructor()
        a10_arr.append(fourier_object.an_arr[0])

    plt.subplot(2, 1, 1)
    plt.plot(x_arr, y_arr_f1)
    plt.plot(x_arr, y_arr_f0)
    plt.xlabel("x")
    plt.ylabel("y")

    plt.subplot(2, 1, 2)
    plt.plot(freq_arr[: (len(x_arr) // 2)], y_arr_f2[: (len(x_arr) // 2)])
    plt.scatter(freq_arr[: (len(x_arr) // 2)], a10_arr[: (len(x_arr) // 2)])
    plt.xlabel("frequency")
    plt.ylabel("y")
    plt.show()

task_1(-5, 5)
#task_2(-0.1, 0.1)