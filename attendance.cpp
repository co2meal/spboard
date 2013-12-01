#include <string>
#include <sstream>
#include <iostream>
#include <json/json.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "led.h"
using namespace std;

size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata) {
    std::ostringstream *stream = (std::ostringstream*)userdata;
    size_t count = size * nmemb;
    stream->write(ptr, count);
    return count;
}

Json::Value attendance(string student_id, string course_id, string filename) {
  Json::Value root;   // will contains the root value after parsing.
  Json::Reader reader;
  string body;

  FILE *fp;
  int status;
  char result[1035];

  mark_communicating(1);
  // cmd = curl -s -X POST -H 'Content-Type: image/jpeg' --data-binary @temp.jpg 'http://overpl.us:3000/courses/1/attendances.json?attendance\[student_id\]=201020323&filename=temp.jpg'
  string cmd = string("curl -s --connect-timeout 60 -X POST -H 'Content-Type: image/jpeg' --data-binary @" + filename + " 'http://overpl.us:3000/courses/" + course_id + "/attendances.json?attendance\\[student_id\\]=" + student_id + "&filename=" + filename + "'");
  cout << cmd << endl;
  fp = popen(cmd.c_str(), "r");
  if (fp == NULL) {
    printf("Failed to run command \n");
    mark_communicating(0);
    return root;
  }

  /* Read the output a line at a time - output it. */
  while (fgets(result, sizeof(result)-1, fp) != NULL) {
    body = result;
  }

  /* close */
  pclose(fp);
  cout << body << endl;
  
  bool parsingSuccessful = reader.parse( body, root );
  if (!parsingSuccessful) {
    reader.parse("{\"error\": \"Server error!\"}", root);
  }
  mark_communicating(0);
  return root;
}

/*
void (void)
{
  cout << attendance("1", "201020323", "temp.jpg") << endl;
}
*/
