#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <regex>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <array>
#include <unistd.h>
#include <algorithm>
#include <cctype>

using namespace std;

class EquationProcessor {
private:
    unordered_map<string, string> eq_dict;
    unordered_map<string, vector<pair<string, string>>> name_duplicates; // normalized_name -> vector of (original_name, category)
    unordered_map<string, vector<pair<string, string>>> formula_duplicates; // normalized_formula -> vector of (name, category)
    
    string load_file(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Cannot open file: " + filename);
        }
        
        stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    
    string normalize_string(const string& s) {
        string normalized = s;
        transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);
        // Remove extra whitespace
        string::iterator new_end = unique(normalized.begin(), normalized.end(),
            [](char a, char b) { return isspace(a) && isspace(b); });
        normalized.erase(new_end, normalized.end());
        normalized.erase(0, normalized.find_first_not_of(" \t"));
        normalized.erase(normalized.find_last_not_of(" \t") + 1);
        return normalized;
    }
    
    void load_equation_dict(const string& filename) {
        string content = load_file(filename);
        istringstream iss(content);
        string line;
        string current_category;
        
        while (getline(iss, line)) {
            // Trim whitespace
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);
            
            if (line.empty()) continue;
            
            // Handle category headers: [category] # description
            if (line[0] == '[') {
                size_t end_bracket = line.find(']');
                if (end_bracket != string::npos) {
                    current_category = line.substr(1, end_bracket - 1);
                }
                continue;
            }
            
            // Skip comment lines
            if (line[0] == '#') continue;
            
            // Handle equation definitions
            size_t colon_pos = line.find(':');
            if (colon_pos != string::npos) {
                string name = line.substr(0, colon_pos);
                string equation = line.substr(colon_pos + 1);
                
                // Trim whitespace
                name.erase(0, name.find_first_not_of(" \t"));
                name.erase(name.find_last_not_of(" \t") + 1);
                equation.erase(0, equation.find_first_not_of(" \t"));
                equation.erase(equation.find_last_not_of(" \t") + 1);
                
                // Prepend category if present
                string full_name = current_category.empty() ? name : current_category + "." + name;
                
                // Check for duplicates
                string norm_name = normalize_string(name);
                string norm_equation = normalize_string(equation);
                
                name_duplicates[norm_name].push_back({full_name, current_category});
                formula_duplicates[norm_equation].push_back({full_name, current_category});
                
                eq_dict[full_name] = equation;
            }
        }
        
        // Process duplicates and create "see:" references
        process_duplicates();
    }
    
    void process_duplicates() {
        // Process name duplicates first
        for (auto& pair : name_duplicates) {
            if (pair.second.size() > 1) {
                // Keep the first one as primary, create "see:" references for others
                string primary_name = pair.second[0].first;
                string primary_category = pair.second[0].second;
                
                for (size_t i = 1; i < pair.second.size(); ++i) {
                    string duplicate_name = pair.second[i].first;
                    string see_ref = "see: " + primary_name + " (" + primary_category + ")";
                    eq_dict[duplicate_name] = see_ref;
                }
            }
        }
        
        // Process formula duplicates (but not already handled by name duplicates)
        for (auto& pair : formula_duplicates) {
            if (pair.second.size() > 1) {
                // Check if any of these were already handled as name duplicates
                bool already_handled = false;
                for (const auto& eq_pair : pair.second) {
                    if (eq_dict[eq_pair.first].substr(0, 4) == "see:") {
                        already_handled = true;
                        break;
                    }
                }
                
                if (!already_handled) {
                    // Keep the first one as primary, create "see:" references for others
                    string primary_name = pair.second[0].first;
                    string primary_category = pair.second[0].second;
                    
                    for (size_t i = 1; i < pair.second.size(); ++i) {
                        string duplicate_name = pair.second[i].first;
                        string see_ref = "see: " + primary_name + " (" + primary_category + ")";
                        eq_dict[duplicate_name] = see_ref;
                    }
                }
            }
        }
        
        // Print duplicate report
        print_duplicate_report();
    }
    
    void print_duplicate_report() {
        cout << "=== DUPLICATE REPORT ===\n\n";
        
        // Name duplicates
        for (const auto& pair : name_duplicates) {
            if (pair.second.size() > 1) {
                cout << "Name duplicate: '" << pair.first << "'\n";
                for (const auto& eq_pair : pair.second) {
                    cout << "  - " << eq_pair.first << " (" << eq_pair.second << ")\n";
                }
                cout << "\n";
            }
        }
        
        // Formula duplicates
        for (const auto& pair : formula_duplicates) {
            if (pair.second.size() > 1) {
                // Check if already handled by name duplicates
                bool already_handled = false;
                for (const auto& eq_pair : pair.second) {
                    if (eq_dict[eq_pair.first].substr(0, 4) == "see:") {
                        already_handled = true;
                        break;
                    }
                }
                
                if (!already_handled) {
                    cout << "Formula duplicate:\n";
                    for (const auto& eq_pair : pair.second) {
                        cout << "  - " << eq_pair.first << " (" << eq_pair.second << ")\n";
                    }
                    cout << "\n";
                }
            }
        }
    }
    
    string replace_equations(const string& content) {
        regex pattern(R"(\\eqn\{([^}]+)\})");
        string result;
        string::const_iterator search_start(content.cbegin());
        smatch match;
        
        while (regex_search(search_start, content.cend(), match, pattern)) {
            // Add text before the match
            result += string(search_start, match[0].first);
            
            string key = match[1].str();
            auto it = eq_dict.find(key);
            
            if (it != eq_dict.end()) {
                string equation = it->second;
                
                // Check if this is a "see:" reference
                if (equation.substr(0, 4) == "see:") {
                    result += "\\begin{equation}\n";
                    result += "\\text{" + equation + "}\\hspace{1em}\\href{http://localhost:8000/eqn/" + key + "}{\\underline{lookup}}\n";
                    result += "\\label{eq:" + key + "}\n";
                    result += "\\end{equation}\n";
                } else {
                    // Create equation with lookup link and alternate text for screen readers
                    result += "\\begin{equation}\n";
                    result += equation + "\\hspace{1em}\\href{http://localhost:8000/eqn/" + key + "}{\\underline{lookup}}\n";
                    result += "\\label{eq:" + key + "}\n";
                    result += "\\end{equation}\n";
                    // Add alternate text for screen readers using \pdftooltip
                    result += "\\pdftooltip{" + equation + "}{LaTeX source: " + equation + "}\n";
                }
            } else {
                result += "\\begin{equation}\n";
                result += "\\text{Equation not found: " + key + "}\\hspace{1em}\\href{http://localhost:8000/eqn/" + key + "}{\\underline{lookup}}\n";
                result += "\\label{eq:" + key + "}\n";
                result += "\\end{equation}\n";
            }
            
            search_start = match[0].second;
        }
        
        // Add remaining text
        result += string(search_start, content.cend());
        return result;
    }
    
    void compile_latex(const string& tex_content, const string& filename) {
        // Prepare the complete LaTeX content with hyperref settings in preamble
        string full_content = tex_content;
        // Insert hyperref and accessibility packages after other packages
        size_t pos = full_content.find("\\usepackage{xcolor}");
        if (pos != string::npos) {
            full_content.insert(pos + 20, "\n\\usepackage[colorlinks=true,urlcolor=blue,hidelinks]{hyperref}\n\\usepackage{accessibility}");
        } else {
            // Fallback: add at beginning
            full_content = "\\usepackage[colorlinks=true,urlcolor=blue,hidelinks]{hyperref}\n\\usepackage{accessibility}\n" + full_content;
        }
        
        // Get base name for output files
        string base_name = filename.substr(0, filename.find_last_of('.'));
        
        // Create temp file in /tmp
        string temp_filename = "/tmp/" + base_name + "_temp.tex";
        ofstream temp_file(temp_filename);
        temp_file << full_content;
        temp_file.close();
        
        // Run pdflatex in /tmp directory
        string command = "cd /tmp && pdflatex -interaction=nonstopmode " + base_name + "_temp.tex 2>&1";
        
        unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
        if (!pipe) {
            throw runtime_error("popen() failed!");
        }
        
        array<char, 128> buffer;
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            cout << buffer.data();
        }
        
        // Move PDF to current directory with correct name
        string temp_pdf = "/tmp/" + base_name + "_temp.pdf";
        string final_pdf = base_name + ".pdf";
        rename(temp_pdf.c_str(), final_pdf.c_str());
        
        // Clean up temp files in /tmp
        remove(temp_filename.c_str());
        remove(("/tmp/" + base_name + "_temp.aux").c_str());
        remove(("/tmp/" + base_name + "_temp.log").c_str());
        remove(("/tmp/" + base_name + "_temp.out").c_str());
    }

public:
    void process_file(const string& input_file) {
        try {
            // Load equation dictionary
            load_equation_dict("eqdict.dat");
            cout << "Loaded " << eq_dict.size() << " equations from dictionary\n";
            
            // Load and process LaTeX file
            string content = load_file(input_file);
            string processed_content = replace_equations(content);
            
            // Compile directly
            compile_latex(processed_content, input_file);
            
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
            exit(1);
        }
    }
};

int main(int argc, char* argv[]) {
    string input_file = "mypaper.tex";
    if (argc > 1) {
        input_file = argv[1];
    }
    
    EquationProcessor processor;
    processor.process_file(input_file);
    
    return 0;
} 