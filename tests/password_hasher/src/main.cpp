//Copyright(C) 2026 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <iostream>
#include <string>
#include <array>

#include "password_hasher.hpp"

using KalaHeaders::KalaPasswordHasher::HASH_SIZE_BYTES;
using KalaHeaders::KalaPasswordHasher::SALT_SIZE_BYTES;
using KalaHeaders::KalaPasswordHasher::HashPassword;
using KalaHeaders::KalaPasswordHasher::VerifyPassword;
using KalaHeaders::KalaPasswordHasher::BytesToString;

using std::cout;
using std::cin;
using std::string;
using std::string_view;
using std::array;
using std::numeric_limits;
using std::streamsize;
using std::pair;

static bool alive = true;
static string nextStep{};

static pair<string, string> userRawData{}; //username and raw password

struct HashResults
{
    array<u8, HASH_SIZE_BYTES> hashedPasswordBytes{};
    array<u8, SALT_SIZE_BYTES> passwordSaltBytes{};

    string hashedPasswordString{};
    string passwordSaltString{};
};

static HashResults hashResults{};

static void ClearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

static void EnterToConfirm()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

static void ReadCommands();

static void Command_Reset()
{
    ClearScreen();

    userRawData = {};
    hashResults = {};
    nextStep.clear();
}
static void Command_Verify()
{
    ClearScreen();

    pair<array<u8, HASH_SIZE_BYTES>, array<u8, SALT_SIZE_BYTES>> result = 
    { 
        hashResults.hashedPasswordBytes,
        hashResults.passwordSaltBytes
    };

    bool success = (VerifyPassword(
        userRawData.second,
        result).empty());

    cout << "Username: " << userRawData.first << "\n" 
        << "Raw password: " << userRawData.second << "\n"
        << "Hashed password: " << hashResults.hashedPasswordString << "\n"
        << "Password salt: " << hashResults.passwordSaltString << "\n"
        << "Result: " << (success ? "valid" : "invalid") << "\n"
        << "Next step: ";

    nextStep.clear();
    cin >> nextStep;

    ReadCommands();
}
static void Command_Exit()
{
    Command_Reset();
    alive = false;
}
static void Command_Invalid()
{
    ClearScreen();

    userRawData = {};
    hashResults = {};

    cout << "Invalid step! Press Enter to reset.";

    //discard previous newline and wait for a fresh Enter
    EnterToConfirm();
}

static void ReadCommands()
{
    if (nextStep == "reset")
    {
        Command_Reset();
        return;
    }
    else if (nextStep == "verify")
    {
        Command_Verify();
        return;
    }
    else if (nextStep == "exit")
    {
        Command_Exit();
        return;
    }

    Command_Invalid();
}

static void PollInput()
{
    if (userRawData.first.empty())
    {
        ClearScreen();

        cout << "Username: ";
        cin >> userRawData.first;
    }

    if (userRawData.second.empty())
    {
        ClearScreen();

        cout << "Password: ";
        cin >> userRawData.second;
    }

    if (!userRawData.first.empty()
        && !userRawData.second.empty())
    {
        ClearScreen();

        pair<array<u8, HASH_SIZE_BYTES>, array<u8, SALT_SIZE_BYTES>> result{};

        string err = HashPassword(
            userRawData.second,
            result);

        if (!err.empty())
        {
            cout << "Failed to hash password! Reason: " << err << " Press Enter to reset.";

            //discard previous newline and wait for a fresh Enter
            EnterToConfirm();

            return;
        }

        hashResults.hashedPasswordBytes = std::move(result.first);
        hashResults.passwordSaltBytes = std::move(result.second);

        BytesToString(
            hashResults.hashedPasswordBytes, 
            hashResults.hashedPasswordString);
        BytesToString(
            hashResults.passwordSaltBytes, 
            hashResults.passwordSaltString);

        cout << "Username: " << userRawData.first << "\n" 
            << "Raw password: " << userRawData.second << "\n"
            << "Hashed password: " << hashResults.hashedPasswordString << "\n"
            << "Password salt: " << hashResults.passwordSaltString << "\n"
            << "Next step: ";

        cin >> nextStep;

        ReadCommands();
    }
}

int main()
{
    while (alive) PollInput();
    return 0;
}
