import os
from PIL import Image
import numpy as np
from tensorflow.keras.datasets.mnist import load_data
from tensorflow.keras.utils import plot_model
import matplotlib.pyplot as plt
from keras import models, layers, saving

# For prototyping to not train model every time
input_model_filename = "model.keras"
output_model_filename = "model.keras"

(data, labels), (_, _) = load_data()

# Just take 10_000 as whole dataset takes hours and gigabytes of memory
data = data[:10_000]
labels = labels[:10_000]

data = data.reshape((data.shape[0], data.shape[1], data.shape[2], 1))
data = data.astype('float32') / 255.0
n_classes = len(np.unique(labels))

if input_model_filename is not None:
  try:
    model = saving.load_model(input_model_filename)
  except:
    raise FileNotFoundError(f"Could not find model '{input_model_filename}'")
else:
  model = models.Sequential()
  model.add(layers.InputLayer(data.shape[1:]))
  model.add(layers.Conv2D(32, (3,3), activation='relu', kernel_initializer='he_uniform'))
  model.add(layers.MaxPool2D((2, 2)))
  model.add(layers.Flatten())
  model.add(layers.Dense(100, activation='relu', kernel_initializer='he_uniform'))
  model.add(layers.Dropout(0.5))
  model.add(layers.Dense(n_classes, activation='softmax'))
  model.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])
  model.fit(data, labels, epochs=100, batch_size=128, verbose=0)

if output_model_filename is not None:
  model.save(output_model_filename, overwrite=True)

model.summary()
plot_model(model, 'model.png', show_shapes=True)

for i in range(25):
    ax = plt.subplot(5, 5, i + 1)
    plt.imshow(data[i], cmap=plt.get_cmap("gray"))
    yh = model.predict(np.asarray([data[i]]))
    pred = np.argmax(yh)
    ax.set_title("true=" + str(labels[i]) + " pred=" + str(pred))

plt.show()

# Custom numbers part
numbers = []

for file in os.listdir("my_numbers"):
  if file.endswith(".png"):
    img = Image.open("my_numbers/" + file).convert('L')

    img_array = np.array(img, dtype='float32')
    img_array = img_array / 255.0
    img_array = img_array.reshape((28, 28, 1))

    numbers.append(img_array)

numbers = np.array(numbers)

print(numbers.shape)

for i in range(10):
    ax = plt.subplot(2, 5, i + 1)
    plt.imshow(numbers[i], cmap=plt.get_cmap("gray"))
    yh = model.predict(np.asarray([numbers[i]]))
    pred = np.argmax(yh)
    ax.set_title("Pred=" + str(pred))

plt.show()