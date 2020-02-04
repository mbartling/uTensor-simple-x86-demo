#ifndef MYMODEL_H
#define MYMODEL_H

#include "uTensor.h"
#include "constants.h"

using namespace uTensor;

const size_t my_model_num_inputs = 1;
const size_t my_model_num_outputs = 1;

// This example Model allocates all RAM tensors on each eval and deletes them as needed
// out = a*b + c + d
class MyModel : public ModelInterface<my_model_num_inputs, my_model_num_outputs> {
  public:
    enum names_in: uint8_t {input};
    enum names_out: uint8_t {output};
    MyModel();

    //Inherits public interface
    //eval();
    //set_inputs(...);
    //set_outputs(...);
  protected:
    virtual void compute();

  private:
    // ROM Tensors
    Tensor b;
    Tensor c;
    Tensor d;

    // Operators
    // Note: only need one instance of each since we can set inputs in the compute call
    MatrixMultOperator<uint8_t> mult;
    AddOperator<uint8_t> add;

    // Memory Allocators
    localCircularArenaAllocator<512> meta_allocator;
    localCircularArenaAllocator<64> ram_allocator;
};


#endif
