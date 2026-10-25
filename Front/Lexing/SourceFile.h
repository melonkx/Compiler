#pragma once

#include <string>

struct SourceFileError {
    std::string path;
    std::string message;
};

class SourceFile {
public:
    explicit SourceFile(std::string_view path_, std::string_view contents_);
    [[nodiscard]] static SourceFile open(std::string_view path);
    std::string_view path() const; 
    std::string_view contents() const;

private:
    std::string m_path;
    std::string m_contents;
};
