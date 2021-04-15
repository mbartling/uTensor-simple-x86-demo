#include <cstring>
#include <cstdio>

#include "MyModel.h"

using namespace uTensor;

/** ============================= 
 * Define a custom event handler 
 *  =============================
 */
class MyEventHandler : public ErrorHandler {
 public:
  // uThrow: normally invoked when a error occurs during op evaluation
  // you can override this method and do what ever you want with the error.
  // In this tutorial, we're going to implement a spin-wait handler so we
  // can debug the error easily in a debugger such as lldb or gdb.
//  virtual void uThrow(Error* err) override {
//      printf("Error thrown. Spinning...\n");
//      while (true) {
//      }
//  }

  // notify: normally invoked when a framework event occurs. For example, in
  // the arenaAllocator, events such as the allocator is created will notified
  // to the handler. You can do check/logging with it.
  // Each event is identified by a static uid.
  virtual void notify(const Event& evt) override {
    if (evt.event_id == MyModelEvalStartEvent::uid) {
      printf("MyModelEvalStartEvent detected\n");
    } else if (evt.event_id == MyModelEvalEndEvent::uid) {
      printf("MyModelEvalEndEvent detected\n");
    } else {
      printf("Unknown Event detected %d\n", evt.event_id);
    }
  }
};

MyEventHandler handler;
MyModel myModel;



/** ============================ 
 * Begin Main
 * =============================
 */
int main() {
  // Setup our custom Error/Event handler
  Context::get_default_context()->set_ErrorHandler(&handler);
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
  printf("B: ");
  print(myModel.b);
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
