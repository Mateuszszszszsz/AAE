import warnings

import numpy as np
import matplotlib.pyplot as plt

warnings.filterwarnings("ignore")

# x boundaries
x0 = 0
xk = 2
L = xk - x0
t0 = 0
tk = 0.5


# values of u at boundary values
def u(x, t) -> float:
    if t == 0:
        return x * (2 - x)

    if x == 0 or x == 2:
        return 0
    
    return -1

def u_k(x, k):
  return x * (2 - x) - k**2


# calculate Fourier series coefficients
x = np.linspace(x0, xk, 1000)

a_k = np.zeros(5)
for k in range(5):
    y = x * (2 - x) * np.sin(x * (k + 1) * np.pi / L)
    a_k[k] = np.trapz(y, x) * (2 / L)


# exact value of u (based on Fourier series)
def u_exact(x, t):
    sum = 0
    for k, a in enumerate(a_k):
        sum += a * np.cos((k + 1) * np.pi * t / L) * np.sin((k + 1) * np.pi * x / L)
    return sum


Lamb = [0.2, 0.4]

N = [20, 40, 80, 160, 320]

print("Please wait, calculations take some time")

for lamb in Lamb:
    fig, ax = plt.subplots(3, 2)

    er = []
    for idx, n in enumerate(N):
        print(f"lambda = {lamb}, n = {n}...".ljust(30), end=" ")
        h = (xk - x0) / n
        k = lamb * h
        m = int(tk / k)

        x = np.linspace(x0, xk, n + 1)
        t = np.linspace(t0, tk, m + 1)

        # matrix so we can store all values of u at all times t
        U = np.zeros((n + 1, m + 1))

        U[:, 0] = u(x, 0)
        U[:, 1] = u(x, 0) - k ** 2
        # U[1: - 1, 1]
        # for i in range(1, n):
        #     U[i, 1] = (1 - lamb**2) * u(x[i], 0) + lamb**2 / 2 * u(x[i+1], 0) + lamb**2 / 2 * u(x[i-1], 0) + k * 0
        U[0, 0] = 0  # boundary conditions

        # constructing A matrix
        A = (-2 * np.eye(n - 1) + np.diag(np.ones(n - 2), 1) + np.diag(np.ones(n - 2), - 1))
        A = 2 * np.eye(n - 1) + lamb**2 * A
        B = -np.eye(n - 1)  # B matrix

        # iterate through all moments in time
        for j in range(1, m):
            U[1:n, j + 1] = A @ U[1:n, j] + B @ U[1:n, j - 1]

        u_appr = U[:, -1]
        u_ex = u_exact(x, 0.5)

        error = np.max(np.abs(u_appr - u_ex))

        er.append(error)
        print(f"- max error: {error}")

        ax_now = ax[idx // 2][idx % 2]
        ax_now.plot(x, u_ex, label="Exact Solution")
        ax_now.plot(x, U[:, -1], label="Approximate Solution", linestyle="dashed")
        ax_now.set_title(f"n = {n}")

    er = np.array(er)

    wykl = er[:-1] / er[1:]
    alpha = np.log2(wykl)
    ax[2][1].plot(wykl)
    ax[2][1].set_title("Convergence rate")

    fig.suptitle(f'lambda = {lamb}')
    fig.show()

plt.show()
