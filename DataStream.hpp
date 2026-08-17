#ifndef ASTRALINK_DATASTREAM_H
#define ASTRALINK_DATASTREAM_H

#include <string>

class DataStream {
private:
    std::string payload;

public:
    DataStream(std::string data = "") : payload(data) {}

    std::string getPayload() const { return payload; }
    void setPayload(const std::string& data) { payload = data; }
    
    friend std::ostream& operator<<(std::ostream& os, const DataStream& ds);

};
inline std::ostream& operator<<(std::ostream& os, const DataStream& ds) {
    os << ds.payload; 
    return os;
}
#endif // ASTRALINK_DATASTREAM_H