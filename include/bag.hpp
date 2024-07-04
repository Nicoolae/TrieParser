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

        // Attributes
        Node* m_front;
        Node* m_back;
        
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
        Node* ptr = this->m_front->next;
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
            return !equal_label && added;
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
}