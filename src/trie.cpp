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

/** Destructor */
template <typename T>
trie<T>::~trie(){
    // Destroy only the label, the trie parent is in the stack
    // it will be automatically destroyed.
    // The destructor of bag will be automatically called.
    if(this->m_l) delete this->m_l;
}

// Assignment operators

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

// Setters

/** Set weight to a leaf */
template <typename T>
void trie<T>::set_weight(double w){
    // Set the weight only if leaf
    if(!this->m_c.empty()) this->m_w = w;
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

/** Set the parent */
template <typename T>
void trie<T>::set_parent(trie<T>* p){
    this->m_p = p;
}

/** Add a child in the bag */
template <typename T>
void trie<T>::add_child(trie<T> const& c){
    // If the element can't be added, this happens only if there is a child with some label
    // Pass also this in order to update m_p on the new child deep copied
    if (!this->m_c.add_ordered(c, this)){
        throw parser_exception{"There is already a child with same label"};
    }
}

// Getters

/** Returns the weight */
template <typename T>
double trie<T>::get_weight() const{
    return this->m_w;
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

/** Get the label */
template <typename T>
T* trie<T>::get_label(){
    // If not the root
    if(this->m_p){
        return this->m_l;
    }else{
        return nullptr;
    }
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

/** Get the parent */
template <typename T>
trie<T>* trie<T>::get_parent(){
    if(this->m_p){
        return this->m_p;
    }else{
        return nullptr;
    }
}

/** Returns the bag of children */
template <typename T>
bag<trie<T>> const& trie<T>::get_children() const {
    return this->m_c;
}

/** Returns the bag of children */
template <typename T>
bag<trie<T>>& trie<T>::get_children() {
    return this->m_c;
}

// Comparison

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
    // Two tries are not equal if: 
    // - 2 leaves(=>no children) && not same weight
    // - Not same children 
    if(this->m_c.empty() && rhs.m_c.empty()){
        return this->m_w != rhs.m_w;
    }else{
        return this->m_c != rhs.m_c;
    }
}

// Prefix search

/**
 * Returns a reference to trie reached using the sequence
 * @param s The sequence with the labels
 * @return The reference to the reached trie
*/
template <typename T>
trie<T>& trie<T>::operator[](std::vector<T> const& s){
    /** 
     * Pointer to the reached trie.
     * PS: can't do a reference because references can't change the referenced obj
     */
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
            // Get the pointer
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
    /** 
     * Pointer to the reached trie.
     * PS: can't do a reference because references can't change the referenced obj
     */
    const trie<T>* reached_trie = this;
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
            // Get the pointer
            reached_trie = &(*it);
            next_label++;
        }
    }
    return (*reached_trie);
}

// Max weight leaf

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
    const_leaf_iterator it{this};
    const trie<T>* max = &(this->begin().get_leaf());
    ++it;
    while (it != this->end()){
        if(it.get_leaf().m_w > max->m_w){
            max = &(it.get_leaf());
        }
        ++it;
    }
    return *max;
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
    if(!this->m_ptr){
        throw parser_exception{"No node pointed"};
    }else if(!this->m_ptr->m_l){
        throw parser_exception{"No label for the root"};
    }else{
        return *(this->m_ptr->m_l);
    }
}

/**
 * Gets the pointer of the label of the trie the iterator points to
 * @returns Pointer to the label
*/
template <typename T>
typename trie<T>::node_iterator::pointer trie<T>::node_iterator::operator->() const{
    return m_ptr->m_l;
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
    // Not incremented object to return
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
    return this->m_ptr != rhs.m_ptr;
}

/** 
 * Returns an iterator that points to the root
 * @return node_iterator that points to the root
 */
template <typename T>
typename trie<T>::node_iterator trie<T>::root(){
    // Root of any trie is the trie its self
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
    if(!this->m_ptr){
        throw parser_exception{"No node pointed"};
    }else if(!this->m_ptr->m_l){
        throw parser_exception{"No label for the root"};
    }else{
        return *(this->m_ptr->m_l);
    }
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
    // Not incremented object to return
    const_node_iterator pre_increment(this->m_ptr);
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
    return this->m_ptr != rhs.m_ptr;
}

/** 
 * Returns an iterator that points to the root
 * @return node_iterator that points to the root
 */
template <typename T>
typename trie<T>::const_node_iterator trie<T>::root() const{
    // Root of any trie is the trie its self
    return const_node_iterator{this};
}

// Leaf iterator

/**
 * Instantiates a leaf iterator that points at the first leaf of the passed trie or points its sel if leaf
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
 * Returns the label of the object that iterators points to
 * @return The label pointed
*/
template <typename T>
typename trie<T>::leaf_iterator::reference trie<T>::leaf_iterator::operator*() const {
    return (m_ptr->m_l) ? *(m_ptr->m_l) : throw parser_exception{"No label for the root"};
}

/**
 * Returns the pointer to the label of the object that iterators points to
 * @return The pointer to label pointed
*/
template <typename T>
typename trie<T>::leaf_iterator::pointer trie<T>::leaf_iterator::operator->() const {
    return m_ptr->m_l;
}

/**
 * Points to the next leaf(pre-increment)
 * @return An iterator that points to the next leaf || nullptr if no other leaves
*/
template <typename T>
typename trie<T>::leaf_iterator& trie<T>::leaf_iterator::operator++(){
    trie<T>* next_node = nullptr;
    // Search the next leaf in the children until find a valid one or reach the root
    while(!next_node && this->m_ptr->m_p){
        trie<T> actual_leaf = this->get_leaf();
        auto it = this->m_ptr->m_p->m_c.begin();
        bool found = false;
        while(!found && it != this->m_ptr->m_p->m_c.end()){
            if(*((*it).get_label()) == *(actual_leaf.get_label()) && *it == actual_leaf){
                found = true;
            }
            ++it;
        }
        if(found && (it != this->m_ptr->m_p->m_c.end())){
            next_node = &(*it);
        }
        // Go to the father
        this->m_ptr = this->m_ptr->m_p;
    } 

    if(!next_node || next_node->m_c.empty()){
        this->m_ptr = next_node;
    }else{
        leaf_iterator pivot{next_node};
        this->m_ptr = pivot.m_ptr;
    }
    return *this;
}

/**
 * Points to the next leaf(post-increment)
 * @return An iterator that points to the next leaf || nullptr if no other leaves
*/
template <typename T>
typename trie<T>::leaf_iterator trie<T>::leaf_iterator::operator++(int){
    leaf_iterator pre_increment{this->m_ptr};
    ++(*this);
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
 * Returns if 2 leaf operators are not equal
 * @return T(!Equal) || F(Equal)
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
    trie<T>* next_node = nullptr;
    // The end leaf is the first one of the next node so go at the father and reach the next node of this
    if(this->m_p){
        trie<T>* father_node = this->m_p;
        trie<T>* actual_leaf = this;
        // Search the next leaf in the children until find a valid one or reach the root
        while(!next_node && father_node){
            auto it = father_node->m_c.begin();
            bool found = false;
            while(!found && it != father_node->m_c.end()){
                if(*((*it).get_label()) == *(actual_leaf->get_label()) && *it == *actual_leaf){
                    found = true;
                }
                ++it;
            }
            if(found && (it != father_node->m_c.end())){
                next_node = &(*it);
            }
            // Go to the father
            actual_leaf = actual_leaf->m_p;
            father_node = actual_leaf->m_p;
        }
        return {next_node};
    }else{
        return {nullptr};
    }
}

// Const leaf iterator

/**
 * Instantiates a const leaf iterator that points at the first leaf of the passed trie or points its sel if leaf
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
        while(!t->m_c.empty()){
            // t points to the first child
            t = &(*(t->m_c.begin()));
        }
        this->m_ptr = t;
    }
}

/**
 * Returns the label of the object that iterators points to
 * @return The label pointed
*/
template <typename T>
typename trie<T>::const_leaf_iterator::reference trie<T>::const_leaf_iterator::operator*() const {
    return (m_ptr->m_l) ? *(m_ptr->m_l) : throw parser_exception{"No label for the root"};
}

/**
 * Returns the pointer to the label of the object that iterators points to
 * @return The pointer to label pointed
*/
template <typename T>
typename trie<T>::const_leaf_iterator::pointer trie<T>::const_leaf_iterator::operator->() const {
    return m_ptr->m_l;
}

/**
 * Points to the next leaf(pre-increment)
 * @return An iterator that points to the next leaf || nullptr if no other leaves
*/
template <typename T>
typename trie<T>::const_leaf_iterator& trie<T>::const_leaf_iterator::operator++(){
    trie<T>* next_node = nullptr;
    // Search the next leaf in the children until find a valid one or reach the root
    while(!next_node && this->m_ptr->m_p){
        trie<T> actual_leaf = this->get_leaf();
        auto it = this->m_ptr->m_p->m_c.begin();
        bool found = false;
        while(!found && it != this->m_ptr->m_p->m_c.end()){
            if(*((*it).get_label()) == *(actual_leaf.get_label()) && *it == actual_leaf){
                found = true;
            }
            ++it;
        }
        if(found && (it != this->m_ptr->m_p->m_c.end())){
            next_node = &(*it);
        }
        // Go to the father
        this->m_ptr = this->m_ptr->m_p;
    } 

    if(!next_node || next_node->m_c.empty()){
        this->m_ptr = next_node;
    }else{
        const_leaf_iterator pivot{next_node};
        this->m_ptr = pivot.m_ptr;
    }
    return *this;
}

/**
 * Points to the next leaf(post-increment)
 * @return An iterator that points to the next leaf || nullptr if no other leaves
*/
template <typename T>
typename trie<T>::const_leaf_iterator trie<T>::const_leaf_iterator::operator++(int){
    const_leaf_iterator pre_increment{this->m_ptr};
    ++(*this);
    return pre_increment;
}

/**
 * Returns if 2 const leaf operators are equal
 * @return Equal || Not Equal
*/
template <typename T>
bool trie<T>::const_leaf_iterator::operator==(const_leaf_iterator const& rhs) const{
    return this->m_ptr == rhs.m_ptr;
}

/**
 * Returns if 2 const leaf operators are not equal
 * @return T(!Equal) || F(Equal)
*/
template <typename T>
bool trie<T>::const_leaf_iterator::operator!=(const_leaf_iterator const& rhs) const{
    return this->m_ptr != rhs.m_ptr;
}

/**
 * Istantiates a const node iterator that points at the same node
 * @return The node iterator
*/
template <typename T>
trie<T>::const_leaf_iterator::operator const_node_iterator() const{
    return const_node_iterator{this->m_ptr};
}

/**
 * Returns a const reference at the actual leaf
 * @return Leaf
*/
template <typename T>
const trie<T>& trie<T>::const_leaf_iterator::get_leaf() const{
    if(this->m_ptr){
        return *(this->m_ptr);
    }else{
        throw parser_exception{"No leaf pointed"};
    }
}

/**
 * Returns a const leaf iterator that points at the first element
 * @return The leaf iterator 
*/
template <typename T>
typename trie<T>::const_leaf_iterator trie<T>::begin() const{
    return const_leaf_iterator{&(*this)};
}

/**
 * Returns a const leaf iterator that points at the next leaf of last element
 * @return The const leaf iterator 
*/
template <typename T>
typename trie<T>::const_leaf_iterator trie<T>::end() const{
    trie<T>* next_node = nullptr;
    // The end leaf is the first one of the next node so go at the father and reach the next node of this
    if(this->m_p){
        trie<T>* father_node = this->m_p;
        const trie<T>* actual_leaf = this;
        // Search the next leaf in the children until find a valid one or reach the root
        while(!next_node && father_node){
            auto it = father_node->m_c.begin();
            bool found = false;
            while(!found && it != father_node->m_c.end()){
                if(*((*it).get_label()) == *(actual_leaf->get_label()) && *it == *actual_leaf){
                    found = true;
                }
                ++it;
            }
            if(found && (it != father_node->m_c.end())){
                next_node = &(*it);
            }
            // Go to the father
            actual_leaf = actual_leaf->m_p;
            father_node = actual_leaf->m_p;
        }
        return {next_node};
    }else{
        return {nullptr};
    }
}

// Writes on stream
template <typename T>
std::ostream& operator<<(std::ostream& os, trie<T> const& t){
    // Call to a support function, bc need a param for the father just to write indented
    print_indented(os, t, t);
    return os;
}


/**
 * Writes on a stream an indented trie
 * @param os the stream to write on
 * @param actual the trie to print
 * @param father the first trie(root) to calculate the tabs
 * @return reference to written OS
*/
template <typename T>
std::ostream& print_indented(std::ostream& os, trie<T> const& actual, trie<T> const& father){
    if(actual.get_parent() && actual.get_children().empty()){ // Leaf
        os << *(actual.get_label()) << " " << actual.get_weight() << " ";
    }else if(!actual.get_parent() && actual.get_children().empty()){ // Leaf as root
       os << actual.get_weight() << " ";
    }else if(actual.get_parent() && !actual.get_children().empty()){
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
 * @param os the stream to write on
 * @param actual the trie to print
 * @param father the first trie(root) to calculate the tabs
 * @return written os
*/
template <typename T>
std::ostream& insert_tabs(std::ostream& os, trie<T> const& actual, trie<T> const& father){
    // Node iterator that will start from actual node and will go to the first printed trie(father)
    typename trie<T>::const_node_iterator it{&actual};
    while(it != father.root()){
        os << "    ";
        // Go up
        ++it;
    }
    return os;
}

/* Puts the next character in the first position without any spaces */
void skip_blank_spaces(std::istream& is) {
    char c = 0;
    // Operator >> consume all the non-valid chars
    is >> c;
    is.putback(c);
}

/**
 * Parse a LEAF -> WEIGHT children = {}
 * @param is the stream to read from
 * @param t the trie to be written
 * @return the created trie(leaf)
*/
template <typename T>
void leaf(std::istream& is, trie<T>& t){
    skip_blank_spaces(is);
    // Try to parse the weight
    double weight = 0.0;
    is >> weight;
    if(is.fail()){ // The value in the is isn't parsed as double, goes in fail
        throw parser_exception{"The weight can't be parsed as double"};
    }else{ // Parsed
        // Try to read children = {}
        std::string s = "";
        is >> s;
        if (s != "children") throw parser_exception{"Expected keyword 'children'"};
        skip_blank_spaces(is);
        char c = 0;
        is >> c;
        if (c != '=') throw parser_exception{"Expected keyword '='"};
        skip_blank_spaces(is);
        is >> c;
        if (c != '{') throw parser_exception{"Expected keyword '{'"};
        skip_blank_spaces(is);
        is >> c;
        if (c != '}') throw parser_exception{"Expected keyword '}'"};
        t = trie<T>{weight};
    }
}

/**
 * Parse a NODE from an input stream
 * @param is the stream to read from
 * @param t the trie to be written
 * @return the created trie
*/
template <typename T>
void node(std::istream& is, trie<T>& t){
    skip_blank_spaces(is);
    // Any node has the label, try to parse it
    T label;
    is >> label;
    skip_blank_spaces(is);
    if(is.fail()){ // The label in the is isn't parsed as T, goes in fail
        throw parser_exception{"The label can't be parsed as type T"};
    }else if(is.peek() >= 48 && is.peek() <= 57){ // Leaf
        // TODO occhio al fatto che magari trie può avere già qualcosa dentro
        trie<T> leaf_to_add;
        leaf(is, leaf_to_add);
        leaf_to_add.set_label(&label);
        leaf_to_add.set_parent(&t);
        t.add_child(leaf_to_add);
    }else{ // NODE
        // Try to read children = {NODE}
        std::string s = "";
        is >> s;
        if (s != "children") throw parser_exception{"Expected keyword 'children'"};
        skip_blank_spaces(is);
        char c = 0;
        is >> c;
        if (c != '=') throw parser_exception{"Expected keyword '='"};
        skip_blank_spaces(is);
        is >> c;
        if (c != '{') throw parser_exception{"Expected keyword '{'"};
        
        trie<T> node_to_add;
        node(is, node_to_add);
        node_to_add.set_label(&label);
        node_to_add.set_parent(&t);
        t.add_child(node_to_add);
        
        skip_blank_spaces(is);
        is >> c;
        if (c != '}') throw parser_exception{"Expected keyword '}'"};
    }

    // Check for NODE, NODE
    skip_blank_spaces(is);
    char c = 0;
    is >> c;
    if(c == ','){
        node(is, t);
    }else{
        is.putback(c);
    }

}

// Read from stream
/**
 * Parse the data from a stream in the passed trie
 * @param is the stream to read from
 * @param t the trie to be written
 * @return is read 
 * Used CFG to parse:
 * ROOT(FIRST TRIE) -> LEAF | children = {NODE}
 * NODE -> LABEL LEAF | LABEL children = {NODE} | NODE, NODE
 * LEAF -> WEIGHT children = {}
 * LABEL -> any unseparated data of type T;
 * WEIGHT -> any double;
 */
template <typename T>
std::istream& operator>>(std::istream& is, trie<T>& t){
    skip_blank_spaces(is);
    char c = is.peek();
    if((int) c >= 48 && (int) c <= 57){ // Root has to be parsed as a LEAF(first case in CFG ROOT)
        leaf(is, t);
    }else{ // Node
        // Try to read children = {NODE}
        std::string s = "";
        is >> s;
        if (s != "children") throw parser_exception{"Expected keyword 'children'"};
        skip_blank_spaces(is);
        is >> c;
        if (c != '=') throw parser_exception{"Expected keyword '='"};
        skip_blank_spaces(is);
        is >> c;
        if (c != '{') throw parser_exception{"Expected keyword '{'"};
        
        node(is, t);
        
        skip_blank_spaces(is);
        is >> c;
        if (c != '}') throw parser_exception{"Expected keyword '}'"};
    }

    // Make sure reached the end of file
    skip_blank_spaces(is);
    if(is.peek() != EOF) throw parser_exception{"Unexpected char detected"};
    return is;
}

// Facultative: union

/**
 * Sums this and another 1 different trie in a new one
 * @param op2 second trie operand(first is this)
 * @return New trie result
*/
template <typename T>
trie<T> trie<T>::operator+(trie<T> const& op2) const {
    if(this->m_c.empty() && op2.m_c.empty()){ // Both are leaves
        trie<T> result = *this;
        result.m_w += op2.m_w;
        return result;
    }else if(!this->m_c.empty() && op2.m_c.empty()){ // The second operand is a leaf
        trie<T> result = *this;
        for(auto it = result.m_c.begin(); it != result.m_c.end(); ++it){
            (*it) = (*it) + op2;
        }
        return result;
    }else if(this->m_c.empty() && !op2.m_c.empty()){ // The first operand is a leaf
        trie<T> result = op2;
        for(auto it = result.m_c.begin(); it != result.m_c.end(); ++it){
            (*it) = (*it) + *this;
        }
        return result;
    }else{
        trie<T> result = *this;
        // Check the children of the second trie
        for(auto it = op2.m_c.begin(); it != op2.m_c.end(); ++it){
            // Try to add second operand child to the result
            bool added = result.m_c.add_ordered(*it, &result);
            if(!added){ // Adding fail
                std::vector<T> s{*(it->m_l)};
                result[s] = result[s] + (*it);
            }
        }
        return result;
    }
}

/**
 * Sums in this another 1 different
 * @param op2 second trie operand(first is this)
 * @return This trie modified
*/
template <typename T>
trie<T>& trie<T>::operator+=(trie<T> const& op2) {
    if(this->m_c.empty() && op2.m_c.empty()){ // Both are leaves
        this->m_w += op2.m_w;
        return *this;
    }else if(!this->m_c.empty() && op2.m_c.empty()){ // The second operand is a leaf
        for(auto it = this->m_c.begin(); it != this->m_c.end(); ++it){
            (*it) = (*it) + op2;
        }
        return *this;
    }else if(this->m_c.empty() && !op2.m_c.empty()){ // The first operand is a leaf
        trie<T> tmp{*this};
        *this = op2; 
        for(auto it = this->m_c.begin(); it != this->m_c.end(); ++it){
            (*it) = (*it) + tmp;
        }
        return *this;;
    }else{
        // Check the children of the second trie
        for(auto it = op2.m_c.begin(); it != op2.m_c.end(); ++it){
            // Try to add second operand child to the result
            bool added = this->m_c.add_ordered(*it, this);
            if(!added){ // Adding fail
                std::vector<T> s{*(it->m_l)};
                (*this)[s] = (*this)[s] + (*it);
            }
        }
        return *this;
    }
}

// Facultative: path compression

/** Compress the children with just one children in one trie */
template <typename T>
void trie<T>::path_compress(){
    if(this->m_c.empty()){ // Leaf
       return;
    }else if(this->m_p && this->m_c.has_one_child()){
        (*(this->m_c.begin())).path_compress();
        int* tmp_l = new int{*(this->m_l) + *(this->m_c.begin()->m_l)};
        this->set_label(tmp_l);
        delete tmp_l;
        trie<T> next_children{*(this->m_c.begin())};
        *this = next_children;
    }else{
        for(auto it = this->m_c.begin(); it != this->m_c.end(); ++it){
            (*it).path_compress();
        }
        this->m_c.reorder();
    }
}

