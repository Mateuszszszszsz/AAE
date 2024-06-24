import numpy as np
import matplotlib.pyplot as plt

a = 0.0
b = np.pi

def f(x):
    return np.sin(x)

def f_prim(x):
    return np.cos(x)

# Linear interpolation function
def linear_interpolation(x_coarse, y_coarse, x_dense):
    y_dense = np.zeros_like(x_dense)
    for i in range(len(x_dense)):
        for j in range(len(x_coarse) - 1):
            if x_coarse[j] <= x_dense[i] <= x_coarse[j + 1]:
                # Linear interpolation formula
                y_dense[i] = y_coarse[j] + (y_coarse[j + 1] - y_coarse[j]) * (x_dense[i] - x_coarse[j]) / (x_coarse[j + 1] - x_coarse[j])
                break
    return y_dense


# Function to calculate the derivative using finite differences
def finite_difference_derivative(y_dense, x_dense):
    y_prime_dense = np.zeros_like(y_dense)
    h = x_dense[1] - x_dense[0]
    y_prime_dense[1:-1] = (y_dense[2:] - y_dense[:-2]) / (2 * h)
    y_prime_dense[0] = (y_dense[1] - y_dense[0]) / h
    y_prime_dense[-1] = (y_dense[-1] - y_dense[-2]) / h
    return y_prime_dense


N = [50, 100, 200, 400, 800]
er = []
er_1 = []

# linear interpolation
for n in N:
    x_coarse = np.linspace(a, b, n + 1)
    points = [f(x) for x in x_coarse]

    x_dense = np.linspace(a, b, 5 * (n + 1))  # Dense grid to compare approximation with exact values 
    y_approx = linear_interpolation(x_coarse, points, x_dense)
    y_approx_prime = finite_difference_derivative(y_approx, x_dense)

    y_exact = [f(x) for x in x_dense]
    y_exact_prime = [f_prim(x) for x in x_dense]

    max_error = np.max(np.abs(np.array(y_exact) - y_approx))
    er.append(max_error)

    errors_1 = np.abs(np.array(y_exact) - y_approx) + np.abs(np.array(y_exact_prime) - y_approx_prime)
    max_error_1 = np.max(errors_1)
    er_1.append(max_error_1)

    plt.figure(n)
    plt.plot(x_coarse, points, '*', x_dense, y_approx, '.')


er = np.array(er)
wykl = er[:-1] / er[1:]
alpha = np.log2(wykl)

for i in range(len(er)):
    print(f'er({N[i]}) = {er[i]}')


er_1 = np.array(er_1)
wykl_1 = er_1[:-1] / er_1[1:]
alpha_1 = np.log2(wykl_1)

for i in range(len(alpha)):
    print(f'alpha[{i+1}] = {alpha[i]}')


for i in range(len(er)):
    print(f'er_1({N[i]}) = {er_1[i]}')

for i in range(len(alpha_1)):
    print(f'alpha[{i+1}]_1 = {alpha_1[i]}')



plt.show()
