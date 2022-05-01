# ProyectoImplementacion
## Basic Example

```cpp
#include <iostream>
#include <rumal/rumal.hpp>

int main(int argc, char **argv){
    using namespace rumal::html::attrs;
    using namespace rumal::html::tags;
    using namespace rumal::css;
    using namespace rumal::css::props;
   
    std::cout << div(_id(42) / _class("test"),
                    span(_id(43) / _class("test"), "Hello"),
                    span("World")
                ) << std::endl;
    
    std::cout << select(".heading", 
                    position("relative") / 
                    display("block"), 
                ) << std::endl;
    return 0;
}

```
## Nested CSS

```cpp
using namespace rumal::css;
using namespace rumal::css::props;

select(".main", 
      display("block") 
    / position("relative"), 
    select(".heading", 
          display("block") 
        / position("relative")
    )
) / select(".container", 
      display("block") 
    / position("relative")
);

```

The above example produces the following CSS.

```css
.container{
    position: relative; 
    display: block;
}

.main{
    position: relative;
    display: block;
}

.main > .heading{
    position: relative;
    display: block;
}
```

## HTML attributes

## HTML Tags