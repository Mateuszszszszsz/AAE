# k/h = lambda = 0.2
# N = [50, 100, 200, ...]
import numpy as np
import matplotlib.pyplot as plt

x0 = 0
xk = 1
t0 = 0
tk = 1

# coefficient next to df/dx - doesn't have to be a function as it's constant
a_xt = -2

# initial values
u_x0 = 0
u_0t = 0

# lambda
lamb = 0.2


def f(x, t):
    return x**2 + 4 * x * t


def f_ex(x, t):
    return (x**2) * t


for m in [50, 100, 200, 400]:
    n = int(m / lamb)
    h = (xk - x0) / m
    k = lamb * h

    x = np.linspace(x0, xk, m + 1)
    t = np.linspace(t0, tk, n + 1)

    U = np.zeros(m)
    for idx in range(m):
        U[idx] = u_x0

    A = np.zeros([m, m])
    B = np.zeros([m, m])

    for idx in range(m - 1):
        A[idx, idx] = 1 - 0.5 * lamb * a_xt
        B[idx, idx] = 1 + 0.5 * lamb * a_xt
        A[idx + 1, idx] = 0.5 * lamb * a_xt
        B[idx + 1, idx] = -0.5 * lamb * a_xt

    A[m - 1, m - 1] = 1 - 0.5 * lamb * a_xt
    B[m - 1, m - 1] = 1 + 0.5 * lamb * a_xt

    for i in range(int(tk / k)):
        W = np.zeros(m)
        W[0] = u_0t

        F = np.zeros(m)
        for idx in range(m):
            F[idx] = f(x[idx + 1], t[i] + k / 2)

        right_side = B @ U - lamb * a_xt * W + k * F

        U = np.linalg.solve(A, right_side)

    x = np.linspace(x0, xk, m)
    f_exact = np.zeros(m)
    for i in range(m):
        f_exact[i] = f_ex(x[i], 1)

    print(f"A = {A}")
    print(f"B = {B}")
    print(f"W = {W}")
    print(f"F = {F}")

    plt.plot(x, U, x, f_exact)
    plt.show()
