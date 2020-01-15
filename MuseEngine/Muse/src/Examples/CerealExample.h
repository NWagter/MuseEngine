﻿#pragma once

#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>
#include <cereal/types/vector.hpp>

#include <vector>
#include <memory>
#include <cereal/types/polymorphic.hpp>



struct Base
{
    Base() = default;
    virtual ~Base() = default;

    std::string common;

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(CEREAL_NVP(common));
    }
};

struct A : public Base
{
    A() = default;

    A(int v)
    {
        a = v;
    }

    int a;

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(cereal::make_nvp("Base", cereal::base_class<Base>(this)));
        ar(a);
    }
};

struct B : public Base
{
    B() = default;

    B(std::string text)
    {
        b = text;
    }

    std::string b;

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(cereal::make_nvp("Base", cereal::base_class<Base>(this)));
        ar(b);
    }
};

struct Config
{
    std::vector<std::shared_ptr<Base>> vector;

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(vector);
    }

    void SaveToFile()
    {
        std::ofstream fs;
        std::string path = "assets/scenes/cereal.txt";
        fs.open(path);
        {
            cereal::JSONOutputArchive oarchive(fs);
            oarchive(cereal::make_nvp("Test", *this));
        }
        fs.close();
    }

    void LoadFromFile()
    {
        std::ifstream fs;
        const std::string path = "assets/scenes/cereal.txt";
        fs.open(path);

        if (fs.is_open())
        {
            cereal::JSONInputArchive iarchive(fs);
            iarchive(cereal::make_nvp("Test", *this));
        }
        fs.close();
    }
};

CEREAL_REGISTER_TYPE(A)

CEREAL_REGISTER_TYPE_WITH_NAME(B, "ClassB")

CEREAL_REGISTER_POLYMORPHIC_RELATION(Base, A)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Base, B)

void TestSerializationCereal()
{


    Config op;
    std::shared_ptr<Base> ptr1 = std::make_shared<A>(123);
    std::shared_ptr<Base> ptr2 = std::make_shared<B>("foobar");

    op.vector.push_back(ptr1);
    op.vector.push_back(ptr2);

    op.SaveToFile();


    Config op2;
    op2.LoadFromFile();
}
