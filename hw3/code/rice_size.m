clear all;
% load("label_rice.txt");

img = imread("rice_threhold_denoise.png");
% img = rgb2gray(img)
% clusters = zeros(500*690,3);
k = 1;
imgoutput = zeros(500,690);
for i = 1:500
    for j = 1:690
        if img(i,j) == 255
            clusters(k,:) = [i,j];
            imgoutput(i,j) = img(i,j);
            k = k + 1;
        end
    end
end



[IDX, C] = kmeans(clusters, 11);
for label = 1:11
    size = sum(IDX == label);
    rank(label,:) = [label,size];
end

for i = 1:11
    img_1(clusters(find(IDX == i),1),clusters(find(IDX == i),2)) = i * 20;
end
imwrite(uint8(img_1),"peer.png")

% imwrite(uint8(imgoutput),"tune.png")
% for i = 1
%     
%     [x, y] = find(label_rice == i);
%     
%     x_c = y - 0.5;
%     y_c = - x + 500 + 0.5;
%     
%     x_means = mean(x_c);
%     y_means = mean(y_c);
%     x_var = var(x_c);
%     y_var = var(y_c);
%     x_norm = (x_c - x_means)/x_var;
%     y_norm = (y_c - y_means)/y_var;
%     
% %     A = [x_norm,y_norm];
% %     [U,D,V] = svds(A,1);
%     s = regress(y_norm,x_norm);
%     
% %     Eval = U*D*V';
%     
% %     idx1 = min(find(Eval(:,2) == max(Eval(:,2))));
% %     idx2 = min(find(Eval(:,2) == min(Eval(:,2))));
%     
% %     k = (Eval(idx2,2) - Eval(idx1,2))/(Eval(idx2,1) - Eval(idx1,1))
% %     k = 
% %     b = y_means - k * x_means + Eval(idx1,2) - k * Eval(idx1,1)
%     
%     h = max(x_c) - min(x_c);
%     
%     length(i) = sqrt((h*s)^2 + h^2);
% %     length(i) = sqrt((h*k(1))^2 + h^2)
%     
%     %PCA
% end
[B,I] = sort(rank(:,2));
    


