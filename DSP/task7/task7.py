import numpy as np
import tensorflow as tf
from tensorflow import keras
import matplotlib.pyplot as plt

# Generate linearly correlated random data
np.random.seed(42)
x = np.random.rand(100).astype(np.float32)  # 100 random points
y = 4 * x + 3 + np.random.randn(100).astype(np.float32)  # Linear equation with noise (y = 4x + 3 + noise)

# Build a single-layer, one-neuron model
model = tf.keras.Sequential([
    keras.layers.Dense(units=1, input_shape=[1])
])

# Compile the model with optimizer and loss function
model.compile(optimizer='sgd', loss='mean_squared_error')

# Train the model
history = model.fit(x, y, epochs=1000, verbose=0)

# Plot loss function during training
loss_values = history.history['loss']
plt.figure(1)
plt.plot(loss_values)
plt.xlabel('Epochs')
plt.ylabel('Loss')
plt.title('Loss Function during Training')
plt.savefig("1")

# Make predictions
pred = np.array([10, 20], dtype=np.float32)
predictions = model.predict(pred)
print(f"Predicted values for inputs {pred}: {predictions.flatten()}")

# Extract weights
weights = model.get_weights()
a = weights[0][0][0]  # Weight
b = weights[1][0]     # Bias
print(f"Weight (a): {a}, Bias (b): {b}")

# Reconstruct and plot the linear function
y2 = a * x + b
plt.figure(2)
plt.scatter(x, y, label='Original Data')
plt.plot(x, y2, color='red', label='Predicted Line')
plt.xlabel('x')
plt.ylabel('y')
plt.title('Linear Regression with One Neuron')
plt.legend()
plt.savefig("2.png")
