#include "DTQueue.h"

using namespace std;

void DTQueue::enqueue(const Pair& item) {
    data.addElementAtEnd(item);
}

