#include "SourceFile.h"
#include <ifstream>

SourceFile::SourceFile(
        const std::string_view path_,
        const std::string_view contents_) :
    m_path{path_},
    m_contents{contents_}
{}

SourceFile SourceFile::open(const std::string_view path) {
    std::ifstream ifs (path);
    
    if (!ifs.is_open()) {
        throw SourceFileError {.path = path, .message = "unable to open"};
    }

    std::string contents;
    std::getline(ifs, contents, '\0');
    return SourceFile(path, contents);
}

std::string_view SourceFile::path() const {
    return m_path;
}

std::string_view SourceFile::contents() const {
    return m_contents;
}
