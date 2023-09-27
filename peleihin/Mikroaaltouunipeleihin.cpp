#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>
#include <ctime> 
#include <cstdlib>

std::string generateRandomString(int length)
{
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const int charsetSize = sizeof(charset) - 1;

    std::string randomString;
    for (int i = 0; i < length; ++i)
    {
        int index = rand() % charsetSize;
        randomString += charset[index];
    }

    return randomString;
}

bool copyToClipboard(const std::string& text)
{
    if (OpenClipboard(nullptr))
    {
        EmptyClipboard();
        HGLOBAL hClipboardData = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
        if (hClipboardData)
        {
            char* pchData = static_cast<char*>(GlobalLock(hClipboardData));
            if (pchData)
            {
                strcpy_s(pchData, text.size() + 1, text.c_str());
                GlobalUnlock(hClipboardData);
                SetClipboardData(CF_TEXT, hClipboardData);
                CloseClipboard();
                return true;
            }
            GlobalFree(hClipboardData);
        }
        CloseClipboard();
    }
    return false;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << " Tama skripti vaatii tiedoston, joka on raahattu tahan." << std::endl;
        Sleep(5000);
        return 1;
    }

    std::cout << " (Mikroaaltouunipeleihin): Valmistellaan tiedostoa..." << std::endl;

    srand(static_cast<unsigned>(time(0)));

    std::string inputFilePath = argv[1];

    std::string randomSoftwareName = generateRandomString(25);
    std::string outputFileName = randomSoftwareName + ".exe";
    std::string outputFilePath = inputFilePath.substr(0, inputFilePath.find_last_of("\\/")) + "\\" + outputFileName;

    Sleep(1000);

    std::ifstream inputFile(inputFilePath, std::ios::binary);
    std::stringstream contentStream;
    contentStream << inputFile.rdbuf();
    inputFile.close();
    std::string content = contentStream.str();

    Sleep(1000);

    srand(static_cast<unsigned>(time(0)));
    std::string newId = generateRandomString(25);
    srand(static_cast<unsigned>(time(0)));
    std::string randomSoftwareNameNew = generateRandomString(45);
    srand(static_cast<unsigned>(time(0)));
    std::string newSDDL = generateRandomString(25);
    srand(static_cast<unsigned>(time(0)));

    int newHitCount = 0;

    size_t pos = 0;
    while ((pos = content.find("Id  : ", pos)) != std::string::npos)
    {
        pos += 7;
        size_t endPos = content.find("\n", pos);
        content.replace(pos, endPos - pos, " " + newId);
    }

    pos = 0;
    while ((pos = content.find("Name: ", pos)) != std::string::npos)
    {
        pos += 6;
        size_t endPos = content.find("\n", pos);
        content.replace(pos, endPos - pos, " " + randomSoftwareNameNew);
    }

    pos = 0;
    while ((pos = content.find("SDDL: ", pos)) != std::string::npos)
    {
        pos += 6;
        size_t endPos = content.find("\n", pos);
        content.replace(pos, endPos - pos, " " + newSDDL);
    }

    pos = 0;
    while ((pos = content.find("HitCount: ", pos)) != std::string::npos)
    {
        pos += 10;
        size_t endPos = content.find("\n", pos);
        content.replace(pos, endPos - pos, " " + std::to_string(newHitCount));
    }

    Sleep(1000);

    std::ofstream outputFile(outputFilePath, std::ios::binary);
    outputFile << content;
    outputFile.close();

    std::cout << " (Mikroaaltouunipeleihin): Valmista tuli!" << std::endl;
    Sleep(1000);

    if (copyToClipboard("\"" +outputFilePath + "\" -run %command% -nointro"))
    {
        std::cout << "\n Tiedoston polku on kopioitu leikepoydalle." << std::endl;
    }
    else
    {
        std::cout << "\n Kopiointi leikepoydalle epaonnistui." << std::endl;

        std::cout << "\n\n(KOPIO TASTA): Startup Asetus: \n\"" << outputFilePath << "\" -run %command% -nointro" << std::endl;
    }

    std::cout << "\n\nPaina mita tahansa nappainta sulkeaksesi taman ikkunan..." << std::endl;

    system("pause > nul");
    return 0;
}
