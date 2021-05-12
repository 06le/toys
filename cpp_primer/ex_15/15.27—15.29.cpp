#include <iostream>
#include <vector>
#include <memory>
using namespace std;

class Quote
{
public:
    Quote() {cout << "QuoteĬ�Ϲ��캯��" << '\n';}
    Quote(const string& s, double p) : bookNO(s), price(p)
        {cout << "Quote���캯��" << '\n';}
    Quote(const Quote& q);
    Quote(Quote&& q) noexcept;
    Quote& operator=(const Quote& q);
    Quote& operator=(Quote&& q) noexcept;
    virtual ~Quote() {cout << "Quote��������" << '\n';}

    string isbn() const {return bookNO;}
    virtual double net_price(size_t n) const {return n * price;}
    virtual void debug() const {cout << "bookNO: " << bookNO << " "
        << "price: " << price << '\n';}

protected:
    string bookNO;
    double price = 0;
};

class bulk_quote : public Quote
{
public:
    bulk_quote() {cout << "bulk_quoteĬ�Ϲ��캯��" << '\n';}
    bulk_quote(const string& s, double p, size_t m, double d) :
            Quote(s, p), min_qty(m), discount(d) {cout << "bulk_quote���캯��" << '\n';}

    bulk_quote(const bulk_quote& q);
    bulk_quote(bulk_quote&& q) noexcept;
    bulk_quote& operator=(const bulk_quote& q);
    bulk_quote& operator=(bulk_quote&& q) noexcept;
    ~bulk_quote() override {cout << "bulk_quote��������" << '\n';}

    double net_price(size_t n) const override;
    void debug() const override;

protected:
    size_t min_qty = 0;
    double discount = 1; //��С����ʾ�ۿ�
};


Quote::Quote(const Quote& q) : bookNO(q.bookNO), price(q.price)
{
    cout << "Quote�������캯��" << '\n';
}

Quote::Quote(Quote&& q) noexcept : bookNO(std::move(q.bookNO)), price(std::move(q.price))
{
    q.bookNO = " ";
    q.price = 0;
    cout << "Quote�ƶ����캯��" << '\n';
}

Quote& Quote::operator=(const Quote& q)
{
    bookNO = q.bookNO;
    price = q.price;
    cout << "Quote������ֵ�����" << '\n';
    return *this;
}

Quote& Quote::operator=(Quote&& q) noexcept
{

    bookNO = std::move(q.bookNO);
    price = std::move(q.price);
    return *this;
    cout << "Quote�ƶ���ֵ�����" << '\n';
}

bulk_quote::bulk_quote(const bulk_quote& q) : Quote(q), min_qty(q.min_qty), discount(q.discount)
{
    cout << "bulk_quote�������캯��" << '\n';
}

bulk_quote::bulk_quote(bulk_quote&& q) noexcept : Quote(q), min_qty(std::move(q.min_qty)), discount(std::move(q.discount))
{
    cout << "bulk_quote�ƶ����캯��" << '\n';
}

bulk_quote& bulk_quote::operator=(const bulk_quote& q)
{
    cout << "bulk_quote������ֵ�����" << '\n';
    Quote::operator=(q);
    min_qty = q.min_qty;
    discount = q.discount;
    return *this;
}

bulk_quote& bulk_quote::operator=(bulk_quote&& q) noexcept
{
    cout << "bulk_quote�ƶ���ֵ�����" << '\n';
    Quote::operator=(q);
    min_qty = std::move(q.min_qty);
    discount = std::move(q.discount);
    return *this;
}


double print_total(const Quote& q, size_t n)
{
    double ret = q.net_price(n);
    cout << q.isbn() << "�ܹ�" << ret << "Ԫ" << '\n';
    return ret;
}

double bulk_quote::net_price(size_t n) const
{
    if(n >= min_qty)
        return n * discount * price;
    else
        return n * price;
}

void bulk_quote::debug() const
{
    cout << "bookNO: " << bookNO << " " << "price: " << price
         << "min_qty: " << min_qty << "discount: " << discount
         << '\n';
}


int main()
{
    vector<shared_ptr<Quote>> v1;
    v1.push_back(make_shared<bulk_quote>("book1", 5, 10, 0.5)); //�鱾���۸񣬴�����͹��������ۿ�
    v1.push_back(make_shared<bulk_quote>("book2", 10, 10, 0.5));
    double sum1 = 0;
    for(auto p : v1)
    {
        sum1 += p -> net_price(20);
    }
    cout << "bulk_quote�ܼ۸� " << sum1 << '\n';


//15.29 ��һ�£���ΪQuote��û���ۿۡ�
    vector<shared_ptr<Quote>> v2;
    v2.push_back(make_shared<Quote>("book1", 5)); //�鱾���۸񣬴�����͹��������ۿ�
    v2.push_back(make_shared<Quote>("book2", 10));
    double sum2 = 0;
    for(auto p : v2)
    {
        sum2 += p -> net_price(20);
    }
    cout << "Quote�ܼ۸� " << sum2 << '\n';
    return 0;
}
