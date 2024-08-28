// 4. Interface Segregation Principle
//  Thay vì dùng 1 interface lớn, người lập trình nên tách thành nhiều interface nhỏ, với nhiều mục đích cụ thể.

#include <vector>

struct Document;

// struct IMachine
//{
//   virtual void print(Document& doc) = 0;
//   virtual void fax(Document& doc) = 0;
//   virtual void scan(Document& doc) = 0;
// };
//
// struct MFP : IMachine
//{
//   void print(Document& doc) override;
//   void fax(Document& doc) override;
//   void scan(Document& doc) override;
// };

/**
 * Nếu ta dùng cách trên, vi phạm "Interface Segregation Principle" do:
// 1. Biên dịch lại (Recompile)
// 2. Client không cần điều này
// 3. Buộc người triển khai triển khai quá nhiều
 */

struct IPrinter
{
    virtual void print(Document &doc) = 0;
};

struct IScanner
{
    virtual void scan(Document &doc) = 0;
};

struct Printer : IPrinter
{
    void print(Document &doc) override;
};

struct Scanner : IScanner
{
    void scan(Document &doc) override;
};

struct IMachine : IPrinter, IScanner
{
};

struct Machine : IMachine
{
    IPrinter &printer; // ở đây có sai không (ref)? -> không, do ta khởi tạo nó ở contructor cũng được ": printer{printer}"
    IScanner &scanner; // không nhất thiết phải đặt giá trị tại đây luôn, vì chắc chắn phải gọi hàm tạo rồi mà

    Machine(IPrinter &printer, IScanner &scanner)
        : printer{printer},
          scanner{scanner}
    {
    }

    void print(Document &doc) override
    {
        printer.print(doc);
    }
    void scan(Document &doc) override;
};

// IPrinter --> Printer
// everything --> Machine

int main()
{
    return 0;
}