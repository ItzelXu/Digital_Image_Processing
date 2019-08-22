% % pepper_dark = imread('pepper_dark.png');
% % pepper_dark = im2double(pepper_dark);
% % pepper_dark_noise = imread('pepper_dark.png');
% % pepper_dark_noise = im2double(pepper_dark_noise);
% 
% 
% 
% [PSNR, pepper_dark_BM3D] = BM3D(1, pepper_dark_trans,1,'lc',1);
% pepper_dark = im2double(pepper_dark);


for i = 1:256
    for j = 1:256
        pepper_dark_output_unbiased(i,j) = pepper_dark_BM3D_int(i,j))^2 /4 - 0.125;
    end
end
        
% pepper_dark_output_unbiased = (pepper_dark_BM3D).^2 ./ 4 - 1/8;
% 
% pepper_dark_output_biased = (pepper_dark_BM3D).^2 ./ 4 - 3/8;
% 
% PSNR_biased = psnr(pepper_dark_output_biased, pepper_dark);
% PSNR_unbiased = psnr(pepper_dark_output_unbiased, pepper_dark);
% 
% imwrite(pepper_dark_output_biased,'pepper_dark_output_biased.png');
imwrite(pepper_dark_output_unbiased,'pepper_dark_output_unbiased.png')




