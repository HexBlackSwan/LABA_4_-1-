#include "RecordManager.h"

#include <algorithm>
#include <fstream>
#include <sstream>

const std::vector<Record>& RecordManager::GetAll() const {
    return records_;
}

bool RecordManager::Add(const Record& record, std::string& error) {
    if (record.id <= 0) {
        error = "ID должен быть положительным.";
        return false;
    }
    if (record.name.empty() || record.group.empty()) {
        error = "Имя и группа не должны быть пустыми.";
        return false;
    }
    if (record.score < 0.0 || record.score > 100.0) {
        error = "Оценка должна быть в диапазоне [0; 100].";
        return false;
    }
    if (FindIndexById(record.id) != -1) {
        error = "Запись с таким ID уже существует.";
        return false;
    }

    records_.push_back(record);
    return true;
}

bool RecordManager::Update(int id, const Record& updatedRecord, std::string& error) {
    const int index = FindIndexById(id);
    if (index < 0) {
        error = "Запись для изменения не найдена.";
        return false;
    }

    if (updatedRecord.name.empty() || updatedRecord.group.empty()) {
        error = "Имя и группа не должны быть пустыми.";
        return false;
    }
    if (updatedRecord.score < 0.0 || updatedRecord.score > 100.0) {
        error = "Оценка должна быть в диапазоне [0; 100].";
        return false;
    }

    records_[index] = updatedRecord;
    return true;
}

bool RecordManager::Remove(int id, std::string& error) {
    const int index = FindIndexById(id);
    if (index < 0) {
        error = "Запись для удаления не найдена.";
        return false;
    }

    records_.erase(records_.begin() + index);
    return true;
}

void RecordManager::SortById() {
    std::sort(records_.begin(), records_.end(), [](const Record& a, const Record& b) {
        return a.id < b.id;
    });
}

void RecordManager::SortByName() {
    std::sort(records_.begin(), records_.end(), [](const Record& a, const Record& b) {
        return a.name < b.name;
    });
}

void RecordManager::SortByScoreDesc() {
    std::sort(records_.begin(), records_.end(), [](const Record& a, const Record& b) {
        return a.score > b.score;
    });
}

bool RecordManager::LoadFromFile(const std::string& path, std::string& error) {
    std::ifstream input(path);
    if (!input.is_open()) {
        error = "Не удалось открыть файл для чтения.";
        return false;
    }

    std::vector<Record> loaded;
    std::string line;
    while (std::getline(input, line)) {
        if (line.empty()) {
            continue;
        }

        Record record{};
        if (!ParseLine(line, record)) {
            error = "Файл содержит некорректные данные.";
            return false;
        }
        loaded.push_back(record);
    }

    records_ = loaded;
    return true;
}

bool RecordManager::SaveToFile(const std::string& path, std::string& error) const {
    std::ofstream output(path, std::ios::trunc);
    if (!output.is_open()) {
        error = "Не удалось открыть файл для записи.";
        return false;
    }

    for (const auto& record : records_) {
        output << ToCsvLine(record) << '\n';
    }

    return true;
}

double RecordManager::GetAverageScore() const {
    if (records_.empty()) {
        return 0.0;
    }

    double sum = 0.0;
    for (const auto& record : records_) {
        sum += record.score;
    }

    return sum / records_.size();
}

bool RecordManager::GetBestRecord(Record& best) const {
    if (records_.empty()) {
        return false;
    }

    best = *std::max_element(records_.begin(), records_.end(), [](const Record& a, const Record& b) {
        return a.score < b.score;
    });

    return true;
}

int RecordManager::FindIndexById(int id) const {
    for (size_t i = 0; i < records_.size(); ++i) {
        if (records_[i].id == id) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

bool RecordManager::ParseLine(const std::string& line, Record& outRecord) {
    std::stringstream ss(line);
    std::string idToken;
    std::string nameToken;
    std::string groupToken;
    std::string scoreToken;

    if (!std::getline(ss, idToken, ';') ||
        !std::getline(ss, nameToken, ';') ||
        !std::getline(ss, groupToken, ';') ||
        !std::getline(ss, scoreToken, ';')) {
        return false;
    }

    try {
        outRecord.id = std::stoi(idToken);
        outRecord.name = nameToken;
        outRecord.group = groupToken;
        outRecord.score = std::stod(scoreToken);
    }
    catch (...) {
        return false;
    }

    return outRecord.id > 0 && !outRecord.name.empty() && !outRecord.group.empty() &&
        outRecord.score >= 0.0 && outRecord.score <= 100.0;
}

std::string RecordManager::ToCsvLine(const Record& record) {
    return std::to_string(record.id) + ";" + record.name + ";" + record.group + ";" + std::to_string(record.score);
}
