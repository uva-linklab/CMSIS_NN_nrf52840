# Number Recognition for Sign Language

The purpose of this repository is to provide a full example for NN deployment on an edge device using the CMSIS NN library provided by ARM using the GCC toolchain. 
Our main project attempts to deploy a CNN for Sign Language Recognition for Digits on an NRF52840.
The original dataset can be found at https://www.kaggle.com/ardamavi/sign-language-digits-dataset
We provide a tutorial for using the CIFAR10 example provided by CMSIS with the gcc embedded toolchain. The CMSIS NN component is designed to work with Cortex-M series processors, mostly intended for Cortex-M4 and Cortex-M7. 
The documentation for the CMSIS library is available at : http://www.keil.com/pack/doc/CMSIS_Dev/NN/html/index.html

Requirements:
- NRF52840 board 
- A computer capable of running arm-gcc cross-compiler
- Text editor
- ARM embedded toolchain
- make
- J-Link tools by Segger
- NRF52x SDK by Nordic
- NRF5x Command Line tools
- CMSIS Library

Contents:
- 