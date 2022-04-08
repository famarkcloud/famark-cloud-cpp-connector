#include <iostream>
#include <stdio.h>
#include <string.h>
#define CURL_STATICLIB 
#include <curl/curl.h>
#include "WebRequest.h"
#include<json-c/json.h>

/* POST data for connect */
static char data[] = "{ \"ActionName\": \"Connect\", \"Input\": { \"EntityName\": \"Credential\", \"Fields\": [{ \"Key\": \"DomainName\", \"Value\": \"D1\"},{ \"Key\": \"UserName\", \"Value\": \"Anubhav\"},{ \"Key\": \"Password\", \"Value\": \"ErasPaas\"}] }}";

int main(int argc, char** argv)
{
    struct WriteThis wt;

    wt.readptr = data;
    wt.sizeleft = strlen(data);

    WebRequest w1;
    struct WriteThis rt = w1.perform(wt);
    
    struct json_object* parsed_json;
    struct json_object* ErrorMessage;
    struct json_object* Output;

   // std::cout << rt.readptr<<std::endl;
    parsed_json = json_tokener_parse(rt.readptr);
   
    json_object_object_get_ex(parsed_json, "ErrorMessage", &ErrorMessage);
    json_object_object_get_ex(parsed_json, "Output", &Output);
   
    if (ErrorMessage != nullptr) {
        printf("ErrorMessage: %s\n", json_object_get_string(ErrorMessage));
        free(rt.readptr);
        return 0;
    }

    std::string sessionId = json_object_get_string(Output);
    std::cout << "SessionId: " << sessionId;
  
    std::string data2 = "{ \"ActionName\": \"CreateRecord\", \"SessionId\": \"" + sessionId + "\", \"Input\": { \"EntityName\": \"Student\", \"Fields\": [{ \"Key\": \"Name\", \"Value\": \"Arunav\"},{ \"Key\": \"Roll\", \"Value\": \"17\"},{ \"Key\": \"Age\", \"Value\": \"35\"}] }}";
    int len2 = data2.length();
    char* data2Array = new char[len2 + 1];
    std::copy(data2.begin(), data2.end(), data2Array);
    data2Array[len2] = '\0';
    struct WriteThis wt2;

    wt2.readptr = data2Array;
    wt2.sizeleft = len2;

    WebRequest w2;
    struct WriteThis rt2 = w2.perform(wt2);

    parsed_json = json_tokener_parse(rt2.readptr);

    json_object_object_get_ex(parsed_json, "ErrorMessage", &ErrorMessage);
    json_object_object_get_ex(parsed_json, "Output", &Output);

    if (ErrorMessage != nullptr) {
        printf("ErrorMessage: %s\n", json_object_get_string(ErrorMessage));
        free(rt.readptr);
        return 0;
    }

    std::string recordId = json_object_get_string(Output);
    std::cout << "RecordId: " << recordId;

   // printf("%s\n", rt.readptr);
    free(rt.readptr);
    free(rt2.readptr);
    return 0;
}