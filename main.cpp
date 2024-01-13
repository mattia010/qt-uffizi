#include <iostream>
#include <vector>

#include "set.hpp"

void test_add_remove() {
    Set<int> tmp;
    for (int i = 0; i < 100; ++i) {
        tmp.add(i);
    }
    for (int i = 0; i < 90; ++i) {
        tmp.remove(i);
    }
}

bool test_add_int() {
    Set<int> tmp;
    tmp.add(21);
    if (!tmp.contains(21)) {
        return false;
    }
    tmp.add(24);
    if (!tmp.contains(24)) {
        return false;
    }
    if (tmp.contains(5) || tmp.contains(0)) {
        return false;
    }
    for (int i = 0; i < 10; ++i) {
        tmp.add(i);
    }
    for (int i = 0; i < 10; ++i) {
        if (!tmp.contains(i)) {
            return false;
        }
    }
    if (!tmp.contains(21)) {
        return false;
    }
    return true;
}

bool test_add_double() {
    Set<double> tmp;
    tmp.add(21.0);
    if (!tmp.contains(21.0)) {
        return false;
    }
    tmp.add(24.243);
    if (!tmp.contains(24.243)) {
        return false;
    }
    if (tmp.contains(5.12) || tmp.contains(0.001)) {
        return false;
    }
    for (int i = 0; i < 10; ++i) {
        tmp.add(i);
    }
    for (int i = 0; i < 10; ++i) {
        if (!tmp.contains(i)) {
            return false;
        }
    }
    if (!tmp.contains(21.0)) {
        return false;
    }
    return true;
}

bool test_remove_int() {
    Set<int> tmp;
    for (int i = 0; i < 50; ++i) {
        if (tmp.contains(i)) {
            return false;
        }
    }
    for (int i = 0; i < 10; ++i) {
        tmp.add(i);
    }
    for (int i = 0; i < 10; ++i) {
        if (!tmp.contains(i)) {
            return false;
        }
    }
    for (int i = 0; i < 10; i += 2) {
        tmp.remove(i);
    }
    for (int i = 0; i < 10; ++i) {
        if (i % 2 == 0) {
            if (tmp.contains(i)) {
                return false;
            }
        } else {
            if (!tmp.contains(i)) {
                return false;
            }
        }
    }
    return true;
}

bool test_parenthesis_operator() {
    Set<int> tmp;
    for (int i = 0; i < 10; ++i) {
        tmp.add(i);
    }
    for (int i = 0; i < 10; ++i) {
        if (tmp[i] != i) {
            return false;
        }
    }
    try {
        tmp[-1];
        tmp[20];
    } catch (std::out_of_range) {
        return true;
    }
    return true;
}

bool test_iterator() {
    Set<int> tmp;
    for (int i = 0; i < 10; ++i) {
        tmp.add(i);
    }
    if (*(tmp.begin()) != 0) {
        return false;
    }
    if (*++(tmp.begin()) != 1) {
        return false;
    }
    auto begin = tmp.begin();
    while (begin != tmp.end()) {
        ++begin;
    }
    return true;
}

bool test_iterators_constructor() {
    Set<int> tmp;
    for (int i = 0; i < 10; ++i) {
        tmp.add(i);
    }
    Set<int> tmp2(tmp.begin(), tmp.end());
    if (!(tmp == tmp2)) {
        return false;
    }

    std::vector<int> vec;
    for (int i = 0; i < 10; ++i) {
        vec.push_back(i);
    }
    Set<double> tmp3(vec.begin(), vec.end());
    for (int i = 0; i < 10; ++i) {
        if (tmp[i] != static_cast<double>(vec[i])) {
            return false;
        }
    }
    return true;
}

bool test_compare_sets() {
    Set<int> tmp1;
    Set<int> tmp2;
    for (int i = 0; i < 10; ++i) {
        tmp1.add(i);
        tmp2.add(9 - i);
    }
    if (!(tmp1 == tmp2)) {
        return false;
    }

    Set<int> tmp3;
    for (int i = 0; i < 5; ++i) {
        tmp3.add(i);
    }
    Set<int> tmp4(tmp3.begin(), tmp3.end());
    if (!(tmp3 == tmp4)) {
        return false;
    }
    return true;
}

bool test_filter_out() {
    Set<int> tmp;
    Set<int> result;
    for (int i = 0; i < 10; ++i) {
        tmp.add(i);
    }
    for (int i = 0; i < 5; ++i) {
        result.add(i);
    }
    auto lambda = [](int x) {
        if (x < 5) 
            return true; 
        else 
            return false;
    };
    if (result == filter_out(tmp, lambda)) {
        return true;
    }
    return false;
}

bool test_concatenation() {
    Set<int> tmp1;
    tmp1.add(3);
    tmp1.add(7);
    tmp1.add(5);
    Set<int> tmp2;
    tmp2.add(4);
    tmp2.add(7);
    Set<int> tmp3 = tmp1 + tmp2;
    Set<int> tmp4;
    tmp4.add(3);
    tmp4.add(7);
    tmp4.add(5);
    tmp4.add(4);
    if (!(tmp3 == tmp4) || tmp3.contains(2)) {
        return false;
    }
    return true;
}

bool test_intersection() {
    Set<int> tmp1;
    tmp1.add(3);
    tmp1.add(7);
    tmp1.add(5);
    Set<int> tmp2;
    tmp2.add(4);
    tmp2.add(7);
    tmp2.add(3);
    Set<int> tmp3 = tmp1 - tmp2;
    if (!(tmp3.contains(7) && tmp3.contains(3))) {
        return false;
    }
    if (tmp3.contains(5) || tmp3.contains(4)) {
        return false;
    }
    return true;
}

bool test_save() {
    Set<std::string> tmp;
    tmp.add("prova1");
    tmp.add("prova2");
    tmp.add("prova3");
    save(tmp, "prova.txt");
    return true;
}

void test_print_operator() {
    Set<int> tmp;
    for (int i = 0; i < 10; ++i) {
        tmp.add(i);
    }
    tmp.remove(3);
    tmp.remove(7);
    std::cout << "Stampa di prova: " << tmp <<std::endl;
}

int main() {
    std::cout << "1: test ha avuto successo.   0: il test è fallito" << std::endl;
    std::cout << "test_add_int: " << test_add_int() << std::endl;
    std::cout << "test_add_double: " << test_add_double() << std::endl;
    std::cout << "test_remove_int: " << test_remove_int() << std::endl;
    std::cout << "test_parenthesis_operator: " << test_parenthesis_operator() << std::endl;
    std::cout << "test_iterator: " << test_iterator() << std::endl;
    std::cout << "test_iterators_constructor: " << test_iterators_constructor() << std::endl;
    std::cout << "test_compare_sets: " << test_compare_sets() << std::endl;
    std::cout << "test_filter_out: " << test_filter_out() << std::endl;
    std::cout << "test_concatenation: " << test_concatenation() << std::endl;
    std::cout << "test_intersection: " << test_intersection() << std::endl;
    
    test_add_remove(); //per verificare se ci sono memory leak con valgrind

    test_print_operator();

    test_save();
    std::cout << "Salvataggio effettuato. Controlla il file." << std::endl;
}