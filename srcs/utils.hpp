#pragma once

namespace ft {

template <bool flag, class is_true, class is_false>
struct choose;

template <class is_true, class is_false>
struct choose<true, is_true, is_false> {
   typedef is_true type;
};

template <class is_true, class is_false>
struct choose<false, is_true, is_false> {
   typedef is_false type;
};

} // namespace ft
