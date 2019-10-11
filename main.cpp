#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <chrono>

/////////// ALGORITHM TO IMPLEMENT ////////////////////

template <typename ForwardIterator, typename OutputIterator, typename Equal, typename Merge>
void merge_adjacent(ForwardIterator first, ForwardIterator last, OutputIterator out, Equal equal, Merge merge)
{
    while ( first != last)
    {
        auto upper = std::find_if_not(first, last, [first,equal](const auto& el )
                {
                    return equal(*first, el); 
                });
        *out++ = std::accumulate(first+1, upper, *first,  merge );
        first = upper;
    }
}


/////////// SALE CLASS /////////////////////////////////

class Sale
{
public:
    Sale(int day, double amount) : day_(day), amount_(amount) {}
    int getDate() const { return day_; }
    double getAmount() const { return amount_; }
    
private:
    int day_;
    double amount_;
};

bool operator==(Sale const& sale1, Sale const& sale2)
{
    return sale1.getDate() == sale2.getDate() && sale1.getAmount() == sale2.getAmount();
}

bool sameDate(Sale const& sale1, Sale const& sale2)
{
    return sale1.getDate() == sale2.getDate();
}

Sale mergeSales(Sale const& sale1, Sale const& sale2)
{
    if (sale1.getDate() != sale2.getDate()) throw "Error: cannot add sales with different dates";
    
    return Sale(sale1.getDate(), sale1.getAmount() + sale2.getAmount());
}

std::ostream& operator<<(std::ostream& os, const Sale& s )
{
    os << "Date: " << s.getDate() << ", Amount : " << s.getAmount() <<'\n';
    return os;
}

////////////////////////////////////////////////////////


/////////// TEST CODE: MAKE IT PASS! ///////////////////

std::vector<Sale> aggregateDailySales(std::vector<Sale> const& sales)
{
    std::vector<Sale> dailySales;
    merge_adjacent(cbegin(sales), cend(sales), std::back_inserter(dailySales), sameDate, mergeSales);
    return dailySales;
}

bool test1()
{
    auto sales = std::vector<Sale>{ {1, 12}, {1, 5}, {1, 6}, {3, 7}, {5, 1}, {5, 1}, {5, 3} };
    auto expected = std::vector<Sale>{ {1, 23}, {3, 7}, {5, 5} };
    return aggregateDailySales(sales) == expected;
}

bool test2()
{
    auto sales = std::vector<Sale>{ {1, 12}, {1, 5}, {1, 6}, {5, 1}, {5, 1}, {5, 3} };
    auto expected = std::vector<Sale>{ {1, 23}, {5, 5} };
    return aggregateDailySales(sales) == expected;
}

bool test3()
{
    auto sales = std::vector<Sale>{ {1, 12}, {3, 5}, {3, 6}, {3, 7} };
    auto expected = std::vector<Sale>{ {1, 12}, {3, 18} };
    return aggregateDailySales(sales) == expected;
}

bool test4()
{
    auto sales = std::vector<Sale>{ {3, 5}, {3, 6}, {3, 7}, {4, 13} };
    auto expected = std::vector<Sale>{ {3, 18}, {4, 13} };
    return aggregateDailySales(sales) == expected;
}

bool test5()
{
    auto sales = std::vector<Sale>{ {3, 5}, {3, 6}, {3, 7} };
    auto expected = std::vector<Sale>{ {3, 18}};
    return aggregateDailySales(sales) == expected;
}

bool test6()
{
    auto sales = std::vector<Sale>{ {3, 5} };
    auto expected = std::vector<Sale>{ {3, 5}};
    return aggregateDailySales(sales) == expected;
}

bool test7()
{
    auto sales = std::vector<Sale>{};
    auto expected = std::vector<Sale>{};
    return aggregateDailySales(sales) == expected;
}

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::cout << "TEST 1: " << (test1() ? "PASSED" : "FAILED") << '\n';
    std::cout << "TEST 2: " << (test2() ? "PASSED" : "FAILED") << '\n';
    std::cout << "TEST 3: " << (test3() ? "PASSED" : "FAILED") << '\n';
    std::cout << "TEST 4: " << (test4() ? "PASSED" : "FAILED") << '\n';
    std::cout << "TEST 5: " << (test5() ? "PASSED" : "FAILED") << '\n';
    std::cout << "TEST 6: " << (test6() ? "PASSED" : "FAILED") << '\n';
    std::cout << "TEST 7: " << (test7() ? "PASSED" : "FAILED") << '\n';
    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms = t2 - t1;
    std::cout << " took " << ms.count() << " ms\n";
}

////////////////////////////////////////////////////////

