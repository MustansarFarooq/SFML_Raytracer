#include "aabb.h"
struct flatBVHNode {
  aabb bbox;

  int leftChild;
  int rightChild;

  int start;
  int count;  // number of primitives
};
