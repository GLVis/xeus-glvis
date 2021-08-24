# Interactive GLVis Jupyter Widget for xeus kernels

## Usage with xeus-cling

### Install Dependencies
1) Install [xeus-cling](https://github.com/jupyter-xeus/xeus-cling)
2) Install [pyglvis](https://github.com/glvis/pyglvis)
    - this is for the widget frontend, we will update the install to improve this step in the future

### Example with saved file

```
#include "path/to/your/xeus-glvis/include/xglvis.hpp";

const std::string stream = glvis::read_file("path/to/stream.saved");

auto glv = glvis::glvis();
glv.plot(stream);
glv
```

### Example with `GridFunction` and `Mesh`

```
#include "path/to/your/xeus-glvis/include/xglvis.hpp";

std::stringstream ss;
ss << "solution\n" << mesh << x << flush;

auto glv = glvis::glvis();
glv.plot(ss.str());
glv

// update a time-dependent solution:
glv.update(stream_string);

// change the plot size:
glv.size_size(width, height);
```


## TODO

- Constructor that takes an `std::string`
- Routines that take a `Mesh` and `GridFunction`
- binder example
