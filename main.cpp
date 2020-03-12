#include "mbed.h"
#include <cstring>
#include <cstdio>

#include "MyModel.h"

using namespace uTensor;

MyModel myModel;
Timer t;

int main() {
  //MyModel* model = new MyModel();
  //MyModel& myModel = *model;
  // Userspace owns the buffer in a buffer tensor
  Tensor in = new RomTensor({ 1,28,28,1 }, flt, s_in_0_stride_1);
  Tensor out = new RamTensor({ 1,28,28,32 }, flt);

  //Go ahead and copy example input to a_buffer
  // Simulate data write
  
  printf("Evaluating model using provided buffer\n");
  t.start();
  myModel
      .set_inputs({{MyModel::input, in}})
      .set_outputs({{MyModel::output, out}})
      .eval();
  t.stop();
  printf("Eval took %d uS\n", t.read_us());

  // Compare results using buffers
  for(int i = 0; i < 1*28*28*32; i++){
    float tmp = out(i);
    if(tmp != s_ref_out_0_stride_1[i]){
      printf("Oh crap, values are different!\n");
      return -1;
    }
  }
  
  printf("Everything went as planned!\n");
  return 0;
}
