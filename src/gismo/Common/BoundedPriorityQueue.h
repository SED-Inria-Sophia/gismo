/** @file BoundedPriorityQueue.h

    @brief Provides declaration of bounded priority queue

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): Keith Schwarz (htiek@cs.stanford.edu),
               minor adjustments to G+Smo by M. Möller

    The original source code can be found at
    @link http://www.keithschwarz.com/interesting/code/?dir=bounded-pqueue
*/

#pragma once

#include <map>
#include <algorithm>
#include <limits>

namespace gismo
{

/**
   \brief An implementation of the bounded priority queue abstraction.

   A bounded priority queue is in many ways like a regular priority
   queue.  It stores a collection of elements tagged with a real-
   valued priority, and allows for access to the element whose
   priority is the smallest.  However, unlike a regular priority
   queue, the number of elements in a bounded priority queue has
   a hard limit that is specified in the constructor.  Whenever an
   element is added to the bounded priority queue such that the
   size exceeds the maximum, the element with the highest priority
   value will be ejected from the bounded priority queue.  In this
   sense, a bounded priority queue is like a high score table for
   a video game that stores a fixed number of elements and deletes
   the least-important entry whenever a new value is inserted.

   When creating a bounded priority queue, you must specify the
   maximum number of elements to store in the queue as an argument
   to the constructor.

   For example:

   gsBoundedPriorityQueue<int> bpq(15); // Holds up to fifteen values.
*/
template <typename T, typename Comparator = std::less<T> >
class gsBoundedPriorityQueue
{
public:
    /**
       Constructs a new, empty bounded priority queue that can hold
       at most the specified number of elements.
    */
    explicit gsBoundedPriorityQueue(size_t capacity) : m_capacity(capacity) {}

    /**
       Inserts a new element into the bounded priority queue.  If the
       queue is at capacity and the new element has higher priority
       than the lowest-priority element already in the queue, the
       lowest-priority element will be ejected.
    */
    void push(const T& value)
    {
        // If we're at capacity and this is worse than the worst, ignore it.
        if (m_capacity <= m_data.size() && !m_data.empty() &&
            Comparator()(value, m_data.rbegin()->first))
            return;

        // Insert the element.
        m_data[value]++;

        // If we're over capacity, remove the worst element.
        if (m_capacity < m_data.size())
        {
            auto last = m_data.end();
            --last;
            if (last->second > 1)
                last->second--;
            else
                m_data.erase(last);
        }
    }

    /**
       Returns whether the bounded priority queue is empty.
    */
    bool empty() const { return m_data.empty(); }

    /**
       Returns the number of elements in the bounded priority queue.
    */
    size_t size() const { return m_data.size(); }

    /**
       Returns the maximum number of elements that can be stored in
       the bounded priority queue.
    */
    size_t capacity() const { return m_capacity; }

    /**
       Returns a const reference to the best element in the bounded
       priority queue.  This element is the one with the lowest priority
       value.
    */
    const T& top() const { return m_data.begin()->first; }

    /**
       Removes the best element from the bounded priority queue.
    */
    void pop()
    {
        if (m_data.begin()->second > 1)
            m_data.begin()->second--;
        else
            m_data.erase(m_data.begin());
    }

private:
    std::map<T, size_t, Comparator> m_data;
    size_t m_capacity;
};

} // namespace gismo