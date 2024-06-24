#include "mainwindow.h"
#include <QApplication>
#include <QDir>
using namespace std;

struct Items {
    QString Name;
    QString Mydate;
    QString Section;
    int Count;

    Items(const QString& name, const QString& mydate, const QString& section, int count) : Name(name), Mydate(mydate), Section(section), Count(count){}
    Items() : Name(""), Mydate(""), Section(""), Count(0){}

};

QDebug& operator << (QDebug& debug, const Items& items) {
    return debug << items.Name << " " << items.Mydate << " " << items.Section << items.Count << "\n";
}

struct Price {
    QString Name;
    QString Mydate;
    double pr;

    Price(const QString& name, const QString& mydate, double price) : Name(name), Mydate(mydate), pr(price){}
    Price() : Name(""), Mydate(""), pr(0.0){}
};

QDebug& operator << (QDebug& debug, const Price& price) {
    return debug << price.Name << " " << price.Mydate << " " << price.pr << "\n";
}

struct Med {
    QString Name;
    QString Mydate;
    QString Section;
    double pr;
    int Count;

    Med(const QString& name, const QString& mydate, const QString& section, double price, int count) : Name(name), Mydate(mydate), Section(section), pr(price), Count(count){}
    Med() : Name(""), Mydate(""), Section(""), pr(0.0), Count(0){}
};

QDebug& operator << (QDebug& debug, const Med& med) {
    return debug << med.Name << " " << med.Mydate << " " << med.Section << " " << med.pr << " " << med.Count << "\n";
}

template <class T>
class MED {
private:
    T* M;
    int Count;
public:
    MED <T>() : M(nullptr), Count(0) {}
    MED <T>(const MED& r): Count(r.Count) {
        M = new T[Count];
        for (int i = 0; i < Count; i++) {
            M[i] = r.M[i];
        }
    }
    MED<T>(int count) : Count(count) {
        M = new T[count];
    }

    ~MED<T>() {
        delete [] M;
    }

    T& operator[] (int index) {
        return M[index];
    }

    bool contains(T elem) {
        for (int i = 0; i < this -> Count; i++) {
            if (this -> M[i] == elem) {
                return true;
            }
        }
        return false;
    }

    void add_items(const QString& name, const QString& mydate, const QString& section, int count) {
        if (M == nullptr) {
            M = new Items[1];
            M[0] = Items(name, mydate, section, count);
        }
        T* newdata = new Items[Count + 1];
        for (int i = 0; i < Count; i++) {
            newdata[i] = M[i];
        }
        delete[] M;
        M = newdata;
        M[Count++] = Items(name, mydate, section, count);
    }

    void add_price(const QString& name, const QString& mydate, double price) {
        if (M == nullptr) {
            M = new Price[1];
            M[0] = Price(name, mydate, price);
        }
        T* newdata = new Price[Count + 1];
        for (int i = 0; i < Count; i++) {
            newdata[i] = M[i];
        }
        delete[] M;
        M = newdata;
        M[Count++] = Price(name, mydate, price);
    }

    void add_med(const QString& name, const QString& mydate, const QString& section, double price, int count) {
        if (M == nullptr) {
            M = new Med[1];
            M[0] = Med(name, mydate, section, price, count);
        }
        T* newdata = new Med[Count + 1];
        for (int i = 0; i < Count; i++) {
            newdata[i] = M[i];
        }
        delete[] M;
        M = newdata;
        M[Count++] = Med(name, mydate, section, price, count);
    }


    MED <T> &operator = (const MED<T>& other) {
        if (this == &other) {
            return *this;
        }
        if (M != nullptr) {
            delete [] M;
        }
        Count = other.Count;
        M = new T[Count];
        for (int i = 0; i < Count; i++) {
            M[i] = other.M[i];
        }
        return *this;
    }

    friend QTextStream& operator << (QTextStream& out, const MED& med) {
        for (int i = 0; i < med.Count; i++) {
            out << med.M[i];
        }
        out << "\n";
        return out;
    }

    int GetCount() {
        return this -> Count;
    }

    T* GetM() {
        return M;
    }

    int resize(int size) {
        this -> Count -= size;
        return Count;
    }
};


inline bool operator == (const Items& a1, const Items& a2) {
    return (a1.Name == a2.Name && a1.Mydate == a2.Mydate && a1.Section == a2.Section && a1.Count == a2.Count);
}

inline bool operator == (const Price& a1, const Price& a2) {
    return (a1.Name == a2.Name && a1.Mydate == a2.Mydate && a1.pr == a2.pr);
}


QTextStream& operator << (QTextStream& out, const Items& items) {
    return out << items.Name << " " << items.Mydate << " " << items.Section << " " << items.Count << "\n";
}

QTextStream& operator << (QTextStream& out, const Price& price) {
    return out << price.Name << " " << price.Mydate << " " << price.pr << "\n";
}

QTextStream& operator << (QTextStream& out, const Med& med) {
    return out << med.Name << " " << med.Mydate << " " << med.Section << " " << med.pr << " " << med.Count << "\n";
}

QTextStream& operator >> (QTextStream& in, Items& items) {
    in >> items.Name >> " " >> items.Mydate >> " " >> items.Section >> " " >> items.Count;
    return in;
}

QTextStream& operator >> (QTextStream& in, Price& price) {
    in >> price.Name >> " " >> price.Mydate >> " " >> price.pr << "\n";
    return in;
}

QTextStream& operator >> (QTextStream& in, Med& med) {
    in >> med.Name >> " " >> med.Mydate >> " " >> med.Section >> " " >> med.pr >> med.Count >> "\n";
    return in;
}

MED<Med> Merge(MED<Items> r, MED<Price> s) {
    int size_1 = r.GetCount();
    int size_2 = s.GetCount();
    MED<Med> k(size_1 + size_2);
    int j;
    for (int i = 0; i < size_1; i++) {
        k.GetM()[i].Name = r.GetM()[i].Name;
        k.GetM()[i].Mydate = r.GetM()[i].Mydate;
        k.GetM()[i].Section = r.GetM()[i].Section;
        k.GetM()[i].Count = r.GetM()[i].Count;
        for (j = 0; j < size_2; j++) {
            if (k.GetM()[i].Name == s.GetM()[j].Name) {
                k.GetM()[i].pr = s.GetM()[j].pr;
                k.resize(1);
                if (k.GetM()[i].Mydate.toInt() < s.GetM()[j].Mydate.toInt()) {
                    k.GetM()[i].Mydate = s.GetM()[j].Mydate;
                }
            }
        }
    }

    int count = 0;
    for (int j = 0; j < size_2; j++) {
        bool check = true;
        for (int i = 0; i < k.GetCount(); i++) {
            if (k.GetM()[i].Name == s.GetM()[j].Name) {
                check = false;
            }
        }
        if (check == true) {
            k.GetM()[k.GetCount() + count - 1].Name = s.GetM()[j].Name;
            k.GetM()[k.GetCount() + count - 1].Mydate = s.GetM()[j].Mydate;
            k.GetM()[k.GetCount() + count - 1].Section = "Unknown";
            k.GetM()[k.GetCount() + count - 1].Count = 0;
            k.GetM()[k.GetCount() + count - 1].pr = s.GetM()[j].pr;
            count++;
        }
    }
    return k;
}

void quicksort(MED <Med>& obj, int left, int right) {
    if (left < right) {
        int i = left;
        int j = right;
        int pivot = (left + right) / 2;
        while (i <= j) {
            while (obj.GetM()[i].Name > obj.GetM()[pivot].Name) {
                i++;
            }
            while (obj.GetM()[j].Name < obj.GetM()[pivot].Name) {
                j--;
            }
            if (i <= j) {
                swap(obj.GetM()[i], obj.GetM()[j]);
                i++;
                j--;
            }
        }
       quicksort(obj, left, j);
       quicksort(obj, i, right);
    }
}

int main(int argc, char *argv[])
{
    MED <Items> M1;
    MED <Price> K1;

    QString filepath = QDir::currentPath();
    QFile file_1(filepath + "/F1.txt");
    QFile file_2(filepath + "/F2.txt");
    QFile tfile_1(filepath + "/Ob1.txt");
    QFile tfile_2(filepath + "/Ob2.txt");
    QFile tfile_3(filepath + "/ROb.txt");

    QTextStream cout(stdout);
    Items items;
    if (file_1.open(QIODevice::ReadOnly) && tfile_1.open(QIODevice::WriteOnly)) {
        QTextStream in1(&file_1);
        QTextStream out1(&tfile_1);
        while (!file_1.atEnd()) {
            QString line1 = file_1.readLine();
            QStringList parts = line1.split(" ");
            items.Name = parts[0];
            items.Mydate = parts[1];
            items.Section = parts[2];
            items.Count = parts[3].toInt();

            if (!M1.contains(items)) {
                M1.add_items(items.Name, items.Mydate, items.Section, items.Count);
            }

            items.Name = "";
            items.Mydate = "";
            items.Section = "";
            items.Count = 0;
        }
        out1 << M1;
    }
    cout << M1;

    Price price;
    if (file_2.open(QIODevice::ReadOnly) && tfile_2.open(QIODevice::WriteOnly)) {
        QTextStream in2(&file_2);
        QTextStream out2(&tfile_2);
        while (!file_2.atEnd()) {
            QString line2 = file_2.readLine();
            QStringList parts_2 = line2.split(" ");
            price.Name = parts_2[0];
            price.Mydate = parts_2[1];
            price.pr = parts_2[2].toDouble();

            if (!K1.contains(price)) {
                K1.add_price(price.Name, price.Mydate, price.pr);
            }

            price.Name = "";
            price.Mydate = "";
            price.pr = 0.0;
        }
        out2 << K1;
    }
    cout << K1;

    MED<Med> S1 = Merge(M1, K1);
    cout << S1;
    quicksort(S1, 0, S1.GetCount() - 1);
    cout << S1;
    if (tfile_3.open(QIODevice::WriteOnly)) {
        QTextStream out3(&tfile_3);
        out3 << S1;
    }
}
