%//(1) Name: Yifei Liu
%//(2) USC ID Number: 3852294243 
%//(3) USC Email: liu534@usc.edu
%//(4) Submission Date: 3/19/2019

clear all;
load("lawsvector_RMS.mat");
[m,n]=size(X);
% X = X.^(1/2);
%% normalize
for i = 1:25
    mu = mean(X(:,i));
    vars = std(X(:,i));
    
    X_norm(:,i) = X(:,i) - mu;
    X_norm(:,i) = X_norm(:,i)./vars;
end

C = cov(X_norm);

 


%% svds
[U,D,V] = svds(C,3);
X_recover = X_norm * U(:,1:3);

initial = [X_recover(1,:);X_recover(2,:);X_recover(3,:);X_recover(4,:)];

% [IDX, C] = kmeans(X_norm, 4);
[IDX, B] = kmeans(X_recover, 4,'Start',initial);

IDX
% %% write file
% fid = fopen('pca_feature.txt','wt');
% 
% for i = 1:m
%     for j = 1:n
%         if j == n
%             fprintf(fid,'%g\n',X_recover(i,j));
%         else
%             fprintf(fid,'%g\t',X_recover(i,j));
%         end
%     end
% end
% 
% fclose(fid);

