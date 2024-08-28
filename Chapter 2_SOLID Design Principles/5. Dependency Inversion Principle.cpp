// 5. Dependency Inversion Principle

// Nói rằng những hệ thống có tính mềm dẻo là:
// - Khi source code dependency của nó chỉ trỏ tới các thành phần trừu tượng (abstraction),
// - Chứ không phải là các thành phần cụ thể (concretion).

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
using namespace std;

// A. Các mô-đun cấp cao không nên phụ thuộc vào các mô-đun cấp thấp.
//    Cả hai đều nên phụ thuộc vào các phép trừu tượng. (abstractions / interfaces)

// B. Các Abstractions không nên phụ thuộc vào details.
//    Details nên phụ thuộc vào các abstractions.

enum class Relationship
{
    parent,
    child,
    sibling
};

struct Person
{
    string name;
};

/**
 * interface
 */
struct RelationshipBrowser
{
    virtual vector<Person> find_all_children_of(const string &name) = 0;
};

struct Relationships : RelationshipBrowser // low-level
{
    vector<tuple<Person, Relationship, Person>> relations;

    void add_parent_and_child(const Person &parent, const Person &child)
    {
        relations.push_back({parent, Relationship::parent, child});
        relations.push_back({child, Relationship::child, parent});
    }

    vector<Person> find_all_children_of(const string &name) override
    {
        vector<Person> result;
        for (auto &&[first, rel, second] : relations)
        {
            if (first.name == name && rel == Relationship::parent)
            {
                result.push_back(second);
            }
        }
        return result;
    }
};

struct Research // high-level
{
    Research(RelationshipBrowser &browser) // khai báo interface, nhưng nạp vào lại là lớp dẫn xuất (vẫn oke do có sử dụng "override")
    {
        for (auto &child : browser.find_all_children_of("John"))
        {
            cout << "John has a child called " << child.name << endl;
        }
    }

    /**
     * Sử dụng hàm sau sẽ vi phạm nguyên tắc
     * Do: bị quá chi tiết, thay vào đó hãy phụ thuộc vào "RelationshipBrowser" Abstractions
     */
    //  Research(const Relationships& relationships)
    //  {
    //    auto& relations = relationships.relations;
    //    for (auto&& [first, rel, second] : relations)
    //    {
    //      if (first.name == "John" && rel == Relationship::parent)
    //      {
    //        cout << "John has a child called " << second.name << endl;
    //      }
    //    }
    //  }
};

int main()
{
    Person parent{"John"};

    Person child1{"Chris"};
    Person child2{"Matt"};

    Relationships relationships;
    relationships.add_parent_and_child(parent, child1);
    relationships.add_parent_and_child(parent, child2);

    Research reseachRelationship(relationships);

    return 0;
}