import numpy as np
import tensorflow as tf
from tensorflow import keras
import matplotlib.pyplot as plt

# Generate data for square function
x = np.linspace(-10, 10, 200).astype(np.float32)
y = x ** 2

# Build a neural network model
model = tf.keras.Sequential([
    keras.layers.Dense(units=10, activation='relu', input_shape=[1]),
    keras.layers.Dense(units=10, activation='relu'),
    keras.layers.Dense(units=1)  # Output layer
])

# Compile the model
model.compile(optimizer='adam', loss='mean_squared_error')

# Train the model
history = model.fit(x, y, epochs=500, verbose=0)

# Plot loss function during training
loss_values = history.history['loss']
plt.figure(1)
plt.plot(loss_values)
plt.xlabel('Epochs')
plt.ylabel('Loss')
plt.title('Loss Function during Training for Square Function')
plt.savefig("e1")

# Make predictions
y_pred = model.predict(x)

# Plot original square function and predicted function
plt.figure(2)
plt.scatter(x, y, label='Original Function', color='blue')
plt.plot(x, y_pred, label='Predicted Function', color='red')
plt.xlabel('x')
plt.ylabel('y')
plt.title('Square Function Approximation')
plt.legend()
plt.savefig("e2")
