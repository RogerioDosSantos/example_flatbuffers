#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <flatbuffers/flatbuffers.h>
#include <flatbuffers/idl.h>
#include "./schema_generated.h"

using flatbuffers::FlatBufferBuilder;
using flatbuffers::Offset;
using flatbuffers::String;
using flatbuffers::Vector;
using flatbuffers::Parser;
using flatbuffers::GenerateText;
using flatbuffers_example::CreateWeapon;
using flatbuffers_example::CreateMonster;
using flatbuffers_example::GetMonster;
using flatbuffers_example::Weapon;
using flatbuffers_example::Monster;
using flatbuffers_example::Vec3;
using flatbuffers_example::Color_Red;
using flatbuffers_example::Equipment_Weapon;
using std::vector;

bool RunBasicExample(const char* schema_file)
{
    printf("Basic Example \n");

    printf("\tSerializing\n");

    FlatBufferBuilder builder(1024);
    Offset<String> weapon_1_name = builder.CreateString("Sword");
    short weapon_1_damage = 3;
    Offset<String> weapon_2_name = builder.CreateString("Axe");
    short weapon_2_damage = 5;

    Offset<Weapon> sword = CreateWeapon(builder, weapon_1_name, weapon_1_damage);
    Offset<Weapon> axe = CreateWeapon(builder, weapon_2_name, weapon_2_damage);

    Offset<String> name = builder.CreateString("Orc");
    unsigned char treasure[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    Offset<Vector<unsigned char>> inventory = builder.CreateVector(treasure, 10);

    vector<Offset<Weapon>> stl_weapons;
    stl_weapons.push_back(sword);
    stl_weapons.push_back(axe);
    Offset<Vector<Offset<Weapon>>> weapons = builder.CreateVector(stl_weapons);

    Vec3 points[] = {Vec3(1.0f, 2.0f, 3.0f), Vec3(4.0f, 5.0f, 6.0f)};
    Offset<Vector<const Vec3*>> path = builder.CreateVectorOfStructs(points, 2);

    int hp = 300;
    int nana = 150;
    Offset<Monster> orc = CreateMonster(builder, &points[0], nana, hp, name, inventory, Color_Red, weapons, Equipment_Weapon, axe.Union(), path);
    builder.Finish(orc);

    uint8_t* buffer = builder.GetBufferPointer();
    int size = builder.GetSize();

    printf("\t\tBuffer Size: %d ; Buffer: 0x%X\n", size, *buffer);

    printf("\tParsing To JSON\n");
    std::string schema;
    if (!flatbuffers::LoadFile(schema_file, false, &schema))
    {
        printf("\t\tCould not load the schema file\n");
        return false;
    }

    printf("\t\tSchema: %s\n", schema.c_str());

    // const char* include_directories[] = {"/mnt/c/Users/roger/git/roger/examples/cpp/flatbuffers/src", nullptr};
    Parser parser;
    if (!parser.Parse(schema.c_str()/* , include_directories */))
    {
        printf("\t\tCould not parse the schema file\n");
        return false;
    }

    std::string jsongen;
    if (!GenerateText(parser, buffer, &jsongen))
        printf("\t\tCouldn't serialize parsed data to JSON!\n");
    else
        printf("\t\tJSON: %s", jsongen.c_str());

    printf("\tUnserializing\n");
    const Monster* monster = GetMonster(buffer);
    int r_hp = monster->hp();
    int r_mana = monster->mana();
    std::string r_name = monster->name()->c_str();

    printf("\t\tHP: %d\n\t\tMANA: %d\n\t\tNAME: %s\n", r_hp, r_mana, r_name.c_str());

    return true;
}

int main(int argc, char const* argv[])
{
    std::string arguments[3];
    switch (argc)
    {
    case 4:
        arguments[2] = argv[3];
    case 3:
        arguments[1] = argv[2];
    case 2:
        arguments[0] = argv[1];
        break;
    default:
        break;
    }

    if (!arguments[0].compare("basic_example"))
        RunBasicExample(arguments[1].c_str());
    else
        std::cout << "Invalid Command (" << argc << "):\n\t" << arguments[0] << "\nOptions:\n"
                  << "\tbasic_example <schema_file>\t\t\t\t\t-E.g.: basic_example ./stage/include/schema.fbs\n"
                  << "\n";

    return 0;
}
