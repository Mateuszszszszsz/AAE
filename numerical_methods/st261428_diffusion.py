import warnings

import numpy as np
import matplotlib.pyplot as plt

warnings.filterwarnings("ignore")

# x boundaries
x0 = 0
xk = 2
t0 = 0
tk = 0.5


# values of u at boundary values
def u(x, t):
    if t == 0:
        return np.sin(2 * np.pi * x)

    if x == 0 or x == 2:
        return 0


# exact value of u (for comparison)
def u_exact(x, t):
    return np.exp(-4 * np.pi * np.pi * t) * np.sin(2 * np.pi * x)


Lamb = [0.2, 0.4, 0.6]

N = [20, 40, 80, 160, 320]

print("Please wait, calculations take some time")

for lamb in Lamb:
    fig, ax = plt.subplots(3, 2)

    er = []
    for idx, n in enumerate(N):
        print(f'lambda = {lamb}, n = {n}...'.ljust(30), end=" ")
        h = (xk - x0) / n
        k = lamb * h * h
        m = int(tk / k)

        x = np.linspace(x0, xk, n + 1)
        t = np.linspace(t0, tk, m + 1)

        # checking stability conditions
        stable = True if k / (h * h) <= 1/2 else False

        if not stable:
            print("- stability condition: NOT OK".ljust(30), end=" ")
        else:
            print("- stability condition: OK".ljust(30), end=" ")

        # matrix so we can store all values of u at all times t
        U = np.zeros((n + 1, m + 1))

        # fill first column with u at time 0
        U[:, 0] = u(x, 0)
        U[0, 0] = 0

        # f(x, t) = 0
        # F = np.zeros(n + 1)
        # this can be skipped as it does not change the value of equation
        # same for the last term (lambda * vector with phi_j and psi_j)

        # constructing A matrix
        A = np.diag((1 - 2 * lamb) * np.ones(n - 1))        # diagonal
        A += np.diag(lamb * np.ones(n - 2), k = 1)          # 1 over diagonal
        A += np.diag(lamb * np.ones(n - 2), k = -1)         # 1 below diagonal 

        # iterate through all moments in time
        for j in range(m):
            U[1:n, j + 1] = A @ U[1:n, j]

        u_appr = U[:, -1]
        u_ex = u_exact(x, 0.5)

        error = np.max(np.abs(u_appr - u_ex))

        er.append(error)
        print(f'- max error: {error}')

        ax_now = ax[idx // 2][idx % 2]
        ax_now.plot(x, u_ex, label='Exact Solution')
        ax_now.plot(x, U[:, -1], label='Approximate Solution')
        ax_now.set_title(f'n = {n}')
        
    er = np.array(er)

    wykl = er[:-1] / er[1:]
    alpha = np.log2(wykl)
    ax[2][1].plot(wykl)
    ax[2][1].set_title('Convergence rate')

    fig.suptitle(f'lambda = {lamb}{"" if stable else " - UNSTABLE"}')
    fig.show()

plt.show()