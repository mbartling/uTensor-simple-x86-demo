#include <cstring>
#include <cstdio>

#include "MyModel.h"

using namespace uTensor;

MyModel myModel;

int main() {
  // Userspace owns the buffer in a buffer tensor
  uint8_t* a_buffer = new uint8_t[2*2];
  uint8_t* out_buffer = new uint8_t[2*2];
  Tensor a = new /*const*/ BufferTensor({2, 2}, u8, a_buffer);
  Tensor out = new /*const*/ BufferTensor({2, 2}, u8, out_buffer);

  //Go ahead and copy example input to a_buffer
  // Simulate data write
  for(int i = 0; i < 4; i++){
    a_buffer[i] = s_a[i];
  }
  
  printf("Evaluating model using provided buffer\n");
  printf("A: ");
  print(a);
  myModel
      .set_inputs({{MyModel::input, a}})
      .set_outputs({{MyModel::output, out}})
      .eval();

  // Compare results using buffers
  for(int i = 0; i < 4; i++){
    if(out_buffer[i] != s_out_ref[i]){
      printf("Oh crap, values are different!\n");
      return -1;
    }
  }
  
  // Compare results using the Tensor
  Tensor out_ref = new RomTensor({2,2}, u8, s_out_ref);
  TensorShape& out_shape = out->get_shape();
  for (int i = 0; i < out_shape[0]; i++) {
    for (int j = 0; j < out_shape[1]; j++) {
      // Just need to cast the output
      if((uint8_t)out(i, j) != (uint8_t)out_ref(i,j)){
        printf("Oh crap, values are different in Tensor Access!\n");
        return -1;
      }
    }
  }

  printf("out = a*b + c + d \n====>\n");
  print(out);

  delete[] a_buffer;
  delete[] out_buffer;

  printf("Everything went as planned!\n");
  return 0;
}
