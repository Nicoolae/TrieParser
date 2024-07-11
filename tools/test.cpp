#include <iostream>
#include <string>
#include "../src/trie.cpp"

template <typename T>
trie<T> foo(trie<T> a){
    return a;
}

int main(){
    /** TEST GETTERS E SETTERS */
    /*
    trie<int> t_1;
    std::cout << "T1: ";
    print_trie(t_1);

    trie<int> t_2{2.1};

    t_2.set_parent(&t_1);
    int* a = new int{21};
    t_2.set_label(a);
    delete a;
    std::cout << "\nT2: ";
    print_trie(t_2);


    trie<int> t_3{81.0};
    t_3.set_parent(&t_1);
    a = new int{100};
    t_3.set_label(a);
    delete a;
    std::cout << "\nT3: ";
    print_trie(t_3);

    trie<int> t_4{6.0};
    t_4.set_parent(&t_1);
    a = new int{77};
    t_4.set_label(a);
    delete a;
    std::cout << "\nT4: ";
    print_trie(t_4);

    try{
        t_1.add_child(t_2);
        t_1.add_child(t_3);
        t_1.add_child(t_4);
    }catch(parser_exception e){
        std::cerr << e.what() << '\n';
        return 1;
    }

    std::cout << "\nT1(after): \n";
    print_trie(t_1);
    */

    
    // TEST COPY/MOVE SEMANTICS
    trie<std::string> t_p_1;
    trie<std::string> t_p_2;
    trie<std::string> t_c_1_1{1.0};
    trie<std::string> t_c_1_2{2.0};
    trie<std::string> t_c_2_1{3.0};
    trie<std::string> t_c_2_2;
    trie<std::string> t_c_2_2_1{4.0};
    trie<std::string> t_c_2_2_2{5.0};

    t_c_1_1.set_parent(&t_p_1);
    std::string* a = new std::string{"a"};
    t_c_1_1.set_label(a);
    delete a;

    t_c_1_2.set_parent(&t_p_1);
    a = new std::string{"b"};
    t_c_1_2.set_label(a);
    delete a;

    t_c_2_1.set_parent(&t_p_2);
    a = new std::string{"c"};
    t_c_2_1.set_label(a);
    delete a;

    t_c_2_2.set_parent(&t_p_2);
    a = new std::string{"d"};
    t_c_2_2.set_label(a);
    delete a;

    t_c_2_2_1.set_parent(&t_c_2_2);
    a = new std::string{"e"};
    t_c_2_2_1.set_label(a);
    delete a;

    t_c_2_2_2.set_parent(&t_c_2_2);
    a = new std::string{"f"};
    t_c_2_2_2.set_label(a);
    delete a;

    try{
        t_c_2_2.add_child(t_c_2_2_1);
        t_c_2_2.add_child(t_c_2_2_2);
        t_p_2.add_child(t_c_2_1);
        t_p_2.add_child(t_c_2_2);
        
        // Prova operator =
        //t_c_1_1 = t_p_2;

        t_p_1.add_child(t_c_1_1);
        t_p_1.add_child(t_c_1_2);       
    }catch(parser_exception e){
        std::cerr << e.what() << '\n';
        return 1;
    }

    // Copy constructor
    //trie<std::string> t_3{t_p_1};

    // Move constructor
    // trie<std::string> t_3 = foo(t_p_1);

    // Move assignment operator
    // trie<std::string> t_3;
    // std::cout << "AOO2";
    // t_3 = foo(t_p_1);

    print_trie(t_p_1);
    std::cout << std::endl;
    print_trie(t_p_2);
    std::cout << std::endl;
    //print_trie(t_3);

    // Operator ==
    //std::cout << (t_c_2_1 != t_c_2_2) << std::endl;
    
    // Node iterator
    // for (auto it = trie<std::string>::node_iterator{&t_c_1_1}; it != t_p_1.root(); ++it)
    // {
    //      std::cout << *it;
    // }
    // std::cout << std::endl;

    // Leaf iterator
    auto it = trie<std::string>::leaf_iterator{&t_p_2};
    ++it;
    print_trie(it.get_leaf());
    std::cout << " | Label: " << *it << std::endl;
    
    it = t_p_1.begin();
    print_trie(it.get_leaf());
    std::cout << " | Label: " << *it << std::endl;

    it = t_c_2_2.begin();
    print_trie(it.get_leaf());
    std::cout << " | Label: " << *it << std::endl;
}