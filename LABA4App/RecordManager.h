#pragma once

#include "Record.h"

#include <string>
#include <vector>

class RecordManager {
public:
    const std::vector<Record>& GetAll() const;

    bool Add(const Record& record, std::string& error);
    bool Update(int id, const Record& updatedRecord, std::string& error);
    bool Remove(int id, std::string& error);

    void SortById();
    void SortByName();
    void SortByScoreDesc();

    bool LoadFromFile(const std::string& path, std::string& error);
    bool SaveToFile(const std::string& path, std::string& error) const;

    double GetAverageScore() const;
    bool GetBestRecord(Record& best) const;

private:
    std::vector<Record> records_;

    int FindIndexById(int id) const;
    static bool ParseLine(const std::string& line, Record& outRecord);
    static std::string ToCsvLine(const Record& record);
};
