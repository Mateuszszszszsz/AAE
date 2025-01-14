from numpy import asarray, unique, argmax
from tensorflow.keras.datasets.mnist import load_data
from tensorflow.keras import Sequential
from tensorflow.keras.layers import Dense, Conv2D, MaxPool2D, Flatten, Dropout
from keras.preprocessing import image  # Make sure this import is not overshadowed
from matplotlib import pyplot as plt

# Load dataset
print(f"loading dataset\n")
(x_train, y_train), (x_test, y_test) = load_data()
x_train = x_train[:10000] #determining the training set x
y_train = y_train[:10000] #determining the training set y
print(f"dataset loaded\n")

# Reshape data 
x_train = x_train.reshape((x_train.shape[0], x_train.shape[1], x_train.shape[2], 1))
x_test = x_test.reshape((x_test.shape[0], x_test.shape[1], x_test.shape[2], 1))

# Determine the shape of the input images
in_shape = x_train.shape[1:]
# Determine the number of classes
n_classes = len(unique(y_train))
print(in_shape, n_classes)

# Normalize pixel values
x_train = x_train.astype('float32') / 255.0
x_test = x_test.astype('float32') / 255.0

# Define model
model = Sequential()
model.add(Conv2D(32, (3, 3), activation='relu', kernel_initializer='he_uniform', input_shape=in_shape))
model.add(MaxPool2D((2, 2)))
model.add(Flatten())
model.add(Dense(100, activation='relu', kernel_initializer='he_uniform'))
model.add(Dropout(0.5))
model.add(Dense(n_classes, activation='softmax'))

# Define loss and optimizer
model.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])

# Fit the model
model.fit(x_train, y_train, epochs=10, batch_size=128, verbose=0)

# Evaluate the model
loss, acc = model.evaluate(x_test, y_test, verbose=0)
print('Accuracy: %.3f' % acc)

# Loading my data 
files = ['o1.png', 'o2.png', 'o3.png', 'o4.png', 'o9.png'] 
x_realTest = []
predictions = []

# converting data to grayscale
for file in files:
    img = image.load_img(file, target_size=(28, 28), color_mode="grayscale")
    img_array = image.img_to_array(img)  
    x_realTest.append(img_array)

x_realTest = asarray(x_realTest) / 255.0  
yhat_custom = model.predict(x_realTest)

# Predictions
for img_array, yhat in zip(x_realTest, yhat_custom):
    pred = argmax(yhat)
    predictions.append(pred)

# # Display images with predictions
# fig, axes = plt.subplots(1, len(files), figsize=(15, 5))
# for i, ax in enumerate(axes):
#     ax.imshow(x_realTest[i].reshape(28, 28), cmap="gray")  
#     ax.set_title(f"Pred: {predictions[i]}", fontsize=12)  
#     ax.axis("off")  
# plt.tight_layout()
# plt.show()

from tensorflow.keras.utils import plot_model #import plot_model before plot
model.summary()
plot_model(model, 'model.png', show_shapes=True)

# for i in range(25): #showing an array of results
#     ax = pyplot.subplot(5, 5, i + 1)
#     pyplot.imshow(x_test[i], cmap=pyplot.get_cmap("gray"))
#     yh = model.predict(asarray([x_test[i]]))
#     pred = argmax(yh)
#     ax.set_title("Is=" + str(y_test[i]) + " pred=" + str(pred))
# pyplot.show()