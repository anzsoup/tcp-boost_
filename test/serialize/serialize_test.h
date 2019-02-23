#include <string>
#include <iostream>

// Following data types are can't be serialized by type casting.
// 1. std::string
// 2. pointers

class inner_class
{
    public:
        inner_class()
        {
            k = 888;
            l = 123.445f;
            //m = "inner_class";
        }

        std::string to_string()
        {
            std::string str;
            str += "[inner_class]\n";
            str += "k = " + std::to_string(k) + "\n";
            str += "l = " + std::to_string(l) + "\n";
            //str += "m = " + m;
            return str;
        }

    public:
        int k;
        float l;
        //std::string m;
};

struct inner_struct
{
    int x[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    //std::string y[4] = {"str1", "str2", "str3", "str4"};
    double z = 30.3;
};

class serialize_test
{
    public:
        serialize_test()
        {
            a = 20;
            b = 55.123f;
            //c = "i love ubuntu.";
        }

        std::string to_string()
        {
            std::string str;
            str += "[serialize_test]\n";
            str += "a = " + std::to_string(a) + "\n";
            str += "b = " + std::to_string(b) + "\n";
            //str += c + "\n";
            str += d.to_string() + "\n";
            str += inner_struct_to_string();
            for (int i = 0; i < 4; ++i)
                str += "\n" + f[i].to_string();
            return str;
        }

        std::string inner_struct_to_string()
        {
            std::string str;
            str += "[inner_struct]\n";
            str += "x =";
            for (int i = 0; i < 10; ++i)
                str += " " + std::to_string(e.x[i]);
            str += "\ny =";
            //for (int j = 0; j < 4; ++j)
                //str += " " + e.y[j];
            str += "\nz = " + std::to_string(e.z);
            return str;
        }

    public:
        int a;
        float b;
        //std::string c;
        inner_class d;
        inner_struct e;
        inner_class f[4];
};