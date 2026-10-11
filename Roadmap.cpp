#include "Step.h"
#include "Roadmap.h"

#include<fstream>
#include<sstream>
#include<iostream>
#include<vector>
#include<string>
#include<exception>
using namespace std;

vector<string> parseCSVLine(const string& line){
  vector<string> fields;
  string field;
  bool insideQuotes = false ;

  for(size_t i = 0; i<line.length(); i++){
    char ch = line[i];

    if (ch == '"'){
      if (insideQuotes && i+1 < line.length() && line[i+1] == '"'){
       field+='"';
       i++;
      }
      else{
        insideQuotes = !insideQuotes;
      }
    }
    else if(ch == ','&& !insideQuotes){
      fields.push_back(field);
      field.clear();
    }
    else{
      field+=ch;
    }
  }
  fields.push_back(field);
  return fields;
}

Roadmap::Roadmap(string CareerName){
  this->CareerName = CareerName;  
}

string Roadmap::getCareerName(){
  return CareerName;
}

vector<Step> Roadmap::getSteps() {
    return steps;
}

void Roadmap::addStep(Step step) {
    steps.push_back(step);
}

bool Roadmap::loadStepsFromCSV(const string& filename){
  ifstream file(filename);

 if(!file.is_open()){
  cout<<"Error : Could not open " << filename<< endl;
  return false;
 }

 steps.clear();

 string line;
 if (!getline(file,line)){
  file.close();
  return false;
 }
 while (getline(file,line)){
  if(line.empty())
   continue;
  vector<string> fields = parseCSVLine(line);

  if(fields.size() != 4)
    continue;
  
  if (fields[0]== CareerName){
    try{
      int number = stoi(fields[1]);

      Step step(number, fields[2],fields[3]);
      addStep(step);
    }
    catch(const exception&){
      continue;
    }
  }
 }
 file.close();
 return !steps.empty();
}
