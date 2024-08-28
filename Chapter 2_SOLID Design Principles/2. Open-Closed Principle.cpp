// 2. Open-Closed Principle

// open for extension, closed for modification
// open với mở rộng, closed với việc sửa đổi

#include <string>
#include <vector>
#include <iostream>
using namespace std;

enum class Color
{
    red,
    green,
    blue
};
enum class Size
{
    small,
    medium,
    large
};

struct Product
{
    string name;
    Color color;
    Size size;
};

/**
 * Specification : Interface
 */
template <typename T>
struct Specification
{
    virtual ~Specification() = default;
    virtual bool is_satisfied(T *item) const = 0;

    // new: Phá vỡ nguyên lý open - closed nếu thêm phần này
    /*AndSpecification<T> operator&&(Specification<T>&& other)
    {
      return AndSpecification<T>(*this, other);
    }*/
};

struct ColorSpecification : Specification<Product>
{
    Color color;

    ColorSpecification(Color color) : color(color) {}

    bool is_satisfied(Product *item) const override
    {
        return item->color == color;
    }
};

struct SizeSpecification : Specification<Product>
{
    Size size;

    explicit SizeSpecification(const Size size)
        : size{size}
    {
    }

    bool is_satisfied(Product *item) const override
    {
        return item->size == size;
    }
};

template <typename T>
struct AndSpecification : Specification<T>
{
    const Specification<T> &first;
    const Specification<T> &second;

    AndSpecification(const Specification<T> &first, const Specification<T> &second)
        : first(first), second(second) {}

    bool is_satisfied(T *item) const override
    {
        return first.is_satisfied(item) && second.is_satisfied(item);
    }
};
template <typename T>
AndSpecification<T> operator&&(const Specification<T> &first, const Specification<T> &second)
{
    return {first, second};
}

/**
 * Filter : Interface
 */
template <typename T>
struct Filter
{
    virtual vector<T *> filter(vector<T *> items,
                               Specification<T> &spec) = 0;
};

struct BetterFilter : Filter<Product>
{
    vector<Product *> filter(vector<Product *> items,
                             Specification<Product> &spec) override
    {
        vector<Product *> result;
        for (auto &p : items)
            if (spec.is_satisfied(p))
                result.push_back(p);
        return result;
    }
};

int main()
{
    Product apple{"Apple", Color::green, Size::small};
    Product tree{"Tree", Color::green, Size::large};
    Product house{"House", Color::blue, Size::large};

    const vector<Product *> all{&apple, &tree, &house};

    BetterFilter bf;

    ColorSpecification green(Color::green);
    SizeSpecification large(Size::large);

    // Tìm ra Color Specification
    auto green_things = bf.filter(all, green);
    for (auto &x : green_things)
        cout << x->name << " is green\n";

    // Tìm ra Color Specification và Size Specification
    AndSpecification<Product> green_and_large(green, large);

    // cách 1:
    // auto big_green_things = bf.filter(all, green_and_large);

    // cách 2:
    // use the operator instead (same for || etc.)
    auto spec = green && large;
    for (auto &x : bf.filter(all, spec))
        cout << x->name << " is green and large\n";

    // cách 3: nhìn có vẻ đúng nhưng nó không hoạt động
    // warning: the following will compile but will NOT work
    // auto spec2 = SizeSpecification{Size::large} &&
    //              ColorSpecification{Color::blue};

    return 0;
}