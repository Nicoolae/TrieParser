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
        bool empty() const;
        void pop_front();
        void bag<T>::push_front(T const&);
        void bag<T>::push_back(T const&);

    public:
        bag();
        ~bag();
        bag<T>& operator=(bag<T> const&);
        bag<T>& operator=(bag<T>&&) 
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
        m_front = new node{val, nullptr};
        m_back = m_front;
        return;
    }
    m_front = new node{val, m_front};
}

/** Add an element at back */
template <typename T>
void bag<T>::push_back(T const& val) {
    if (empty()) {
        push_front(val);
        return;
    }
    m_back->next = new node{val, nullptr};
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

