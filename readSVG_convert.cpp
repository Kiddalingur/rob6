// read libs
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>


unsigned int microseconds = 500;


using namespace std;

bool is_it_style = false;
int counter = 0;
const int m = 10;
string svg_width[m];
string svg_height[m];

string circle_cx[m];
string circle_cy[m];
string circle_r[m];
string circle_style[m];
string circle_stroke[m];
string circle_strokeWidth[m];
string circle_fill[m];

string rect_x[m];
string rect_y[m];
string rect_width[m];
string rect_height[m];
string rect_style[m];
string rect_stroke[m];
string rect_strokeWidth[m];
string rect_fill[m];

string ellipse_x[m];
string ellipse_y[m];
string ellipse_rx[m];
string ellipse_ry[m];
string ellipse_style[m];
string ellipse_stroke[m];
string ellipse_strokeWidth[m];
string ellipse_fill[m];

string line_x1[m];
string line_y1[m];
string line_x2[m];
string line_y2[m];
string line_strokeWidth[m];
string line_stroke[m];

const int n = 250;         
double segments[n][15];
double TSP_segments[n * 4][15];
double temp_pre_pattern[4];
double temp_post_pattern[4];
double pre_pattern[n][7];
double post_pattern[n][7];
double travel_seg[n - 1][15];
int fill_seg_counter = 0;


double robot_speed;
int choise_of_algo;
string input_file_name = "input_file";
string output_file_name = "Output_Of_fast_algorithm";
int drive2start_case = 0;
bool save_travles = false;
bool last_travel;
double total_travle_weight = 0;
bool outlines_only =  false;
double pre_pattern_val = 10;
double post_pattern_val = 10;

// function to printout a segments from array
void print_seg(int counter) {
    cout << "segments found: " << endl;
    for (int i = 0; i < 14; i++) {
        cout << " - - " << segments[counter][i] << endl;
    }
    cout << " - - -- - - - - - - -- " << endl;
}

// 
bool compare_func(string input_str, string word) {
    bool found = false;
    int full_word_couter = 1;
    int word_length = word.length();
    int full_word_counter = 1;
    bool letter_found = false;
    char char_w = word.at(0);
    string::size_type position = input_str.find(char_w);
    int pos = position;
    string word_out;
    if ((position) != string::npos) {
        word_out = char_w;
        try {
            for (size_t i = 1; i < word_length; i++)
            {
                pos++;
                char input_letter = input_str.at(pos);
                char word_letter = word.at(i);
                if (input_letter == word_letter) {
                    word_out = word_out + word.at(i);
                    full_word_couter++;
                }
                else {
                    letter_found = false;
                    while ((input_str.length() > pos) && (letter_found == false)) {
                        if (input_str.at(pos) == word.at(0)) {
                            full_word_counter = 2;
                            i = 1;
                            pos++;
                            letter_found = true;
                        }
                        else {
                            pos++;
                            i = 1;
                        }
                    }
                }
            }
            if (full_word_couter == word_length) {
                int word_possition[2] = { position, position + word_length };
                found = true;

            }
        }
        catch (exception& e) {
            //cerr << "Cought: " << e.what() << endl;          
        };
    }
    return found;
}

// function to fine the variables within a keyword
string find_var_func(string input_str, string word) {
    bool found;
    string value;
    int full_word_counter = 1;
    int word_length = word.length();
    char char_w = word.at(0);
    string::size_type position = input_str.find(char_w);
    int pos = position;
    string word_out;

    bool letter_found = false;
    if ((position) != string::npos) {
        word_out = char_w;

        try {
            for (size_t i = 1; i < word_length; i++)
            {
                pos++;
                if (input_str.at(pos) == word.at(i) && input_str.length() >= pos)
                {
                    word_out = word_out + word.at(i);
                    full_word_counter++;
                }
                else {
                    letter_found = false;
                    while ((input_str.length() > pos) && (letter_found == false)) {
                        if (input_str.at(pos) == word.at(0)) {
                            full_word_counter = 2;
                            i = 1;
                            pos++;
                            letter_found = true;
                        }
                        else {
                            pos++;
                            i = 1;
                        }
                    }

                }
            }
            if (full_word_counter == word_length) {
                int word_position[2] = { position, position + word_length };
                found = true;

                if (is_it_style == true) {
                    if ((word == "fill:rgb") || (word == "stroke:rgb")) {
                        while (input_str.at(pos) != '(') {
                            pos = pos++;
                        }
                        int pos_counter;
                        pos = pos++;

                        while (input_str.at(pos) != ')') {
                            value = value + input_str.at(pos);
                            pos++;
                        }
                    }
                    else if (word == "stroke-width") {
                        while (input_str.at(pos) != ':') {
                            pos = pos++;
                        }
                        int pos_counter;
                        pos = pos++;

                        while (input_str.at(pos) != ';') {         //|| (input_str.at(pos) != '\"')) 
                            value = value + input_str.at(pos);
                            pos++;
                        }
                    }
                    else {
                        //std::cout << "Error: style unknown \n";
                    }
                }

                else {
                    while (input_str.at(pos) != '\"') {
                        pos++;
                    }
                    int pos_counter;
                    pos++;

                    while (input_str.at(pos) != '\"') {
                        value = value + input_str.at(pos);
                        pos++;
                    }
                }
            }
        }
        catch (exception& e) {
            //cerr << "Cought: " << e.what() << endl;
        };
    }
    else {
    }
    return value;
}

// function used to read the path in a svg
void pathfunc(string input_str, string word) {
    bool found;
    string value;
    int full_word_counter = 1;
    int word_length = word.length();
    char char_w = word.at(0);
    string::size_type position = input_str.find(char_w);
    int pos = position;
    string word_out;

    bool letter_found = false;
    if ((position) != string::npos) {
        word_out = char_w;

        try {
            for (size_t i = 1; i < word_length; i++)
            {
                pos++;
                if (input_str.at(pos) == word.at(i) && input_str.length() >= pos)
                {
                    word_out = word_out + word.at(i);
                    full_word_counter++;
                }
                else {
                    letter_found = false;
                    while ((input_str.length() > pos) && (letter_found == false)) {
                        if (input_str.at(pos) == word.at(0)) {
                            full_word_counter = 2;
                            i = 1;
                            pos++;
                            letter_found = true;
                        }
                        else {
                            pos++;
                            i = 1;
                        }
                    }
                }
            }
            if (full_word_counter == word_length) {
                int word_position[2] = { position, position + word_length };
                while (input_str.at(pos) != '\"') {
                    pos++;
                }
                pos++;
                string output[50];
                double output_dub[50];
                int poly_counter = 0;
                bool found_all_val = false;
                while (input_str.at(pos) != 'M' && input_str.at(pos) != 'L' && input_str.at(pos) != 'H' && input_str.at(pos) != 'V' && input_str.at(pos) != 'C' && input_str.at(pos) != 'S' && input_str.at(pos) != 'Q' && input_str.at(pos) != 'T' && input_str.at(pos) != 'A' && input_str.at(pos) != 'Z') {
                    output[poly_counter] = output[poly_counter] + input_str.at(pos);
                    ++pos;
                    output_dub[poly_counter] = std::stod(output[poly_counter]);
                }
                ++pos;
                ++pos;          
                while ((input_str.at(pos) != ' ') && (input_str.at(pos) != ',')) {
                    output[poly_counter] = output[poly_counter] + input_str.at(pos);
                    ++pos;
                    output_dub[poly_counter] = std::stod(output[poly_counter]);
                }
                ++pos;
                poly_counter++;
                while (input_str.at(pos) != 'M' && input_str.at(pos) != 'L' && input_str.at(pos) != 'H' && input_str.at(pos) != 'V' && input_str.at(pos) != 'C' && input_str.at(pos) != 'S' && input_str.at(pos) != 'Q' && input_str.at(pos) != 'T' && input_str.at(pos) != 'A' && input_str.at(pos) != 'Z' && input_str.at(pos) != '\"/>' && input_str.at(pos) != ' ' && input_str.at(pos) != ',') {
                    output[poly_counter] = output[poly_counter] + input_str.at(pos);
                    output_dub[poly_counter] = std::stod(output[poly_counter]);
                    ++pos;
                }
                poly_counter++;

                while (found_all_val == false) {
                    bool exit_loop = false;
                    if (input_str.at(pos) == 'M') {			//Move to
                        ++pos;
                        while (input_str.at(pos) != 'M' && input_str.at(pos) != 'L' && input_str.at(pos) != 'V' && input_str.at(pos) != 'C' && input_str.at(pos) != 'S' && input_str.at(pos) != 'Q' && input_str.at(pos) != 'T' && input_str.at(pos) != 'A' && input_str.at(pos) != 'Z' && input_str.at(pos) != '\"/>') {
                            ++pos;

                            while (input_str.at(pos) != ' ') {
                                output[poly_counter] = output[poly_counter] + input_str.at(pos);
                                ++pos;
                            }
                            output_dub[poly_counter] = std::stod(output[poly_counter]);
                            poly_counter++;
                            while (input_str.at(pos) != 'M' && input_str.at(pos) != 'L' && input_str.at(pos) != 'V' && input_str.at(pos) != 'C' && input_str.at(pos) != 'S' && input_str.at(pos) != 'Q' && input_str.at(pos) != 'T' && input_str.at(pos) != 'A' && input_str.at(pos) != 'Z' && input_str.at(pos) != '\"/>' && input_str.at(pos) != ' ') {
                               output[poly_counter] = output[poly_counter] + input_str.at(pos);
                                ++pos;
                            }
                            output_dub[poly_counter] = std::stod(output[poly_counter]);
                            poly_counter++;


                            for (size_t i = 1; i >= 0; i--) {
                                segments[counter][0] = counter;     //number of sline segment
                                segments[counter][1] = 0;           //type of line segments
                                segments[counter][2] = output_dub[poly_counter - i];
                                segments[counter][3] = output_dub[poly_counter - i + 1];
                                segments[counter][4] = output_dub[poly_counter - i + 2];
                                segments[counter][5] = output_dub[poly_counter - i + 3];
                                segments[counter][6] = NAN;
                                segments[counter][7] = NAN;
                                segments[counter][8] = NAN;        //bezier curve
                                segments[counter][9] = NAN;        //bezier curve
                                segments[counter][10] = NAN;        //bezier curve
                                segments[counter][11] = NAN;        //bezier curve
                                segments[counter][12] = NAN;    //filled
                                segments[counter][13] = 1;

                                counter++;
                            }
                        }
                        //print_seg(counter);

                    } 

                    else if (input_str.at(pos) == 'L') {		//line to
                        ++pos;
                        while (exit_loop == false) {
                            ++pos;
                            while (input_str.at(pos) == ' ') {
                                ++pos;
                            }
                            while (input_str.at(pos) != ' ') {
                                output[poly_counter] = output[poly_counter] + input_str.at(pos);
                                ++pos;
                            }
                            output_dub[poly_counter] = std::stod(output[poly_counter]);
                            poly_counter++;
                            while (input_str.at(pos) == ' ') {
                                ++pos;
                            }
                            while (input_str.at(pos) != 'M' && input_str.at(pos) != 'L' && input_str.at(pos) != 'V' && input_str.at(pos) != 'C' && input_str.at(pos) != 'S' && input_str.at(pos) != 'Q' && input_str.at(pos) != 'T' && input_str.at(pos) != 'A' && input_str.at(pos) != 'Z' && input_str.at(pos) != '\"/>' && input_str.at(pos) != ' ') {
                                output[poly_counter] = output[poly_counter] + input_str.at(pos);
                                ++pos;
                            }
                            output_dub[poly_counter] = std::stod(output[poly_counter]);                            
                            segments[counter][0] = counter;     
                            segments[counter][1] = 0;           
                            segments[counter][2] = output_dub[poly_counter - 3];
                            segments[counter][3] = output_dub[poly_counter - 2];
                            segments[counter][4] = output_dub[poly_counter - 1];
                            segments[counter][5] = output_dub[poly_counter];
                            segments[counter][6] = NAN;
                            segments[counter][7] = NAN;
                            segments[counter][8] = NAN;
                            segments[counter][9] = NAN;    
                            segments[counter][10] = NAN;       
                            segments[counter][11] = NAN;        
                            segments[counter][12] = NAN;    
                            segments[counter][13] = 1;

                            poly_counter++;
                            counter++;
                            for (size_t i = 0; i < 3; i++)
                            {
                                if ((input_str.at(pos + i) != 'M') && (input_str.at(pos + i) != 'L') && (input_str.at(pos + i) != 'V') && (input_str.at(pos + i) != 'C') && (input_str.at(pos + i) != 'S') && (input_str.at(pos + i) != 'Q') && (input_str.at(pos + i) != 'T') && (input_str.at(pos + i) != 'A') && (input_str.at(pos + i) != 'Z') && (input_str.at(pos + i) != '\"/>')) {
                                    exit_loop = true;
                                }
                            }
                        }
                    }

                    else if (input_str.at(pos) == 'H') {    //Horizontal-lineto
                        ++pos;
                        while (exit_loop == false) {

                            output_dub[poly_counter] = output_dub[poly_counter - 2];
                            poly_counter++;
                            string temp_str;

                            while (input_str.at(pos) == ' ') {
                                ++pos;
                            }
                            while (input_str.at(pos) != 'M' && input_str.at(pos) != 'L' && input_str.at(pos) != 'V' && input_str.at(pos) != 'C' && input_str.at(pos) != 'S' && input_str.at(pos) != 'Q' && input_str.at(pos) != 'T' && input_str.at(pos) != 'A' && input_str.at(pos) != 'Z' && input_str.at(pos) != '\"/>' && input_str.at(pos) != ' ') {
                                output[poly_counter] = output[poly_counter] + input_str.at(pos);
                                ++pos;
                            }
                            output_dub[poly_counter] = std::stod(output[poly_counter]);
                            
                            segments[counter][0] = counter;   
                            segments[counter][1] = 0;          
                            segments[counter][2] = output_dub[poly_counter - 1];
                            segments[counter][3] = output_dub[poly_counter - 2];
                            segments[counter][4] = output_dub[poly_counter];
                            segments[counter][5] = output_dub[poly_counter - 1];
                            segments[counter][6] = NAN;
                            segments[counter][7] = NAN;
                            segments[counter][8] = NAN;       
                            segments[counter][9] = NAN;       
                            segments[counter][10] = NAN;     
                            segments[counter][11] = NAN; 
                            segments[counter][12] = NAN; 
                            segments[counter][13] = 1;
                            poly_counter++;
                            counter++;
                            for (size_t i = 0; i < 3; i++)
                            {   
                                if ((input_str.at(pos + i) != 'M') && (input_str.at(pos + i) != 'L') && (input_str.at(pos + i) != 'V') && (input_str.at(pos + i) != 'C') && (input_str.at(pos + i) != 'S') && (input_str.at(pos + i) != 'Q') && (input_str.at(pos + i) != 'T') && (input_str.at(pos + i) != 'A') && (input_str.at(pos + i) != 'Z') && (input_str.at(pos + i) != '\"/>')) {
                                    exit_loop = true;
                                }
                            }
                        }
                    }

                    //done
                    else if (input_str.at(pos) == 'V') {		// Vertical line - y-axis
                        ++pos;
                        while (exit_loop == false) {
                            //std::cout << input_str.at(pos);
                            ++pos;
                            if (input_str.at(pos) == ' ') {
                                ++pos;
                            }
                            while (input_str.at(pos) != 'M' && input_str.at(pos) != 'L' && input_str.at(pos) != 'V' && input_str.at(pos) != 'C' && input_str.at(pos) != 'S' && input_str.at(pos) != 'Q' && input_str.at(pos) != 'T' && input_str.at(pos) != 'A' && input_str.at(pos) != 'Z' && input_str.at(pos) != '\"/>' && input_str.at(pos) != ' ') {
                              //  std::cout << input_str.at(pos);
                                //std::cout << pos << "\n";	
                                output[poly_counter] = output[poly_counter] + input_str.at(pos);
                                ++pos;
                            }
                            output_dub[poly_counter] = std::stod(output[poly_counter]);
                            poly_counter++;
                            output_dub[poly_counter] = output_dub[poly_counter - 2];

                            segments[counter][0] = counter;    
                            segments[counter][1] = 0;        
                            segments[counter][2] = output_dub[poly_counter - 1];
                            segments[counter][3] = output_dub[poly_counter - 2];
                            segments[counter][4] = output_dub[poly_counter - 2];
                            segments[counter][5] = output_dub[poly_counter];
                            segments[counter][6] = NAN;
                            segments[counter][7] = NAN;
                            segments[counter][8] = NAN;       
                            segments[counter][9] = NAN;     
                            segments[counter][10] = NAN;       
                            segments[counter][11] = NAN;      
                            segments[counter][12] = NAN;  
                            segments[counter][13] = 1;

                            poly_counter++;
                            counter++;

                            for (size_t i = 0; i < 3; i++)
                            {
                                if ((input_str.at(pos + i) != 'M') && (input_str.at(pos + i) != 'L') && (input_str.at(pos + i) != 'V') && (input_str.at(pos + i) != 'C') && (input_str.at(pos + i) != 'S') && (input_str.at(pos + i) != 'Q') && (input_str.at(pos + i) != 'T') && (input_str.at(pos + i) != 'A') && (input_str.at(pos + i) != 'Z') && (input_str.at(pos + i) != '\"/>')) {
                                    exit_loop = true;
                                }
                            }
                        }
                    }

                    //done
                    else if (input_str.at(pos) == 'C') {		//curved line (bizier curve)
                        ++pos;
                        while (exit_loop == false) {
                            for (size_t i = 0; i < 2; i++) {
                                if (input_str.at(pos) == ' ') {
                                    ++pos;
                                }
                                while (input_str.at(pos) != ' ') {
                                    output[poly_counter] = output[poly_counter] + input_str.at(pos);
                                    ++pos;
                                }
                                output_dub[poly_counter] = std::stod(output[poly_counter]);
                                ++pos;
                                poly_counter++;

                                while (input_str.at(pos) != ',') {
                                    output[poly_counter] = output[poly_counter] + input_str.at(pos);
                                    ++pos;
                                }
                                output_dub[poly_counter] = std::stod(output[poly_counter]);
                                poly_counter++;
                                ++pos;
                            }
                            if (input_str.at(pos) == ' ') {
                                ++pos;
                            }
                            while (input_str.at(pos) != ' ') {
                                output[poly_counter] = output[poly_counter] + input_str.at(pos);
                                ++pos;
                            }
                            output_dub[poly_counter] = std::stod(output[poly_counter]);
                            poly_counter++;
                            ++pos;

                            while (input_str.at(pos) != 'M' && input_str.at(pos) != 'L' && input_str.at(pos) != 'V' && input_str.at(pos) != 'C' && input_str.at(pos) != 'S' && input_str.at(pos) != 'Q' && input_str.at(pos) != 'T' && input_str.at(pos) != 'A' && input_str.at(pos) != 'Z' && input_str.at(pos) != '\"/>' && input_str.at(pos) != ' ') {
                                output[poly_counter] = output[poly_counter] + input_str.at(pos);
                                ++pos;
                            }
                            output_dub[poly_counter] = std::stod(output[poly_counter]);

                            segments[counter][0] = counter; 
                            segments[counter][1] = 4;          
                            segments[counter][2] = output_dub[poly_counter-7];
                            segments[counter][3] = output_dub[poly_counter-6];
                            segments[counter][4] = output_dub[poly_counter - 1];
                            segments[counter][5] = output_dub[poly_counter];
                            segments[counter][6] = NAN;
                            segments[counter][7] = NAN;
                            segments[counter][8] = output_dub[poly_counter - 5];    
                            segments[counter][9] = output_dub[poly_counter - 4];      
                            segments[counter][10] = output_dub[poly_counter - 3];   
                            segments[counter][11] = output_dub[poly_counter - 2];     
                            segments[counter][12] = NAN;    
                            segments[counter][13] = 1;

                            poly_counter++;
                            counter++; for (size_t i = 0; i < 3; i++)
                            {
                                if ((input_str.at(pos + i) != 'M') && (input_str.at(pos + i) != 'L') && (input_str.at(pos + i) != 'V') && (input_str.at(pos + i) != 'C') && (input_str.at(pos + i) != 'S') && (input_str.at(pos + i) != 'Q') && (input_str.at(pos + i) != 'T') && (input_str.at(pos + i) != 'A') && (input_str.at(pos + i) != 'Z') && (input_str.at(pos + i) != '\"/>')) {
                                    exit_loop = true;
                                }
                            }
                        }
                    }

                    //done
                    else if (input_str.at(pos) == 'S') {		//smooth curve
                        ++pos;
                        if (input_str.at(pos) == ' ') {
                            ++pos;
                        }
                        while (input_str.at(pos) != ' ') {
                            output[poly_counter] = output[poly_counter] + input_str.at(pos);
                            ++pos;
                        }
                        output_dub[poly_counter] = std::stod(output[poly_counter]);
                        ++pos;
                        poly_counter++;

                        while (input_str.at(pos) != ',') {
                            output[poly_counter] = output[poly_counter] + input_str.at(pos);
                            ++pos;
                        }
                        output_dub[poly_counter] = std::stod(output[poly_counter]);
                        ++pos;
                        poly_counter++;
                        if (input_str.at(pos) == ' ') {
                            ++pos;
                        }
                        while (input_str.at(pos) != ' ') {
                            output[poly_counter] = output[poly_counter] + input_str.at(pos);
                            ++pos;
                        }
                        output_dub[poly_counter] = std::stod(output[poly_counter]);
                        poly_counter++;
                        ++pos;
                        while (input_str.at(pos) != 'M' && input_str.at(pos) != 'L' && input_str.at(pos) != 'V' && input_str.at(pos) != 'C' && input_str.at(pos) != 'S' && input_str.at(pos) != 'Q' && input_str.at(pos) != 'T' && input_str.at(pos) != 'A' && input_str.at(pos) != 'Z' && input_str.at(pos) != '\"/>' && input_str.at(pos) != ' ') {
                            output[poly_counter] = output[poly_counter] + input_str.at(pos);
                            ++pos;
                        }
                        output_dub[poly_counter] = std::stod(output[poly_counter]);


                        segments[counter][0] = counter;    
                        segments[counter][1] = 4;           
                        segments[counter][2] = output_dub[poly_counter - 5];
                        segments[counter][3] = output_dub[poly_counter - 4];
                        segments[counter][4] = output_dub[poly_counter - 1];
                        segments[counter][5] = output_dub[poly_counter];
                        segments[counter][6] = NAN;
                        segments[counter][7] = NAN;
                        segments[counter][8] = output_dub[poly_counter - 7];        
                        segments[counter][9] = output_dub[poly_counter - 6];     
                        segments[counter][10] = output_dub[poly_counter - 3];      
                        segments[counter][11] = output_dub[poly_counter - 2];;      
                        segments[counter][12] = NAN;    
                        segments[counter][13] = 1;

                        poly_counter++;
                        counter++;
                    }

                    //done
                    else if (input_str.at(pos) == 'Q') {		//quadradriv bizier
                        ++pos;
                        for (size_t i = 0; i < 3; i++) {
                            while (input_str.at(pos) == ' ') {
                                ++pos;
                            }
                            while (input_str.at(pos) != ' ') {
                                output[poly_counter] = output[poly_counter] + input_str.at(pos);
                                ++pos;
                            }
                            output_dub[poly_counter] = std::stod(output[poly_counter]);
                            poly_counter++;
                        }

                        if (input_str.at(pos) == ' ') {
                            ++pos;
                        }
                        while (input_str.at(pos) != 'M' && input_str.at(pos) != 'L' && input_str.at(pos) != 'V' && input_str.at(pos) != 'C' && input_str.at(pos) != 'S' && input_str.at(pos) != 'Q' && input_str.at(pos) != 'T' && input_str.at(pos) != 'A' && input_str.at(pos) != 'Z' && input_str.at(pos) != '\"/>' && input_str.at(pos) != ' ') {
                            output[poly_counter] = output[poly_counter] + input_str.at(pos);
                            ++pos;
                            output_dub[poly_counter] = std::stod(output[poly_counter]);
                        }

                        segments[counter][0] = counter;   
                        segments[counter][1] = 4;           
                        segments[counter][2] = output_dub[poly_counter-5];
                        segments[counter][3] = output_dub[poly_counter-4];
                        segments[counter][4] = output_dub[poly_counter - 1];
                        segments[counter][5] = output_dub[poly_counter];
                        segments[counter][6] = NAN;
                        segments[counter][7] = NAN;
                        segments[counter][8] = output_dub[poly_counter - 3];      
                        segments[counter][9] = output_dub[poly_counter - 2];       
                        segments[counter][10] = output_dub[poly_counter - 3];     
                        segments[counter][11] = output_dub[poly_counter - 2];       
                        segments[counter][12] = NAN;   
                        segments[counter][13] = 1;
                        poly_counter++;
                        counter++;

                    }

                    else if (input_str.at(pos) == 'T') {		//smooth quadratic bezier curve
                        ++pos;
                        while (input_str.at(pos) == ' ') {
                            ++pos;
                        }
                        while (input_str.at(pos) != ' ') {
                            output[poly_counter] = output[poly_counter] + input_str.at(pos);
                            ++pos;
                        }
                        output_dub[poly_counter] = std::stod(output[poly_counter]);
                        poly_counter++;

                        while (input_str.at(pos) == ' ') {
                            ++pos;
                        }
                        while (input_str.at(pos) != 'M' && input_str.at(pos) != 'L' && input_str.at(pos) != 'V' && input_str.at(pos) != 'C' && input_str.at(pos) != 'S' && input_str.at(pos) != 'Q' && input_str.at(pos) != 'T' && input_str.at(pos) != 'A' && input_str.at(pos) != 'Z' && input_str.at(pos) != '\"/>' && input_str.at(pos) != ' ') {
                            output[poly_counter] = output[poly_counter] + input_str.at(pos);
                            ++pos;
                        }
                        output_dub[poly_counter] = std::stod(output[poly_counter]);

                        segments[counter][0] = counter;    
                        segments[counter][1] = 4;           
                        segments[counter][2] = segments[counter-1][4];
                        segments[counter][3] = segments[counter-1][5];
                        segments[counter][4] = output_dub[poly_counter - 1];
                        segments[counter][5] = output_dub[poly_counter];
                        segments[counter][6] = NAN;
                        segments[counter][7] = NAN;
                        segments[counter][8] = segments[counter - 1][10]; 
                        segments[counter][9] = segments[counter-1][11];                        
                        segments[counter][10] = segments[counter - 1][10];  
                        segments[counter][11] = segments[counter - 1][11];	
                        segments[counter][12] = NAN;    //filled
                        segments[counter][13] = 1;

                        poly_counter++;
                        counter++;

                        for (size_t i = 0; i < 3; i++)
                        {
                            if ((input_str.at(pos + i) != 'M') && (input_str.at(pos + i) != 'L') && (input_str.at(pos + i) != 'V') && (input_str.at(pos + i) != 'C') && (input_str.at(pos + i) != 'S') && (input_str.at(pos + i) != 'Q') && (input_str.at(pos + i) != 'T') && (input_str.at(pos + i) != 'A') && (input_str.at(pos + i) != 'Z') && (input_str.at(pos + i) != '\"/>')) {
                                exit_loop = true;
                            }
                        }
                    }

                    else if (input_str.at(pos) == 'A') {		//ellipse arc
                        ++pos;
                        for (size_t i = 0; i < 6; i++) {
                            while (input_str.at(pos) == ' ') {
                                ++pos;
                            }
                            bool minus_val = false;
                            while (input_str.at(pos) != ' ') {
                                output[poly_counter] = output[poly_counter] + input_str.at(pos);                            
                                ++pos;
                            }
                            output_dub[poly_counter] = std::stod(output[poly_counter]);
                            poly_counter++;
                        }
                        while (input_str.at(pos) == ' ') {
                            ++pos;
                        }
                        while (input_str.at(pos) != 'M' && input_str.at(pos) != 'L' && input_str.at(pos) != 'V' && input_str.at(pos) != 'C' && input_str.at(pos) != 'S' && input_str.at(pos) != 'Q' && input_str.at(pos) != 'T' && input_str.at(pos) != 'A' && input_str.at(pos) != 'Z' && input_str.at(pos) != '\"/>' && input_str.at(pos) != ' ') {
                            output[poly_counter] = output[poly_counter] + input_str.at(pos);
                            ++pos;
                        }
                        output_dub[poly_counter] = std::stod(output[poly_counter]);
                        
                        segments[counter][0] = counter;    
                        segments[counter][1] = 5;          
                        segments[counter][2] = output_dub[poly_counter - 8];
                        segments[counter][3] = output_dub[poly_counter - 7];
                        segments[counter][4] = output_dub[poly_counter - 1];
                        segments[counter][5] = output_dub[poly_counter];
                        segments[counter][6] = output_dub[poly_counter - 6];
                        segments[counter][7] = output_dub[poly_counter - 5];
                        segments[counter][8] = output_dub[poly_counter - 4];	
                        segments[counter][9] = output_dub[poly_counter - 3];  
                        segments[counter][10] = output_dub[poly_counter - 2];   
                        segments[counter][11] = NAN;    
                        segments[counter][12] = NAN;  
                        segments[counter][13] = 1;
                        ++counter;
                        poly_counter++;

                    }

                    else if (input_str.at(pos) == 'Z') {		//closepath

                        segments[counter][0] = counter;   
                        segments[counter][1] = 0;          
                        segments[counter][2] = segments[counter - 1][4];
                        segments[counter][3] = segments[counter-1][5];
                        segments[counter][4] = output_dub[0];
                        segments[counter][5] = output_dub[1];
                        segments[counter][6] = NAN;
                        segments[counter][7] = NAN;
                        segments[counter][8] = NAN;      
                        segments[counter][9] = NAN;       
                        segments[counter][10] = NAN;    
                        segments[counter][11] = NAN;    
                        segments[counter][12] = NAN; 
                        segments[counter][13] = 1;
                        ++counter;
                        return;

                    }
                    ++pos;
                }

                return;
            }
        }
        catch (exception& e) {
            //cerr << "Cought: " << e.what() << endl;
        };


    }

    return;
}

// polyline and polygon read function
double* polyfunc(string input_str, string word, bool polygon_bool) {
    bool found;
    string value;
    int full_word_counter = 1;
    int word_length = word.length();
    char char_w = word.at(0);
    string::size_type position = input_str.find(char_w);
    int pos = position;
    string word_out;

    bool letter_found = false;
    if ((position) != string::npos) {
        word_out = char_w;
        try {
            for (size_t i = 1; i < word_length; i++)
            {
                pos++;
                if (input_str.at(pos) == word.at(i) && input_str.length() >= pos)
                {
                    word_out = word_out + word.at(i);
                    full_word_counter++;
                }

                else {
                    letter_found = false;
                    while ((input_str.length() > pos) && (letter_found == false)) {
                        if (input_str.at(pos) == word.at(0)) {
                            full_word_counter = 2;
                            i = 1;
                            pos++;
                            letter_found = true;
                        }
                        else {
                            pos++;
                            i = 1;
                        }
                    }

                }
            }
            if (full_word_counter == word_length) {
                int word_position[2] = { position, position + word_length };
                while (input_str.at(pos) != '\"') {         
                    pos++;
                }

                pos++;

                double* output = new double[50];
                int poly_counter = 0;
                bool found_all_val = false;
                while (found_all_val == false) {
                    string temp_poly_x = "";
                    string temp_poly_y = "";

                    while (input_str.at(pos) != ',') {
                        temp_poly_x = temp_poly_x + input_str.at(pos);
                        pos = pos++;
                    }                                                        
                    int pos_counter;               
                    pos = pos++;
                    while (input_str.at(pos) != ' ') {
                        temp_poly_y = temp_poly_y + input_str.at(pos);
                        pos = pos++;
                    }                         
                    poly_counter = poly_counter +2;
                    double poly_counter_double = (double)poly_counter;
                    output[0] = poly_counter;
                    double temp_poly_double_x = stod(temp_poly_x);
                    double temp_poly_double_y = stod(temp_poly_y);
                    output[poly_counter - 1] = temp_poly_double_x;
                    output[poly_counter] = temp_poly_double_y;
                    pos = pos - 1;

                    for (size_t i = 0; i < 3; i++)      
                    {
                        if (input_str.at(pos + i) == '\"') {                            
                            found_all_val = true;
                        }
                    }
                    pos++;
                }

                int size_of_poly = output[0];
                for (size_t i = 1; i < (size_of_poly - 1); i = i + 2)                   
                {
                    segments[counter][0] = counter;     
                    segments[counter][1] = 0;           
                    segments[counter][2] = output[i];
                    segments[counter][3] = output[i + 1];
                    segments[counter][4] = output[i + 2];
                    segments[counter][5] = output[i + 3];
                    segments[counter][6] = NAN;
                    segments[counter][7] = NAN;
                    segments[counter][8] = NAN;        
                    segments[counter][9] = NAN;      
                    segments[counter][10] = NAN;      
                    segments[counter][11] = NAN;      
                    segments[counter][12] = NAN;   
                    segments[counter][13] = 1;

                    counter++;
                }
                if (polygon_bool == true) {
                    segments[counter][0] = counter;    
                    segments[counter][1] = 0;          
                    segments[counter][2] = output[size_of_poly - 1];
                    segments[counter][3] = output[size_of_poly];
                    segments[counter][4] = output[1];
                    segments[counter][5] = output[2];
                    segments[counter][6] = NAN;
                    segments[counter][7] = NAN;
                    segments[counter][8] = NAN;   
                    segments[counter][9] = NAN;    
                    segments[counter][10] = NAN;       
                    segments[counter][11] = NAN;     
                    segments[counter][12] = NAN; 
                    segments[counter][13] = 1;

                    counter++;
                }
                return  output;
            }
        }
            catch (exception& e) {
                //cerr << "Cought: " << e.what() << endl;
            };
        }     

    return 0;
}

double* getLine(string x_start, string y_start, string x_end, string y_end) {
    double* output = new double[4];
    try {
        output[0] = std::stof(x_start);
        output[1] = std::stof(y_start);
        output[2] = std::stof(x_end);
        output[3] = std::stof(y_end);
    }
    catch (exception& e) {
        cerr << "Caught [getLine]: " << e.what() << endl;
    };
    return output;
}

double* getCircle(string x_center, string y_center, string radius, string filled) {
    double* output = new double[4];
    double isFilled = 0;

    if ((filled == "0,0,0") || (filled == "black")) {
        isFilled = 1;
    }
    try {
        output[0] = std::stof(x_center);
        output[1] = std::stof(y_center);
        output[2] = std::stof(radius);
        output[3] = isFilled;
    }
    catch (exception& e) {
        cerr << "Caught [getCircle]: " << e.what() << endl;
    };

    return output;
}

double* getEllipse(string x_center, string y_center, string x_radius, string y_radius, string filled) {
    double* output = new double[5];
    double isFilled = 0;
    if ((filled == "0,0,0") || (filled == "black")) {
        isFilled = 1;
    }
    try {
        output[0] = std::stof(x_center);
        output[1] = std::stof(y_center);
        output[2] = std::stof(x_radius);
        output[3] = std::stof(y_radius);
        output[4] = isFilled;
    }
    catch (exception& e) {
        cerr << "Caught [getEllipse]: " << e.what() << endl;
    };

    return output;
}

void getRect(string x_pos, string y_pos, string width, string height, string filled) {

    double* output = new double[6];
    double isFilled = 0;
    double line_type = 0;

    if ((filled == "0,0,0") || (filled == "black")) {
        isFilled = 1;
        line_type = 3;

        output[0] = std::stof(x_pos);
        output[1] = std::stof(y_pos);
        output[2] = std::stof(width);
        output[3] = std::stof(height);
        output[4] = isFilled;
        output[5] = line_type;

        segments[counter][0] = counter;   
        segments[counter][1] = 3;       
        segments[counter][2] = output[0];
        segments[counter][3] = output[1];
        segments[counter][4] = output[2];
        segments[counter][5] = output[3];
        segments[counter][6] = NAN;
        segments[counter][7] = NAN;
        segments[counter][8] = NAN;       
        segments[counter][9] = NAN;     
        segments[counter][10] = NAN;  
        segments[counter][11] = NAN;      
        segments[counter][12] = 1;        
        segments[counter][13] = 1;
        counter++;
        return;
    }

    else {
        line_type = 1;
        double x1 = std::stod(x_pos);
        double y1 = std::stod(y_pos);
        double w = std::stod(width);
        double h = std::stod(height);
        double x2 = x1 + w;
        double y2 = y1;
        double x3 = x1 + w;
        double y3 = y1 + h;
        double x4 = x1;
        double y4 = y1 + h;

        string sx2 = to_string(x2);
        string sy2 = to_string(y2);
        string sx3 = to_string(x3);
        string sy3 = to_string(y3);
        string sx4 = to_string(x4);
        string sy4 = to_string(y4);


        double* array1 = getLine(x_pos, y_pos, sx2, sy2);
        segments[counter][0] = counter;    
        segments[counter][1] = 0;         
        segments[counter][2] = array1[0];
        segments[counter][3] = array1[1];
        segments[counter][4] = array1[2];
        segments[counter][5] = array1[3];
        segments[counter][6] = NAN;
        segments[counter][7] = NAN;
        segments[counter][8] = NAN;
        segments[counter][9] = NAN;
        segments[counter][10] = NAN;
        segments[counter][11] = NAN;
        segments[counter][12] = NAN;
        segments[counter][13] = 1;
        counter++;
        double* array2 = getLine(sx2, sy2, sx3, sy3);
        segments[counter][0] = counter;     
        segments[counter][1] = 0;           
        segments[counter][2] = array2[0];
        segments[counter][3] = array2[1];
        segments[counter][4] = array2[2];
        segments[counter][5] = array2[3];
        segments[counter][6] = NAN;
        segments[counter][7] = NAN;
        segments[counter][8] = NAN;
        segments[counter][9] = NAN;
        segments[counter][10] = NAN;
        segments[counter][11] = NAN;
        segments[counter][12] = NAN;
        segments[counter][13] = 1;
        counter++;

        double* array3 = getLine(sx3, sy3, sx4, sy4);
        segments[counter][0] = counter;    
        segments[counter][1] = 0;          
        segments[counter][2] = array3[0];
        segments[counter][3] = array3[1];
        segments[counter][4] = array3[2];
        segments[counter][5] = array3[3];
        segments[counter][6] = NAN;
        segments[counter][7] = NAN;
        segments[counter][8] = NAN;
        segments[counter][9] = NAN;
        segments[counter][10] = NAN;
        segments[counter][11] = NAN;
        segments[counter][12] = NAN;
        segments[counter][13] = 1;
        counter++;
        double* array4 = getLine(sx4, sy4, x_pos, y_pos);
        segments[counter][0] = counter;     
        segments[counter][1] = 0;           
        segments[counter][2] = array4[0];
        segments[counter][3] = array4[1];
        segments[counter][4] = array4[2];
        segments[counter][5] = array4[3];
        segments[counter][6] = NAN;
        segments[counter][7] = NAN;
        segments[counter][8] = NAN;
        segments[counter][9] = NAN;
        segments[counter][10] = NAN;
        segments[counter][11] = NAN;
        segments[counter][12] = NAN;
        segments[counter][13] = 1;
        counter++;

    }

    try {
        output[0] = std::stof(x_pos);
        output[1] = std::stof(y_pos);
        output[2] = std::stof(width);
        output[3] = std::stof(height);
        output[4] = isFilled;
    }
    catch (exception& e) {
        cerr << "Caught [getEllipse]: " << e.what() << endl;
    };
}

double get_angle(double vec_ax, double vec_ay, double vec_bx, double vec_by){  
    double alfa_angle;
    double value1 = vec_ax * vec_bx + vec_ay * vec_by;
    double value2 = (sqrt((vec_ax * vec_ax) + (vec_ay * vec_ay))) * (sqrt((vec_bx * vec_bx) + (vec_by * vec_by)));
    alfa_angle = acos((value1 / value2));
    
    double half_circle = 3.14159;
    double omkreads = 23.561945;                        // distance for a 0,75m radius (180deg)         https://www.regneregler.dk/cirkel-omkreds
    double rashio = alfa_angle / half_circle;
    double weight = omkreads * rashio;
    return weight;
}

long double pattern_weight(double x1, double y1, double x2, double y2, int chosen_seg) {
    double pattern_length = 5;       
    double added_dis = 5;             
    double weight = 0;


    //line 
    if (segments[chosen_seg][1] == 0) {
        double vec1_x = x2 - x1;
        double vec1_y = y2 - y1;
        long double vec1_length = sqrt( pow(vec1_x, 2) + pow(vec1_y, 2));

        //add post-pattern to prev seg
        double PostPattern_Px = (((vec1_length + post_pattern_val) / vec1_length) * vec1_x) + x1;
        double PostPattern_Py = (((vec1_length + post_pattern_val) / vec1_length) * vec1_y) + y1;

        //post pattern vector (x2,y2 ;PP_Px,PP_Py add it to the array;
        temp_post_pattern[0] = x2;          // segments[chosen_seg][4];
        temp_post_pattern[1] = y2;          // segments[chosen_seg][5];
        temp_post_pattern[2] = PostPattern_Px;
        temp_post_pattern[3] = PostPattern_Py;

        //add the additional distance to the vector.
        double added_post_dis_x = (((vec1_length + post_pattern_val + added_dis) / (vec1_length + post_pattern_val)) * (PostPattern_Px - x1)) + x1;
        double added_post_dis_y = (((vec1_length + post_pattern_val + added_dis) / (vec1_length + post_pattern_val)) * (PostPattern_Py - y1)) + y1;

        //find the -vector of the next segment
        double vec2_x = segments[chosen_seg][2] - segments[chosen_seg][4];
        double vec2_y = segments[chosen_seg][3] - segments[chosen_seg][5];
        long double vec2_length = sqrt((vec2_x * vec2_x) + (vec2_y * vec2_y));

        //find the length of vec_2 & the prepattern start point
        double PrePattern_Px = (((vec2_length + pre_pattern_val) / vec2_length) * vec2_x) + segments[chosen_seg][4];
        double PrePattern_Py = (((vec2_length + pre_pattern_val) / vec2_length) * vec2_y) + segments[chosen_seg][5];

        //add prepattern to next vector(pre_Patter_Px,pp_py ; segment[chosen_seg][2], segment[chosen_seg][3]
        temp_pre_pattern[0] = PrePattern_Px;
        temp_pre_pattern[1] = PrePattern_Py;                                        
        temp_pre_pattern[2] = segments[chosen_seg][2];
        temp_pre_pattern[3] = segments[chosen_seg][3];

        //add the additional distance to the fector
        double added_pre_dis_x = (((vec2_length + (pre_pattern_val + added_dis)) / vec2_length) * vec2_x) + segments[chosen_seg][4];
        double added_pre_dis_y = (((vec2_length + (pre_pattern_val + added_dis)) / vec2_length) * vec2_y) + segments[chosen_seg][5];

        // find estimated travel vector
        double travel_vec_x = added_pre_dis_x - added_post_dis_x;
        double travel_vec_y = added_pre_dis_y - added_post_dis_y;
        long double travel_length = sqrt((travel_vec_x * travel_vec_x) + (travel_vec_y * travel_vec_y));

        //add post, added_post, pre, added_pre travel length to get the total distance
        double Total_travel_length = travel_length + pre_pattern_val + post_pattern_val + added_dis + 2;            // + PostPattern_length + PrePattern_length + added_post_length + added_pre_length;;
        
        return Total_travel_length;        
    }

    //circle
    else if (segments[chosen_seg][1] == 1) {
        double vec1_x = x2 - x1;
        double vec1_y = y2 - y1;
        long double vec1_length = sqrt((vec1_x * vec1_x) + (vec1_y * vec1_y));

        //add post-pattern to prev seg
        double PostPattern_start_Px = (((vec1_length + post_pattern_val) / vec1_length) * vec1_x) + x1;
        double PostPattern_start_Py = (((vec1_length + post_pattern_val) / vec1_length) * vec1_y) + y1;

        //add the additional distance to the vector
        double added_post_dis_x = (((vec1_length + post_pattern_val + added_dis) / vec1_length) * vec1_x) + x1;
        double added_post_dis_y = (((vec1_length + post_pattern_val + added_dis) / vec1_length) * vec1_y) + y1;

        double tang_point_x;
        double tang_point_y;
        
        if ((segments[chosen_seg][12] == 1) && (outlines_only == false) ){
            tang_point_x = segments[chosen_seg][2] + 1;
            tang_point_y = segments[chosen_seg][3];
        }
        else {      
            double vec_cx = segments[chosen_seg][2] - added_post_dis_x;
            double vec_cy = segments[chosen_seg][3] - added_post_dis_y;
            long double length_vec_c = sqrt(pow(vec_cx, 2) + pow(vec_cy, 2));
            long double length_vec_b = sqrt(pow(length_vec_c, 2) - pow(segments[chosen_seg][7], 2));
            long double angle_c = acos((pow(length_vec_b, 2) + pow(length_vec_c, 2) - pow(segments[chosen_seg][7], 2)) / (2 * length_vec_b * length_vec_c));
            tang_point_x = (((cos(angle_c) * vec_cx) - (sin(angle_c) * vec_cy))) + added_post_dis_x;                             
            tang_point_y = ((sin(angle_c) * vec_cx + cos(angle_c) * vec_cy)) + added_post_dis_y;
        }
        
            //find start point of pre pattern
            double temp_vec_x = tang_point_x - added_post_dis_x;
            double temp_vec_y = tang_point_y - added_post_dis_y;

            long double length_temp_vec = sqrt(pow(temp_vec_x, 2) + pow(temp_vec_y, 2));
            double pre_pattern_start_x = (((length_temp_vec - pre_pattern_val) / length_temp_vec) * temp_vec_x) + added_post_dis_x;           //find start point
            double pre_pattern_start_y = (((length_temp_vec - pre_pattern_val) / length_temp_vec) * temp_vec_y) + added_post_dis_y;
            //cout << "pre_pattern_start_x: " << pre_pattern_start_x << ", pre_pattern_start_y: " << pre_pattern_start_y << endl;

            //PostPattern_end_point_
            double PostPattern_end_Px = (((vec1_length + pre_pattern_val) / vec1_length) * vec1_x + x1);
            double PostPattern_end_Py = (((vec1_length + pre_pattern_val) / vec1_length) * vec1_y + y1);

            double travle_x = tang_point_x - pre_pattern_start_x;       //cahnge this, to make sure its the right vector
            double travle_y = tang_point_y - pre_pattern_start_y;
            long double length_travle = sqrt(pow(travle_x, 2) + pow(travle_y, 2));

            //add prepattern to next vector
            temp_pre_pattern[0] = pre_pattern_start_x;
            temp_pre_pattern[1] = pre_pattern_start_y;
            temp_pre_pattern[2] = tang_point_x;
            temp_pre_pattern[3] = tang_point_y;

            // add start point to circle segment           
            segments[chosen_seg][4] = tang_point_x;
            segments[chosen_seg][5] = tang_point_y;

            //post pattern vector 
            temp_post_pattern[0] = x2;
            temp_post_pattern[1] = y2;
            temp_post_pattern[2] = PostPattern_end_Px;        
            temp_post_pattern[3] = PostPattern_end_Py;
            
            double Total_travel_length = length_travle + pre_pattern_val + post_pattern_val + added_dis + 2;     

            return Total_travel_length;
    }

    //ellipse
    else if (segments[chosen_seg][1] == 2) {
    double tangent_px;
    double tangent_py;
    bool type_x;
    if (segments[chosen_seg][6] > segments[chosen_seg][7]) {
        type_x = false;
        double temp_tang_ypros_px = segments[chosen_seg][2];
        double temp_tang_ypros_py = segments[chosen_seg][3] + segments[chosen_seg][7];
        double vec_ypros_x = temp_tang_ypros_px - x2;
        double vec_ypros_y = temp_tang_ypros_py - y2;
        long double length_vec_ypros = sqrt(pow(vec_ypros_x, 2) + pow(vec_ypros_x, 2));


        double temp_tang_yneg_px = segments[chosen_seg][2];
        double temp_tang_yneg_py = segments[chosen_seg][3] - segments[chosen_seg][7];
        double vec_yneg_x = temp_tang_yneg_px - x2;
        double vec_yneg_y = temp_tang_yneg_py - y2;
        long double length_vec_yneg = sqrt(pow(vec_yneg_x, 2) + pow(vec_yneg_y, 2));

        if (length_vec_ypros > length_vec_yneg) {
            tangent_px = temp_tang_yneg_px;
            tangent_py = temp_tang_yneg_py;
        }
        else {
            tangent_px = temp_tang_ypros_px;
            tangent_py = temp_tang_ypros_py;
        }
    }
    else{
        type_x = true;
        double temp_tang_xpros_px = segments[chosen_seg][2] + segments[chosen_seg][6];
        double temp_tang_xpros_py = segments[chosen_seg][3];
        double vec_xpros_x = temp_tang_xpros_px - x2;
        double vec_xpros_y = temp_tang_xpros_py - y2;
        long double length_vec_xpros = sqrt(pow(vec_xpros_x, 2) + pow(vec_xpros_y, 2));

        double temp_tang_xneg_px = segments[chosen_seg][2] - segments[chosen_seg][6];
        double temp_tang_yneg_py = segments[chosen_seg][3];
        double vec_xneg_x = temp_tang_xneg_px - x2;
        double vec_xneg_y = temp_tang_yneg_py - y2;
        long double length_vec_xneg = sqrt(pow(vec_xneg_x, 2) + pow(vec_xneg_y, 2));

        if (length_vec_xneg > length_vec_xneg) {
            tangent_px = temp_tang_xpros_px;
            tangent_py = temp_tang_xpros_py;
        }
        else {
            tangent_px = temp_tang_xneg_px;
            tangent_py = temp_tang_yneg_py;
        }        
    }
    double vec_2tangent_x = x2 - tangent_px;
    double vec_2tangent_y = y2 - tangent_py;
    double vec_prev_x = x2 - x1;
    double vec_prev_y = y2 - y1;
    double tang_angle = get_angle(vec_prev_x, vec_prev_y, vec_2tangent_x, vec_2tangent_y);

    if (tang_angle == 0) {
        segments[chosen_seg][4] = tangent_px;
        segments[chosen_seg][5] = tangent_py;
        long double length_vec_2tangent = sqrt(pow(vec_2tangent_x, 2) + pow(vec_2tangent_x, 2));
        return length_vec_2tangent;
    }

        else {
            double vec1_x = x2 - x1;
            double vec1_y = y2 - y1;
            long double vec1_length = sqrt((vec1_x * vec1_x) + (vec1_y * vec1_y));

            //add post-pattern to prev seg
            double PostPattern_Px = (((vec1_length + post_pattern_val) / vec1_length) * vec1_x) + x1;
            double PostPattern_Py = (((vec1_length + post_pattern_val) / vec1_length) * vec1_y) + y1;

            //add the additional distance to the vector.
            double added_post_dis_x = (((vec1_length + post_pattern_val + added_dis) / (vec1_length + post_pattern_val)) * (PostPattern_Px - x1)) + x1;
            double added_post_dis_y = (((vec1_length + post_pattern_val + added_dis) / (vec1_length + post_pattern_val)) * (PostPattern_Py - y1)) + y1;
        
            double tang_patter_x;
            double tang_patter_y;
            double tang_added_dis_x;
            double tang_added_dis_y;

            if (type_x == true) {
                if (added_post_dis_y > tangent_py) {
                    tang_patter_x = tangent_px;
                    tang_patter_y = tangent_py + pre_pattern_val;
                    tang_added_dis_x = tangent_px;
                    tang_added_dis_y = tang_patter_y + added_dis;
                }
                else {
                    tang_patter_x = tangent_px;
                    tang_patter_y = tangent_py - pre_pattern_val;
                    tang_added_dis_x = tangent_px;
                    tang_added_dis_y = tang_patter_y- added_dis;
                }
            }
            else {
                if (added_post_dis_x > tangent_px) {
                    tang_patter_x = tangent_px + pre_pattern_val;
                    tang_patter_y = tangent_py;
                    tang_added_dis_x = tang_patter_x + added_dis;
                    tang_added_dis_y = tangent_py;
                }
                else {
                    tang_patter_x = tangent_px - pre_pattern_val;
                    tang_patter_y = tangent_py;
                    tang_added_dis_x = tang_patter_x - added_dis;
                    tang_added_dis_y = tangent_py;
                }
            }

            double new_vecx = added_post_dis_x - tang_added_dis_x;
            double new_vecy = added_post_dis_y - tang_added_dis_y;
            long double length_new_vec = sqrt(pow(new_vecx, 2) + pow(new_vecy, 2));

            temp_post_pattern[0] = x2;
            temp_post_pattern[1] = y2;
            temp_post_pattern[2] = PostPattern_Px;
            temp_post_pattern[3] = PostPattern_Py;
                
            segments[chosen_seg][4] = tangent_px;
            segments[chosen_seg][5] = tangent_py;

            temp_pre_pattern[0] = tang_patter_x;
            temp_pre_pattern[1] = tang_patter_y;
            temp_pre_pattern[2] = tangent_px;
            temp_pre_pattern[3] = tangent_py;        

            double Total_travel_length = length_new_vec + pre_pattern_val + post_pattern_val + added_dis + 2;            
            return Total_travel_length;
        }
    }

    // rect
    else if (segments[chosen_seg][1] == 3) {
    double vec1_x = x2 - x1;
    double vec1_y = y2 - y1;
    long double vec1_length = sqrt(pow(vec1_x, 2) + pow(vec1_y, 2));

    //add post-pattern to prev seg
    double PostPattern_Px = (((vec1_length + post_pattern_val) / vec1_length) * vec1_x) + x1;
    double PostPattern_Py = (((vec1_length + post_pattern_val) / vec1_length) * vec1_y) + y1;

    //post pattern vector (x2,y2 ;PP_Px,PP_Py add it to the array;
    temp_post_pattern[0] = x2;        
    temp_post_pattern[1] = y2;         
    temp_post_pattern[2] = PostPattern_Px;
    temp_post_pattern[3] = PostPattern_Py;

    //add the additional distance to the vector.
    double added_post_dis_x = (((vec1_length + post_pattern_val + added_dis) / (vec1_length + post_pattern_val)) * (PostPattern_Px - x1)) + x1;
    double added_post_dis_y = (((vec1_length + post_pattern_val + added_dis) / (vec1_length + post_pattern_val)) * (PostPattern_Py - y1)) + y1;

    double width = segments[chosen_seg][4];
    double height = segments[chosen_seg][5];
    double new_x = segments[chosen_seg][2];
    double new_y = segments[chosen_seg][3];
    double p1_x = 0;
    double p1_y = 0;
    double p2_x = 0;
    double p2_y = 0;

    if (width > height) {
        double p1_x = new_x;
        double p1_y = new_y + (height / 2);
        double p2_x = new_x + width;
        double p2_y = new_y + (height / 2);
    }
    else {
        double p1_x = new_x + (width / 2);
        double p1_y = new_y;
        double p2_x = new_x + (width / 2);
        double p2_y = new_y + height;
    }

    double vec2_x = p2_x - p1_x;
    double vec2_y = p2_y - p1_y;
    long double vec2_length = sqrt((vec2_x * vec2_x) + (vec2_y * vec2_y));

    //find the length of vec_2 & the prepattern start point
    double PrePattern_Px = (((vec2_length + pre_pattern_val) / vec2_length) * vec2_x) + p2_x;
    double PrePattern_Py = (((vec2_length + pre_pattern_val) / vec2_length) * vec2_y) + p2_y;

    //add prepattern to next vector
    temp_pre_pattern[0] = PrePattern_Px;
    temp_pre_pattern[1] = PrePattern_Py;
    temp_pre_pattern[2] = p1_x;
    temp_pre_pattern[3] = p1_y;

    //add the additional distance to the fector
    double added_pre_dis_x = (((vec2_length + (pre_pattern_val + added_dis)) / vec2_length) * vec2_x) + p2_x;
    double added_pre_dis_y = (((vec2_length + (pre_pattern_val + added_dis)) / vec2_length) * vec2_y) + p2_y;

    // find estimated travel vector
    double travel_vec_x = added_pre_dis_x - added_post_dis_x;
    double travel_vec_y = added_pre_dis_y - added_post_dis_y;
    long double travel_length = sqrt((travel_vec_x * travel_vec_x) + (travel_vec_y * travel_vec_y));

    //add post, added_post, pre, added_pre travel length to get the total distance
    double Total_travel_length = travel_length + pre_pattern_val + post_pattern_val + added_dis + 2;            

    return Total_travel_length;
}
    
    //bezier
    else if (segments[chosen_seg][1] == 4) {
        //post_vector
        double vec1_x = x2 - x1;
        double vec1_y = y2 - y1;
        long double vec1_length = sqrt((vec1_x * vec1_x) + (vec1_y * vec1_y));

        //add post-pattern to prev seg
        double PostPattern_end_Px = (((vec1_length + post_pattern_val) / vec1_length) * vec1_x) + x1;
        double PostPattern_end_Py = (((vec1_length + post_pattern_val) / vec1_length) * vec1_y) + y1;

        //add the additional distance to the vector
        double added_post_dis_x = (((vec1_length + post_pattern_val + added_dis) / vec1_length) * vec1_x) + x1;
        double added_post_dis_y = (((vec1_length + post_pattern_val + added_dis) / vec1_length) * vec1_y) + y1;

        //find pre vector of bezier
        double pre_vec_x = segments[chosen_seg][8] - segments[chosen_seg][2];
        double pre_vec_y = segments[chosen_seg][9] - segments[chosen_seg][3];
        long double pre_vec_length = sqrt(pow(pre_vec_x, 2) + pow(pre_vec_y, 2));
    
        //find pre_pattern start point
        double pre_start_point_x = (((pre_pattern_val) / pre_vec_length) * pre_vec_x) + segments[chosen_seg][2];
        double pre_start_point_y = (((pre_pattern_val) / pre_vec_length) * pre_vec_y) + segments[chosen_seg][3];

        //find added distance
        double added_pre_start_point_x = (((pre_pattern_val + added_dis) / pre_vec_length) * pre_vec_x) + segments[chosen_seg][2];
        double added_pre_start_point_y = (((pre_pattern_val + added_dis) / pre_vec_length) * pre_vec_y) + segments[chosen_seg][3];

        //total distance
        double travle_x = added_post_dis_x - added_pre_start_point_x;
        double travle_y = added_post_dis_y - added_pre_start_point_y;
        long double travle_length = sqrt(pow(travle_x, 2) * pow(travle_y, 2));
        weight = pre_pattern_val + post_pattern_val + 2 * added_dis + travle_length;

        //add prepattern to next vector(pre_Patter_Px,pp_py ; segment[chosen_seg][2], segment[chosen_seg][3]
        temp_pre_pattern[0] = pre_start_point_x;
        temp_pre_pattern[1] = pre_start_point_y;
        temp_pre_pattern[2] = segments[chosen_seg][2];
        temp_pre_pattern[3] = segments[chosen_seg][3];

        //post pattern vector
        temp_post_pattern[0] = x2;
        temp_post_pattern[1] = y2;
        temp_post_pattern[2] = PostPattern_end_Px;       
        temp_post_pattern[3] = PostPattern_end_Py;

        return weight;

    }
    return weight;
}

///find pattern and add weight
long double get_pattern_dis(int k, int prev_seg, int chosen_seg, double tangent_px, double tangent_py) {
    //line
    long double pattern_dis = 0;
    if (segments[prev_seg][1] == 0) {
        pattern_dis = pattern_weight(segments[prev_seg][2], segments[prev_seg][3], segments[prev_seg][4], segments[prev_seg][5], chosen_seg);
    }
    else if (segments[prev_seg][1] == 1) {
        pattern_dis = pattern_weight(pre_pattern[k - 1][3], pre_pattern[k - 1][4], pre_pattern[k - 1][5], pre_pattern[k - 1][6], chosen_seg);        
    }    
    else if (segments[prev_seg][1] == 2) {
        pattern_dis = pattern_weight(pre_pattern[k - 1][3], pre_pattern[k - 1][4], pre_pattern[k - 1][5], pre_pattern[k - 1][6], chosen_seg);
    }

    //bezier
    else if (segments[prev_seg][1] == 4) {
        double some_dis = 10;                  
        double init_vec_x = segments[prev_seg][4] - segments[prev_seg][10];
        double init_vec_y = segments[prev_seg][5] - segments[prev_seg][11];
        long double init_vec_legth = sqrt(pow(init_vec_x, 2) + pow(init_vec_y, 2));
        double start_point_of_some_vec_x = (((init_vec_legth + some_dis) / init_vec_legth) * init_vec_x) + segments[prev_seg][10];
        double start_point_of_some_vec_y = (((init_vec_legth + some_dis) / init_vec_legth) * init_vec_y) + segments[prev_seg][11];
        
        pattern_dis = pattern_weight(start_point_of_some_vec_x, start_point_of_some_vec_y, segments[prev_seg][4], segments[prev_seg][5], chosen_seg);
    }

    // arc 
    else if (segments[prev_seg][1] == 5) {
        pattern_dis = pattern_weight(segments[prev_seg][4], segments[prev_seg][5], segments[prev_seg][4], segments[prev_seg][5], chosen_seg);
    }
    else {
        //std::cout << "Error, get_pattern: worng type defined." << endl;
    }

    return pattern_dis;
}

void post_pattern_func(double x1, double y1, double x2, double y2, int k) {
    double pattern_length = 5;          //default set to 0.5m

    double vec1_x = x2 - x1;
    double vec1_y = y2 - y1;
    long double vec1_length = sqrt((vec1_x * vec1_x) + (vec1_y * vec1_y));

    //add post-pattern to prev seg
    double PostPattern_Px = (((vec1_length + post_pattern_val) / vec1_length) * vec1_x) + x1;
    double PostPattern_Py = (((vec1_length + post_pattern_val) / vec1_length) * vec1_y) + y1;

    post_pattern[k - 1][0] = k;
    post_pattern[k - 1][1] = k;
    post_pattern[k - 1][2] = 0;
    post_pattern[k - 1][3] = x2;
    post_pattern[k - 1][4] = y2;
    post_pattern[k - 1][5] = PostPattern_Px;
    post_pattern[k - 1][6] = PostPattern_Py;
    post_pattern[k - 1][7] = NAN;
    post_pattern[k - 1][8] = NAN;
    post_pattern[k - 1][9] = NAN;
    post_pattern[k - 1][10] = NAN;
    post_pattern[k - 1][11] = NAN;
    post_pattern[k - 1][12] = NAN;
    post_pattern[k - 1][13] = NAN;
    post_pattern[k - 1][14] = 0;
}

void get_last_post_pattern(int prev_seg, int couter) {
    //line
    long double pattern_dis = 0;
    if (segments[prev_seg][1] == 0) {
        post_pattern_func(segments[prev_seg][2], segments[prev_seg][3], segments[prev_seg][4], segments[prev_seg][5], couter);
    }
    else if (segments[prev_seg][1] == 1) {
        post_pattern_func(pre_pattern[prev_seg - 1][3], pre_pattern[prev_seg - 1][4], pre_pattern[prev_seg - 1][5], pre_pattern[prev_seg - 1][6], couter);
    }
    else if (segments[prev_seg][1] == 2) {
        post_pattern_func(pre_pattern[prev_seg - 1][3], pre_pattern[prev_seg - 1][4], pre_pattern[prev_seg - 1][5], pre_pattern[prev_seg - 1][6], couter);
    }
    else {
      //  std::cout << "Error, get_pattern: worng type defined." << endl;
    }
    
}

long double get_vector_length(double x1, double y1, int type, double x2, double y2, double radius, int i) {
    long double vector_lenth;
    long double v_x = x2 - x1;
    long double v_y = y2 - y1;
    vector_lenth = sqrt((v_x * v_x) + (v_y * v_y));         


    if (type == 1) {       //circle
        vector_lenth = vector_lenth - radius;
        long double vec_2_Cp = (vector_lenth) / (vector_lenth + radius);
        segments[i][4] = (x1 + (v_x * vec_2_Cp));
        segments[i][5] = (y1 + (v_y * vec_2_Cp));
    }
    else if (type == 2) {
        vector_lenth = vector_lenth;        
        segments[i][4] = x2 + radius;
        segments[i][5] = y2;
    }
    return vector_lenth;
}

double normal_dis_func(int a_x1, int a_y1, int a_x2, int a_y2 , int b_x1, int b_y1, int b_x2, int b_y2) {
    double vec_ax = a_x2 - a_x1;
    double vec_ay = a_y2 - a_y1;
    double vec_bx = b_x2 - b_x1;
    double vec_by = b_y2 - b_y1;
    double vec_vx = b_x2 - a_x1;
    double vec_vy = b_y2 - a_y1;

    double a_length = sqrt(pow(vec_ax, 2) + pow(vec_ay, 2));
    double v_length = sqrt(pow(vec_vx, 2) + pow(vec_vy, 2));
    double D_angle = get_angle(vec_ax, vec_ay, vec_vx, vec_vy);
    double d_length = (v_length / sin(90)) * sin(D_angle);

    return d_length;
}

//calculate distance of segments
double drawing_dis(int seg_id) {
    int case_id = TSP_segments[seg_id][1];
    long double dis = 0;
    double vec_x;
    double vec_y;
    if (TSP_segments[seg_id][1] == 0) {
        vec_x = TSP_segments[seg_id][2] - TSP_segments[seg_id][4];
        vec_y = TSP_segments[seg_id][3] - TSP_segments[seg_id][5];
        dis = sqrt(pow(vec_x, 2) + pow(vec_y, 2));
    }
    else if (TSP_segments[seg_id][1] == 1) {     //circle
        dis = 2 * TSP_segments[seg_id][6] * 3.14159265358979323846;
    }
    else if (TSP_segments[seg_id][1] == 2) {     // ellipse
        dis = 2 * 3.14159265358979323846 * sqrt((pow(TSP_segments[seg_id][6], 2) + pow(TSP_segments[seg_id][7], 2)) / 2);
    }
    else if (TSP_segments[seg_id][1] == 3) {    //rect      
        //not needed since even the filled rect will be line/type 0
    }
    else if (TSP_segments[seg_id][1] == 4){  //biezir
        vec_x = TSP_segments[seg_id][2] - TSP_segments[seg_id][4];
        vec_y = TSP_segments[seg_id][3] - TSP_segments[seg_id][5];
        dis = sqrt(pow(vec_x, 2) + pow(vec_y, 2));
    }
    else if (TSP_segments[seg_id][1] == 5) {     //arc
        dis = 2 * 3.14159265358979323846 * TSP_segments[seg_id][6] * (TSP_segments[seg_id][8] - TSP_segments[seg_id][9]);
    }
    return dis;
}
//ja lol
double fill_pattern_func(int type, int k, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
    double pattern_length = 5;          
    double added_dis = 10;              
    double weight = 0;

    if (type == 0) {
        //line 
        double vec1_x = x2 - x1;
        double vec1_y = y2 - y1;
        long double vec1_length = sqrt(pow(vec1_x, 2) + pow(vec1_y, 2));

        //add post-pattern to prev seg
        double PostPattern_Px = (((vec1_length + post_pattern_val) / vec1_length) * vec1_x) + x1;
        double PostPattern_Py = (((vec1_length + post_pattern_val) / vec1_length) * vec1_y) + y1;

        //add the additional distance to the vector.
        double added_post_dis_x = (((vec1_length + post_pattern_val + added_dis) / (vec1_length + post_pattern_val)) * (PostPattern_Px - x1)) + x1;
        double added_post_dis_y = (((vec1_length + post_pattern_val + added_dis) / (vec1_length + post_pattern_val)) * (PostPattern_Py - y1)) + y1;

        //find the -vector of the next segment
        double vec2_x = x3 - x4;
        double vec2_y = y3 - y4;
        long double vec2_length = sqrt((vec2_x * vec2_x) + (vec2_y * vec2_y));

        //find the length of vec_2 & the prepattern start point
        double PrePattern_Px = (((vec2_length + pre_pattern_val) / vec2_length) * vec2_x) + x4;
        double PrePattern_Py = (((vec2_length + pre_pattern_val) / vec2_length) * vec2_y) + y4;

        pre_pattern[k][0] = k;
        pre_pattern[k][1] = k;
        pre_pattern[k][2] = 0;
        pre_pattern[k][3] = PrePattern_Px;
        pre_pattern[k][4] = PrePattern_Py;
        pre_pattern[k][5] = x3;
        pre_pattern[k][6] = y3;
        pre_pattern[k][7] = NAN;
        pre_pattern[k][8] = NAN;
        pre_pattern[k][9] = NAN;
        pre_pattern[k][10] = 0;

        post_pattern[k - 1][0] = k - 1;
        post_pattern[k - 1][1] = k - 1;
        post_pattern[k - 1][2] = 0;
        post_pattern[k - 1][3] = x2;
        post_pattern[k - 1][4] = y2;
        post_pattern[k - 1][5] = PostPattern_Px;
        post_pattern[k - 1][6] = PostPattern_Py;
        post_pattern[k - 1][7] = NAN;
        post_pattern[k - 1][8] = NAN;
        post_pattern[k - 1][9] = NAN;
        post_pattern[k - 1][10] = 0;


        //add the additional distance to the fector
        double added_pre_dis_x = (((vec2_length + (pre_pattern_val + added_dis)) / vec2_length) * vec2_x) + x4;
        double added_pre_dis_y = (((vec2_length + (pre_pattern_val + added_dis)) / vec2_length) * vec2_y) + y4;

        // find estimated travel vector
        double travel_vec_x = added_pre_dis_x - added_post_dis_x;
        double travel_vec_y = added_pre_dis_y - added_post_dis_y;
        long double travel_length = sqrt((travel_vec_x * travel_vec_x) + (travel_vec_y * travel_vec_y));

        //add post, added_post, pre, added_pre travel length to get the total distance
        double Total_travel_length = travel_length + pre_pattern_val + post_pattern_val + added_dis + 2;            // + PostPattern_length + PrePattern_length + added_post_length + added_pre_length;;

        return Total_travel_length;
    }

    else if ((type == 1) or (type == 2)) {
        double vec1_x = x3 - x2;
        double vec1_y = y3 - y2;
        long double vec1_length = sqrt(pow(vec1_x, 2) + pow(vec1_y, 2));

        //add post-pattern to prev seg
        double PostPattern_Px = (((post_pattern_val) / vec1_length) * vec1_x) + x1;
        double PostPattern_Py = (((post_pattern_val) / vec1_length) * vec1_y) + y1;

        double vec2_x = x4 - x3;
        double vec2_y = y4 - y3;
        long double vec2_length = sqrt(pow(vec1_x, 2) + pow(vec1_y, 2));

        //add post-pattern to prev seg
        double PrePattern_Px = (((vec2_length + post_pattern_val) / vec2_length) * vec2_x) + x3;
        double PrePattern_Py = (((vec2_length + post_pattern_val) / vec2_length) * vec2_y) + y3;

        pre_pattern[k][0] = k;
        pre_pattern[k][1] = k;
        pre_pattern[k][2] = 0;
        pre_pattern[k][3] = PrePattern_Px;
        pre_pattern[k][4] = PrePattern_Py;
        pre_pattern[k][5] = x3;
        pre_pattern[k][6] = y3;
        pre_pattern[k][7] = NAN;
        pre_pattern[k][8] = NAN;
        pre_pattern[k][9] = NAN;
        pre_pattern[k][10] = 0;        

        post_pattern[k - 1][0] = k - 1;
        post_pattern[k - 1][1] = k - 1;
        post_pattern[k - 1][2] = 0;
        post_pattern[k - 1][3] = x2;
        post_pattern[k - 1][4] = y2;
        post_pattern[k - 1][5] = PostPattern_Px;
        post_pattern[k - 1][6] = PostPattern_Py;
        post_pattern[k - 1][7] = NAN;
        post_pattern[k - 1][8] = NAN;
        post_pattern[k - 1][9] = NAN;
        post_pattern[k - 1][10] = 0;
    }

    else if (type == 3) {
        double vec1_x = x3 - x2;
        double vec1_y = y3 - y2;
        long double vec1_length = sqrt(pow(vec1_x, 2) + pow(vec1_y, 2));

        //add post-pattern to prev seg
        double PostPattern_Px = (((post_pattern_val) / vec1_length) * vec1_x) + x1;
        double PostPattern_Py = (((post_pattern_val) / vec1_length) * vec1_y) + y1;

        double vec2_x = x4 - x3;
        double vec2_y = y4 - y3;
        long double vec2_length = sqrt(pow(vec1_x, 2) + pow(vec1_y, 2));

        //add post-pattern to prev seg
        double PrePattern_Px = (((vec2_length + post_pattern_val) / vec2_length) * vec2_x) + x3;
        double PrePattern_Py = (((vec2_length + post_pattern_val) / vec2_length) * vec2_y) + y3;

        pre_pattern[k][0] = k;
        pre_pattern[k][1] = k;
        pre_pattern[k][2] = 0;
        pre_pattern[k][3] = PrePattern_Px;
        pre_pattern[k][4] = PrePattern_Py;
        pre_pattern[k][5] = x3;
        pre_pattern[k][6] = y3;
        pre_pattern[k][7] = NAN;
        pre_pattern[k][8] = NAN;
        pre_pattern[k][9] = NAN;
        pre_pattern[k][10] = 0;

        post_pattern[k - 1][0] = k - 1;
        post_pattern[k - 1][1] = k - 1;
        post_pattern[k - 1][2] = 0;
        post_pattern[k - 1][3] = x2;
        post_pattern[k - 1][4] = y2;
        post_pattern[k - 1][5] = PostPattern_Px;
        post_pattern[k - 1][6] = PostPattern_Py;
        post_pattern[k - 1][7] = NAN;
        post_pattern[k - 1][8] = NAN;
        post_pattern[k - 1][9] = NAN;
        post_pattern[k - 1][10] = 0;
    }

    else if (type == 4) {

    }
    else if (type == 5) {

    }
}

int  filled_circle(double x, double y, double radius, int k) {
    int amount_of_segments = 0;
    double temp_radius = 0;
    bool first_seg = true;

    for (size_t i = 0; i < radius-1; i++)
    {
        ++temp_radius;
        TSP_segments[k][0] = k;     //number of sline segment
        TSP_segments[k][1] = 1;           //type of linesegments
        TSP_segments[k][2] = x;
        TSP_segments[k][3] = y;
        TSP_segments[k][4] = x + temp_radius;               //needs to be updated       and make sure that the robot will drive clockwise
        TSP_segments[k][5] = y;
        TSP_segments[k][6] = temp_radius;
        TSP_segments[k][7] = temp_radius;
        TSP_segments[k][8] = NAN;
        TSP_segments[k][9] = NAN;
        TSP_segments[k][10] = NAN;
        TSP_segments[k][11] = NAN;
        TSP_segments[k][12] = NAN;
        TSP_segments[k][13] = 1;


        if (first_seg == false) {
            for (size_t n_i = 0; n_i < 7; n_i++)
            {
                pre_pattern[k][n_i] = NAN;
                post_pattern[k][n_i] = NAN;
            }
        }
        else {
            for (size_t n_i = 0; n_i < 7; n_i++)
            {
                post_pattern[k][n_i] = NAN;
            }
            first_seg = false;
        }

        ++k;
        ++counter;
        ++amount_of_segments;
    }

    TSP_segments[k][0] = k;    
    TSP_segments[k][1] = 1;         
    TSP_segments[k][2] = x;
    TSP_segments[k][3] = y;
    TSP_segments[k][4] = x + radius;             
    TSP_segments[k][5] = y;
    TSP_segments[k][6] = radius;
    TSP_segments[k][7] = radius;
    TSP_segments[k][8] = NAN;
    TSP_segments[k][9] = NAN;
    TSP_segments[k][10] = NAN;
    TSP_segments[k][11] = NAN;
    TSP_segments[k][12] = 1;
    TSP_segments[k][13] = 1;
    for (size_t n_i = 0; n_i < 7; n_i++)
    {
        pre_pattern[k][n_i] = NAN;
    }


    post_pattern[k][0] = k;
    post_pattern[k][1] = 0;
    post_pattern[k][2] = x + radius;
    post_pattern[k][3] = y;
    post_pattern[k][4] = x + radius;
    post_pattern[k][5] = y+post_pattern_val;
    post_pattern[k][6] = NAN;

    return k;
    cout << "#";          
}

int check_ellipsepoint(int cx, int cy, int x, int y, int a, int b)
{
    double p = (pow((x - cx), 2) / pow(a, 2)) + (pow((y - cy), 2) / pow(b, 2));
    return p;
}

int filled_ellipse(double cx, double cy, double rx, double ry, int q) {
    double temp_weight = 0;
    double x1, x2, x3, x4, y1, y2, y3, y4;
    x1 = 0;
    y1 = 0;
    x2 = 0;
    y2 = 0;
    x3 = 0;
    y3 = 0;
    x4 = 0;
    y4 = 0;
    double a;
    double b; 
    double p_ellipse_x;
    double p_ellipse_y;

    if (rx > ry) {
        a = rx;
        b = ry;

        double p1_x = cx - rx;
        double p1_y = cy;
        double p2_x = cx + rx;
        double p2_y = cy;
        p_ellipse_x = cx;
        p_ellipse_y = cy-ry;

        // first segments
        TSP_segments[q][0] = q;    
        TSP_segments[q][1] = 0;          
        TSP_segments[q][2] = p1_x;
        TSP_segments[q][3] = p1_y;
        TSP_segments[q][4] = p2_x;              
        TSP_segments[q][5] = p2_y;
        TSP_segments[q][6] = NAN;
        TSP_segments[q][7] = NAN;
        TSP_segments[q][8] = NAN;
        TSP_segments[q][9] = NAN;
        TSP_segments[q][10] = NAN;
        TSP_segments[q][11] = NAN;
        TSP_segments[q][12] = NAN;
        TSP_segments[q][13] = 1;
        x1 = p1_x;
        y1 = p1_y;
        x2 = p2_x;
        y2 = p2_y;
        temp_weight = fill_pattern_func(segments[q - 1][1] ,q, segments[q-1][2], segments[q-1][3], segments[q-1][4], segments[q-1][5], x1, y1, x2, y2);
        total_travle_weight = total_travle_weight + temp_weight;

        ++q;
        ++counter;
        cout << "#";       

        double temp_p1_x;
        double temp_p1_y;
        double temp_p2_x;
        double temp_p2_y;

        for (size_t i = 1; i < b; i++)
        {
            bool point_not_found = true;
            int k = 0;
            while (point_not_found)
            {
                double p2_check_val = check_ellipsepoint(cx, cy, (p2_x - k), (p2_y + i), a, b);

                if (p2_check_val < 1) {
                    temp_p2_x = p2_x - k;
                    temp_p2_y = p2_y + i;
                    point_not_found = false;
                }
                else {
                    ++k;
                }
            }

            point_not_found = true;
            --k;
            //k = 0;
            while (point_not_found)
            {
                double p2_check_val = check_ellipsepoint(cx, cy, (p1_x + k), (p1_y + i), a, b);

                if (p2_check_val < 1) {
                    temp_p1_x = p1_x + k;
                    temp_p1_y = p2_y + i;
                    point_not_found = false;
                }
                else {
                    ++k;
                }
            }

            TSP_segments[q][0] = q;     
            TSP_segments[q][1] = 0;          
            TSP_segments[q][2] = temp_p2_x;
            TSP_segments[q][3] = temp_p2_y;
            TSP_segments[q][4] = temp_p1_x;              
            TSP_segments[q][5] = temp_p1_y;
            TSP_segments[q][6] = NAN;
            TSP_segments[q][7] = NAN;
            TSP_segments[q][8] = NAN;
            TSP_segments[q][9] = NAN;
            TSP_segments[q][10] = NAN;
            TSP_segments[q][11] = NAN;
            TSP_segments[q][12] = NAN;
            TSP_segments[q][13] = 1;                     
            x3 = temp_p2_x;
            y3 = temp_p2_y;
            x4 = temp_p1_x;
            y4 = temp_p1_y;
            temp_weight = fill_pattern_func(0, q, x1, y1, x2, y2, x3, y3, x4, y4);
            total_travle_weight = total_travle_weight + temp_weight;
            ++counter;
            q++;
            cout << "#";        

            point_not_found = true;
            --k;
            //k = 0;
            while (point_not_found)
            {
                double p2_check_val = check_ellipsepoint(cx, cy, (p1_x + k), (p1_y - i), a, b);

                if (p2_check_val < 1) {
                    temp_p1_x = p1_x + k;
                    temp_p1_y = p2_y - i;
                    point_not_found = false;
                }
                else {
                    ++k;
                }
            }

            point_not_found = true;
            --k;
            //k = 0;
            while (point_not_found)
            {
                double p2_check_val = check_ellipsepoint(cx, cy, (p2_x - k), (p2_y - i), a, b);

                if (p2_check_val < 1) {
                    temp_p2_x = p2_x - k;
                    temp_p2_y = p2_y - i;
                    point_not_found = false;
                }
                else {
               //     cout << "Outside" << endl;
                    ++k;
                }
            }

            TSP_segments[q][0] = q;     
            TSP_segments[q][1] = 0;          
            TSP_segments[q][2] = temp_p1_x;
            TSP_segments[q][3] = temp_p1_y;
            TSP_segments[q][4] = temp_p2_x;              
            TSP_segments[q][5] = temp_p2_y;
            TSP_segments[q][6] = NAN;
            TSP_segments[q][7] = NAN;
            TSP_segments[q][8] = NAN;
            TSP_segments[q][9] = NAN;
            TSP_segments[q][10] = NAN;
            TSP_segments[q][11] = NAN;
            TSP_segments[q][12] = NAN;
            TSP_segments[q][13] = 1;                           
            x1 = temp_p1_x;
            y1 = temp_p1_y;
            x2 = temp_p2_x;
            y2 = temp_p2_y;
            temp_weight = fill_pattern_func(1,q, x3, y3, x4, y4, x1, y1, x2, y2);
            total_travle_weight = total_travle_weight + temp_weight;
            ++counter;
            q++;
            cout << "#";      
        }
                
        post_pattern[q - 1][0] = q - 1;
        post_pattern[q - 1][1] = q - 1;
        post_pattern[q - 1][2] = 0;
        post_pattern[q - 1][3] = temp_p2_x;
        post_pattern[q - 1][4] = temp_p2_y;
        post_pattern[q - 1][5] = temp_p2_x + post_pattern_val;          
        post_pattern[q - 1][6] = temp_p2_y;
        post_pattern[q - 1][7] = NAN;
        post_pattern[q - 1][8] = NAN;
        post_pattern[q - 1][9] = NAN;
        post_pattern[q - 1][10] = 0;

        pre_pattern[q][0] = q;
        pre_pattern[q][1] = q;
        pre_pattern[q][2] = 0;
        pre_pattern[q][3] = p_ellipse_x - pre_pattern_val;             
        pre_pattern[q][4] = p_ellipse_y;
        pre_pattern[q][5] = p_ellipse_x;           
        pre_pattern[q][6] = p_ellipse_y;
        pre_pattern[q][7] = NAN;
        pre_pattern[q][8] = NAN;
        pre_pattern[q][9] = NAN;
        pre_pattern[q][10] = 0;

    }
    else {
        b = ry;
        a = rx;   

        double p1_x = cx;
        double p1_y = cy-ry;
        double p2_x = cx;
        double p2_y = cy+ry;

        p_ellipse_x = cx- rx;
        p_ellipse_y = cy;

        TSP_segments[q][0] = q;  
        TSP_segments[q][1] = 0;         
        TSP_segments[q][2] = p1_x;
        TSP_segments[q][3] = p1_y;
        TSP_segments[q][4] = p2_x+post_pattern_val;             
        TSP_segments[q][5] = p2_y;
        TSP_segments[q][6] = NAN;
        TSP_segments[q][7] = NAN;
        TSP_segments[q][8] = NAN;
        TSP_segments[q][9] = NAN;
        TSP_segments[q][10] = NAN;
        TSP_segments[q][11] = NAN;
        TSP_segments[q][12] = NAN;
        TSP_segments[q][13] = 1;
        x1 = p1_x;
        y1 = p1_y;
        x2 = p2_x;
        y2 = p2_y;

        temp_weight = fill_pattern_func(0,q, segments[q - 1][2], segments[q - 1][3], segments[q - 1][4], segments[q - 1][5], x1, y1, x2, y2);
        total_travle_weight = total_travle_weight + temp_weight;

        ++q;
        ++counter;
        cout << "#";        

        double temp_p1_x;
        double temp_p1_y;
        double temp_p2_x;
        double temp_p2_y;

        for (size_t i = 1; i < a; i++)
        {
            bool point_not_found = true;
            double k = 0;
            double add2k = 0.01;
            while (point_not_found)
            {
                double p2_check_val = check_ellipsepoint(cx, cy, (p2_x - i), (p2_y -k), a, b);

                if (p2_check_val < 1) {
                    temp_p2_x = p2_x - i;
                    temp_p2_y = p2_y - k;
                    point_not_found = false;
                }
                else {
                    k = add2k;
                }
            }

            point_not_found = true;
            --k;
            //k = 0;
            while (point_not_found)
            {
                double p2_check_val = check_ellipsepoint(cx, cy, (p1_x - i), (p1_y + k), a, b);

                if (p2_check_val < 1) {
                    temp_p1_x = p1_x - i;
                    temp_p1_y = p2_y + k;
                    point_not_found = false;
                }
                else {
                    k = add2k;
                }
            }

            TSP_segments[q][0] = q;    
            TSP_segments[q][1] = 0;           
            TSP_segments[q][2] = temp_p2_x;
            TSP_segments[q][3] = temp_p2_y;
            TSP_segments[q][4] = temp_p1_x;              
            TSP_segments[q][5] = temp_p1_y;
            TSP_segments[q][6] = NAN;
            TSP_segments[q][7] = NAN;
            TSP_segments[q][8] = NAN;
            TSP_segments[q][9] = NAN;
            TSP_segments[q][10] = NAN;
            TSP_segments[q][11] = NAN;
            TSP_segments[q][12] = NAN;
            TSP_segments[q][13] = 1;
            x3 = temp_p2_x;
            y3 = temp_p2_y;
            x4 = temp_p1_x;
            y4 = temp_p1_y;
            temp_weight = fill_pattern_func(0,q, x1, y1, x2, y2, x3, y3, x4, y4);
            total_travle_weight = total_travle_weight + temp_weight;
            ++counter;
            q++;
            cout << "#";    

            point_not_found = true;
            --k;
            //k = 0;
            while (point_not_found)
            {
                double p2_check_val = check_ellipsepoint(cx, cy, (p1_x - i), (p1_y + k), a, b);

                if (p2_check_val < 1) {
                    temp_p1_x = p1_x - i;
                    temp_p1_y = p2_y + k;
                    point_not_found = false;
                }
                else {
                    k = add2k;
                }
            }


            point_not_found = true;
            --k;
            while (point_not_found)
            {
                double p2_check_val = check_ellipsepoint(cx, cy, (p2_x + i), (p2_y - k), a, b);

                if (p2_check_val < 1) {
                    temp_p2_x = p2_x + i;
                    temp_p2_y = p2_y + k;
                    point_not_found = false;
                }
                else {
                    k = add2k;
                }
            }


            TSP_segments[q][0] = q;    
            TSP_segments[q][1] = 0;          
            TSP_segments[q][2] = temp_p1_x;
            TSP_segments[q][3] = temp_p1_y;
            TSP_segments[q][4] = temp_p2_x;             
            TSP_segments[q][5] = temp_p2_y;
            TSP_segments[q][6] = NAN;
            TSP_segments[q][7] = NAN;
            TSP_segments[q][8] = NAN;
            TSP_segments[q][9] = NAN;
            TSP_segments[q][10] = NAN;
            TSP_segments[q][11] = NAN;
            TSP_segments[q][12] = NAN;
            TSP_segments[q][13] = 1;                           
            x1 = temp_p1_x;
            y1 = temp_p1_y;
            x2 = temp_p2_x;
            y2 = temp_p2_y;
            temp_weight = fill_pattern_func(0,q, x3, y3, x4, y4, x1, y1, x2, y2);
            total_travle_weight = total_travle_weight + temp_weight;
            ++counter;
            q++;
            cout << "#";  
        }
    }

    //outline of the ellipse
    TSP_segments[q][0] = q;  
    TSP_segments[q][1] = 2;       
    TSP_segments[q][2] = cx;
    TSP_segments[q][3] = cy;
    TSP_segments[q][4] = p_ellipse_x;             
    TSP_segments[q][5] = p_ellipse_y;
    TSP_segments[q][6] = rx;
    TSP_segments[q][7] = ry;
    TSP_segments[q][8] = NAN;
    TSP_segments[q][9] = NAN;
    TSP_segments[q][10] = NAN;
    TSP_segments[q][11] = NAN;
    TSP_segments[q][12] = NAN;
    TSP_segments[q][13] = 1;

    cout << "#";     
    return q;
}

int filled_rect(double x, double y, double width, double height, int k) {
    double temp_weight;
    double x1, x2, x3, x4, y1, y2, y3, y4;
    x1 = 0;
    y1 = 0;
    x2 = 0;
    y2 = 0;
    x3 = 0;
    y3 = 0;
    x4 = 0;
    y4 = 0;
    if (width > height) {
        double p1_x = x;
        double p1_y = y + (height / 2);
        double p3_x = x + width;
        double p3_y = y + (height / 2);

        TSP_segments[k][0] = k;    
        TSP_segments[k][1] = 0;         
        TSP_segments[k][2] = p1_x;
        TSP_segments[k][3] = p1_y;
        TSP_segments[k][4] = p3_x;
        TSP_segments[k][5] = p3_y;
        TSP_segments[k][6] = NAN;
        TSP_segments[k][7] = NAN;
        TSP_segments[k][8] = NAN;
        TSP_segments[k][9] = NAN;
        TSP_segments[k][10] = NAN;
        TSP_segments[k][11] = NAN;
        TSP_segments[k][12] = NAN;
        TSP_segments[k][13] = 1;
        x1 = p1_x;
        y1 = p1_y;
        x2 = p3_x;
        y2 = p3_y;
        x3 = 0;
        y3 = 0;
        x4 = 0;
        y4 = 0;

        temp_weight = fill_pattern_func(segments[k - 1][1], k, segments[k-1][2], segments[k-1][3], segments[k-1][4], segments[k-1][5], x1, y1, x2, y2);
        total_travle_weight = total_travle_weight + temp_weight;
        ++k;
        ++counter;
        cout << "#";      

        int fill_counter = (height) / 2;    
        for (size_t i = 1; i < fill_counter; i++)
        {
            double p1_pos_y = p1_y + i;
            double p1_neg_y = p1_y - i;
            double p3_pos_y = p3_y + i;
            double p3_neg_y = p3_y - i;

            TSP_segments[k][0] = k;    
            TSP_segments[k][1] = 0;         
            TSP_segments[k][2] = p3_x;
            TSP_segments[k][3] = p3_pos_y;
            TSP_segments[k][4] = p1_x;
            TSP_segments[k][5] = p1_pos_y;
            TSP_segments[k][6] = NAN;
            TSP_segments[k][7] = NAN;
            TSP_segments[k][8] = NAN;
            TSP_segments[k][9] = NAN;
            TSP_segments[k][10] = NAN;
            TSP_segments[k][11] = NAN;
            TSP_segments[k][12] = NAN;
            TSP_segments[k][13] = 1;
            x3 = p3_x;
            y3 = p3_pos_y;
            x4 = p1_x;
            y4 = p1_pos_y;
            temp_weight = fill_pattern_func(0, k, x1, y1, x2, y2, x3, y3, x4, y4);
            total_travle_weight = total_travle_weight + temp_weight;
            ++k;
            ++counter;
            cout << "#";       
           
            TSP_segments[k][0] = k;   
            TSP_segments[k][1] = 0;       
            TSP_segments[k][2] = p1_x;
            TSP_segments[k][3] = p1_neg_y;
            TSP_segments[k][4] = p3_x;
            TSP_segments[k][5] = p3_neg_y;
            TSP_segments[k][6] = NAN;
            TSP_segments[k][7] = NAN;
            TSP_segments[k][8] = NAN;
            TSP_segments[k][9] = NAN;
            TSP_segments[k][10] = NAN;
            TSP_segments[k][11] = NAN;
            TSP_segments[k][12] = NAN;
            TSP_segments[k][13] = 1;
            x1 = p1_x;
            y1 = p1_neg_y;
            x2 = p3_x;
            y2 = p3_neg_y;
            temp_weight = fill_pattern_func(0, k, x3, y3, x4, y4, x1, y1, x2, y2);
            total_travle_weight = total_travle_weight + temp_weight;
            ++k;
            ++counter;
            cout << "#";    

        }
    }
    else {
        double p4_x = x + (width / 2);
        double p4_y = y;
        double p2_x = x + (width / 2);
        double p2_y = y + height;

        TSP_segments[k][0] = k;    
        TSP_segments[k][1] = 0;         
        TSP_segments[k][2] = p4_x;
        TSP_segments[k][3] = p4_y;
        TSP_segments[k][4] = p2_x;
        TSP_segments[k][5] = p2_y;
        TSP_segments[k][6] = NAN;
        TSP_segments[k][7] = NAN;
        TSP_segments[k][8] = NAN;
        TSP_segments[k][9] = NAN;
        TSP_segments[k][10] = NAN;
        TSP_segments[k][11] = NAN;
        TSP_segments[k][12] = NAN;
        TSP_segments[k][13] = 1;
        x1 = p4_x;
        y1 = p4_y;
        x2 = p2_x;
        y2 = p2_y;
        x3 = 0;
        y3 = 0;
        x4 = 0;
        y4 = 0;
        temp_weight = fill_pattern_func(0, k, segments[k-1][2], segments[k-1][3], segments[k-1][4], segments[k-1][5], x1, y1, x2, y2);
        total_travle_weight = total_travle_weight + temp_weight;
        ++k;
        ++counter;
        cout << "#";       

        int fill_counter = (width) / 2;    
        for (size_t i = 1; i < fill_counter; i++)
        {
            double p4_pos_x = p4_x + i;
            double p4_neg_x = p4_x - i;
            double p2_pos_x = p2_x + i;
            double p2_neg_x = p2_x - i;

            TSP_segments[k][0] = k;    
            TSP_segments[k][1] = 0;    
            TSP_segments[k][2] = p2_neg_x;
            TSP_segments[k][3] = p2_y;
            TSP_segments[k][4] = p4_neg_x;
            TSP_segments[k][5] = p4_y;
            TSP_segments[k][6] = NAN;
            TSP_segments[k][7] = NAN;
            TSP_segments[k][8] = NAN;
            TSP_segments[k][9] = NAN;
            TSP_segments[k][10] = NAN;
            TSP_segments[k][11] = NAN;
            TSP_segments[k][12] = NAN;
            TSP_segments[k][13] = 1;
            x3 = p2_neg_x;
            y3 = p2_y;
            x4 = p4_neg_x;
            y4 = p4_y;
            temp_weight = fill_pattern_func(0, k, x1, y1, x2, y2, x3, y3, x4, y4);
            total_travle_weight = total_travle_weight + temp_weight;
            ++k;
            ++counter;
            cout << "#";      

            //save upper segments
            TSP_segments[k][0] = k; 
            TSP_segments[k][1] = 0;    
            TSP_segments[k][2] = p4_pos_x;
            TSP_segments[k][3] = p4_y;
            TSP_segments[k][4] = p2_pos_x;
            TSP_segments[k][5] = p2_y;
            TSP_segments[k][6] = NAN;
            TSP_segments[k][7] = NAN;
            TSP_segments[k][8] = NAN;
            TSP_segments[k][9] = NAN;
            TSP_segments[k][10] = NAN;
            TSP_segments[k][11] = NAN;
            TSP_segments[k][12] = NAN;
            TSP_segments[k][13] = 1;
            x1 = p4_pos_x;
            y1 = p4_y;
            x2 = p2_pos_x;
            y2 = p2_y;
            temp_weight = fill_pattern_func(0, k, x3, y3, x4, y4, x1, y1, x2, y2);
            total_travle_weight = total_travle_weight + temp_weight;
            ++k;
            ++counter;
            cout << "#";     

        }
    }

    double cor_1x = x;
    double cor_1y = y;
    double cor_2x = x + width;
    double cor_2y = y;
    double cor_3x = x + width;
    double cor_3y = y + height;
    double cor_4x = x;
    double cor_4y = y + height;

    TSP_segments[k][0] = k;   
    TSP_segments[k][1] = 0;       
    TSP_segments[k][2] = cor_3x;
    TSP_segments[k][3] = cor_3y;
    TSP_segments[k][4] = cor_4x;
    TSP_segments[k][5] = cor_4y;
    TSP_segments[k][6] = NAN;
    TSP_segments[k][7] = NAN;
    TSP_segments[k][8] = NAN;
    TSP_segments[k][9] = NAN;
    TSP_segments[k][10] = NAN;
    TSP_segments[k][11] = NAN;
    TSP_segments[k][12] = NAN;
    TSP_segments[k][13] = 1;

    temp_weight = fill_pattern_func(0, k, x1, y1, x2, y2, cor_3x, cor_3y, cor_4x, cor_4y);
    total_travle_weight = total_travle_weight + temp_weight;
    ++k;
    ++counter;
    cout << "#";     

    TSP_segments[k][0] = k;  
    TSP_segments[k][1] = 0;        
    TSP_segments[k][2] = cor_4x;
    TSP_segments[k][3] = cor_4y;
    TSP_segments[k][4] = cor_1x;
    TSP_segments[k][5] = cor_1y;
    TSP_segments[k][6] = NAN;
    TSP_segments[k][7] = NAN;
    TSP_segments[k][8] = NAN;
    TSP_segments[k][9] = NAN;
    TSP_segments[k][10] = NAN;
    TSP_segments[k][11] = NAN;
    TSP_segments[k][12] = NAN;
    TSP_segments[k][13] = 1;
    temp_weight = fill_pattern_func(0, k, cor_3x, cor_3y, cor_4x, cor_4y, cor_4x, cor_4y, cor_1x, cor_1y);
    total_travle_weight = total_travle_weight + temp_weight;
    ++k;
    ++counter;
    cout << "#";     
    TSP_segments[k][0] = k;   
    TSP_segments[k][1] = 0;         
    TSP_segments[k][2] = cor_1x;
    TSP_segments[k][3] = cor_1y;
    TSP_segments[k][4] = cor_2x;
    TSP_segments[k][5] = cor_2y;
    TSP_segments[k][6] = NAN;
    TSP_segments[k][7] = NAN;
    TSP_segments[k][8] = NAN;
    TSP_segments[k][9] = NAN;
    TSP_segments[k][10] = NAN;
    TSP_segments[k][11] = NAN;
    TSP_segments[k][12] = NAN;
    TSP_segments[k][13] = 1;
    temp_weight = fill_pattern_func(0, k, cor_4x, cor_4y, cor_1x, cor_1y, cor_1x, cor_1y, cor_2x, cor_2y);
    total_travle_weight = total_travle_weight + temp_weight;
    ++k;
    ++counter;
    cout << "#";        
    TSP_segments[k][0] = k;   
    TSP_segments[k][1] = 0;      
    TSP_segments[k][2] = cor_2x;
    TSP_segments[k][3] = cor_2y;
    TSP_segments[k][4] = cor_3x;
    TSP_segments[k][5] = cor_3y;
    TSP_segments[k][6] = NAN;
    TSP_segments[k][7] = NAN;
    TSP_segments[k][8] = NAN;
    TSP_segments[k][9] = NAN;
    TSP_segments[k][10] = NAN;
    TSP_segments[k][11] = NAN;
    TSP_segments[k][12] = NAN;
    TSP_segments[k][13] = 1;
    temp_weight = fill_pattern_func(0, k, cor_1x, cor_1y, cor_2x, cor_2y, cor_2x, cor_2y, cor_3x, cor_3y);
    total_travle_weight = total_travle_weight + temp_weight;
    cout << "#";        


    return k;
}

//Terminal output [interface]
void gui_func() {

    int robot_speed_case;
    bool got_correct_input = false;

    std::cout << "__________________________________________________\n";
    while (got_correct_input == false) {
        // select speed
        cout << "Select the speed of the robot, enter:" << endl;
        cout << "   1 - for 1m/s" << endl;
        cout << "   2 - for 0,65m/s" << endl;
        cout << "   3 - for 0,35m/s" << endl;
        cin >> robot_speed_case;
        if (robot_speed_case == 1)
        {
            cout << "   1 - for 1m/s, is selected" << endl << endl;
            robot_speed = 1;
            got_correct_input = true;
        }
        else if (robot_speed_case == 2)
        {
            cout << "   2 - for 0,65m/s, is selected" << endl << endl;
            robot_speed = 0.65;
            got_correct_input = true;
        }
        else if (robot_speed_case == 3)
        {
            cout << "   3 - for 0,35m/s, is selected" << endl << endl;
            robot_speed = 0.35;
            got_correct_input = true;
        }
    }
    got_correct_input = false;

    // select algorithm
    std::cout << "__________________________________________________\n";
    while (got_correct_input == false) {
        cout << "Select choice of algorithm, enter:" << endl << "   1 - for fast algorithm " << endl;
        cin >> choise_of_algo;
        if (choise_of_algo == 1) {
            cout << "good, choice" << endl << "Fast algorithm is selected" << endl << endl;
            got_correct_input = true;
        }
        else {
            cout << "Currectly not avalible" << endl;
        }
    }
    got_correct_input = false;
    
    // select outlines
    std::cout << "__________________________________________________\n";
    while (got_correct_input == false) {
        cout << "What should the robot draw:" << endl;
        cout << "   1 - outlines only " << endl;
        cout << "   2 - everything (outlines and fill)" << endl;
        cin >> choise_of_algo;
        if (choise_of_algo == 1) {
            cout << "outlines only is selected" << endl << endl;
            outlines_only = true;
            got_correct_input = true;
        }
        else if (choise_of_algo == 2) {
            cout << "everything is selected" << endl << endl;
            outlines_only = false;                  
            got_correct_input = true;
        }
        else {
            cout << "Currectly not avalible" << endl;
        }
    }
    got_correct_input = false;
    
    // select simple or exendet output
    std::cout << "__________________________________________________\n";
    while (got_correct_input == false) {
        cout << "which details should the CSV file have" << endl;
        cout << "   1 - only segmentes which should be drawn " << endl;
        cout << "   2 - everything (segmentes which should be drawn, pre-patterns, post-patterns, travels inbetween patterns)" << endl;
        cin >> choise_of_algo;
        if (choise_of_algo == 1) {
            cout << "only segmentes which should be drawn is selected" << endl << endl;
            save_travles = false;
            got_correct_input = true;
        }
        else if (choise_of_algo == 2) {
            cout << "everything is selected" << endl << endl;
            save_travles = true;
            got_correct_input = true;
        }
        else {
            cout << "wrong input" << endl;
        }
    }
    got_correct_input = false;


    // pre pattern
    std::cout << "__________________________________________________\n";
    string pre_patter_intput_str;
    double pre_pattern_input = 5;
    while (got_correct_input == false) {
        cout << "enter length of pre-pattern in cm" << endl;
        cin >> pre_patter_intput_str;
        pre_pattern_input = stod(pre_patter_intput_str);
        if (pre_pattern_input > 0) {
            cout << "pre-pattern length selected: " << pre_pattern_input << endl << endl;
            pre_pattern_val = pre_pattern_input/10;
            got_correct_input = true;
        }
        else {
            cout << "number needs to be bigger then 0" << endl;
        }
}
    got_correct_input = false;

    // post pattern
    std::cout << "__________________________________________________\n";
    string post_patter_intput_str;
    double post_pattern_input = 5;
    while (got_correct_input == false) {
        cout << "enter length of post-pattern in cm" << endl;
        cin >> post_patter_intput_str;
        post_pattern_input = stod(post_patter_intput_str);
        if (post_pattern_input > 0) {
            cout << "pre-pattern length selected: " << post_pattern_val << endl << endl;
            pre_pattern_val = post_pattern_input/10;
            got_correct_input = true;
        }
        else {
            cout << "number needs to be bigger then 0" << endl;
        }
    }
    got_correct_input = false;
    
    // select simple or exendet output
    std::cout << "__________________________________________________\n";
    if (save_travles == true) {
    
        while (got_correct_input == false) {
            cout << "Should the robot drive back to the start possition" << endl;
            cout << "   1 - yes " << endl;
            cout << "   2 - no" << endl;
            cin >> choise_of_algo;
            if (choise_of_algo == 1) {
                cout << "yes is selected" << endl << endl;
                last_travel = true;
                got_correct_input = true;
            }
            else if (choise_of_algo == 2) {
                cout << "no is selected" << endl << endl;
                last_travel = false;
                got_correct_input = true;
            }
            else {
                cout << "wrong input" << endl;
            }
        }
        got_correct_input = false;
    }
    
    // get robot speed
    std::cout << "__________________________________________________\n";
    cout << "Secect a demo path by entering one of the following numbers or enter the path/name.svg." << endl;
    cout << "   1 - input/Football_field_v1,0.svg" << endl;
    cout << "   2 - input/Football_field_v1,1.svg" << endl;
    cout << "   3 - input/Football_field_v2,1.svg" << endl;
    cout << "   4 - input/Football_field_v2,12.svg" << endl;
    cout << "   5 - input/Football_field_v2,2.svg" << endl;
    cout << "   6 - input/turftanklogo_final.svg" << endl;
    cout << "   7 - input/final_logo_polygon.svg" << endl;
    cout << "   8 - input/FinalTest_field.svg" << endl;
    cout << "   9 - input/simple_shaps.svg" << endl;
    cout << "   0 - input/Random_path.svg" << endl;
    cin >> robot_speed_case;
    switch (robot_speed_case) {
    case 1:
        cout << "input/Football_field_v1,0.svg - is selected" << endl << endl;
        input_file_name = "input/Football_field_v1,0.svg";
        got_correct_input = true;
        break;
    case 2:
        cout << "input/Football_field_v1,1.svg - is selected" << endl << endl;
        input_file_name = "input/Football_field_v1,1.svg";
        got_correct_input = true;
        break;
    case 3:
        cout << "input/Football_field_v2,1.svg - is selected" << endl << endl;
        input_file_name = "input/Football_field_v2,1.svg";
        got_correct_input = true;
        break;
    case 4:
        cout << "input/Football_field_v2,12.svg - is selected" << endl << endl;
        input_file_name = "input/Football_field_v2,12.svg";
        got_correct_input = true;
        break;
    case 5:
        cout << "input/Football_field_v2,2.svg - is selected" << endl << endl;
        input_file_name = "input/Football_field_v2,2.svg";
        got_correct_input = true;
        break;
    case 6:
        cout << "input/turftanklogo_final.svg - is selected" << endl << endl;
        input_file_name = "input/turftanklogo_final.svg";
        got_correct_input = true;
        break;
    case 7:
        cout << "input/final_logo_polygon.svg - is selected" << endl << endl;
        input_file_name = "input/final_logo_polygon.svg";
        got_correct_input = true;
        break;
    case 8:
        cout << "input/FinalTest_field.svg - is selected" << endl << endl;
        input_file_name = "input/FinalTest_field.svg";
        got_correct_input = true;
        break;
    case 9:
        cout << "input/simple_shaps.svg - is selected" << endl << endl;
        input_file_name = "input/simple_shaps.svg";
        got_correct_input = true;
        break;
    case 0:
        cout << "input/Random_path.svg - is selected" << endl << endl;
        input_file_name = "input/Random_path.svg";
        got_correct_input = true;
        break;
    }
    if (got_correct_input == false) {
        input_file_name = robot_speed_case;
        cout << input_file_name << " - is selected" << endl << endl;
        got_correct_input = true;
    }
    got_correct_input = false;

    std::cout << "__________________________________________________\n";
    cout << "enter name of output file [eks: first_output]" << endl;
    cout << "Do not use spaces in the name" << endl;
    cin >> output_file_name;

    cout << "Name of outputfile is: " << output_file_name<< endl;
}

// ---------------------- Main -------------------------------------------------
int main() {
    std::cout << "Welcome to Turf Tanks SVG to Path converter v.3.2!\n";
    std::cout << "__________________________________________________\n";
    gui_func();
    ifstream MyReadFile(input_file_name);

    string input_str;                                               
    int svg_counter = 0;
    int circle_counter = 0;
    int rect_counter = 0;
    int ellipse_counter = 0;
    int line_counter = 0;

    string s;
    int sTotal = 0;

    cout << "Reading input File: " << endl;
    cout << "  ||";

    // MyReadFile
    if (MyReadFile.is_open()) {
        while (getline(MyReadFile, input_str)) {
            cout << "#";
            if (compare_func(input_str, "svg")) {
                svg_width[svg_counter] = find_var_func(input_str, "width");
                svg_height[svg_counter] = find_var_func(input_str, "height");
            }

            else if (compare_func(input_str, "circle")) {
                circle_cx[circle_counter] = find_var_func(input_str, "cx");
                circle_cy[circle_counter] = find_var_func(input_str, "y");        
                circle_r[circle_counter] = find_var_func(input_str, "r=");
                if (compare_func(input_str, "style")) {
                    is_it_style = true;
                    circle_strokeWidth[circle_counter] = find_var_func(input_str, "stroke-width");
                    circle_fill[circle_counter] = find_var_func(input_str, "fill:rgb");
                    circle_stroke[circle_counter] = find_var_func(input_str, "stroke:rgb");
                    is_it_style = false;
                    double* array = getCircle(circle_cx[circle_counter], circle_cy[circle_counter], circle_r[circle_counter], circle_fill[circle_counter]);
                    segments[counter][0] = counter;     
                    segments[counter][1] = 1;           
                    segments[counter][2] = array[0];  
                    segments[counter][3] = array[1];  
                    segments[counter][4] = NAN;          
                    segments[counter][5] = NAN;         
                    segments[counter][6] = array[2];    
                    segments[counter][7] = array[2];  
                    segments[counter][8] = NAN;     
                    segments[counter][9] = NAN;      
                    segments[counter][10] = NAN;       
                    segments[counter][11] = NAN;       
                    segments[counter][12] = array[3]; 
                    segments[counter][13] = 1;
                }
                else {
                    circle_stroke[circle_counter] = find_var_func(input_str, "stroke");
                    circle_strokeWidth[circle_counter] = find_var_func(input_str, "stroke-width");
                    circle_fill[circle_counter] = find_var_func(input_str, "fill");
                    double* array = getCircle(circle_cx[circle_counter], circle_cy[circle_counter], circle_r[circle_counter], circle_fill[circle_counter]);
                    segments[counter][0] = counter;   
                    segments[counter][1] = 1;          
                    segments[counter][2] = array[0];  
                    segments[counter][3] = array[1];  
                    segments[counter][4] = NAN;          
                    segments[counter][5] = NAN;       
                    segments[counter][6] = array[2];
                    segments[counter][7] = array[2]; 
                    segments[counter][8] = NAN;       
                    segments[counter][9] = NAN;   
                    segments[counter][10] = NAN;   
                    segments[counter][11] = NAN;     
                    segments[counter][12] = array[3];  
                    segments[counter][13] = 1;
                }
                counter++;
                line_counter++;
            }

            else if (compare_func(input_str, "rect")) {
                rect_x[rect_counter] = find_var_func(input_str, "x");
                rect_y[rect_counter] = find_var_func(input_str, "y");
                rect_width[rect_counter] = find_var_func(input_str, "width");
                rect_height[rect_counter] = find_var_func(input_str, "height");
                if (compare_func(input_str, "style")) {
                    is_it_style = true;
                    rect_fill[rect_counter] = find_var_func(input_str, "fill:rgb");
                    rect_strokeWidth[rect_counter] = find_var_func(input_str, "stroke-width");
                    rect_stroke[rect_counter] = find_var_func(input_str, "stroke:rgb");
                    is_it_style = false;
                    getRect(rect_x[rect_counter], rect_y[rect_counter], rect_width[rect_counter], rect_height[rect_counter], rect_fill[rect_counter]);
                }
                else {
                    rect_stroke[rect_counter] = find_var_func(input_str, "stroke");
                    rect_strokeWidth[rect_counter] = find_var_func(input_str, "stroke-width");
                    rect_fill[rect_counter] = find_var_func(input_str, "fill");

                    getRect(rect_x[rect_counter], rect_y[rect_counter], rect_width[rect_counter], rect_height[rect_counter], rect_fill[rect_counter]);
                }
                rect_counter++;
            }

            else if (compare_func(input_str, "ellipse")) {
                ellipse_x[ellipse_counter] = find_var_func(input_str, "x");
                ellipse_y[ellipse_counter] = find_var_func(input_str, "y");
                ellipse_rx[ellipse_counter] = find_var_func(input_str, "rx");
                ellipse_ry[ellipse_counter] = find_var_func(input_str, "ry");
                if (compare_func(input_str, "style")) {
                    is_it_style = true;
                    ellipse_strokeWidth[ellipse_counter] = find_var_func(input_str, "stroke-width");
                    ellipse_fill[ellipse_counter] = find_var_func(input_str, "fill:rgb");
                    ellipse_stroke[ellipse_counter] = find_var_func(input_str, "stroke:rgb");
                    is_it_style = false;


                    double* array = getEllipse(ellipse_x[ellipse_counter], ellipse_y[ellipse_counter], ellipse_rx[ellipse_counter], ellipse_ry[ellipse_counter], ellipse_fill[ellipse_counter]);
                    segments[counter][0] = counter;     
                    segments[counter][1] = 2;           
                    segments[counter][2] = array[0];
                    segments[counter][3] = array[1];
                    segments[counter][4] = NAN;
                    segments[counter][5] = NAN;
                    segments[counter][6] = array[2];
                    segments[counter][7] = array[3];
                    segments[counter][8] = NAN;      
                    segments[counter][9] = NAN;     
                    segments[counter][10] = NAN;     
                    segments[counter][11] = NAN;     
                    segments[counter][12] = array[4]; 
                    segments[counter][13] = 1;
                }
                else {
                    ellipse_stroke[ellipse_counter] = find_var_func(input_str, "stroke");
                    ellipse_strokeWidth[ellipse_counter] = find_var_func(input_str, "stroke-width");
                    ellipse_fill[ellipse_counter] = find_var_func(input_str, "fill");

                    double* array = getEllipse(ellipse_x[ellipse_counter], ellipse_y[ellipse_counter], ellipse_rx[ellipse_counter], ellipse_ry[ellipse_counter], ellipse_fill[ellipse_counter]);
                    segments[counter][0] = counter;  
                    segments[counter][1] = 2;         
                    segments[counter][2] = array[0];
                    segments[counter][3] = array[1];
                    segments[counter][4] = NAN;
                    segments[counter][5] = NAN;
                    segments[counter][6] = array[2];
                    segments[counter][7] = array[3];
                    segments[counter][8] = NAN;       
                    segments[counter][9] = NAN;    
                    segments[counter][10] = NAN;       
                    segments[counter][11] = NAN;      
                    segments[counter][12] = array[4];  
                    segments[counter][13] = 1;
                }

                counter++;
                ellipse_counter++;
            }

            else if (compare_func(input_str, "line")) {
                line_x1[line_counter] = find_var_func(input_str, "x");
                line_y1[line_counter] = find_var_func(input_str, "y");
                line_x2[line_counter] = find_var_func(input_str, "x2");
                line_y2[line_counter] = find_var_func(input_str, "y2");
                if (compare_func(input_str, "style")) {
                    is_it_style = true;
                    line_strokeWidth[line_counter] = find_var_func(input_str, "stroke:rgb");
                    line_stroke[line_counter] = find_var_func(input_str, "stroke-width");
                    is_it_style = false;
                }
                else {
                    line_stroke[line_counter] = find_var_func(input_str, "rect_fill");
                    line_strokeWidth[line_counter] = find_var_func(input_str, "stroke");
                }
                double* array = getLine(line_x1[line_counter], line_y1[line_counter], line_x2[line_counter], line_y2[line_counter]);
                segments[counter][0] = counter;    
                segments[counter][1] = 0;        
                segments[counter][2] = array[0];
                segments[counter][3] = array[1];
                segments[counter][4] = array[2];
                segments[counter][5] = array[3];
                segments[counter][6] = NAN;
                segments[counter][7] = NAN;
                segments[counter][8] = NAN;      
                segments[counter][9] = NAN;       
                segments[counter][10] = NAN;    
                segments[counter][11] = NAN;      
                segments[counter][12] = NAN;  
                segments[counter][13] = 1;

                counter++;
                line_counter++;
            }

            else if (compare_func(input_str, "polygon")) {
                double* poly_array = polyfunc(input_str, "points", true);
            }

            else if (compare_func(input_str, "polyline")) {
                double* poly_array = polyfunc(input_str, "points", false);
            }

            else if (compare_func(input_str, "path")) {
                pathfunc(input_str, "=");
            }

            else {
                //std::cout << "none found \n";
            }
        }
    }
    else {
    cout << "Unable to open file: " << input_file_name;
    }
    cout << "||\n";
    cout << "  |__________________________________________|" << endl;

    //Output will be shown in the terminal and stored in a file.
    ofstream myfile("Output/Output_of_the_svg.csv");
    string devider = ",";
    myfile << "new_order" << "," << "old_order" << "," << "line-type" << "," << "p1_x" << "," << "p1_y" << "," << "p2_x" << "," << "p2_y" << "," << "rx" << "," << "ry" << "," << "bezier p3_x" << "," << "bezier p3_y" << "," << "bezier p4_x" << "," << "bezier p4_y" << "," << "filled" << "," << "work" << "," << "travle_type" << std::endl;

    if (myfile.is_open())
    {
        for (size_t i = 0; i < counter; i++)
        {

            myfile << segments[i][0];
            myfile << devider;
            myfile << segments[i][0];
            for (size_t j = 1; j <= 13; j++)
            {
                myfile << devider;
                myfile << segments[i][j];
            }
            myfile << "\n";
        }
        myfile.close();
    }
    else cout << "Unable to open file";
    MyReadFile.close();
    
    int processbar_size = counter-1;      // 10;
    double one_up = (counter - 1 )/ processbar_size;
    double temp_one_up = one_up;
    cout << "Algorithm Processbar" << endl << "   ||";
    cout << "   ||";

    const int n_seg = counter - 1;              
    int allready_used_seg[500];               
    allready_used_seg[0] = 0;
    int size_of_used_seg = 0;
    int added_loop_counter = 0;
    const int dis_array_size = counter;
    long double dis_array[255];
    long double anlge_array[255]; 
    int added_loop = 0;
    int chosen_seg = 0;
    int prev_chosen = 0;         
    int temp_lowest_weight = 0;
    double angle_weight;
    double dis_weight;

    //save the first seg
    for (size_t i = 0; i <= 13; i++)
    {
        TSP_segments[0][i] = segments[0][i];
    }
    double p_weight_not_used = pattern_weight(segments[0][2], segments[0][3], segments[0][4], segments[0][5], 0);


    pre_pattern[0][0] = 0;
    pre_pattern[0][1] = 0;
    pre_pattern[0][2] = 0;
    pre_pattern[0][3] = temp_pre_pattern[0];
    pre_pattern[0][4] = temp_pre_pattern[1];
    pre_pattern[0][5] = temp_pre_pattern[2];
    pre_pattern[0][6] = temp_pre_pattern[3];
    pre_pattern[0][7] = NAN;
    pre_pattern[0][8] = NAN;
    pre_pattern[0][9] = NAN;
    pre_pattern[0][10] = 0;

   
    for (int k = 1; k <= (counter - 1 + fill_seg_counter); k++)        
    {
        bool chosen_not_found = true;                        
        for (int i = 1; i <= (n_seg + added_loop); i++)     
        {
            if (i < counter) {                      
                bool seg_already_used = false;
                added_loop_counter = 0;
                for (size_t j = 0; j < size_of_used_seg; j++)
                {
                    if (segments[i][0] == allready_used_seg[j]) {
                        added_loop++;
                        added_loop_counter++;
                        seg_already_used = true;
                    }
                }

                if (seg_already_used == false) {
                    double normal_dis = normal_dis_func(segments[prev_chosen][2], segments[prev_chosen][3], segments[prev_chosen][4], segments[prev_chosen][5], segments[chosen_seg][2], segments[chosen_seg][3], segments[chosen_seg][4], segments[chosen_seg][5]);
                    if (segments[i][1] == 0) {
                        double vec_ax = segments[prev_chosen][4] - segments[prev_chosen][2];
                        double vec_ay = segments[prev_chosen][5] - segments[prev_chosen][3];
                        double vec_bx = segments[chosen_seg][5] - segments[chosen_seg][3];
                        double vec_by = segments[chosen_seg][5] - segments[chosen_seg][3];
                        double angle_alfa = get_angle(vec_ax, vec_ay, vec_bx, vec_by);

                        if ((angle_alfa != 0) && (normal_dis = !0)) {
                            dis_weight = get_pattern_dis(k, prev_chosen, i, 0, 0);
                        }
                        else {
                            double vec_ab_x = vec_bx - vec_ax;
                            double vec_ab_y = vec_by - vec_ay;
                            dis_weight = sqrt(pow(vec_ab_x, 2) + pow(vec_ab_y, 2));
                        }
                        dis_array[i] = dis_weight;
                    }

                    else if (segments[i][1] == 1) {
                        double vec_cx = segments[i][2] - segments[prev_chosen][4];
                        double vec_cy = segments[i][3] - segments[prev_chosen][5];
                        long double length_vec_c = sqrt(pow(vec_cx, 2) + pow(vec_cy, 2));
                        long double length_vec_b = sqrt(pow(length_vec_c, 2) - pow(segments[prev_chosen][6], 2));
                        long double angle_c = acos((pow(length_vec_b, 2) + pow(length_vec_c, 2) - pow(segments[prev_chosen][6], 2)) / (2 * length_vec_b * length_vec_c));
                        long double tang_point_x = ((cos(angle_c) * vec_cx - sin(angle_c) * vec_cx) * (length_vec_b / length_vec_c) + segments[prev_chosen][2]);
                        long double tang_point_y = ((sin(angle_c) * vec_cy + cos(angle_c) * vec_cy) * (length_vec_b / length_vec_c) + segments[prev_chosen][3]);

                        double vec_Tx = tang_point_x - segments[prev_chosen][4];
                        double vec_Ty = tang_point_y - segments[prev_chosen][5];
                        double angle_beta = get_angle(vec_cx, vec_cy, vec_Tx, vec_Ty);
                        if ((angle_beta != 0) && (normal_dis =! 0)) {          
                            dis_weight = dis_weight = get_pattern_dis(k, prev_chosen, i, tang_point_x, tang_point_y); 
                        }
                        else {
                            dis_weight = length_vec_b;
                            temp_pre_pattern[0] = sqrt(-1);
                            temp_pre_pattern[1] = sqrt(-1);
                            temp_pre_pattern[2] = sqrt(-1);                 
                            temp_pre_pattern[3] = sqrt(-1);

                            temp_post_pattern[0] = sqrt(-1);
                            temp_post_pattern[1] = sqrt(-1);
                            temp_post_pattern[2] = sqrt(-1);
                            temp_post_pattern[3] = sqrt(-1);                            
                        }

                        dis_array[i] = dis_weight;
                    }

                    // ellipse
                    else if (segments[i][1] == 2) {
                        double x = segments[chosen_seg][1];
                        double y = segments[chosen_seg][1];

                        dis_weight = get_pattern_dis(k, prev_chosen, i, post_pattern[prev_chosen][4], post_pattern[prev_chosen][5]);     
                        dis_array[i] = dis_weight;
                    }

                    // rectangle
                    else if (segments[i][1] == 3) {
                        dis_weight = dis_weight = get_pattern_dis(k, prev_chosen, i, 0, 0);     
                    }

                    // bizier curve
                    else if (segments[i][1] == 4) {
                        dis_weight =  get_pattern_dis(k, prev_chosen, i, post_pattern[prev_chosen][4], post_pattern[prev_chosen][5]);      //change it to a function.
                    }

                    // arc curve
                    else if (segments[i][1] == 5) {
                        double travle_vec_x = segments[prev_chosen][4] - segments[chosen_seg][2];
                        double travle_vec_y = segments[prev_chosen][5] - segments[chosen_seg][3];                        
                        long double travle_dis = sqrt(pow(travle_vec_x, 2) + pow(travle_vec_y, 2));
                        dis_array[i] = dis_weight;
                    }

                    if (i == (1 + added_loop)) {
                        pre_pattern[k][0] = k;
                        pre_pattern[k][1] = k;
                        pre_pattern[k][2] = 0;
                        pre_pattern[k][3] = temp_pre_pattern[0];
                        pre_pattern[k][4] = temp_pre_pattern[1];
                        pre_pattern[k][5] = temp_pre_pattern[2];
                        pre_pattern[k][6] = temp_pre_pattern[3];
                        pre_pattern[k][7] = NAN;
                        pre_pattern[k][8] = NAN;
                        pre_pattern[k][9] = NAN;
                        pre_pattern[k][10] = 0;

                        if (segments[chosen_seg][1] == 1) {     // if circle add the tangenpoit to the array
                            segments[chosen_seg][4] = pre_pattern[k][3];
                            segments[chosen_seg][5] = pre_pattern[k][4];
                        }

                        post_pattern[k - 1][0] = k- 1;
                        post_pattern[k - 1][1] = k - 1;
                        post_pattern[k - 1][2] = 0;
                        post_pattern[k - 1][3] = temp_post_pattern[0];
                        post_pattern[k - 1][4] = temp_post_pattern[1];
                        post_pattern[k - 1][5] = temp_post_pattern[2];
                        post_pattern[k - 1][6] = temp_post_pattern[3];
                        post_pattern[k - 1][7] = NAN;
                        post_pattern[k - 1][8] = NAN;
                        post_pattern[k - 1][9] = NAN;
                        post_pattern[k - 1][10] = 0;

                        chosen_seg = segments[i][0];        
                        temp_lowest_weight = dis_array[i];   
                    }

                    else if (temp_lowest_weight > dis_array[i]) {
                        pre_pattern[k][0] = k;
                        pre_pattern[k][1] = k;
                        pre_pattern[k][2] = 0;
                        pre_pattern[k][3] = temp_pre_pattern[0];
                        pre_pattern[k][4] = temp_pre_pattern[1];
                        pre_pattern[k][5] = temp_pre_pattern[2];
                        pre_pattern[k][6] = temp_pre_pattern[3];
                        pre_pattern[k][7] = NAN;
                        pre_pattern[k][8] = NAN;
                        pre_pattern[k][9] = NAN;
                        pre_pattern[k][10] = 0;
                        
                        post_pattern[k-1][0] = k;
                        post_pattern[k-1][1] = k;
                        post_pattern[k-1][2] = 0;
                        post_pattern[k-1][3] = temp_post_pattern[0];
                        post_pattern[k-1][4] = temp_post_pattern[1];
                        post_pattern[k-1][5] = temp_post_pattern[2];
                        post_pattern[k-1][6] = temp_post_pattern[3];
                        post_pattern[k-1][7] = NAN;
                        post_pattern[k-1][8] = NAN;
                        post_pattern[k-1][9] = NAN;
                        post_pattern[k-1][10] = 0;
                        
                        chosen_seg = segments[i][0];
                        temp_lowest_weight = dis_array[i];
                    }
                }
            }
        }

        //add fill_segments
        if ((segments[chosen_seg][12] == 1) && (outlines_only == false)) {
            if (segments[chosen_seg][1] == 1) {
                fill_seg_counter = filled_circle(segments[chosen_seg][2], segments[chosen_seg][3], segments[chosen_seg][6], k);
                k = fill_seg_counter;
            }
            else if (segments[chosen_seg][1] == 2) {
                fill_seg_counter = filled_ellipse(segments[chosen_seg][2], segments[chosen_seg][3], segments[chosen_seg][6], segments[chosen_seg][7], k);
                k = fill_seg_counter;
            }
            else if (segments[chosen_seg][1] == 3) {
                fill_seg_counter = filled_rect(segments[chosen_seg][2], segments[chosen_seg][3], segments[chosen_seg][4], segments[chosen_seg][5], k);
                k = fill_seg_counter;
            }
            else {
                //fill not defined
            }
        }

        if (temp_lowest_weight> 0){
            total_travle_weight = total_travle_weight + temp_lowest_weight;
        }

        allready_used_seg[size_of_used_seg] = chosen_seg;
        for (size_t i = 0; i <= 14; i++)
        {
            TSP_segments[k][i] = segments[chosen_seg][i];     
        }

        size_of_used_seg = size_of_used_seg++;
        prev_chosen = segments[chosen_seg][0];
        added_loop = 0;

        if (k > temp_one_up)
        {
            cout << "#";
            temp_one_up = temp_one_up + one_up;
        }
    }

    cout << "#|| \n";

    //add last post pattern
    get_last_post_pattern(prev_chosen, counter);

    double temp_added_dis[4];

    processbar_size = counter - 1;      
    one_up = (counter - 1) / processbar_size;
    temp_one_up = one_up;
    cout << "Add travels [Processbar]" << endl << "   ||";
    cout << "   ||";
    
    if (save_travles) {
        for (size_t seg_counter = 0; seg_counter < counter - 1; seg_counter++)
        {
            double min_radius = 5;      
            double post_sx = post_pattern[seg_counter][3];
            double post_sy = post_pattern[seg_counter][4];
            double post_ex = post_pattern[seg_counter][5];
            double post_ey = post_pattern[seg_counter][6];
            double pre_sx = pre_pattern[seg_counter + 1][3];
            double pre_sy = pre_pattern[seg_counter + 1][4];
            double pre_ex = pre_pattern[seg_counter + 1][5];
            double pre_ey = pre_pattern[seg_counter + 1][6];

            //vec of pre and post pattern
            double vec_post_x = post_ex - post_sx;
            double vec_post_y = post_ey - post_sy;
            double vec_pre_x = pre_sx - pre_ex;
            double vec_pre_y = pre_sy - pre_ey;

            //length of vec
            long double vec_post_length = sqrt(pow(vec_post_x, 2) + pow(vec_post_y, 2));
            long double vec_pre_length = sqrt(pow(vec_pre_x, 2) + pow(vec_pre_y, 2));

            //find bezier points (3 and 4)
            double bezier_post_px = (((min_radius + vec_pre_length) / vec_post_length) * vec_post_x) + post_sx;
            double bezier_post_py = (((min_radius + vec_pre_length) / vec_post_length) * vec_post_y) + post_sy;
            double bezier_pre_px = (((min_radius + vec_pre_length) / vec_post_length) * vec_pre_x) + pre_ex;
            double bezier_pre_py = (((min_radius + vec_pre_length) / vec_post_length) * vec_pre_y) + pre_ey;

            //save in array
            travel_seg[seg_counter][0] = seg_counter;
            travel_seg[seg_counter][1] = seg_counter;
            travel_seg[seg_counter][2] = 4;
            travel_seg[seg_counter][3] = post_pattern[seg_counter][5];
            travel_seg[seg_counter][4] = post_pattern[seg_counter][6];
            travel_seg[seg_counter][5] = pre_pattern[seg_counter + 1][3];
            travel_seg[seg_counter][6] = pre_pattern[seg_counter + 1][4];
            travel_seg[seg_counter][7] = NAN;
            travel_seg[seg_counter][8] = NAN;
            travel_seg[seg_counter][9] = bezier_post_px;      
            travel_seg[seg_counter][10] = bezier_post_py;
            travel_seg[seg_counter][11] = bezier_pre_px;
            travel_seg[seg_counter][12] = bezier_pre_py;
            travel_seg[seg_counter][13] = NAN;
            travel_seg[seg_counter][14] = 0;

            if (seg_counter > temp_one_up)
            {
                cout << "#";
                temp_one_up = temp_one_up + one_up;
            }
        }
    }
        
    cout << "#|| \n";

    if (save_travles) {
        if (last_travel == true) {
            double min_radius = 10;     
            double post_sx = post_pattern[counter - 1][3];     
            double post_sy = post_pattern[counter - 1][4];
            double post_ex = post_pattern[counter - 1][5];
            double post_ey = post_pattern[counter - 1][6];
            double pre_sx = pre_pattern[0][3];                
            double pre_sy = pre_pattern[0][4];
            double pre_ex = pre_pattern[0][5];
            double pre_ey = pre_pattern[0][6];

            //vec of pre and post pattern
            double vec_post_x = post_ex - post_sx;
            double vec_post_y = post_ey - post_sy;
            double vec_pre_x = pre_sx - pre_ex;
            double vec_pre_y = pre_sy - pre_ey;

            //length of vec
            long double vec_post_length = sqrt(pow(vec_post_x, 2) + pow(vec_post_y, 2));
            long double vec_pre_length = sqrt(pow(vec_pre_x, 2) + pow(vec_pre_y, 2));

            //find bezier points (3 and 4)
            double bezier_post_px = (((min_radius + vec_pre_length) / vec_post_length) * vec_post_x) + post_sx;
            double bezier_post_py = (((min_radius + vec_pre_length) / vec_post_length) * vec_post_y) + post_sy;
            double bezier_pre_px = (((min_radius + vec_pre_length) / vec_post_length) * vec_pre_x) + pre_ex;
            double bezier_pre_py = (((min_radius + vec_pre_length) / vec_post_length) * vec_pre_y) + pre_ey;

            //find length of blub
            double tvec_x = pre_pattern[0][3] - post_pattern[counter - 1][5];
            double tvec_y = pre_pattern[0][4] - post_pattern[counter - 1][6];
            long double tvec_length = sqrt(pow(tvec_x, 2) + pow(tvec_y, 2));

            //save in array
            travel_seg[counter - 1][0] = counter;
            travel_seg[counter - 1][1] = counter;
            travel_seg[counter - 1][2] = 4;
            travel_seg[counter - 1][3] = post_pattern[counter - 1][5];
            travel_seg[counter - 1][4] = post_pattern[counter - 1][6];
            travel_seg[counter - 1][5] = pre_pattern[0][3];
            travel_seg[counter - 1][6] = pre_pattern[0][4];
            travel_seg[counter - 1][7] = NAN;
            travel_seg[counter - 1][8] = NAN;
            travel_seg[counter - 1][9] = bezier_post_px;
            travel_seg[counter - 1][10] = bezier_post_py;
            travel_seg[counter - 1][11] = bezier_pre_px;
            travel_seg[counter - 1][12] = bezier_pre_py;
            travel_seg[counter - 1][13] = NAN;
            travel_seg[counter - 1][14] = 0;

            //add distance to total dis
            total_travle_weight = total_travle_weight + tvec_length + vec_pre_length + vec_post_length;
        }
    }

    //find total weight 
    double distance_drawn = 0;
    for (size_t i_drawn = 0; i_drawn < counter; i_drawn++)
    {
        double temp_dis_drawn = drawing_dis(i_drawn);
        distance_drawn = distance_drawn + temp_dis_drawn;
    }
    total_travle_weight = total_travle_weight;  //  / 10;
    distance_drawn = distance_drawn;            //  / 10;
    double total_distance_driven = total_travle_weight + distance_drawn;


    //Save data in file
    string output_file_location = "Output/";
    string output_file_type = ".csv";
    string output_file_final_location = (output_file_location + output_file_name + output_file_type);
    ofstream OutPutfile(output_file_final_location);

    if (OutPutfile.is_open())
    {
        OutPutfile << "new_order" << "," << "old_order" << "," << "line-type" << "," << "p1_x" << "," << "p1_y" << "," << "p2_x" << "," << "p2_y" << "," << "rx" << "," << "ry" << "," << "bezier p3_x" << "," << "bezier p3_y" << "," << "bezier p4_x" << "," << "bezier p4_y" << "," <<  "filled" << "," << "work" << "," << "travle_type" << std::endl;
        for (size_t p_k = 0; p_k < counter; p_k++) {
            if (save_travles) {
                for (size_t pre_i = 0; pre_i < 7; pre_i++)
                {
                    OutPutfile << pre_pattern[p_k][pre_i] << ",";
                }
                for (size_t i = 0; i < 7; i++)
                {
                    OutPutfile << NAN << ",";
                }
                OutPutfile << 0 << "," << "pre-pattern " << endl;
            }
                OutPutfile << p_k << ",";
            for (size_t p_i = 0; p_i < 14; p_i++)
            {
                OutPutfile << TSP_segments[p_k][p_i] << ",";
            }
            OutPutfile << "drawn_seg " << endl;

            if (save_travles) {
                for (size_t post_i = 0; post_i < 7; post_i++)
                {
                    OutPutfile << post_pattern[p_k][post_i] << ",";
                }
                for (size_t i = 0; i < 7; i++)
                {
                    OutPutfile << NAN << ",";
                }
                OutPutfile << 0 << "," << "post-pattern " << endl;

                for (size_t travel_i = 0; travel_i < 15; travel_i++)
                {
                    OutPutfile << travel_seg[p_k][travel_i] << ",";
                }
                OutPutfile << "travel " << endl;
            }
        }
        OutPutfile.close();
    }
    else cout << "Unable to open file";
    MyReadFile.close();

    double time_converstion = 60 * 60;
    cout << endl << endl;
    cout << "############################################" << endl;
    cout << "# Total distance driving: " << total_distance_driven << "m" << endl;
    cout << "# Total distance drawing: " << distance_drawn << "m" << endl;
    cout << "# Total distance driven with out drawing: " << total_travle_weight << "m" << endl;
    cout << "# Set driving speed of the robot: " << robot_speed << 'm/s' << endl;
    cout << "# Estimated driving time: " << (total_distance_driven/ robot_speed) << "sec || " << (total_distance_driven / robot_speed) / 60 << " Min || " << (total_distance_driven / robot_speed) / time_converstion << " hours" << endl;
    cout << "# Estimated drawing time: " << (distance_drawn / robot_speed) << "sec || " << (distance_drawn / robot_speed) / 60 << " Min || " << (distance_drawn / robot_speed) / time_converstion << " hours" << endl;
    cout << "# Estimated driven time with out drawing: " << (total_travle_weight / robot_speed) << "sec || " << (total_distance_driven / robot_speed) / 60 << " Min || " << (total_distance_driven / robot_speed) / time_converstion << " hours" << endl;
    cout << "############################################" << endl;
    cout << endl << "Output is save here: " << output_file_final_location << endl;
}
