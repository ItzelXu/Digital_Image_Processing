import cv2
from matplotlib import pyplot as plt
import numpy as np
import math
import rawpy
import imageio

def Findvertices(img1):
	vertices_right = []
	vertices_left = []
	vertices_up = []
	vertices_down = []
	
	temp_right_i = 0
	temp_right_j = 0
	temp_left_i = 0
	temp_left_j = img1.shape[1]
	temp_up_i = img1.shape[0]
	temp_up_j = 0
	temp_down_i = 0
	temp_down_j = 0
	p = 4
	for i in range(img1.shape[0]):
		for j in range(img1.shape[1]):

			if img1[i][j] != 255:
				
				if img1[i-p][j] == 255 and img1[i+p][j] == 255 and img1[i][j+p] == 255: #right
					if j > temp_right_j:
						temp_right_i = i
						temp_right_j = j
				elif img1[i-p][j] == 255 and img1[i+p][j] == 255 and img1[i][j-p] == 255: #left
					if j < temp_left_j:
						temp_left_i = i
						temp_left_j = j
				elif img1[i][j-p] == 255 and img1[i][j+p] == 255 and img1[i-p][j] == 255: #up 
					if i < temp_up_i:
						temp_up_i = i
						temp_up_j = j

				elif img1[i][j-p] == 255 and img1[i][j+p] == 255 and img1[i+p][j] == 255: #down
					if i > temp_down_i:
						temp_down_i = i
						temp_down_j = j

	vertices_right.append(temp_right_i)
	vertices_right.append(temp_right_j)
	vertices_left.append(temp_left_i)
	vertices_left.append(temp_left_j)
	vertices_up.append(temp_up_i)
	vertices_up.append(temp_up_j)
	vertices_down.append(temp_down_i)
	vertices_down.append(temp_down_j)
		
	return vertices_right,vertices_left,vertices_up,vertices_down
	
def findhull(img):
	
	for i in range(img.shape[0]):
		for j in range(img.shape[1]):
			if img[i][j] < 255:
				img[i][j] = 0;
			else:
				img[i][j] = 1

				
	vertices_right_up = []
	vertices_left_up = []
	vertices_right_down = []
	vertices_left_down = []
	
	Window_leftup = np.array([[0,0,0,0,0],[0,0,0,0,0],[0,0,1,1,1],[0,0,1,1,1],[0,0,1,1,1]])
	Window_leftdown = np.array([[0,0,1,1,1],[0,0,1,1,1],[0,0,1,1,1],[0,0,0,0,0],[0,0,0,0,0]])
	Window_rightup = np.array([[0,0,0,0,0],[0,0,0,0,0],[1,1,1,0,0],[1,1,1,0,0],[1,1,1,0,0]])
	Window_rightdown = np.array([[1,1,1,0,0],[1,1,1,0,0],[1,1,1,0,0],[0,0,0,0,0],[0,0,0,0,0]])
	
#	print("Window_leftup:\n",Window_leftup)
	

	for i in range(2,img.shape[0]-2):
		for j in range(2,img.shape[1]-2):
			if img[i][j] == 1:
#				print(img[i-2:i+3,j-2:j+3])
				
#				print(sum(sum(np.dot(Window_leftup,img[i-2:i+3,j-2:j+3]))))
				
				if  np.array_equal(Window_leftup,img[i-2:i+3,j-2:j+3]):
					vertices_left_up.append(i)
					vertices_left_up.append(j)
#					print("Find Left up Vertix!")
				elif np.array_equal(Window_leftdown,img[i-2:i+3,j-2:j+3]):
					vertices_left_down.append(i)
					vertices_left_down.append(j)
#					print("Find Left down Vertix!")
				elif np.array_equal(Window_rightup,img[i-2:i+3,j-2:j+3]):
					vertices_right_up.append(i)
					vertices_right_up.append(j)
#					print("Find Right up Vertix!")
				elif np.array_equal(Window_rightdown,img[i-2:i+3,j-2:j+3]):
					vertices_right_down.append(i)
					vertices_right_down.append(j)
#					print("Find Right down Vertix!")
					

			
	return vertices_right_up, vertices_left_up, vertices_right_down, vertices_left_down
		
def caculate_paras(right,left,up,down):
	theta = []
	central_i = []
	central_j = []
	width = []
	height = []
	central = np.zeros((2,))
	
	width.append(np.sqrt((left[0] - up[0])**2 + (left[1] - up[1])**2))
	width.append(np.sqrt((right[0] - down[0])**2 + (right[1] - down[1])**2))
	
	height.append(np.sqrt((left[0] - down[0])**2 + (left[1] - down[1])**2))
	height.append(np.sqrt((right[0] - up[0])**2 + (right[1] - up[1])**2))
	
	central_i.append((left[0]+right[0])/2)
	central_i.append((up[0] + down[0])/2)
	central_j.append((left[1]+right[1])/2)
	central_j.append((up[1] + down[1])/2)
	central[0] = np.mean(central_i)
	central[1] = np.mean(central_j)
	
	theta.append(np.arctan((down[0] - left[0])/(down[1] - left[1])))
	theta.append(np.arctan((right[0] - up[0])/(right[1] - up[1])))
	theta.append(np.arctan((up[1] - left[1])/(left[0] - up[0])))
	theta.append(np.arctan((right[1] - down[1])/(down[0] - right[0])))
	
	return np.mean(width), np.mean(height), central, np.mean(theta)
	
	
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


def caculate_rotation_mtx(img, theta, central):
	central = Image_to_cartesian(img, central[0], central[1])
	rotation_mtx = np.array([[1,0,central[0]],[0,1,central[1]],[0,0,1]]) @ np.array([[np.cos(theta),-np.sin(theta),0],[np.sin(theta),np.cos(theta),0],[0,0,1]]) @ np.array([[1,0,-central[0]],[0,1,-central[1]],[0,0,1]])
	inv_rotation_mtx = np.linalg.inv(rotation_mtx)
	print("rotation_mtx: \n",rotation_mtx)
#	print("inv_rotation_mtx: \n",inv_rotation_mtx)
	
	return inv_rotation_mtx
	


def caculate_scaling_mtx(img, height,width,hull_height,hull_width,central):
	central = Image_to_cartesian(img, central[0], central[1])	
	
	S_x = abs(hull_width / width)
	S_y = abs(hull_height / height)

#	print("S_x:",S_x)
#	print("S_y:",S_y)
	
	scaling_mtx = np.array([[1,0,central[0]],[0,1,central[1]],[0,0,1]]) @ np.array([[S_x,0,0],[0,S_y,0],[0,0,1]]) @ np.array([[1,0,-central[0]],[0,1,-central[1]],[0,0,1]])
	print("scaling_mtx:\n", scaling_mtx)
	inv_scaling_mtx = np.linalg.inv(scaling_mtx)

	return S_x, S_y, inv_scaling_mtx
	
def caculate_translation_mtx(img_seg, img, central, hull_center):
	central = Image_to_cartesian(img_seg, central[0], central[1])
	hull_center = Image_to_cartesian(img, hull_center[0], hull_center[1])

	T_x = hull_center[0] - central[0]
	T_y = hull_center[1] - central[1]
	
	translation_mtx = np.array([[1,0,central[0]],[0,1,central[1]],[0,0,1]]) @ np.array([[1,0, T_x],[0,1,T_y],[0,0,1]]) @ np.array([[1,0,-central[0]],[0,1,-central[1]],[0,0,1]])

	inv_translation_mtx = np.linalg.inv(translation_mtx)
	
	print(translation_mtx)
	
	
	return inv_translation_mtx
	
	
def geometric_manipulation(output_cartesian, inv_rotation_mtx):
	input_cartesian = inv_rotation_mtx @ output_cartesian
	
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
		
		
			
	
def main():


	[right_up, left_up, right_down, left_down] = findhull(img)
	
	
	print("right_up:", right_up)
	print("left_up: ", left_up)
	print("right_down: ", right_down)
	print("left_down: ", left_down)
	
	#calculate para of img1
	[right_1,left_1,up_1,down_1] = Findvertices(img1)
	[width_1, height_1, central_1, theta_1] = caculate_paras(right_1,left_1,up_1,down_1)
	
	hull_width_2 = right_up[1] - left_up[1] + 1
	hull_height_2 = right_down[0] - right_up[0] + 1
	hull_center_2 = np.array([(right_down[0] + right_up[0])/2, (right_up[1] + left_up[1])/2])
	 
	
	
	
	hull_width_1 = right_up[3] - left_up[3] + 1
	hull_height_1 = right_down[2] - right_up[2] + 1
	hull_center_1 = np.array([(right_down[2] + 1 + right_up[2])/2, (right_up[3] + 1 + left_up[3])/2]) 
	
	mark_1 = np.mean(img[left_up[2],left_up[3]:right_up[3]])

	
	hull_width_3 = right_up[5] - left_up[5] + 1
	hull_height_3 = right_down[4] - right_up[4] + 1
	hull_center_3 = np.array([(right_down[4] + 1 + right_up[4])/2, (right_up[5] + 1 + left_up[5])/2]) 

	
	
	
	print("hull_width:",hull_width_1)
	print("hull_height:",hull_height_1)
	print("hull_center:",hull_center_1)
	
	print("lighthouse1:")
	print("right:",right_1)
	print("left:",left_1)
	print("up:",up_1)
	print("down:",down_1)
	
	print("width_1:", width_1)
	print("height_1:",height_1)
	print("central_1:",central_1)
	print("theta_1:",theta_1)
	print("\n")

	
	#rotate img1
	
	
	inv_rotation_mtx_1 = caculate_rotation_mtx(img1,theta_1 - math.pi / 2 ,central_1)
	[S_x_1, S_y_1, inv_scaling_mtx_1] =  caculate_scaling_mtx(img1,height_1,width_1,hull_height_1,hull_width_1,central_1)
	inv_translation_mtx_1 = caculate_translation_mtx(img1, img, central_1, hull_center_1)
	transform_mtx_1 = inv_rotation_mtx_1 @ inv_scaling_mtx_1 @ inv_translation_mtx_1

	
	img1_t = np.full((img.shape[0],img.shape[1]),255) 

	
	for i in range(img1_t.shape[0]):
		for j in range(img1_t.shape[1]):
			output_cartesian = Image_to_cartesian(img1_t,i,j)
			input_cartesian = geometric_manipulation(output_cartesian, transform_mtx_1)
			idx = cartesian_to_Image(img1,input_cartesian)
			img1_t[i][j] = bilinear_interpolation(img1,idx)
			
	
#	#compute mark and fill the hole
#	puzzle_mark = np.mean(img1_t[left_up[2],left_up[3]:right_up[3]])
#	while puzzle_mark != mark_1:
#		theta_1 = theta_1 - math.pi / 2
#		inv_rotation_mtx_1 = caculate_rotation_mtx(img1,theta_1 ,hull_center_1)
#		
#		for i in range(img1_t.shape[0]):
#			for j in range(img1_t.shape[1]):
#				output_cartesian = Image_to_cartesian(img1_t,i,j)
#				input_cartesian = geometric_manipulation(output_cartesian, inv_rotation_mtx_1)
#				idx = cartesian_to_Image(img1,input_cartesian)
#				img1_t[i][j] = bilinear_interpolation(img1,idx)
	
	
	


		
	cv2.imwrite("img1_t.png",img1_t)
	
		
#	calculate para of img2
	[right_2,left_2,up_2,down_2] = Findvertices(img2)
	[width_2, height_2, central_2, theta_2] = caculate_paras(right_2,left_2,up_2,down_2)	
	
	print("lighthouse2:")
	print("right:",right_2)
	print("left:",left_2)
	print("up:",up_2)
	print("down:",down_2)
	
	print("width_2:", width_2)
	print("height_2:",height_2)
	print("central_2:",central_2)
	print("theta_2:",theta_2)
	print("\n")
	
#	rotate img2
	inv_rotation_mtx_2 = caculate_rotation_mtx(img2, theta_2 + math.pi ,central_2)
	[S_x_2, S_y_2, inv_scaling_mtx_2] =  caculate_scaling_mtx(img2,height_2,width_2,hull_height_2,hull_width_2,central_2)
	inv_translation_mtx_2 = caculate_translation_mtx(img2, img, central_2, hull_center_2)
	transform_mtx_2 = inv_rotation_mtx_2 @ inv_scaling_mtx_2 @ inv_translation_mtx_2
	
	img2_t = np.full((img.shape[0],img.shape[1]),255) 
	
	
	for i in range(img2_t.shape[0]):
		for j in range(img2_t.shape[1]):
			output_cartesian = Image_to_cartesian(img2_t,i,j)
			input_cartesian = geometric_manipulation(output_cartesian, transform_mtx_2)
			idx = cartesian_to_Image(img2,input_cartesian)
			img2_t[i][j] = bilinear_interpolation(img2,idx)
			
	cv2.imwrite("img2_rotated.png",img2_t)
		
#	calculate para of img3
	[right_3,left_3,up_3,down_3] = Findvertices(img3)
	[width_3, height_3, central_3, theta_3] = caculate_paras(right_3,left_3,up_3,down_3)	
	
	print("lighthouse3:")
	print("right:",right_3)
	print("left:",left_3)
	print("up:",up_3)
	print("down:",down_3)
	
	print("width_3:", width_3)
	print("height_3:",height_3)
	print("central_3:",central_3)
	print("theta_3:",theta_3)
	print("\n")
	
#	rotate img3
	inv_rotation_mtx_3 = caculate_rotation_mtx(img3, theta_3 ,central_3)
	[S_x_3, S_y_3, inv_scaling_mtx_3] =  caculate_scaling_mtx(img3,height_3,width_3,hull_height_3,hull_width_3,central_3)
	inv_translation_mtx_3 = caculate_translation_mtx(img3, img, central_3, hull_center_3)
	transform_mtx_3 = inv_rotation_mtx_3 @ inv_scaling_mtx_3 @ inv_translation_mtx_3
	
	img3_t = np.full((img.shape[0],img.shape[1]),255) 
	
	
	for i in range(img3_t.shape[0]):
		for j in range(img3_t.shape[1]):
			output_cartesian = Image_to_cartesian(img3_t,i,j)
			input_cartesian = geometric_manipulation(output_cartesian, transform_mtx_3)
			idx = cartesian_to_Image(img3,input_cartesian)
			img3_t[i][j] = bilinear_interpolation(img3,idx)
			
	cv2.imwrite("img3_rotated.png",img3_t)
	
	
	
	
	for i in range(157,316):
		for j in range(62,221):
			img_output[i][j] = img1_t[i][j]
			
	for i in range(31,190):
		for j in range(278,437):
			img_output[i][j] = img2_t[i][j]
			
	for i in range(328,487):
		for j in range(326,485):
			img_output[i][j] = img3_t[i][j]
			

			
	
	cv2.imwrite("lighthouse_filled.png",img_output)
#	imageio.imsave('lighthouse_filled.raw', img_output)
		
if __name__ == "__main__":
	
	img = np.fromfile('lighthouse.raw', dtype=np.uint8)
	img.shape = (512,512)
	
	img_output = np.fromfile('lighthouse.raw', dtype=np.uint8)
	img_output.shape = (512,512)
	#plt.imshow(img, cmap=plt.cm.gray)
	img1 = np.fromfile('lighthouse1.raw', dtype=np.uint8)
	img1.shape = (256,256)
	#plt.imshow(img, cmap=plt.cm.gray)
	img2 = np.fromfile('lighthouse2.raw', dtype=np.uint8)
	img2.shape = (256,256)
	#plt.imshow(img, cmap=plt.cm.gray)
	img3 = np.fromfile('lighthouse3.raw', dtype=np.uint8)
	img3.shape = (256,256)
	#plt.imshow(img, cmap=plt.cm.gray)

	main()