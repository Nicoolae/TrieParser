#include <iostream>
//#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
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

    trie<std::string> boss;
    t_p_1.set_parent(&boss);
    t_p_2.set_parent(&boss);
    std::string* a = new std::string{"1uno"};
    t_p_1.set_label(a);
    delete a;
     a = new std::string{"2due"};
    t_p_2.set_label(a);
    delete a;

    t_c_1_1.set_parent(&t_p_1);
     a = new std::string{"a"};
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

        boss.add_child(t_p_1);
        boss.add_child(t_p_2);
    }catch(parser_exception e){
        std::cerr << e.what() << '\n';
        return 1;
    }

    // Copy constructor
    // auto it = trie<std::string>::leaf_iterator{&t_c_1_1};
    // ++it;
    // trie<std::string> t_3 = it.get_leaf();

    // Move constructor
    //  trie<std::string> t_3 = foo(t_p_1);
    // std::cout << t_3;
    //  std::cout << std::endl;
    
    // std::cout << boss;
    // // Move assignment operator
    //  //trie<std::string> t_3;
    // // std::cout << "AOO2";
    //  //t_3 = foo(t_p_1);

    // // print_trie(t_p_1);
    //  std::cout << std::endl;
    // // print_trie(t_p_2);
    //    //print_trie(t_3);
    //   // std::cout << boss.max();
    // // std::cout << std::endl;
    //   std::vector<std::string> s = {"2due", "d"};
    // //   //print_trie(boss[s]);
    //  std::cout << boss[s];
//

    // trie<std::string> boss;
    // t_p_1.set_parent(&boss);
    // t_p_2.set_parent(&boss);
    // a = new std::string{"1uno"};
    // t_p_1.set_label(a);
    // delete a;
    //  a = new std::string{"2due"};
    // t_p_2.set_label(a);
    // delete a;
    // boss.add_child(t_p_1);
    // boss.add_child(t_p_2);
    // print_trie(boss);


    // Operator ==
    //std::cout << (t_c_2_1 != t_c_2_2) << std::endl;
    
    // Node iterator
    // for (auto it = trie<std::string>::node_iterator{&t_c_1_1}; it != t_p_1.root(); ++it)
    // {
    //      std::cout << *it;
    // }
    // std::cout << std::endl;

    // Leaf iterator
    //   auto it = trie<std::string>::leaf_iterator{&boss};
    //    ++it;
    //    ++it;
    //    ++it;
    //    for(      auto it = trie<std::string>::leaf_iterator{&boss}; it!=boss.end(); ++it){
    //         try{
    //     // print_trie(t_c_2_2.end().get_leaf());
    //   print_trie(it.get_leaf());
    // }catch(parser_exception e){
    //     std::cerr << e.what() << '\n';
    // }
    //    }

    // try{
    //     // print_trie(t_c_2_2.end().get_leaf());
    //   print_trie(it.get_leaf());
    // }catch(parser_exception e){
    //     std::cerr << e.what() << '\n';
    // }

    //print_trie(it.get_leaf());
    // std::cout << " | Label: " << *it << std::endl;
    
    // it = t_p_1.begin();
    // print_trie(it.get_leaf());
    // std::cout << " | Label: " << *it << std::endl;

    // it = t_c_2_2.begin();
    // print_trie(it.get_leaf());
    // std::cout << " | Label: " << *it << std::endl;
    // int c = '1';
    // std::cout << (int) c << std::endl;




    // Operator >>
    // std::ifstream inputFile("datasets/test_leaf_ok.tr");
    // try{
    //     inputFile >> t_3;
    //     std::cout << t_3;
    // }catch(parser_exception e){
    //     std::cerr << e.what() << '\n';
    // }

    // std::ifstream inputFile("datasets/trie_string.tr");
    // trie<std::string> t_o;
    // try{
    //     inputFile >> t_o;
    //     std::cout << t_o;
    // }catch(parser_exception e){
    //     std::cerr << e.what() << '\n';
    // }
    // std::cout << "MAX LEAF: "<< std::endl;
    // std::cout << t_o.max();

    // Facultative +

    // // std::string t1 = "children = {a children = {b children = {a 1.5 children = {}, c 1.1 children = {}}}, c 0.5 children = {}}";
    // std::string t1 = "children = {a children = {b children = {a 1.5 children = {}, c 1.1 children = {}}}, c 0.5 children = {}, d 1.3 children = {}}";
    // // std::string t2 = "children = {a children = {a 1.5 children = {}, c children = {c 0.2 children = {}}}}";
    // // std::string t2 = "children = {a children = {b children = {a 1 children = {}}, c children = {c 0.2 children = {}}}, c 0.2 children = {}}";
    // std::string t2 = "children = {a children = {b 0.2 children = {}}, c 0.2 children = {}, d children = {e 1.0 children = {}}}";
    // std::stringstream ss1;
    // ss1 << t1;
    // trie<char> t_s_1;
    // //ss1 >> t_s_1;
    // //std::cout << t_s_1;
    // std::cout << std::endl;
    // std::stringstream ss2;
    // ss2 << t2;
    // trie<char> t_s_2;
    // //ss2 >> t_s_2;
    // // std::cout << t_s_2;

    // try{
    //     ss1 >> t_s_1;
    //     ss2 >> t_s_2;
    // }catch(parser_exception e){
    //     std::cerr << e.what() << '\n';
    // }

    //  std::cout << "Result: " << std::endl;
    //  std::cout << t_s_1 + t_s_2;
    // //  std::cout << "Max: " << std::endl;
    // //  std::cout << (t_s_1 + t_s_2).max();
    // std::cout << "\nOp1: " << std::endl;
    // std::cout << t_s_1;
    // std::cout << "\nOp2: " << std::endl;
    // std::cout << t_s_2;

    // Facultative +=
//     // // std::string t1 = "children = {a children = {b children = {a 1.5 children = {}, c 1.1 children = {}}}, c 0.5 children = {}}";
//      std::string t1 = "children = {a children = {b children = {a 1.5 children = {}, c 1.1 children = {}}}, c 0.5 children = {}, d 1.3 children = {}}";
//     // // std::string t2 = "children = {a children = {a 1.5 children = {}, c children = {c 0.2 children = {}}}}";
//     // // std::string t2 = "children = {a children = {b children = {a 1 children = {}}, c children = {c 0.2 children = {}}}, c 0.2 children = {}}";
//      std::string t2 = "children = {a children = {b 0.2 children = {}}, c 0.2 children = {}, d children = {e 1.0 children = {}}}";
//      std::stringstream ss1;
//      ss1 << t1;
//      trie<char> t;
//     // //ss1 >> t_s_1;
//     // //std::cout << t_s_1;
//     // std::cout << std::endl;
//      std::stringstream ss2;
//      ss2 << t2;
//      trie<char> t_s_2;
//     // //ss2 >> t_s_2;
//     // // std::cout << t_s_2;

//     try{
//         ss1 >> t;
//         ss2 >> t_s_2;
//     }catch(parser_exception e){
//         std::cerr << e.what() << '\n';
//     }

//     // std::cout << "Op1 before: " << std::endl;
//     // std::cout << t_s_1;
//     // t_s_1 += t_s_2;
//     // std::cout << "\nOp1 after: " << std::endl;
//      std::cout << t;
//     // std::cout << "\nOp2: " << std::endl;
//     // std::cout << t_s_2;
//     // std::cout << "Max op1 after: " << std::endl;
//     // std::cout << t_s_1.max();

//     for (auto leaf_it = t.begin(); leaf_it != t.end(); ++leaf_it) {
// 	trie<char>::node_iterator node_it = leaf_it; // we convert leaf_it into node_it to navigate from leaf to root
// 	std::vector<char> s;
// 	while (node_it != t.root()) {
// 		s.push_back(*node_it);
// 		++node_it;
// 	}
// 	std::reverse(s.begin(), s.end());
// 	for (auto const& x: s) std::cout << x << ' ';
// 	std::cout << '\n';
// }

    // Facultative path_compress()

    // std::string t1 = "children = {4 3.1 children = {}, 5 children = {2 2.9 children = {}, 3 children = {21 children = {7 7 children = {}, 12 5.3 children = {}}}}}";
    std::string t1 = "children = {5 children = {2 children = {7 children = {1 children = {2 children = {7 children = {7 children = {8 children = {9 children = {12 5 children = {}, 13 6 children = {}}}}, 12 5.3 children = {}}}}}}}, 7 3 children = {}}";
    std::stringstream ss1;
    ss1 << t1;
    trie<int> t;
    ss1 >> t;

    std::cout << "Before compression: \n";
    std::cout << t;
    std::cout << "\nAfter compression: \n";
    t.path_compress();
    std::cout << t;
    std::cout << "\nMAX: \n";
    std::cout << t.max();
}

