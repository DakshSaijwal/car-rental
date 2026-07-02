#ifndef FILE_REPOSITORY_H
#define FILE_REPOSITORY_H

#include <string>
#include <fstream>
#include <unordered_map>
#include <memory>

template <typename Key, typename T>
class FileRepository {
public:
    static void save(const std::string& filename, const std::unordered_map<Key, std::shared_ptr<T>>& data) {
        std::ofstream outFile(filename, std::ios::trunc);
        if (outFile) {
            for (const auto& pair : data) {
                outFile << *(pair.second);
            }
        }
    }

    template <typename Func>
    static void load(const std::string& filename, std::unordered_map<Key, std::shared_ptr<T>>& data, Func updateNextIdFunc) {
        std::ifstream inFile(filename);
        if (inFile) {
            T item;
            while (inFile >> item) {
                data[item.getId()] = std::make_shared<T>(item);
                updateNextIdFunc(item);
            }
        }
    }
};

#endif // FILE_REPOSITORY_H
