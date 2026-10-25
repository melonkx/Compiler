#pragma once
#include <vector>
#include <span>
#include <optional>
#include <functional>

#include "AET.h"

class DiagnosticsEngine {
public:
    using Hook = std::function<void(const Diagnostic&)>;
    void report(AET error);
    void finalize();
    void clear();

 private:
    std::vector<AET> m_errors;
};


