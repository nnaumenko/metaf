/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"

#include <iostream>
#include <fstream>

#ifdef __EMSCRIPTEN__
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;

    void printFile(const std::string & path) {
     	std::ifstream f(path);

        if (!f.is_open()) {
        	std::cout << "Cannot open MEMFS file " << path << std::endl;
        	return;
        }
        std::cout << "MEMFS file " << path << " contents to follow" << std::endl;
        std::cout << "<<<<" << path << ":BEGIN>>>>" << std::endl;
        std::cout << f.rdbuf();
        std::cout << "<<<<" << path << ":END>>>>" << std::endl;
    }

    void printFiles(const std::string & path) {
        std::vector<std::string> subdirs;
    	for (const auto & entry : fs::directory_iterator(path))
            try {
                if (fs::is_directory(entry)) {
                    subdirs.push_back(entry.path());
                }
                if (fs::is_regular_file(entry)) {
                    printFile(entry.path());
                }
            } catch (...) {}
        for (auto p : subdirs) {
            printFiles(p);
        }
    }

#endif

//Since webassembly file access is sandboxed to MEMFS filesystem, it is not 
//possbile to create files (e.g. test xml output) directly. All files created 
//by gtest are saved into MEMFS partition, which is volatile and only exists 
//until page is open in browser.
//
//To get access to files created by gtest, all files found on MEMFS partition  
//are printed to std::cout after gtest run is complete.
//
//This makes it possible to run the tests with emrun (included in Emscripten 
//SDK) and use part of the output where an MEMFS file is printed.

int main(int argc, char ** argv) {
	::testing::InitGoogleTest(&argc, argv);
    const auto testResult = RUN_ALL_TESTS();
    #ifdef __EMSCRIPTEN__
    {
        printFiles("/");
    }
    #endif
    return(testResult);
}
