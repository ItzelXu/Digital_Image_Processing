import keras
from keras.datasets import mnist
from keras.utils import np_utils
from keras.models import Sequential
from keras import models, layers
import matplotlib.pyplot as plt

## import data and prepare
# Load dataset as train and test sets
(x_train, y_train), (x_test, y_test) = mnist.load_data()

# Normalize value to [0, 1]
x_train = x_train / 255
x_test = x_test / 255

# Transform lables to one-hot encoding
y_train = np_utils.to_categorical(y_train, 10)
y_test = np_utils.to_categorical(y_test, 10)

# Reshape the dataset into 4D array
x_train = x_train.reshape(x_train.shape[0], 28,28,1)
x_test = x_test.reshape(x_test.shape[0], 28,28,1)

#Instantiate an empty model
model = Sequential()

## CNN

model.add(layers.Conv2D(6, kernel_size=(5, 5), strides=(1, 1), activation='relu', input_shape=(28,28,1), padding="same"))
model.add(layers.MaxPooling2D(pool_size=(2, 2), strides=None, padding='valid'))
model.add(layers.Conv2D(16, kernel_size=(5, 5), strides=(1, 1), activation='relu', padding='valid'))
model.add(layers.MaxPooling2D(pool_size=(2, 2), strides=None, padding='valid'))
model.add(layers.Conv2D(120, kernel_size=(5, 5), strides=(1, 1), activation='relu', padding='valid'))
model.add(layers.Flatten())
model.add(layers.Dense(84, activation='tanh'))
model.add(layers.Dense(10, activation='softmax'))
plot_model(model, to_file='model__.png',show_shapes=True,show_layer_names=True)

# Compile the model
model.compile(loss=keras.losses.categorical_crossentropy, optimizer='SGD', metrics=["accuracy"])
model.summary()

tensorboard = TensorBoard(
	log_dir="./logs/"+str(1e-3)+'_epoch_+'+str(15)+'_batch_'+str(64),
	histogram_freq=0, batch_size=self.batch_size, write_graph=True
	)
checkpoint = ModelCheckpoint('./checkpoints/checkpoint.h5', monitor='val_acc', save_best_only=True, verbose=1, mode='max')
hist = model.fit(x=x_train,y=y_train, epochs=15, batch_size=128, validation_data=(x_test, y_test), verbose=1)

test_score = model.evaluate(x_test, y_test)
print("Test loss {:.4f}, accuracy {:.2f}%".format(test_score[0], test_score[1] * 100))


f, ax = plt.subplots()
ax.plot([None] + hist.history['acc'], 'o-')
ax.plot([None] + hist.history['val_acc'], 'x-')
# Plot legend and use the best location automatically: loc = 0.
ax.legend(['Train acc', 'Validation acc'], loc = 0)
ax.set_title('Training/Validation acc per Epoch')
ax.set_xlabel('Epoch')
ax.set_ylabel('acc')
# plt.show()

#plt.figure(figsize=(10,8))
#plt.plot(hist.history['acc'], label='Training Accuracy')
#plt.plot(hist.history['val_acc'], label='Test Accuracy')
#plt.legend(loc='best')
#plt.title('Training and test Accuracy')
#plt.savefig('model-'+str(self.l_r)+'_epoch_+'+str(self.epochs)+'_batch_'+str(self.batch_size)+'.png')
## plt.show()





#f, ax = plt.subplots()
#ax.plot([None] + hist.history['loss'], 'o-')
#ax.plot([None] + hist.history['val_loss'], 'x-')
## Plot legend and use the best location automatically: loc = 0.
#ax.legend(['Train Loss', 'Validation Loss'], loc = 0)
#ax.set_title('Training/Validation Loss per Epoch')
#ax.set_xlabel('Epoch')
#ax.set_ylabel('Loss')