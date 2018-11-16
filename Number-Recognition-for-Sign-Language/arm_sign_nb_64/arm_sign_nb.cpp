
/**
 *
 * Description:
 * 
 * Demonstrates a convolutional neural network (CNN) example with the use of convolution,
 * ReLU activation, pooling and fully-connected functions.
 *
 *
 * \par Variables Description:
 * \par
 * \li \c conv1_wt, \c conv2_wt, \c conv3_wt are convolution layer weight matrices
 * \li \c conv1_bias, \c conv2_bias, \c conv3_bias are convolution layer bias arrays
 * \li \c ip1_wt, ip1_bias point to fully-connected layer weights and biases
 * \li \c input_data points to the input image data
 * \li \c output_data points to the classification output
 * \li \c col_buffer is a buffer to store the \c im2col output
 * \li \c scratch_buffer is used to store the activation data (intermediate layer outputs)
 *
 * CMSIS DSP Software Library Functions Used:
 *
 * - arm_convolve_HWC_q7_RGB()
 * - arm_convolve_HWC_q7_fast()
 * - arm_relu_q7()
 * - arm_maxpool_q7_HWC()
 * - arm_avepool_q7_HWC()
 * - arm_fully_connected_q7_opt()
 * - arm_fully_connected_q7()
 * 
 */

#include <stdint.h>
#include <stdio.h>
#include "arm_math.h"
#include "arm_sign_nb_parameter.h"
#include "arm_sign_nb_weights.h"

#include "arm_nnfunctions.h"
#include "arm_sign_nb_inputs.h"

// include the input and weights

static q7_t conv1_wt[(CONV1_PADDING + CONV1_STRIDE) * CONV1_IM_DIM * CONV1_OUT_CH] = CONV1_WT;
static q7_t conv1_bias[CONV1_OUT_CH] = CONV1_BIAS;

static q7_t conv2_wt[(CONV2_PADDING + CONV2_STRIDE) * CONV2_IM_DIM * CONV2_OUT_CH] = CONV2_WT;
static q7_t conv2_bias[CONV2_OUT_CH] = CONV2_BIAS;

static q7_t conv3_wt[(CONV3_PADDING + CONV3_STRIDE) * CONV3_IM_CH * CONV3_OUT_CH] = CONV3_WT;
static q7_t conv3_bias[CONV3_OUT_CH] = CONV3_BIAS;

static q7_t ip1_wt[IP1_DIM * IP1_OUT] = IP1_WT;
static q7_t ip1_bias[IP1_OUT] = IP1_BIAS;

static q7_t ip2_wt[IP2_DIM * IP2_OUT] = IP2_WT;
static q7_t ip2_bias[IP2_OUT] = IP2_BIAS;

/* Here the image_data should be the raw uint8 type RGB image in [RGB, RGB, RGB ... RGB] format */
uint8_t   image_data[CONV1_IM_CH * CONV1_IM_DIM * CONV1_IM_DIM] = IMG_DATA;
q7_t      output_data[IP2_OUT];

//vector buffer: max(im2col buffer,average pool buffer, fully connected buffer)
q7_t      col_buffer[2 * 5 * 5 * 32 * 2];

q7_t      scratch_buffer[64 * 64 * 10 * 4];

int main()
{

  printf("start execution\n");
  /* start the execution */

  q7_t     *img_buffer1 = scratch_buffer; 
  q7_t     *img_buffer2 = img_buffer1 + 64 * 64 * 32; // + size of output volume

  /* input pre-processing */
  int mean_data = INPUT_MEAN_SHIFT;
  unsigned int scale_data = INPUT_RIGHT_SHIFT;
  for (int i=0;i<64 * 64; i+=1) {
    img_buffer2[i] =   (q7_t)__SSAT( ((((int)image_data[i]   - mean_data)<<7) + (0x1<<(scale_data-1)))
                             >> scale_data, 8);
  }
  
  // --------------- block 1 -------------------------------
  // conv1 img_buffer2 -> img_buffer1
  arm_convolve_HWC_q7_fast(img_buffer2, CONV1_IM_DIM, CONV1_IM_CH, conv1_wt, CONV1_OUT_CH, CONV1_KER_DIM, CONV1_PADDING,
                          CONV1_STRIDE, conv1_bias, CONV1_BIAS_LSHIFT, CONV1_OUT_RSHIFT, img_buffer1, CONV1_OUT_DIM,
                          (q15_t *) col_buffer, NULL);

  arm_relu_q7(img_buffer1, CONV1_OUT_DIM * CONV1_OUT_DIM * CONV1_OUT_CH);

  // pool1 img_buffer1 -> img_buffer2
  arm_maxpool_q7_HWC(img_buffer1, CONV1_OUT_DIM, CONV1_OUT_CH, POOL1_KER_DIM,
                     POOL1_PADDING, POOL1_STRIDE, POOL1_OUT_DIM, NULL, img_buffer2);

 // --------------- block 2 -------------------------------
  // conv2 img_buffer2 -> img_buffer1
  arm_convolve_HWC_q7_fast(img_buffer2, CONV2_IM_DIM, CONV2_IM_CH, conv2_wt, CONV2_OUT_CH, CONV2_KER_DIM,
                           CONV2_PADDING, CONV2_STRIDE, conv2_bias, CONV2_BIAS_LSHIFT, CONV2_OUT_RSHIFT, img_buffer1,
                           CONV2_OUT_DIM, (q15_t *) col_buffer, NULL);

  arm_relu_q7(img_buffer1, CONV2_OUT_DIM * CONV2_OUT_DIM * CONV2_OUT_CH);

  // pool2 img_buffer1 -> img_buffer2
  arm_maxpool_q7_HWC(img_buffer1, CONV2_OUT_DIM, CONV2_OUT_CH, POOL2_KER_DIM,
                     POOL2_PADDING, POOL2_STRIDE, POOL2_OUT_DIM, col_buffer, img_buffer2);

 // --------------- block 3 -------------------------------
// conv3 img_buffer2 -> img_buffer1
  arm_convolve_HWC_q7_fast(img_buffer2, CONV3_IM_DIM, CONV3_IM_CH, conv3_wt, CONV3_OUT_CH, CONV3_KER_DIM,
                           CONV3_PADDING, CONV3_STRIDE, conv3_bias, CONV3_BIAS_LSHIFT, CONV3_OUT_RSHIFT, img_buffer1,
                           CONV3_OUT_DIM, (q15_t *) col_buffer, NULL);

  arm_relu_q7(img_buffer1, CONV3_OUT_DIM * CONV3_OUT_DIM * CONV3_OUT_CH);

  // pool3 img_buffer-> img_buffer2
  arm_maxpool_q7_HWC(img_buffer1, CONV3_OUT_DIM, CONV3_OUT_CH, POOL3_KER_DIM,
                     POOL3_PADDING, POOL3_STRIDE, POOL3_OUT_DIM, col_buffer, img_buffer2);

  // -------------------- dense layers ----------------------

  arm_fully_connected_q7_opt(img_buffer2, ip1_wt, IP1_DIM, IP1_OUT, IP1_BIAS_LSHIFT, IP1_OUT_RSHIFT, ip1_bias,
                             output_data, (q15_t *) img_buffer1);

  arm_relu_q7(img_buffer1, IP1_DIM * IP2_DIM);

  arm_fully_connected_q7_opt(img_buffer2, ip2_wt, IP2_DIM, IP2_OUT, IP2_BIAS_LSHIFT, IP2_OUT_RSHIFT, ip2_bias,
                             output_data, (q15_t *) img_buffer1);

  arm_softmax_q7(output_data, IP2_OUT, output_data);

  for (int i = 0; i < 10; i++)
  {
      printf("%d: %d\n", i, output_data[i]);
  }

  return 0;
}
