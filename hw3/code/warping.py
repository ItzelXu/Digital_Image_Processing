import cv2
from matplotlib import pyplot as plt
import numpy as np
import math

		
	
def Image_to_cartesian(img,i,j):
	index = np.array([[i+1],[j+1],[1]])
	cartesian = np.zeros((3,1))
	mtx = np.array([[0,1,-0.5],[-1,0,img.shape[0]+0.5],[0,0,1]])
	cartesian = mtx @ index
	
#	print(cartesian)
	return cartesian

def cartesian_to_Image(img,cartesian):
	cartesian = np.array([[cartesian[0]],[cartesian[1]],[1]])
	idx = np.zeros((3,1))
	mtx = np.array([[0,-1,img.shape[0]+0.5],[1,0,0.5],[0,0,1]])
	idx = mtx @ cartesian - [[1],[1],[0]]
#	print(idx)
	
#	if idx[0] > img.shape[0] - 1:
#		idx[0] = img.shape[0] - 1
#	elif idx[0] < 0:
#		idx[0] = 0
#		
#	if idx[1] > img.shape[1] - 1:
#		idx[1] = img.shape[1] - 1
#	elif idx[1] < 0:
#		idx[1] = 0
		
#	print("idx:\n",idx)
	return idx




	
def geometric_manipulation(output_cartesian, inv_rotation_mtx):
	
	output_cartesian
	
	output_vector = np.zeros((6,))

	x = output_cartesian[0]
	y = output_cartesian[1]
	output_vector = [1,x,y,x**2,x*y,y**2]
	input_cartesian = inv_rotation_mtx @ output_vector
	
	return input_cartesian
	
def bilinear_interpolation(input_img,idx):
#	print(idx)
	if idx[0] == input_img.shape[0] - 1:
		if idx[1] == input_img.shape[1] - 1:	
			return input_img[int(idx[0])][int(idx[1])]
			
		else:
			w1 = 1 - (idx[1] - int(idx[1]))
			w2 = 1 - (int(idx[1]) + 1 - idx[1])
			return w1 * input_img[int(idx[0])][int(idx[1])] + w2 * input_img[int(idx[0])][int(idx[1]) + 1]
					
	elif idx[1] == (input_img.shape[1] - 1):
		
		w3 = 1 - (idx[0] - int(idx[0]))
		w4 = 1 - (int(idx[0]) + 1 - idx[0])
		
		return w3 * input_img[int(idx[0])][int(idx[1])] + w4 * input_img[int(idx[0]) + 1][int(idx[1])]
		
	else:
		w1 = 1 - (idx[1] - int(idx[1]))
		w2 = 1 - (int(idx[1]) + 1 - idx[1])
		w3 = 1 - (idx[0] - int(idx[0]))
		w4 = 1 - (int(idx[0]) + 1 - idx[0])
		p1 = input_img[int(idx[0])][int(idx[1])]
		p2 = input_img[int(idx[0])][int(idx[1]) + 1]
		p3 = input_img[int(idx[0]) + 1][int(idx[1])]
		p4 = input_img[int(idx[0]) + 1][int(idx[1]) + 1]
		
		
		return w3 * (w1 * p1 + w2 * p2) + w4 * (w1 * p3 + w2 * p4)
		
		
def comput_paras(input_point, output_point):

	input_point_cart = np.zeros((3,len(input_point))) #3*6
	output_point_cart = np.zeros((3,len(output_point))) #3*6
	
	output_mtx = np.zeros((6,len(output_point)))
	print("# of control point:",len(input_point))
	
	for i in range(len(input_point)):
		input_point_cart[:,i] = list(Image_to_cartesian(img,input_point[i][0],input_point[i][1]))
		output_point_cart[:,i] = list(Image_to_cartesian(img,output_point[i][0],output_point[i][1]))
		x = output_point_cart[0,i]
		y = output_point_cart[1,i]
		output_mtx[:,i] = list([1,x,y,x**2,x*y,y**2])
	


		
#	print(np.linalg.pinv(output_mtx))
	inv_warp_mtx = input_point_cart[0:2,:] @ np.linalg.pinv(output_mtx)
		
	print(inv_warp_mtx)
	
	return inv_warp_mtx
		
	
def main():
	input_point_up = np.array([[0,256],[0,512],[0,0],[256,256],[128,128],[128,384]])
	output_point_up = np.array([[128, 256], [0, 512],[0,0],[256,256],[128,128],[128,384]])
	
	input_point_down = np.array([[512,0],[512,512],[512,256],[384,128],[384,384],[256,256]])
	output_point_down = np.array([[512,0], [512, 512],[384,256],[384,128],[384,384],[256,256]])
	
	input_point_left = np.array([[256,0],[512,0],[0,0],[256,256],[128,128],[384,128]])
	output_point_left = np.array([[256,128], [512,0],[0,0],[256,256],[128,128],[384,128]])
	
	input_point_right = np.array([[256, 512],[0,512],[512,512],[256,256],[128,384],[384,384]])
	output_point_right = np.array([[256, 384], [0, 512],[512,512],[256,256],[128,384],[384,384]])

	inv_warp_mtx_up = comput_paras(input_point_up,output_point_up)
	inv_warp_mtx_down = comput_paras(input_point_down,output_point_down)
	inv_warp_mtx_left = comput_paras(input_point_left,output_point_left)
	inv_warp_mtx_right = comput_paras(input_point_right,output_point_right)
		
	
	
#	for i in range(512):
#		for j in range(0,min(i,512 - i)):
#			img_left[i,j] = img[i,j]
#		
#
#	cv2.imwrite("hat_left.png",img_left)

	
	img_w = np.full((img.shape[0],img.shape[1]),0) 
	
	for i in range(256):
		for j in range(i,512 - i):
			output_cartesian = Image_to_cartesian(img_w,i,j)
#			print(output_cartesian)
			input_cartesian = geometric_manipulation(output_cartesian[0:2], inv_warp_mtx_up) #[u,v]
#			print(input_cartesian)
			idx = cartesian_to_Image(img,input_cartesian) #
#			print(idx)
#			img_w[i][j] = img[int(idx[0]),int(idx[1])]
			if idx[0] >= 0 and idx[1] >= 0:
				img_w[i][j] = bilinear_interpolation(img,idx)
			else:
				img_w[i][j] = 0
#			print(img_w[i][j])

	for i in range(512):
		for j in range(0,min(i,512 - i)):
			output_cartesian = Image_to_cartesian(img_w,i,j)
#			print(output_cartesian)
			input_cartesian = geometric_manipulation(output_cartesian[0:2], inv_warp_mtx_left) #[u,v]
#			print(input_cartesian)
			idx = cartesian_to_Image(img,input_cartesian) #
#			print(idx)
#			img_w[i][j] = img[int(idx[0]),int(idx[1])]
			if idx[0] >= 0 and idx[1] >= 0:
				img_w[i][j] = bilinear_interpolation(img,idx)
			else:
				img_w[i][j] = 0
#			print(img_w[i][j])

	for i in range(512):
		for j in range(max(i,512 - i),512):
			output_cartesian = Image_to_cartesian(img_w,i,j)
#			print(output_cartesian)
			input_cartesian = geometric_manipulation(output_cartesian[0:2], inv_warp_mtx_right) #[u,v]
#			print(input_cartesian)
			idx = cartesian_to_Image(img,input_cartesian) #
#			print(idx)
#			img_w[i][j] = img[int(idx[0]),int(idx[1])]
			if idx[0] >= 0 and idx[1] >= 0 and idx[0] < 511 and idx[1] < 511:
				img_w[i][j] = bilinear_interpolation(img,idx)
			else:
				img_w[i][j] = 0
#			print(img_w[i][j])

	for i in range(256, 512):
		for j in range(512 - i,i):
			output_cartesian = Image_to_cartesian(img_w,i,j)
#			print(output_cartesian)
			input_cartesian = geometric_manipulation(output_cartesian[0:2], inv_warp_mtx_down) #[u,v]
#			print(input_cartesian)
			idx = cartesian_to_Image(img,input_cartesian) #
#			print(idx)
#			img_w[i][j] = img[int(idx[0]),int(idx[1])]
			if idx[0] >= 0 and idx[1] >= 0 and idx[0] < 511 and idx[1] < 511:
				img_w[i][j] = bilinear_interpolation(img,idx)
			else:
				img_w[i][j] = 0
#			print(img_w[i][j])


			
	cv2.imwrite("hat_w_left.png",img_w)

	
		
		
if __name__ == "__main__":
	
	img = np.fromfile('hat.raw', dtype=np.uint8)
	img.shape = (512,512)
	

	main()