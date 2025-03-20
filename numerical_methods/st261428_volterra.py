import numpy as np
import matplotlib.pyplot as plt

# constant definitions
a = 0
b = 1 * np.pi
N = [50, 100, 200, 400, 800]


# helper functions
def k(x, s):
    return x - s


def f(x):
    return x


errors = []

# loop over N
for n in N:
    x = np.linspace(a, b, n)
    h = (b - a) / n

    u = np.zeros_like(x)
    for i in range(n):
        u[i] = f(x[i])
        for j in range(i - 1):
            u[i] -= 0.5 * h * (k(x[i], x[j]) * u[j] + k(x[i], x[j + 1]) * u[j + 1])

    u_exact = [np.sin(x_i) for x_i in x]

    error = np.max(np.abs(u_exact - u))
    errors.append(error)

    plt.figure(n)
    plt.plot(x, u, label='Numerical Solution')
    plt.plot(x, u_exact, label='Exact solution', linestyle='dashed')
    plt.legend()

er = np.array(errors)

wykl = er[:-1] / er[1:]
alphas = np.log2(wykl)
for alpha in alphas:
    print(f"alpha: {alpha}")

plt.show()
