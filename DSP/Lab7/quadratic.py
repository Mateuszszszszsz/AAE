from tensorflow import keras
from matplotlib import pyplot as plt
import numpy as np
import random
import tensorflow as tf

# Generate a random quadratic function
og_a, og_b, og_c = random.random() * 5, -random.random() * 3, random.random() * 4

# Generate data points
_x = np.array([(random.random() - 0.5) * 10 for _ in range(100)])
y = np.array(
    [
        (_x[i] ** 2) * og_a + _x[i] * og_b + og_c + (random.random() - 0.5) * 10
        for i in range(100)
    ]
)

# Plot original data
plt.scatter(_x, y)
plt.title("Original Data")
plt.show()

# Prepare input features (x and x^2)
x = np.stack([_x, _x**2], axis=1)  # Shape (100, 2)

# Build the model
model = tf.keras.Sequential([keras.layers.Dense(units=1, input_shape=[2])])
model.compile(
    optimizer=keras.optimizers.SGD(learning_rate=0.001), loss="mean_squared_error"
)

# Train the model
history = model.fit(
    x,
    y,
    epochs=1000,
    callbacks=[keras.callbacks.EarlyStopping(monitor="loss", patience=10)],
)
loss_values = history.history["loss"]

# Plot the training loss
plt.plot(loss_values)
plt.title("Training Loss")
plt.xlabel("Epoch")
plt.ylabel("Loss")
plt.show()

# Extract learned weights
weights = model.get_weights()
a_learned = weights[0][1][0]  # Coefficient for x^2
b_learned = weights[0][0][0]  # Coefficient for x
c_learned = weights[1][0]  # Bias term

print("Learned coefficients:")
print(f"a (for x^2): {a_learned}")
print(f"b (for x): {b_learned}")
print(f"c (bias): {c_learned}")

print("Original coefficients:")
print(f"a: {og_a}")
print(f"b: {og_b}")
print(f"c: {og_c}")

# Plot model predictions
x_test = np.linspace(-5, 5, 100)
x_test_input = np.stack([x_test, x_test**2], axis=1)
y_pred = model.predict(x_test_input)

plt.scatter(_x, y, label="Original Data")
plt.plot(x_test, y_pred, color="red", label="Model Prediction")
plt.legend()
plt.title("Model Approximation")
plt.savefig("quadratic.png", dpi=300)
with open("results_quadratic", "w") as results_file:
    results_file.write(
        f"Original coefficients:\n"
        f"a: {og_a}\n"
        f"b: {og_b}\n"
        f"c: {og_c}\n"
        f"Learned coefficients:\n"
        f"a: {a_learned}\n"
        f"b: {b_learned}\n"
        f"c: {c_learned}\n"
        f"Loss: {loss_values[-1]}"
    )
plt.show()
