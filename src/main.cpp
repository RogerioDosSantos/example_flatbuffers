#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <flatbuffers/flatbuffers.h>
#include <flatbuffers/idl.h>
#include "./message_schema_generated.h"
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
using Buffer::ObjectBuilder;
using Buffer::Object;
using Buffer::Value;
using Buffer::CreateBool;
using Buffer::CreateUint;
using Buffer::CreateInt;
using Buffer::CreateLong;
using Buffer::CreateUlong;
using Buffer::CreateStringDirect;

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
    if (!parser.Parse(schema.c_str() /* , include_directories */))
    {
        printf("\t\tCould not parse the schema file\n");
        return false;
    }

    std::string json;
    if (!GenerateText(parser, buffer, &json))
        printf("\t\tCouldn't serialize parsed data to JSON!\n");
    else
        printf("\t\tJSON: %s", json.c_str());

    printf("\tUnserializing\n");
    const Monster* monster = GetMonster(buffer);
    int r_hp = monster->hp();
    int r_mana = monster->mana();
    std::string r_name = monster->name()->c_str();

    printf("\t\tHP: %d\n\t\tMANA: %d\n\t\tNAME: %s\n", r_hp, r_mana, r_name.c_str());

    return true;
}

std::string GetCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    auto now_t = std::chrono::system_clock::to_time_t(now);
    return std::ctime(&now_t);
}

bool Serialize(FlatBufferBuilder* builder)
{
    // Note: You cannot create anything (e.g.: vectors, tables, objects, etc...) between Builder construction and Finish. For this reazon we created
    // the "flat_" variables as teporary variables for the items that needs to be created.
    Offset<String> flat_key;
    Offset<String> flat_timestamp;
    Offset<String> flat_name;
    Offset<void> flat_value;
    Offset<Vector<Offset<Object>>> flat_objects;
    Buffer::Value flat_value_type = Buffer::Value_NONE;
    std::vector<Offset<Object>> messages;
    for (int message_index = 0; message_index < 10; ++message_index)
    {
        // Message
        std::stringstream string_helper;
        string_helper << "message_id_" << message_index;

        std::string message_id = string_helper.str();
        std::vector<Offset<Object>> objects;
        for (int object_index = 0; object_index < 10; ++object_index)
        {
            // Object
            string_helper << ".object_" << object_index;

            std::string object_name = string_helper.str();
            std::vector<Offset<Object>> states;
            for (int state_index = 0; state_index < 6; ++state_index)
            {
                // State
                switch (state_index)
                {
                case 0:
                    flat_value = CreateBool(*builder, false).Union();
                    flat_value_type = Buffer::Value_Bool;
                    break;
                case 1:
                    flat_value = CreateUint(*builder, 10).Union();
                    flat_value_type = Buffer::Value_Uint;
                    break;
                case 2:
                    flat_value = CreateInt(*builder, -10).Union();
                    flat_value_type = Buffer::Value_Int;
                    break;
                case 3:
                    flat_value = CreateLong(*builder, -1234567890).Union();
                    flat_value_type = Buffer::Value_Long;
                    break;
                case 4:
                    flat_value = CreateUlong(*builder, 1234567890).Union();
                    flat_value_type = Buffer::Value_Ulong;
                    break;
                case 5:
                    string_helper.str("");
                    string_helper.clear();
                    string_helper << "State Value: " << object_index << " ; " << message_index;
                    flat_value = CreateStringDirect(*builder, string_helper.str().c_str()).Union();
                    flat_value_type = Buffer::Value_String;
                    break;
                }

                flat_key = builder->CreateString(GetCurrentTime().c_str());
                ObjectBuilder state(*builder);
                state.add_key_type(Buffer::KeyType_TimeStamp);
                state.add_key(flat_key);
                state.add_value(flat_value);
                state.add_value_type(flat_value_type);
                states.push_back(state.Finish());
            }

            flat_objects = builder->CreateVector(states);
            flat_key = builder->CreateString(object_name.c_str());
            ObjectBuilder object(*builder);
            object.add_key_type(Buffer::KeyType_Name);
            object.add_key(flat_key);
            object.add_objects(flat_objects);
            objects.push_back(object.Finish());
        }

        flat_objects = builder->CreateVector(objects);
        flat_key = builder->CreateString(message_id.c_str());
        flat_timestamp = builder->CreateString(GetCurrentTime().c_str());
        ObjectBuilder message(*builder);
        message.add_key_type(Buffer::KeyType_Name);
        message.add_key(flat_key);
        message.add_timestamp(flat_timestamp);
        message.add_objects(flat_objects);
        messages.push_back(message.Finish());
    }

    flat_objects = builder->CreateVector(messages);
    flat_key = builder->CreateString("root");
    flat_timestamp = builder->CreateString(GetCurrentTime().c_str());
    ObjectBuilder root(*builder);
    root.add_key_type(Buffer::KeyType_Name);
    root.add_key(flat_key);
    root.add_timestamp(flat_timestamp);
    root.add_objects(flat_objects);
    builder->Finish(root.Finish());
    return true;
}

bool RunSerialize()
{
    printf("\tSerializing\n");

    FlatBufferBuilder builder(1024);
    if (!Serialize(&builder))
        return false;

    uint8_t* buffer = builder.GetBufferPointer();
    int size = builder.GetSize();
    printf("\t\tBuffer Size: %d ; Buffer: 0x%X\n", size, *buffer);
    return true;
}

bool SerializeJson(const char* schema_file)
{
    printf("\tParsing To JSON\n");

    std::string schema;
    if (!flatbuffers::LoadFile(schema_file, false, &schema))
    {
        printf("\t\tCould not load the schema file\n");
        return false;
    }

    printf("\t\tSchema: \n%s\n", schema.c_str());

    Parser parser;
    if (!parser.Parse(schema.c_str()))
    {
        printf("\t\tCould not parse the schema file.\n");
        return false;
    }

    FlatBufferBuilder builder(1024);
    if (!Serialize(&builder))
        return false;

    uint8_t* buffer = builder.GetBufferPointer();
    std::string json;
    if (!GenerateText(parser, buffer, &json))
        printf("\t\tCouldn't serialize parsed data to JSON!\n");
    else
        printf("\t\tJSON: \n%s", json.c_str());

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

    if (!arguments[0].compare("serialize"))
        RunSerialize();
    else if (!arguments[0].compare("serialize_json"))
        SerializeJson(arguments[1].c_str());
    else
        std::cout << "Invalid Command (" << argc << "):\n\t" << arguments[0] << "\nOptions:\n"
                  << "\t\tseralize\t\t\t\t\t-E.g.: serialize\n"
                  << "\tserialize_json <schema_file>\t\t\t\t\t-E.g.: serialize_json ./stage/include/message_schema.fbs\n"
                  << "\n";

    return 0;
}
