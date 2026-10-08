#include <iostream>
#include <time.h>
#include <fstream>
#include <cstdint>
#include <string>
#include <random>
#include <vector>
#include <algorithm> 
#include <iomanip>
#include <map>

struct Date_Time
{
    uint16_t y{};
    uint8_t m{};
    uint8_t d{};
    uint8_t h{};
    uint8_t mm{};
    uint8_t s{};
};

uint32_t get_time();
Date_Time s_to_Date(const uint32_t);
uint32_t Date_to_s(const Date_Time&);
std::string Date_to_string(const Date_Time&);
Date_Time string_to_Date(const std::string&);
uint32_t rand_int(const uint32_t, const uint32_t);

template <size_t N1, size_t N2>
void generate_logs(const uint32_t, const uint16_t, std::ofstream&, const std::string_view(&)[N1],
                   const std::string_view(&)[N2]);

std::vector<std::string> get_logs(std::ifstream&);
bool logs_comparator(const std::string&, const std::string&);
void update_logs(std::ofstream&, const std::vector<std::string>&);

template <size_t N1>
void top3_unlucky(const std::vector<std::string>&, const std::string_view(&)[N1]);

bool fails_comparator(std::pair<uint16_t, uint16_t>, std::pair<uint16_t, uint16_t>);

template <size_t N1>
void top3_active(const std::vector<std::string>&, const std::string_view(&)[N1]);

template <size_t N1>
void activity_by_user(const std::vector<std::string>&, const std::string_view(&)[N1], std::string = {});

std::string get_field(const std::string&, const uint8_t);
void print_bar(const std::string&, uint32_t, uint32_t, uint32_t = 40);

int main() { 
    constexpr uint32_t lower_time_border{1788220800}; // 1788220800 - 2026-09-01 00:00:00 UTC
    constexpr uint16_t log_lines_amount{10000};
    constexpr std::string_view users[] = {"user1", "user2", "user228", "user666", 
                                          "Vitalik", "someuser", "basic_user", "unknown", 
                                          "user2007", "user911", "muhtar", "chechnya"};
    constexpr std::string_view ops[] = {"login", "signin", "connect", "redirect", "download", "upload", 
                                        "some_actions", "magic", "unknown", "share", "deploy", "commit", 
                                        "pull", "push"};
    std::ofstream fout("logfile.txt", std::ios::out | std::ios::trunc);
    generate_logs(lower_time_border, log_lines_amount, fout, users, ops);
    fout.clear();
    fout.seekp(0);
    std::ifstream fin("logfile.txt", std::ios::in);
    std::vector<std::string> logs = get_logs(fin);
    fin.close();
    std::sort(logs.rbegin(), logs.rend(), logs_comparator);
    update_logs(fout, logs);
    fout.close();
    top3_unlucky(logs, users);
    top3_active(logs, users);
    activity_by_user(logs, users);

    return 0;
}

uint32_t get_time() {
    return (uint32_t)time(NULL);
}

Date_Time s_to_Date(const uint32_t total_seconds) {
    Date_Time result{};
    uint32_t day_number = total_seconds / 86400;
    uint32_t seconds_inside_day = total_seconds % 86400;
    result.h = (uint8_t)(seconds_inside_day / 3600);
    seconds_inside_day %= 3600;
    result.mm = (uint8_t)(seconds_inside_day / 60);
    result.s = (uint8_t)(seconds_inside_day % 60);
    uint32_t days_since_0000_03_01{day_number + 719468};
    uint32_t cycle = days_since_0000_03_01 / 146097;
    uint32_t day_of_cycle = days_since_0000_03_01 % 146097;
    uint32_t year_of_cycle = (day_of_cycle - day_of_cycle / 1460 + day_of_cycle / 36524 - day_of_cycle / 146096) / 365;
    uint32_t year = cycle * 400 + year_of_cycle;
    uint32_t day_of_march_year = day_of_cycle - (365 * year_of_cycle + year_of_cycle / 4 - year_of_cycle / 100);
    uint32_t march_month_index = (5 * day_of_march_year + 2) / 153;
    result.d = (uint8_t)(day_of_march_year - (153 * march_month_index + 2) / 5 + 1);
    uint32_t month = march_month_index + 3 - 12 * (march_month_index / 10);
    year += (14 - month) / 12;
    result.m = (uint8_t)(month);
    result.y = (uint16_t)(year);
    return result;    
}

uint32_t Date_to_s(const Date_Time& Date) {
    uint16_t year = Date.y - (Date.m <= 2);
    uint8_t march_month_index = (Date.m > 2) ? (Date.m - 3) : (Date.m + 9);
    uint16_t year_of_cycle = year % 400;
    uint16_t day_of_march_year = (153 * march_month_index + 2) / 5 + Date.d - 1;
    uint32_t day_of_cycle = year_of_cycle * 365 + year_of_cycle / 4 - year_of_cycle / 100 + day_of_march_year;
    uint32_t days_since_0000_03_01 = (year / 400) * 146097 + day_of_cycle;
    uint32_t days_since_epoch = days_since_0000_03_01 - 719468;
    return days_since_epoch * 86400 + Date.h * 3600 + Date.mm * 60 + Date.s;
}

std::string Date_to_string(const Date_Time& Date) {
    std::string str = std::to_string(Date.y) + '-' + (Date.m > 9 ? "" : "0") + std::to_string(Date.m) + 
                      '-' + (Date.d > 9 ? "" : "0") + std::to_string(Date.d) + ' ' + (Date.h > 9 ? "" : "0") +
                      std::to_string(Date.h) + ':' + (Date.mm > 9 ? "" : "0") + std::to_string(Date.mm) + 
                      ':' + (Date.s > 9 ? "" : "0") + std::to_string(Date.s) + " UTC | "; 
    return str;
}

Date_Time string_to_Date(const std::string& str) {
    Date_Time date{};
    date.y = (uint16_t)std::stoi(str.substr(0, 4));
    date.m = (uint8_t)std::stoi(str.substr(5, 2));
    date.d = (uint8_t)std::stoi(str.substr(8, 2));
    date.h = (uint8_t)std::stoi(str.substr(11, 2));
    date.mm = (uint8_t)std::stoi(str.substr(14, 2));
    date.s = (uint8_t)std::stoi(str.substr(17, 2));
    return date;
}

uint32_t rand_int(const uint32_t min, const uint32_t max) {
    std::random_device rd;
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<uint32_t> dstrb(min, max);
    return dstrb(gen);
}

template <size_t users_amount, size_t ops_amount>
void generate_logs(const uint32_t time_low, const uint16_t lines, std::ofstream& fout,
                const std::string_view (&users)[users_amount], const std::string_view (&ops)[ops_amount]) {
    const uint32_t time_now = get_time();
    for(size_t i{}; i < lines; ++i) {
        fout << (Date_to_string(s_to_Date(rand_int(time_low, time_now))) 
                + (std::string)users[rand_int(0, users_amount - 1)]) + " | " + (std::string)ops[rand_int(0, ops_amount - 1)]
                + " | " + std::to_string(rand_int(100, 599)) + "\n";               
    }
    std::cout << "Logs created!\n";
}

std::vector<std::string> get_logs(std::ifstream& fin) {
    std::vector<std::string> logs{};
    std::string str{};
    std::cout << "Reading logs...\n";
    while(getline(fin, str)) {
        logs.push_back(str);
    }
    std::cout << "Logs read!\n";
    return logs;
}

bool logs_comparator(const std::string& a, const std::string& b) {
    const uint32_t a_seconds = Date_to_s(string_to_Date(a));
    const uint32_t b_seconds = Date_to_s(string_to_Date(b));
    return a > b;
}

void update_logs(std::ofstream& fout, const std::vector<std::string>& logs) {
    for(size_t i{}; i < logs.size(); ++i) {
        fout << logs[i] << '\n';
    }
    std::cout << "Logs sorted!\n";
}

template <size_t users_amount>
void top3_unlucky(const std::vector<std::string>& logs, const std::string_view(&users)[users_amount]) {
    std::string temp{};
    std::string user{};
    std::pair<uint16_t, uint16_t> fails[users_amount]{};
    for(size_t i{}; i < users_amount; ++i) {
        fails[i].second = i;
    }
    for(size_t i{}; i < logs.size(); ++i) {
        temp = get_field(logs[i], 3);
        if(atoi(temp.c_str()) >= 400) {
            user = get_field(logs[i], 1);
            for(size_t j{}; j < users_amount; ++j) {
                if(users[j] == user) {
                    ++fails[j].first;
                    continue;
                }
            }
        }
    }
    std::sort(fails, fails + std::size(fails), fails_comparator);
    std::cout << "\nTop 3 unlucky users:(most failed operations)\n" << "1) " 
              << users[fails[0].second] << " - " << fails[0].first <<
              '\n' << "2) " << users[fails[1].second] << " - " << fails[1].first << '\n' << "3) " << 
              users[fails[2].second] << " - " << fails[2].first << '\n';
}

bool fails_comparator(std::pair<uint16_t, uint16_t> a, std::pair<uint16_t, uint16_t> b) {
    return a.first > b.first;
}

template <size_t users_amount>
void top3_active(const std::vector<std::string>& logs, const std::string_view(&users)[users_amount]) {
    std::string user{};
    std::pair<uint16_t, uint16_t> activity[users_amount]{};
    for(size_t i{}; i < users_amount; ++i) {
        activity[i].second = i;
    }
    for(size_t i{}; i < logs.size(); ++i) {
        user = get_field(logs[i], 1);
        for(size_t j{}; j < users_amount; ++j) {
            if(users[j] == user) {
                ++activity[j].first;
                continue;
            }
        }
    }
    std::sort(activity, activity + std::size(activity), fails_comparator);
    std::cout << "\nTop 3 most active users:\n" << "1) " 
              << users[activity[0].second] << " - " << activity[0].first <<
              '\n' << "2) " << users[activity[1].second] << " - " << activity[1].first << '\n' << "3) " << 
              users[activity[2].second] << " - " << activity[2].first << '\n';
}

template <size_t users_amount>
void activity_by_user(const std::vector<std::string>& logs, const std::string_view(&users)[users_amount],
                      std::string username) {
    while(true) {
        if(username.empty()) {
            std::cout << "Enter username to check the activity, or \"exit\" to stop checking users activity: ";
            std::cin >> username;
        }
        if(username == "exit") {
            return;
        }
        bool is_true_user{false};
        for(size_t i{0}; i < users_amount; ++i) {
            if(username == users[i]) {
                is_true_user = true;
                break;
            }
        }
        if(!is_true_user) {
            std::cout << "\nNo such user!\n";
            username.clear();
            continue;
        }
        std::map<std::string, uint64_t> operations_count;
        uint64_t total_operations = 0;
        uint64_t failed_operations = 0;
        for(size_t i{0}; i < logs.size(); ++i) {
            const std::string& log_line = logs[i];
            if(get_field(log_line, 1) != username) {
                continue;
            }
            ++total_operations;
            const std::string operation = get_field(log_line, 2);
            ++operations_count[operation];
            const uint16_t code = atoi(get_field(log_line, 3).c_str());
            if(code >= 400) {
                ++failed_operations;
            }
        }
        uint32_t max_count = 0;
        for(std::map<std::string, uint64_t>::const_iterator item = operations_count.begin();
            item != operations_count.end(); ++item) {
            if(item->second > max_count) {
                max_count = item->second;
            }
        }
        std::cout << "\nActivity for user: " << username << '\n';
        std::cout << "Total operations: " << total_operations << '\n';
        std::cout << "Failed operations: " << failed_operations << "\n\n";
        if(operations_count.empty()) {
            std::cout << "No operations found for this user.\n";
        } else {
        for(std::map<std::string, uint64_t>::const_iterator item = operations_count.begin();
            item != operations_count.end(); ++item) {
            print_bar(item->first, item->second, max_count);
            }
        }
        username.clear();
    }
}

std::string get_field(const std::string& line, const uint8_t field_index) { // 0 - date
    if(field_index > 3) {                                                  // 1 - name
        return {};                                                         // 2 - op
    }                                                                      // 3 - code
    size_t start = 0;
    size_t end = line.find(" | ");
    uint8_t index = 0;
    while(end != std::string::npos) {
        if(index == field_index) {
            return line.substr(start, end - start);
        }
        start = end + 3;
        end = line.find(" | ", start);
        ++index;
    }
    if(index == field_index) {
        return line.substr(start);
    }
    return "";
}

void print_bar(const std::string& label, uint32_t value, uint32_t max_value, uint32_t bar_width) {
    std::cout << std::left << std::setw(15) << label << " | ";
    if(max_value == 0 || value == 0) {
        std::cout << "0\n";
        return;
    }
    uint32_t bar_length = (uint32_t)((value * bar_width + max_value - 1) / max_value);
    for(uint32_t i{0}; i < bar_length; ++i) {
        std::cout << '#';
    }
    std::cout << ' ' << value << '\n';
}