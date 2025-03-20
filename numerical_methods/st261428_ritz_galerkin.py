import numpy as np
import matplotlib.pyplot as plt

a = 0.0
b = 1.0

def f(x):
    return np.sin(np.pi * x)

def p(x):
    return 1

def q(x):
    return x + 1

def fd(x):
    return np.pi * np.cos(np.pi * x)

N = [50, 100, 200, 400, 800]
er = []

# Galerkin approximation
for n in N:
    h = (b - a) / n

    y_exact = []
    for i in range(n + 1):
        y_exact.append(f(i * h))


    A = np.zeros((n + 1, n + 1))
    for i in range(n):
        A[i, i] = (p(h * i - (h/2)) + p(h * i + (h/2)))/h + (q(h * i - h/2) + q(h * i + h/2)) * (h/3)
        A[i, i + 1] = -p(h * i + h/2) / h + q(h * i + h/2) * (h/6)
        A[i + 1, i] = -p(h * i + h/2) / h + q(h * i + h/2) * (h/6)

    A[n, n] = (p(h * n - (h/2)) + p(h * n + (h/2)))/h + (q(h * n - h/2) + q(h * n + h/2)) * (h/3)


    F = np.zeros(n + 1)
    for i in range(n + 1):
        F[i] = (f(h * (i - 0.5)) + f(h * (i + 0.5))) / 2

    C = np.zeros(n + 1)
    C = np.linalg.solve(A, F)

    f_calc = np.dot(A, C)

    error = 0
    for i in range(n):
        error += ((f(h * i) - f_calc[i])**2 + 4 * (f(h * (i + 0.5)) - (f_calc[i + 1] + f_calc[i]) / 2)**2 + (f(h * (i + 1)) - f_calc[i + 1])**2) * (h / 6)

    error = np.sqrt(error)
    er.append(error)

    x = np.linspace(a, b, n + 1)

    plt.figure(n)
    plt.plot(x, f_calc, x, y_exact)

er = np.array(er)

wykl = er[:-1] / er[1:]
alpha = np.log2(wykl)

for i in range(len(alpha)):
    print(alpha[i])



plt.show()
