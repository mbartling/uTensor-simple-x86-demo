# Welcome to the uTensor-simple-x86-demo!

The purpose of this demo is to showcase some of the features in the upcoming changes to the uTensor core runtime, as well as source of discussion around TinyML embedded development strategies. If you are looking for a complete end to end reference workflow with model translation, code generation, and runtime optimizations checkout TODO.

uTensor itself supports both Arm and x86 architectures and for the most part is OS/RTOS agnostic. In other words, if your high performance models run correctly on your laptop then they will run on pretty much any tiny Cortex-M board! Obviously, this assumes you have the resources available to represent/evaluate your model.

## Core Ideas and Extensions
We, the uTensor team, spent the past year integrating the old version of uTensor into various research projects and have poured our learnings into the new version by doing a ground up rearchitecture. This new version augments the uTensor core ideas, leading to a much smaller and more IoT-able solution.

Old key points:
- Tensors describe how data is accessed and where from
  - Performance of ops depends on which tensors are used
- Operators are Tensor agnostic
  - High performance ops can fetch blocks of data at once
- Strive for low total power in execution
- Low static and dynamic footprint, be small
  - Low cost per Tensor throughout the entire system, since most generated models have 100+ including intermediates, also impacts dynamic footprint
  - Lightweight class hierarchy
  - Duh

New additional key ideas:
- System safety
  - All tensor metadata and actual data are owned in dedicated regions
    - This can either be user provided, or one we create
  - We can guarantee that runtime will use no more than N bytes of RAM at code gen time or at compile time!
  - Generally should not collide with userspace or system space memory, i.e. dont share heaps
  - Generally implications: a safe runtime means we can safely update models remotely
  - As many compile time errors as possible!
    - Mismatched inputs, outputs, or numbers
    - wrong sizes used
    - Impossible memory accesses
    - etc.
- Clear, Concise, and Debuggable
  - Previous iteration of uTensor relied almost too heavily on codegen, making changes to a model for any reason was near impossible
  - A developer should be able to make changes to the model without relying on code gen
  - A developer should be able to look at a model file and immediately understand what the graph looks like, without a massive amound of jumping around
  - Default tensor interface should behave like a higher level language, but exploit the speed of C++
    - Generally: No more pointer bullshit! C is super error prone, fight me
      - Only specialized operators have access to raw data blocks, and these ops will be wicked fast
  - Extensible, configurable, and optimize-outable error handling
  - GDB debugging IS NOW TRIVIAL

 Ultimately, these additions help decouple the data sciency parts from the embedded engineery bits, so both parties can iterate quickly and reliably in their dev cycles. 

## Building

```
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_PRINTING=1 ..
make
./demo
```

I intentionally injected a lifecycle management bug into this demo which causes a specific Tensor to be destroyed in the wrong order at program exit. Can you find it? :)
Hint, backtrace and `findEvent.py`. 

```
python ./findEvent.py EVENT_CODE
```
