#//(1) Name: Yifei Liu
#//(2) USC ID Number: 3852294243 
#//(3) USC Email: liu534@usc.edu
#//(4) Submission Date: 3/19/2019

import numpy as np
import imageio
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d
import cv2
from scipy.spatial import distance
import math


def Image_to_cartesian(img,i,j):
	index = np.array([[i+1],[j+1],[1]])
	cartesian = np.zeros((3,1))
	mtx = np.array([[0,1,-0.5],[-1,0,img.shape[0]+0.5],[0,0,1]])
	cartesian = mtx @ index
	
#	print(cartesian)
	return cartesian

def cartesian_to_Image(img,cartesian):
	cartesian_array = np.array([[cartesian[0]],[cartesian[1]],[1]])
	print(cartesian_array)
	idx = np.zeros((3,1))
	mtx = np.array([[0,-1,img.shape[0]+0.5],[1,0,0.5],[0,0,1]])
	idx = mtx @ cartesian_array - [[1],[1],[0]]
#	print(idx)
	
	if idx[0] > img.shape[0] - 1:
		idx[0] = img.shape[0] - 1
	elif idx[0] < 0:
		idx[0] = 0
		
	if idx[1] > img.shape[1] - 1:
		idx[1] = img.shape[1] - 1
	elif idx[1] < 0:
		idx[1] = 0
		
#	print("idx:\n",idx)
	return idx[:2]
	

def SIFT(img):
	gray_img = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
	sift = cv2.xfeatures2d.SIFT_create()
	key_point = sift.detect(gray_img,None)
	key_point, des = sift.compute(gray_img,key_point)
#	img_output=cv2.drawKeypoints(gray_img, key_point, img, flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
#	cv2.imwrite("river2_keypoint.png",img_output)
	return key_point, des	

def largest_scale(key_point,des):
	num_of_keypoint = len(key_point)
	scale = np.zeros((num_of_keypoint,1))
	for num in range(num_of_keypoint):
		scale[num] = np.linalg.norm(des[num,:])
	print(scale)
	print("The row number of the largest l2 norm of the discriptor: ", np.where(scale == max(scale))[0])
	[largest_scale_idx] = np.where(scale == np.max(scale))[0]
	img_idx = key_point[largest_scale_idx].pt

	return img_idx, largest_scale_idx

def nearest_neighbour(largest_scale_des_1, key_point_2, des_2, k):
	dist_list = []
	for des_ in des_2:
		dist = distance.euclidean(largest_scale_des_1, des_)
		dist_list.append(dist)
	return_list = np.argpartition(dist_list, k)
	return return_list[0:k]
	
def circle_point(idx, img, color, angle, dia, filename):
#	color_img = cv2.cvtColor(img, cv2.COLOR_GRAY2RGB)
	x = math.floor(idx[0])
	y = math.floor(idx[1])
	
	x_ = math.floor(x+30*math.sin(angle))
	y_ = math.floor(y+30*math.cos(angle))
	
#	img[x,y] = color
	cv2.circle(img, (x,y), 30, color)
	cv2.arrowedLine(img, (x,y), (x_,y_), color, 1)
	cv2.imwrite(filename,cv2.cvtColor(img, cv2.COLOR_RGB2BGR))

def matching(img1, kp1, des1, img2, kp2, des2 ):

	bf = cv2.BFMatcher()
	matches = bf.knnMatch(des1,des2, k=2)
	mathing_points = []
	for m,n in matches:
		if m.distance / n.distance < 0.75:
			mathing_points.append([m])
	img3 = None		
	img3 = cv2.drawMatchesKnn(img1,kp1,img2,kp2,mathing_points[:500], img3, flags=2)
	cv2.imwrite('matching_river.png',cv2.cvtColor(img3, cv2.COLOR_RGB2BGR))
	


if __name__ == "__main__":	
	img1 = np.fromfile('river1.raw', dtype=np.uint8)
	img1.shape = (1024,768,3)
	img2 = np.fromfile('river2.raw', dtype=np.uint8)
	img2.shape = (1024,768,3)
	
	
	[key_point_1, des_1] = SIFT(img1)
	num_of_keypoint_1 = len(key_point_1)
	print("number of key point extract from river1.raw: ",num_of_keypoint_1)
	
	[key_point_2, des_2] = SIFT(img2)
	num_of_keypoint_2 = len(key_point_2)
	print("number of key point extract from river2.raw: ",num_of_keypoint_2)
	
	
	[img_coor_1,largest_scale_idx] = largest_scale(key_point_1,des_1)
#	img_idx_1 = cartesian_to_Image(img1,img_coor_1)
	largest_scale_des_1 = des_1[largest_scale_idx,:]
	print("The index of the largest scale key point in river1.raw: ", img_coor_1)
	print("The orientation of the largest scale key point in river1.raw: ", key_point_1[largest_scale_idx].angle)
	print("The diameter of of the largest scale key point neighbourhood in river1.raw: ", key_point_1[largest_scale_idx].size)
	circle_point(img_coor_1, img1, (255, 0, 255), key_point_1[largest_scale_idx].angle, key_point_1[largest_scale_idx].size, "river1_largest.png")
	
	
	nearest_in_img2 = nearest_neighbour(largest_scale_des_1, key_point_2, des_2,2)
#	print(nearest_in_img2)
	for idx in nearest_in_img2:
		print("nearest point in river2: ", key_point_2[idx].pt)
		print("the orintation of the nearest point in river2: ", key_point_2[idx].angle)
		print("The diameter of of the largest scale key point neighbourhood in river1.raw: ",  key_point_2[idx].size)

		circle_point(key_point_2[idx].pt, img2, (255, 0, 255), key_point_2[idx].angle, key_point_2[idx].size, "2nn_river2.png")
		
	matching(img1, key_point_1, des_1, img2, key_point_2, des_2 )
		