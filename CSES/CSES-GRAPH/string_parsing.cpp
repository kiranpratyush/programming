#include <iostream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <string>

int main()
{
    std::vector<std::string> stack;
    int lines, queries;
    std::cin >> lines >> queries;
    std::cin.ignore(); // Ignore the newline left in the buffer after numbers input

    std::unordered_map<std::string, std::string> hashmap;
    std::string line;

    while (lines--)
    {
        std::getline(std::cin, line);
        line = line.substr(1, line.size() - 2); // Remove < and >

        std::stringstream ss(line);

        // Closing tag
        if (line[0] == '/')
        {
            if (!stack.empty())
                stack.pop_back(); // Remove last element safely
        }
        else
        {
            std::string tagname, attr_name, attr_value, equal_sign;
            ss >> tagname;

            // Construct full tag path
            std::string full_tagname = stack.empty() ? tagname : stack.back() + "." + tagname;
            stack.push_back(full_tagname);

            // Parse attributes
            while (ss >> attr_name >> equal_sign >> attr_value)
            {
                attr_value = attr_value.substr(1, attr_value.length() - 2); // Remove quotes

                // Build full attribute name
                std::string full_attr_name = full_tagname + "~" + attr_name;
                hashmap[full_attr_name] = attr_value;
            }
        }
    }
    while (queries--)
    {
        std::string queryline;
        std::getline(std::cin, queryline);

        if (hashmap.count(queryline))
            std::cout << hashmap[queryline] << std::endl;
        else
            std::cout << "Not Found!" << std::endl;
    }

    return 0;
}
