 //
//  main.cpp
//  animeloop-cli
//
//  Created by ShinCurry on 2017/2/9.
//  Copyright © 2017年 ShinCurry. All rights reserved.
//

#include <iostream>
#include <boost/filesystem.hpp>

#include "cxxopts.hpp"
#include "loop_video.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace cxxopts;

int main(int argc, char * argv[]) {

    auto rpath = path(argv[0]).parent_path();
    double min_duration, max_duration;
    
    try {
        cxxopts::Options options("animeloop", "anime loop video generator.");

        string input, output, title;
        
        options.add_options()
        ("h,help", "Show animeloop help")
        ("v,version", "Show animeloop version")
        ("i,input", "Input video file path", value<string>(input))
        ("o,output", "Output video directory path", value<string>(output)->default_value(rpath.string()))
        ("title", "Title name of the source video (default: <filename>)", value<string>(title))
        ("min-duration", "Minimum duration (second) of loop video", value<double>(min_duration)->default_value("0.6"))
        ("max-duration", "Maximum duration (second) of loop video", value<double>(max_duration)->default_value("6.0"))
        ("cover", "Output loop video cover image.")
        ;
        
        options.parse(argc, argv);
        
        if (options.count("version")) {
            cout << "version: " << al::kVersion << endl;
        }
        
        if (options.count("help")) {
            cout << options.help() << endl;
        }
        
        if (options.count("input")) {
            if (title == "") {
                title = path(input).stem().string();
            }

            al::LoopVideo loop_video(input, output);
            loop_video.title = title;
            loop_video.min_duration = min_duration;
            loop_video.max_duration = max_duration;
            
            if (options.count("cover")) {
                loop_video.cover_enabled = true;
            }
            
            loop_video.init();
            loop_video.filter();
            loop_video.print(loop_video.filtered_durations);
            
            loop_video.generate(loop_video.filtered_durations);
        }
        
        
    } catch (const OptionException& e) {
        cout << "error parsing options: " << e.what() << endl;
        exit(1);
    }

    return 0;
}


