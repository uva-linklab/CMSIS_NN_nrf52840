# Number Recognition for Sign Language using CIMSIS NN 

The purpose of this repository is to provide a full example for NN deployment on an edge device using the CMSIS NN library provided by ARM using the GCC toolchain. 
Our main project attempts to deploy a CNN for Sign Language Recognition for Digits on an NRF52840.
The original dataset can be found at https://www.kaggle.com/ardamavi/sign-language-digits-dataset
We provide a tutorial for using the CIFAR10 example provided by CMSIS with the gcc embedded toolchain. The CMSIS NN component is designed to work with Cortex-M series processors, mostly intended for Cortex-M4 and Cortex-M7. 
The documentation for the CMSIS library is available at : http://www.keil.com/pack/doc/CMSIS_Dev/NN/html/index.html
A tutorial for using CMSIS NN for the CIFAR10 problem with the GCC toolchain can be found in the `cifar10_gcc_tutorial` folder.

For more information on this project please check the pdf of the given presentation and the detailed pdf report.

###Requirements:
- NRF52840 board 
- A computer capable of running arm-gcc cross-compiler
- Text editor
- ARM embedded toolchain
- make
- J-Link tools by Segger
- NRF52x SDK by Nordic
- NRF5x Command Line tools
- CMSIS Library

###Contents:
- `presentation_ie3ne.pdf`: brief presentation
- `report_ie3ne.pdf`: detailed report of the project.
- `cifar10_gcc_tutorial/`: tutorial for using the gcc toolchain with the CMSIS Neural Network component.
- `Number-Recognition-for-Sign-Language/`:
	- `arm_sign_nb_64`: contains the C/C++ compatible with the CMSIS library for the first attempt at the problem with 64x64 input images. (Does not work because of the number of parameters, it is only included for reference)
	- `arm_sign_nb_45`: contains the C/C++ compatible with the CMSIS library for the second more succesfull attempt at the problem with 45x45 input images. This version runs but there is an issue with the way the weights were quantized and stored in the weights header. 
	- `sign_language_dataset_color`: contains the dataset's formatted images in `Dataset/x_45.csv` and `Dataset/y_45` where x contains the images and y the labels. (`x_45.csv` contains 45x45 images whereas `x.csv` contains 64x64 images)
	- `tensorboard_logs`: logs for Tensorboard visualization
	- `cnn_models_graphics`: graphic representing the model architectures we tested. One with a `64 x 64` input and the other with a 45x45 input layer and different dense layers. 
	- `output_64`: best model weights for architecture with 64x64 input architecture. 
	- `output_45`: best model weights for architecture with 45x45 input architecture. 
	- `best_weights`: contains the selected weights for the two architecture. 
	- `sign_nb.ipynb`: python file containing the formatting of the images, visualization of the dataset, model training and finally generating the CMSIS compatible `inputs.h` file.
	- `quantization.ipynb`: python file that loads the weights and attempts to do quantization and stores the modified weights with their parameters in the `weights.h` file . (Note that the implementation of quantization in this code is incorrect)