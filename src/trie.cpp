#include "trie.hpp"  // It is forbidden to include other libraries!

// Constructors

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
trie<T>::trie(trie<T> const& rhs)
    : m_c(rhs.m_c){
    this->m_p = rhs.m_p;
    if(rhs.m_l){
        this->m_l = new T{*(rhs.m_l)};
    }else{
        this->m_l = nullptr;
    } 
    this->m_w = rhs.m_w;
    this->m_c = rhs.m_c;
    // Update the children' father with actual one
    this->m_c.update_parent(this);
}

/** Move constructor */
template <typename T>
trie<T>::trie(trie<T>&& rhs)
    :m_c(std::move(rhs.m_c)){
    this->m_p = rhs.m_p;
    this->m_l = rhs.m_l;
    rhs.m_l = nullptr;
    this->m_w = rhs.m_w;

    // Update the children' father with actual one
    this->m_c.update_parent(this);
}

/** Assignment operator overloaded */
template <typename T>
trie<T>& trie<T>::operator=(trie<T> const& rhs){
    // Copy only children and weight
    this->m_w = rhs.m_w;
    this->m_c = rhs.m_c;
    // Update the children' father with actual one
    this->m_c.update_parent(this);
    return *this;
}

/** Move assignment operator */
template <typename T>
trie<T>& trie<T>::operator=(trie<T>&& rhs){
    // Copy only children and weight
    this->m_w = rhs.m_w;
    this->m_c = std::move(rhs.m_c);
    // Update the children' father with actual one
    this->m_c.update_parent(this);
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
    if (!this->m_c.add_ordered(c, this)){
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
    std::cout << "M_I: " << &t << "| P_I: " << t.get_parent() << " | ";
    if (t.get_label())  std::cout << *(t.get_label()) << " ";
    if (t.get_parent() && t.get_children().empty()) std::cout << t.get_weight() << " ";
    if(!t.get_children().empty()){
        std::cout << "children = {\n";
        // t.get_children().print_children();
        // if (t.get_parent()){
        //     std::cout << "\n    }";
        // }else{
        //     std::cout << "\n}";
        // }
         auto it = t.get_children().begin();
         while(it != t.get_children().end()){
             std::cout << "    ";
             print_trie(*it);
             ++it;
             if(it != t.get_children().end()){
                 std::cout << ",\n";
             }
         }
         std::cout << "\n}";
    }else{
         std::cout << "children = {}";
    }
}

// Node Iterator

/** 
 * Creates an iterator that points to any trie
 * @param t trie to be pointed
 */
template <typename T>
trie<T>::node_iterator::node_iterator(trie<T>* t) : m_ptr(t) {}

/**
 * Gets the label of the trie the iterator points to
 * @returns A reference to the label
*/
template <typename T>
typename trie<T>::node_iterator::reference trie<T>::node_iterator::operator*() const{
    return (m_ptr->m_l) ? *(m_ptr->m_l) : throw parser_exception{"No label for the root"};
}

/**
 * Gets the pointer of the label of the trie the iterator points to
 * @returns Pointer to the label
*/
template <typename T>
typename trie<T>::node_iterator::pointer trie<T>::node_iterator::operator->() const{
    return &(*m_ptr);
}

/**
 * Make the iterator to point to his parent(Pre-increment)
 * @return A reference to an iterator that points at the parent
*/
template <typename T>
typename trie<T>::node_iterator& trie<T>::node_iterator::operator++(){
    this->m_ptr = this->m_ptr->m_p;
    return *this;
}

/**
 * Make the iterator to point to his parent(Post-increment)
 * @return A reference to an iterator that points at the parent
*/
template <typename T>
typename trie<T>::node_iterator trie<T>::node_iterator::operator++(int){
    node_iterator pre_increment(this->m_ptr);
    this->m_ptr++;
    return pre_increment;
}

/**
 * Check if 2 node_iterators are equal
 * @return If equal
*/
template <typename T>
bool trie<T>::node_iterator::operator==(node_iterator const& rhs) const{
    return this->m_ptr == rhs.m_ptr;
}

/**
 * Check if 2 node_iterators are not equal
 * @return If not equal
*/
template <typename T>
bool trie<T>::node_iterator::operator!=(node_iterator const& rhs) const{
    std::cout << this->m_ptr << " != " << rhs.m_ptr << std::endl;
    return this->m_ptr != rhs.m_ptr;
}

/** 
 * Returns an iterator that points to the root
 * @return node_iterator that points to the root
 */
template <typename T>
typename trie<T>::node_iterator trie<T>::root(){
    // node_iterator root{this};
    // while(root.m_ptr->m_p){
        // ++root;
    // }
    // return root;
    return  node_iterator{this};
}

// Const iterator

/** 
 * Creates an iterator that points to any trie
 * @param t trie to be pointed
 */
template <typename T>
trie<T>::const_node_iterator::const_node_iterator(const trie<T>* t) : m_ptr(t) {}

/**
 * Gets the label of the trie the iterator points to
 * @returns A reference to the label
*/
template <typename T>
typename trie<T>::const_node_iterator::reference trie<T>::const_node_iterator::operator*() const{
    return (m_ptr->m_l) ? *(m_ptr->m_l) : throw parser_exception{"No label for the root"};
}

/**
 * Gets the pointer of the label of the trie the iterator points to
 * @returns Pointer to the label
*/
template <typename T>
typename trie<T>::const_node_iterator::pointer trie<T>::const_node_iterator::operator->() const{
    return m_ptr->m_l;
}

/**
 * Make the iterator to point to his parent(Pre-increment)
 * @return A reference to an iterator that points at the parent
*/
template <typename T>
typename trie<T>::const_node_iterator& trie<T>::const_node_iterator::operator++(){
    if(this->m_ptr){
        this->m_ptr = this->m_ptr->m_p;
    }
    return *this;
}

/**
 * Make the iterator to point to his parent(Post-increment)
 * @return A reference to an iterator that points at the parent
*/
template <typename T>
typename trie<T>::const_node_iterator trie<T>::const_node_iterator::operator++(int){
    node_iterator pre_increment(this->m_ptr);
    this->m_ptr++;
    return pre_increment;
}

/**
 * Check if 2 node_iterators are equal
 * @return If equal
*/
template <typename T>
bool trie<T>::const_node_iterator::operator==(const_node_iterator const& rhs) const{
    return this->m_ptr == rhs.m_ptr;
}

/**
 * Check if 2 node_iterators are not equal
 * @return If not equal
*/
template <typename T>
bool trie<T>::const_node_iterator::operator!=(const_node_iterator const& rhs) const{
        //std::cout << this->m_ptr << " != " << rhs.m_ptr << std::endl;
    return this->m_ptr != rhs.m_ptr;
}

/** 
 * Returns an iterator that points to the root
 * @return node_iterator that points to the root
 */
template <typename T>
typename trie<T>::const_node_iterator trie<T>::root() const{
    return  const_node_iterator{this};
}


/** Operator ++ leaf */
/*
- vado al padre dell'attuale foglia
- cerco foglia nei figli, la prima che trovo raggiunta
- se non trovo vai ancora al padre
- e cerca ancora
- questo fino a quando non arrivo alla radice e non ha nessun figlio
*/

// Begin/costruttore
/*
- due casi:
- foglia tra i figli
- figli solo nodi
- se foglia sposta puntatore a quella
- se figlio guarda tra i figli, se foglia sposta
- se non foglia


*/

// Leaf iterator

/**
 * Instantiates a leaf iterator that points at the first leaf of the passed trie
 * @param t the trie whose first leaf has to be pointed
*/
template <typename T>
trie<T>::leaf_iterator::leaf_iterator(trie<T>* t){
    if(!t){
        this->m_ptr = t;
    }else{
        // The trie t can be a leaf or any other node
        // If t is a node, so the leaf has to be in his children or in his children' children
        // Check every first child until reach the leaf
        while(!t->m_c.empty()){
            // t points to the first child
            t = &(*(t->m_c.begin()));
        }
        this->m_ptr = t;
    }
}

/**
 * Returns the object that iterators points to
 * @return The trie pointed
*/
template <typename T>
typename trie<T>::leaf_iterator::reference trie<T>::leaf_iterator::operator*() const {
    return (m_ptr->m_l) ? *(m_ptr->m_l) : throw parser_exception{"No label for the root"};
}

/**
 * Returns the pointer to the object that iterators points to
 * @return The pointer to trie pointed
*/
template <typename T>
typename trie<T>::leaf_iterator::pointer trie<T>::leaf_iterator::operator->() const {
    return &(*m_ptr);
}

/**
 * Points to the next leaf(pre-increment)
 * @return An iterator that points to the next leaf || nullptr if no other leaves
*/
template <typename T>
typename trie<T>::leaf_iterator& trie<T>::leaf_iterator::operator++(){
    //std::cout << "\nOperator++" << std::endl;
    trie<T>* next_leaf = nullptr;
    // Search the next leaf in the children until find a valid one or reach the root
    while(!next_leaf && this->m_ptr->m_p){
    //     next_leaf = this->m_ptr->m_p->m_c.get_next(&(this->get_leaf()));
    //     // Go to the father
    //     this->m_ptr = this->m_ptr->m_p;
    // }
        trie<T> actual_leaf = this->get_leaf();
        auto it = this->m_ptr->m_p->m_c.begin();
        bool found = false;
        while(!found && it != this->m_ptr->m_p->m_c.end()){
            //std::cout << "ACTUAL" << std::endl;
            //print_trie(actual_leaf);
            //std::cout << std::endl;
            //std::cout << "POINTED BY ITERATOR: " << std::endl;
            //print_trie(*it);
            //std::cout << std::endl;
            if(*((*it).get_label()) == *(actual_leaf.get_label()) && *it == actual_leaf){
           // std::cout << "FOUND" << std::endl;
                found = true;
            }
            ++it;
        }
        if(found && (it != this->m_ptr->m_p->m_c.end())){
            next_leaf = &(*it);
        }
       // std::cout << "NEXT_LEAF:" << std::endl;   
        //if(next_leaf){
        //    print_trie(*next_leaf);
        //}//else{
         //   std::cout << "NULL" << std::endl;
       // }
        // Go to the father
        this->m_ptr = this->m_ptr->m_p;
    } 

    if(!next_leaf || next_leaf->m_c.empty()){
        this->m_ptr = next_leaf;
    }else{
        leaf_iterator pivot{next_leaf};
        this->m_ptr = pivot.m_ptr;
    }

    // print_trie(*(this->m_ptr->m_p));
    // trie<T> actual_leaf = this->get_leaf();
    // trie<T> actual_leaf = this->get_leaf();
    // // Go to the parent
    // this->m_ptr = this->m_ptr->m_p;
    // // Get the next child
    // while(!this->m_ptr->m_c.get_next(actual_leaf)){
    //     this->m_ptr = this->m_ptr->m_p;
    // }
    // // Once got the next child, instatiate a leaf iterator which will point to the first leaf
    // // The just steal its pointer
    // leaf_iterator pivot{this->m_ptr};
    // this->m_ptr = pivot.m_ptr;
    // return *this;


    // Until found a valid next_leaf or until there are no other children to check
    // trie<T>* next_leaf = nullptr;
    // while(!next_leaf && this->m_ptr->m_p){
    //     trie<T> actual_leaf = this->get_leaf();
    //     print_trie(actual_leaf);
    //     auto it = this->m_ptr->m_p->m_c.begin();
    //     bool found = false;
    //     while(!found && it != this->m_ptr->m_p->m_c.end()){
    //         if((*it).get_label() == actual_leaf.get_label() && *it == actual_leaf){
    //             found = true;
    //         }
    //         ++it;
    //     }
    //     if(found && (it != this->m_ptr->m_p->m_c.end())){
    //     //std::cout << "A" << std::endl;
    //         next_leaf = &(*it);
    //     }
    //     // print_trie(next_leaf->m_w);
    //     // Go to the father
    //     this->m_ptr = this->m_ptr->m_p;
    // }
    // this->m_ptr = next_leaf;
    return *this;
    // TODO test
}

/**
 * Points to the next leaf(post-increment)
 * @return An iterator that points to the next leaf || nullptr if no other leaves
*/
template <typename T>
typename trie<T>::leaf_iterator trie<T>::leaf_iterator::operator++(int){
    leaf_iterator pre_increment{this->m_ptr};
    // Get the next child
    while(!this->m_ptr->m_p->m_c.get_next(*(this->m_ptr))){
        this->m_ptr = this->m_ptr->m_p;
    }
    // Once got the next child, instatiate a leaf iterator which will point to the first leaf
    // The just steal its pointer
    leaf_iterator pivot{this->m_ptr};
    this->m_ptr = pivot.m_ptr;
    return pre_increment;
}

/**
 * Returns if 2 leaf operators are equal
 * @return Equal || Not Equal
*/
template <typename T>
bool trie<T>::leaf_iterator::operator==(leaf_iterator const& rhs) const{
    return this->m_ptr == rhs.m_ptr;
}

/**
 * Returns if 2 leaf operators are equal
 * @return Equal || Not Equal
*/
template <typename T>
bool trie<T>::leaf_iterator::operator!=(leaf_iterator const& rhs) const{
    return this->m_ptr != rhs.m_ptr;
}

/**
 * Istantiates a node iterator that points at the same node
 * @return The node iterator
*/
template <typename T>
trie<T>::leaf_iterator::operator node_iterator() const{
    return node_iterator{this->m_ptr};
}

/**
 * Returns a const reference at the actual leaf
 * @return Leaf
*/
template <typename T>
trie<T>& trie<T>::leaf_iterator::get_leaf() const{
    if(this->m_ptr){
        return *(this->m_ptr);
    }else{
        throw parser_exception{"No leaf pointed"};
    }
}

/**
 * Returns a leaf iterator that points at the first element
 * @return The leaf iterator 
*/
template <typename T>
typename trie<T>::leaf_iterator trie<T>::begin(){
    return leaf_iterator{&(*this)};
}

/**
 * Returns a leaf iterator that points at the next leaf of last element
 * @return The leaf iterator 
*/
template <typename T>
typename trie<T>::leaf_iterator trie<T>::end(){
    trie<T>* next_leaf = nullptr;
    // The end leaf is the first one of the next node so go at the father and reach the next node of this
    if(this->m_p){
         trie<T>* father_leaf = this->m_p;
         trie<T>* actual_leaf = this;
    // Search the next leaf in the children until find a valid one or reach the root
    while(!next_leaf && father_leaf){
    //     next_leaf = this->m_ptr->m_p->m_c.get_next(&(this->get_leaf()));
    //     // Go to the father
    //     this->m_ptr = this->m_ptr->m_p;
    // }
        //trie<T> actual_leaf = this->get_leaf();
        auto it = father_leaf->m_c.begin();
        bool found = false;
        while(!found && it != father_leaf->m_c.end()){
            //std::cout << "ACTUAL" << std::endl;
            //print_trie(actual_leaf);
            //std::cout << std::endl;
            //std::cout << "POINTED BY ITERATOR: " << std::endl;
            //print_trie(*it);
            //std::cout << std::endl;
            if(*((*it).get_label()) == *(actual_leaf->get_label()) && *it == *actual_leaf){
           // std::cout << "FOUND" << std::endl;
                found = true;
            }
            ++it;
        }
        if(found && (it != father_leaf->m_c.end())){
            next_leaf = &(*it);
        }
       // std::cout << "NEXT_LEAF:" << std::endl;   
        //if(next_leaf){
        //    print_trie(*next_leaf);
        //}//else{
         //   std::cout << "NULL" << std::endl;
       // }
        // Go to the father
        actual_leaf = actual_leaf->m_p;
        father_leaf = actual_leaf->m_p;
    }
    return {next_leaf};
    }else{
        return {nullptr};
    }
}

// Const leaf iterator

/**
 * Instantiates a leaf iterator that points at the first leaf of the passed trie
 * @param t the trie whose first leaf has to be pointed
*/
template <typename T>
trie<T>::const_leaf_iterator::const_leaf_iterator(const trie<T>* t){
    if(!t){
        this->m_ptr = t;
    }else{
        // The trie t can be a leaf or any other node
        // If t is a node, so the leaf has to be in his children or in his children' children
        // Check every first child until reach the leaf
        while(t->m_c.empty()){
            // t points to the first child
            t = &(*(t->m_c.begin()));
        }
    }
}

/**
 * Returns the object that iterators points to
 * @return The trie pointed
*/
template <typename T>
typename trie<T>::const_leaf_iterator::reference trie<T>::const_leaf_iterator::operator*() const {
    return (m_ptr->m_l) ? *(m_ptr->m_l) : throw parser_exception{"No label for the root"};
}

/**
 * Returns the pointer to the object that iterators points to
 * @return The pointer to trie pointed
*/
template <typename T>
typename trie<T>::const_leaf_iterator::pointer trie<T>::const_leaf_iterator::operator->() const {
    return &(*m_ptr);
}

/**
 * Points to the next leaf(pre-increment)
 * @return An iterator that points to the next leaf || nullptr if no other leaves
*/
template <typename T>
typename trie<T>::const_leaf_iterator& trie<T>::const_leaf_iterator::operator++(){
    // Get the next child
    while(!this->m_ptr->m_p->m_c.get_next(*(this->m_ptr))){
        this->m_ptr = this->m_ptr->m_p;
    }
    // Once got the next child, instatiate a leaf iterator which will point to the first leaf
    // The just steal its pointer
    leaf_iterator pivot{this->m_ptr};
    this->m_ptr = pivot.m_ptr;
    return *this;
}

/**
 * Points to the next leaf(post-increment)
 * @return An iterator that points to the next leaf || nullptr if no other leaves
*/
template <typename T>
typename trie<T>::const_leaf_iterator trie<T>::const_leaf_iterator::operator++(int){
    leaf_iterator pre_increment{this->m_ptr};
    // Get the next child
    while(!this->m_ptr->m_p->m_c.get_next(*(this->m_ptr))){
        this->m_ptr = this->m_ptr->m_p;
    }
    // Once got the next child, instatiate a leaf iterator which will point to the first leaf
    // The just steal its pointer
    leaf_iterator pivot{this->m_ptr};
    this->m_ptr = pivot.m_ptr;
    return pre_increment;
}

/**
 * Returns if 2 leaf operators are equal
 * @return Equal || Not Equal
*/
template <typename T>
bool trie<T>::const_leaf_iterator::operator==(const_leaf_iterator const& rhs) const{
    return this->m_ptr == rhs.m_ptr;
}

/**
 * Returns if 2 leaf operators are equal
 * @return Equal || Not Equal
*/
template <typename T>
bool trie<T>::const_leaf_iterator::operator!=(const_leaf_iterator const& rhs) const{
    return this->m_ptr != rhs.m_ptr;
}

/**
 * Istantiates a node iterator that points at the same node
 * @return The node iterator
*/
template <typename T>
trie<T>::const_leaf_iterator::operator const_node_iterator() const{
    return node_iterator{this->m_ptr};
}

/**
 * Returns a const reference at the actual leaf
 * @return Leaf
*/
template <typename T>
const trie<T>& trie<T>::const_leaf_iterator::get_leaf() const{
    return *(this->m_ptr);
}

/**
 * Returns a leaf iterator that points at the first element
 * @return The leaf iterator 
*/
template <typename T>
typename trie<T>::const_leaf_iterator trie<T>::begin() const{
    return const_leaf_iterator{&(*this)};
}

/**
 * Returns a leaf iterator that points at the next leaf of last element
 * @return The leaf iterator 
*/
template <typename T>
typename trie<T>::const_leaf_iterator trie<T>::end() const{
    // The next of last leaf is the first of the next node of trie parent of this
    if(this->m_p->m_c->get_next(&(*this))){
        return {this->m_p->m_c->get_next(&(*this))};
    }else{
        return {nullptr};
    }
}

// TODO OPERATOR == e testing iteratori
template <typename T>
bool trie<T>::operator==(trie<T> const& rhs) const{
    // Two tries are equal if: 
    // - 2 leaves(=>no children) && same weight
    // - Have same children 
    if(this->m_c.empty() && rhs.m_c.empty()){
        return this->m_w == rhs.m_w;
    }else{
        return this->m_c == rhs.m_c;
    }
}

template <typename T>
bool trie<T>::operator!=(trie<T> const& rhs) const{
    // Two tries are equal if: 
    // - 2 leaves(=>no children) && not same weight
    // - Not same children 
    if(this->m_c.empty() && rhs.m_c.empty()){
        return this->m_w != rhs.m_w;
    }else{
        return this->m_c != rhs.m_c;
    }
}

/**
 * Returns the leaf with max weight
 * @return The leaf with max weight
*/
template <typename T>
trie<T>& trie<T>::max(){
    leaf_iterator it{this};
    trie<T>* max = &(this->begin().get_leaf());
    ++it;
    while (it != this->end()){
        if(it.get_leaf().m_w > max->m_w){
            max = &(it.get_leaf());
        }
        ++it;
    }
    return *max;
}

/**
 * Returns the leaf with max weight
 * @return The leaf with max weight
*/
template <typename T>
trie<T> const& trie<T>::max() const{
    leaf_iterator it{this};
    trie<T>* max = &(this->begin().get_leaf());
    ++it;
    while (it != this->end()){
        if(it.get_leaf().m_w > max->m_w){
            max = &(it.get_leaf());
        }
        ++it;
    }
    return *max;
}

/**
 * Returns a reference to trie reached using the sequence
 * @param s The sequence with the labels
 * @return The reference to the reached trie
*/
template <typename T>
trie<T>& trie<T>::operator[](std::vector<T> const& s){
    trie<T>* reached_trie = this;
    /** Index of the next label to search */
    long unsigned int next_label = 0;
    bool contains_label = true;
    while(contains_label && next_label < s.size()){
        // Search if there is a children with the label_searched
        auto it = reached_trie->m_c.begin();
        bool found = false;
        while (!found && it != reached_trie->m_c.end()){
            if(*((*it).get_label()) == s.at(next_label)){
                found = true;
            }else{
                ++it;
            }
        }
        if(!found){
            contains_label = false;
        }else{
            reached_trie = &(*it);
            next_label++;
        }
    }
    return (*reached_trie);
}

/**
 * Returns a reference to trie reached using the sequence
 * @param s The sequence with the labels
 * @return The reference to the reached trie
*/
template <typename T>
trie<T> const& trie<T>::operator[](std::vector<T> const& s) const{
    trie<T>* reached_trie = this;
    /** Index of the next label to search */
    long unsigned int next_label = 0;
    bool contains_label = true;
    while(contains_label && next_label < s.size()){
        // Search if there is a children with the label_searched
        auto it = this->m_c.begin();
        bool found = false;
        while (!found && it != this->m_c.end()){
            if(*((*it).get_label()) == s.at(next_label)){
                found = true;
            }
            ++it;
        }
        if(!found){
            contains_label = false;
        }else{
            reached_trie = &(*it);
            next_label++;
        }
    }
    return *reached_trie;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, trie<T> const& t){
    // if (t.get_label())  os << *(t.get_label()) << " ";
    // if (t.get_parent() && t.get_children().empty()) std::cout << t.get_weight() << " ";
    // if(!t.get_children().empty()){
    //     std::cout << "children = {\n";
    //     auto it = t.get_children().begin();
    //     while(it != t.get_children().end()){
    //          std::cout << "    ";
    //          print_trie(*it);
    //          ++it;
    //         //  if(it != t.get_children().end()){
    //         //      std::cout << ",\n";
    //         //  }
    //      }
    //      std::cout << "\n}";
    // }else{
    //      std::cout << "children = {}";
    // }
    print_indented(os, t, t);
    return os;
}

template <typename T>
std::ostream& print_indented(std::ostream& os, trie<T> const& actual, trie<T> const& father){
    // typename trie<T>::const_node_iterator it{&actual};
    // std::cout << *(father.root()) << " ||||";
    // // while(it != father.root()){
    // //     os << "    ";
    // //     // Go up
    // //     ++it;
    // // }

    if(actual.get_parent() && actual.get_children().empty()){ // Leaf
        os << *(actual.get_label()) << " " << actual.get_weight() << " ";
    }else if(actual.get_parent() && !actual.get_children().empty()){ // Node with children
        os << *(actual.get_label()) << " ";
    }

    if(actual.get_children().empty()){
        os << "children = {}";
    }else{
        os << "children = {\n";
        // Write the children using iterators
        auto it = actual.get_children().begin();
        while(it != actual.get_children().end()){
            insert_tabs(os, *it, father);
            print_indented(os, *it, father);
            // Check next
            if(++it != actual.get_children().end()){
                os << ",\n";
            }
        }
        os << "\n";
        insert_tabs(os, actual, father);
        os << "}";
    }

    return os;
}


/**
 * Insert tabs on output stream
 * @param os the output stream to write on
 * @param t trie that has to be written after some tabs depending on which child it is
 * @return written os
*/
template <typename T>
std::ostream& insert_tabs(std::ostream& os, trie<T> const& actual, trie<T> const& father){
    typename trie<T>::const_node_iterator it{&actual};
    // std::cout << *(father.root()) << " ||||";
    // std::cout << *(++it) << " ----";
    // std::cout << ((++it) != father.root()) << " ----";
     while(it != father.root()){
         os << "    ";
         // Go up
         ++it;
     }
    return os;
}
