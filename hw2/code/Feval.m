//(1) Name: Yifei Liu
//(2) USC ID Number: 3852294243 
//(3) USC Email: liu534@usc.edu
//(4) Submission Date: 1/22/2019


% clear all

% tune to binary
n = 100
G = load('Pig.mat');
F = zeros(n,1);
for iter = 1:n
    Binary = E;
    for i = 1:321
        for j = 1:481
            if E(i,j) > 0.01 * iter
                Binary(i,j) = 0;
            else
                Binary(i,j) = 1;
            end
        end
    end

    % imshow(Binary)

    
    % RGB = imread('pig_structured_0.92.png');
    % img = rgb2gray(RGB);
    P = zeros(5,1)
    R = zeros(5,1)


    for i = 1:5
        G1 = G.groundTruth{i}
        G1 = {G1}
        [thrs,cntR,sumR,cntP,sumP,V]=edgesEvalImg(1-Binary,G1);

    %  prms       - parameters (struct or name/value pairs)
    %   .out        - [''] optional output file for writing results
    %   .thrs       - [99] number or vector of thresholds for evaluation
    %   .maxDist    - [.0075] maximum tolerance for edge match
    %   .thin       - [1] if true thin boundary maps

        P(i) = cntP / sumP;
        R(i) = cntR / sumR;

    end

    mean_P = mean(P);
    mean_R = mean(R);

    F(iter) = 2 * mean_P * mean_R / (mean_P + mean_R)
end