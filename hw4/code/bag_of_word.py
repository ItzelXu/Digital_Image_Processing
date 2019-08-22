#//(1) Name: Yifei Liu
#//(2) USC ID Number: 3852294243 
#//(3) USC Email: liu534@usc.edu
#//(4) Submission Date: 3/19/2019

import glob
import numpy as np
import imageio
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d
import cv2
from sklearn.cluster import KMeans
from scipy.spatial import distance
import math

def read_raw(img_path):
	img = np.fromfile(img_path, dtype=np.uint8)
	img.shape = (28,28)
	return img

def Image_to_cartesian(img,i,j):
	index = np.array([[i+1],[j+1],[1]])
	cartesian = np.zeros((3,1))
	mtx = np.array([[0,1,-0.5],[-1,0,img.shape[0]+0.5],[0,0,1]])
	cartesian = mtx @ index
	
#	print(cartesian)
	return cartesian

def cartesian_to_Image(img,cartesian):
	idx = np.zeros((3,1))
	mtx = np.array([[0,-1,img.shape[0]+0.5],[1,0,0.5],[0,0,1]])
	idx = mtx @ cartesian - [[1],[1],[0]]
	
	if idx[0] > img.shape[0] - 1:
		idx[0] = img.shape[0] - 1
	elif idx[0] < 0:
		idx[0] = 0
		
	if idx[1] > img.shape[1] - 1:
		idx[1] = img.shape[1] - 1
	elif idx[1] < 0:
		idx[1] = 0
		
#	print("idx:\n",idx)
	return idx
	

def SIFT(img):
	sift = cv2.xfeatures2d.SIFT_create()
	key_point = sift.detect(img,None)
	key_point, des = sift.compute(img,key_point)
	return key_point, des	


	
def circle_point(idx,img,color,angle):
	x = math.floor(idx[0])
	y = math.floor(idx[1])
	
#	x_ = math.floor(x+3*math.cos(angle))
#	y_ = math.floor(y+3*math.sin(angle))
	
#	img[x,y] = color
	cv2.circle(img, (x,y), 0, color,-1)
#	cv2.arrowedLine(img, (x,y), (x_,y_), color, 1)
	

def K_means(training_set):
	kmeans = KMeans(n_clusters = 2, random_state = 0).fit(training_set)
	return kmeans
	
def plot_key_point(img, idx, angle, filename, predict_label):
	if predict_label == 0:
		color = (167, 47, 0)
	elif predict_label == 1:
		color = (255, 0, 255)

	circle_point(idx,img,color,angle)
	cv2.imwrite(filename,img)
		
		
		
if __name__ == "__main__":	
	
	#load image
	
	# "0"
	img_0 = []
	for filename in glob.glob('zero_*.raw'):
		print('reading ', filename)
		img_0.append(read_raw(filename))
	
	num_of_0 = np.shape(img_0)[0]
	print("Total number of 0 is : ", num_of_0)
	
	# "1"
	img_1 = []
	for filename in glob.glob('one_*.raw'):
		print('reading ', filename)
		img_1.append(read_raw(filename))
	
	num_of_1 = np.shape(img_1)[0]
	print("Total number of 1 is : ", num_of_1)

	
	
	# collect SIFT feature
	training_set = []
	for i in range(num_of_0):
		key_point_0, des_0 = SIFT(img_0[i])
		print("number of key point in 0: ", len(key_point_0))
		for j in range(np.shape(des_0)[0]):
			training_set.append(des_0[j,:])
	
	for i in range(num_of_1):
		key_point_1, des_1 = SIFT(img_1[i])
		print("number of key point in 1: ",len(key_point_1))
		if len(key_point_1) != 0:
			for j in range(np.shape(des_1)[0]):
				training_set.append(des_1[j,:])
			
	print(len(training_set))
	
	# train cluster
	kmeans = K_means(training_set)
	print(kmeans.labels_)
	
	
	#load test image and extract SIFT
	img_test = read_raw("eight.raw")
#	print(img_test)
	key_point, des = SIFT(img_test)
	print("number of key point: ", len(key_point))
	for i in range(len(key_point)):
		print("The index of key-point is: ",key_point[i].pt)
		print("The orientation of key-point is: ",key_point[i].angle)
	label = kmeans.predict(des)
	print(label)
	
	
	color_img = cv2.cvtColor(img_test, cv2.COLOR_GRAY2RGB)
	for index, label_ in enumerate(label):
		plot_key_point(color_img, key_point[index].pt, key_point[index].angle, "eight_kp.jpg", label_)

			

	
#	print(label)
		
