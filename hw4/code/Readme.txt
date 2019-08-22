# EE569 Homework Assignment #1
# Date:January 22nd, 2019
# Name:Yifei Liu
# ID:3852-2942-43
# email:liu534@usc.edu
# Compiled on MAC OS with g++
# Define compiler and linker

	Readme

	Problem 1: Texture Analysis

	(a).Texture Classification
	run laws.cpp in coderunner
	input: texture1.raw testoutput.raw 1 128 128
		   texture2.raw testoutput.raw 1 128 128
		   texture3.raw testoutput.raw 1 128 128
		   texture4.raw testoutput.raw 1 128 128
		   texture5.raw testoutput.raw 1 128 128
		   texture6.raw testoutput.raw 1 128 128
		   texture7.raw testoutput.raw 1 128 128
		   texture8.raw testoutput.raw 1 128 128
		   texture9.raw testoutput.raw 1 128 128
		   texture10.raw testoutput.raw 1 128 128
		   texture11.raw testoutput.raw 1 128 128
		   texture12.raw testoutput.raw 1 128 128
	open Matlab 9.5.0.1033004 (R2018b) Update 2
	run: texture_classification.m

	(b).Texture Segmentation
	run texture_segmentation.cpp in coderunner
	input: comb.raw Imagedata_filted.raw 1 510 510
	open MATLAB 9.5.0.1033004 (R2018b) Update 2
	run: image_segmentation.m
	
	Problem 2: Image Feature Extractor 

	(b).Image Matching
	open terminal
	input: python3 img_match.py

	(c).Bag of Words
	open terminal
	input: python3 bag_of_word.py
