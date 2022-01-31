/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2003-2021. All rights reserved.
 ******************************************************************************************/

#pragma once

#if defined(DSA_STACK_VECTOR)
#include "stack_vector/stack_vector.h"  //由向量派生的栈
#elif defined(DSA_STACK_LIST)
#include "stack_list/stack_list.h"  //由列表派生的栈
#else                               // Add this section to avoid circlic dependency
#include <stack>

template <typename T>
class Stack : public std::stack<T> {
public:
    void push(T const &e) { this->std::stack<T>::push(e); }
    T pop() {
        T const &e = this->std::stack<T>::top();
        this->std::stack<T>::pop();
        return e;
    }
    T &top() { return this->std::stack<T>::top(); }
};
#endif
