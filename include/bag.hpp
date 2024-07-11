/*
 * Your implementation of the container bag<Val>.
 * Write both the declaration and definition of the
 * container in this file.
 */

// Bag is implemented as a single linked list
template <typename T>
struct bag{
    private:
        struct Node {
            T val;
            Node* next;
        };
        
        // Methods
        void pop_front();
        void push_front(T const&);
        void push_back(T const&);
        bool contains_equal_label(T const&);

    public:
        bag();
        ~bag();
        bool empty() const;
        bag<T>& operator=(bag<T> const&);
        bag<T>& operator=(bag<T>&&);
        bool add_ordered(T const&);
        void print_children() const;
        T* get_next(const T&);
        bool operator==(const bag<T>& rhs) const;
        bool operator!=(const bag<T>& rhs) const;
        

        // Iterators
        struct iterator {
            using iterator_category = std::forward_iterator_tag;
            using value_type = T;
            using reference = T&;
            using pointer = T*;

            iterator(Node* ptr);

            iterator& operator++();
            reference operator*();
            pointer operator->();

            bool operator==(iterator const& rhs);
            bool operator!=(iterator const& rhs);

        private:
            Node* m_ptr;
        };

        struct const_iterator {
            using iterator_category = std::forward_iterator_tag;
            using value_type = T const;
            using reference = T const&;
            using pointer = T const*;

            const_iterator(Node const* ptr);

            const_iterator& operator++();
            reference operator*();
            pointer operator->();

            bool operator==(const_iterator const& rhs);
            bool operator!=(const_iterator const& rhs);

        private:
            Node const* m_ptr;
        };

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    private:
        // Attributes
        Node* m_front;
        Node* m_back;
};

/** Default constructor */
template <typename T>
bag<T>::bag(){
    this->m_front = this->m_back = nullptr;
}

/** Destructor */
template <typename T>
bag<T>::~bag(){
    while (!empty()) pop_front();
}

/** Copy assignment operator */
template <typename T>
bag<T>& bag<T>::operator=(bag<T> const& rhs){
    // Handle self-assigments
    if (this == &rhs){
        return *this;
    }else{
        // Clear the list
        while (!empty()) pop_front();
        // Copy from rhs
        Node* ptr = rhs.m_front;
        while (ptr){
            push_back(ptr->val);
            ptr = ptr->next;
        }
    }
    return *this;
}

/** Move assignment operator */
template <typename T>
bag<T>& bag<T>::operator=(bag<T>&& rhs){
    if (this == &rhs){
        return *this;
    }else{
        // Clear the list
        while (!empty()) pop_front();
        m_front = rhs.m_front;
        m_back = rhs.m_back;
        rhs.m_front = nullptr;
        rhs.m_back = nullptr;
    }
    return *this;
}


/** Return if the bag is empty */
template <typename T>
bool bag<T>::empty() const {
    return m_front == nullptr;
}

/** Add an element in front */
template <typename T>
void bag<T>::push_front(T const& val) {
    if (empty()) {
        m_front = new Node{val, nullptr};
        m_back = m_front;
        return;
    }
    m_front = new Node{val, m_front};
}

/** Add an element at back */
template <typename T>
void bag<T>::push_back(T const& val) {
    if (empty()) {
        push_front(val);
        return;
    }
    m_back->next = new Node{val, nullptr};
    m_back = m_back->next;
}


/** Removes the first element from the bag */
template <typename T>
void bag<T>::pop_front() {
    if (!empty()){
        Node* tmp = m_front;
        m_front = m_front->next;
        delete tmp;
        if (empty()) m_back = nullptr;
    }
}

/** Check if bag contains a child equal val_labels */
template <typename T>
bool bag<T>::contains_equal_label(T const& val){
    if (!empty){
        Node* ptr = this->m_front;
        bool equal_label = false;
        while (ptr && !equal_label){
            // This works only on types that has get_label() as method
            equal_label = ptr->val.get_label() == val.get_label();
            ptr = ptr->next;
        }
        return equal_label;
    }else{
        return false;
    }
}

/** 
 * Add in order of label a child
 * @param c child to add
 * @return - If child was added or not
 */
template <typename T>
bool bag<T>::add_ordered(T const& val){
    if (!empty()){
        // if (ptr->val.get_label() == val.get_label()){ // Same label(->can't add a child with same label)
        //     return false;
        // }else if(ptr->val.get_label() < val.get_label()){ // Replace the first element
        //     Node* new_head = new Node{c, m_front};
        //     m_front = new_head;
        //     return true;
        // }else{ // Check next elements
        //     while (ptr->next && !equal_label && !added){
        //         if (ptr->next->val.get_label() == val.get_label()){ // Same label(->can't add a child with same label)
        //             equal_label = true;
        //         }else if(ptr->next->val.get_label() > val.get_label()){ // If next is >, has to add before it this element
        //             added = true;
        //             Node* new_next = new Node{c, ptr->next};
        //             ptr->next = new_next;
        //         }else{
        //             ptr = ptr->next;
        //         }
        //     }
        //     // Return successful when there is no equal label and the element is added
        //     if (!equal_label && !added){ // Has to added at the end

        //     }
            
            
        //     return !equal_label || added;
        // }
        Node* ptr = this->m_front;
        bool equal_label = false;
        bool added = false;
        // This works only on types that has get_label() as method
        // First of all, check if it has to added at the front or at the end
        if (*(m_front->val.get_label()) == *(val.get_label()) || *(m_back->val.get_label()) == *(val.get_label())){ // Same label(->can't add a child with same label)
            return false;
        }else if (*(val.get_label()) < *(m_front->val.get_label())){ // Add in front
            push_front(val);
            return true;
        }else if (*(val.get_label()) > *(m_back->val.get_label())){ // Add at the end
            push_back(val);
            return true;
        }else{
            while (ptr->next && !equal_label && !added){
                if (*(ptr->next->val.get_label()) == *(val.get_label())){ // Same label(->can't add a child with same label)
                    equal_label = true;
                }else if(*(ptr->next->val.get_label()) > *(val.get_label())){ // If next is >, has to add before it this element
                    added = true;
                    Node* new_next = new Node{val, ptr->next};
                    ptr->next = new_next;
                }else{
                    ptr = ptr->next;
                }
            }
            // Return successful when there is no equal label and the element is added
            return !equal_label || added;
        }
    }else{
        push_front(val);
        return true;
    }
}

template <typename T>
void bag<T>::print_children() const{
     Node* ptr = m_front;
     while (ptr){
         print_trie(ptr->val);
         if (ptr->next) std::cout << ",\n    ";
         ptr = ptr->next;
     }
    // for(auto it = begin(); it != end(); ++it){
    //     print_trie(*it);
    //     if(it)
    // }
}

// Iterator

/** Initialise an iterator from a defined element */
template <typename T>
bag<T>::iterator::iterator(Node* ptr) : m_ptr(ptr) {}

template <typename T>
typename bag<T>::iterator::reference bag<T>::iterator::operator*() {
    return m_ptr->val;
}

template <typename T>
typename bag<T>::iterator::pointer bag<T>::iterator::operator->() {
    return &(m_ptr->val);
}

template <typename T>
typename bag<T>::iterator& bag<T>::iterator::operator++() {
    m_ptr = m_ptr->next;
    return *this;
}

template <typename T>
bool bag<T>::iterator::operator==(const bag<T>::iterator& rhs) {
    return m_ptr == rhs.m_ptr;
}

template <typename T>
bool bag<T>::iterator::operator!=(const bag<T>::iterator& rhs) {
    return m_ptr != rhs.m_ptr;
}

template <typename T>
typename bag<T>::iterator bag<T>::begin() {
    return {m_front};
}

template <typename T>
typename bag<T>::iterator bag<T>::end() {
    return {nullptr};
}

// Const iterator
/** Initialise an iterator from a defined element */
template <typename T>
bag<T>::const_iterator::const_iterator(const Node* ptr) : m_ptr(ptr) {}

template <typename T>
typename bag<T>::const_iterator::reference bag<T>::const_iterator::operator*() {
    return m_ptr->val;
}

template <typename T>
typename bag<T>::const_iterator::pointer bag<T>::const_iterator::operator->() {
    return &(m_ptr->val);
}

template <typename T>
typename bag<T>::const_iterator& bag<T>::const_iterator::operator++() {
    m_ptr = m_ptr->next;
    return *this;
}

template <typename T>
bool bag<T>::const_iterator::operator==(const bag<T>::const_iterator& rhs) {
    return m_ptr == rhs.m_ptr;
}

template <typename T>
bool bag<T>::const_iterator::operator!=(const bag<T>::const_iterator& rhs) {
    return m_ptr != rhs.m_ptr;
}

template <typename T>
typename bag<T>::const_iterator bag<T>::begin() const{
    return {m_front};
}

template <typename T>
typename bag<T>::const_iterator bag<T>::end() const{
    return {nullptr};
}

/**
 * Returns the next children if have
 * @param from the actual trie whose next has to be returned
 * @return The next element of the passed trie 
*/
template <typename T>
T* bag<T>::get_next(const T& from){
    auto it = begin();
    std::cout << "AO";
    while (it != end() && ((*it).get_label() != from.get_label() || (*it) != from)){
        ++it;
    }
    if (it == end()){
        return nullptr;
    }else{
        return &(*(++it));
    }
}

// Comparison
template <typename T>
bool bag<T>::operator==(const bag<T>& rhs) const {
    bool equal = true;
    auto it1 = this->begin();
    auto it2 = rhs.begin();
    // Two bags are equal when:
    // - Are both empty
    // - Children have same attributes
    if(this->empty() && rhs.empty()){ // Both are empty
        return true;
    }else{
        // Check until one bag reaches the end before the other(has more elements || some element is not equal)
        do{
            if((it1 == this->end() && it2 != rhs.end()) || (it1 != this->end() && it2 == rhs.end())){
                equal = false;
            }else{
                // Check also the label bc operator== on trie doesn't control if equal
                equal = *(it1->get_label()) == *(it2->get_label()) && *it1 == *it2;
                ++it1;
                ++it2;
            }
        }while(equal && it1 != this->end() && it2 != rhs.end());
    }
    return equal;
}

template <typename T>
bool bag<T>::operator!=(const bag<T>& rhs) const {
   return !(*this == rhs);
}