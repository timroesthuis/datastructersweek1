//
// Created by Saxion ACS.
//

#ifndef forward_listS_FORWARD_forward_list_H
#define forward_listS_FORWARD_forward_list_H

#include <type_traits>
#include <iterator>
#include <initializer_list>
#include <memory>

namespace saxion {

    //forward declaration of the class forward_list
    template<typename _T>
    class forward_list;

    namespace detail {
        template<typename _T, typename _Nd>
        class forward_list_iterator;

        template<typename _T, typename _Nd>
        class const_forward_list_iterator;
    }

    // normally all the things that are not intended for public use are grouped in an internal namespace (often "detail")
    namespace detail {

        template<typename _T>
        struct forward_list_node_t {
            template<typename T> friend
            class ::saxion::forward_list;

            _T _value;
            std::unique_ptr<forward_list_node_t> _next;

            forward_list_node_t(const forward_list_node_t&) = delete;

            forward_list_node_t& operator&(forward_list_node_t&) = delete;

            forward_list_node_t(forward_list_node_t&& other) noexcept = default;

            forward_list_node_t& operator=(forward_list_node_t&& other) noexcept = default;

            ~forward_list_node_t(){
                // if a node owns itself, resolve it by releasing this ownership
                // destructor is called before the automatic objects are destroyed, so
                // wwe can safely resolve the circular ownership before _next will be destroyed

                if (_next.get() == this){
                    _next.release();
                }
            }

            forward_list_node_t():
                    _value(),
                    _next(nullptr){
            }

            explicit forward_list_node_t(_T&& v) :
                    forward_list_node_t{std::move(v), nullptr} {}

            explicit forward_list_node_t(_T const& v) :
                    forward_list_node_t{v, nullptr} {}

            void swap(forward_list_node_t& other) noexcept {
                std::swap(_next, other._next);
                std::swap(_value, other._value);
            }

            forward_list_node_t(_T&& v, forward_list_node_t* next) :
                    _value{std::move(v)},
                    _next{next} {}

            forward_list_node_t(_T const& v, forward_list_node_t* next) :
                    _value{v},
                    _next{next} {}

            _T& value() {
                return _value;
            }

            _T const& value() const {
                return _value;
            }

            [[nodiscard]]
            forward_list_node_t* next() const noexcept{
                return _next.get();
            }

        };

        template<typename _T, typename _Nd = forward_list_node_t<_T>>
        class forward_list_iterator {
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::forward_iterator_tag;
            using pointer = _T*;
            using reference = _T&;
            using value_type = _T;

            template<typename> friend
            class ::saxion::forward_list;

            friend
            class ::saxion::detail::const_forward_list_iterator<_T, _Nd>;

            using node_t = _Nd;

            node_t* _current;

            node_t* node() {
                return _current;
            }

            const node_t* node() const {
                return _current;
            }

        public:
            explicit forward_list_iterator(node_t* element) noexcept:
                    _current(element) {}

            explicit forward_list_iterator(const const_forward_list_iterator<_T, _Nd>& iter) noexcept:
                    _current(const_cast<node_t*>(iter._current)) {
            }

            reference operator*() const {
                return _current->value();
            }

            [[nodiscard]]
            pointer operator->() const {
                return std::addressof(_current->value);
            }

            forward_list_iterator& operator++() {
                _current = _current->_next.get();
                return *this;
            }

            forward_list_iterator operator++(int) {
                forward_list_iterator tmp(*this);
                ++(*this);
                return tmp;
            }

            [[nodiscard]]
            bool operator==(const forward_list_iterator& other) const {
                return _current == other._current;
            }

            [[nodiscard]]
            bool operator!=(const forward_list_iterator& other) const {
                return !(*this == other);
            }

            template <typename T, typename Nd>
            friend
            bool operator==(const forward_list_iterator<T, Nd>& lhs, const const_forward_list_iterator<T, Nd>& rhs);

            template <typename T, typename Nd>
            friend
            bool operator!=(const forward_list_iterator<_T, _Nd>& lhs, const const_forward_list_iterator<_T, _Nd>& rhs);
        };

        template<typename _T, typename _Nd = forward_list_node_t<_T>>
        class const_forward_list_iterator {
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::forward_iterator_tag;
            using pointer = const _T*;
            using reference = const _T&;
            using value_type = _T;

            template<typename> friend
            class ::saxion::forward_list;

            friend
            class ::saxion::detail::forward_list_iterator<_T, _Nd>;

            using node_t = _Nd;

            const node_t* _current;

            const node_t* node() const {
                return _current;
            }

        public:
            explicit const_forward_list_iterator(const node_t* element) noexcept:
                    _current(element) {}

            explicit const_forward_list_iterator(const forward_list_iterator<_T, _Nd>& iter) noexcept:
                    _current(iter._current) {
            }

            [[nodiscard]]
            reference operator*() const {
                return _current->value();
            }

            [[nodiscard]]
            pointer operator->() const {
                return std::addressof(_current->value);
            }

            const_forward_list_iterator& operator++() {
                _current = _current->_next.get();
                return *this;
            }

            const_forward_list_iterator operator++(int) {
                const_forward_list_iterator tmp(*this);
                ++(*this);
                return tmp;
            }

            [[nodiscard]]
            bool operator==(const const_forward_list_iterator& other) {
                return _current == other._current;
            }

            [[nodiscard]]
            bool operator!=(const const_forward_list_iterator& other) {
                return !(*this == other);
            }

            template <typename T, typename Nd>
            friend
            bool operator==(const forward_list_iterator<T, Nd>& lhs, const const_forward_list_iterator<T, Nd>& rhs);

            template <typename T, typename Nd>
            friend
            bool operator!=(const forward_list_iterator<_T, _Nd>& lhs, const const_forward_list_iterator<_T, _Nd>& rhs);

        };

        template <typename _T, typename _Nd>
        [[nodiscard]]
        inline bool operator==(const forward_list_iterator<_T, _Nd>& lhs, const const_forward_list_iterator<_T, _Nd>& rhs) {
            return lhs._current == rhs._current;
        }

        template <typename _T, typename _Nd>
        [[nodiscard]]
        inline bool operator!=(const forward_list_iterator<_T, _Nd>& lhs, const const_forward_list_iterator<_T, _Nd>& rhs) {
            return !(lhs == rhs);
        }
    }



    template<typename _T>
    class forward_list {
    public:
        using value_type = _T;
        using reference = _T&;
        using const_reference = _T const&;
        using pointer = _T*;
        using const_pointer = _T const*;
        using size_type = std::size_t;

    private:

        using node_t = detail::forward_list_node_t<_T>;

        node_t _node;
        node_t* _tail; //not really needed but speeds things up a lot
        size_type _size;

        [[nodiscard]]
        node_t* head() const noexcept{
            return _node.next();
        }

        [[nodiscard]]
        node_t* tail() const noexcept{
            return _tail;
        }

    public:

        using iterator = detail::forward_list_iterator<_T, node_t>;
        using const_iterator = detail::const_forward_list_iterator<_T, node_t>;

        // default ctor
        forward_list() :
                _node{},
                _tail{&_node},
                _size{0} {
            //empty forward_list has a self-referencing node!
            _node._next.reset(&_node);
            // so the _node owns itself through the _next pointer
        }

        template<typename _V>
        forward_list(std::initializer_list<_V> init_list) :
                forward_list() {
            for (auto item : init_list) {
                push_back(std::move(item));
            }
        }

        // copy ctor
        forward_list(const forward_list& other) :
                forward_list() {
            for (node_t* other_next{other.head()};
                 other_next->next() != other.head();
                 other_next = other_next->next()) {

                push_back(other_next->value());
            }
        }

        // copy assignment operator
        forward_list& operator=(const forward_list& other) {
            if (this != &other) {
                clear();
                for (auto current = other.begin(); current != other.end(); ++current) {
                    push_back(*current);
                }
            }
            return *this;
        }

        // move ctor
        forward_list(forward_list&& other) noexcept :
                forward_list() {
            // since out list is already initialized with no content
            // we can just swap it with the other list, this way the content of
            // th other list is taken over, and the other list will contain nothing
            swap(other);
        }

        // move assignment operator
        forward_list& operator=(forward_list&& other) noexcept {
            if (this != &other) {
                clear();
                swap(other);
            }
            return *this;
        }

        template<typename _Iter, typename = std::enable_if_t<
                std::is_same_v<
                        typename std::iterator_traits<_Iter>::value_type,
                        value_type >>>
        forward_list(_Iter begin, _Iter end):
                forward_list() {
            for (; begin != end; ++begin) {
                push_back(*begin);
            }
        }


        [[nodiscard]]
        iterator begin() noexcept {
            return iterator(_node.next());
        }

        [[nodiscard]]
        iterator before_begin() noexcept {
            return iterator(&_node);
        }

        [[nodiscard]]
        iterator end() noexcept {
            return iterator(&_node);
        }

        [[nodiscard]]
        const_iterator begin() const noexcept {
            return const_iterator(_node.next());
        }

        [[nodiscard]]
        const_iterator before_begin() const noexcept {
            return const_iterator(&_node);
        }

        [[nodiscard]]
        const_iterator end() const noexcept {
            return const_iterator(&_node);
        }

        [[nodiscard]]
        const_iterator cbegin() const noexcept {
            return const_iterator(_node.next());
        }

        [[nodiscard]]
        const_iterator cbefore_begin() const noexcept {
            return const_iterator(&_node);
        }

        [[nodiscard]]
        const_iterator cend() const noexcept {
            return const_iterator(&_node);
        }

        void swap(forward_list& other) noexcept {
            std::swap(tail()->_next, other.tail()->_next);
            _node.swap(other._node);
            std::swap(_tail, other._tail);
            std::swap(_size, other._size);
        }

        // accessors
        [[nodiscard]]
        reference front() {
            return *begin();
        }

        [[nodiscard]]
        const_reference front() const {
            return head()->value();
        }

        [[nodiscard]]
        reference back() {
            return tail()->value();
        }

        [[nodiscard]]
        const_reference back() const {
            return tail()->value();
        }

        [[nodiscard]]
        reference operator[](size_type index) {
            node_t* current = head();
            while (index--) { current = current->next(); }
            return current->value();
        }

        [[nodiscard]]
        const_reference operator[](size_type index) const {
            node_t* current = head();
            while (index--) { current = current->next(); }
            return current->value();
        }

        [[nodiscard]]
        reference at(size_type index) {
            if (index < _size) {
                node_t* current = head();
                while (index--) { current = current->next(); }
                return current->value();
            }
            throw std::length_error("index out of bounds");
        }

        [[nodiscard]]
        const_reference at(size_type index) const {
            if (index < _size) {
                node_t* current = head();
                while (index--) { current = current->next(); }
                return current->value();
            }
            throw std::length_error("index out of bounds");
        }

        void pop_front() noexcept {
            if (begin() != end()) {
                _node._next = std::move(_node._next->_next);
                if (--_size == 0){
                    _tail = &_node;
                }
            }
        }


        [[nodiscard]]
        bool empty() const {
            return _size == 0;
        }

        [[nodiscard]]
        size_type size() const {
            return _size;
        }

        void clear() noexcept {
            if (begin() != end()) {
                // unlink the nodes iteratively
                while (head() != &_node) {
                    _node._next = std::move(_node._next->_next);
                    --_size;
                }
                _tail = &_node;
                _size = 0;
            }
        }

        ~forward_list() noexcept {
            clear();
        }

        // modifiers
        iterator push_back(_T&& value) {
            tail()->_next = std::make_unique<node_t>(std::move(value), tail()->_next.release());
            _tail = tail()->_next.get();
            ++_size;
            return iterator(tail());
        }

        iterator push_back(const_reference value) {
            tail()->_next = std::make_unique<node_t>(value, tail()->_next.release());
            _tail = tail()->_next.get();
            ++_size;
            return iterator(tail());
        }

        // emplace tries to construct a value in-place. It uses variadic templates
        // this way, instead of pushing an already constructed object
        // we can pass the argument to emplace_back, that the constructor of the stored element takes.
        // for example for strings, instead of using push_back("aaaaa") we can write:
        // emplace_back(5, 'a'). Emplace then uses the arguments passed to it to call a string constructor:
        // by calling _T(std::forward<Args>(args)...) which in this example becomes :
        // std::string(5, 'a')
        // notice that we are actually constructing _T in this function and passing it to the node_t ctor
        // normally, the node_t ctor would also be overloaded for taking a list of parameters to create objects in-place
        template<typename... Args>
        iterator emplace_back(Args&& ... args) {
            tail()->_next = std::make_unique<node_t>(_T(std::forward<Args>(args)...),  tail()->_next.release());
            _tail = tail()->_next.get();
            ++_size;
            return iterator(tail());
        }

        // instead of writing two overloads that take an l-value and an r-value references
        // we can write a function template that takes a forwarding reference
        // the compiler will make the two overloads from it for us
        template<typename V>
        iterator push_front(V&& value) {
            _node._next = std::make_unique<node_t>(std::forward<V>(value), _node._next.release());
            if (_size++ == 0){
                _tail = _node.next();
            }
            return iterator(head());
        }


        iterator erase_after(iterator pos) {
            if (begin() != end()){
                auto res(pos.node()->next()->next());
                pos.node()->_next = std::move(pos.node()->_next->_next);
                --_size;
                if (res == &_node){
                    _tail = &_node;
                }
                return iterator(res);
            }
            return end();
        }

        // insert element after pos
        // returns iterator to inserted element
        iterator insert_after(iterator pos, const_reference value) {
            // grab previous element?
            pos.node()->_next = std::make_unique<node_t>(value, pos.node()->_next.release());
            if(_size++ == 0){
                _tail = pos.node()->next();
            }
            return iterator(pos.node()->next());
        }

        iterator insert_after(iterator pos, _T&& value) {
            pos.node()->_next = std::make_unique<node_t>(std::move(value), pos.node()->_next.release());
            if(_size++ == 0){
                _tail = pos.node()->next();
            }
            return iterator(pos.node()->next());
        }

        template<typename... Args>
        iterator emplace_after(iterator pos, Args&& ... args) {
            pos.node()->_next = std::make_unique<node_t>(_T(std::forward<Args>(args)...), pos.node()->_next.release());
            if(_size++ == 0){
                _tail = pos.node()->next();
            }
            return iterator(pos.node()->next());
        }

    };

    template<typename _Iter>
    forward_list(_Iter b, _Iter e) -> forward_list<typename std::iterator_traits<_Iter>::value_type>;

    template<typename _V>
    forward_list(std::initializer_list<_V>) -> forward_list<_V>;

    forward_list(std::initializer_list<const char*>) -> forward_list<std::string>;

}

namespace std{
    template<typename _T>
    inline void swap(saxion::forward_list<_T>& x, saxion::forward_list <_T>& y) noexcept {
        x.swap(y);
    }
}

#endif //forward_listS_FORWARD_forward_list_H
