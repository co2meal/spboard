#include <string>
#include <sstream>
#include <iostream>
#include <json/json.h>
#include <curl/curl.h>
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

  CURL *curl;
  CURLM *multi_handle;
  int still_running;
  string body;
  Json::Value root;   // will contains the root value after parsing.
  Json::Reader reader;
 
  struct curl_httppost *formpost=NULL;
  struct curl_httppost *lastptr=NULL;
  struct curl_slist *headerlist=NULL;
  static const char buf[] = "Expect:";
  std::ostringstream stream;
 
  /* Fill in the file upload field. This makes libcurl load data from
     the given file name when curl_easy_perform() is called. */ 
  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "attendance[picture][image]",
               CURLFORM_FILE, filename.c_str(),
               CURLFORM_END);
 
  /* Fill in the submit field too, even if this is rarely needed */ 
  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "attendance[student_id]",
               CURLFORM_COPYCONTENTS, student_id.c_str(),
               CURLFORM_END);
  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "submit",
               CURLFORM_COPYCONTENTS, "send",
               CURLFORM_END);
 
  curl = curl_easy_init();
  multi_handle = curl_multi_init();
 
  /* initalize custom header list (stating that Expect: 100-continue is not
     wanted */ 
  headerlist = curl_slist_append(headerlist, buf);
  if(curl && multi_handle) {
 
    /* what URL that receives this POST */ 
    curl_easy_setopt(curl, CURLOPT_URL, string("http://localhost:3000/courses/" + course_id + "/lectures/" + lecture_id + "/attendances.json").c_str());
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
 
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stream);
 
    curl_multi_add_handle(multi_handle, curl);
 
    curl_multi_perform(multi_handle, &still_running);
 
    do {
      struct timeval timeout;
      int rc; /* select() return code */ 
 
      fd_set fdread;
      fd_set fdwrite;
      fd_set fdexcep;
      int maxfd = -1;
 
      long curl_timeo = -1;
 
      FD_ZERO(&fdread);
      FD_ZERO(&fdwrite);
      FD_ZERO(&fdexcep);
 
      /* set a suitable timeout to play around with */ 
      timeout.tv_sec = 1;
      timeout.tv_usec = 0;
 
      curl_multi_timeout(multi_handle, &curl_timeo);
      if(curl_timeo >= 0) {
        timeout.tv_sec = curl_timeo / 1000;
        if(timeout.tv_sec > 1)
          timeout.tv_sec = 1;
        else
          timeout.tv_usec = (curl_timeo % 1000) * 1000;
      }
 
      /* get file descriptors from the transfers */ 
      curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);
 
      /* In a real-world program you OF COURSE check the return code of the
         function calls.  On success, the value of maxfd is guaranteed to be
         greater or equal than -1.  We call select(maxfd + 1, ...), specially in
         case of (maxfd == -1), we call select(0, ...), which is basically equal
         to sleep. */ 
 
      rc = select(maxfd+1, &fdread, &fdwrite, &fdexcep, &timeout);
 
      switch(rc) {
      case -1:
        /* select error */ 
        break;
      case 0:
      default:
        /* timeout or readable/writable sockets */ 
        //printf("perform!\n");
        curl_multi_perform(multi_handle, &still_running);
        //printf("running: %d!\n", still_running);
        break;
      }
    } while(still_running);

    int ct;
    CURLcode res;
    res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &ct);
    if ((CURLE_OK == res) && ct) {
        printf("We received HTTP_CODE: %d\n", ct);
    } else {
        printf("tq\n");
    }

    body = stream.str();


    curl_multi_cleanup(multi_handle);
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
 
    /* then cleanup the formpost chain */ 
    curl_formfree(formpost);
 
    /* free slist */ 
    curl_slist_free_all (headerlist);
  }
  bool parsingSuccessful = reader.parse( body, root );
  return root;
}

int main(void)
{
  cout << attendance("1", "1", "30", "photo.jpg") << endl;
  cout << attendance("2", "1", "30", "photo.jpg") << endl;
}
