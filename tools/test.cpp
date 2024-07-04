#include <iostream>
#include "../src/trie.cpp"
int main(){
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

    try{
        t_1.add_child(t_2);
        t_1.add_child(t_3);
    }catch(parser_exception e){
        std::cerr << e.what() << '\n';
        return 1;
    }

    std::cout << "\nT1(after): \n";
    print_trie(t_1);
}