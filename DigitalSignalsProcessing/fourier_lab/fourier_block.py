import math

def numerical_integration(y, a, b, d=0.01):
    summ = 0
    x = a
    while x < b:
        summ += (y(x) + y(x+d)) / 2 * d
        x += d
    return summ



class Fourier:
    def __init__(self, function, period, start_point, order_of_decomposition, d=0.01):
        self.function = function
        self.period = period
        self.start_point = start_point
        self.order_of_decomposition = order_of_decomposition
        self.d = d

        self.a0 = None
        self.an_arr = [None] * self.order_of_decomposition
        self.bn_arr = [None] * self.order_of_decomposition

    def a0_f(self):
        return numerical_integration(self.function, self.start_point, self.start_point + self.period, self.d) * 2 / self.period

    def an_f(self, n):
        def f1(t):
            return self.function(t) * math.cos(n * t / self.period * 2 * math.pi)
        return numerical_integration(f1, self.start_point, self.start_point + self.period, self.d) * 2 / self.period

    def bn_f(self, n):
        def f1(t):
            return self.function(t) * math.sin(n * t / self.period * 2 * math.pi)
        return numerical_integration(f1, self.start_point, self.start_point + self.period, self.d) * 2 / self.period



    def ab_constructor(self):
        self.a0 = self.a0_f()
        for n in range(1, self.order_of_decomposition + 1):
            self.an_arr[n-1] = self.an_f(n)
            self.bn_arr[n-1] = self.bn_f(n)

    def fourier_series(self, t):
        summ = 0
        for n in range(1, self.order_of_decomposition + 1):
            summ += self.an_arr[n-1] * math.cos(n * t / self.period * 2 * math.pi) + self.bn_arr[n-1] * math.sin(n * t / self.period * 2 * math.pi)
        return self.a0 / 2 + summ

    def approximation_error(self, t):
        return self.function(t) - self.fourier_series(t)