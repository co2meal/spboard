#include <string>
#include <sstream>
#include <iostream>
#include <json/json.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata) {
    std::ostringstream *stream = (std::ostringstream*)userdata;
    size_t count = size * nmemb;
    stream->write(ptr, count);
    return count;
}

Json::Value attendance(string student_id, string course_id, string lecture_id, string filename) {
  Json::Value root;   // will contains the root value after parsing.
  Json::Reader reader;
  string body;

  FILE *fp;
  int status;
  char result[1035];

  fp = popen(string("curl -s -X POST -H 'Content-Type: image/jpeg' --data-binary @" + filename + " 'http://192.168.0.227:3000/courses/1/lectures/10/attendances.json?attendance\\[student_id\\]=" + student_id + "&filename=" + filename + "'").c_str(), "r");
  if (fp == NULL) {
    printf("Failed to run command \n");
    return root;
  }

  /* Read the output a line at a time - output it. */
  while (fgets(result, sizeof(result)-1, fp) != NULL) {
    body = result;
  }

  /* close */
  pclose(fp);
  
  bool parsingSuccessful = reader.parse( body, root );
  return root;
}

void curltest(void)
{
  cout << attendance("1", "1", "30", "photo.jpg") << endl;
  cout << attendance("2", "1", "30", "photo.jpg") << endl;
}
