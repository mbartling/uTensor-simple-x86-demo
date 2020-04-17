#include "MyModel.h"

MyModel::MyModel()
{
  Context::get_default_context()->set_metadata_allocator(&meta_allocator);
  Context::get_default_context()->set_ram_data_allocator(&ram_allocator);
  
  b = new RomTensor({2, 2}, u8, s_b);
  c = new RomTensor({2,2}, u8, s_c);
  d = new RomTensor({2,2}, u8, s_d);

}

// The two rom Tensors will be freed after eval is called
void MyModel::compute() {
  // First update the default context to this model in case multiple models are being run
  Context::get_default_context()->set_metadata_allocator(&meta_allocator);
  Context::get_default_context()->set_ram_data_allocator(&ram_allocator);
  
  // Ram Tensors are temporary values
  // Can probably come up with some form of a naming scheme around this
  Tensor mult_1_out = new RamTensor({2,2}, u8);
  Tensor add_1_out = new RamTensor({2,2}, u8);

  // First Multiply
  // mult_1_out = input*b;
  mult
      .set_inputs({{MatrixMultOperator<uint8_t>::a, inputs[input].tensor()}, {MatrixMultOperator<uint8_t>::b, b}})
      .set_outputs({{MatrixMultOperator<uint8_t>::c, mult_1_out}})
      .eval();
  
  // First Add
  // output = mult_1_out + c;
  add
      .set_inputs({{AddOperator<uint8_t>::a, mult_1_out}, {AddOperator<uint8_t>::b, c}})
      .set_outputs({{AddOperator<uint8_t>::c, add_1_out}})
      .eval();
  
  mult_1_out.free();


  // Second Add
  // output = add_1_out + d
  add
      .set_inputs({{AddOperator<uint8_t>::a, add_1_out}, {AddOperator<uint8_t>::b, d}})
      .set_outputs({{AddOperator<uint8_t>::c, outputs[output].tensor()}})
      .eval();
  add_1_out.free();
}
