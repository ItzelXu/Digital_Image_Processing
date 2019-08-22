clear all
K1 = -0.3536;
K2 =  0.1730;
u_c = (712/2) ;
v_c = (1072/2);

distoredImg = imread('classroom.png');
new_img = zeros(712,1072);
p = 1.5;

x_cut = 356;
y_cut = 536;


% Mtx_Xd = zeros(3,100)

% Mtx_Xd = zeros(N,3);
% Mtx_Yd = zeros(N,3);

%% left-up:
% Mtx_Xd_left_up = zeros(356*536,3);
% Mtx_Yd_left_up = zeros(356*536,3);
i = 1;
for x = 1:x_cut
    for y = 1:y_cut
        
        x_c = (x - u_c)/600;
		y_c = (y - v_c)/600;

        Xd = x_c * (1 + K1 * (x_c^2 + y_c^2) + K2 * (x_c^2 + y_c^2)^2);
        Yd = y_c * (1 + K1 * (x_c^2 + y_c^2) + K2 * (x_c^2 + y_c^2)^2);
    
    
        Mtx_Xd_left_up(i,:) = [x_c, y_c, Xd];
        Mtx_Yd_left_up(i,:) = [x_c, y_c, Yd];
        
        i = i + 1;
    end


end

[Yb_left_up,BINT,Rb_Y_left_up] = regress(Mtx_Yd_left_up(:,3),Mtx_Yd_left_up(:,1:2));
[Xb_left_up,BINT,Rb_X_left_up] = regress(Mtx_Xd_left_up(:,3),Mtx_Xd_left_up(:,1:2));


% 
% for x = 1:356
%     for y = 1:536
%         x_c = (x - u_c)/600;
% 		y_c = (y - v_c)/600;
% 
%         Xd_new = [x_c,y_c] * Xb_left_up + mean(Rb_X_left_up);
%         Yd_new = [x_c,y_c] * Yb_left_up + mean(Rb_Y_left_up);
%         
%         Xd_new = p * Xd_new;
%         Yd_new = p * Yd_new;
%         
%         u = 600 * Xd_new + u_c;
% 		v = 600 * Yd_new + v_c;
%         
%         if int64(u) <= 712 && int64(u) >= 1 
%             if int64(v) <= 1072 && int64(v) >= 1
%                 new_img(x,y) = distoredImg(int64(u),int64(v));
%             end
%         end
%     end
% end
    
%% left-down

% Mtx_Xd_left_down = zeros(356*536,3);
% Mtx_Yd_left_down = zeros(356*536,3);

i = 1;
for x = 712-x_cut+1:712
    for y = 1:y_cut
        
        x_c = (x - u_c)/600;
		y_c = (y - v_c)/600;

        Xd = x_c * (1 + K1 * (x_c^2 + y_c^2) + K2 * (x_c^2 + y_c^2)^2);
        Yd = y_c * (1 + K1 * (x_c^2 + y_c^2) + K2 * (x_c^2 + y_c^2)^2);
    
    
        Mtx_Xd_left_down(i,:) = [x_c, y_c, Xd];
        Mtx_Yd_left_down(i,:) = [x_c, y_c, Yd];
        
        
        i = i + 1;
    end


end

[Yb_left_down,BINT,Rb_Y_left_down] = regress(Mtx_Yd_left_down(:,3),Mtx_Yd_left_down(:,1:2));
[Xb_left_down,BINT,Rb_X_left_down] = regress(Mtx_Xd_left_down(:,3),Mtx_Xd_left_down(:,1:2));



% for x =  712-x_cut+1:712
%     for y = 1:y_cut
%         x_c = (x - u_c)/600;
% 		y_c = (y - v_c)/600;
% 
%         Xd_new = [x_c,y_c] * Xb_left_down + mean(Rb_X_left_down);
%         Yd_new = [x_c,y_c] * Yb_left_down + mean(Rb_Y_left_down);
%         
%         Xd_new = p * Xd_new;
%         Yd_new = p * Yd_new;
%         
%         u = 600 * Xd_new + u_c;
% 		v = 600 * Yd_new + v_c;
%         
%         if int64(u) <= 712 && int64(u) >= 1 
%             if int64(v) <= 1072 && int64(v) >= 1
%                 new_img(x,y) = distoredImg(int64(u),int64(v));
%             end
%         end
%     end
% end


%% right-up

% Mtx_Xd_right_up = zeros(356*536,3);
% Mtx_Yd_right_up = zeros(356*536,3);

i = 1;
for x = 1:x_cut
    for y = 1072-y_cut+1:1072
        
        x_c = (x - u_c)/600;
		y_c = (y - v_c)/600;

        Xd = x_c * (1 + K1 * (x_c^2 + y_c^2) + K2 * (x_c^2 + y_c^2)^2);
        Yd = y_c * (1 + K1 * (x_c^2 + y_c^2) + K2 * (x_c^2 + y_c^2)^2);
    
    
        Mtx_Xd_right_up(i,:) = [x_c, y_c, Xd];
        Mtx_Yd_right_up(i,:) = [x_c, y_c, Yd];
        
        
        i = i + 1;
    end


end

[Yb_right_up,BINT,Rb_Y_right_up] = regress(Mtx_Yd_right_up(:,3),Mtx_Yd_right_up(:,1:2));
[Xb_right_up,BINT,Rb_X_right_up] = regress(Mtx_Xd_right_up(:,3),Mtx_Xd_right_up(:,1:2));

% 
% 
% for x = 1:x_cut
%     for y = 1072-y_cut+1:1072
%         x_c = (x - u_c)/600;
% 		y_c = (y - v_c)/600;
% 
%         Xd_new = [x_c,y_c] * Xb_right_up + mean(Rb_X_right_up);
%         Yd_new = [x_c,y_c] * Yb_right_up + mean(Rb_Y_right_up);
%         
%         Xd_new = p * Xd_new;
%         Yd_new = p * Yd_new;
%         
%         u = 600 * Xd_new + u_c;
% 		  v = 600 * Yd_new + v_c;
%         
%         if int64(u) <= 712 && int64(u) >= 1 
%             if int64(v) <= 1072 && int64(v) >= 1
%                 new_img(x,y) = distoredImg(int64(u),int64(v));
%             end
%         end
%     end
% end

%% right-down

% Mtx_Xd_right_down = zeros(356*536,3);
% Mtx_Yd_right_down = zeros(356*536,3);
i = 1;
for x = 712-x_cut+1:712
    for y = 1072-y_cut+1:1072
        
        x_c = (x - u_c)/600;
		y_c = (y - v_c)/600;

        Xd = x_c * (1 + K1 * (x_c^2 + y_c^2) + K2 * (x_c^2 + y_c^2)^2);
        Yd = y_c * (1 + K1 * (x_c^2 + y_c^2) + K2 * (x_c^2 + y_c^2)^2);
    
    
        Mtx_Xd_right_down(i,:) = [x_c, y_c, Xd];
        Mtx_Yd_right_down(i,:) = [x_c, y_c, Yd];
        
 
        
        i = i + 1;
    end


end

[Yb_right_down,BINT,Rb_Y_right_down] = regress(Mtx_Yd_right_down(:,3),Mtx_Yd_right_down(:,1:2));
[Xb_right_down,BINT,Rb_X_right_down] = regress(Mtx_Xd_right_down(:,3),Mtx_Xd_right_down(:,1:2));



for x = 1:712
    for y = 1:1072
        x_c = (x - u_c)/600;
		y_c = (y - v_c)/600;

        Xd_new = [x_c,y_c] * Xb_right_down + mean(Rb_X_right_down);
        Yd_new = [x_c,y_c] * Yb_right_down + mean(Rb_Y_right_down);
        
        Xd_new = [x_c,y_c] * Xb_left_up + mean(Rb_X_left_up);
        Yd_new = [x_c,y_c] * Yb_left_up + mean(Rb_Y_left_up);   
        
        Xd_new = [x_c,y_c] * Xb_right_up + mean(Rb_X_right_up);
        Yd_new = [x_c,y_c] * Yb_right_up + mean(Rb_Y_right_up);
        
        Xd_new = [x_c,y_c] * Xb_left_down + mean(Rb_X_left_down);
        Yd_new = [x_c,y_c] * Yb_left_down + mean(Rb_Y_left_down);   
        
        
        Xd_new = p * Xd_new;
        Yd_new = p * Yd_new;
        
        u = 600 * Xd_new + u_c;
		v = 600 * Yd_new + v_c;
        
        if int64(u) <= 712 && int64(u) >= 1 
            if int64(v) <= 1072 && int64(v) >= 1
                new_img(x,y) = distoredImg(int64(u),int64(v));
            end
        end
    end
end

        


imwrite(uint8(new_img),"recovered_class_two_corner.png")

%% fuction mapping

% new_img_funtion = zeros(712,1072);
% for i = 1:712
%     for j = 1:1072
% %      img to camera
% 
% 		x = (i-u_c)/600;
% 		y = (j-v_c)/600;
%         
%         
%         Xd_new = x * (1 + K1 * (x^2 + y^2) + K2 * (x^2 + y^2)^2);
%         Yd_new = y * (1 + K1 * (x^2 + y^2) + K2 * (x^2 + y^2)^2);
%         
%         Xd_new = p * Xd_new;
%         Yd_new = p * Yd_new;
% %       camera to img
%         
% 
% 		u = 600 * Xd_new + u_c;
% 		v = 600 * Yd_new + v_c;
%         
%         if int64(u) <= 712 && int64(u) >= 1
%             if int64(v) <= 1072 && int64(v) >= 1
%                 new_img_funtion(i,j) = distoredImg(int64(u),int64(v));
%             else
%             new_img_funtion(i,j) = 0;
%             end
%         end
%     end
% end
% 
% imwrite(uint8(new_img_funtion),"recovered_class_fuction.png")

    