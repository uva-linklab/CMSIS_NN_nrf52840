# CMSIS_NN_nrf52840

The purpose of this repository is to provide a full example for NN deployment on an edge device using the CMSIS NN library provided by ARM using the GCC toolchain. 
For this example, we used an nrf62840 development board to test the CIFAR10 example provided by CMSIS. The CMSIS NN component is designed to work with Cortex-M series processors, mostly intended for Cortex-M4 and Cortex-M7. 
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

## Installation:

- ARM toolchain available at: https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads 
	- Depending on the OS you are running there may be a brew tap or apt-get ppa. For Windows, we recommend using the installer at developer.arm.com.
	- Notes: 
		Make sure the arm-none-eabi-gcc is added to your PATH. 
		For this example, we used the newest ARM compiler update: 7-2018-q2-update. 
		For Windows, we recommend using MSYS https://sourceforge.net/projects/mingw/files/ to provide the make command. 

- JLink tools: https://www.segger.com/downloads/jlink/
	- Download and install J-Link Software and Documentation Pack
	- This will provide you with the necessary tools for recognizing the board, program and debug.
	- One of these tools is the J-Link RTT viewer wich we recommend for debugging

- Nordic tools: https://www.nordicsemi.com/eng/Products/nRF52840-DK
	- download the SDK zip file : nRF5-SDK-zip. Extract the files and make sure you know where the folder was extracted.
		- In the SDK folder go to <SDK>/components/toolchain/gcc, you will find Makefile.posix (for Unix systems) and (Makefile.windows). Depending on your OS, edit the file which should contain the RooT installation of the gcc compiler on your computer, the version and prefix. In order to know the compiler version run: 
			```
			arm-none-eabi-cc --version
			```
		For reference this is what my Makefile.windows looks like:
			```
			GNU_INSTALL_ROOT := C:\ARM-GCC\7 2018-q2-update
			GNU_VERSION := 7.3.1
			GNU_PREFIX := arm-none-eabi
			```

	- download and install the nRF5x-Command-Line-Tools for your software platform. Confirm that it is a recognized command after installation. (if not you might need to add it your PATH or run it from the full path)

- CMSIS Library: https://github.com/ARM-software/CMSIS_5
	- download the CMSIS repository. We used CMSIS 5.

Now you should have all the tools necessary. We recommend you test the setup arm-gcc, SDK, etc by trying to compile/run the blinky example provided by Nordic in <SDK>/examples/peripheral/blinky/pca10056/blank/armgcc. 

## CIFAR10:
	Cifar10 is a classical image classification problem most commonly solved with a CNN. More information about the problem and the dataset can be found [here](https://www.cs.toronto.edu/~kriz/cifar.html ).
	In this section, we will explain the structre/contents of this repository and how to run the code.

	### Explanation:
	The code for the CMSIS-NN cifar10 implementation is located at :
	```
	CMSIS_5/CMSIS/NN/Examples/ARM/arm_nn_examples/cifar10/
	```

	- Comparing with the example provided by ARM. We will not be using RTE or Keil so we deleted the following files and folders:
	```
	 	RTE/
	 	EventRecorderStub.scvd
		arm_nnexamples_cifar10.uvoptx
		arm_nnexamples_cifar10.uvprojx
	```

	- We also have a series of nested folders in pca10056 (should correspond to the pca number of your device). If it doesn't you may need to change the makefile, linker script and sdk_config.h (check the troubleshooting section for details on how to find the right files) In this example, we kept the directory structure used by Nordic examples. 
	- In the pca10056/blank/armgcc folder:
		- the linker script (cifar_gcc_nrf52.ld) is a copy of the linker script provided for the nrf52 blinky example (common for the same board, it initializes memory locations for your board).
		- the Makefile: a few adjustments will be needed here for it to work on your computer. It is important to know that this MakeFile references the Nordic SDK common makefile. 
		Adjust that the directory variables correspond to your configuration. SDK_ROOT should point to the root directory of your Nordic SDK, in our case, we used a copy of the SDK located in the parent directory of our CMSIS_NN_nrf52840 directory (root directory of this repo). The PROJ_DIR should point to the location of your C/C++ code (where our arm_nnexamples_cifar10.cpp, etc are located). The CMSIS_DIR sould point to the location of your CMSIS library folder.
		```
	 	SDK_ROOT := ../../../../nRF5_SDK_12.3.0_d7731ad_copy
		PROJ_DIR := ../../..
		CMSIS_DIR := ../../../../CMSIS_LIB/CMSIS/5.4.0
		```

		The targets variable will be the name of the .hex file to be flashed on the board. The output directory defines a location of a directory to put all the object files and compilation/linking/binary outputs. Make sure the linkerscript name is correct. 
		In the SRC_FILES: add all the files .C/.CPP corresponding to the used functions. In this case, line 31 includes our main C file, lines 21-39 included the functions that are called in the main C file from the CMSIS library. Because this Makefile was intially copied from the blinky example there could be some excess includes/src files. In the INCLUDE_FOLDERS make sure the CMSIS Includes are there:
		```
	 	  $(CMSIS_DIR)/CMSIS/Core/Include \
 		  $(CMSIS_DIR)/CMSIS/DSP/Include \
  		  $(CMSIS_DIR)/CMSIS/NN/Include \
  		  $(CMSIS_DIR)/CMSIS/Include \
		```
		Notice that the config folder is in the INC_FOLDERS, if you decide to change the directory structure or the location of the SDK_config.h, you will need to update that line.

		Most Compiler/linker flags are the same as the default blinky. The RTT flags are in order to use RTT for debugging, and inlcuding the RTT related files in the compilation/linking. We added two defines in order to include the correct arm_math library: (our board has an FPU, and it is a CM4-Cortex-M4)
		```
	 	  CFLAGS += -D__FPU_PRESENT="1"
		  CFLAGS += -DARM_MATH_CM4
		```
		- SDK_config.h: this file defines a number of variables depending on your compilation flags to enable/disable certain interfaces (UART, RTT, SPI, etc) and define configuration variables for them. An untouched SDK_config.h file can be found in the SDK examples. Because we wanted to use RTT for debugging, we disabled UART and enabled RTT.
		
	### Running the code:

	The final part of the makefile defines very useful targets for easy deployment in the armgcc directory: 
		- in order to compile/link run (mingw32-make.exe for msys on windows):
		```
	 	  make 
		```
		- In order to flash you can flash manually by taking the .hex output in the _build directory and copying it on the board through a file explorer or you can run:
		```
	 	  make flash
		```
		Finally to erase your board run:
		```
	 	  make erase
		```
	Debugging: Our program uses RTT debugging, thus you can see the output of the program by using an RTT viewer (we used the one with J-Link). Launch J-Link RTT viewer, it will ask for a type of connection (we used USB). It will then ask for a target device, choosing the ... browsing button will load a long list of boards, choose the NRF52840_xxaa. We used SWD as an interface with speed 4000kHz and Auto detection. Then click OK. If the RTT viewer recognizes your device green logs will appear with RTT Viewer connected. Flash your board with the program you should see "start execution" and the list of classes and prediction in the Terminal 0 tab. 

## Troubleshooting:
	We have encountered some issues with RTT sometimes. (Generates a linker error). According to  documentation you need to include SEGGER_RTT.h in your .c file and using SEGGER_RTT_printf or SEGGER_RTT_WriteString. This has not worked with us, but it did work for the blinky example. Thus in this example, the SDK_config.h enables us to log printf to RTT implicitly.

	Useful tutorials/posts:
		- Starting with RTT : https://devzone.nordicsemi.com/tutorials/b/getting-started/posts/debugging-with-real-time-terminal
		- Detailed tutorial about CIFAR10 and the example code: https://community.arm.com/processors/b/blog/posts/deploying-convolutional-neural-network-on-cortex-m-with-cmsis-nn