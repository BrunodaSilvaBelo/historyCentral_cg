#ifndef CLEAR_H
#define CLEAR_H

#include <GL/glew.h>

namespace components {
  struct NoClear {
    static const GLbitfield value = 0x0;
  };

  template<GLbitfield mask, class Parent = NoClear>
  struct Clear {
    static const GLbitfield value = mask | Parent::value;
  };
}

#endif /* CLEAR_H */
