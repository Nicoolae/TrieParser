#include "trie.hpp"  // It is forbidden to include other libraries!

/** Default constructor */
template <typename T>
trie<T>::trie()
    : m_c(){
    // Creates a default trie like this:
    // 0.0 children = {}
    this->m_p = nullptr;
    this->m_l = nullptr;
    this->m_w = 0.0;
}

/** Conversion constructor */
template <typename T>
trie<T>::trie(double weight)
    : m_c(){
    // Creates a leaf(which is a trie at this moment) like this
    // $weight children = {}
    this->m_p = nullptr;
    this->m_l = nullptr;
    this->m_w = weight;
}

/** Copy constructor */
template <typename T>
trie<T>::trie(trie<T> const& rhs){
    this->m_p = rhs.m_p;
    if (rhs.m_l) m_l = new T{*(rhs.m_l)};
    this->m_w = rhs.m_w;
    this->m_c = rhs.m_c;
}

/** Move constructor */
template <typename T>
trie<T>::trie(trie<T>&& rhs){
    std::cout << "AOO";
    this->m_p = rhs.m_p;
    this->m_l = rhs.m_l;
    this->m_w = rhs.m_w;
    this->m_c = rhs.m_c;

    rhs.m_p = nullptr;
    rhs.m_l = nullptr;
}

/** Assignment operator overloaded */
template <typename T>
trie<T>& trie<T>::operator=(trie<T> const& rhs){
    // Copy only children and weight
    this->m_w = rhs.m_w;
    this->m_c = rhs.m_c;
    return *this;
}

/** Move assignment operator */
template <typename T>
trie<T>& trie<T>::operator=(trie<T>&& rhs){
    // Copy only children and weight
    this->m_w = rhs.m_w;
    this->m_c = rhs.m_c;
    return *this;
}

/** Destructor */
template <typename T>
trie<T>::~trie(){
    // Destroy only the label, the trie parent is in the stack
    // it will be automatically destroyed.
    // The destructor of bag will be automatically called.
    if(this->m_l) delete this->m_l;
}

/** Set weight to a leaf */
template <typename T>
void trie<T>::set_weight(double w){
    // Set the weight only if leaf
    if(!this->m_c.empty()) this->m_w = w;
}

/** Returns the weight */
template <typename T>
double trie<T>::get_weight() const{
    return this->m_w;
}

/** Set label */
template <typename T>
void trie<T>::set_label(T* l){
    // First delete the prev label
    if(this->m_l) delete this->m_l;
    // Duplicate the ptr
    T* new_l = new T{(*l)};
    this->m_l = new_l;
}

/** Get the label */
template <typename T>
T const* trie<T>::get_label() const{
    // If not the root
    if(this->m_p){
        return this->m_l;
    }else{
        return nullptr;
    }
}

/** Set the parent */
template <typename T>
void trie<T>::set_parent(trie<T>* p){
    this->m_p = p;
}

/** Get the parent */
template <typename T>
trie<T> const* trie<T>::get_parent() const{
    if(this->m_p){
        return this->m_p;
    }else{
        return nullptr;
    }
}

/** Add a child in the bag */
template <typename T>
void trie<T>::add_child(trie<T> const& c){
    // If the element can't be added, this happens only if there is a child with some label
    if (!this->m_c.add_ordered(c)){
        throw parser_exception{"There is already a child with same label"};
    }
}

/** Returns the bag of children */
template <typename T>
bag<trie<T>> const& trie<T>::get_children() const {
    return this->m_c;
}

template <typename T>
void print_trie(trie<T> const& t){
    if (t.get_label())  std::cout << *(t.get_label()) << " ";
    if (t.get_parent() && t.get_children().empty()) std::cout << t.get_weight() << " ";
    if(!t.get_children().empty()){
        std::cout << "children = {\n    ";
        t.get_children().print_children();
        if (t.get_parent()){
            std::cout << "\n    }";
        }else{
            std::cout << "\n}";
        }
    }else{
         std::cout << "children = {}";
    }
}
