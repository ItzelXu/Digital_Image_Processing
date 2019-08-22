%//(1) Name: Yifei Liu
%//(2) USC ID Number: 3852294243 
%//(3) USC Email: liu534@usc.edu
%//(4) Submission Date: 3/19/2019

% clear all
% %% load data
% X = load('segmentation.txt');
% Y = reshape(X',[510,510,25]);
% 
% for i = 1:25
%     Y(:,:,i) = Y(:,:,i)';
% end
% 
% %% normalization
% Y_norm = zeros(510,510,25);
% for k = 1:25
%     for i = 1:510
%         for j = 1:510
%             Y_norm(i,j,k) = Y(i,j,k) / Y(i,j,1);
%         end
%     end
% end
% 
% %% k-means prepare data
% num = 1;
% for i = 1:510
%     for j = 1:510
%         feature = reshape(Y_norm(i,j,:),[1,25]);
%         K(num,:) = [i j feature];
%         num = num + 1;
%     end
% end

%% PCA
    %%% normalize
    K_feature = K(:,3:27);
    for i = 1:25
        mu = mean(K_feature(:,i));
        vars = max(K_feature(:,i)) - min(K_feature(:,i));
        if vars == 0
            vars = 1;
        end
        K_norm(:,i) = K_feature(:,i) - mu;
        K_norm(:,i) = K_norm(:,i)./vars;
    end

    C = cov(K_norm);
    %% svds
    [U,D,V] = svds(C,7);
    K_recover = K_norm * U(:,1:7);

%% kmeans
% initial = [K(2,3:27);K(127515,3:27);K(213725,3:27);K(215149,3:27);K(129765,3:27);K(20145,3:27);K(50980,3:27)];
% [IDX, B] = kmeans(K(:,3:27), 7,'Start',initial, 'MaxIter',5000);
% 
initial = [K_recover(2,:);K_recover(127515,:);K_recover(213725,:);K_recover(215149,:);K_recover(129765,:);K_recover(20145,:);K_recover(50980,:)];
[IDX, B] = kmeans(K_recover, 7,'Start',initial, 'MaxIter',5000);

% initial = [K_norm(2,:);K_norm(127515,:);K_norm(213725,:);K_norm(215149,:);K_norm(129765,:);K_norm(20145,:);K_norm(50980,:)];
% [IDX, B] = kmeans(K_norm, 7,'Start',initial, 'MaxIter',5000);





%% put color in
newimg = zeros(510,510);

cor = K(find(IDX == 1),1:2);
for i = 1: size(cor,1)
    newimg(cor(i,1),cor(i,2)) = 0;
end

cor = K(find(IDX == 2),1:2);
for i = 1: size(cor,1)
    newimg(cor(i,1),cor(i,2)) = 42;
end

cor = K(find(IDX == 3),1:2);
for i = 1: size(cor,1)
    newimg(cor(i,1),cor(i,2)) = 84;
end

cor = K(find(IDX == 4),1:2);
for i = 1: size(cor,1)
    newimg(cor(i,1),cor(i,2)) = 126;
end

cor = K(find(IDX == 5),1:2);
for i = 1: size(cor,1)
    newimg(cor(i,1),cor(i,2)) = 168;
end

cor = K(find(IDX == 6),1:2);
for i = 1: size(cor,1)
    newimg(cor(i,1),cor(i,2)) = 210;
end

cor = K(find(IDX == 7),1:2);
for i = 1: size(cor,1)
    newimg(cor(i,1),cor(i,2)) = 255;
end

% newimg(K(find(IDX == 1),1:2)) = 0;
% 
% newimg(K(find(IDX == 2),1:2)) = 42;
% 
% newimg(K(find(IDX == 3),1:2)) = 84;
%     
% newimg(K(find(IDX == 4),1:2)) = 126;
%    
% newimg(K(find(IDX == 5),1),K(find(IDX == 5),2)) = 168;
% 
% newimg(K(find(IDX == 6),1:2)) = 210;
% 
% newimg(K(find(IDX == 7),1:2)) = 255;


imwrite(uint8(newimg),"texture_seg_pca_15.png")



