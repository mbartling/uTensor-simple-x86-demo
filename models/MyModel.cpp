#include "MyModel.h"

MyModel::MyModel() : meta_allocator(), ram_allocator(), conv_Aw({1,1,1,1}, SAME)
{
  Context::get_default_context()->set_metadata_allocator(&meta_allocator);
  Context::get_default_context()->set_ram_data_allocator(&ram_allocator);
  
 w = new RomTensor({ 5,5,1,32 }, flt, s_w_0_stride_1);

}

// The two rom Tensors will be freed after eval is called
void MyModel::compute() {
  // First update the default context to this model in case multiple models are being run
  Context::get_default_context()->set_metadata_allocator(&meta_allocator);
  Context::get_default_context()->set_ram_data_allocator(&ram_allocator);
  

  conv_Aw
       .set_inputs({ {ConvOperator<float>::in, *inputs[input].tensor}, {ConvOperator<float>::filter, w} })
       .set_outputs({ {ConvOperator<float>::out, *outputs[output].tensor} })
       .eval();

}
