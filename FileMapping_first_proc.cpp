#include <windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

constexpr auto FINISH = 100;

void ConvertIntToChars(int value, char buff[])
{
    std::string str = std::to_string(value);
    size_t size = str.size();
    for (auto i = 0; i < size; i++)
    {
        buff[i] = str[i];
    }
    buff[size] = '\0';
}
void WorkImitation(int& value)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Sending progress: " << value++ << std::endl;
}
bool CompareValues(int& prev, const int next)
{
    if (prev < next)
    {
        prev = next;
        return true;
    }
    return false;
}
int main()
{
    int nextProgress = 0;
    int prevProgress = 0;
    std::thread th([&]()
        {
            while (nextProgress <= FINISH)
            {
                WorkImitation(nextProgress);
            }
        });
    while (true)
    {
        if (CompareValues(prevProgress, nextProgress))
        {
            if (prevProgress <= FINISH)
            {
                HANDLE  hFileMap;
                PCHAR   pBuffer = nullptr;
                char    cBuffer[12];
                size_t  size = sizeof(cBuffer);
                ConvertIntToChars(prevProgress, cBuffer);
                hFileMap = CreateFileMapping(
                    INVALID_HANDLE_VALUE,
                    NULL,
                    PAGE_READWRITE,
                    0,
                    256,
                    L"Local\\MyFileMap");
                pBuffer = (PCHAR)MapViewOfFile(
                    hFileMap,
                    FILE_MAP_ALL_ACCESS,
                    0,
                    0,
                    256);
                CopyMemory(pBuffer, cBuffer, size);
            }
            else
            {
                break;
            }
        }
    }
    th.join();
    return 0;
}