import pickle
import numpy as np
import cv2
from skimage.util.shape import view_as_windows
import data
from tensorflow.python.platform import flags



def Compute_PSNR(img, img_reconst):
	#  output PSNR
	mse = np.mean(np.square(img_reconst-img))
	psnr =10*np.log10(255**2/mse)
	return psnr
	

def getfeature(img,k0,k1,b1):
	patch = view_as_windows(img,(4, 4),step=(4, 4)).reshape(8,8,16)

	patch = np.dot(patch, np.transpose(k0)) ## reduce dim
	print("Shape of patch in 1st stage :", patch.shape)

	patch_s2 = view_as_windows(patch.copy(),(4,4,1),step=(4,4,1))
	patch_s2 = patch_s2.reshape(2,2,s1 * 16)

	patch_s2 = patch_s2 + 1 / np.sqrt(s1 * s2) * b1
	patch_s2 = np.dot(patch_s2, np.transpose(k1))
	print("Shape of patch in 2nd stage :", patch_s2.shape)
	
	return patch_s2
	
def compute_inv(patch_s2,s1,s2,b1,k1,k0):
	inv_patch = np.dot(patch_s2, np.linalg.pinv(np.transpose(k1)))
	inv_patch = inv_patch - 1 / np.sqrt(s1*s2) * b1
	inv_patch = inv_patch.reshape(2,2,s1,16)
	inv_patch = np.moveaxis(inv_patch,2,3)
	print("Second stage inv_patch: ", inv_patch.shape)
	img_reconst_s1 = np.zeros((8,8,s1))

	for i in range(0,2):
		for j in range(0,2):
			for k in range(0,16):
				img_reconst_s1[i * 4 + k // 4,j * 4 + k % 4] = inv_patch[i][j][k]

	print("2nd stage reconstruct image: ", img_reconst_s1.shape)

	inv_patch = np.dot(img_reconst_s1, np.linalg.pinv(np.transpose(k0)))
	img_reconst = np.zeros((32,32))
	for i in range(0,8):
		for j in range(0,8):
			for k in range(0,16):
				img_reconst[i * 4 + k // 4, j * 4 + k % 4] = inv_patch[i][j][k]
	img_output = 255*(img_reconst-np.min(img_reconst))/(np.max(img_reconst)-np.min(img_reconst))
	
	return img_output



s1 = 6
s2 = 16

fr = open('./pca_params_compact.pkl', 'rb')
saab_weight = pickle.load(fr)
fr.close()

k0 = np.array(saab_weight['Layer_0/kernel'])
k1 = np.array(saab_weight['Layer_1/kernel'])

b1 = saab_weight['Layer_1/bias'].astype(np.float32)



img = cv2.imread('4.png',0)
patch_s2 = getfeature(img,k0,k1,b1)

img_output = compute_inv(patch_s2,s1,s2,b1,k1,k0)


#  save image
cv2.imwrite('_%d_%d.jpg'%(s1,s2),img_output)

psnr = Compute_PSNR(img, img_reconst);
print ( "The PSNR of reconstruct image is: ", psnr)

