import tensorflow as tf
import numpy as np
from tensorflow.keras.datasets.mnist import load_data
import matplotlib.pyplot as plt
from keras import models, layers

(data, labels), (_, _) = load_data()

data = data[:10000]
labels = labels[:10000]

print(data.shape)

for i in range(25):
  plt.subplot(5, 5, i+1)
  plt.imshow(data[i], cmap=plt.get_cmap('gray'))

plt.show()

data = data.reshape((data.shape[0], data.shape[1], data.shape[2], 1))

data = data.astype('float32') / 255.0

n_classes = len(np.unique(labels))
model = models.Sequential()
model.add(layers.InputLayer(data.shape[1:]))
model.add(layers.Conv2D(32, (3,3), activation='relu', kernel_initializer='he_uniform'))
model.add(layers.MaxPool2D((2, 2)))
model.add(layers.Flatten())
model.add(layers.Dense(100, activation='relu', kernel_initializer='he_uniform'))
model.add(layers.Dropout(0.5))
model.add(layers.Dense(n_classes, activation='softmax'))
model.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])
model.fit(data, labels, epochs=10, batch_size=128, verbose=0)

print(model.summary())

for i in range(25):
    ax = plt.subplot(5, 5, i + 1)
    plt.imshow(data[i], cmap=plt.get_cmap("gray"))
    yh = model.predict(np.asarray([data[i]]))
    pred = np.argmax(yh)
    ax.set_title("Is=" + str(labels[i]) + " pred=" + str(pred))

plt.show()