// xml_utils.cpp
#include "xml_utils.h"

std::string createXML(const std::string& message) {
    return "<message>" + message + "</message>";
}

std::string parseXML(const std::string& xml) {
    size_t start = xml.find("<message>") + 9;
    size_t end = xml.find("</message>");

    if (start != std::string::npos && end != std::string::npos) {
        return xml.substr(start, end - start);
    }

    return "";
}
