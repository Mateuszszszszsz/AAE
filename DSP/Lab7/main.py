from tensorflow import keras
from matplotlib import pyplot as plt
import numpy as np
import random
import tensorflow as tf

x = np.array([random.random() for _ in range(100)])
y = np.array([x[i] + (random.random() - .5) * .2 for i in range(100)])

# plt.scatter(y, x)
# plt.show()

model = tf.keras.Sequential([keras.layers.Dense(units=1, input_shape=[1])])
model.compile(optimizer='sgd', loss='mean_squared_error')

history = model.fit(x, y, epochs=1000, callbacks=[keras.callbacks.EarlyStopping(monitor="loss", patience=10)])
loss_values = history.history['loss']
plt.plot(loss_values)
plt.show()

pred = np.array([10, 20])
print(model.predict(pred))

print(model.get_weights())


# Extract learned weights
weights = model.get_weights()
a = weights[0][0] # Coefficient for x
b = weights[1][0] # Bias

print("Learned coefficients:")
print(f"a: {a}")
print(f"b: {b}")
y_pred = x * a + b

plt.scatter(x, y, label="Original Data")
plt.plot(x, y_pred, color="red", label="Model Prediction")
plt.legend()
plt.title("Model Approximation")
plt.savefig("linear.png", dpi=300)
with open("results_linear", "w") as results_file:
    results_file.write(
        f"Learned coefficients:\n"
        f"a: {a}\n"
        f"b: {b}\n"
        f"Loss: {loss_values[-1]}"
    )
plt.show()