#ifndef GETOPT_WINDOWS_H
#define GETOPT_WINDOWS_H

#include <cstring>

char* optarg = nullptr;
int optind = 1;
int optopt = 0;

struct option {
    const char* name;
    int has_arg;
    int* flag;
    int val;
};

#define no_argument 0
#define required_argument 1
#define optional_argument 2

int getopt(int argc, char* const argv[], const char* optstring) {
    static int current = 1;
    if (optind >= argc || argv[optind][0] != '-' || argv[optind][1] == '\0') {
        return -1;  // All arguments is processed
    }

    char option = argv[optind][current++];
    const char* p = std::strchr(optstring, option);
    if (p == nullptr) {
        optopt = option;
        return '?';  // Unknown option
    }

    if (*(p + 1) == ':') {
        // Option with a value requirement
        if (argv[optind][current] != '\0') {
            optarg = argv[optind] + current;
            current = 1;
            optind++;
            return option;
        }
        else if (optind + 1 < argc && argv[optind + 1][0] != '-') {
            optarg = argv[optind + 1];
            current = 1;
            optind += 2;
            return option;
        }
        else {
            optarg = nullptr;
            current = 1;
            optind++;
            return ':';
        }
    }
    else {
        // Option without value
        if (argv[optind][current] == '\0') {
            current = 1;
            optind++;
        }
        return option;
    }
}

int getopt_long(int argc, char* const argv[], const char* optstring, const struct option* longopts, int* longindex) {
    static int current = 1;
    if (optind >= argc || argv[optind][0] != '-' || (argv[optind][1] == '-' && argv[optind][2] == '\0')) {
        return -1;  // All arguments is processed
    }

    if (argv[optind][1] == '-') {
        // Long option
        const char* long_option = argv[optind] + 2;
        const char* equals = std::strchr(long_option, '=');

        if (equals != nullptr) {
            // Option with value (--option=value)
            for (int i = 0; longopts[i].name != nullptr; i++) {
                if (std::strncmp(long_option, longopts[i].name, equals - long_option) == 0) {
                    if (longopts[i].has_arg == required_argument) {
                        optarg = const_cast<char*>(equals + 1);
                        optind++;
                        if (longindex != nullptr) {
                            *longindex = i;
                        }
                        return longopts[i].val;
                    }
                    else if (longopts[i].has_arg == optional_argument) {
                        if (*(equals + 1) != '\0') {
                            optarg = const_cast<char*>(equals + 1);
                            optind++;
                            if (longindex != nullptr) {
                                *longindex = i;
                            }
                            return longopts[i].val;
                        }
                        else if (optind + 1 < argc && argv[optind + 1][0] != '-') {
                            optarg = argv[optind + 1];
                            optind += 2;
                            if (longindex != nullptr) {
                                *longindex = i;
                            }
                            return longopts[i].val;
                        }
                        else {
                            optarg = nullptr;
                            optind++;
                            if (longindex != nullptr) {
                                *longindex = i;
                            }
                            return longopts[i].val;
                        }
                    }
                }
            }

            optopt = long_option[0];
            return '?';  // Unknown option
        }
        else {
            // Option without value (--option)
            for (int i = 0; longopts[i].name != nullptr; i++) {
                if (std::strcmp(long_option, longopts[i].name) == 0) {
                    if (longopts[i].has_arg == required_argument || longopts[i].has_arg == optional_argument) {
                        if (optind + 1 < argc && argv[optind + 1][0] != '-') {
                            optarg = argv[optind + 1];
                            optind += 2;
                            if (longindex != nullptr) {
                                *longindex = i;
                            }
                            return longopts[i].val;
                        }
                        else if (longopts[i].has_arg == required_argument) {
                            optarg = nullptr;
                            optind++;
                            if (longindex != nullptr) {
                                *longindex = i;
                            }
                            return ':';
                        }
                    }
                    else {
                        optarg = nullptr;
                        optind++;
                        if (longindex != nullptr) {
                            *longindex = i;
                        }
                        return longopts[i].val;
                    }
                }
            }

            optopt = long_option[0];
            return '?';  // Unknown option
        }
    }
    else {
        // Short option
        char option = argv[optind][current++];
        const char* p = std::strchr(optstring, option);
        if (p == nullptr) {
            optopt = option;
            return '?';  // Unknown option
        }

        if (*(p + 1) == ':') {
            // Option with a value requirement
            if (argv[optind][current] != '\0') {
                optarg = argv[optind] + current;
                current = 1;
                if (argv[optind][current] == '\0') {
                    optind++;
                }
                return option;
            }
            else if (optind + 1 < argc && argv[optind + 1][0] != '-') {
                optarg = argv[optind + 1];
                current = 1;
                optind += 2;
                return option;
            }
            else {
                optarg = nullptr;
                current = 1;
                optind++;
                return ':';
            }
        }
        else {
            // Option without value
            if (argv[optind][current] == '\0') {
                current = 1;
                optind++;
            }
            return option;
        }
    }
}

#endif  // GETOPT_WINDOWS_H