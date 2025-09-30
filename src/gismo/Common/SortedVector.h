/** @file SortedVector.h

    @brief An std::vector with sorting capabilities

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris, based on
    https://www.thedigitalmachine.com/wiki/C%2B%2B_sorted_vector

    C++23 introduces std::flat_set
*/

#pragma once

#include <vector>
#include <algorithm>

namespace gismo {

/** \brief This class is derived from std::vector, and adds sort tracking.

    There are two basic ways to use a sorted vector:

    METHOD 1
    Always maintain sort order by inserting with push_sorted() -
    the location of new items is determined before inserting;
    since the vector remains sorted, this doesn't take too long
    (although for large batch insertions METHOD 2 is definitely
    faster);

    METHOD 2
    Insert all items with push_back(), then call sort() once at the end.
*/
template <typename T, typename Compare = std::less<T> >
class gsSortedVector : public std::vector<T>
{
public:
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;

    gsSortedVector() : std::vector<T>(), m_sorted(true) {}

    explicit gsSortedVector(size_t capacity) : std::vector<T>(capacity), m_sorted(true) {}

    // Insert item, maintaining sorted order
    iterator push_sorted(const T& item)
    {
        iterator it = std::lower_bound(this->begin(), this->end(), item, Compare());
        it = this->insert(it, item);
        return it;
    }

    // Mark as unsorted
    void push_back(const T& item)
    {
        std::vector<T>::push_back(item);
        m_sorted = false;
    }

    // Sort the vector
    void sort()
    {
        std::sort(this->begin(), this->end(), Compare());
        m_sorted = true;
    }

    // Check if sorted
    bool is_sorted() const { return m_sorted; }

private:
    bool m_sorted;
};

} // namespace gismo