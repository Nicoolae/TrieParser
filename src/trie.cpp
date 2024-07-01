#include "trie.hpp"  // It is forbidden to include other libraries!

/** Default constructor */
template <typename T>
trie<T>::trie(){
    // Creates a default trie like this:
    // 0.0 children = {}
    this->m_p = nullptr;
    this->m_l = nullptr;
    this->m_w = 0.0;
    this->m_c = bag<trie<T>>();
}

/** Conversion constructor */
template <typename T>
trie<T>::trie(double weight){
    // Creates a leaf(which is a trie at this moment) like this
    // $weight children = {}
    this->m_p = nullptr;
    this->m_l = nullptr;
    this->m_w = weight;
    this->m_c = bag<trie<T>>();
}

/** Copy constructor */
template <typename T>
trie<T>::trie(trie<T> const& rhs){
    this->m_p = rhs.m_p;
    this->m_l = new T{rhs.m_l};
    this->m_w = rhs.m_w;
    this->m_c = rhs.m_c;
}

/** Move constructor */
template <typename T>
trie<T>::trie(trie<T>&& rhs){
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
    // Copy only children
    this->m_c = rhs.m_c;
    return *this;
}

/** Move assignment operator */
template <typename T>
trie<T>& trie<T>::operator=(trie<T>&& rhs){
    // Copy only children
    this->m_c = rhs.m_c;
    return *this;
}

/** Destructor */
template <typename T>
trie<T>::~trie(){
    
}