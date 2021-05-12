#include <iostream>
#include <vector>
#include <string>
#include <memory>
using namespace std;

template <typename T> class Blob
{
  public:
      using size_type = typename vector<T>::size_type;


      Blob();
      Blob(initializer_list<T> il);

      template <typename It>
      Blob(It f, It e);


      size_type size() const {return data -> size();}
      bool empty() const {return data -> empty();}

      void push_back(T& t) {data -> push_back(t);}
      void push_back(T&& t) {data -> push_back(std::move(t));}
      void pop_back();

      T& back();// const {return data ->  back();}
      T& operator[] (size_type n)
      {
          check(n, "subscript out of range");
          return (*data)[n];
      }

  private:
      void check(size_type n, const string& msg) const ;

      shared_ptr<vector<T>> data;
};

template <typename T>
Blob<T>::Blob() : data(make_shared<vector<T>>) {}

template <typename T>
Blob<T>::Blob(initializer_list<T> il) : data(make_shared<vector<T>>(il)) {}

template <typename T>
template <typename It>
Blob<T>::Blob(It f, It e) : data(make_shared<vector<T>>(f, e)) {}

template <typename T>
void Blob<T>::pop_back()
{
    check(0, "pop_back on empty BLob");
    data -> pop_back();
}

template <typename T>
T& Blob<T>::back()
{
    check(0, "back on empty BLob");
    return data -> back();
}

template <typename T>
void Blob<T>::check(size_type n, const string& msg) const
{
    if(n >= data -> size())
        throw out_of_range(msg);
}

int main()
{
    Blob<int> ex {1, 2, 3};
    return 0;
}
