#include "trie.hpp"  // It is forbidden to include other libraries!

/** Default constructor */
trie::trie(){
    // Creates a default trie like this: 
    //0.0 children = {}
    this->m_p = nullptr;
    this->m_l = nullptr;
    this->m_w = 0.0;
}