// 3. Liskov Substitution Principle

// Bất cứ instance nào của lớp cha cũng có thể được thay thế bởi instance của lớp con của nó
// mà không làm thay đổi tính đúng đắn của chương trình

#include <iostream>

class Rectangle
{
protected:
  int width, height;

public:
  Rectangle(const int width, const int height)
      : width{width}, height{height} {}

  int get_width() const { return width; } // const đứng sau tên hàm: nghĩa là hàm này CHỈ ĐỌC, không thể thay đổi
  int get_height() const { return height; }
  int area() const { return width * height; }

  virtual void set_width(const int width) { this->width = width; }
  virtual void set_height(const int height) { this->height = height; }
};

class Square : public Rectangle
{
public:
  Square(int size) : Rectangle(size, size) {}
  void set_width(const int width) override
  {
    this->width = height = width;
  }
  void set_height(const int height) override
  {
    this->height = width = height;
  }
};

void process(Rectangle &r)
{
  int w = r.get_width();

  r.set_height(10);

  std::cout << "diện tích dự kiến: " << (w * 10)
            << ", thực tế: " << r.area() << std::endl;
}

int main()
{
  Rectangle r{5, 5};
  process(r);

  Square s{5}; // override + virtual -> nhận theo type Square
  process(s);

  getchar();
  return 0;
}