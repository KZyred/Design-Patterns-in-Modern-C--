// 1. Single Responsibility Principle

// một class chỉ nên giữ một trách nhiệm (chức năng) duy nhất.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct Journal
{
    string title;
    vector<string> entries;

    explicit Journal(const string &title)
        : title{title}
    {
    }

    void add(const string &entry);

    // persistence is a separate concern
    void save(const string &filename);
};

void Journal::add(const string &entry)
{
    static int count = 1;
    entries.push_back(to_string(count++) + ": " + entry);
}

void Journal::save(const string &filename)
{
    ofstream ofs(filename);
    for (auto &s : entries)
        ofs << s << endl;
}

struct PersistenceManager
{
    static void save(const Journal &j, const string &filename)
    {
        ofstream ofs(filename);
        for (auto &s : j.entries)
            ofs << s << endl;
    }
};

int main()
{
    Journal journal{"Dear Diary"};
    journal.add("I ate a bug");
    journal.add("I cried today");

    // journal.save("diary.txt");   // trái với nguyên lý trách nhiệm duy nhất

    PersistenceManager pm;
    pm.save(journal, "diary.txt");

    return 0;
}