import math
import random


def flashing_function(x):
    if int(x%2)==1 or int(x%2)==-1:
        return 0
    if int(x%2)==0:
        return 2


def special_cos(x):
    frequency = 100
    a = 1
    w = 2 * math.pi * frequency
    return a * math.cos(w * x)