/**
* @file MutablePriorityQueue.h
 * @brief A priority queue that allows updating the value of its elements.
 */

#ifndef DA_TP_CLASSES_MUTABLEPRIORITYQUEUE
#define DA_TP_CLASSES_MUTABLEPRIORITYQUEUE

#include <vector>


/**
 * @brief A generic mutable priority queue.
 * @tparam T Type of elements stored in the queue.
 */
template <class T>
class MutablePriorityQueue {
    std::vector<T *> H;
    void heapifyUp(unsigned i);
    void heapifyDown(unsigned i);
    inline void set(unsigned i, T * x);
public:
    MutablePriorityQueue();
    /**
     * @brief Inserts a new element into the queue.
     * @param x Pointer to the element to insert.
     */
    void insert(T * x);
    /**
     * @brief Extracts the element with the minimum value.
     * @return Pointer to the extracted element.
     */
    T * extractMin();
    /**
     * @brief Updates the position of an element in the queue after its key decreased.
     * @param x Pointer to the element to update.
     */
    void decreaseKey(T * x);
    /**
     * @brief Checks if the queue is empty.
     * @return True if empty, False otherwise.
     */
    bool empty();
};

// Index calculations
#define parent(i) ((i) / 2)
#define leftChild(i) ((i) * 2)

template <class T>
MutablePriorityQueue<T>::MutablePriorityQueue() {
    H.push_back(nullptr);
    // indices will be used starting in 1
    // to facilitate parent/child calculations
}

template <class T>
bool MutablePriorityQueue<T>::empty() {
    return H.size() == 1;
}

template <class T>
T* MutablePriorityQueue<T>::extractMin() {
    auto x = H[1];
    H[1] = H.back();
    H.pop_back();
    if(H.size() > 1) heapifyDown(1);
    x->queueIndex = 0;
    return x;
}

template <class T>
void MutablePriorityQueue<T>::insert(T *x) {
    H.push_back(x);
    heapifyUp(H.size()-1);
}

template <class T>
void MutablePriorityQueue<T>::decreaseKey(T *x) {
    heapifyUp(x->queueIndex);
}

template <class T>
void MutablePriorityQueue<T>::heapifyUp(unsigned i) {
    auto x = H[i];
    while (i > 1 && *x < *H[parent(i)]) {
        set(i, H[parent(i)]);
        i = parent(i);
    }
    set(i, x);
}

template <class T>
void MutablePriorityQueue<T>::heapifyDown(unsigned i) {
    auto x = H[i];
    while (true) {
        unsigned k = leftChild(i);
        if (k >= H.size())
            break;
        if (k+1 < H.size() && *H[k+1] < *H[k])
            ++k; // right child of i
        if ( ! (*H[k] < *x) )
            break;
        set(i, H[k]);
        i = k;
    }
    set(i, x);
}

template <class T>
void MutablePriorityQueue<T>::set(unsigned i, T * x) {
    H[i] = x;
    x->queueIndex = i;
}

#endif