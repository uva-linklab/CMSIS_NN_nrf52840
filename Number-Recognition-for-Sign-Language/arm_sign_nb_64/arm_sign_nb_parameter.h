#define CONV1_IM_DIM 64
#define CONV1_IM_CH 1
#define CONV1_KER_DIM 5
#define CONV1_PADDING 2
#define CONV1_STRIDE 1
#define CONV1_OUT_CH 32
#define CONV1_OUT_DIM 64

#define POOL1_KER_DIM 2
#define POOL1_STRIDE 2
#define POOL1_PADDING 1
#define POOL1_OUT_DIM 32

#define CONV2_IM_DIM 32
#define CONV2_IM_CH 32
#define CONV2_KER_DIM 5
#define CONV2_PADDING 2
#define CONV2_STRIDE 1
#define CONV2_OUT_CH 32
#define CONV2_OUT_DIM 32

#define POOL2_KER_DIM 2
#define POOL2_STRIDE 2
#define POOL2_PADDING 1
#define POOL2_OUT_DIM 16

#define CONV3_IM_DIM 16
#define CONV3_IM_CH 32
#define CONV3_KER_DIM 5
#define CONV3_PADDING 2
#define CONV3_STRIDE 1
#define CONV3_OUT_CH 16
#define CONV3_OUT_DIM 16

#define POOL3_KER_DIM 2
#define POOL3_STRIDE 2
#define POOL3_PADDING 1
#define POOL3_OUT_DIM 8

#define IP1_DIM 8*8*16
#define IP1_IM_DIM 8
#define IP1_IM_CH 16
#define IP1_OUT 300

#define IP2_DIM 300
#define IP2_IM_DIM 300
#define IP2_IM_CH 1
#define IP2_OUT 10