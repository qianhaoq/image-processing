- USC EE569 Homework Assignments
- Date: Feb - Mar, 2017
- Name: Xuejing Lei
- Email: xjlei1117@gmail.com

## Makefile: for make command.
  Use " ‘make all’ " to compile and run all files. 
  Use " ‘make basic’ " to compile and run files of basic manipulations. 
  Use " ‘make histogram’ " to compile and run files of histogram equalization and transformation. 
  Use " ’make denoising‘ " to compile and run files of denoising.
  Use " ‘make geommatch’ " to compile and run files of geometrical warping. 
  Use " ’make puzzlematch‘ " to compile and run files of puzzle matching. 
  Use " ‘make homogtrans’ " to compile and run files of homographic transformation and image overlay. 
  Use " ’make halftoning‘ " to compile and run files of digital halftoning. 
  Use " ‘make morphproce’ " to compile and run files of morphological processing.

## basic_func_mrc.cpp: 
  Implementation of HW1 Problem 1 part (a) Mirroring, Resizing and Compositing. It can mirror the Dog image across the vertical axis and resize image to 3 different sizes and composite two images.

## basic_func_CSTLB.cpp: 
  Implementation of HW1 Problem 1 part (b) RGB to CMY and HSL Color space transformation and (c) Screen Layer blending mode implementation.

## hist_equa_gray.cpp: 
  Implementation of HW1 Problem 2 part (a) two Histogram Enhancement methods for grayscale images.
  - Method A: the linear transfer-function-based histogram enhancement method
  - Method B: the cumulative-probability-based histogram equalization method

## hist_equa_color.cpp: 
  Implementation of HW1 Problem 2 part (b) three Histogram Equalization methods for color images.
  - Method A: the linear transfer-function-based histogram enhancement method
  - Method B: the cumulative-probability-based histogram equalization method
  - Method C: preserve both hue and saturation histogram distribution and equalize brightness

## hist_trans.cpp:
  Implementation of HW1 Problem 2 part (c) Histogram transform which transforms the histogram of an input image to normal distribution.

## denoising_basic_filters.cpp: 
  Implementation of HW1 Problem 3 part (a) Remove mixed noises in color image using basic filters

## denoising_NLM.cpp: 
  Implementation of HW1 Problem 3 part (b) Remove mixed noises in color image using non-local means filter. 

## geom_warp.cpp:
  Implementation of Problem 1 part (a) Geometrical warping using triangle control points

## puzzle_match.cpp:
  Implementation of Problem 1 part (b) Puzzle matching. Apply geometric transformations to each puzzle piece and put them back to the correct positions.

## homog_trans.cpp: 
  Implementation of Problem 1  part (c) Homographic transformation and image overlay.

## halftoning.cpp: 
  Implementation of Problem 2 Digital Halftoning part (a) Dithering Matrix and (b) Error diffusion.

## morph_proce.cpp: 
  Implementation of Problem 3 Morphological Processing part (a) Shrinking, (b) Thinning, (c) Skeletonizing and (d) Counting game.

## Note:
  These programs are compiled and tested on Linux-ubuntu 16.04 (x86_64 Linux) with gcc version 5.4.0.
  It can also be runned successfully on windows 10 but Makefile may not work so that commands in Makefile can be used to compile on Windows 10 using MinGW GCC.
  If necessary, a unix shell "bash" on Windows 10 can be installed easily to set up the environment.
