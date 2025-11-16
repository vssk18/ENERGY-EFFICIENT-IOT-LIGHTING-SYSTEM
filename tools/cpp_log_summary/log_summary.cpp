#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct Sample {
    std::string ts;
    double power_w;
};

class LogSummary {
public:
    bool load(const std::string &path) {
        std::ifstream in(path);
        if (!in.is_open()) {
            std::cerr << "Could not open log file: " << path << "\n";
            return false;
        }

        std::string line;
        bool first = true;
        while (std::getline(in, line)) {
            if (first) {
                first = false; // skip header
                continue;
            }
            Sample s;
            if (parse_line(line, s)) {
                samples_.push_back(s);
            }
        }
        return true;
    }

    double total_energy_wh(double minutes_step = 1.0) const {
        double sum_w = 0.0;
        for (const auto &s : samples_) {
            sum_w += s.power_w;
        }
        double hours_step = minutes_step / 60.0;
        return sum_w * hours_step;
    }

    std::size_t count() const { return samples_.size(); }

private:
    static bool parse_line(const std::string &line, Sample &out) {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> cols;

        while (std::getline(ss, token, ',')) {
            cols.push_back(token);
        }
        if (cols.size() < 6) return false;

        out.ts = cols[0];
        try {
            out.power_w = std::stod(cols[5]);
        } catch (...) {
            return false;
        }
        return true;
    }

    std::vector<Sample> samples_;
};

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " data/prototype_log.csv\n";
        return 1;
    }

    LogSummary ls;
    if (!ls.load(argv[1])) {
        return 1;
    }

    double wh = ls.total_energy_wh(1.0);
    std::cout << "Loaded samples : " << ls.count() << "\n";
    std::cout << "Total energy   : " << wh << " Wh (assuming 1-minute step)\n";
    return 0;
}
